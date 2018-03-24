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

#ifndef INC_CG_KERNEL_H
#define INC_CG_KERNEL_H

//#include <map>
#include <string.h>

#include "logger.hpp"
#include "cgTool.h"
#include "project.h"
#include "stemnt.h"
#include "cgResource.h"

// ----------------------------------------------------------------------------

#define cgVarPrefix "__"

// ----------------------------------------------------------------------------

enum InputType
{
    INPUT_NOT_SAMPLER = 0,
    INPUT_ACCESS_RANDOM,
    INPUT_ACCESS_STREAM,
    OUTPUT_ACCESS_RANDOM,
    OUTPUT_ACCESS_STREAM,
    INPUT_CONSTBUFFER
};


typedef enum _OutputStreamType
{
    ORDINARY_OUTPUT_STREAM = 0,
    SCATTER_OUTPUT_STREAM,
    MIXED_OUTPUT_STREAM
}OutputStreamType;


/**
 * a class represents the meta data for an input
 */
class CgInput
{
public:
    CgInput(int, int, const std::string &hlslname, int resid = -1,InputType type = INPUT_NOT_SAMPLER);
    CgInput(int, const std::string&, const std::string &hlslname, int resid = -1, InputType type = INPUT_NOT_SAMPLER);
    void setHlslName(const std::string &name) {hlslName = name;}
    const std::string& getHlslName() const {return hlslName;}

    void setResourceId(int resid) {resourceId = resid;}
    int getResourceId() {return resourceId;}
    bool isInputasConstBuffer() {return (INPUT_CONSTBUFFER == inputType);}

    void gen(std::ostream &, char*, char*) const;

private:
    int argumentIndex;
    std::string componentName;
    int resourceId;
    std::string hlslName;

    //! input type information
    InputType inputType;
};
typedef std::vector<CgInput> CgInputTbl;
typedef std::vector<CgInput>::const_iterator CgInputIter;

// ----------------------------------------------------------------------------

/**
 * a class represents a shader for a pass and its meta data
 */
class CgPass
{
public:
    CgPass(int first, int bound)
        : firstOutput(first), lastOutput(bound - 1)
    {
        LOG(LOG_FN_ENTRY, "CgPass::CgPass(%d, %d)\n", first, bound);
    }

    void addConstant(int arg, int comp, const std::string &hlslname)
    {
        LOG(LOG_FN_ENTRY, "CgPass::addConstant(%d, %d, %s)\n", arg, comp, hlslname.c_str());
        constants.push_back(CgInput(arg, comp, hlslname));
    }

    void addConstant(int arg, const std::string & comp, const std::string &hlslname)
    {
        LOG(LOG_FN_ENTRY, "CgPass::addConstant(%d, %s, %s)\n", arg, comp.c_str(), hlslname.c_str());
        constants.push_back(CgInput(arg, comp, hlslname));
    }

    void addSampler(int arg, int comp, const std::string &hlslname, int resid, InputType type)
    {
        LOG(LOG_FN_ENTRY, "CgPass::addSampler(%d, %d, %s, %d)\n", arg, comp, hlslname.c_str(), resid);
        samplers.push_back(CgInput(arg, comp, hlslname, resid, type));
    }
#if KEEP_INTERPOLANTS
    void addInterpolant(int arg, int comp, const std::string &hlslname)
    {
        LOG(LOG_FN_ENTRY, "CgPass::addInterpolant(%d, %d, %s)\n", arg, comp, hlslname.c_str());
        interpolants.push_back(CgInput(arg, comp, hlslname));
    }
    void addInterpolant(int arg, const std::string & comp, const std::string &hlslname)
    {
        LOG(LOG_FN_ENTRY, "CgPass::addInterpolant(%d, %s, %s)\n", arg, comp.c_str(), hlslname.c_str());
        interpolants.push_back(CgInput(arg, comp, hlslname));
    }
#endif // KEEP_INTERPOLANTS
    void addOutput(int arg, int comp, const std::string &hlslname, int resid, InputType type)
    {
        LOG(LOG_FN_ENTRY, "CgPass::addOutput(%d, %d, %s, %d)\n",
            arg, comp, hlslname.c_str(), resid);
        outputs.push_back(CgInput(arg, comp, hlslname, resid, type));
    }
    void addScatter(int arg, int comp, const std::string &hlslname, int resid, InputType type)
    {
        LOG(LOG_FN_ENTRY, "CgPass::addOutput(%d, %d, %s, %d)\n",
            arg, comp, hlslname.c_str(), resid);
        scatter.push_back(CgInput(arg, comp, hlslname, resid, type));
    }
    void adjustSampler(const std::string &hlslname, int resourceid);
    void adjustOutput(const std::string &hlslname, int resourceid);
    void adjustScatter(const std::string &hlslname, int resourceid);
    void adjustConstant(const std::string &hlslname, int, int);

