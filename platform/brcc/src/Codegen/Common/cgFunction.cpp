/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
All rights reserved.


The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the
contents of this directory.

****************************************************************************/

#include "cgProgram.h"
#include "cgFunction.h"
#include "cgSemantic.h"
#include "cgBuiltin.h"

#include "logger.hpp"

#include "brtdecl.h"

//fixme:
#include "main.h"
#include "brtkernel.h"
#include "brtstemnt.h"
#include "brtreduce.h"
#include "brtexpress.h"

// ----------------------------------------------------------------------------
CgEdge::CgEdge(CgFunction *func)
{
    LOG(LOG_FN_ENTRY, "CgEdge::CgEdge(%p)\n", func);
    funcNode = func;
    nextEdge = NULL;
}

// ----------------------------------------------------------------------------
CgFunction::CgFunction(FunctionDef *pfDef, CgProgram* prog)
    : next(NULL), funcDef(pfDef), keepFuncDef(0), cgProgram(prog), numArg(0), 
      inEdge(NULL), outEdge(NULL),
      target(false), numScatterArray(0), isCurMain(false), hasInstanceInGroup(false)
      {
    LOG(LOG_FN_ENTRY, "CgFunction::CgFunction(%p, %p)\n", pfDef, prog);
    isReduce = pfDef->decl->isReduce();
}

CgFunction::~CgFunction()
{
    LOG(LOG_FN_ENTRY, "CgFunction::~CgFunction()\n");
    //free CgResourceParam
}

char*
CgFunction:: getTargetString()
{
    return "";
}

CgFunction * CgFunction::curCgFunc = NULL;
std::ostream * CgFunction::curShader = NULL;

// ----------------------------------------------------------------------------
//Query


std::string&
CgFunction::getFuncName()
{
    LOG(LOG_FN_ENTRY, "CgFunction::getFuncName()\n");
    return GetFunctionDefName(funcDef);
}

// ----------------------------------------------------------------------------
//Code gen

void CgFunction::cpuCodegen(std::ostream &outs, std::ostream &kernelDeclHeader)
{
    LOG(LOG_FN_ENTRY, "CgFunction::cpuCodegen(%p)\n", &outs);
    //irEdit_2(); //vout trans, etc

    //generate CPU inner kernel
    isMain = true; //temporary

    if ((globals.userTargets & TARGET_CPU)!=0)
        genCpuKernel(outs);
    else
    {
        outs << "static const void * __"
             << funcDef->decl->name->name
             << "_cpu" << " = NULL;" << std::endl;
    }

    if (isMain)
    {

        //generate kernel entry
        genKernelStub(outs, kernelDeclHeader);
    }

}

int getGatherStructureSamplerCount(StructDef * structure)
{
    LOG(LOG_FN_ENTRY, " getGatherStructureSamplerCount(%p)\n", structure);
    int result = 0;
    int fieldCount = structure->nComponents;

    for (int i = 0; i < fieldCount; i++)
    {
        Decl *fieldDecl = structure->components[i];

        if (fieldDecl->isStatic())
            continue;
        if (fieldDecl->isTypedef())
            continue;

        StructDef *subStructure = findStructureDef(fieldDecl->form);

        if (subStructure)
            result += getGatherStructureSamplerCount(subStructure);
        else
            result++;
    }
    return result;
}

int getGatherStructureSamplerCount(Type * form)
{
    LOG(LOG_FN_ENTRY, " getGatherStructureSamplerCount(%p)\n", form);
    StructDef *structure = findStructureDef(form);

    if (!structure)
        return 1;
    return getGatherStructureSamplerCount(structure);
}

static bool
genGatherStructureFunctionBody(std::ostream & out,
                               const std::string & name, StructDef * structure,
                               int &ioIndex)
{
    LOG(LOG_FN_ENTRY, "genGatherStructureFunctionBody(%p, %s, %p, %p)\n", &out, name.c_str(), structure, &ioIndex);
    int fieldCount = structure->nComponents;

    for (int i = 0; i < fieldCount; i++)
    {
        Decl *fieldDecl = structure->components[i];

        if (fieldDecl->isStatic())
            continue;
        if (fieldDecl->isTypedef())
            continue;

        std::string subName = name + "." + fieldDecl->name->name;

        Type *form = fieldDecl->form;
        StructDef *subStructure = findStructureDef(form);

        if (subStructure)
        {
            if (!genGatherStructureFunctionBody(out, subName, subStructure, ioIndex))
                return false;
        }
        else
        {
            out << "result" << subName << " = ";

            BaseType *base = form->getBase();
            out << "__fetch_";
            base->printBase(out, 0);
            out << "( ";
            out << "samplers[" << ioIndex++ << "], index );\n";
        }
    }

    return true;
}

