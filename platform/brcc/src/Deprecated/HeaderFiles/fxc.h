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

/*
 * fxc.h --
 *
 *      Interface to fxc-- Used to call fxc.exe compiler
 */
#ifndef INC_FXC_H
#define INC_FXC_H

#include "codegen.h"

char *compile_fxc(const char *name,
                  const char *shader, CodeGenTarget target, ShaderResourceUsage * outUsage = 0, bool inValidate = true);

#endif // INC_FXC_H
