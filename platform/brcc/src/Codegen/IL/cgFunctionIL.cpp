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

#include "cgFunctionIL.h"
#include "cgKernelIL.h"
#include "cgResourceIL.h"
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
CgFunctionIL::CgFunctionIL(CgFunction* base)
             : CgFunction(*base)
{
}

CgFunctionIL::~CgFunctionIL()
{
    LOG(LOG_FN_ENTRY, "CgFunctionIL::~CgFunctionIL()\n");
}

char*
CgFunctionIL::getTargetString()
{
    return "cal";
}

void
CgFunctionIL::codegen(std::ostream &outs)
{
    LOG(LOG_FN_ENTRY, "CgFunctionIL::codegen(%p)\n", &outs);

    std::string tmpstr = cgVarPrefix+getFuncName()+"_cal";
    CgKernelIL cgKernel(this, tmpstr, CAL_MRT);

    cgKernel.codegen(outs, target);
}

void
CgFunctionIL::genPassLocal(std::ostream &outs, bool outaddr, bool inaddr, int reduceFactor, char* linePrefix, OutputStreamType outType)
{
    LOG(LOG_FN_ENTRY, "CgFunctionIL::genPassLocal(%p, %d, %d, %s)\n", &outs, outaddr, inaddr, linePrefix);
    CgResourceParamIter iter;

    for (iter = paramResource.begin(); iter!=paramResource.end(); iter++)
    {
        if((*iter)->isOutput() && outType != SCATTER_OUTPUT_STREAM)
        {
            // all outputs are grouped into struct __output
            // declare __output variable if atleast one output stream is present
            outs << "\t__output __o;" << std::endl;
            break;
        }
    }

    for (iter = paramResource.begin(); iter!=paramResource.end(); iter++)
    {
        (*iter)->genLocal(outs, outaddr, inaddr, reduceFactor, linePrefix, getShaderType());
    }
    if (outaddr)
    {
        outs << linePrefix << "int4 " << CgResourceParam::getIndexofOutputName() << ";" << std::endl;
    }
}

void
CgFunctionIL::genPassCallmain(std::ostream &outs, char* linePrefix)
{
    LOG(LOG_FN_ENTRY, "CgFunctionIL::genPassCallmain(%p, %s)\n", &outs, linePrefix);
    outs << linePrefix << getFuncName() << "(";

    FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    Decl **allArg = funcType->args;
    int nArg = funcType->nArgs;
    bool isfirst = true;
    for (int i = 0; i < nArg; i++)
    {
        Decl *arg = allArg[i];

        if (isScatterType(arg->form))
            continue;

        //! Skip samplers for gather arrays which using constant buffer as resource
        if(arg->isArray())
        {
            ArrayType* arrayType = AsArrayType(arg->form);
            if(arrayType->resourceType == ConstBufferType)
                continue;
        }

        if (isfirst == false)
            outs << ",";

        outs << std::endl << linePrefix << "\t" << arg->name->name;

        isfirst = false;
#if 0 //to do extra arg for gather array
        if (arg->isArray())
        {
        }
#endif
    }

    outs << " );";
}

void CgFunctionIL::genAsSupport(std::ostream &shader, CgKernel* cgkernel)
{
    LOG(LOG_FN_ENTRY, "CgFunctionIL::genAsSupport(%p, %p)\n", &shader, cgkernel);

    if (getNumScatterArray() > 0 && isCurrentMain() == false)
        return;

    FunctionType *funcType = AsFunctionType(funcDef->decl->form);
    Decl **allArg = funcType->args;
    int nArg = funcType->nArgs;
    bool isfirst = true;

    funcType->printBase(shader, 0);
    shader << getFuncName() << " (";
    for (int i = 0; i < nArg; i++)
    {
        Decl *arg = allArg[i];
        TypeQual qual = arg->form->getQualifiers();

        if (isScatterType(arg->form))
        {
            continue;
        }
        if (arg->isArray())
        {
            ArrayType* arrayType = AsArrayType(arg->form);
            if(arrayType->resourceType == ConstBufferType)
            {
                //! Skip the gather array declaration for main kernel if constant
                //! buffer used for gather array
                if(getFuncName().compare(cgkernel->getCurKernelName()) != 0)
                {
                    if (isfirst == false)
                        shader << "," << std::endl << "\t";
                    //! Declare gather array declarations for non main kernel as 
                    //! parameter instead of global declaration
                    arrayType->printBase(shader, 0);
                    shader << " " << *arg->name;
                    shader << "[" << arrayType->getTotalElementCount() << "]";
                    isfirst = false; 
                }
                continue;
            }
        }

        if (isfirst == false)
            shader << "," << std::endl << "\t";


        if (arg->isArray())
        {
            cgkernel->genGatherStreamParameter(shader,
                                               arg->name->name,
                                               getArrayRank(arg->form),
                                               paramResource[i]->getGatherComponent());
        }
        else
        {
            if ((qual & TQ_Reduce) != 0)
                shader << "inout ";

            Type *type = arg->form;
            if (arg->isStream())
                type = static_cast < ArrayType * >(type)->subType;

            type->printType(shader, arg->name, true, 0);
        }
        isfirst = false;
    }

    shader << ")" << std::endl;

    funcDef->Block::print(shader, 0);
}

