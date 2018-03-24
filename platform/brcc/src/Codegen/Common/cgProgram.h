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

#ifndef INC_CG_PROGRAM_H
#define INC_CG_PROGRAM_H

//#include <map>
#include <string.h>

#include "cgTool.h"
#include "project.h"
#include "stemnt.h"

#include "cgResource.h"
#include "cgKernel.h"
#include "cgFunction.h"

typedef std::map <std::string, CgFunction*> CgFuncMap;
typedef std::map <std::string, CgFunction*>::iterator CgFuncIter;

// ----------------------------------------------------------------------------

typedef void (*fnCgFunctionCallback) (CgFunction*, void*);

/**
 *
 */
class CgProgram
{
public:
    CgProgram(TransUnit *tu);
    ~CgProgram();

    void codegen(std::ostream &cppout, std::ostream &kernelDeclHeader, std::ostringstream &nonKernellDecls, 
                 std::ostream &headerout, std::string &headername, std::string & kernelDeclFileName);

    void addCgFunction(CgFunction*, std::string&);
    CgFunction *getCgFunction(std::string&);
    void applyCgFunction(fnCgFunctionCallback, void*);

    TransUnit *getTransUnit() {return transUnit;}

    int semanticCheck();


private:
    void irEdit_1();
    void addCgFunctionCAL();

private:
    /// record all CgFunction
    TransUnit *transUnit;
    CgFuncMap cgFuncTbl;
    CgFunction *firstFunc;
    CgFunction *lastFunc;
};

#endif // INC_CG_PROGRAM_H
