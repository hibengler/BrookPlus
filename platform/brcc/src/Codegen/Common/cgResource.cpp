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
#include "cgResource.h"
#include "cgKernel.h"
#include "main.h"

// ----------------------------------------------------------------------------

#define AsVpos(t) static_cast<CgResourceVpos*>(t)
std::string
CgResource::getVarName()
{
    LOG(LOG_FN_ENTRY, "CgResource::getVarName()\n");
    std::string name = compPath;
    stringReplace(name, '.', '_');

    std::ostringstream outs;
    outs << getResourceNamePrefix() << name;

    return outs.str();
}

std::string
CgResource::getExpName()
{
    LOG(LOG_FN_ENTRY, "CgResource::getExpName()\n");
    return compPath;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

std::string
CgResourceConst::getResourceNamePrefix()
{
    LOG(LOG_FN_ENTRY, "CgResourceConst::getResourceNamePrefix()\n");
    return std::string(""); //const");
}

bool
CgResourceConst::genDeclare(std::ostream &outs, char *lineprefix, CgPass &pass, CgKernel *cgkernel, int argIndex)
{
    LOG(LOG_FN_ENTRY, "CgResourceConst::genDeclare(%p, %s, %p, %p, %d)\n", &outs, lineprefix, &pass, cgkernel, argIndex);


    return true;
}

// ----------------------------------------------------------------------------

std::string
CgResourceSampler::getResourceNamePrefix()
{
    LOG(LOG_FN_ENTRY, "CgResourceSampler::getResourceNamePrefix()\n");
    //outs << "__" << getResourceNamePrefix() << resourceId << "_"
    if (isGatherArray())
        return std::string();

    return std::string("__stream");
}

bool
CgResourceSampler::genDeclare(std::ostream &outs,char *lineprefix, CgPass &pass, CgKernel* cgkernel, int argIndex)
{
    LOG(LOG_FN_ENTRY, "CgResourceSampler::genDeclare(%p, %s, %p, %p, %d)\n", &outs, lineprefix, &pass, cgkernel, argIndex);

    bool hasOut = false;
    if (isGatherArray())
    {
        if (compIndex == 0)
        {
            if(isResourceTypeConstBuffer())
            {
                hasOut = false;
            }
            else
            {
                std::string tmpstr = parent->getName();
                cgkernel->genGatherStreamDeclare(outs,
                    tmpstr,
                    getRank(),
                    parent->getGatherComponent(),
                    resourceId);

                hasOut = true;
            }
        }
    }
    else
    {
        std::string tmpstr = getVarName();
        cgkernel->genFetchStreamDeclare(outs,
                                        tmpstr,
                                        getRank(),
                                        resourceId);

        hasOut = true;
    }

    int myIndex = argIndex;

    InputType type = INPUT_NOT_SAMPLER;
    if(isResourceTypeConstBuffer())
    {
        type = INPUT_CONSTBUFFER;
    }
    else if(isGatherArray())
    {
        type = INPUT_ACCESS_RANDOM;
    }
    else
    {
        type = INPUT_ACCESS_STREAM;
    }

    pass.addSampler(myIndex, compIndex, isGatherArray()? parent->getName() : getVarName(), resourceId, type);

    return hasOut;
}

void
CgResourceSampler::genFetch(std::ostream &outs, char *lineprefix, std::string &texpos, const char *targetname)
{
    LOG(LOG_FN_ENTRY, "CgResourceSampler::genFetch(%p, %s, %s, %s)\n", &outs, lineprefix, texpos.c_str(), targetname);
}

bool
CgResourceSampler::isGatherArray()
{
    LOG(LOG_FN_ENTRY, "CgResourceSampler::isGatherArray()\n");
    return (parent && parent->isGatherArray());
}

//! Returns true if constant buffer used for gather array
bool
CgResourceSampler::isResourceTypeConstBuffer()
{
    return (parent->isResourceTypeConstBuffer());
}
int CgResourceSampler::getRank()
{
    LOG(LOG_FN_ENTRY, " CgResourceSampler::getRank()\n");
    if (parent && parent->isGatherArray())
        return parent->getGatherRank();
    return 2;
}

// ----------------------------------------------------------------------------

std::string
CgResourceOutput::getResourceNamePrefix()
{
    LOG(LOG_FN_ENTRY, "CgResourceOutput::getResourceNamePrefix()\n");
    return std::string("__output");
}

bool
CgResourceOutput::genDeclare(std::ostream &outs,
                             char *lineprefix,
                             CgPass &pass,
                             CgKernel *dummy,
                             int argIndex)
{
    LOG(LOG_FN_ENTRY, "CgResourceOutput::genDeclare(%p, %s, %p, %p, %d)\n", &outs, lineprefix, &pass, dummy, argIndex);

    if (resourceId >= pass.getFirstOutput() && resourceId <= pass.getLastOutput())
    {
        std::ostringstream tmps;
        tmps << getVarName();
        
        BaseTypeInfo *typeInfo = getBaseTypeInfo(compType->typemask);
        outs << "float4" << " " << tmps.str();

        int myIndex = argIndex;

        pass.addOutput(myIndex, compIndex, getVarName(), resourceId - pass.getFirstOutput(), OUTPUT_ACCESS_STREAM);

        return true;
    }

    return false;
}

void
CgResourceOutput::genStore(std::ostream &outs, char *lineprefix,
                            int firstoutput, int lastoutput)
{
    LOG(LOG_FN_ENTRY, "CgResourceOutput::genStore(%p, %s, %d, %d)\n", &outs, lineprefix, firstoutput, lastoutput);

    if (resourceId >= firstoutput && resourceId <= lastoutput)
    {

        BaseTypeSpec tyMask = compType->typemask;
        bool isUnsigned = isUnsignedType(tyMask);
        if (isUnsigned) 
            tyMask = stripUnsignedMask(tyMask);

        switch (tyMask)
        {
        case BT_Float:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4( " << getExpName() << ", 0.0f, 0.0f, 0.0f);";
            break;
        case BT_Float2:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4( " << getExpName() << ", 0.0f, 0.0f);";
            break;
        case BT_Float3:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4( " << getExpName() << ", 0.0f);";
            break;
        case BT_Float4:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << getExpName() << ";";
            break;
        case BT_Double:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4(asfloat(splitdouble( " << getExpName() << ")), 0.0f, 0.0f);";
            break;
        case BT_Double2:
            outs << std::endl << lineprefix << "__o." << getVarName() << ".xy = " ;
            outs << "asfloat(splitdouble( " << getExpName() << ".x));";
            outs << std::endl << lineprefix << "__o." << getVarName() << ".zw = " ;
            outs << "asfloat(splitdouble( " << getExpName() << ".y));";
            break;
        case BT_Int:
        case BT_Char:
        case BT_Short:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4(asfloat( " << getExpName() << ".x), 0.0f, 0.0f, 0.0f);";
            break;
        case BT_Int2:
        case BT_Char2:
        case BT_Short2:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4(asfloat( " << getExpName() << ".xy), 0.0f, 0.0f);";
            break;
        case BT_Int3:
        case BT_Char3:
        case BT_Short3:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4(asfloat( " << getExpName() << ".xyz), 0.0f);";
            break;
        case BT_Int4:
        case BT_Char4:
        case BT_Short4:
            outs << std::endl << lineprefix << "__o." << getVarName() << " = " ;
            outs << "float4(asfloat( " << getExpName() << ".xyzw));";
            break;
        default:
            CGEXIT("unrecognized stream base type");
        }
    }

}

// ----------------------------------------------------------------------------
std::string
CgResourceVpos::getResourceNamePrefix()
{
    LOG(LOG_FN_ENTRY, "CgResourceVpos::getResourceNamePrefix()\n");
    return std::string(VPOS_PREFIX);
}

bool
CgResourceVpos::genDeclare(std::ostream &outs, char *lineprefix, CgPass &pass, CgKernel *dummy, int argIndex)
{
    LOG(LOG_FN_ENTRY, "CgResourceVpos::genDeclare(%p, %s, %p, %p, %d)\n", &outs, lineprefix, &pass, dummy, argIndex);
    outs << "float2 " << getVarName() << " : " << "TEXCOORD" << resourceId;

    //outs << "," << std::endl << lineprefix;

    //outs << "float2 " << getVarName() << "_dummy_" << compPath << " : " << "TEXCOORD99";
#if KEEP_INTERPOLANTS
    if (parent == NULL)
        pass.addInterpolant(argIndex, compName, getVarName());
    else if (parent->isReduceInput() || parent->isReduceOutput())
        pass.addInterpolant(0, 0, getVarName());
    else if (parent->isOutput())
        pass.addInterpolant(argIndex, "kOutputInterpolant_Position", getVarName());
    else
        pass.addInterpolant(argIndex, "kStreamInterpolant_Position", getVarName());
#endif // KEEP_INTERPOLANTS
    return true;
}

// ----------------------------------------------------------------------------
CgResourceParam::CgResourceParam(Decl *decl, int arg)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::CgResourceParam(%p, %d)\n", decl, arg);
    param = decl;
    argIndex = arg;
    indexof = false;
    if (isScatterType(decl->form))
        paramKind = PK_ScatterOut;
    else
        paramKind = PK_Other;
#if KEEP_VPOS
    vpos = NULL;
#endif // KEEP_VPOS
}

