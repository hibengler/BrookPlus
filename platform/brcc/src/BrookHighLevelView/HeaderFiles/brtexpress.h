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
 * brtexpress.h
 * Gather, stream, and indexof exprs get transformed into these
 */

#ifndef BRTEXPRESS_H
#define BRTEXPRESS_H

#include "logger.hpp"
#include "express.h"
#include "brtdecl.h"
#include "cgTool.h"
#include "cgSemantic.h"


    class BrtGatherExpr:public Expression
{
public:
    BrtGatherExpr(const IndexExpr * expr);
    BrtGatherExpr(const Location l):Expression(ET_BrtGatherExpr, l) {}

    ~BrtGatherExpr()
        {                           /* do nothing */
            LOG(LOG_FN_ENTRY, " ~BrtGatherExpr()\n");
        }

    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

    Expression *dup0() const;

    BaseType *getBaseType() const
        {
            LOG(LOG_FN_ENTRY, "BrtGatherExpr::getBaseType()\n");
            // we need the type of the gather variable
            assert(base->etype == ET_Variable);
            Variable *v = (Variable *) base;
            assert(v->name);
            assert(v->name->entry);
            assert(v->name->entry->IsParamDecl());
            assert(v->name->entry->uVarDecl);
            assert(v->name->entry->uVarDecl->form);
            assert(v->name->entry->uVarDecl->form->isArray());
            ArrayType *a = (ArrayType *) v->name->entry->uVarDecl->form;
            BaseType *b = a->getBase();

            if       (b->typemask & BT_UserType)
            {
                BaseType *tmp;

                // If we have a typedef of a base type, then we need to change the
                // gather to reflect the base type.  If it's a struct though, we want
                // the struct-derived special gather codegen will generate.

                tmp = b->typeName->entry->uVarDecl->form->getBase();
                if       ((tmp->typemask & BT_Struct) == 0)
                    b = tmp;
            }

            return b;
        }

    virtual Type *getType() const
        {
            LOG(LOG_FN_ENTRY, "BrtGatherExpr::getType()\n");
            BaseType *bt = getBaseType();
            if       (bt)
                return bt;
            else
            {
                assert(type);
                return type;
            }
        }

    int ndims;

    Variable *base;
    ExprVector dims;
#if BROOK_TO_IL_CODE_GEN_PATH
private:
    void printILGatherFloat(std::ostream & outs, bril::store * dst, bril::store * indexVar, u32 sampler) const;
    void printILGatherIndex(std::ostream & outs, bril::store * indexVar) const;
    void printIL(std::ostream & out, bril::store * r, int *sampler_offset) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH
};


class BrtStreamInitializer:public Expression
{
public:

    BrtStreamInitializer(const BrtStreamType * t, const Location l);
    ~BrtStreamInitializer();

    Expression *dup0() const;
    void print(std::ostream & out) const;

    BrtStreamType *t;
    Location l;
};


SymEntry *find_parameter_in_current_kernel_by_name(std::string const &pname);

class BrtIndexofExpr:public Expression
{
public:
    BrtIndexofExpr(Variable * operand, const Location l);
    ~BrtIndexofExpr();

    int precedence() const
        {
            return 15;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

    void findExpr(fnExprCallback cb);

    Variable *expr;             // want the size of this expression.

    virtual Type *getType() const
        {
            LOG(LOG_FN_ENTRY, "BrtIndexOfExpr::getType()\n");
            if (!type)
                *(Type **) & type = new BaseType(BT_Float4);

            return type;
        }
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual bril::store * get_il_store() const
        {
            LOG(LOG_FN_ENTRY, "BrtIndexofExpr::get_il_store()\n");
            assert(expr);
            assert(expr->name);

            // Map the thing we are getting the index of to the parameter that holds the index.
            assert(find_parameter_in_current_kernel_by_name("__indexof_" + expr->name->name)->get_il_store());
            return find_parameter_in_current_kernel_by_name("__indexof_" + expr->name->name)->get_il_store();
        }
#endif //BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

};

class BrtInstanceExpr:public Expression
{
public:
    BrtInstanceExpr(const Location l);
    BrtInstanceExpr(Variable * operand, const Location l);
    ~BrtInstanceExpr();

    int precedence() const
        {
            return 15;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;

    void findExpr(fnExprCallback cb);

    Variable *expr;             // want the size of this expression.

    virtual Type *getType() const
        {
            LOG(LOG_FN_ENTRY, "BrtInstanceExpr::getType()\n");
            if (!type)
                *(Type **) & type = new BaseType(BT_Int4);

            return type;
        }

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

};

class BrtInstanceInGroupExpr:public Expression
{
public:
    BrtInstanceInGroupExpr(const Location l);
    BrtInstanceInGroupExpr(Variable * operand, const Location l);
    ~BrtInstanceInGroupExpr();

    int precedence() const
        {
            return 15;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;

    void findExpr(fnExprCallback cb);

    Variable *expr;             // want the size of this expression.

    virtual Type *getType() const
        {
            LOG(LOG_FN_ENTRY, "BrtInstanceInGroupExpr::getType()\n");
            if (!type)
                *(Type **) & type = new BaseType(BT_Int4);

            return type;
        }

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

};

class BrtSyncGroupExpr:public Expression
{
public:
    BrtSyncGroupExpr(const Location l);
    ~BrtSyncGroupExpr();

    int precedence() const
        {
            return 15;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;

    void findExpr(fnExprCallback cb);

    virtual Type *getType() const
        {
            LOG(LOG_FN_ENTRY, "BrtSyncGroupExpr::getType()\n");
            if (!type)
                *(Type **) & type = new BaseType(BT_Void);

            return type;
        }

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

};

#endif /* BRTEXPRESS_H */