static void
genGatherStructureFunction(std::ostream & out, const std::string & name, Type * form)
{
    LOG(LOG_FN_ENTRY, "genGatherStructureFunction(%p, %s, %p)\n", &out, name.c_str(), form);
    StructDef *structure = findStructureDef(form);

    if (!structure)
        return;

    std::stringstream s;
    int index = 0;

    if (!genGatherStructureFunctionBody(s, "", structure, index))
        return;

    std::string body = s.str();

    out << name << " __gather_" << name << "( _stype1 samplers[" << getGatherStructureSamplerCount(form);
    out << "], float index ) {\n";
    out << name << " result;\n";
    out << body;
    out << "\treturn result;\n}\n\n";

    out << name << " __gather_" << name << "( _stype2 samplers[" << getGatherStructureSamplerCount(form);
    out << "], float2 index ) {\n";
    out << name << " result;\n";
    out << body;
    out << "\treturn result;\n}\n\n";

    out << name << " __gather_" << name << "( _stype3 samplers[" << getGatherStructureSamplerCount(form);
    out << "], float3 index ) {\n";
    out << name << " result;\n";
    out << body;
    out << "\treturn result;\n}\n\n";
}

static void
genTypedef(std::ostream & out, DeclStemnt *inStmt)
{
    LOG(LOG_FN_ENTRY, "genTypedef(%p, %p)\n", &out, inStmt);
    for (DeclVector::iterator i = inStmt->decls.begin(); i != inStmt->decls.end(); ++i)
    {
        Decl *decl = *i;
        Type *form = decl->form;
        if (decl->isTypedef() && form->isBaseType())
        {
            //fixme
#if  0 //work around the fact that amd-hlsl doesn't support typedef
            out << "typedef ";
            form->printBase(out, 0);
            out << " " << decl->name->name;
            out << ";";
            out << "\n";
#else
            BaseType *baseType = AsBaseType(form);
            if ((baseType->typemask & BT_Struct) && baseType->stDefn != NULL)
            {
                out << "struct ";
                out << decl->name->name << " {\n";

                for (int j = 0; j < baseType->stDefn->nComponents; j++)
                {
                    baseType->stDefn->components[j]->print(out, true, 1);
                    Decl *subdecl = baseType->stDefn->components[j]->next;
                    while (subdecl != NULL)
                    {
                        out << ", ";
                        subdecl->print(out, false, 1);
                        subdecl = subdecl->next;
                    }

                    out << ";\n";
                }
                out << "};\n";
            }
#endif

            genGatherStructureFunction(out, decl->name->name, decl->form);
        }
    }
}

// ----------------------------------------------------------------------------

void
CgFunction::genCpuKernel(std::ostream &outs)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genCpuKernel(%p)\n", &outs);
    //fix me: re-implement this ...
    BRTCPUKernelCode cpukernel(*funcDef);

    if (isMain)
    {
        if(funcDef->getShaderType() == ComputeShader && funcDef->getLocalDataShareSize() != 0)
        {
            cpukernel.printMultiThreadedCode(outs);
        }
        else
        {
            cpukernel.printCode(outs);
        }
    }
    else
        cpukernel.printInnerCode(outs);

}

void
CgFunction::codegen(std::ostream &outs)
{
    LOG(LOG_FN_ENTRY, "CgFunction::codegen(%p)\n", &outs);
}

void
CgFunction::genKernelStub(std::ostream &outs, std::ostream &kernelDeclHeader)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genKernelStub(%p)\n", &outs);
    //generate kernel entry
    //fix me: re-implement this ...
    BRTKernelDef kerneldef(*funcDef);
    kerneldef.printStub(outs, kernelDeclHeader);
}

void
CgFunction::genPassGlobal(std::ostream &outs, char* linePrefix)
{
    CgResourceParamIter iter;
    for (iter = paramResource.begin(); iter!=paramResource.end(); iter++)
    {
        if ((*iter)->isScatterArray())
            (*iter)->genGlobal(outs, linePrefix);
    }
}

void
CgFunction::genPassLocal(std::ostream &outs, bool outaddr, bool inaddr, int reduceFactor, char* linePrefix, OutputStreamType outType)
{
}

