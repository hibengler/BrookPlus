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
#include "cgFunctionIL.h"

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
//impgenPassTypedef
//copied from codegen.cpp

int getGatherRank(Type * form)
{
    LOG(LOG_FN_ENTRY, " getGatherRank(%p)\n", form);
    Type *f = form;
    int rank = 0;

    while (f->isArray())
    {
        rank++;
        f = ((ArrayType *) f)->subType;
    }
    return rank;
}

// ----------------------------------------------------------------------------

CgProgram::CgProgram(TransUnit *tu)
    : transUnit(tu), firstFunc(NULL), lastFunc(NULL)
{
    LOG(LOG_FN_ENTRY, "CgProgram::CgProgram(%p)\n", tu);
    //program = tu;
}

CgProgram::~CgProgram()
{
    LOG(LOG_FN_ENTRY, "CgProgram::~CgProgram()\n");
    //free CgFunction
}

void
CgProgram::applyCgFunction(fnCgFunctionCallback func, void *data)
{
    LOG(LOG_FN_ENTRY, "CgProgram::applyCgFunction(%p, %p)\n", func, data);
    CgFunction *curFunc = firstFunc;
    while (curFunc != NULL)
    {
        (func)(curFunc, data);
        curFunc = curFunc->next;
    }
}

static void
impIrEdit_3(CgFunction *func, void *dummy)
{
    LOG(LOG_FN_ENTRY, "impIrEdit_3(%p, %p)\n", func, dummy);
    func->irEdit_3();
}

static void
impIrEdit_4(CgFunction *func, void *dummy)
{
    LOG(LOG_FN_ENTRY, "impIrEdit_4(%p, %p)\n");
    func->irEdit_4();
}

static void
impCalCodegen(CgFunction *func, void *data)
{
    LOG(LOG_FN_ENTRY, "impCalCodegen(%p, %p)\n", func, data);
    std::ostream &outs = *(std::ostream *) data;
    func->setTarget(semanticChecker.isTargetCalPass());
    func->codegen(outs);
}

void
CgProgram::codegen(std::ostream &cppout, std::ostream &kernelDeclHeader, std::ostringstream &nonKernelDecls, 
                   std::ostream &headerout, std::string &headername, std::string& kernelDeclFileName)
{
    LOG(LOG_FN_ENTRY, "CgProgram::codegen(%p, %p, %s)\n", &cppout, &headerout, headername.c_str());
    irEdit_1();

    //output header
    cppout << std::endl
           << "////////////////////////////////////////////" << std::endl
           << "// Generated by BRCC "<< globals.brccVersion << std::endl
           << "// BRCC Compiled on: " << __DATE__
           << " " << __TIME__ << std::endl
           << "////////////////////////////////////////////"
           << std::endl << std::endl;

    //if (globals.enableGPUAddressTranslation)
    //{
    //    cppout << "#define BROOK_ENABLE_ADDRESS_TRANSLATION 1" << std::endl;
    //}

    cppout << "#include \"brook/brook.h\"" << std::endl;
    cppout << "#include \"" << stripPath(headername) << "\"" << std::endl;
    cppout << "#include \"" << stripPath(kernelDeclFileName) << "\"" << std::endl;

    bool isStructThere = true;

    int inInclude = 0;
    Statement *stmt;
    for (stmt = transUnit->head; stmt; stmt = stmt->next)
    {
        if (inInclude > 0)
        {
            if (stmt->isEndInclude())
                inInclude--;
            else if (stmt->isInclude())
                inInclude++;
        }
        else
        {
            if (stmt->isInclude())
                inInclude++;

            if (stmt->isFuncDef()==false || AsFunctionDef(stmt)->decl->isKernel()==false)
            {
                if(stmt->isDeclaration() == true && AsDeclaration(stmt)->decls[0]->isTypedef())
                {
                    //! include brook.h only once if any structure declared  
                    if(isStructThere)
                    {
                        kernelDeclHeader << "#include \"brook/brook.h\"" << std::endl;
                        isStructThere = false;
                    }
                    kernelDeclHeader << *stmt;
                }
                else
                {
                    cppout << *stmt;
                }

                if(stmt->isFuncDef() == true)
                {
                    FunctionDef *funcDef = AsFunctionDef(stmt);
                    std::string entryPoint = "main";
                    //! Do not add main in declarations
                    if(entryPoint.compare(funcDef->decl->name->name))
                    {
                        funcDef->decl->print(nonKernelDecls, true);
                        nonKernelDecls << ";" << std::endl;
                    }
                }
            }
            else
            {
                CgFunction *cgFunc = getCgFunction(GetFunctionDefName(AsFunctionDef(stmt)));
                
                //! Set target to cpu
                globals.setPresentTarget(TARGET_CPU);
                cgFunc->cpuCodegen(cppout, kernelDeclHeader);
                //! Reset terget path to None
                globals.resetPresentTarget();
            }

            cppout << std::endl;
        }
    }

    applyCgFunction(impIrEdit_3, NULL);

    //! Set target to CAL
    

    globals.setPresentTarget(TARGET_CAL);
    addCgFunctionCAL();
    applyCgFunction(impCalCodegen, &headerout);
    globals.resetPresentTarget();
}