    void genMetadata(std::ostream &outs, char *lineprefix) const;
    void genShader(std::ostream &outs, char *lineprefix) const;

    void setShader(const char*);

    const std::string& getShader() const {return shader;}
    int getFirstOutput() {return firstOutput;}
    int getLastOutput() {return lastOutput;}

private:
    int firstOutput;
    int lastOutput;
    std::string shader;
    CgInputTbl constants;
    CgInputTbl samplers;
#if KEEP_INTERPOLANTS
    CgInputTbl interpolants;
#endif //KEEP_INTERPOLANTS
    CgInputTbl outputs;
    CgInputTbl scatter;
};
typedef std::vector<CgPass> CgPassTbl;
typedef std::vector<CgPass>::const_iterator CgPassIter;

// ----------------------------------------------------------------------------

class CgTechnique
{
public:
    CgTechnique(int ref = 0, bool outaddr = false, bool inaddr = false);

    void addPass(CgPass &pass)
        {
            LOG(LOG_FN_ENTRY, "CgTechnique::addPass(%p)\n", &pass);
            passes.push_back(pass);
        }
    const CgPassTbl& getPass() const {return passes;}

    void genMetadata(std::ostream &outs, char* lineprefix) const;

    int getReduceFactor() const {return reduceFactor;}
    bool isOutputAT() {return addressTranslation;}
    //bool isInputAddrVirt() {return inputAddrVirt;}
    //void setReduceFactor(int v) {reduceFactor = v;}

private:
    CgPassTbl passes;
    int reduceFactor;
    
    //! AddressTranslation is requied on output
    bool addressTranslation;
    //bool outputAddrVirt;
    //bool inputAddrVirt;
};

typedef std::vector<CgTechnique> CgTechniqueTbl;
typedef std::vector<CgTechnique>::const_iterator CgTechniqueIter;

// ----------------------------------------------------------------------------

class CgFunction;

//not sure how dx hlsl different from amdhlsl, we may not need to simplify the logic here ...

class CgKernel
{
public:
    CgKernel(CgFunction *func, std::string& name, int, int rfactor = 8);

    void codegen(std::ostream &outs, bool);
    std::string getCurKernelName();

    virtual void genConstDeclare(std::ostream &shader, std::string &name, const char* typeName, int id) = 0;
    virtual void genFetchStreamDeclare(std::ostream &shader, std::string &name, int rank, int id) = 0;
    virtual void genGatherStreamDeclare(std::ostream &shader, std::string &name, int rank, int component, int id) = 0;
    virtual void genGatherStreamParameter(std::ostream &shader, std::string &name, int rank, int component) = 0;

    virtual ~CgKernel(){}

protected:

    virtual int genPass(CgPass&, int, bool, bool, int passNum, OutputStreamType outType);
    virtual int genTechnique(CgTechnique&);

    virtual void genPassMacro(std::ostream &) = 0;
    virtual void genPassOpen(std::ostream &) {};
    virtual void genPassClose(std::ostream &) {};
    virtual char *genPassCompile(CgPass &pass, std::string &shadername, const char *hlslstring, ShaderType shaderType = PixelShader) = 0;

protected:
    CgFunction *cgFunc;
    std::string kernelVarName; //kernel function pointer name, fixme syn issue with cpuKernel ...
    CgTechniqueTbl allTechnique;
    int mrt; //maximum render target per pass
    int maxReductionFactor;
};

// ----------------------------------------------------------------------------

#endif // INC_CG_KERNEL_H