void
CgFunction::genPassIndexofAssign(std::ostream &outs,
                                 bool outaddr,
                                 bool inaddr,
                                 int reduceFactor,
                                 char* linePrefix,
                                 int firstOutput,
                                 int lastOutput)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genPassIndexofAssign(%p, %d, %d, %s, %d, %d)\n", &outs, outaddr, inaddr, linePrefix, firstOutput, lastOutput);
    if (outaddr)
    {
        if(getShaderType() == ComputeShader)
        {
            outs << linePrefix << CgResourceParam::getIndexofOutputName() << " = "
                 << "__getOutputIndex(__threadInfo.atid, __outputStreamShape, __outputBufferShape);" << std::endl;

        }
        else
        {
            outs << linePrefix << CgResourceParam::getIndexofOutputName() << " = "
                 << "__getOutputIndex(__vPos, __outputStreamShape, __outputBufferShape);" << std::endl;
        }
    }

    for (int i = 0; i < numArg; i++)
    {
        CgResourceParam *param = paramResource[i];
        //if (param->hasIndexof())
        param->genIndexofAssign(outs, outaddr, inaddr, reduceFactor, linePrefix, firstOutput, lastOutput);

    }
}

void
CgFunction::genPassConstAndOutput(std::ostream &outs,
                            char* linePrefix,
                            CgPass &pass,
                            CgKernel *cgkernel, OutputStreamType outType)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genPassConstAndOutput(%p, %s, %p, %p)\n", &outs, linePrefix, &pass, cgkernel);
    CgResourceParamIter iter;
    bool hasOut = false;
    bool curOut = false;

    std::ostringstream declareConsts;
    unsigned int constCount = 0;

    std::ostringstream declareOutputStreams;
    unsigned int outputStreamCount = 0;

    unsigned int constBufferCount = 0;
    
    int outputArgIndex = 0;
    bool gotOutIndex = false;
    //The extra vpos for reduction is associated with the FIRST stream (input or output stream),
    //but allocated before the second (input or output) stream and after the first
    //(input or output) stream. Changing this requires corresponding change in the runtime.
    for (iter = paramResource.begin(); iter!=paramResource.end(); iter++)
    {
        CgResourceParam *argResource = (*iter);

        if((argResource->isFetchStream() && !argResource->isReduceOutput()) || argResource->isGatherArray())
        {
            
            if(argResource->isGatherArray() && argResource->isResourceTypeConstBuffer())
            {
                //! Declare constant buffer for gather array
                //! Convert n dimension array declaration to 1D array declartion
                const Decl* dclType = static_cast< const Decl* > (argResource->getDeclaration());
                ArrayType* arrayType = static_cast< ArrayType* >(dclType->form);
                outs << "cbuffer __cb" << constBufferCount << "\n";
                outs << "{" << std::endl;
                arrayType->printBase(outs, 0);
                outs << " " << *dclType->name;
                outs << "[" << arrayType->getTotalElementCount() << "];" << std::endl;
                outs << "};" << std::endl;
                constBufferCount++;
            }
            else
            {
                // Address translation constants required only for gather arrays
                declareConsts << linePrefix;
                bool isThere = argResource->genDeclareConst(declareConsts, linePrefix, pass, cgkernel);
                if(isThere)
                {
                    declareConsts << ";" << std::endl;
                    constCount++;
                }
            }
        }
        else if(argResource->isOutput())
        {
            if(outType == MIXED_OUTPUT_STREAM)
            {
                declareOutputStreams << linePrefix;
                bool isDeclared = argResource->genDeclare(declareOutputStreams, linePrefix, pass, cgkernel, ";");
                if(isDeclared)
                {
                    declareOutputStreams << ";" << std::endl;
                }
                outputStreamCount++;
                
                //! get the first output index
                //! This index used for AT constant while generating kernel descriptor
                if(gotOutIndex == false)
                {
                    outputArgIndex = argResource->getArgIndex();
                    gotOutIndex = true;
                }
            }
            else
            {
                if(getNumScatterArray() <= 0)
                {
                    declareOutputStreams << linePrefix;
                    bool isDeclared = argResource->genDeclare(declareOutputStreams, linePrefix, pass, cgkernel, ";");
                    if(isDeclared)
                    {
                        declareOutputStreams << ";" << std::endl;
                    }
                    outputStreamCount++;
                    
                    //! get the first output index
                    //! This index used for AT constant while generating kernel descriptor
                    if(gotOutIndex == false)
                    {
                        outputArgIndex = argResource->getArgIndex();
                        gotOutIndex = true;
                    }
                }
                else
                {
                    std::ostringstream temp;
                    argResource->genDeclare(temp, linePrefix, pass, cgkernel, ";");

                    //! get the first output index
                    //! This index used for AT constant while generating kernel descriptor
                    if(gotOutIndex == false)
                    {
                        outputArgIndex = argResource->getArgIndex();
                        gotOutIndex = true;
                    }
                }
            }
        }
        else
        {
            declareConsts << linePrefix;
            argResource->genDeclare(declareConsts, linePrefix, pass, cgkernel, ";");
            declareConsts << ";" << std::endl;
            constCount++;
        }
    }

    CgResourceIter iter2;
    for (iter2 = otherResource.begin(); iter2 != otherResource.end(); iter2++)
    {
        declareConsts << linePrefix;
        (*iter2)->genDeclare(declareConsts, linePrefix, pass, cgkernel, outputArgIndex);
        declareConsts << ";" << std::endl;
        constCount++;
    }

    if(constCount)
    {
        //outs << "\n//constants declarations" << std::endl;
        outs << "cbuffer __cb" << constBufferCount << "\n";
        outs << "{" << std::endl;
        outs << declareConsts.str() << std::endl;
        outs << "};" << std::endl;
    }

    if(outputStreamCount)
    {
        //outs << "\n//output structure" << std::endl;
        outs << "struct __output \n";
        outs << "{" << std::endl;
        outs << declareOutputStreams.str() << std::endl;
        outs << "};" << std::endl;
    }
}

