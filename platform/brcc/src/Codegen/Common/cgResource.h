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

#ifndef INC_CG_RESOURCE_H
#define INC_CG_RESOURCE_H

//#include <map>
#include <string.h>

#include "logger.hpp"
#include "cgTool.h"
#include "project.h"
#include "stemnt.h"
#include "decl.h"

// ----------------------------------------------------------------------------

typedef enum
{
    CGT_CAL = 0,
    CGT_DX9,
    CGT_CPU,
    CGT_TOTAL
} CgTarget;

class CgPass;
class CgKernel;

#define VPOS_PREFIX "__vPos"
// ----------------------------------------------------------------------------

class CgResourceManager
{

public:
    CgResourceManager()
    {
        nextSamplerId = 0;
        nextOutputId = 0;
        nextVposId = 0;
        nextConstId = 0;
        hasoutvpos = false;
    }

    int allocateVpos()
    {
        LOG(LOG_FN_ENTRY, "CgResourceManager::allocateVpos()\n");
        return nextVposId ++;
    }

    int allocateSampler()
    {
        LOG(LOG_FN_ENTRY, "CgResourceManager::allocateSampler()\n");
        return nextSamplerId ++;
    }

    int allocateOutput()
    {
        LOG(LOG_FN_ENTRY, "CgResourceManager::allocateOutput()\n");
        return nextOutputId ++;
    }

    int allocateConst()
    {
        LOG(LOG_FN_ENTRY, "CgResourceManager::allocateConst()\n");
        return nextConstId ++;
    }

    int getNextOutputId() {return nextOutputId;}
    int getNextConstId() {return nextConstId;}
    bool hasOutputVpos() {return hasoutvpos;}
    void setHasOutputVpos() {hasoutvpos = true;}

private:
    int nextVposId;
    int nextSamplerId;
    int nextOutputId;
    int nextConstId;
    bool hasoutvpos;
};

// ----------------------------------------------------------------------------
class CgResourceVpos;
class CgResourceParam;

class CgResource
{
public:
    CgResource(BaseType *cType, std::string &cPath, CgResourceParam* par, int compid, const char* cName=NULL)
        : resourceId(-1), compIndex(compid), compType(cType), compPath(cPath), parent(par)
    {
        LOG(LOG_FN_ENTRY, "CgResource::CgResource(%p, %s, %p, %d, %s)\n", cType, cPath.c_str(), par, compid, cName);
        if (cName != NULL)
            compName = cName;
    }

    virtual void allocate(CgResourceManager& manager) = 0;
    virtual bool genDeclare(std::ostream &, char *lineprefix, CgPass&, CgKernel *, int) = 0;

    virtual void genFetch(std::ostream &, char* , std::string &texpos, const char * target=NULL) {}
    virtual void genStore(std::ostream &outs, char* linePrefix, int, int) {}

    virtual ~CgResource(){};

    std::string getVarName();
    std::string getExpName();

protected:

    virtual std::string getResourceNamePrefix() = 0;

    int         resourceId;
    int         compIndex;
    std::string compName;
    BaseType    *compType;  //reference
    std::string compPath;
    CgResourceParam *parent;
};

typedef std::vector<CgResource *> CgResourceTbl;
typedef std::vector<CgResource *>::iterator CgResourceIter;

// ----------------------------------------------------------------------------
class CgResourceConst : public CgResource
{
public:
    CgResourceConst(BaseType *cType, std::string &cPath, CgResourceParam *par, int compid, const char* cName = NULL )
        : CgResource(cType, cPath, par, compid, cName)
        {
            LOG(LOG_FN_ENTRY, "CgResourceConst::CgResourceConst(%p, %s, %p, %d, %s)\n", cType, cPath.c_str(), par, compid, cName);
        }

    void allocate(CgResourceManager &manager)
    {
        LOG(LOG_FN_ENTRY, "CgResourceConst::allocate(%p)\n", &manager);
        resourceId = manager.allocateConst();
    }

    std::string getResourceNamePrefix();
    bool genDeclare(std::ostream &, char *lineprefix, CgPass&, CgKernel *, int);

private:

};

// ----------------------------------------------------------------------------
class CgResourceVpos;
class CgResourceSampler : public CgResource
{
public:
    CgResourceSampler(BaseType *cType, std::string &cPath, CgResourceParam *par, int compid)
        : CgResource(cType, cPath, par, compid)
        {
            LOG(LOG_FN_ENTRY, "CgResourceSampler::CgResourceSampler(%p, %s, %p, %d)\n", cType, cPath.c_str(), par, compid);
        }

    void allocate(CgResourceManager &manager)
    {
        LOG(LOG_FN_ENTRY, "CgResourceSampler::allocate(%p)\n", &manager);
        resourceId = manager.allocateSampler();
    }

