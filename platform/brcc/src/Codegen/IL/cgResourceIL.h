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

#ifndef INC_CG_RESOURCE_IL_H
#define INC_CG_RESOURCE_IL_H

#include "cgResource.h"

// ----------------------------------------------------------------------------
class CgResourceConstIL : public CgResourceConst
{
public:
    CgResourceConstIL(BaseType *cType, std::string &cPath, CgResourceParam *par, int compid, const char* cName = NULL )
        : CgResourceConst(cType, cPath, par, compid, cName)
        {
            LOG(LOG_FN_ENTRY, "CgResourceConst::CgResourceConst(%p, %s, %p, %d, %s)\n", cType, cPath.c_str(), par, compid, cName);
        }

    bool genDeclare(std::ostream &, char *lineprefix, CgPass&, CgKernel *, int);

};


// ----------------------------------------------------------------------------
class CgResourceSamplerIL : public CgResourceSampler
{
public:
    CgResourceSamplerIL(BaseType *cType, std::string &cPath, CgResourceParam *par, int compid)
        : CgResourceSampler(cType, cPath, par, compid)
        {
            LOG(LOG_FN_ENTRY, "CgResourceSampler::CgResourceSampler(%p, %s, %p, %d)\n", cType, cPath.c_str(), par, compid);
        }

    void genFetch(std::ostream &outs, char* linePrefix, std::string &texpos, const char * target = NULL);
};

// ----------------------------------------------------------------------------

/**
 * a set of Sampler/Output/Constant resource for a parameter
 */
class CgResourceParamIL : public CgResourceParam
{
public:
    CgResourceParamIL(Decl *decl, int arg)
        : CgResourceParam(decl, arg)
    {
    }

    CgResourceParamIL(CgResourceParam* param)
        : CgResourceParam(*param)
    {
    }

    void allocate(CgResourceManager& manager, int factor, bool, bool);
    void allocate(CgResourceManager& manager, CgRTKind, BaseType*, std::string&, int);
    void genLocal(std::ostream &outs, bool, bool, int, char* linePrefix, ShaderType shaderType = PixelShader);
};



// ----------------------------------------------------------------------------
#endif // INC_CG_RESOURCE_IL_H