void
CgFunction::genPassInputs(std::ostream &outs, char* linePrefix, CgPass &pass, CgKernel *cgkernel)
{
    std::ostringstream declareInputStreams;
    unsigned int inputStreamCount = 0;
    CgResourceParamIter iter;

    for (iter = paramResource.begin(); iter!=paramResource.end(); iter++)
    {
        CgResourceParam *argResource = (*iter);

        if((argResource->isFetchStream() && !argResource->isReduceOutput())|| argResource->isGatherArray())
        {
            if(argResource->isResourceTypeConstBuffer() == false)
            {
                declareInputStreams << linePrefix;
                argResource->genDeclare(declareInputStreams, "", pass, cgkernel, ",");
                declareInputStreams << "," << std::endl;
                inputStreamCount++;
            }
            else
            {
                std::ostringstream temp;
                argResource->genDeclare(temp, "", pass, cgkernel, ",");
            }
        }
    }

    if(inputStreamCount)
    {
        outs << declareInputStreams.str();
    }
}

void
CgFunction::genPassStreamfetch(std::ostream &outs, bool outaddr, bool inaddr, int reduceFactor, char* linePrefix)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genPassStreamfetch(%p, %s)\n", &outs, linePrefix);
    //FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    //Decl **allArg = funcType->args;

    for (int i = 0; i < numArg; i++)
    {
        CgResourceParam *param = paramResource[i];
        std::string tmpStr;
        if(getShaderType() == ComputeShader)
        {
            tmpStr = "float2(__threadInfo.atid, 0.0f)";
        }
        else
        {
            tmpStr = VPOS_PREFIX;
        }


        // Scatter fetch stream
        if(inaddr && param->isFetchStream())
        {
            tmpStr = "__getGatherPosFrom3D(" + param->getIndexofOutputName() + ", "
                        + param->getGatherStreamShapeName() + ", " 
                        + param->getGatherBufferShapeName() + ")";
        }

        if (param->isGatherArray() == false && param->isScatterArray() == false)
        {
            param->genFetch(outs, outaddr, inaddr, reduceFactor, linePrefix, tmpStr);
        }
    }
}

void
CgFunction::genPassCallmain(std::ostream &outs, char* linePrefix)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genPassCallmain(%p, %s)\n", &outs, linePrefix);
}

void
CgFunction::genPassStreamstore(std::ostream &outs, char* linePrefix,
                               int firstoutput, int lastoutput, OutputStreamType outType)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genPassStreamstore(%p, %s, %d, %d)\n", &outs, linePrefix, firstoutput, lastoutput);
    FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    Decl **allArg = funcType->args;

    bool returnOutput = false;
    for (int i = 0; i < numArg; i++)
    {
        Decl *arg = allArg[i];
        TypeQual qual = arg->form->getQualifiers();

        if ((qual & TQ_Out)!=0 || (qual & TQ_Reduce)!=0) //?? || (qual & TQ_Reduce) != 0)
        {
            CgResourceParam *param = paramResource[i];
            param->genStore(outs, linePrefix, firstoutput, lastoutput);
            if(arg->form->isArray() && outType == SCATTER_OUTPUT_STREAM)
            {
                returnOutput = false;
                break;
            }
            returnOutput = true;
        }
    }

    if(returnOutput || outType == MIXED_OUTPUT_STREAM)
        outs << "\n\treturn __o;" << std::endl;
}

struct genPassSubroutineData
{
    genPassSubroutineData(std::ostream &s, CgKernel *k)
        :shader(s), kerne(k)
        {}
    std::ostream &shader;
    CgKernel* kerne;
};

