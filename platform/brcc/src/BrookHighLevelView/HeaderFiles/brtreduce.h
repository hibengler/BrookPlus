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
 * brtreduce.h
 *
 *      Header for the various BRT Reduce classes (objects responsible for
 *      compiling and emitting reducers for the various backends).
 */
#ifndef INC_BRTREDUCE_H
#define INC_BRTREDUCE_H

#include "logger.hpp"
#include "dup.h"
#include "stemnt.h"
#include "brtkernel.h"
#include "b2ctransform.h"

#if BROOK_TO_IL_CODE_GEN_PATH
class BRTFP30ReduceCode:public BRTFP30KernelCode
{
public:
    BRTFP30ReduceCode(const FunctionDef & _fDef):BRTFP30KernelCode(_fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTFP30ReduceCode::BRTFP30ReduceCode(%p)\n", &_fDef);
        };

    ~BRTFP30ReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTFP30ReduceCode::~BRTFP30ReduceCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTFP30ReduceCode::dup0()\n");
            return new BRTFP30ReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};

class BRTFP40ReduceCode:public BRTFP40KernelCode
{
public:
    BRTFP40ReduceCode(const FunctionDef & _fDef):BRTFP40KernelCode(_fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTFP40ReduceCode::BRTFP40ReduceCode(%p)\n", &_fDef);
        };

    ~BRTFP40ReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTFP40ReduceCode::~BRTFP40ReduceCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTFP40ReduceCode::dup0()\n");
            return new BRTFP40ReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};

class BRTARBReduceCode:public BRTARBKernelCode
{
public:
    BRTARBReduceCode(const FunctionDef & _fDef):BRTARBKernelCode(_fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTARBReduceCode::BRTARBReduceCode(%p)\n", &_fDef);
        };

    ~BRTARBReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTARBReduceCode::~BRTARBReduceCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTARBReduceCode::dup0()\n");
            return new BRTARBReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};

class BRTPS20ReduceCode:public BRTPS20KernelCode
{
public:
    BRTPS20ReduceCode(const FunctionDef & fDef);
    ~BRTPS20ReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS20ReduceCode::~BRTPS20ReduceCode(%p)\n", &fDef);
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTPS20ReduceCode::dup0()\n");
            return new BRTPS20ReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};

class BRTPS2BReduceCode:public BRTPS2BKernelCode
{
public:
    BRTPS2BReduceCode(const FunctionDef & fDef);
    ~BRTPS2BReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS2BReduceCode::~BRTPS2BReduceCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTPS2BReduceCode::dup0()\n");
            return new BRTPS2BReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};

class BRTPS2AReduceCode:public BRTPS2AKernelCode
{
public:
    BRTPS2AReduceCode(const FunctionDef & fDef);
    ~BRTPS2AReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS2AReduceCode::~BRTPS2AReduceCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTPS2AReduceCode::dup0()\n");
            return new BRTPS2AReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};

class BRTPS30ReduceCode:public BRTPS30KernelCode
{
public:
    BRTPS30ReduceCode(const FunctionDef & fDef);
    ~BRTPS30ReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS30ReduceCode::~BRTPS30ReduceCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTPS30ReduceCode::dup0()\n");
            return new BRTPS30ReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};
#endif
class BRTCALReduceCode:public BRTCALKernelCode
{
public:
    BRTCALReduceCode(const FunctionDef & fDef);
    ~BRTCALReduceCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTCALReduceCode::~BRTCALReduceCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTCALReduceCode::dup0()\n");
            return new BRTCALReduceCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};


class BRTCPUReduceCode:public BRTCPUKernelCode
{
public:
    BRTCPUReduceCode(const FunctionDef & fDef):BRTCPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTCPUReduceCode::BRTCPUReduceCode(%p)\n", &fDef);
        };
};

#endif /* INC_BRTREDUCE_H */