    std::string getResourceNamePrefix();

    bool genDeclare(std::ostream &, char *lineprefix, CgPass&, CgKernel *, int);
    void genFetch(std::ostream &outs, char* linePrefix, std::string &texpos, const char * target = NULL);

    bool isGatherArray();
    bool isResourceTypeConstBuffer();
    int getRank();
private:

};

// ----------------------------------------------------------------------------


class CgResourceOutput : public CgResource
{
    public:
    CgResourceOutput(BaseType *cType, std::string &cPath, CgResourceParam *par, int compid)
        : CgResource(cType, cPath, par, compid)
        {}

    void allocate(CgResourceManager &manager)
    {
        resourceId = manager.allocateOutput();
    }

    std::string getResourceNamePrefix();

    bool genDeclare(std::ostream &, char *lineprefix, CgPass&, CgKernel *, int);
    void genStore(std::ostream &outs, char* linePrefix, int, int);
private:
};

// ----------------------------------------------------------------------------

class CgResourceVpos : public CgResource
{
public:
    CgResourceVpos(BaseType *cType, std::string &cPath, CgResourceParam *par, int compid, const char* cName = NULL)
        : CgResource(cType, cPath, par, compid, cName)
    {
            LOG(LOG_FN_ENTRY, "CgResourceVpos::CgResourceVpos(%p, %s, %p, %d, %s)\n", cType, cPath.c_str(), par, compid, cName);
    }

    void allocate(CgResourceManager &manager)
    {
        LOG(LOG_FN_ENTRY, "CgResourceVpos::allocate(%p)\n", &manager);
        resourceId = manager.allocateVpos();
    }

    std::string getResourceNamePrefix();
    bool genDeclare(std::ostream &, char *lineprefix, CgPass&, CgKernel *, int);

private:

};
// ----------------------------------------------------------------------------

/**
 * a set of Sampler/Output/Constant resource for a parameter
 */
class CgResourceParam
{
public:
    typedef enum
    {
        RT_Const, RT_Sampler, RT_Output, RT_Vpos
    } CgRTKind;

    typedef enum
    {
        PK_ReduceIn, PK_ReduceOut, PK_ScatterOut, PK_Other
    } CgParamKind;
public:
    CgResourceParam(Decl *decl, int);
    virtual ~CgResourceParam();

    virtual void allocate(CgResourceManager& manager, int factor, bool, bool);
    virtual void allocate(CgResourceManager& manager, CgRTKind, BaseType*, std::string&, int);
    virtual void genLocal(std::ostream &outs, bool, bool, int, char* linePrefix, ShaderType shaderType = PixelShader);

    void allocateReduceVpos(CgResourceManager& manager, int factor, bool, bool);
    void resetAllocate();

    bool genDeclare(std::ostream &outs, char *lineprefix, CgPass& pass, CgKernel *, char *delimiter);
    bool genDeclareConst(std::ostream &outs, char *lineprefix, CgPass& pass, CgKernel *);
    bool genDeclareReduceVpos(std::ostream &outs, char *lineprefix, CgPass& pass, CgKernel *);
    void genFetch(std::ostream &outs, bool, bool, int, char* linePrefix, std::string& pos);
    void genIndexofAssign(std::ostream &outs, bool, bool, int, char* linePrefix, int, int);
    void genGlobal(std::ostream &outs, char* linePrefix);
    void genStore(std::ostream &outs, char* linePrefix, int, int);


    int setIndexof();
    bool hasIndexof() {return indexof;}

    static std::string getVposOutput()
    {
        return "outputtexcoord";
    }

    static std::string getInterOutput()
    {
        return "outputaddrinterpolant";
    }

    static std::string getOutputLinearizeName()
    {
        return "__outputlinearize";
    }

    static std::string getOutputStrideName()
    {
        return "__outputstride";
    }

    static std::string getOutputInvstrideName()
    {
        return "__outputinvstride";
    }

    static std::string getOutputInvextentName()
    {
        return "__outputinvextent";
    }

    static std::string getOutputDomainminName()
    {
        return "__outputdomainmin";
    }

    static std::string getOutputDomainsizeName()
    {
        return "__outputdomainsize";
    }

    static std::string getOutputInvshapeName()
    {
        return "__outputinvshape";
    }

    static std::string getOutputHackconstName()
    {
        return "__hackconst";
    }

    static std::string getIndexofOutputName()
    {
        return "__indexofoutput";
    }

    static std::string getVposOutputName()
    {
        return VPOS_PREFIX + getVposOutput();
    }

    std::string getIndexofName()
    {

        return "__indexof_" + param->name->name;
    }

    std::string getVposName()
    {
        return VPOS_PREFIX;
    }

    std::string getStreamindexofnumerName()
    {
        return "__streamindexofnumer_" + param->name->name;
    }