void impgenPassSubroutine(CgFunction *func, void *vdata)
{
    LOG(LOG_FN_ENTRY, "impgenPassSubroutine(%p, %p)\n", func, vdata);
    struct genPassSubroutineData *pdata = (struct genPassSubroutineData*)vdata;
    func->genAsSupport(pdata->shader, pdata->kerne);
}

void CgFunction::genPassSubroutine(std::ostream &shader, CgKernel *cgkernel)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genPassSubroutine(%p, %p)\n", &shader, cgkernel);
    //todo: gen only those that are needed
    struct genPassSubroutineData data(shader, cgkernel);
    cgProgram->applyCgFunction(impgenPassSubroutine, &data);

}

void
CgFunction::impgenPassTypedef(Statement *stmt)
{
    LOG(LOG_FN_ENTRY, "CgFunction::impgenPassTypedef(%p)\n", stmt);
    if (stmt->isDeclaration()) //|| stmt->isTypedef()
    {
        genTypedef(*curShader, AsDeclaration(stmt));
    }
}

void
CgFunction::genPassTypedef(std::ostream &shader)
{
    curShader = &shader;
    LOG(LOG_FN_ENTRY, "CgFunction::genPassTypedef(%p)\n", &shader);
    cgProgram->getTransUnit()->findStemnt(impgenPassTypedef);
}

void CgFunction::genAsSupport(std::ostream &shader, CgKernel* cgkernel)
{
    LOG(LOG_FN_ENTRY, "CgFunction::genAsSupport(%p, %p)\n", &shader, cgkernel);
}

// ----------------------------------------------------------------------------
//todo: add parameter to findExpr to avoid using static
static CgFunction *traverseCgFunc;
static bool traverseIsAddrTrans;

static Expression*
convertGatherExprCallExpr(Expression* expr)
{
    LOG(LOG_FN_ENTRY, "convertGatherExpr(%p)\n", expr);

    if (expr->etype == ET_IndexExpr)
    {
        IndexExpr *indexExpr = AsIndexExpr(expr);
        Expression* arrayExpr = indexExpr->array;
        while (arrayExpr->etype == ET_BinaryExpr &&
               (static_cast<BinaryExpr*>(arrayExpr))->op() == BO_Member)
            arrayExpr = (AsBinaryExpr(arrayExpr))->leftExpr();

        if (arrayExpr->etype == ET_Variable)
        {
            Variable* arrayVar = static_cast<Variable*>(arrayExpr);
            if (arrayVar->name && arrayVar->name->entry)
            {
                if (arrayVar->name->entry->IsParamDecl())
                {
                    if (arrayVar->name->entry->uVarDecl &&
                        arrayVar->name->entry->uVarDecl->form &&
                        arrayVar->name->entry->uVarDecl->form->type == TT_Array)
                    {
                        if (isScatterType(arrayVar->name->entry->uVarDecl->form))
                        {
                            //make indexExpr int
                            if (isABaseType(indexExpr->getType(), BT_Int) == false)
                            {
                                //add cast to int
                                BaseType *intType = new BaseType(BT_Int);
                                CastExpr *castExpr = new CastExpr(intType, indexExpr, expr->location);
                                indexExpr->replace(castExpr);
                            }
                        }
                        else
                        {
                            //fix me: use CgGatherExpr
                            indexExpr->_subscript->findExpr(convertGatherExprCallExpr);
                            BrtGatherExpr *gatherExpr = new BrtGatherExpr(indexExpr);
                            expr->replace(gatherExpr);
                            expr = gatherExpr;
                        }
                    }
                }
            }
        }
    }
    else if (expr->etype == ET_FunctionCall)
    {
        FunctionCall *fcall = AsFunctionCall(expr);
        if (fcall->function->etype == ET_Variable)
        {
            Variable *funcVar = AsVariable(fcall->function);
            CgFunction *calleeFunc = traverseCgFunc->getCgProgram()
                ->getCgFunction(funcVar->name->name);
            if (calleeFunc)
            {
                for (int i = 0; i < calleeFunc->getResourceTotal(); i++)
                {
                    //propagate the indexof attri from callee to caller
                    CgResourceParam *param = calleeFunc->getResourceParam(i);
                    if (param->hasIndexof())
                    {
                        //find the ith actual
                        Expression *actExp = getActualArg(fcall, i);
                        if (actExp->etype == ET_Variable)
                        {
                            Symbol *indexofSym = new Symbol;
                            indexofSym->name = param->getIndexofName();
                            indexofSym->entry = traverseCgFunc->findFormal(indexofSym->name);
                            assert(indexofSym->entry != NULL);
                            Variable *indexofVar = new Variable(indexofSym,(AsVariable(actExp))->location);
                            fcall->addArg(indexofVar);
                        }
                    }
                }
            }
        }
    }

    return expr;
}

