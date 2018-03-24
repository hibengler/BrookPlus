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

#include "logger.hpp"
#include "cgResourceIL.h"
#include "cgKernel.h"
#include "main.h"

bool
CgResourceConstIL::genDeclare(std::ostream &outs, char *lineprefix, CgPass &pass, CgKernel *cgkernel, int argIndex)
{
    LOG(LOG_FN_ENTRY, "CgResourceConstIL::genDeclare(%p, %s, %p, %p, %d)\n", &outs, lineprefix, &pass, cgkernel, argIndex);

    //todo struct const is not working yet
    BaseTypeInfo *typeInfo = getBaseTypeInfo(compType->typemask);

    std::string tmpstr = getVarName();
    cgkernel->genConstDeclare(outs, tmpstr, typeInfo->brTypeName, resourceId);

    char dummyType[16];
    bool needDummy = false;

    switch (compType->typemask)
    {
        case BT_Float:
            strcpy(dummyType, "float3");
            needDummy = true;
            break;
        case BT_Float2:
            strcpy(dummyType, "float2");
            needDummy = true;
            break;
        case BT_Float3:
            strcpy(dummyType, "float");
            needDummy = true;
            break;
        case BT_Int:
            strcpy(dummyType, "int3");
            needDummy = true;
            break;
        case BT_Char:
            strcpy(dummyType, "char3");
            needDummy = true;
            break;
        case BT_Short:
            strcpy(dummyType, "short3");
            needDummy = true;
            break;
        case BT_Int2:
            strcpy(dummyType, "int2");
            needDummy = true;
            break;
        case BT_Char2:
            strcpy(dummyType, "char2");
            needDummy = true;
            break;
        case BT_Short2:
            strcpy(dummyType, "short2");
            needDummy = true;
            break;
        case BT_Int3:
            strcpy(dummyType, "int");
            needDummy = true;
            break;
        case BT_Char3:
            strcpy(dummyType, "char");
            needDummy = true;
            break;
        case BT_Short3:
            strcpy(dummyType, "short");
            needDummy = true;
            break;
        case (BT_Int | BT_UnSigned):
            strcpy(dummyType, "uint3");
            needDummy = true;
            break;
        case (BT_Char | BT_UnSigned):
            strcpy(dummyType, "uchar3");
            needDummy = true;
            break;
        case (BT_Short | BT_UnSigned):
            strcpy(dummyType, "ushort3");
            needDummy = true;
            break;
        case (BT_Int2 | BT_UnSigned):
            strcpy(dummyType, "uint2");
            needDummy = true;
            break;
        case (BT_Char2 | BT_UnSigned):
            strcpy(dummyType, "uchar2");
            needDummy = true;
            break;
        case (BT_Short2 | BT_UnSigned):
            strcpy(dummyType, "ushort2");
            needDummy = true;
            break;
        case (BT_Int3 | BT_UnSigned):
            strcpy(dummyType, "uint");
            needDummy = true;
            break;
        case (BT_Char3 | BT_UnSigned):
            strcpy(dummyType, "uchar");
            needDummy = true;
            break;
        case (BT_Short3 | BT_UnSigned):
            strcpy(dummyType, "ushort");
            needDummy = true;
            break;
        case BT_Double:
            strcpy(dummyType, "double");
            needDummy = true;
            break;
    }

    if (needDummy)
    {
        outs << ";" << std::endl << lineprefix;
        outs << dummyType << " __dummy_" << compPath;
    }

    if (compName.empty())
        pass.addConstant(argIndex, "brook::CONST_USER_PARAM", getVarName());
    else
        pass.addConstant(argIndex, compName, getVarName());

    return true;
}

// ----------------------------------------------------------------------------

void
CgResourceSamplerIL::genFetch(std::ostream &outs, char *lineprefix, std::string &texpos, const char *targetname)
{
    LOG(LOG_FN_ENTRY, "CgResourceSamplerIL::genFetch(%p, %s, %s, %s)\n", &outs, lineprefix, texpos.c_str(), targetname);

    BaseTypeInfo *typeInfo = getBaseTypeInfo(compType->typemask);
    //outs << lineprefix << (targetname == NULL ? getExpName() : targetname)
    //    << " = " << "__fetch_" << typeInfo->brTypeName;
    //outs << "(" << getVarName() << ", "
    //    << texpos
    //    << ");" << std::endl;

    outs << lineprefix << (targetname == NULL ? getExpName() : targetname)
        << " = " << "__fetch_" << typeInfo->brTypeName;
    outs << "(" << getVarName() << ", float2(" << texpos << "));" << std::endl;

}

void
CgResourceParamIL::allocate(CgResourceManager &manager, CgRTKind resType, BaseType* compType,
                          std::string& path, int compid)
{
    LOG(LOG_FN_ENTRY, "CgResourceParamIL::allocate(%p, %d, %p, %s, %d)\n", &manager, resType, compType, path.c_str(), compid);
    CgResource *resource = NULL;
    switch (resType)
    {
        case RT_Const:
            resource = new CgResourceConstIL(compType, path, this, compid);
            break;
        case RT_Sampler:
            resource = new CgResourceSamplerIL(compType, path, this, compid);
            break;
        case RT_Output:
            resource = new CgResourceOutput(compType, path, this, compid);
            break;
        default:
            CGEXIT("problem with resource type");
    }
    resource->allocate(manager);
    inout.push_back(resource);
}