void
CgProgram::addCgFunctionCAL()
{
    // Replace cgFunc with CAL specific function
    CgFunction *oldFunc = firstFunc;
    CgFunction * prevFunc = oldFunc;
    while (oldFunc != NULL)
    {
        CgFunctionIL* func = new CgFunctionIL(oldFunc);
        func->createResource();
        if(oldFunc == firstFunc)
        {
            firstFunc = func;
            lastFunc = func;
        }
        else
        {
            lastFunc->next = func;
            lastFunc = func;
        }
        prevFunc = oldFunc;
        oldFunc = oldFunc->next;
        delete prevFunc;
    }
}

void
CgProgram::addCgFunction(CgFunction *cgFunc, std::string& funcName)
{
    LOG(LOG_FN_ENTRY, "CgProgram::addCgFunction(%p, %s)\n", cgFunc, funcName.c_str());
    if (getCgFunction(funcName) != NULL)
    {
        CGEXIT("***function redefined: %s\n", funcName.c_str());
    }

    cgFuncTbl[funcName] = cgFunc;

    if (firstFunc == NULL)
    {
        firstFunc = cgFunc;
        lastFunc = cgFunc;
    }
    else
    {
        lastFunc->next = cgFunc;
        lastFunc = cgFunc;
    }
}


CgFunction *
CgProgram::getCgFunction(std::string& funcName)
{
    LOG(LOG_FN_ENTRY, "CgProgram::getCgFunction(%s)\n", funcName.c_str());
    CgFuncIter iter = cgFuncTbl.find(funcName);
    if (iter != cgFuncTbl.end())
        return (*iter).second;

    return NULL;
}


// ----------------------------------------------------------------------------
// Implement irEdit_1

static void
convertNonkernelFunctionType(FunctionType * fType)
{
    LOG(LOG_FN_ENTRY, "convertNonkernelFunctionType(%p)\n", fType);
    for (int i = 0; i < AsInt(fType->nArgs); i++)
    {
        Type **paramTyp = &fType->args[i]->form;

        while (paramTyp)
        {
            if ((*paramTyp)->isStream() ||
                (fType->isKernel() && (*paramTyp)->isArray()))
            {
                Type *newForm;

                newForm = new BrtStreamParamType((ArrayType *) *paramTyp);

                /*
                 * Types are all on the global type list, so we can't just nuke it.
                 delete fType->args[i]->form;
                */
                (*paramTyp) = newForm;
            }
            paramTyp = (*paramTyp)->getSubType();
        }
    }

}

