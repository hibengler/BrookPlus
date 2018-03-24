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
 * brtkernel.h
 *
 *      Header for the various BRT Kernel classes (objects responsible for
 *      compiling and emitting kernels for the various backends).
 */
#ifndef INC_BRTKERNEL_H
#define INC_BRTKERNEL_H

#include "dup.h"
#include "stemnt.h"
#include "b2ctransform.h"

typedef enum
{
    CODEGEN_CAL = 0,
    CODEGEN_NUM_TARGETS
} CodeGenTarget;

static const char *CODEGEN_TARGET_NAMES[CODEGEN_NUM_TARGETS] = {
    "cal"
};


class BRTKernelCode;
typedef Dup < BRTKernelCode > DupableBRTKernelCode;


class BRTKernelCode:public DupableBRTKernelCode
{
public:
    BRTKernelCode(const FunctionDef & _fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTKernelCode::BRTKernelCode(%p)\n", &_fDef);
            fDef = (FunctionDef *) (&_fDef);
        };

    /*
     * Deleting fDef, even though it's dupped, appears to free memory that
     * has already been freed.  I'm suspicious that one of the dup methods
     * doesn't go deep enough, but haven't chased it down.  --Jeremy.
     */
    virtual ~BRTKernelCode()
        {
            LOG(LOG_FN_ENTRY, " BRTKernelCode::~BRTKernelCode()\n");
        };

    BRTKernelCode *dup0() const = 0;
    virtual void printCode(std::ostream & out) const = 0;
    virtual void printInnerCode(std::ostream & out) const = 0;
    friend std::ostream & operator<<(std::ostream & o, const BRTKernelCode & k);

    FunctionDef *fDef;
    bool standAloneKernel() const;

    virtual void onlyPrintInner(std::ostream &out) const
        {
            LOG(LOG_FN_ENTRY, " BRTKernelCode::onlyPrintInner(%p)\n", &out);
        }
};

class BRTGPUKernelCode:public BRTKernelCode
{
public:
    BRTGPUKernelCode(const FunctionDef & fDef);
    ~BRTGPUKernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTGPUKernelCode::~BRTGPUKernelCode()\n");
        }

    void printInnerCode(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void printILCode(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

    /* static so it can be passed as a findExpr() callback */
    static Expression *ConvertGathers(Expression * e);


    void printCodeForType(std::ostream & out, CodeGenTarget target) const;
    virtual BRTKernelCode *dup0() const = 0;
    virtual void printCode(std::ostream & out) const = 0;
};

#if BROOK_TO_IL_CODE_GEN_PATH
class BRTFP30KernelCode:public BRTGPUKernelCode
{
public:
    BRTFP30KernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTFP30KernelCode::BRTFP30KernelCode()\n");
        }
    ~BRTFP30KernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTFP30KernelCode::~BRTFP30KernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTKernelCode::dup0()\n");
            return new BRTFP30KernelCode(*this->fDef);
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    void printCode(std::ostream & out) const;
#endif
};

class BRTFP40KernelCode:public BRTGPUKernelCode
{
public:
    BRTFP40KernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTFP40KernelCode::BRTFP40KernelCode()\n");
        }
    ~BRTFP40KernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTFP40KernelCode::~BRTFP40KernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, "BRTFP40KernelCode::dup0()\n");
            return new BRTFP40KernelCode(*this->fDef);
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    void printCode(std::ostream & out) const;
#endif 
};

class BRTARBKernelCode:public BRTGPUKernelCode
{
public:
    BRTARBKernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTARBKernelCode::BRTARBKernelCode(%p)\n", &fDef);
        }
    ~BRTARBKernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTARBKernelCode::~BRTARBKernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTKernelCode::dup0()\n");
            return new BRTARBKernelCode(*this->fDef);
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    void printCode(std::ostream & out) const;
#endif 
};

class BRTPS20KernelCode:public BRTGPUKernelCode
{
public:
    BRTPS20KernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTPR20KernelCode::BRTPS20KernelCode(%p)\n", &fDef);
        }
    ~BRTPS20KernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS20KernelCode::~BRTPS20KernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTKernelCode::dup0()\n");
            return new BRTPS20KernelCode(*this->fDef);
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    void printCode(std::ostream & out) const;
#endif 
};

class BRTPS2BKernelCode:public BRTGPUKernelCode
{
public:
    BRTPS2BKernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTPS2BKernelCode::BRTPS2BKernelCode(%p)\n", &fDef);
        }
    ~BRTPS2BKernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS2BKernelCode::~BRTPS2BKernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTPS2BKernelCode::dup0()\n");
            return new BRTPS2BKernelCode(*this->fDef);
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    void printCode(std::ostream & out) const;
#endif 
};

class BRTPS2AKernelCode:public BRTGPUKernelCode
{
public:
    BRTPS2AKernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTPS2AKernelCode::BRTPS2AKernelCode(%p)\n");
        }
    ~BRTPS2AKernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS2AKernelCode::~BRTPS2AKernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTPS2AKernelCode::dup0()\n");
            return new BRTPS2AKernelCode(*this->fDef);
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    void printCode(std::ostream & out) const;
#endif 
};

class BRTPS30KernelCode:public BRTGPUKernelCode
{
public:
    BRTPS30KernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTPS30KernelCode::BRTPS30KernelCode(%p)\n", &fDef);
        }
    ~BRTPS30KernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTPS30KernelCode::~BRTPS30KernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTPS30KernelCode::dup0()\n");
            return new BRTPS30KernelCode(*this->fDef);
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    void printCode(std::ostream & out) const;
#endif 
};

#endif

class BRTCALKernelCode:public BRTGPUKernelCode
{
public:
    BRTCALKernelCode(const FunctionDef & fDef):BRTGPUKernelCode(fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTCALKernelCode::BRTCALKernelCode(%p)\n", &fDef);
        }
    ~BRTCALKernelCode()
        {                           /* Nothing, ~BRTKernelCode() does all the work */
            LOG(LOG_FN_ENTRY, " BRTCALKernelCode::~BRTCALKernelCode()\n");
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, "BRTCALKernelCode::dup0()\n");
            return new BRTCALKernelCode(*this->fDef);
        }
    void printCode(std::ostream & out) const;
};


class BRTCPUKernelCode:public BRTKernelCode
{
public:

    BRTCPUKernelCode(const FunctionDef & _fDef):BRTKernelCode(_fDef)
        {
            LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::BRTCPUKernelCode(%p)\n", &_fDef);
            fDef = (FunctionDef *) _fDef.dup0();
        }
    ~BRTCPUKernelCode()
        {
            LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::~BRTCPUKernelCode()\n");
            delete fDef;
        }

    BRTKernelCode *dup0() const
        {
            LOG(LOG_FN_ENTRY, " BRTCPUKernelCode::dup0()\n");
            return new BRTCPUKernelCode(*this->fDef);
        }

    void printCode(std::ostream & out) const;
    void onlyPrintInner(std::ostream & out) const;
    void printInnerCode(std::ostream & out) const;

    //! Functions to print multithreaded code
    void printMultiThreadedCode(std::ostream & out) const;
    void printThreadConditionVariable(std::ostream& out) const;
    void printThreadedInnerCode(std::ostream & out) const;
    void printThreadDataStruct(std::ostream& out) const;
    void printThreadEntryPointFunc(std::ostream& out) const;
};

#endif /* INC_BRTKERNEL_H */
