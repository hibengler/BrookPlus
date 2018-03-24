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
 * brtdecl.h  --
 * Stream type and gather type are operated on here
 */

#ifndef    BRTDECL_H
#define    BRTDECL_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>

#include "logger.hpp"

#include "symbol.h"
#include "callback.h"
#include "location.h"

#include "dup.h"

#include "decl.h"
#include "express.h"


                    class BrtStreamType:public Type
{
public:
    ~BrtStreamType();

    Type *dup0() const = 0;

    bool printStructureStreamHelperType(std::ostream &out, const std::string & name, bool raw) const
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::printStructureStreamHelperType(%p, %s, %d)\n", &out, name.c_str(), raw);
            return false;
        }
    void printBase(std::ostream & out, int level) const
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::printBase(%p, %d)\n", &out, level);
            assert(0);
        }
    void printBefore(std::ostream & out, Symbol * name, int level) const
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::printBefore(%p, %p, %d)\n", &out, name, level);
            assert(0);
        }
    void printAfter(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::printAfter(%p)\n", &out);
            assert(0);
        }

    void printType(std::ostream & out, Symbol * name, bool showBase, int level, bool raw = false) const = 0;
    void printForm(std::ostream & out) const = 0;
    virtual void printInitializer(std::ostream & out) const = 0;

    void findExpr(fnExprCallback cb);
    bool lookup(Symbol * sym) const
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::lookup(%p)\n", sym);
            return base ? base->lookup(sym) : false;
        }

    TypeQual getQualifiers(void) const
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::getQualifiers()\n");
            return base->getQualifiers();
        }
    BaseType *getBase(void)
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::getBase()\n");
            return base;
        }

    BaseType *base;
    ExprVector dims;
    bool isIterator;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void allocateILStorage(bril::store * r) const
        {
            LOG(LOG_FN_ENTRY, "BitStreamType::allocateILStorage(%p)\n", r);
            ((BrtStreamType *) this)->getBase()->allocateILStorage(r);
        }
#endif // BROOK_TO_IL_CODE_GEN_PATH
protected:
    /*
     * All constructors are protected because no one should be instantiating
     * these directly.  Use the children.
     */
    BrtStreamType(const ArrayType * t);
    BrtStreamType(const BaseType * _base, const ExprVector _dims);      /* For dup0 */
};


class BrtStreamParamType:public BrtStreamType
{
public:
    BrtStreamParamType(const ArrayType * t):BrtStreamType(t)
        {
            LOG(LOG_FN_ENTRY, "BrtStreamParamType::BrtStreamParamType(%p)\n", t);
        };

    Type *dup0() const
        {
            LOG(LOG_FN_ENTRY, "BrtStreamParamType::dup0()\n");
            return new BrtStreamParamType(base, dims);
        }

    void printType(std::ostream & out, Symbol * name, bool showBase, int level, bool raw = false) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const
        {
            LOG(LOG_FN_ENTRY, "BrtStreamType::printBefore(%p, %p, %d)\n", &out, name, level);
        }
    void printAfter(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "BrtStreamType::printAfter(%p)\n", &out);
        }

    void printForm(std::ostream & out) const;
    void printInitializer(std::ostream & out) const;
    void printBase(std::ostream & out, int level) const;

protected:
    BrtStreamParamType(const BaseType * _base, const ExprVector _dims):BrtStreamType(_base, _dims)
        {
        }                           /* Only for dup0() */
};


class BrtInitializedStreamType:public BrtStreamType
{
public:
    BrtInitializedStreamType(const ArrayType * t):BrtStreamType(t)
        {
            LOG(LOG_FN_ENTRY, "BrtInitializedStreamType::BrtInitializedStreamType(%p)\n", t);
        };

    Type *dup0() const
        {
            LOG(LOG_FN_ENTRY, "BrtInitializedStreamType::dup0()\n");
            return new BrtInitializedStreamType(base, dims);
        }

    void printType(std::ostream & out, Symbol * name, bool showBase, int level, bool raw = false) const;
    void printForm(std::ostream & out) const;
    void printInitializer(std::ostream & out) const;

protected:
    BrtInitializedStreamType(const BaseType * _base, const ExprVector _dims):BrtStreamType(_base, _dims)
        {
            LOG(LOG_FN_ENTRY, "BrtInitializedStreamType::BrtInitializedStreamType(%p, %p)\n", _base, &_dims);
        }                           /* Only for dup0() */
};

class BrtIterType:public BrtStreamType
{
public:
    BrtIterType(const ArrayType * stream, const FunctionCall * f);
    ~BrtIterType();

    Type *dup0() const
        {
            LOG(LOG_FN_ENTRY, "BrtIterType::dup0()\n");
            return new BrtIterType(base, dims, args);
        }

    void printType(std::ostream & out, Symbol * name, bool showBase, int level, bool raw = false) const;
    void printForm(std::ostream & out) const;
    void printInitializer(std::ostream & out) const;

    void findExpr(fnExprCallback cb);

    ExprVector args;

protected:
    BrtIterType(const BaseType * _base, const ExprVector _dims, const ExprVector _args);     /* Only for dup0() */
};

class CPUGatherType
{
    mutable bool raw;
    // used instead of changing argument signature of modified printBefore
public:
    Type * at;
    Type *subtype;
    bool copy_on_write;
    unsigned int dimension;
    CPUGatherType(ArrayType & t, bool copy_on_write);
    virtual ~CPUGatherType()
        {
            LOG(LOG_FN_ENTRY, "CPUGatherType::~CPUGatherType()\n");
        }
    Type *dup0() const;
    virtual Type **getSubType()
        {
            LOG(LOG_FN_ENTRY, "CPUGatherType::getSubType()\n");
            return &subtype;
        }
    void printType(std::ostream & out, Symbol * name, bool showBase, int level, bool raw = false) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const;
    void printAfter(std::ostream & out) const;
    void printSubtype(std::ostream & out, Symbol * name, bool showBase, int level) const;
};

class BrtKernelType:public FunctionType
{
public:
    BrtKernelType(FunctionType * functionType);
    ~BrtKernelType();
    Type *dup0() const
        {
            LOG(LOG_FN_ENTRY, "BrtKernelType::dup0()\n");
            return new BrtKernelType(_functionType);
        }

private:
    Type *convertArgumentType(Type *);
    FunctionType *_functionType;
};

#endif /* BRTDECL_H */
