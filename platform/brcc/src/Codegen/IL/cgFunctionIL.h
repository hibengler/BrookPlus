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

#ifndef INC_CG_FUNCTION_IL_H
#define INC_CG_FUNCTION_IL_H

#include "cgProgram.h"

class CgFunctionIL : public CgFunction
{
public:

    CgFunctionIL(CgFunction* base);
    ~CgFunctionIL();

    void createResource(void);
    void codegen(std::ostream &outs);
    void allocateResource(CgResourceManager&, int factor, bool outaddr, bool inaddr = false);
    void genPassCallmain(std::ostream &outs, char* linePrefix);
    void genPassLocal(std::ostream &outs, bool, bool, int, char* linePrefix, OutputStreamType outType);
    void genAsSupport(std::ostream &shader, CgKernel*);
    char* getTargetString();
};

#endif // INC_CG_FUNCTION_IL_H
