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
 * brtreduce.cpp
 *
 *      Classes reflecting the body of Brook reduces for the different
 *      backends.  Each one knows how to build itself from a function
 *      definition and then how to emit C++ for itself.
 */
#ifdef _WIN32
#pragma warning(disable:4786)
//the above warning disables visual studio's annoying habit of warning when using the standard set lib
#endif

#include <cstring>
#include <cassert>
#include <sstream>

#include "brtreduce.h"
#include "brtexpress.h"
#include "main.h"

#if BROOK_TO_IL_CODE_GEN_PATH
void BRTFP30ReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTFP30ReduceCode::printCode(%p)\n", &out);
    this->BRTFP30KernelCode::printCode(out);
}


void BRTFP40ReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTFP40ReduceCode::printCode(%p)\n", &out);
    this->BRTFP40KernelCode::printCode(out);
}


void BRTARBReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTARBReduceCode::printCode(%p)\n", &out);
    this->BRTARBKernelCode::printCode(out);
}


BRTPS20ReduceCode::BRTPS20ReduceCode(const FunctionDef & _fDef):BRTPS20KernelCode(_fDef)        //converts gathers
{
    LOG(LOG_FN_ENTRY, "BRTPS20ReduceCode::BRTPS20ReduceCode(%p)\n", &_fDef);
}


BRTPS2BReduceCode::BRTPS2BReduceCode(const FunctionDef & _fDef):BRTPS2BKernelCode(_fDef)        //converts gathers
{
    LOG(LOG_FN_ENTRY, "BRTPS2BReduceCode::BRTPS2BReduceCode(%p)\n", &_fDef);
}


BRTPS2AReduceCode::BRTPS2AReduceCode(const FunctionDef & _fDef):BRTPS2AKernelCode(_fDef)        //converts gathers
{
    LOG(LOG_FN_ENTRY, "BRTPS2AReduceCode::BRTPS2AReduceCode(%p)\n", &_fDef);
}


BRTPS30ReduceCode::BRTPS30ReduceCode(const FunctionDef & _fDef):BRTPS30KernelCode(_fDef)        //converts gathers
{
    LOG(LOG_FN_ENTRY, "BRTPS30ReduceCode::BRTPS30ReduceCode(%p)\n", &_fDef);
}
void BRTPS20ReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS20ReduceCode::printCode(%p)\n", &out);
    this->BRTPS20KernelCode::printCode(out);
}

void BRTPS2BReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS2BReduceCode::printCode(%p)\n", &out);
    this->BRTPS2BKernelCode::printCode(out);
}

void BRTPS2AReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS2AReduceCode::printCode(%p)\n", &out);
    this->BRTPS2AKernelCode::printCode(out);
}

void BRTPS30ReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTPS30ReduceCode::printCode(%p)\n", &out);
    this->BRTPS30KernelCode::printCode(out);
}
#endif

BRTCALReduceCode::BRTCALReduceCode(const FunctionDef & _fDef):BRTCALKernelCode(_fDef)   //converts gathers
{
    LOG(LOG_FN_ENTRY, "BRTCALReduceCode::BRTCALReduceCode(%p)\n", &_fDef);
}

void BRTCALReduceCode::printCode(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, " BRTCALReduceCode::printCode(%p)\n", &out);
    this->BRTCALKernelCode::printCode(out);
}
