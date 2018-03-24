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

/*  o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o

CTool Library
Copyright (C) 1998-2001     Shaun Flisakowski

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

/*  o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o
    o+
    o+     File:         express.h
    o+
    o+     Programmer:   Shaun Flisakowski
    o+     Date:         Aug 9, 1998
    o+
    o+     A high-level view of expressions.
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

#ifndef INC_EXPRESS_H
#define INC_EXPRESS_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>

#include "logger.hpp"
#include "symbol.h"
#include "decl.h"
#include "callback.h"
#include "location.h"
#include "utype.h"

#include "dup.h"
#include "cgTool.h"
#define SEMANTIC_CHECK

class Decl;
class Type;

typedef Expression *(*fnExprCallback) (Expression * expr);


enum ExpressionType
{
    ET_VoidExpr = 0,            // No value

    ET_Constant,
    ET_Variable,
    ET_FunctionCall,

    ET_AssignExpr,
    ET_RelExpr,

    ET_UnaryExpr,
    ET_BinaryExpr,
    ET_TrinaryExpr,             // x ? y : z

    ET_CastExpr,                // (type) x
    ET_SizeofExpr,              // sizeof(type) -or- sizeof(expr)
    ET_IndexExpr,               // x[y]...[]

    ET_ConstructorExpr,         // float2(x, y)

    ET_BrtStreamInitializer,    // __BrtCreateStream(...)
    ET_BrtGatherExpr,           // For inside kernel
    ET_BrtIndexofExpr,
    ET_BrtInstanceExpr,
    ET_BrtInstanceInGroupExpr,
    ET_BrtSyncGroupExpr

};


enum ConstantType
{
    CT_Char,
    CT_Int,
    CT_UInt,
    CT_Float,
    CT_Double,

    CT_String,
    CT_Array,                   // For an array initializer

    CT_Enum                     // A single enumeration constant
};


enum AssignOp
{
    AO_Equal,                   //  =

    AO_PlusEql,                 // +=
    AO_MinusEql,                // -=
    AO_MultEql,                 // *=
    AO_DivEql,                  // /=
    AO_ModEql,                  // %=

    AO_ShlEql,                  // <<=
    AO_ShrEql,                  // >>=

    AO_BitAndEql,               // &=
    AO_BitXorEql,               // ^=
    AO_BitOrEql                 // |=
};

enum RelOp
{
    RO_Equal,                   // ==
    RO_NotEqual,                // !=

    RO_Less,                    // <
    RO_LessEql,                 // <=
    RO_Grtr,                    // >
    RO_GrtrEql                  // >=
};

enum UnaryOp
{
    UO_Plus = 0,                // +
    UO_Minus,                   // -
    UO_BitNot,                  // ~
    UO_Not,                     // !

    UO_PreInc,                  // ++x
    UO_PreDec,                  // --x
    UO_PostInc,                 // x++
    UO_PostDec,                 // x--

    UO_AddrOf,                  // &
    UO_Deref                    // *
};

enum BinaryOp
{
    BO_Plus,                    // +
    BO_Minus,                   // -
    BO_Mult,                    // *
    BO_Div,                     // /
    BO_Mod,                     // %

    BO_Shl,                     // <<
    BO_Shr,                     // >>
    BO_BitAnd,                  // &
    BO_BitXor,                  // ^
    BO_BitOr,                   // |

    BO_And,                     // &&
    BO_Or,                      // ||

    BO_Comma,                   // x,y

    BO_Member,                  // x.y
    BO_PtrMember,               // x->y

    BO_Assign,                  // An AssignExpr
    BO_Rel                      // A RelExpr
};



class Expression;
typedef Dup < Expression > DupableExpression;

typedef enum
{
    ExpParent = 0,
    StmtParent,
    DeclParent,
    TypeParent,
    EnumdefParent,
} ExpParentKind;

class Expression:public DupableExpression
{
public:
    Expression(ExpressionType et, const Location l);
    virtual ~ Expression();

    virtual int precedence() const
        {
            return 16;
        }

    virtual Expression *dup0() const;

    virtual void print(std::ostream & out) const;
    virtual void printExprWithTypeInfo(std::ostream& out) const;
    virtual void printSubExprInfo(std::ostream& out) const
    {
    }
    virtual void findExpr(fnExprCallback cb)
        {
            LOG(LOG_FN_ENTRY, "Expression::findExpr(%p)\n", cb);
            (cb) (this);
        }
    virtual void replaceExpr(Expression * oldExp, Expression * newExp)
        {
            LOG(LOG_FN_ENTRY, "Expression::replaceExpr(%p, %p)\n", oldExp, newExp);
            assert(0);
        }
    void replace(Expression * newExp);

    virtual bool isSimple()
        {
            return false;
        }

    ExpressionType etype;
    Location location;

    // For type-checking. This is not always set during the parse...
    Type *type;

    // ... which is what this function is for.
    virtual Type *getType() const
        {
            static BaseType* defaultType = NULL;
            LOG(LOG_FN_ENTRY, "Expression::getType()\n");
            //assert( type );
            if (type)
                return type->getRealType();

            LOG(LOG_ERROR, "%s: %d -> Type information missing\n", __FILE__, __LINE__);
            //! Default type becomes int type
            return defaultType ? defaultType : defaultType = new BaseType(BT_Int);
        }

    // For constructing a list of expressions (in parser).
    Expression *next;

    friend std::ostream & operator<<(std::ostream & out, const Expression & expr);
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out) const;


    virtual bril::store * get_il_store() const
        {
            LOG(LOG_FN_ENTRY, "Expression::get_il_store()\n");
            getType()->allocateILStorage(&il_store);
            return &il_store;
        }

    virtual std::string getILRegName(bril::reg_style style) const
        {
            LOG(LOG_FN_ENTRY, "Expression::getILRegName(%d)\n", style);
            getType()->allocateILStorage(get_il_store());
            return bril::get_gpr_name(get_il_store(), style);
        }
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    virtual void semanticCheck(bool isKernel)
        {
            LOG(LOG_FN_ENTRY, "semanticCheck()\n");
        }
#endif

    Expression *getRootExp();
    Expression *getParentExp();
    Decl *getParentDcl();
    Statement *getParentStmt();
    Type *getParentType();
    EnumDef *getParentEnumdef();

    void setParent(Expression *);
    void setParent(Decl *);
    void setParent(Statement *);
    void setParent(Type *);
    void setParent(EnumDef *);

#if BROOK_TO_IL_CODE_GEN_PATH
    mutable bril::store il_store;
#endif
protected:
    union
    {
        Expression *parentExp;
        Statement *parentStmt;
        Decl *parentDcl;
        Type *parentTyp;
        EnumDef *parentEnm;
    };
    ExpParentKind parentKind;
};



typedef std::vector < Expression * >ExprVector;


class Constant:public Expression
{
public:
    Constant(ConstantType ct, const Location l);
    virtual ~ Constant();

    virtual Expression *dup0() const = 0;
    virtual void print(std::ostream & out) const = 0;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out) const = 0;
#endif // BROOK_TO_IL_CODE_GEN_PATH

    virtual bool isSimple()
        {
            return true;
        }

    ConstantType ctype;
};

//! To indicate whether numeric constant is in decimal, hex or octal form
typedef enum _NumericForm
{
    BR_Decimal_Form = 0,
    BR_HexDecimal_Form,
    BR_Octal_Form
}NumericForm;

class IntConstant:public Constant
{
public:
    IntConstant(long val, const Location l, NumericForm printForm = BR_Decimal_Form);
    virtual ~ IntConstant();

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;


    virtual bril::store * get_il_store() const
        {
            LOG(LOG_FN_ENTRY, "IntConstant::get_il_store()\n");
            bril::get_scalar_constant_store(&il_store, f32(lng));
            return &il_store;
        }

    virtual std::string getILRegName(bril::reg_style style) const
        {
            LOG(LOG_FN_ENTRY, "IntConstant::getILRegName(%d)\n");
            assert(style == bril::rs_src_sel);
            return bril::get_scalar_constant_name(f32(lng));
        }
#endif //BROOK_TO_IL_CODE_GEN_PATH
    long lng;
    //! Indicates printed form in code generation
    NumericForm numericForm;
};


class UIntConstant:public Constant
{
public:
    UIntConstant(unsigned long val, const Location l, NumericForm printForm = BR_Decimal_Form);
    virtual ~ UIntConstant();

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH

    unsigned long ulng;

    //! Indicates printed form in code generation
    NumericForm numericForm;
};


class FloatConstant:public Constant
{
public:
    FloatConstant(std::string strVal, const Location l);
    virtual ~ FloatConstant();

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;


    virtual bril::store * get_il_store() const
        {
            LOG(LOG_FN_ENTRY, "FloatConstant::get_il_store()\n");
            bril::get_scalar_constant_store(&il_store, f32(doub));
            return &il_store;
        }

    virtual std::string getILRegName(bril::reg_style style) const
        {
            LOG(LOG_FN_ENTRY, "getILRegName(%d)\n", style);
            assert(style == bril::rs_src_sel);
            return bril::get_scalar_constant_name(f32(doub));
        }
#endif // BROOK_TO_IL_CODE_GEN_PATH
    std::string strVal;
};

class DoubleConstant:public Constant
{
  public:
    DoubleConstant(std::string strVal, const Location l);
    virtual ~ DoubleConstant();

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;

    
    virtual bril::store * get_il_store() const;
    virtual std::string getILRegName(bril::reg_style style) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

    std::string strVal;
};

class CharConstant:public Constant
{
public:
    CharConstant(char chr, const Location l, bool isWide = false);
    ~CharConstant();

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH

    char ch;                    // the character
    bool wide;                  // wide char?
};


class StringConstant:public Constant
{
public:
    StringConstant(const std::string & str, const Location l, bool isWide = false);
    ~StringConstant();

    int length() const;

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    std::string buff;           // dynamic string storage
    bool wide;                  // wide string?
};


class ArrayConstant:public Constant
{
public:
    ArrayConstant(const Location l);
    ~ArrayConstant();

    void addElement(Expression * expr);

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
    void print_il(std::ostream & out, Type * type) const;
    virtual std::string getILRegName(bril::reg_style style) const
        {
            LOG(LOG_FN_ENTRY, "ArrayConstant::getILRegName(%d)\n", style);
            // Should be allocated by print_il!
            return bril::get_gpr_name(&il_store, style);
        }
#endif // BROOK_TO_IL_CODE_GEN_PATH


#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif
    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    ExprVector items;           // Array of whatever, including another array.
};


class EnumConstant:public Constant
{
public:
    EnumConstant(Symbol * nme, Expression * val, const Location l);
    ~EnumConstant();

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

    Symbol *name;               // The constant itself.
    Expression *value;          // It's value.
};


class Variable:public Expression
{
public:
    Variable(Symbol * varname, const Location l);
    ~Variable();

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
    virtual bril::store * get_il_store() const
        {
            LOG(LOG_FN_ENTRY, "get_il_store()\n");
            return name->get_il_store();
        }

    virtual std::string getILRegName(bril::reg_style style) const
        {
            LOG(LOG_FN_ENTRY, "getILRegName(%d)\n", style);
            return name->getILRegName(style);
        }
#endif // BROOK_TO_IL_CODE_GEN_PATH

    virtual bool isSimple()
        {
            return true;
        }

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    Symbol *name;
};


class FunctionCall:public Expression
{
public:
    FunctionCall(Expression * func, const Location l);
    ~FunctionCall();

    int nArgs() const
        {
            LOG(LOG_FN_ENTRY, "FunctionCall::nArgs()\n");
            return (int) args.size();
        }

    void addArg(Expression * arg);
    void addArgs(Expression * argList);

    Expression *dup0() const;
    void print(std::ostream & out) const;
    void print_param(std::ostream & out, int idx) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
    virtual bril::store * get_il_store() const;
#endif // BROOK_TO_IL_CODE_GEN_PATH
    virtual Type *getType() const;

#ifdef SEMANTIC_CHECK
    void lvalueSemantics(Expression* leftExpr, int argPos, bool isKernel);
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *function;
    ExprVector args;
private:
#if BROOK_TO_IL_CODE_GEN_PATH
    bril::store getReturnILStore() const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

};


class UnaryExpr:public Expression
{
public:
    UnaryExpr(UnaryOp op, Expression * expr, const Location l);
    ~UnaryExpr();

    int precedence() const;

    Expression *operand() const
        {
            LOG(LOG_FN_ENTRY, "UnaryExpr::operand()\n");
            return _operand;
        }

    UnaryOp op() const
        {
            LOG(LOG_FN_ENTRY, "UnaryExpr::op()\n");
            return uOp;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    virtual void semanticCheck(bool isKernel);
#endif

    virtual Type *getType() const
        {
            LOG(LOG_FN_ENTRY, "UnaryExpr::getType()\n");
            if (type)
                return type->getRealType();
            else
                return _operand->getType();
        }

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    UnaryOp uOp;
    Expression *_operand;
};


class BinaryExpr:public Expression
{
public:
    BinaryExpr(BinaryOp op, Expression * lExpr, Expression * rExpr, const Location l);
    ~BinaryExpr();

    Expression *leftExpr() const
        {
            LOG(LOG_FN_ENTRY, "BinaryExpr::leftExpr()\n");
            return _leftExpr;
        }
    Expression *rightExpr() const
        {
            LOG(LOG_FN_ENTRY, "BinaryExpr::rightExpr()\n");
            return _rightExpr;
        }

    virtual int precedence() const;

    BinaryOp op() const
        {
            LOG(LOG_FN_ENTRY, "BinaryExpr::op()\n");
            return bOp;
        }

    virtual Expression *dup0() const;
    virtual void print(std::ostream & out) const;
    virtual void printSubExprInfo(std::ostream& out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out) const;

    virtual bril::store * get_il_store() const;

    virtual std::string getILRegName(bril::reg_style style) const
        {
            LOG(LOG_FN_ENTRY, "BinaryExpr::getILRegName(%d)\n", style);
            if (bOp == BO_Member)
            {
                if (leftExpr()->getType()->isStructure())
                    return bril::get_gpr_name(get_il_store(), bril::rs_src_sel);
                else
                    return bril::get_gpr_name(leftExpr()->get_il_store(),
                                              bril::rs_base) + "." + rightExpr()->getILRegName(bril::rs_base);
            }
            else
                return Expression::getILRegName(style);
        }

#endif // BROOK_TO_IL_CODE_GEN_PATH

    virtual Type *getType() const;



#ifdef SEMANTIC_CHECK
    bool isIntegerOp(){ return (bOp == BO_Mod || bOp == BO_Shl || bOp == BO_Shr || bOp == BO_BitAnd || bOp == BO_BitXor || bOp == BO_BitOr);}
    void semanticsForIntOps(BinaryOperandsDetails details);
    void semanticsForIntRealOps(BinaryOperandsDetails details);
    virtual void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    BinaryOp bOp;

    Expression *_leftExpr;
    Expression *_rightExpr;
};


class TrinaryExpr:public Expression
{
public:
    TrinaryExpr(Expression * cExpr, Expression * tExpr, Expression * fExpr, const Location l);
    ~TrinaryExpr();

    Expression *condExpr() const
        {
            LOG(LOG_FN_ENTRY, "TrinaryExpr::condExpr()\n");
            return _condExpr;
        }
    Expression *trueExpr() const
        {
            LOG(LOG_FN_ENTRY, "TrinaryExpr::trueExpr()\n");
            return _trueExpr;
        }
    Expression *falseExpr() const
        {
            LOG(LOG_FN_ENTRY, "TrinaryExpr::falseExpr()\n");
            return _falseExpr;
        }

    int precedence() const
        {
            return 3;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;
    void printSubExprInfo(std::ostream& out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    virtual void semanticCheck(bool isKernel);
#endif

    Type *getType() const;

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *_condExpr;
    Expression *_trueExpr;
    Expression *_falseExpr;
};

class IndexExpr;
class AssignExpr:public BinaryExpr
{
public:
    AssignExpr(AssignOp op, Expression * lExpr, Expression * rExpr, const Location l);
    ~AssignExpr();

    Expression *lValue() const
        {
            LOG(LOG_FN_ENTRY, "AssignExpr::lValue()\n");
            return leftExpr();
        }
    Expression *rValue() const
        {
            LOG(LOG_FN_ENTRY, "AssignExpr::rValue()\n");
            return rightExpr();
        }

    AssignOp op() const
        {
            LOG(LOG_FN_ENTRY, "AssignExpr::op()\n");
            return aOp;
        }

    int precedence() const
        {
            return 2;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;
    void printSubExprInfo(std::ostream& out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void lvalueSemantics(bool isKernel);
    void checkArrayIndexingSemantics(IndexExpr *index);
    void checkTypeSemantics(bool isKernel);
    void semanticCheck(bool isKernel);
#endif

    AssignOp aOp;
};


class RelExpr:public BinaryExpr
{
public:
    RelExpr(RelOp op, Expression * lExpr, Expression * rExpr, const Location l);
    ~RelExpr();

    int precedence() const;

    RelOp op() const
        {
            LOG(LOG_FN_ENTRY, "RelExpr::op()\n");
            return rOp;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;
    void printSubExprInfo(std::ostream& out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif


    RelOp rOp;
};


class CastExpr:public Expression
{
public:
    CastExpr(Type * typeExpr, Expression * operand, const Location l);
    ~CastExpr();

    Type *castType()
        {
            LOG(LOG_FN_ENTRY, "CastExpr::castType()\n");
            return castTo;
        }

    int precedence() const
        {
            return 14;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;
    void printSubExprInfo(std::ostream& out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Type *castTo;
    Expression *expr;           // The expression being cast.
};


class SizeofExpr:public Expression
{
public:
    SizeofExpr(Expression * operand, const Location l);
    SizeofExpr(Type * operand, const Location l);
    ~SizeofExpr();

    int precedence() const
        {
            return 15;
        }

    Expression *dup0() const;
    void print(std::ostream & out) const;

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);

    Type *sizeofType;           // want the size of this type.
    Expression *expr;           // want the size of this expression.
};

typedef enum _IndexAccessType
{
    IndexWriteAccess = 0,
    IndexReadAccess
}IndexAccessType;
class IndexExpr:public Expression
{
    // I should extend this to be able to handle
    // multi-dimensional arrays directly.

public:
    IndexExpr(Expression * _array, Expression * sub, const Location l);
    ~IndexExpr();

    //addSubscript( Expression *sub );
    Expression *subscript(int i);

    Expression *dup0() const;
    Expression *getArrayVariableExpr() const;
    int getIndexExprSize() const;
    void getSubscriptExprStr(std::ostream & out, ArrayType *arrayType) const;
    void getSubscriptExprStrFromNonCStyleIndexing(std::ostream & out, ArrayType *arrayType) const;

    void print(std::ostream & out) const;
    void printNonCStyleGatherIndexing(std::ostream & out) const;
    void printCStyleGatherIndexing(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream &out) const;
    virtual bril::store * get_il_store() const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

    virtual Type* getType() const;

#ifdef SEMANTIC_CHECK
    void checkSubscriptSemantics(bool isKernel);
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *array;
    Expression *_subscript;

    //! It becomes true if parent of this is not a IndexExpr expression
	bool isParent;

    //! specifies shared local array access type
    //! legal to use this only if array type is shared array type
    //! By default it is set to IndexReadAccess
    //! It is set to IndexWriteAccess if indexExpr is left expression of AssignExpr 
    IndexAccessType accessType;
};


class ConstructorExpr:public Expression
{
public:
    ConstructorExpr(BaseType * bType, Expression * exprs[], const Location l);
    ~ConstructorExpr();

    Expression *exprN(unsigned int n) const
        {
            LOG(LOG_FN_ENTRY, "ConstructorExpr::exprN(%d)\n", n);
            assert(n < _nExprs);
            return _exprs[n];
        }

    BaseType *bType() const
        {
            LOG(LOG_FN_ENTRY, "ConstructorExpr::bType()\n");
            return _bType;
        }

    virtual Expression *dup0() const;
    virtual void print(std::ostream & out) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    unsigned int _nExprs;
    BaseType *_bType;
    Expression **_exprs;
};



Expression *ReverseList(Expression * eList);

// For debugging, show the enumerated name.
const char *nameOfExpressionType(ExpressionType type);
const char *nameOfConstantType(ConstantType type);
const char *nameOfAssignOp(AssignOp op);
const char *nameOfRelOp(RelOp op);
const char *nameOfUnaryOp(UnaryOp op);
const char *nameOfBinaryOp(BinaryOp op);


#include "cgSemantic.h"
#endif /* INC_EXPRESS_H */
