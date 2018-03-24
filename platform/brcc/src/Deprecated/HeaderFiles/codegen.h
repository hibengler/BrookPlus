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
 * codegen.h --
 *
 *      Interface to codegen-- which is the module responsible for taking
 *      parsed brook and spitting out the required gpu assembly and stubs.
 */
#ifndef INC_CODEGEN_H
#define INC_CODEGEN_H

#include "decl.h"

typedef enum
{
    CODEGEN_PS20 = 0,
    CODEGEN_FP30,
    CODEGEN_ARB,
    CODEGEN_FP40,
    CODEGEN_PS30,
    CODEGEN_PS2B,
    CODEGEN_PS2A,
    CODEGEN_CAL,
    CODEGEN_NUM_TARGETS
} CodeGenTarget;

static const char *CODEGEN_TARGET_NAMES[CODEGEN_NUM_TARGETS] = {
    "ps20",
    "fp30",
    "arb",
    "fp40",
    "ps30",
    "ps2b",
    "ps2a",
    "cal"
};

struct ShaderResourceUsage
{
    int arithmeticInstructionCount;
    int textureInstructionCount;
    int samplerRegisterCount;
    int interpolantRegisterCount;
    int constantRegisterCount;
    int temporaryRegisterCount;
    int outputRegisterCount;
};

static inline const char *CodeGen_TargetName(CodeGenTarget t)
{
    assert(t >= 0 && t < CODEGEN_NUM_TARGETS);
    return CODEGEN_TARGET_NAMES[t];
}

void CodeGen_Init(void);

extern char *CodeGen_GenerateCode(Type * retType, const char *name,
                                  Decl ** args, int nArgs, const char *body, CodeGenTarget target);

void CodeGen_SplitAndEmitCode(FunctionDef * inFunctionDef, CodeGenTarget target, std::ostream & inStream);

#endif // INC_CODEGEN_H