static void
convertVarType(Statement * s)
{
    LOG(LOG_FN_ENTRY, "convertVarType(%p)\n", s);
    if (!s->isDeclaration())
        return;

    DeclStemnt *declStemnt = (DeclStemnt *) s;

    for (int i = 0; i < AsInt(declStemnt->decls.size()); i++)
    {
        Decl *decl = declStemnt->decls[i];

        if (!decl->form)
            continue;

        if (decl->form->isFunction())
        {
            CGASSERT((decl->form->type == TT_Function), "IR problem");
            //this makes a assumption that it is a non kernel function
            convertNonkernelFunctionType((FunctionType *) decl->form);
            continue;
        }

        if (!decl->isStream())
            continue;

        ArrayType *stream = (ArrayType *) decl->form;
        Type *brtType = decl->form;

        if ((stream->getQualifiers() & TQ_Iter) != 0)
        {
            RecordSemanticError(decl, declStemnt, SemanticChecker::SCEBStreamIter);
        }
        else if (decl->initializer != NULL)
        {
            RecordSemanticError(decl, declStemnt, SemanticChecker::SCEHStreamInit);
        }
        else
            brtType = new BrtInitializedStreamType(stream);
#if 0
        if (decl->initializer == NULL)
        {
            //todo: add to semantic check ...
            CGASSERT(((stream->getQualifiers() & TQ_Iter) == 0), "iter must have an initializer ");
            brtType = new BrtInitializedStreamType(stream);
        }
        else
        {
            CGASSERT((decl->initializer->etype == ET_FunctionCall), "problem with iter initializer");
            CGASSERT(((stream->getQualifiers() & TQ_Iter) != 0), "stream variable can't have an initializer");
            brtType = new BrtIterType(stream, (FunctionCall *) decl->initializer);
            delete decl->initializer;

            decl->initializer = NULL;
        }
#endif
        decl->form = brtType;
    }
}

/**
 *    1. Convert the TYPE for stream declaration statement into BrtStream
 *    (removed . Convert the TYPE for kernel declaration statement into BrtKernelType)
 *    (todo: remove BrtKernelType)
 *    2. Convert the TYPE for stream formal into BrtStream
 *    retire this ...3. Convert FunctionDef into KernelDef with map/reduction attribute properly set
 */
void CgProgram::irEdit_1()
{
    LOG(LOG_FN_ENTRY, "CgProgram::irEdit_1()\n");
    Statement *ste, *prev;

    for (ste = transUnit->head, prev = NULL; ste; prev = ste, ste = ste->next)
    {
        ste->findStemnt(convertVarType);  //perform task 1 and 2

        // Function definition can only occur at the top level.
        if (ste->isFuncDef() && AsFunctionDef(ste)->decl->isKernel() == false)
        {
            FunctionDef *fDef = AsFunctionDef(ste);

            convertNonkernelFunctionType((FunctionType *) fDef->decl->form);


#if 0
            //delay this to avoid the updating of pointers to FunctionDef
            //in call graph etc
            if (fDef->decl->isKernel())
            {
                FunctionDef *newDef = new KernelDef(*fDef, fDef->decl->isReduce());

                (prev ? prev->next : head) = newDef;
                newDef->next = ste->next;
                ste->next = NULL;

                //todo:
                //fix the symbol table to replace fDef with newDef
                //delete fDef

                ste = newDef;
            }
#endif
        }
    }
}

// ----------------------------------------------------------------------------
static void
impCollectIndexof(CgFunction *func, void *data)
{
    LOG(LOG_FN_ENTRY, "impCollectIndexof(%p, %p)\n");
    int *pres = (int *)data;
    *pres = func->collectIndexof() + (*pres);
}

static void
impBuildCallgraph(CgFunction *func, void *dummy)
{
    LOG(LOG_FN_ENTRY, "impBuildCallgraph(%p, %p)\n", func, dummy);
    func->addEdge();
}

int
CgProgram::semanticCheck()
{
    LOG(LOG_FN_ENTRY, "CgProgram::semanticCheck()\n");
    semanticChecker.init();
    if ((globals.userTargets & TARGET_CAL)!=0)
        semanticChecker.setCheckTargetCal();
    if ((globals.userTargets & TARGET_PS30)!=0)
        semanticChecker.setCheckTargetDx9();

    //perform irEdit_1 here so that semanticCheck and codegen work on the same IR ...
    //perform part of the semantic check as well
    //irEdit_1();

    //semantic check
    Statement *stmt;
    for (stmt = transUnit->head; stmt; stmt = stmt->next)
    {


#ifdef SEMANTIC_CHECK
        stmt->semanticCheck(false);
#endif
        if (stmt->isFuncDef())
        {
            if (AsFunctionDef(stmt)->decl->isKernel())
            {
                //semanticCheckDeclare(fDef, true);
                //fDef->findExpr(semanticCheckKernelExpr);
                CgFunction *cgFunc = new CgFunction(AsFunctionDef(stmt), this);
                cgFunc->createResource();
                addCgFunction(cgFunc, GetFunctionDefName(AsFunctionDef(stmt)));
            }
            else
            {
#if 0
                RecordSemanticWarn(stmt, SemanticChecker::SCWNonKernel,
                    "It is recommended that non-kernel routines be placed outside .br file", SemanticChecker::SCWarningLevelOne);
#endif 
                //semanticCheckDeclare(fDef, false);
                //fDef->findExpr(semanticCheckHostExpr);
            }
        }
        //deal with other kinds of stmt here
    }

    //build call graph
    applyCgFunction(impBuildCallgraph, NULL);

    //! Collect indexof information if there is no sematic check error
    if(semanticChecker.getErrorTotal() == 0)
    {
        //fixme: we don't need iterate if we have topological sorted ...
        int change = 1;
        while (change)
        {
            change = 0;
            applyCgFunction(impCollectIndexof, &change);
        }
    }

    semanticChecker.fini();

    return semanticChecker.getErrorTotal();
}

