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

#ifndef INC_CG_KERNEL_IL_H
#define INC_CG_KERNEL_IL_H

#include "cgKernel.h"

class CgKernelIL : public CgKernel
{
public:
    CgKernelIL(CgFunction *func, std::string& name, int rt, int rfactor = 8)
        :CgKernel(func, name, rt, rfactor)
    {
        LOG(LOG_FN_ENTRY, "CgKernelIL::CgKernelIL(%p, %s, %d, %d)\n", func, name.c_str(), rt, rfactor);
    }

    void genConstDeclare(std::ostream &shader, std::string &name, const char* typeName, int id);
    void genFetchStreamDeclare(std::ostream &shader, std::string &name, int rank, int id);
    void genGatherStreamDeclare(std::ostream &shader, std::string &name, int rank, int component, int id);
    void genGatherStreamParameter(std::ostream &shader, std::string &name, int rank, int component);

private:

    int genPass(CgPass&, int, bool, bool, int passNum, OutputStreamType outType);
    int genTechnique(CgTechnique&);

    void genPassMacro(std::ostream &);
    void genPassOpen(std::ostream &);
    void genPassClose(std::ostream &);
    char *genPassCompile(CgPass &pass, std::string &shadername, const char *hlslstring, ShaderType shaderType = PixelShader);
    int normGatherRank(int v);

};

// ----------------------------------------------------------------------------

#endif // INC_CG_KERNEL_IL_H