static Expression *
convertCallWithGather(Expression* expr)
{
    /* Check function calls inside of kernels */
    if (expr->etype != ET_FunctionCall)
        return expr;

    // convert gathers that are passed into function call
    FunctionCall *fc = AsFunctionCall(expr);
    int nargs = fc->nArgs();
    for (int i = 0; i < nargs; ++i)
    {
        if (fc->args[i]->etype == ET_Variable)
        {
            Variable *argVar = AsVariable(fc->args[i]);
            CgResourceParam *argResource = traverseCgFunc->findResource(argVar->name->name);

            if (argResource == NULL || argResource->isGatherArray() == false)
                continue;
             
            if (traverseIsAddrTrans)
            {
                Symbol *sym = new Symbol;
                sym->name = argResource->getGatherStreamShapeName();
                sym->entry = traverseCgFunc->findFormal(sym->name);
                assert(sym->entry != NULL);
                Variable *var = new Variable(sym, argVar->location);
                fc->addArg(var);

                sym = new Symbol;
                sym->name = argResource->getGatherBufferShapeName();
                sym->entry = traverseCgFunc->findFormal(sym->name);
                assert(sym->entry != NULL);
                var = new Variable(sym, argVar->location);
                fc->addArg(var);
            }
        }
    }

    return expr;
}

CgResourceParam *
CgFunction::findResource(std::string &parName)
{
    CgResourceParamIter iter;

    for (iter = paramResource.begin(); iter!=paramResource.end(); iter++)
    {
        if ((*iter)->getName() == parName)
            return (*iter);
    }

    return NULL;
}

SymEntry *
CgFunction::findFormal(std::string &parName) const
{
    FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    Decl **allArg = funcType->args;
    int nArg = funcType->nArgs;

    for (int i = 0; i < nArg; i++)
    {
        Decl *arg = allArg[i];
        if (arg->name->name == parName)
            return arg->name->entry;
    }

    return NULL;
}

// ----------------------------------------------------------------------------

void
CgFunction::createResource(void)
{
    LOG(LOG_FN_ENTRY, "CgFunction::createResource()\n");
    //add sampler/vpos/const resource
    FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    Decl **allArg = funcType->args;
    int nArg = funcType->nArgs;

    numArg = nArg;

    paramResource.clear();

    for (int i = 0; i < nArg; i++)
    {
        Decl *arg = allArg[i];
        CgResourceParam * argResource = new CgResourceParam(arg, i+1);

        if (isReduce)
        {
            if (arg->isReduce())
                argResource->setReduceOutput();
            else if (arg->isStream())
                argResource->setReduceInput();
        }

        if (argResource->isScatterArray())
            numScatterArray ++;

        paramResource.push_back(argResource);
    }
}

static void
addArg(FunctionType *funcType, Symbol *sym, BaseType *type)
{
    LOG(LOG_FN_ENTRY, "addArg(%p, %p, %p)\n", funcType, sym, type);
    Decl *decl = new Decl(sym);
    sym->entry = mk_paramdecl(sym->name, decl);
    decl->form = type;
    funcType->addArg(decl);
}

void
CgFunction::irEdit_5(bool isAddrTrans)
{

    //funcDef->decl->form = keepFuncType->dup0();
    if (keepFuncDef == NULL)
        return;

    funcDef = AsFunctionDef(keepFuncDef->dup0());
    FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    //Decl **allArg = funcType->args;

    for (int i = 0; i < numArg; i++)
    {
        //Decl *arg = allArg[i];

        CgResourceParam * argResource = paramResource[i];
        
        //! Add address translation constants of gather array to
        //! subroutine where actual position calculated for gather array
        if (argResource->isGatherArray())
        {
            //! Added constants if Texture used for gather array
            if(argResource->isResourceTypeConstBuffer() == false)
            {
                if (isAddrTrans)
                {
                    Symbol *sym = new Symbol;
                    sym->name = argResource->getGatherStreamShapeName();
                    BaseType *type = new BaseType(BT_Int4);
                    type->qualifier |= TQ_GeneratedGatherConst;
                    addArg(funcType, sym, type);

                    sym = new Symbol;
                    sym->name = argResource->getGatherBufferShapeName();
                    type = new BaseType(BT_Int4);
                    type->qualifier |= TQ_GeneratedGatherConst;
                    addArg(funcType, sym, type);
                }
            }
        }
    }

    traverseIsAddrTrans = isAddrTrans;
    traverseCgFunc = this;
    // add parameter to calls with gather array as parameter
    funcDef->findExpr(convertCallWithGather);
}