CgResourceParam::~CgResourceParam()
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::~CgResourceParam()\n");
    resetAllocate();
}

void
CgResourceParam::allocate(CgResourceManager &manager, CgRTKind resType, BaseType* compType,
                          std::string& path, int compid)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::allocate(%p, %d, %p, %s, %d)\n", &manager, resType, compType, path.c_str(), compid);
}

void
CgResourceParam::allocateReduceVpos(CgResourceManager &manager, int factor, bool outaddr, bool inaddr)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::allocateReduceVpos(%p, %d, %d, %d)\n", &manager, factor, outaddr, inaddr);
    assert(isReduceInput());
    //Type *argType = param->form;
    std::string argName = param->name->name;

    for (int i = 2; i <= factor; i++)
    {
        std::ostringstream tmpout;
        tmpout << "reduce" << i << argName;

        std::string tmpstr = tmpout.str();
        CgResourceVpos *vposNode = new CgResourceVpos(NULL, tmpstr, this, i);

        vposNode->allocate(manager);
        reduceVpos.push_back(vposNode);
    }
}

void
CgResourceParam::allocate(CgResourceManager &manager, int factor, bool outaddr, bool inaddr)
{
}

void
CgResourceParam::resetAllocate()
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::resetAllocate()\n");
    //if (scalebias)
    //    free(scalebias);

    CgResourceIter iter;
    for (iter = inout.begin(); iter != inout.end(); iter++)
    {
        delete *iter;
    }
    inout.clear();

    for (iter = constRes.begin(); iter != constRes.end(); iter++)
    {
        delete *iter;
    }
    constRes.clear();

    for (iter = reduceVpos.begin(); iter != reduceVpos.end(); iter++)
    {
        delete *iter;
    }
    reduceVpos.clear();

}