void
CgResourceParamIL::allocate(CgResourceManager &manager, int factor, bool outaddr, bool inaddr)
{
    LOG(LOG_FN_ENTRY, "CgResourceParamIL::allocate(%p, %d, %d, %d)\n", &manager, factor, outaddr, inaddr);

    Type *argType = param->form;
    std::string argName = param->name->name;

    if (isOutput())
    {
        if (isScatterArray() == false)
        {
            allocateInfo data(this, manager, RT_Output, 0);
            applyScalarType(argType, argName, impAllocate, &data);
        }
    }
    else if (isFetchStream())
    {
        if (isReduceInput() == false)
        {
            allocateInfo data(this, manager, RT_Sampler, 0);
            applyScalarType(argType, argName, impAllocate, &data);
        }
        else
        {
            allocateInfo data(this, manager, RT_Sampler, 0);
            applyScalarType(argType, argName, impAllocate, &data);

            BaseType *bt = new BaseType(BT_Int4);

            std::string tmpstr = getReduceConstantName();
            CgResourceConst *resConst = new CgResourceConstIL(bt, tmpstr, this, 0, "brook::CONST_REDUCTIONFACTOR");

            resConst->allocate(manager);
            constRes.push_back(resConst);
            //data.compid = 0;
            //applyScalarType(argType, argName, impAllocate, &data);
        }
    }
    else if (isGatherArray())
    {
        assert(indexof == false);
        ArrayType* arrayType = AsArrayType(param->form);
        allocateInfo data(this, manager, RT_Sampler, 0);
        applyScalarType(argType, argName, impAllocate, &data);
    }
    else
    {
        allocateInfo data(this, manager, RT_Const, 0);
        applyScalarType(argType, argName, impAllocate, &data);
    }

    if (outaddr)
    {
        if (isGatherArray() || isReduceInput() || (inaddr && isFetchStream()))
        {
            //project is responsible for delete bt ...
            if(isGatherArray())
            {
                //! No additional constants required for gather arrays which is using
                //! constant buffer as resource
                ArrayType* arrayType = AsArrayType(param->form);
                if(arrayType->resourceType == ConstBufferType)
                    return;
            }
            BaseType *bt = new BaseType(BT_Int4);
            CgResourceConst *resConst;

            std::string tn = getGatherStreamShapeName();
            resConst = new CgResourceConstIL(bt, tn, this, 0, "brook::CONST_STREAMSHAPE");

            resConst->allocate(manager);
            constRes.push_back(resConst);

            tn = getGatherBufferShapeName();
            resConst = new CgResourceConstIL(bt, tn, this, 0, "brook::CONST_BUFFERSHAPE");

            resConst->allocate(manager);
            constRes.push_back(resConst);
        }
    }
}

void
CgResourceParamIL::genLocal(std::ostream &outs, bool outaddr, bool inaddr, int reduceFactor, char *lineprefix, ShaderType shaderType)
{
    LOG(LOG_FN_ENTRY, "CgResourceParamIL::genLocal(%p, %d, %d, %s, %d)\n", &outs, outaddr, inaddr, lineprefix, shaderType);
    Type *argType = param->form;
    TypeQual argQual = argType->getQualifiers();
    std::string argName = param->name->name;

    if ((isReduceInput() || IsOutputArg(argQual)) && isScatterArray() == false)
    {
        //assert((argQual & TQ_Out) != 0);
        argType->getBase()->qualifier &= ~TQ_Out;
        outs << lineprefix;
        argType->printBase(outs, 0);
        outs << " " << argName << ";" << std::endl;
        argType->getBase()->qualifier = argQual;
    }
    else if (param->isStream())
    {
        outs << lineprefix;
        argType->printBase(outs, 0);
        outs << " " << argName << ";" << std::endl;
    }

    if(isReduceInput() && !outaddr)
    {
        outs << lineprefix << "float4 " << getIndexofName() << ";" << std::endl;
    }
    else if(isReduceOutput() && !outaddr)
    {
        outs << lineprefix << "int4 " << getIndexofOutputName() << " = int4(__vPos);" << std::endl;
    }
    else if (outaddr)
    {
        if (isOutput() == false && outaddr && param->isStream())
        {
            outs << lineprefix << "int4 " << getIndexofName() << ";" << std::endl;
        }
        else if (isOutput() && outaddr && (param->isStream() || isScatterArray()))
        {
            outs << lineprefix << "int4 " << getIndexofName() << ";" << std::endl;
            //if (isReduceOutput())
            //    outs << lineprefix << "float2 " << getVposName() << ";" << std::endl;
        }
    }
    else if (indexof)
    {
        if(shaderType == ComputeShader)
        {
            outs << lineprefix << "float4 " << getIndexofName() << " = float2(__threadInfo.atid, 0.0f);" << std::endl;
        }
        else
        {
            outs << lineprefix << "float4 " << getIndexofName() << " = __vPos - float2(0.5f, 0.5f);" << std::endl;
        }
    }
}

// ----------------------------------------------------------------------------