    std::string getStreamindexofdenomName()
    {
        return "__streamindexofdenom_" + param->name->name;
    }

    std::string getStreamlinearizeName()
    {
        return "__streamlinearize_" + param->name->name;
    }

    std::string getStreamtextureshapeName()
    {
        return "__streamtextureshape_" + param->name->name;
    }

    std::string getStreamdomainminName()
    {
        return "__streamdomainmin_" + param->name->name;
    }

    //bool  hasGatherScalebias() {return indexof == false && scalebias != NULL;}
    std::string getGatherScalebiasName()
    {
        return "_const_" + param->name->name + "_scalebias";
    }

    std::string getGatherlinearizeName()
    {
        return "__gatherlinearize_" + param->name->name;
    }

    std::string getGathertexshapeName()
    {
        return "__gathertexshape_" + param->name->name;
    }

    std::string getGatherdomainminName()
    {
        return "__gatherdomainmin_" + param->name->name;
    }

    std::string getFetchScalebiasName()
    {
        return "_const_" + param->name->name + "_invscalebias";
    }

    //! get variable names for constants required for Address translation 
    std::string getGatherStreamShapeName()
    {
        return "__gatherStreamShape_" + param->name->name;
    }
    std::string getGatherBufferShapeName()
    {
        return "__gatherBufferShape_" + param->name->name;
    }
    std::string getInputStreamShapeName()
    {
        return "__inputStreamShape_" + param->name->name;
    }
    std::string getInputBufferShapeName()
    {
        return "__inputBufferShape_" + param->name->name;
    }    
    static std::string getOutputStreamShapeName()
    {
        return "__outputStreamShape";
    }
    static std::string getOutputBufferShapeName()
    {
        return "__outputBufferShape";
    }
    static std::string getOutputBufferPitchName()
    {
        return "__outputBufferPitch";
    }
    std::string getBufferPitchName()
    {
        return "__bufferPitch_" + param->name->name;
    }
    static std::string getReduceConstantName()
    {
        return "__reductionFactor";
    }
    void setReduceOutput() {paramKind = PK_ReduceOut; /*argIndex = 0;*/}
    void setReduceInput() {paramKind = PK_ReduceIn; /*argIndex = 0;*/}

    bool isReduceInput() {return paramKind == PK_ReduceIn;}
    bool isReduceOutput() {return paramKind == PK_ReduceOut;}

    bool isScatterArray() {return paramKind == PK_ScatterOut;}
    //gather stream
    bool isGatherArray() {return paramKind != PK_ScatterOut && param->isArray();}

    //! Returns true if constant buffer used for gather array
    bool isResourceTypeConstBuffer()
    {
        if(isGatherArray())
        {
            ArrayType* arrayType = AsArrayType(param->form);
            return (arrayType->resourceType == ConstBufferType) ? true : false;
        }
        return false;
    }

    int getGatherRank() {return ::getArrayRank(param->form);}
    int getGatherComponent();

    bool isOutput() {return IsOutputArg(param->form->getQualifiers());}
    bool isFetchStream() {return param->isStream() && (param->form->getQualifiers() & TQ_Out) == 0;}

    std::string getName() {return param->name->name;}
    int getArgIndex(){return argIndex;}
    //int getSamplerRank();

    const Decl* getDeclaration() { return param;}

protected:
    CgResourceTbl inout;
    CgResourceTbl reduceVpos;
#if KEEP_VPOS
    CgResourceVpos *vpos;
#endif //KEEP_VPOS
    //CgResourceConst *scalebias;  //scalebias => input stream,  invert scale bias => gather stream

    CgResourceTbl constRes; //additional constant param
    Decl *param;
    CgParamKind paramKind;
    int argIndex;
    bool indexof;

};


typedef std::vector<CgResourceParam *> CgResourceParamTbl;
typedef std::vector<CgResourceParam *>::iterator CgResourceParamIter;

struct allocateInfo
{
    CgResourceParam *resourceParam;
    CgResourceManager &manager;
    CgResourceParam::CgRTKind resourceType;
    int compid;

    allocateInfo(CgResourceParam *r, CgResourceManager &m,
        CgResourceParam::CgRTKind t, int start)
        : resourceParam(r), manager(m), resourceType(t), compid(start)
    {};
};

static void impAllocate(BaseType* componentType, std::string path, void *vdata)
{
    LOG(LOG_FN_ENTRY, " impAllocate(%p, %s, %p)\n", componentType, path.c_str(), vdata);
    allocateInfo *data = (struct allocateInfo*)vdata;
    data->resourceParam->allocate(data->manager, data->resourceType,
        componentType, path, data->compid);
    data->compid ++;
}

// ----------------------------------------------------------------------------
#endif // INC_CG_RESOURCE_H