bool
CgResourceParam::genDeclareReduceVpos
    (std::ostream &outs, char *lineprefix, CgPass &pass, CgKernel *cgkernel)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::genDeclareReduceVpos(%p, %s, %p, %p)\n", &outs, lineprefix, &pass, cgkernel);

    assert(isReduceInput());

    bool curOut = false;
    bool hasOut = false;
    CgResourceIter iter;

    for (iter = reduceVpos.begin(); iter != reduceVpos.end(); iter++)
    {
        std::ostringstream tmpout;
        if (hasOut)
            tmpout << "," << std::endl << lineprefix;
        curOut = (*iter)->genDeclare(tmpout, lineprefix, pass, cgkernel, argIndex);

        if (curOut)
        {
            hasOut = true;
            outs << tmpout.str();
        }
    }

    return hasOut;
}

bool
CgResourceParam::genDeclare(std::ostream &outs, char *lineprefix, CgPass &pass, CgKernel *cgkernel, char *delimiter)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::genDeclare(%p, %s, %p, %p)\n", &outs, lineprefix, &pass, cgkernel);
    CgResourceIter iter;
    bool curOut = false;
    bool hasOut = false;

    if (isScatterArray())
    {
        pass.addScatter(argIndex, 0, getName(), 0, OUTPUT_ACCESS_RANDOM);
    }

    for (iter = inout.begin(); iter != inout.end(); iter++)
    {
        std::ostringstream tmpout;

        if (hasOut == true)
            tmpout << delimiter << std::endl << lineprefix;

        curOut = (*iter)->genDeclare(tmpout, lineprefix, pass, cgkernel, argIndex);
        if (curOut)
        {
            hasOut = true;
            outs << tmpout.str();
        }
    }
