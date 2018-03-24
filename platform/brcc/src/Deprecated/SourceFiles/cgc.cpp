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
 * cgc.cpp
 *
 *  Stub, cgc not needed for CAL runtime. FIXME - should never be called and eventually removed.
 */

#ifdef _WIN32
#pragma warning(disable:4786)
#include <ios>
#else
#include <iostream>
#endif
#include <sstream>
#include <iomanip>
#include <fstream>
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <assert.h>
}

#include "logger.hpp"
#include "main.h"
#include "decl.h"
#include "subprocess.h"
#include "cgc.h"

/*
 * compile_cgc --
 *
 *      Takes CG code and runs it through the CG compiler (and parses the
 *      results) to produce the corresponding fragment program.
 */

char *compile_cgc(UNUSED(const char *name),
                  UNUSED(const char *shader), 
		  UNUSED(CodeGenTarget target), 
		  UNUSED(ShaderResourceUsage *outUsage),
                  UNUSED(bool inValidate))
{
    fputs("Internal error: compile_cgc called", stderr);
    return NULL;
}