// ----------------------------------------------------------------------------

void
CgFunctionIL::createResource(void)
{
    LOG(LOG_FN_ENTRY, "CgFunctionIL::createResource()\n");
    //add sampler/vpos/const resource

    for (unsigned int i = 0; i < paramResource.size(); i++)
    {
        CgResourceParam * argResource = new CgResourceParamIL(paramResource[i]);
        paramResource[i] = argResource;
    }
}

void
CgFunctionIL::allocateResource(CgResourceManager &resourceManager,
                             int factor, bool outaddr, bool inaddr)
{
    LOG(LOG_FN_ENTRY, "CgFunctionIL::allocateResource(%p, %d, %d, %d)\n", &resourceManager, factor, outaddr, inaddr);

    CgResourceIter iter;
    for (iter = otherResource.begin(); iter != otherResource.end(); iter++)
    {
        free(*iter);
    }
    otherResource.clear();

    //allocate sampler/vpos/const resource
    //
    //The extra vpos for reduction is associated with the FIRST (intput/output) stream,
    //but allocated before the second (input or output) stream and after the first
    //(input or output) stream. Changing this requires corresponding change in the runtime.
    for (int i = 0; i < numArg; i++)
    {
        CgResourceParam * argResource = paramResource[i];
        argResource->resetAllocate();

        argResource->allocate(resourceManager, factor, outaddr, inaddr);

        if(argResource->isScatterArray())
        {
            ArrayType *arrayType = AsArrayType(argResource->getDeclaration()->form);
            int arrayDim = arrayType->getDimensions();

            // We don't need to add this constant if scatter dimension is 1 or 3
            if(arrayDim == 2)
            {
                BaseType *bt = new BaseType(BT_Int);

                std::string tmpstr = CgResourceParam::getOutputBufferPitchName();
                CgResourceConst *resConst = new CgResourceConstIL(bt, tmpstr, NULL, 0, "brook::CONST_BUFFERPITCH");

                resConst->allocate(resourceManager);
                otherResource.push_back(resConst);
            }
        }

        //if (outaddr==false && inaddr==false && argResource->isReduceInput())
        //    argResource->allocateReduceVpos(resourceManager, factor, outaddr, inaddr);
    }

    //! For Address translation, output streams required two extra constants 
    //! stream shape : logical shape of stream
    //! buffer shape : physical shape used by runtime
    if (outaddr)
    {
        BaseType *bt = new BaseType(BT_Int4);
        CgResourceConst *resConst;

        std::string tmpstr;
        tmpstr = CgResourceParam::getOutputStreamShapeName();
        resConst = new CgResourceConstIL(bt, tmpstr, NULL, 0, "brook::CONST_STREAMSHAPE");

        resConst->allocate(resourceManager);
        otherResource.push_back(resConst);

        tmpstr = CgResourceParam::getOutputBufferShapeName();
        resConst = new CgResourceConstIL(bt, tmpstr, NULL, 0, "brook::CONST_BUFFERSHAPE");

        resConst->allocate(resourceManager);
        otherResource.push_back(resConst);
    }
}
// ----------------------------------------------------------------------------

