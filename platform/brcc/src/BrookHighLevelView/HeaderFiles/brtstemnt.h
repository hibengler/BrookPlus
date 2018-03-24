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
 * brtstemnt.h
 *
 *      Brook extensions to cTool's stemnt.h for kernel definitions.
 */
#ifndef INC_BRTSTEMNT_H
#define INC_BRTSTEMNT_H

#include "logger.hpp"
#include "stemnt.h"
#include "b2ctransform.h"

//#define KEEP_VOUT

class BRTKernelDef:public FunctionDef
{
  public:
    BRTKernelDef(const FunctionDef & fDef);

    /* Pass ourselves (as a FunctionDef) to our own constructor */
    Statement *dup0() const
    {
        LOG(LOG_FN_ENTRY, "BRTKernelDef::dup0()\n");
        return new BRTKernelDef(*static_cast < const FunctionDef * >(this));
    };
    void print(std::ostream & out, int level) const;

    void printStub(std::ostream & out, std::ostream &kernelDeclHeader) const;
#if KEEP_VOUT
    virtual void PrintVoutPrefix(std::ostream & out) const;
    virtual void PrintVoutDimensionalShift(std::ostream & out, Decl * decl, unsigned int dim) const;
    virtual void PrintVoutPostfix(std::ostream & out) const;
#endif // KEEP_VOUT

    virtual bool CheckSemantics(void) const;
};

class BRTMapKernelDef:public BRTKernelDef
{
  public:
    BRTMapKernelDef(const FunctionDef & fDef):BRTKernelDef(fDef)
    {
        LOG(LOG_FN_ENTRY, "BRTMapKernelDef::BRTMapKernelDef(%p)\n", &fDef);
        if (!CheckSemantics())
            assert(false);
    }

    /* Pass ourselves (as a FunctionDef) to our own constructor */
    Statement *dup0() const
    {
        LOG(LOG_FN_ENTRY, "BRTMapKernelDef::dup0()\n");
        return new BRTMapKernelDef(*static_cast < const FunctionDef * >(this));
    };

    bool CheckSemantics(void) const;
};

class BRTReduceKernelDef:public BRTKernelDef
{
  public:
    BRTReduceKernelDef(const FunctionDef & fDef):BRTKernelDef(fDef)
    {
        LOG(LOG_FN_ENTRY, "BRTReduceKernelDef::BRTReduceKernelDef(%p)\n", &fDef);
        if (!CheckSemantics())
            assert(false);
    }

    /* Pass ourselves (as a FunctionDef) to our own constructor */
    Statement *dup0() const
    {
        LOG(LOG_FN_ENTRY, "BRTReduceKernelDef::dup0()\n");
        return new BRTReduceKernelDef(*static_cast < const FunctionDef * >(this));
    };

    bool CheckSemantics(void) const;
};

#endif /* INC_BRTSTEMNT_H */
