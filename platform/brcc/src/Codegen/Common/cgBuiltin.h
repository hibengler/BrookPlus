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

#ifndef INC_CG_BUILTIN_H
#define INC_CG_BUILTIN_H

#include "logger.hpp"
#include "decl.h"
#include "main.h"

// ----------------------------------------------------------------------------

// We take an approach to manually construct the built-in function property table.
// An alternative approach would be to write out the declaration
// of the built-in function and let the parser parse it and construct the proper symbol
// entry. The reason that we don't that this approach is that the parser is a C parser,
// which doesn't support function overloading needed for the built-ins.

// ----------------------------------------------------------------------------
//! An implementation of a builtin function. 
class CgBuiltinImp
{
public:
    CgBuiltinImp();
    ~CgBuiltinImp();

    //! Returns the total number of arguments
    int getArgTot() {return argTotal;}

    //! Adds the array arguments to implementation
    void addArgs(int argCount, BaseType** args);

    //! Sets the return type of implementation
    void setReturnType(BaseType* returnType);

    //! Retruns the Nth argument
    BaseType *getArg(int nthArg);

    //! Returns the return type of implementations
    BaseType *getReturnType();

    //! Set next implementation
    void setNext(CgBuiltinImp *nextImp){next = nextImp;}

    //! Set next implementation
    CgBuiltinImp *getNext(){return next;}

private:
    //! array of argument types
    BaseType **argType;

    //! Arguments count
    int argTotal;

    //! Return type of function implemenatation
    BaseType *retType;

    //! Pointer to next implementation
    CgBuiltinImp *next;
};

// ----------------------------------------------------------------------------

class CgBuiltin
{
public:
    CgBuiltin(std::string n)
        :name(n), impTotal(0), impList(NULL)
    {
    }

    ~CgBuiltin()
    {
        LOG(LOG_FN_ENTRY, " ~CgBuiltin()\n");
        delete impList;
    }

    std::string& getName()
    {
        LOG(LOG_FN_ENTRY, " getName()\n");
        return name;
    }
    //! Add the implementation
    int addImplementation(int argTotal, BaseType *returnType, BaseType **args);

    //! Get total no of implementations added
    int getImpCount(){return impTotal;}

    //! Get nth implementation
    CgBuiltinImp* getImplementation(int n);
private:
    std::string name;
    CgBuiltinImp *impList;
    int impTotal;
};

// ----------------------------------------------------------------------------

typedef std::map < std::string, CgBuiltin* >::iterator CgBuiltinIter;
class CgBuiltinMap : public std::map < std::string, CgBuiltin* >
{
public:
    void addBuiltin(CgBuiltin *val);

    CgBuiltin * getBuiltin(const std::string &name);
private:
};

// ----------------------------------------------------------------------------
extern void builtinInit(void);
extern void builtinFinit(void);
extern CgBuiltinMap hlslBuiltin;

#define findBuiltin(n) hlslBuiltin.getBuiltin(n)
// ----------------------------------------------------------------------------

#endif // INC_CG_BUILTIN_H