#if KEEP_VPOS
    for (iter = conres.begin(); iter != conres.end(); iter++)
    {
        std::ostringstream tmpout;

        if (hasOut == true)
            tmpout << "," << std::endl << lineprefix;

        curOut = (*iter)->genDeclare(tmpout, lineprefix, pass, cgkernel, argIndex);
        if (curOut)
        {
            hasOut = true;
            outs << tmpout.str();
        }
    }

    if (vpos)
    {
        std::ostringstream tmpout;
        if (hasOut)
            tmpout << "," << std::endl << lineprefix;
        curOut = vpos->genDeclare(tmpout, lineprefix, pass, cgkernel, argIndex);

        if (curOut)
        {
            hasOut = true;
            outs << tmpout.str();
        }
    }
#endif  // KEEP_VPOS
    return hasOut;

}

bool
CgResourceParam::genDeclareConst(std::ostream &outs, char *lineprefix, CgPass &pass, CgKernel *cgkernel)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::genDeclare(%p, %s, %p, %p)\n", &outs, lineprefix, &pass, cgkernel);
    CgResourceIter iter;
    bool curOut = false;
    bool hasOut = false;

    for (iter = constRes.begin(); iter != constRes.end(); iter++)
    {
        std::ostringstream tmpout;

        if (hasOut == true)
            tmpout << ";" << std::endl << lineprefix;

        curOut = (*iter)->genDeclare(tmpout, lineprefix, pass, cgkernel, argIndex);
        if (curOut)
        {
            hasOut = true;
            outs << tmpout.str();
        }
    }
    return hasOut;

}

void
CgResourceParam::genFetch(std::ostream &outs, bool outaddr, bool inaddr, int reduceFactor, char *lineprefix, std::string& pos)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::genFetch(%p, %d, %d, %d, %s)\n", &outs, outaddr, inaddr, reduceFactor, lineprefix);
    CgResourceIter iter;

    for (iter = inout.begin(); iter != inout.end(); iter++)
    {
        (*iter)->genFetch(outs, lineprefix, pos);
    }
}