void
CgFunction::irEdit_3()
{
    LOG(LOG_FN_ENTRY, "CgFunction::irEdit_3()\n");
    FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    //Decl **allArg = funcType->args;
    bool hasGatherArray = false;

    //! Add argument to get the indexof operator value
    for (int i = 0; i < numArg; i++)
    {
        //Decl *arg = allArg[i];
        CgResourceParam * argResource = paramResource[i];
        //argResource->allocate(resourceManager);

        //add indexof as extra parameter
        if (argResource->hasIndexof())
        {
            Symbol *indexofSym = new Symbol;
            indexofSym->name = argResource->getIndexofName();
            BaseType *indexofType = new BaseType(BT_Int4);
            indexofType->qualifier |= TQ_GeneratedIndexOf;
            indexofType->qualifier |= TQ_Const;
            addArg(funcType, indexofSym, indexofType);
        }
        if (argResource->isGatherArray())
            hasGatherArray = true;
    }

    //! Add arguments to kernel for instanceInGroup

    if(hasInstanceInGroup)
    {
        Symbol *instanceInGroup = new Symbol;
        instanceInGroup->name = "__instanceInGroup";
        BaseType *instanceInGroupType = new BaseType(BT_Int);
        instanceInGroupType->qualifier |= TQ_Const;
        addArg(funcType, instanceInGroup, instanceInGroupType);
    }

    //replace ET_IndexExpr with cgGatherExpr so print method can be used to output ...
    traverseCgFunc = this;
#ifdef DISABLE_C_STYLE_INDEXING //! GEN: Now no need to change IndexExpr to GatherExpr for printing purpose
      //!      Still we need to remove related code
    funcDef->findExpr(convertGatherExprCallExpr);
#endif 
    if (hasGatherArray)
    {       
        keepFuncDef = funcDef;
    }
    else
    {
        keepFuncDef = NULL;
    }

}

void
CgFunction::allocateResource(CgResourceManager &resourceManager,
                             int factor, bool outaddr, bool inaddr)
{
    LOG(LOG_FN_ENTRY, "CgFunction::allocateResource(%p, %d, %d, %d)\n", &resourceManager, factor, outaddr, inaddr);
}
// ----------------------------------------------------------------------------

void
CgFunction::irEdit_4()
{
    LOG(LOG_FN_ENTRY, "CgFunction::irEdit_4()\n");
    //replace call to user define subroutine to cgCallExp
}

// ----------------------------------------------------------------------------
//return 0 when a change is made, otherwise 1 to indicated a problem
int
CgFunction::setIndexof(Symbol *formalArg)
{
    LOG(LOG_FN_ENTRY, "CgFunction::setIndexof(%p)\n", formalArg);
    int index = findArgPos(AsFunctionType(funcDef->decl->form), formalArg);
    
    // -1 Indicates there is no such argument
    if(index == -1)
    {
        return 1;
    }
    CgResourceParam * argResource = paramResource[index];
    return argResource->setIndexof();
}

// ----------------------------------------------------------------------------
//return 0 when a change is made, otherwise 1 to indicated a problem
int
CgFunction::setInstance(Variable* formalArg)
{
    LOG(LOG_FN_ENTRY, "CgFunction::setInstance()\n");
    int index = findFirstOutputPos(AsFunctionType(funcDef->decl->form));
    
    // -1 Indicates there is no such argument
    if(index == -1)
    {
        return 1;
    }

    CgResourceParam * argResource = paramResource[index];

    // Set the symbol and its type same as first output
    formalArg->name = AsFunctionType(funcDef->decl->form)->args[index]->name->dup();
    if (formalArg->name->entry && formalArg->name->entry->uVarDecl)
    {
        formalArg->type = formalArg->name->entry->uVarDecl->form;
    }

    return argResource->setIndexof();
}

int CgFunction::propagateIndexof(FunctionCall *fcall)
{
    LOG(LOG_FN_ENTRY, "CgFunction::propagateIndexof(%p)\n", fcall);
    int numChange = 0;
    Expression *calleeExp = fcall->function;
    if (calleeExp->etype == ET_Variable)
    {
        CgFunction *calleeFunc = cgProgram->getCgFunction(AsVariable(calleeExp)->name->name);
        if (calleeFunc)
        {
            for (int i = 0; i < AsInt(calleeFunc->paramResource.size()); i++)
            {
                //propagate the indexof attri from callee to caller
                CgResourceParam *param = calleeFunc->paramResource[i];
                if (param->hasIndexof())
                {
                    //find the ith actual
                    Expression *actExp = getActualArg(fcall, i);
                    if (actExp->etype == ET_Variable)
                    {
                        numChange += setIndexof(AsVariable(actExp)->name);
                    }
                    else
                    {
                        //problem
                    }
                }
            }
        }
    }
    else
    {
        //indirect call not support
    }

    return numChange;
}

