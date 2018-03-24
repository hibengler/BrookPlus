/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the
contents of this directory.

****************************************************************************/

/*
 * brtscatter.h
 *
 *      Utility functions to print out the scatter fuctor for execution on CPU.
 */
#ifndef    _BRTSCATTER_H_
#define    _BRTSCATTER_H_

#include "stemnt.h"
#include "b2ctransform.h"
extern Expression *ConvertToBrtScatterCalls(Expression * e);


class BRTScatterDef:public FunctionDef
{
  public:
    BRTScatterDef(const FunctionDef & fDef);
         ~BRTScatterDef();

    /* Pass ourselves (as a FunctionDef) to our own constructor */
    Statement *dup0() const
    {
        LOG(LOG_FN_ENTRY, "BRTScatterDef::dup0()\n");
        return new BRTScatterDef(*static_cast < const FunctionDef * >(this));
    };
    void print(std::ostream & out, int level) const;
};

#endif /* STEMNT_H */