// ----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  converts string to upper case string
//!
////////////////////////////////////////////////////////////////////////////////
void strToUpper(std::string& input)
{
    for (std::string::iterator i = input.begin(); i != input.end(); ++i)
    {
        *i = toupper(*i);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Initializes the kernel declaration header file by adding guards, 
//!         copy right and extern "C"
//!
////////////////////////////////////////////////////////////////////////////////
void initializeKernelDeclarationHeader(std::ofstream& out)
{
    //! Get position where actual file name starts
    //! ignore the path
    unsigned int fileNamePos = 0;
    for(unsigned int i = 0; globals.shaderOutputName[i] != '\0'; ++i)
    {
        char c = globals.shaderOutputName[i];
        if('\\' == c || '/' == c)
            fileNamePos = i;
    }
    fileNamePos = fileNamePos? fileNamePos + 1: fileNamePos;

    //! Get actual file Name
    std::string tempStr = std::string(&globals.shaderOutputName[fileNamePos]);
    strToUpper(tempStr);
    
    //! Add gaurd to header file to avoid multiple inclusion
    out << "#ifndef _" << tempStr.c_str() << "_AUTO_GENERATED_H_" << "\n"
        << "#define _" << tempStr.c_str() << "_AUTO_GENERATED_H_" << "\n" << std::endl;

    //! Add copy right
    out << "/**************************************************************************** \n"
        << "                                                                              \n"
        << "Copyright (c) 2003, Stanford University                                       \n"
        << "All rights reserved.                                                          \n"
        << "                                                                              \n"
        << "Copyright (c) 2008, Advanced Micro Devices, Inc.                              \n"
        << "All rights reserved.                                                          \n"
        << "                                                                              \n"
        << "                                                                              \n"
        << "The BRCC portion of BrookGPU is derived from the cTool project                \n"
        << "(http://ctool.sourceforge.net) and distributed under the GNU Public License.  \n"
        << "                                                                              \n"
        << "Additionally, see LICENSE.ctool for information on redistributing the         \n"
        << "contents of this directory.                                                   \n"
        << "                                                                              \n"
        << "****************************************************************************/ \n"
        << std::endl;

    out << "#include \"brook/Stream.h\" \n"
        << "#include \"brook/KernelInterface.h\" \n";

    out << "\n//! Kernel declarations" << std::endl;

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  finilizes the kernel declaration header file by adding closing of
//!         extern "C" and closing guard
//!
////////////////////////////////////////////////////////////////////////////////
void finalizeKernelDeclarationHeader(std::ofstream& out, std::ostringstream &nonKernelDecls)
{
    
    if(globals.enableExternCForNonKernelDecls)
    {
        out << "\n//! Non Kernel declarations" << std::endl;
        out << "#ifdef __cplusplus \n"
            << "extern \"C\" \n"
            << "{ \n"
            << "#endif" << std::endl;

        out << nonKernelDecls.str().c_str() << std::endl;

        out << "#ifdef __cplusplus \n"
            << "} \n"
            << "#endif \n"<< std::endl;
    }
    
    //! Get position where actual file name starts
    //! ignore the path
    unsigned int fileNamePos = 0;
    for(unsigned int i = 0; globals.shaderOutputName[i] != '\0'; ++i)
    {
        char c = globals.shaderOutputName[i];
        if('\\' == c || '/' == c)
            fileNamePos = i;
    }
    fileNamePos = fileNamePos? fileNamePos + 1: fileNamePos;

    //! Get actual file Name
    std::string tempStr = std::string(&globals.shaderOutputName[fileNamePos]);
    strToUpper(tempStr);
    
    //! close gaurd 
    out << "#endif "
        << "// _" << tempStr.c_str() << "_AUTO_GENERATED_H_" << "\n" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  delete the specified file
//!
////////////////////////////////////////////////////////////////////////////////
void deleteFile(std::string fileName)
{
    int commandErr = 0;
    
    // set the command line statement to remove folder
#ifdef _WIN32
    std::string commandStr = "del /F ";
#else
    std::string commandStr = "rm -rf ";
#endif 
    commandStr.append("\"");
    commandStr.append(fileName.c_str());
    commandStr.append("\"");

    fprintf(stderr, "\ndeleting file : %s\n", fileName.c_str());

    // execute command to clean all temp files
    commandErr = system(commandStr.c_str());
    if(commandErr)
        fprintf(stderr, "ERROR : Failed to execute command - %s\n", commandStr.c_str());
}
//new driver, newMain is just a temporary name ...
//
void newMain(TransUnit *tu)
{
    LOG(LOG_FN_ENTRY, "newMain(%p)\n", tu);
    if (tu)
    {
        //std::cout << *tu << std::endl;
        // create CgProgram object with TransUnit
        CgProgram cgProgram(tu);

        //Initialize fucntion property table for semantic check of
        //builtin fucntion
        builtinInit();
        
        std::string cppFileName(globals.cppOutputName);

        std::string kernelDeclFileName(globals.shaderOutputName);
        kernelDeclFileName += std::string(".h");

        std::string headername(globals.shaderOutputName);
        headername += std::string("_gpu.h");
        
        //! Delete generated cpp files if already exist
        FILE *tmpPtr = fopen(globals.cppOutputName, "r");
        if(tmpPtr)
        {
            fclose(tmpPtr);

            //! Delete generated cpp file file
            deleteFile(cppFileName);

            //! Delete generated .h file 
            deleteFile(kernelDeclFileName);

            //! Delete generated _gpu.h file 
            deleteFile(headername);
        }

        //! Semantic check 
        globals.enableSematicCheck = true;
        int semErrNum = cgProgram.semanticCheck();
        globals.enableSematicCheck = false;

        if (semErrNum > 0)
        {
            CGEXIT("***Semantic check found %d errors\n", semErrNum);
            exit(1);
        }

        std::ofstream cppout;
        cppout.open(globals.cppOutputName);
        if (cppout.fail())
        {
            CGEXIT("***Unable to open file %s \n", globals.cppOutputName);
        }

        //! Declare all kernel declarations and non kernel declarations in seperate header file
        std::ofstream kernelDeclarationHeader;

        //! To collect non kernel declarations in .br file and add at the end of all kernel declaration
        //! in kernelDeclarationHeader file
        std::ostringstream nonKernelDecls;

        kernelDeclarationHeader.open(kernelDeclFileName.c_str());
        if (kernelDeclarationHeader.fail())
        {
            CGEXIT("***Unable to open file %s \n", globals.cppOutputName);
        }
        //! initialize kernel declaration in hearder file
        initializeKernelDeclarationHeader(kernelDeclarationHeader);

        std::ofstream headerout;
        headerout.open(headername.c_str());
        if (cppout.fail())
        {
            CGEXIT("***Unable to open file %s\n", headername.c_str());
        }

        cgProgram.codegen(cppout, kernelDeclarationHeader, nonKernelDecls, headerout, headername, kernelDeclFileName);
        
        //! finalize kernel declaration in hearder file
        finalizeKernelDeclarationHeader(kernelDeclarationHeader, nonKernelDecls);

        //! close all files
        cppout.close();
        kernelDeclarationHeader.close();
        headerout.close();
        

        if (globals.codeGenFailed)
        {
            //! Delete generated cpp file file
            deleteFile(cppFileName);

            //! Delete generated .h file 
            deleteFile(kernelDeclFileName);

            //! Delete generated _gpu.h file 
            deleteFile(headername);

            CGEXIT("\n\n***Code generation found errors\n");
            exit(1);
        }

        //Uninitialize the fucntion property table
        builtinFinit();
    }
}