static int numIndexof;
Expression *
CgFunction::impCollectIndexof(Expression *expr)
{
    LOG(LOG_FN_ENTRY, "CgFunction::impCollectIndexof(%p)\n", expr);
    if (expr->etype == ET_BrtIndexofExpr)
    {
        Variable *var = AsBrtIndexofExpr(expr)->expr;
        numIndexof += curCgFunc->setIndexof(var->name);
    }
    else if(expr->etype == ET_BrtInstanceExpr)
    {
        Variable *var = static_cast<BrtInstanceExpr*>(expr)->expr;
        numIndexof += curCgFunc->setInstance(var);
    }
    else if (expr->etype == ET_FunctionCall)
    {
        numIndexof += curCgFunc->propagateIndexof(AsFunctionCall(expr));
    }
    else if(expr->etype == ET_BrtInstanceInGroupExpr)
    {
        curCgFunc->setInstanceInGroup(true);
    }

    return expr;
}

int
CgFunction::collectIndexof(void)
{
    LOG(LOG_FN_ENTRY, "CgFunction::collectIndexof()\n");
    numIndexof = 0;
    curCgFunc = this;
    funcDef->findExpr(impCollectIndexof);
    return numIndexof;
}

// ----------------------------------------------------------------------------

int
CgFunction::semanticCheck(void)
{
    LOG(LOG_FN_ENTRY, "CgFunction::semanticCheck()\n");
    return 0;
}

// ----------------------------------------------------------------------------
//call graph related

int
CgFunction::addCallee(Variable *var)
{
    LOG(LOG_FN_ENTRY, "CgFunction::addCallee(%p)\n", var);
    CgFunction *calleeCgFunc = cgProgram->getCgFunction(var->name->name);
    if (calleeCgFunc == NULL)
    {
        //check against built in table
        //return 1 if not found;
    }
    else
    {
        addOutEdge(calleeCgFunc);
        calleeCgFunc->addInEdge(this);
    }
    return 0;
}

int
CgFunction::addInEdge(CgFunction *func)
{
    LOG(LOG_FN_ENTRY, "CgFunction::addInEdge(%p)\n", func);
    return addEdge(&inEdge, func);
}

int
CgFunction::addOutEdge(CgFunction *func)
{
    LOG(LOG_FN_ENTRY, "CgFunction::addOutEdge(%p)\n", func);
    return addEdge(&outEdge, func);
}

int
CgFunction::addEdge(CgEdge **head, CgFunction *func)
{
    LOG(LOG_FN_ENTRY, "CgFunction::addEdge(%p, %p)\n", head, func);
    CgEdge *cur = *head;
    CgEdge *pre = NULL;
    while (cur)
    {
        if (cur->funcNode == func)
            break;
        pre = cur;
        cur = cur->nextEdge;
    }
    if (cur)
    {
        //duplicate
        return 1;
    }
    else
    {
        CgEdge *newedge =  new CgEdge(func);
        if (pre == NULL)
            *head = newedge;
        else
            pre->nextEdge = newedge;
    }

    return 0;
}

Expression*
CgFunction::impAddEdge(Expression *exp)
{
    LOG(LOG_FN_ENTRY, "CgFunction::impAddEdge(%p)\n", exp);
    if (exp->etype == ET_FunctionCall)
    {
        FunctionCall *fcall = AsFunctionCall(exp);
        Expression *callee = fcall->function;
        if (callee->etype == ET_Variable)
        {
            curCgFunc->addCallee(AsVariable(callee));
        }
        else
        {
            //indirect call not support
        }
    }

    return exp;
}


void CgFunction::addEdge()
{
    LOG(LOG_FN_ENTRY, "CgFunction::addEdge()\n");
    curCgFunc = this;
    funcDef->findExpr(impAddEdge);
    curCgFunc = NULL;
}

// ----------------------------------------------------------------------------

CgResourceParam *
CgFunction::getReduceoutStream()
{
    LOG(LOG_FN_ENTRY, "CgFunction::getReduceinStream()\n");
    for (int i = 0; i < numArg; i++)
    {
        CgResourceParam *param = paramResource[i];
        if (param->isReduceOutput())
            return param;
    }
    return NULL;
}

CgResourceParam *
CgFunction::getReduceinStream()
{
    LOG(LOG_FN_ENTRY, "CgFunction::getReduceinStream()\n");
    for (int i = 0; i < numArg; i++)
    {
        CgResourceParam *param = paramResource[i];
        if (param->isReduceInput())
            return param;
    }
    return NULL;
}

// ----------------------------------------------------------------------------

