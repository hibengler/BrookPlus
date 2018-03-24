/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/


/*
 * cgc.h --
 *
 *      Interface to cgc -- Used to call cgc.exe compiler, just a stub nowadays.
 */
#ifndef INC_CGC_H
#define INC_CGC_H

#include "codegen.h"

char *compile_cgc(const char *name, const char *shader, CodeGenTarget target,
                  ShaderResourceUsage * outUsage = NULL, bool inValidate = true);

#endif // INC_CGC_H
