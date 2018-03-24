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
 * extract_il.h --
 *
 *      Extract IL from shader string
 */
#ifndef INC_EXTRACT_IL_H
#define INC_EXTRACT_IL_H

#include "codegen.h"

char *passthrough_il(const char *name,
                     const char *shader,
                     CodeGenTarget target, ShaderResourceUsage * outUsage = 0, bool inValidate = true);

char *gpusa_il(const char *name,
               const char *shader, CodeGenTarget target, ShaderResourceUsage * outUsage = 0, bool inValidate = true);

char *strip_il(char const *shader);
char *extract_il(char const *shader, bool leave_prefix = false);

inline char *extract_prefixed_il(char const *shader)
{
    return extract_il(shader, true);
}

#endif // INC_EXTRACT_IL_H