void
CgResourceParam::genIndexofAssign(std::ostream &outs,
                                  bool outaddr,
                                  bool inaddr,
                                  int reduceFactor,
                                  char *lineprefix,
                                  int dummy1,
                                  int dummy2)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::genIndexofAssign(%p, %d, %d, %s, %d, %d)\n", &outs, outaddr, inaddr, lineprefix, dummy1, dummy2);

    if(isReduceInput())
    {
        outs << lineprefix << getIndexofName() << " = "
             << getIndexofOutputName() << " * " << getReduceConstantName() << ";" << std::endl;;
    }
    else if (inaddr || outaddr)
    {
        if (isFetchStream())
        {
           if (isReduceOutput())
           {
               //do nothing?
           }
           else
           {
                outs << lineprefix << getIndexofName() << " = "
                    << getIndexofOutputName() << ";" << std::endl;

                //outs << lineprefix << getVposName() << " = "
                //    << getVposOutputName() << ";" << std::endl;
           }
        }
        else if (isOutput() && isReduceOutput() == false)
        {
            outs << lineprefix << getIndexofName() << " = ";
            outs << getIndexofOutputName() << ";" << std::endl;
        }
    }
    else if (indexof)
    {
#if KEEP_VPOS
        if (isOutput() == false)
        {
            //assert(vpos.empty()==false && scalebias != NULL);
            assert(vpos);
            outs << lineprefix << getIndexofName() << " = " << "_computeindexof("
                << vpos->getVarName()<< ", " << conres.front()->getVarName() << ");" << std::endl;
        }
        else
        {
            if (vpos)
                outs << lineprefix << getIndexofOutputName() << " = " << "_computeindexof("
                    << vpos->getVarName()<< ", " << conres.front()->getVarName() << ");" << std::endl;
            outs << lineprefix << getIndexofName() << " = " << getIndexofOutputName() << ";" << std::endl;
        }
#endif // KEEP_VPOS
    }
}

void
CgResourceParam::genGlobal(std::ostream &outs, char *lineprefix)
{

    if (isScatterArray()==false)
        return;

    outs << lineprefix << "global ";
    Type *argType = param->form;
    TypeQual argQual = argType->getQualifiers();
    std::string argName = param->name->name;

    //assert (IsOutputArg(argQual));

    //assert((argQual & TQ_Out) != 0);
    argType->getBase()->qualifier &= ~TQ_Out;
    argType->printBase(outs, 0);
    outs << " " << argName << "[] ;" << std::endl;
    argType->getBase()->qualifier = argQual;
}

void
CgResourceParam::genLocal(std::ostream &outs, bool outaddr, bool inaddr, int reduceFactor, char *lineprefix, ShaderType shaderType)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::genLocal(%p, %d, %d, %s, %d)\n", &outs, outaddr, inaddr, lineprefix, shaderType);
}

void
CgResourceParam::genStore(std::ostream &outs, char *lineprefix, int firstoutput, int lastoutput)
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::genStore(%p, %s, %d, %d)\n", &outs, lineprefix, firstoutput, lastoutput);
    CgResourceIter iter;

    for (iter = inout.begin(); iter != inout.end(); iter++)
    {
        (*iter)->genStore(outs, lineprefix, firstoutput, lastoutput);
    }
}

static void impCountComponent(BaseType* componentType, std::string path, void *vdata)
{
    LOG(LOG_FN_ENTRY, " impCountComponent(%p, %s, %p)\n", componentType, path.c_str(), vdata);
    int *pint = (int*) (vdata);
    //fixme: one basetype may corresponds to two component, e.g double4
    (*pint)++;
}

int
CgResourceParam::getGatherComponent()
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::getGatherComponent()\n");
    CGASSERT(isGatherArray(), "internal problem");

    StructDef *structure = findStructureDef(param->form);

    if (inout.empty())
    {
        //before resource allocation
        Type *argType = param->form;
        std::string argName = param->name->name;
        int numComp = 0;
        applyScalarType(argType, argName, impCountComponent, &numComp);

        //! Return no of elements in structure 
        //! Returns zero if it is not a structure
        if(structure)
        {
            return numComp;
        }
        else
        {
            return 0;
        }
    }

    //after resource allocation

    //! Return no of elements in structure 
    //! Returns zero if it is not a structure
    if(structure)
    {
        return AsInt(inout.size());
    }
    else
    {
        return 0;
    }
    
}

int
CgResourceParam::setIndexof()
{
    LOG(LOG_FN_ENTRY, "CgResourceParam::setIndexof()\n");
    if (indexof == false)
    {
        indexof = true;
        return 1;
    }

    //otherwise indicate no change
    return 0;
}
// ----------------------------------------------------------------------------
