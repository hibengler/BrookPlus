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
    o+     File:         express.cpp
    o+
    o+     Programmer:   Shaun Flisakowski
    o+     Date:         Aug 9, 1998
    o+
    o+     A high-level view of expressions.
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */
#ifdef _WIN32
#pragma warning(disable:4786)
//debug symbol warning
#endif

#include <cstdio>
#include <cstring>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <map>

#include "logger.hpp"
#include "main.h"
#include "config.h"
#include "express.h"
#include "decl.h"

#include "gram.h"
#include "token.h"
#include "stemnt.h"

#include "brtdecl.h"
#include "decl.h"
#include "project.h"
#include "stemnt.h"
#include "brtkernel.h"
#include "b2ctransform.h"
#include "brtdecl.h"


extern bool recursiveIsGather(Type *);

// Cheap and cheerful, just in case it isn't already there

#ifndef min
#define min(a,b)        ((a)<(b)?(a):(b))
#endif

//#define SHOW_TYPES



/* Print a char, converting chars to escape sequences. */
static std::ostream & MetaChar(std::ostream & out, char c, bool inString)
{
    LOG(LOG_FN_ENTRY, "MetaChar(%p, %c, %d)\n", &out, c, inString);
    switch (c)
    {
    case '\'':
        if (inString)
            out << "'";
        else
            out << "\\'";
        break;

    case '"':
        if (inString)
            out << "\\\"";
        else
            out << "\"";
        break;

    case '\0':
        out << "\\0";
        break;
    case '\\':
        out << "\\\\";
        break;
    case '\n':
        out << "\\n";
        break;
    case '\t':
        out << "\\t";
        break;
    case '\r':
        out << "\\r";
        break;
    case '\f':
        out << "\\f";
        break;
    case '\b':
        out << "\\b";
        break;
    case '\v':
        out << "\\v";
        break;
    case '\a':
        out << "\\a";
        break;
    case ESC_VAL:
        out << "\\e";
        break;

    default:
        // Show low and high ascii as octal
        if ((c < ' ') || (c >= 127))
        {
            char octbuf[8];

            sprintf(octbuf, "%03o", (unsigned char) c);
            out << "\\" << octbuf;
        }
        else
            out << c;
        break;
    }

    return out;
}

/*  ###############################################################  */

/* Print a string, converting chars to escape sequences. */
std::ostream & MetaString(std::ostream & out, const std::string & string)
{
    LOG(LOG_FN_ENTRY, "MetaString(%p, %s)\n", &out, string.c_str());
    for (unsigned i = 0; i < string.size(); i++)
        MetaChar(out, string[i], true);

    return out;
}


static void PrintAssignOp(std::ostream & out, AssignOp op)
{
    LOG(LOG_FN_ENTRY, "PrintAssignOp(%p, %d)\n", &out, op);
    switch (op)
    {
    case AO_Equal:              //  =
        out << "=";
        break;

    case AO_PlusEql:            // +=
        out << "+=";
        break;

    case AO_MinusEql:           // -=
        out << "-=";
        break;

    case AO_MultEql:            // *=
        out << "*=";
        break;

    case AO_DivEql:             // /=
        out << "/=";
        break;

    case AO_ModEql:             // %=
        out << "%=";
        break;

    case AO_ShlEql:             // <<=
        out << "<<=";
        break;

    case AO_ShrEql:             // >>=
        out << ">>=";
        break;

    case AO_BitAndEql:          // &=
        out << "&=";
        break;

    case AO_BitXorEql:          // ^=
        out << "^=";
        break;

    case AO_BitOrEql:           // |=
        out << "|=";
        break;
    }
}


static void PrintRelOp(std::ostream & out, RelOp op)
{
    LOG(LOG_FN_ENTRY, "PrintRelOp(%p, %d)\n", &out, op);
    switch (op)
    {
    case RO_Equal:              // ==
        out << "==";
        break;

    case RO_NotEqual:           // !=
        out << "!=";
        break;

    case RO_Less:               // < 
        out << "<";
        break;

    case RO_LessEql:            // <=
        out << "<=";
        break;

    case RO_Grtr:               // > 
        out << ">";
        break;

    case RO_GrtrEql:            // >=
        out << ">=";
        break;
    }
}


static void PrintILRelOp(std::ostream & out, RelOp op)
{
    LOG(LOG_FN_ENTRY, "PrintILRelOp(%p, %d)\n", &out, op);
    switch (op)
    {
    case RO_Equal:              // ==
        out << "set_relop(eq)";
        break;

    case RO_NotEqual:           // !=
        out << "set_relop(ne)";
        break;

    case RO_Less:               // < 
        out << "set_relop(lt)";
        break;

    case RO_LessEql:            // <=
        out << "set_relop(le)";
        break;

    case RO_Grtr:               // > 
        out << "set_relop(gt)";
        break;

    case RO_GrtrEql:            // >=
        out << "set_relop(ge)";
        break;
    }
}


static void PrintUnaryOp(std::ostream & out, UnaryOp op)
{
    LOG(LOG_FN_ENTRY, "PrintUnaryOp(%p, %d)\n", &out, op);
    switch (op)
    {
    case UO_Plus:               // +
        out << "+";
        break;

    case UO_Minus:              // -
        out << "-";
        break;

    case UO_BitNot:             // ~
        out << "~";
        break;

    case UO_Not:                // !
        out << "!";
        break;

    case UO_PreInc:             // ++x
    case UO_PostInc:            // x++
        out << "++";
        break;

    case UO_PreDec:             // --x
    case UO_PostDec:            // x--
        out << "--";
        break;

    case UO_AddrOf:             // &
        out << "&";
        break;

    case UO_Deref:              // *
        out << "*";
        break;
    }
}


static void PrintBinaryOp(std::ostream & out, BinaryOp op)
{
    LOG(LOG_FN_ENTRY, "PrintBinaryOp(%p, %d)\n", &out, op);
    switch (op)
    {
    case BO_Plus:               // +
        out << "+";
        break;

    case BO_Minus:              // -
        out << "-";
        break;

    case BO_Mult:               // *
        out << "*";
        break;

    case BO_Div:                // /
        out << "/";
        break;

    case BO_Mod:                // %
        out << "%";
        break;

    case BO_Shl:                // <<
        out << "<<";
        break;

    case BO_Shr:                // >>
        out << ">>";
        break;

    case BO_BitAnd:             // &
        out << "&";
        break;

    case BO_BitXor:             // ^
        out << "^";
        break;

    case BO_BitOr:              // |
        out << "|";
        break;

    case BO_And:                // &&
        out << "&&";
        break;

    case BO_Or:         // ||
        out << "||";
        break;

    case BO_Comma:              // x,y
        out << ",";
        break;

    case BO_Member:             // x.y
        out << ".";
        break;

    case BO_PtrMember:          // x->y
        out << "->";
        break;

    default:
        //  case BO_Index        // x[y]
    case BO_Assign:             // An AssignExpr
    case BO_Rel:                // A RelExpr
        break;
    }
}



//! This implementation returns the one Type based on conversion rules and based on no of components
static Type *typeArithmetic(Type * t1, Type * t2)
{
    LOG(LOG_FN_ENTRY, "typeArithmetic(%p, %p)\n", t1, t2);
    if (t1 == NULL)
    {
        assert(0);
        LOG(LOG_ERROR, "Internal Error:left type is NULL");
        return t2;
    }
    if (t2 == NULL)
    {
        assert(0);
        LOG(LOG_ERROR, "Internal Error:right type is NULL");
        return t1;
    }
#if 0 // old Implementation: This implementation returns the one with more components
    int c1 = t1->getBase()->getNumComponent();
    int c2 = t2->getBase()->getNumComponent();

    if (c2 > c1)
        return t2;

    return t1;
#else //! This implementation returns the one based on conversion rules irrespect of components
    BTDetails leftDetails = getTypeDetails(t1->getBase()->typemask);
    BTDetails rightDetails = getTypeDetails(t2->getBase()->typemask);

    int promotionID = requiredTypePromotion(leftDetails, rightDetails);
    switch(promotionID)
    {
        case -1:  //! promotion type is t2
            return t2;
            break;
        case 1:  //! promotion type is t1;
            return t1;
            break;
        case 0:  //! component promotion required so we can return t1 or t2
            {
                int c1 = t1->getBase()->getNumComponent();
                int c2 = t2->getBase()->getNumComponent();

                if (c2 > c1)
                    return t2;
                return t1;
            }
            break;
        default:  //! one of the requested type is not a supported type category
            assert(0);
            LOG(LOG_ERROR, "Requested type is a unsupported type category ");
            //! return any one
            return t1;

    }
#endif
}

static Type *typeArithmetic(Type * t1, Type * t2, Type * t3)
{
    LOG(LOG_FN_ENTRY, "typeArithmetic(%p, %p, %p)\n", t1, t2, t3);
#if 0
    Type *res = typeArithmetic(t1, t2);

    res = typeArithmetic(res, t3);
    return res;
#else 
    //! return type is either t2 or t3
    return typeArithmetic(t2, t3);
#endif 
}


Expression::Expression(ExpressionType et, const Location l):location(l)
{
    LOG(LOG_FN_ENTRY, "Expression::Expression(%d, {%d, %d, %s})\n", et, 
        l.line, l.column, l.file);
    etype = et;
    type = NULL;
    next = NULL;
    parentExp = NULL;
    parentKind = ExpParent;
    //parentStmt = NULL;
    //parentDcl = NULL;
}

Expression::~Expression()
{
    LOG(LOG_FN_ENTRY, "Expression::~Expression()\n");
    //delete type;
}


Expression *Expression::getRootExp()
{
    LOG(LOG_FN_ENTRY, "Expression::getRootExp()\n");
    Expression *parent = getParentExp();

    if (parent == NULL)
        return this;

    while (parent->getParentExp())
        parent = parent->getParentExp();

    return parent;
}

Expression *Expression::getParentExp()
{
    LOG(LOG_FN_ENTRY, "Expression::getParentExp()\n");
    return parentKind == ExpParent ? parentExp : NULL;
}

Decl *Expression::getParentDcl()
{
    LOG(LOG_FN_ENTRY, "Expression::getParentDcl()\n");
    return parentKind == DeclParent ? parentDcl : NULL;
}

Statement *Expression::getParentStmt()
{
    LOG(LOG_FN_ENTRY, "Expression::getParentStmt()\n");
    return parentKind == StmtParent ? parentStmt : NULL;
}

Type *Expression::getParentType()
{
    LOG(LOG_FN_ENTRY, "Expression::getParentType()\n");
    return parentKind == TypeParent ? parentTyp : NULL;
}

EnumDef *Expression::getParentEnumdef()
{
    LOG(LOG_FN_ENTRY, "Expression::getParentEnumdef()\n");
    return parentKind == EnumdefParent ? parentEnm : NULL;
}

void Expression::setParent(Expression * p)
{
    LOG(LOG_FN_ENTRY, "Expression::setParent(%p)\n", p);
    parentKind = ExpParent;
    parentExp = p;
}

void Expression::setParent(Decl * p)
{
  LOG(LOG_FN_ENTRY, "Expression::setParent(%p)\n", p);
    parentKind = DeclParent;
    parentDcl = p;
}

void Expression::setParent(Statement * p)
{
    LOG(LOG_FN_ENTRY, "Expression::setParent(%p)\n", p);
    parentKind = StmtParent;
    parentStmt = p;
}

void Expression::setParent(Type * p)
{
    LOG(LOG_FN_ENTRY, "Expression::setParent(%p)\n", p);
    parentKind = TypeParent;
    parentTyp = p;
}

void Expression::setParent(EnumDef * p)
{
    LOG(LOG_FN_ENTRY, "Expression::setParent(%p)\n", p);
    parentKind = EnumdefParent;
    parentEnm = p;
}


Expression *Expression::dup0() const
{
    LOG(LOG_FN_ENTRY, "Expression::dup0()\n");
    Expression *ret = new Expression(etype, location);

    ret->type = type;
    return ret;
}


void Expression::replace(Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "Expression::replace(%p)\n", newExp);
    if (parentKind == ExpParent)
    {
        parentExp->replaceExpr(this, newExp);
    }
    else if (parentKind == StmtParent)
    {
        parentStmt->replaceExpr(this, newExp);
    }
    else if (parentKind == DeclParent)
    {
        parentDcl->replaceExpr(this, newExp);
    }
    else
        assert(0);
}


void Expression::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "Expression::print(%p)\n", &out);
    out << __PRETTY_FUNCTION__ << std::endl;
}

void Expression::printExprWithTypeInfo(std::ostream& out) const
{
    out << "\tExpression : ";
    print(out);
    Type* actualType = getType();
    if(actualType && actualType->getBase())
    {
    	out << ", Type : ";
        BaseType* baseType = actualType->getBase();
        BaseTypeInfo* baseTypeInfo = getBaseTypeInfo(baseType->typemask);
        out << baseTypeInfo->brTypeName;
    }
	out << std::endl;
}
#if BROOK_TO_IL_CODE_GEN_PATH
void Expression::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "Expression::print_il(%p)\n", &out);
    out << IL_COMMENT_STRING << "no il generation for expression type : " << nameOfExpressionType(etype) << "\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

Constant::Constant(ConstantType ct, const Location l):Expression(ET_Constant, l)
{
    LOG(LOG_FN_ENTRY, "Constant::Constant(%d, %p)\n", ct, &l);
    ctype = ct;
}


Constant::~Constant()
{
    LOG(LOG_FN_ENTRY, "Constant::~Constant()\n");
}


IntConstant::IntConstant(long val, const Location l, NumericForm printForm):Constant(CT_Int, l)
{
    LOG(LOG_FN_ENTRY, "IntConstant::IntConstant(%ld, %p)\n", val, &l);
    lng = val;
    numericForm = printForm;
    type = new BaseType(BT_Int);
}


IntConstant::~IntConstant()
{
    LOG(LOG_FN_ENTRY, "IntConstant::~IntConstant()\n");
}


Expression *IntConstant::dup0() const
{
    LOG(LOG_FN_ENTRY, "IntConstant::dup0()\n");
    IntConstant *ret = new IntConstant(lng, location, numericForm);

    ret->type = type;
    return ret;
}


void IntConstant::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "IntConstant::print(%p)\n", &out);
    if(numericForm == BR_Decimal_Form)
        out << lng;
    else
    {
        //! Presently printing hexdecimal for both hex and octal forms
        char valueStr[100] = {0};
        sprintf(valueStr, "0x%x", lng);
        out << valueStr;
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void IntConstant::print_il(std::ostream &out) const
{
    LOG(LOG_FN_ENTRY, "IntConstant::print_il(%p)\n", &out);
}
#endif // BROOK_TO_IL_CODE_GEN_PATH


UIntConstant::UIntConstant(unsigned long val, const Location l, NumericForm printForm):Constant(CT_UInt, l)
{
    LOG(LOG_FN_ENTRY, "UIntConstant::UIntConstant(%lu, %p)\n", val, &l);
    ulng = val;
    numericForm = printForm;
    type = new BaseType(BT_Int | BT_UnSigned);
}


UIntConstant::~UIntConstant()
{
    LOG(LOG_FN_ENTRY, "UIntConstant::~UIntConstant()\n");
}


Expression *UIntConstant::dup0() const
{
    LOG(LOG_FN_ENTRY, "UIntConstant::dup0()\n");
    UIntConstant *ret = new UIntConstant(ulng, location, numericForm);

    ret->type = type;
    return ret;
}


void UIntConstant::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "UIntConstant::print(%p)\n", &out);

    if(numericForm == BR_Decimal_Form)
        out << ulng << "u";
    else
    {
        //! Presently printing hexdecimal for both hex and octal forms
        char valueStr[100] = {0};
        sprintf(valueStr, "0x%xu", ulng);
        out << valueStr;
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void UIntConstant::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "UIntConstant::print_il(%p)\n", &out);
    out << IL_PREFIX_STRING << "uint constant\n";
}
#endif //BROOK_TO_IL_CODE_GEN_PATH



FloatConstant::FloatConstant(std::string str, const Location l):Constant(CT_Float, l)
{
    LOG(LOG_FN_ENTRY, "FloatConstant::FloatConstant(%s, %p)\n", str.c_str(), &l);
    strVal = str;
    type = new BaseType(BT_Float);
}

FloatConstant::~FloatConstant()
{
    LOG(LOG_FN_ENTRY, "FloatConstant::~FloatConstant()\n");
}


Expression *FloatConstant::dup0() const
{
    LOG(LOG_FN_ENTRY, "FloatConstant::dup0()\n");
    FloatConstant *ret = new FloatConstant(strVal, location);
    ret->strVal = strVal;
    return ret;
}


void FloatConstant::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "FloatConstant::print(%p)\n", &out);

    out << strVal.c_str();


#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void FloatConstant::print_il(std::ostream &out) const
{
    LOG(LOG_FN_ENTRY, "FloatConstant::print_il(%p)\n", &out);
}
#endif // BROOK_TO_IL_CODE_GEN_PATH


DoubleConstant::DoubleConstant(std::string str, const Location l):Constant(CT_Double, l)
{
    strVal = str;
    type = new BaseType(BT_Double);
}


DoubleConstant::~DoubleConstant()
{
}
#if BROOK_TO_IL_CODE_GEN_PATH
bril::store * DoubleConstant::get_il_store() const
{
    semanticCheckRecord(const_cast<DoubleConstant*>(this), SCROther, "double constant not supported", true);
    //bril::get_scalar_constant_store(&il_store, double(val));
    return &il_store;
}


std::string DoubleConstant::getILRegName(bril::reg_style style) const
{
    semanticCheckRecord(const_cast<DoubleConstant*>(this), SCROther, "double constant not supported", true);
    //assert(style == bril::rs_src_sel);
    return bril::get_scalar_constant_name(float(val));
}
#endif // BROOK_TO_IL_CODE_GEN_PATH
Expression *DoubleConstant::dup0() const
{
    DoubleConstant *ret = new DoubleConstant(strVal, location);
    ret->strVal = strVal;
    return ret;
}


void DoubleConstant::print(std::ostream & out) const
{
    out << strVal.c_str();

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void DoubleConstant::print_il(std::ostream & UNUSED(out)) const
{
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

CharConstant::CharConstant(char chr, const Location l, bool isWide /* =false */ )
:Constant(CT_Char, l)
{
    LOG(LOG_FN_ENTRY, "CharConstant::CharConstant(%c, %p, %d)\n", chr, &l, isWide);
    ch = chr;
    wide = isWide;
    type = new BaseType(BT_Char);
}

CharConstant::~CharConstant()
{
    LOG(LOG_FN_ENTRY, "CharConstant::~CharConstant()\n");
}


Expression *CharConstant::dup0() const
{
    LOG(LOG_FN_ENTRY, "CharConstant::dup0()\n");
    CharConstant *ret = new CharConstant(ch, location, wide);

    ret->type = type;
    return ret;
}

void CharConstant::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "CharConstant::print(%p)\n", &out);
    if (wide)
        out << 'L';

    out << "'";
    MetaChar(out, ch, false);
    out << "'";

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void CharConstant::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "CharConstant::print_il(%p)\n", &out);
    out << IL_PREFIX_STRING << "char constant\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH


StringConstant::StringConstant(const std::string & str, const Location l, bool isWide /* =false */ )
:Constant(CT_String, l), buff(str)
{
    LOG(LOG_FN_ENTRY, "StringConstant::StringConstant(%s, %p, %d)\n", str.c_str(), &l, isWide);
    wide = isWide;

    // Or should this be const char*?
    PtrType *ptrType = new PtrType();

    type = ptrType;
    ptrType->subType = new BaseType(BT_Char);
}


StringConstant::~StringConstant()
{
    LOG(LOG_FN_ENTRY, "StringConstant::~StringConstant()\n");
}


int StringConstant::length() const
{
    LOG(LOG_FN_ENTRY, "StringConstant::length()\n");
    return (int) buff.size();
}


Expression *StringConstant::dup0() const
{
    LOG(LOG_FN_ENTRY, "StringConstant::dup0()\n");
    StringConstant *ret = new StringConstant(buff, location, wide);

    ret->type = type;
    return ret;
}


void StringConstant::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "StringConstant::print(%p)\n", &out);
    if (wide)
        out << 'L';

    out << '"';
    MetaString(out, buff);
    out << '"';

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void StringConstant::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "StringConstant::print_il(%p)\n");
    out << IL_PREFIX_STRING << "string constant\n";
}
#endif //BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void 
StringConstant::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "StringConstant::semanticCheck(%d)\n", isKernel);
    if (isKernel)
    {
        RecordSemanticError(this, SemanticChecker::SCEKExprStringConst);
    }
}
#endif
ArrayConstant::ArrayConstant(const Location l):Constant(CT_Array, l)
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::ArrayConstant(%p)\n", &l);
}


ArrayConstant::~ArrayConstant()
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::~ArrayConstant()\n");
    ExprVector::iterator j;

    for (j = items.begin(); j != items.end(); j++)
    {
        delete *j;
    }
}


void ArrayConstant::addElement(Expression * expr)
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::addElement(%p)\n", expr);
    items.push_back(expr);
    expr->setParent(this);
}


Expression *ArrayConstant::dup0() const
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::dup0()\n");
    ArrayConstant *ret = new ArrayConstant(location);

    ExprVector::const_iterator j;
    for (j = items.begin(); j != items.end(); j++)
    {
        ret->addElement((*j)->dup());
    }

    ret->type = type;
    return ret;
}


void ArrayConstant::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::print(%p)\n", &out);
    out << "{ ";

    if (!items.empty())
    {
        ExprVector::const_iterator j = items.begin();

        (*j)->print(out);

        for (j++; j != items.end(); j++)
        {
            out << ", ";
            (*j)->print(out);
        }
    }

    out << " }";

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void ArrayConstant::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::print_il(%p)\n", &out);
    out << IL_PREFIX_STRING << "// array constant\n";
}

void ArrayConstant::print_il(std::ostream & out, Type * force_type) const
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::print_il(%p, %p)\n", &out, force_type);
    *(Type **) & type = force_type->dup();

    type->allocateILStorage(&il_store);

    assert(il_store.size > 0);

    static const char *components[] = { "x", "y", "z", "w" };
    if (!type->isStructure())
    {
        for (ExprVector::const_iterator j = items.begin(); j != items.end(); ++j)
            (*j)->print_il(out);

        assert(!is_struct(&il_store));
        u32 sz = (u32) min(il_store.size, items.size());

        assert(sz >= 1 && sz <= 4);

        for (u32 i = 0; i < sz; ++i)
        {
            out << IL_PREFIX_STRING << "mov " << getILRegName(bril::rs_base) << "." << components[i];
            out << ", "<< items[i]->getILRegName(bril::rs_src_sel) << "\n";
        }
    }
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void ArrayConstant::semanticCheck(bool isKernel)
{
    if (!items.empty())
    {
        ExprVector::const_iterator j ;
        for (j = items.begin(); j != items.end(); j++)
        {
            (*j)->semanticCheck(isKernel);
        }
    }
    else
    {
        RecordSemanticWarn(this, SemanticChecker::SCWNull, "Empty array initializer", SemanticChecker::SCWarningLevelOne);
    }

}
#endif

void ArrayConstant::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::findExpr(%p)\n", cb);
    (cb) (this);

    ExprVector::iterator j;

    for (j = items.begin(); j != items.end(); j++)
    {
        //*j = (cb)(*j);
        (*j)->findExpr(cb);
    }
}


void ArrayConstant::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "ArrayConstant::replaceExpr(%p, %p)\n", oldExp, newExp);
    bool done = false;

    for (unsigned int i = 0; i != items.size(); i++)
    {
        if (items[i] == oldExp)
        {
            items[i] = newExp;
            if (newExp)
                newExp->setParent(this);
            done = true;
        }
    }
    assert(done);
}


EnumConstant::EnumConstant(Symbol * nme, Expression * val, const Location l):Constant(CT_Enum, l)
{
    LOG(LOG_FN_ENTRY, "EnumConstant::EnumConstant(%p, %p, %p)\n", nme, val, &l);
    name = nme;
    value = val;
}


EnumConstant::~EnumConstant()
{
    LOG(LOG_FN_ENTRY, "EnumConstant::~EnumConstant()\n");
    delete name;
    delete value;
}


Expression *EnumConstant::dup0() const
{
    LOG(LOG_FN_ENTRY, "EnumConstant::dup0()\n");
    EnumConstant *ret = new EnumConstant(name->dup(), value->dup(), location);

    ret->type = type;
    return ret;
}


void EnumConstant::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "EnumConstant::print(%p)\n", &out);
    out << *name;

    if (value)
    {
        out << " = " << *value;
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void EnumConstant::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "EnumConstant::print_il(%p)\n", &out);
    out << IL_PREFIX_STRING << "enum constant\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH


Variable::Variable(Symbol * varname, const Location l):Expression(ET_Variable, l)
{
    LOG(LOG_FN_ENTRY, "Variable::Variable(%p, %p)\n", varname, &l);

    name = varname;
    if(varname)
    {
        /*
         * If the name is in the symbol table, grab its type.
         */
        if (name->entry && name->entry->uVarDecl)
        {
            type = name->entry->uVarDecl->form;
        }
    }
}


Variable::~Variable()
{
    LOG(LOG_FN_ENTRY, "Variable::~Variable()\n");
    delete name;
}


Expression *Variable::dup0() const
{
    LOG(LOG_FN_ENTRY, "Variable::dup0()\n");
    Variable *ret = new Variable(name->dup(), location);

    ret->type = type;
    return ret;
}


void Variable::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "Variable::print(%p)\n", &out);
    out << *name;

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
void Variable::print_il(std::ostream &out) const
{
    LOG(LOG_FN_ENTRY, "Variable::print_il(%p)\n", &out);
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void
Variable::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "Variable::semanticCheck(%d)\n", isKernel);
    if (isKernel)
    {
        //check here: must be defined, non global
        //checked are decl: no static variable, no voliaile, no pointers 

        if (name->entry == NULL || name->entry->uVarDecl == NULL)
        {
            //undefined variable
            RecordSemanticError(this, SemanticChecker::SCEKExprVariable, "variable not defined");
        }
    }
}
#endif 
FunctionCall::FunctionCall(Expression * func, const Location l):Expression(ET_FunctionCall, l)
{
    LOG(LOG_FN_ENTRY, "FunctionCall::FunctionCall(%p, %p)\n", func, &l);
    function = func;
    function->setParent(this);
}


FunctionCall::~FunctionCall()
{
    LOG(LOG_FN_ENTRY, "FunctionCall::~FunctionCall()\n");
    delete function;

    ExprVector::iterator j;

    for (j = args.begin(); j != args.end(); j++)
        delete *j;
}


void FunctionCall::addArg(Expression * arg)
{
    LOG(LOG_FN_ENTRY, "FunctionCall::addArg(%p)\n", arg);

    args.push_back(arg);
    if (arg)
        arg->setParent(this);
}


void FunctionCall::addArgs(Expression * argList)
{
    LOG(LOG_FN_ENTRY, "FunctionCall::addArgs(%p)\n", argList);
    Expression *arg = argList;

    while (argList != NULL)
    {
        argList = argList->next;
        arg->next = NULL;
        addArg(arg);
        arg = argList;
    }
}


Expression *FunctionCall::dup0() const
{
    LOG(LOG_FN_ENTRY, "FunctionCall::dup0()\n");
    FunctionCall *ret = new FunctionCall(function->dup(), location);

    ExprVector::const_iterator j;
    for (j = args.begin(); j != args.end(); j++)
    {
        ret->addArg((*j)->dup());
    }

    return ret;
}


void FunctionCall::print_param(std::ostream & out, int idx) const
{
    LOG(LOG_FN_ENTRY, "FunctionCall::print_param(%p, %d)\n", &out, idx);
    Expression *p = args[idx];

    if (p->etype == ET_BinaryExpr && ((BinaryExpr *) (p))->op() == BO_Comma)
    {
        out << "(";
        p->print(out);
        out << ")";
    }
    else
        p->print(out);

}

void FunctionCall::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "FunctionCall::print(%p)\n", &out);

    // get function name
    std::ostringstream tempStrStream;
    function->print(tempStrStream);
    std::string tempStr = tempStrStream.str();

    //! Print return type appropriately for unsupported double math operations
    if(globals.enableSematicCheck == false && isSupportedDoubleMathOps(tempStr) 
         && TARGET_CAL == globals.getPresentTarget())
    {
        BaseType *type = function->type->getBase();
        BaseTypeInfo *typeInfo = getBaseTypeInfo(type->typemask);
        std::string doubleDataType = "double";
        std::string double2DataType = "double2";
        if(doubleDataType.compare(typeInfo->brTypeName) == 0)
        {
            out << "(float)";
        }
        else if(double2DataType.compare(typeInfo->brTypeName) == 0)
        {
            out << "(float2)";
        }
    }

    if (function->precedence() < precedence())
    {
        out << "(";
        function->print(out);
        out << ")";
    }
    else
        function->print(out);

    out << "(";

    if (!args.empty())
    {
        //! check the Math fucntion parameters. convert double parameters to float for cal back end
        //! we know there are no IL instructions for double math operations
        if(globals.enableSematicCheck == false && isSupportedDoubleMathOps(tempStr) 
            && TARGET_CAL == globals.getPresentTarget())
        {
            for (int i = 0; i < int (args.size()); ++i)
            {
                //! Type information is not set always during parse 
                //! so skep if type is NULL
                if(args[i]->getType() == NULL || args[i]->getType()->getBase() == NULL)
                {
                    LOG(LOG_ERROR, "%s :line %d -> Type information is not set by parser \n", __FILE__, __LINE__);
                    print_param(out, i);
                    continue;
                }
                BaseType *type = args[i]->getType()->getBase();
                BaseTypeInfo *typeInfo = getBaseTypeInfo(type->typemask);
                if (i != 0)
                    out << ", ";
                std::string doubleDataType = "double";
                std::string double2DataType = "double2";
                if(doubleDataType.compare(typeInfo->brTypeName) == 0)
                {
                    out << "(float)";
                }
                else if(double2DataType.compare(typeInfo->brTypeName) == 0)
                {
                    out << "(float2)";
                }
                print_param(out, i);
            }
        }
        else
        {
            for (int i = 0; i < int (args.size()); ++i)
            {
                if (i != 0)
                    out << ", ";
                print_param(out, i);
            }
        }
    }

    out << ")";

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#if BROOK_TO_IL_CODE_GEN_PATH
FunctionDef *find_gpu_kernel_by_name(std::string const &fname);
extern int getKernelInstance(std::string const &fname, SamplerParamInfo const &params);

void FunctionCall::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "FunctionCall::print_il(%p)\n", &out);
    //fixme: print_il is incorrectly tag as "const"
    if (function->etype == ET_Variable)
    {
        Variable *v = static_cast < Variable * >(function);

        out << "\n" << IL_COMMENT_STRING << "calling " << v->name->name << ": param evaluation\n";

        for (ExprVector::const_iterator j = args.begin(); j != args.end(); j++)
            (*j)->print_il(out);

        if (function->type && function->type->isKernel())
        {
            FunctionDef *fd = find_gpu_kernel_by_name(v->name->name);

            FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);
            SamplerParamInfo samplers;

            u32 idx = 0;

            for (ExprVector::const_iterator j = args.begin(); j != args.end(); j++, ++idx)
            {
                if (!ft->args[idx]->isArray())
                {
                    out << "\n" << IL_COMMENT_STRING << "calling " << v->name->name << ": loading param " << ft->
                        args[idx]->name->name << "\n";
                    bril::emitCopy(out, ft->args[idx]->name->get_il_store(), (*j)->get_il_store());
                }
                else
                {
                    assert((*j)->etype == ET_Variable);
                    Variable *s_param = (Variable *) (*j);

                    samplers.insert(ft->args[idx]->name->name, bril::getTextureId(s_param->name->name));
                }
            }

            out << "\n" << IL_COMMENT_STRING << "calling " << v->name->name << " ( ";
            samplers.print(out);
            out << ")\n";

            out << "\n" << IL_PREFIX_STRING << "call " << getKernelInstance(v->name->name, samplers) << "\n";

            idx = 0;
            for (ExprVector::const_iterator j = args.begin(); j != args.end(); j++, ++idx)
            {
                if ((ft->args[idx]->form->getQualifiers() & TQ_Out) == TQ_Out)
                {
                    out << "\n" << IL_COMMENT_STRING << "calling " << v->name->name << ": copying out param " << ft->
                        args[idx]->name->name << "\n";
                    bril::emitCopy(out, (*j)->get_il_store(), ft->args[idx]->name->get_il_store(), true);
                }
            }
            if (fd->returnsVoid() == false)
            {
                bril::store t = getReturnILStore();
                bril::emitCopy(out, get_il_store(), &t);
            }
        }
        else
        {
            out << IL_COMMENT_STRING << "calling " << v->name->name << ": builtin\n";

            if (bril::emitBuiltin(out, this) == false)
            {
                semanticCheckRecord((Expression *) this, SCRCallCallee, v->name->name.c_str(), true);
            }

        }
    }
    else
        semanticCheckRecord((Expression *) this, SCRCallCallee, NULL, true);    //indirect call
}
#endif // BROOK_TO_IL_CODE_GEN_PATH
#ifdef SEMANTIC_CHECK
static bool 
isVectorSwizzleRepeated(const BaseType * type, const std::string & swizzle, std::string& msg);
void
FunctionCall::lvalueSemantics(Expression* leftExpr, int argPos, bool isKernel)
{
    //! Check inside kernel
    if(isKernel)
    {
        std::ostringstream tmpStr;
        tmpStr << "argument " << argPos << " must be lvalue type";
        if(leftExpr->etype == ET_Constant) //! for constant
            RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpStr.str().c_str());
        else if(leftExpr->etype == ET_FunctionCall) //! for function call
            RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpStr.str().c_str());
        else if(leftExpr->etype == ET_Variable) //! for variable
        {
            Variable *v = AsVariable(leftExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;

            Type* argType = leftExpr->getType();

            TypeQual qualifier = argType->getQualifiers();
            if(v->type->isKernelParam())
            {
                //! For non-outputs
                if((qualifier & TQ_Out) == 0 && (qualifier & TQ_Reduce) == 0 )
                {
                    if(argType->isStream())
                        RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpStr.str().c_str());
                    else
                        RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpStr.str().c_str());
                }
            }
            else
            {
                //! for local constant variable
                if((qualifier & TQ_Const) == true)
                    RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpStr.str().c_str());
            }

        }
        else if(leftExpr->etype == ET_IndexExpr) //! for Array variable
        {
            IndexExpr *index = AsIndexExpr(leftExpr);

            Type* arrayType = index->array->type;

            if(arrayType)
            {
                TypeQual qualifier = arrayType->getQualifiers();
                if(arrayType->isKernelParam())
                {
                    //! For non-outputs
                    if((qualifier & TQ_Out) == 0 && (qualifier & TQ_Reduce) == 0 )
                    {
                        if(arrayType->isArray())
                            RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpStr.str().c_str());
                    }
                }
                else
                {
                    //! for local constant Array
                    if((qualifier & TQ_Const) == true)
                        RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpStr.str().c_str());
                }
            }
        }
        else if(leftExpr->etype == ET_BinaryExpr)
        {
            BinaryExpr *expr = AsBinaryExpr(leftExpr);
            if(expr->bOp == BO_Member)
            {
                BaseType *exprType = expr->leftExpr()->getType()->getBase();
                Variable *expVar = AsVariable(expr->rightExpr());

                if (!exprType->isStructure())
                {
                    std::string msg = "";
                    if(isScalarType(getTypeDetails(exprType->typemask)))
                    {
                        std::ostringstream tmpmsg;
                        tmpmsg << "Illegal to use swizzle on scalar types";
                        RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str());
                    }

                    //! check for components repetition in lvalue expression
                    if (isVectorSwizzleRepeated(exprType, expVar->name->name, msg) == false)
                    {
                        std::ostringstream tmpmsg;
                        if(!msg.empty())
                        {
                            tmpmsg << "illegal to use components repetition in lvalue expression :" << msg.c_str() << " in ";
                        }
                        else
                        {
                            tmpmsg << "unrecognized field/swizzle ";
                        }
                        expr->print(tmpmsg);
                        RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str());
                    }
                }
            }
            else
            {
                assert(0);
                print(tmpStr);
                LOG(LOG_ERROR, "%s : %d -> Internal error for %s : Unsupported binary expression as lvalue", __FILE__, __LINE__, tmpStr.str().c_str());
            }
        }
        else
        {
            assert(0);
            print(tmpStr);
            LOG(LOG_ERROR, "%s : %d -> Internal error for %s", __FILE__, __LINE__, tmpStr.str().c_str());
        }
    }
    else
    {
        // Here handle outside kernel assignments
    }
}

void 
FunctionCall::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "FunctionCall::semanticCheck(%d)\n", isKernel);
    if (isKernel == false)
        function->semanticCheck(isKernel);

    ExprVector::iterator j = args.begin();

    for (j = args.begin(); j != args.end(); j++)
    {
        (*j)->semanticCheck(isKernel);
    }

    if (isKernel)
    {
        //check function call within kernel code
        if (function->etype != ET_Variable)
        {
            RecordSemanticError(this, SemanticChecker::SCEKExprCall, "indirect call not supported");
            return;
        }

        Variable *funcVar = AsVariable(function);

        if (funcVar->name->entry == NULL)
        {
            semanticChecker.checkBuiltin(this, funcVar);
            return;
        }

        Decl *dcl;
        if (funcVar->name->entry->u2FunctionDef)
        {
            //function with definition
            dcl = funcVar->name->entry->u2FunctionDef->decl;
        }
        else
        {
            //function declaration
            RecordSemanticError(this, SemanticChecker::SCEKExprCall, "definition must be available before invoking");
            return;
        }

        if (dcl->form->type != TT_Function)
        {
            RecordSemanticError(this, SemanticChecker::SCEKExprCall, "callee is not a function");
            return;
        }

        if (dcl->form->isKernel() == false)
        {
            RecordSemanticError(this, SemanticChecker::SCEKExprCall, "kernel can't call a non-kernel");
            return;
        }

        if (dcl->form->isKernel() && dcl->form->isReduce())
        {
            RecordSemanticError(this, SemanticChecker::SCEKExprCall, "callee can't call a reduction kernel");
            return;
        }

        FunctionType *funcType = (FunctionType *) dcl->form;

        //check the callee

        //check formal actual matching
        if (args.size() != funcType->nArgs)
        {
            std::ostringstream tmpmsg;
            tmpmsg << "incorrect number of parameters expect " << funcType->nArgs <<" actual " << args.size();
            RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str());
            return;
        }
        else
        {
            //! Check type checking between formal parameters and actual parameters
            for(int i = 0; i < AsInt(args.size()); ++i)
            {
                Type *acutalType = args[i]->getType();
                Type *formalType = funcType->args[i]->form;

                if(formalType->type == TT_Base)  //! if formal parameter is constant
                {
                    if(args[i]->etype == ET_Variable)
                    {
                        Variable *v = AsVariable(args[i]);
                        //! return if variable is not declared
                        if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                            continue;
                        if(v->name->entry->uVarDecl->isArray())
                        {
                            std::ostringstream tmpmsg;
                            tmpmsg << "Illegal to use array type at argument " << i << ": Exprected stream type/variable";
                            RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                        }
                    }
                }
                else if(formalType->isArray()) //! if formal type is array type
                {
                    if(isScatterType(formalType))  //! Scatter array
                    {
                            std::ostringstream tmpmsg;
                            tmpmsg << "Illegal to call scatter kernel from any kernel";
                            RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                    }
                    else
                    {
                        if(args[i]->etype == ET_Variable)
                        {
                            Variable *v = AsVariable(args[i]);
                            //! return if variable is not declared
                            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                                continue;
                            
                            if(v->name->entry->uVarDecl->isArray()) 
                            {
                                if(v->name->entry->uVarDecl->form->isKernelParam())
                                {
                                    if(formalType->type == TT_Base || formalType->isStream())
                                    {
                                        std::ostringstream tmpmsg;
                                        tmpmsg << "Illegal to use array type at argument" << i << ": Exprected value";
                                        RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                                    }
                                    else if(formalType->isArray())
                                    {
                                        ArrayType *formalArray = AsArrayType(funcType->args[i]->form);
                                        ArrayType *actualArray = AsArrayType(args[i]->type);

                                        if(formalArray->getDimensions() != actualArray->getDimensions())
                                        {
                                            std::ostringstream tmpmsg;
                                            tmpmsg << "Mismatched dimensions of formal and actual arguments for argument " << i;
                                            RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                                        }
                                        else if(formalArray->resourceType != actualArray->resourceType)
                                        {
                                            std::ostringstream tmpmsg;
                                            tmpmsg << "Mismatched resource type of gather array for argument " << i;
                                            RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                                        }
                                        else
                                        {
                                            if(formalArray->resourceType == ConstBufferType)
                                            {
                                                if(formalArray->getTotalElementCount() != actualArray->getTotalElementCount())
                                                {
                                                    std::ostringstream tmpmsg;
                                                    tmpmsg << "Mismatched element count of gather array for argument " << i;
                                                    RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        std::ostringstream tmpmsg;
                                        tmpmsg << "Local array not allowed to pass as kernel parameter for argument " << i;
                                        RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                                    }
                                }
                            }
                        }
                        else
                        {
                            std::ostringstream tmpmsg;
                            tmpmsg << "Must be gather array type for argument " << i;
                            RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                        }

                    }
                }
                else if(formalType->isStream())  //! stream type
                {
                    if(args[i]->etype == ET_Variable)
                    {
                        Variable *v = AsVariable(args[i]);
                        //! return if variable is not declared
                        if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                            continue;
                        if(v->name->entry->uVarDecl->isArray())
                        {
                            std::ostringstream tmpmsg;
                            tmpmsg << "Illegal to use array type at argument" << i << ": Exprected stream type/variable";
                            RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str()); 
                        }
                    }
                    if(formalType->getQualifiers() == TQ_Out)
                    {
                        lvalueSemantics(args[i], i, isKernel);
                    }
                }

                StructDef *structure = findStructureDef(formalType);

                //! Skip structure semantics
                if(structure == NULL)
                {
                    assert(acutalType->getBase());
                    assert(formalType->getBase());

                    BaseTypeInfo *actualTypeInfo = getBaseTypeInfo(acutalType->getBase()->typemask);
                    BaseTypeInfo *formalTypeInfo = getBaseTypeInfo(formalType->getBase()->typemask);


                    if(acutalType->getBase()->typemask != formalType->getBase()->typemask)
                    {
                        std::ostringstream tmpmsg;
                        tmpmsg << "Mismatched type for argument " << i << ": actual type is " << actualTypeInfo->brTypeName
                               << " but expected type is " << formalTypeInfo->brTypeName; 
                        RecordSemanticError(this, SemanticChecker::SCEKExprCall, tmpmsg.str().c_str());
                    }
                }

            }
        }
    }
    else
    {
        //check calls to kernel with host code
    }
}

#endif
void FunctionCall::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "FunctionCall::findExpr(%p)\n", cb);
    (cb) (this);
    function->findExpr(cb);

    ExprVector::iterator j = args.begin();

    for (j = args.begin(); j != args.end(); j++)
    {
        //*j = (cb)(*j);
        (*j)->findExpr(cb);
    }
}


void FunctionCall::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "FunctionCall::replaceExpr(%p, %p)\n", oldExp, newExp);
    bool done = false;

    if (oldExp == function)
    {
        function = newExp;
        done = true;
    }
    else
    {
        for (int j = 0; j < static_cast < int >(args.size()); j++)
        {
            if (args[j] == oldExp)
            {
                args[j] = newExp;
                done = true;
            }
        }
    }
    if (newExp)
        newExp->setParent(this);
    assert(done);
}



Type *FunctionCall::getType() const
{
    LOG(LOG_FN_ENTRY, "FunctionCall::getType()\n");
    static BaseType *defaultType = NULL;

    if (type)
        return type->getRealType();

    assert(function->etype == ET_Variable);

    if (function->type && function->type->isKernel())
    {
        Variable *v = AsVariable(function);

        Decl *dcl;
        if (v->name->entry->u2FunctionDef)
        {
            //function with definition
            dcl = v->name->entry->u2FunctionDef->decl;
        }
        else
        {
            //function declaration
            assert(v->name->entry->IsFctDecl());      //fixme, should not assert ...
            dcl = v->name->entry->uVarDecl;
        }
        FunctionType *ft = AsFunctionType(dcl->form);

        return ft->subType->getBase();
    }


    //! fucntion->type is not set by parser
    //! function->type is set during semantic check of perticular builtin function
    if(function->type)
        return function->type;
    else
        return defaultType ? defaultType : defaultType = new BaseType(BT_Int);


}

#if BROOK_TO_IL_CODE_GEN_PATH
bril::store FunctionCall::getReturnILStore() const
{
    LOG(LOG_FN_ENTRY, "FunctionCall::getReturnILStore()\n");
    bril::store retval;
    assert(function->etype == ET_Variable);

    if (function->type && function->type->isKernel())
    {
        Variable *v = static_cast < Variable * >(function);
        FunctionDef *fd = find_gpu_kernel_by_name(v->name->name);

        //FunctionType* ft = static_cast<FunctionType*>(fd->decl->form);
        //return ft->subType->getBase();
        retval = *(fd->get_il_store());
    }
    else
    {
        assert(0);
    }

    return retval;
}


bril::store * FunctionCall::get_il_store() const
{
    LOG(LOG_FN_ENTRY, "FunctionCall::get_il_store()\n");
    assert(function->etype == ET_Variable);
    getType()->allocateILStorage(&il_store);

    return &il_store;
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

UnaryExpr::UnaryExpr(UnaryOp _op, Expression * expr, const Location l):Expression(ET_UnaryExpr, l)
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::UnaryExpr(%d, %p, %p)\n", &_op, expr, &l);
    uOp = _op;
    _operand = expr;
    if (expr)
        expr->setParent(this);
}


UnaryExpr::~UnaryExpr()
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::~UnaryExpr()\n");
    delete _operand;
}


Expression *UnaryExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::dup0()\n");
    UnaryExpr *ret = new UnaryExpr(uOp, _operand->dup(), location);

    ret->type = type;
    return ret;
}


void UnaryExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::print(%p)\n", &out);
    switch (uOp)
    {
    default:
        PrintUnaryOp(out, uOp);

        if (operand()->precedence() < precedence())
            out << "(" << *operand() << ")";
        else
            out << *operand();
        break;

    case UO_Minus:
        PrintUnaryOp(out, uOp);
        if ((operand()->precedence() < precedence())
            || ((operand()->etype == ET_UnaryExpr) && (((UnaryExpr *) operand())->op() == UO_Minus)))
            out << "(" << *operand() << ")";
        else
            out << *operand();
        break;

    case UO_PostInc:
    case UO_PostDec:
        if (operand()->precedence() < precedence())
            out << "(" << *operand() << ")";
        else
            out << *operand();

        PrintUnaryOp(out, uOp);
        break;
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}
#ifdef SEMANTIC_CHECK
void 
UnaryExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::semanticCheck(%d)\n", isKernel);
    _operand->semanticCheck(isKernel);
    if(isKernel)
    {
        //! Operand type is array type?
        if(_operand->etype == ET_Variable)
        {
            Variable *v = AsVariable(_operand);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as unary operand";
                RecordSemanticError(this, SemanticChecker::SCEKExprOp, tmpmsg.str().c_str()); 
                return;
            }
        }
    }

    if (isKernel)
    {
        if (uOp == UO_AddrOf)
            RecordSemanticError(this, SemanticChecker::SCEKExprOp, " variable address(&)");
        else if (uOp == UO_Deref)
            RecordSemanticError(this, SemanticChecker::SCEKExprOp, " pointer dereference(*)");
        else
        {
            Type *operandType = _operand->getType();
            if(operandType != NULL)
            {
                BTDetails details = getTypeDetails(operandType->getBase()->typemask);
                if(isArithmeticType(details))
                {
                    if(uOp == UO_BitNot && isIntegerType(details) == false)
                        RecordSemanticError(this, SemanticChecker::SCEKNull, "operand is not a Integer type");
                }
                else
                {
                    RecordSemanticError(this, SemanticChecker::SCEKNull, "operand is not a Arithmetic type");
                }
            }
            else
            {
                assert(0);
                std::ostringstream tmpStr;
                print(tmpStr);
                LOG(LOG_ERROR, "%s : %d -> Internal error for %s", __FILE__, __LINE__, tmpStr.str().c_str());
            }
        }
    }
}
#endif

int UnaryExpr::precedence() const
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::precedence()\n");
    switch (uOp)
    {
    case UO_Plus:               // +
    case UO_Minus:              // -
    case UO_BitNot:             // ~
    case UO_Not:                // !

    case UO_PreInc:             // ++x
    case UO_PreDec:             // --x

    case UO_AddrOf:             // &
    case UO_Deref:              // *
        return 15;

    case UO_PostInc:            // x++
    case UO_PostDec:            // x--
        return 16;
    }

    /* Not reached */
    return 16;
}


void UnaryExpr::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::findExpr(%d)\n", cb);
    (cb) (this);
    _operand->findExpr(cb);
}


void UnaryExpr::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::replaceExpr(%p, %p)\n", oldExp, newExp);
    assert(oldExp == _operand);
    _operand = newExp;
    if (newExp)
        newExp->setParent(this);
}


BinaryExpr::BinaryExpr(BinaryOp _op, Expression * lExpr, Expression * rExpr,
                       const Location l):Expression(ET_BinaryExpr, l)
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::BinaryExpr(%d, %p, %p, %p)\n", _op, lExpr, rExpr, &l);
    bOp = _op;
    _leftExpr = lExpr;
    _rightExpr = rExpr;
    if (lExpr)
        lExpr->setParent(this);
    if (rExpr)
        rExpr->setParent(this);
}


BinaryExpr::~BinaryExpr()
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::~BinaryExpr()\n");
    delete _leftExpr;
    delete _rightExpr;
}


Expression *BinaryExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::dup0()\n");
    BinaryExpr *ret = new BinaryExpr(bOp, _leftExpr->dup(), _rightExpr->dup(), location);

    ret->type = type;

    return ret;
}


Type *BinaryExpr::getType() const
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::getType()\n");
    if (type)
        return type->getRealType();

    if (bOp == BO_Member)
    {
        Type *bt = leftExpr()->getType();
        if(bt == NULL)
            return NULL;

        if (bt->isStructure())
        {
            assert((rightExpr()->etype & ET_Variable) == ET_Variable);
            Symbol *sym = ((Variable *) rightExpr())->name;

            assert(sym);
            u32 offset;
            Decl *decl = bt->findComponent(sym, &offset);

            assert(decl);
            return decl->form->getRealType();
        }
        else
        {
            // RHS had better be a swizzle!
            assert(rightExpr()->etype == ET_Variable);
            Variable *right = (Variable *) rightExpr();
            u32 sw_len = (u32) right->name->name.length();

            BTDetails details = getTypeDetails(bt->getBase()->typemask);

            //! Check for Number of components
            if(details.category == BT_Double_Group)
            {
                if (sw_len != 1 && sw_len != 2)
                    RecordSemanticError((BinaryExpr *)this, SemanticChecker::SCEKNull, "double data types can have 1 or 2 components");
            }
            else
            {
                if (sw_len < 1 && sw_len > 4)
                    RecordSemanticError((BinaryExpr *)this, SemanticChecker::SCEKNull, "Non double data types can have 1, 2, 3, or 4 components except void type");

            }

            //! Return appropriate type of given swizzle operator
            switch(details.category)
            {
                case BT_Char_Group:     //! Integer groups
                    *(Type **) & type = new BaseType(BT_Char << (sw_len - 1));
                    break;
                case BT_UChar_Group:
                    *(Type **) & type = new BaseType((BT_Char << (sw_len - 1)) | BT_UnSigned);
                    break;
                case BT_Short_Group:
                    *(Type **) & type = new BaseType(BT_Short << (sw_len - 1));
                    break;
                case BT_UShort_Group:
                    *(Type **) & type = new BaseType((BT_Short << (sw_len - 1)) | BT_UnSigned);
                    break;
                case BT_Int_Group:
                    *(Type **) & type = new BaseType(BT_Int << (sw_len - 1));
                    break;
                case BT_Long_Group:
                    *(Type **) & type = new BaseType(BT_Long << (sw_len - 1));
                    break;
                case BT_UInt_Group:
                    *(Type **) & type = new BaseType((BT_Int << (sw_len - 1)) | BT_UnSigned );
                    break;
                case BT_Float_Group:    //! Real groups
                    *(Type **) & type = new BaseType(BT_Float << (sw_len - 1));
                    break;
                case BT_Double_Group:
                    *(Type **) & type = new BaseType(BT_Double << (sw_len - 1));
                    break;
                default:
                    *(Type **) & type = new BaseType(BT_Int << (sw_len - 1));
                    break;
            }
            return type;
        }
    }
    else
    {
        return typeArithmetic(leftExpr() ? leftExpr()->getType() : NULL, 
            rightExpr() ? rightExpr()->getType() : NULL);
    }
}

#if BROOK_TO_IL_CODE_GEN_PATH
bril::store * BinaryExpr::get_il_store() const
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::get_il_store()\n");
    //fixme: get_il_store is incorrectly tag as "const"
    if (bOp == BO_Member)
    {
        Type *bt = leftExpr()->getType();

        if (bt == NULL)
        {
            semanticCheckRecord((Expression *) (this), SCROther, NULL, true);
            return NULL;
        }
        else if (bt->isStructure())
        {
            assert((rightExpr()->etype & ET_Variable) == ET_Variable);
            Symbol *sym = ((Variable *) rightExpr())->name;

            assert(sym);
            u32 offset;
            Decl *decl = bt->findComponent(sym, &offset);

            assert(decl);
            return get_struct_elt(leftExpr()->get_il_store(), offset);
        }
        else
        {
            // RHS had better be a swizzle!
            assert(rightExpr()->etype == ET_Variable);
            Variable *right = (Variable *) rightExpr();
            u32 sw_len = (u32) right->name->name.length();

            assert(sw_len >= 1 && sw_len <= 4);

            il_store = *leftExpr()->get_il_store();
            il_store.size = sw_len;
            if (isDouble(bt))
            {
                il_store.size *= 2;
                if (bril::set_swizzle_double(&il_store, right->name->name) != CGResultOK)
                    semanticCheckRecord((Expression *) (this), SCRVectorSwizzle, NULL, true);
            }
            else
            {
                if (bril::set_swizzle_float(&il_store, right->name->name) != CGResultOK)
                    semanticCheckRecord((Expression *) (this), SCRVectorSwizzle, NULL, true);
            }

            return &il_store;
        }
    }
    else
        return Expression::get_il_store();
}
#endif // BROOK_TO_IL_CODE_GEN_PATH
#define needAvoidWarning(op) (op == BO_Shl || op == BO_Shr)

void BinaryExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::print(%p)\n", &out);
    assert(leftExpr());

    //16 is the default precedence for non-expression operand
    if (leftExpr()->precedence() < precedence() || 
        (leftExpr()->precedence()!=16 && needAvoidWarning(bOp)))
        out << "(" << *leftExpr() << ")";
    else
        out << *leftExpr();

    bool useSpace = !((bOp == BO_Member) || (bOp == BO_PtrMember));

    if (useSpace)
        out << " ";

    PrintBinaryOp(out, bOp);

    if (useSpace)
        out << " ";

    assert(rightExpr());

    if ((rightExpr()->precedence() < precedence())
        || ((rightExpr()->precedence() == precedence()) && (rightExpr()->etype != ET_Variable)))
    {
        out << "(" << *rightExpr() << ")";
    }
    else
        out << *rightExpr();

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

void BinaryExpr::printSubExprInfo(std::ostream& out) const
{
    _leftExpr->printExprWithTypeInfo(out);
    _rightExpr->printExprWithTypeInfo(out);
}
#if BROOK_TO_IL_CODE_GEN_PATH
void UnaryExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "UnaryExpr::print_il(%p)\n", &out);
    _operand->print_il(out);

    bril::emitOp(out, this, uOp, get_il_store(), _operand->get_il_store());
}



void BinaryExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::print_il(%p)\n", &out);
    if (bOp == BO_Member)
    {
        leftExpr()->print_il(out);
    }
    else
    {
        assert(leftExpr());

        leftExpr()->print_il(out);
        rightExpr()->print_il(out);

        if (bOp != BO_Comma)
        {
            if (bOp == BO_Minus && isDouble(getType()))
            {
                //work around the fact that there is no DSub in IL
                (const_cast<BinaryExpr *>(this))->bOp = BO_Plus;
                bril::store *rightop = rightExpr()->get_il_store();
                bril::negate(rightop);
            }
            bril::emitOp(out, this, bOp, get_il_store(), 
                leftExpr()->get_il_store(), 
                rightExpr()->get_il_store());
        }
    }
}

#endif // BROOK_TO_IL_CODE_GEN_PATH

int BinaryExpr::precedence() const
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::precedence()\n");
    switch (bOp)
    {
    case BO_Plus:               // +
    case BO_Minus:              // -
        return 12;

    case BO_Mult:               // *
    case BO_Div:                // /
    case BO_Mod:                // %
        return 13;

    case BO_Shl:                // <<
    case BO_Shr:                // >>
        return 11;

    case BO_BitAnd:             // &
        return 8;

    case BO_BitXor:             // ^
        return 7;

    case BO_BitOr:              // |
        return 6;

    case BO_And:                // &&
        return 5;

    case BO_Or:         // ||
        return 4;

    case BO_Comma:              // x,y
        return 1;

    case BO_Member:             // x.y
    case BO_PtrMember:          // x->y
        return 16;

    case BO_Assign:             // An AssignExpr
    case BO_Rel:                // A RelExpr
        break;
    }

    return 1;
}

#ifdef SEMANTIC_CHECK

static bool 
isVectorSwizzle(const BaseType * type, const std::string & swizzle)
{
    LOG(LOG_FN_ENTRY, "isVectorSwixzzle(%p, %s)\n", type, swizzle.c_str());
    static const char srcSwizzle[5] = "xyzw";
    BaseTypeInfo *info = getBaseTypeInfo(type->typemask);

    if (info == NULL)
        return false;

    for (int i = 0; i < static_cast < int >(swizzle.size()); i++)
    {
        char c = swizzle[i];
        bool found = false;

        for (int j = 0; j < info->component; j++)
        {
            if (c == srcSwizzle[j])
            {
                found = true;
            }
        }
        if (found == false)
            return false;
    }

    return true;
}

static bool 
isVectorSwizzleRepeated(const BaseType * type, const std::string & swizzle, std::string& msg)
{
    LOG(LOG_FN_ENTRY, "isVectorSwixzzle(%p, %s)\n", type, swizzle.c_str());
    static const char srcSwizzle[5] = "xyzw";
    int srcSwizzleCount[4] = {0};
    BaseTypeInfo *info = getBaseTypeInfo(type->typemask);
    msg = "";

    if (info == NULL)
        return false;

    for (int i = 0; i < static_cast < int >(swizzle.size()); i++)
    {
        char c = swizzle[i];
        bool found = false;

        for (int j = 0; j < info->component; j++)
        {
            if (c == srcSwizzle[j])
            {
                found = true;
                srcSwizzleCount[j] = srcSwizzleCount[j] + 1;
                if(srcSwizzleCount[j] > 1)
                {
                    msg = c;
                    msg.append(" component repeated");
                    return false;
                }
            }
        }
        if (found == false)
            return false;
    }

    return true;
}

//! Check semantics of integer operators
void 
BinaryExpr::semanticsForIntOps(BinaryOperandsDetails details)
{
    //! Give error if any operand is not a Integer type
    if(isIntegerType(details.leftOperandDetails) == false)
        RecordSemanticError(this, SemanticChecker::SCEKNull, "left operand is not a Integer type");
    if(isIntegerType(details.rightOperandDetails) == false)
        RecordSemanticError(this, SemanticChecker::SCEKNull, "right operand is not a Integer type");

    int promotionID;
    promotionID = requiredTypePromotion(details.leftOperandDetails, details.rightOperandDetails);
    SemanticChecker::SCWarningLevel warningLevel = SemanticChecker::SCWarningLevelOne;

    //! Check semantics on type and components
    if(isIntegerType(details.leftOperandDetails) && isIntegerType(details.rightOperandDetails))
    {
        switch(details.category)
        {
            //! For Different types 
            case BT_DiffTypes_UnequalComponents:
                if(bOp == BO_Shl || bOp == BO_Shr)
                {
                    std::ostringstream tmpStr;
                    if(details.leftOperandDetails.components > details.rightOperandDetails.components)
                    {
                        tmpStr << "shift count not specified for all components";
                    }
                    else
                    {
                        tmpStr << "unused shift count components";
                    }
                    
                    if(globals.enableStrongTypeChecking)
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                    else
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                    }
                }
                else
                {
                    std::ostringstream tmpStr;
                    if(promotionID == -1)
                    {
                        tmpStr << "implicit conversion from left type to right type ";
                        if(details.leftOperandDetails.components > details.rightOperandDetails.components)
                        {
                            tmpStr << ": unused left operand components ";
                        }
                        else
                        {
                            tmpStr << ": uninitialized left operand components ";
                        }
                        if(checkForConversionLose(details.rightOperandDetails, details.leftOperandDetails))
                        {
                            tmpStr << " and possible lose of data";
                        }
                    }
                    else if(promotionID == 1)
                    {
                        tmpStr << "implicit conversion from right type to left type ";
                        if(details.rightOperandDetails.components > details.leftOperandDetails.components)
                        {
                            tmpStr << ": unused right operand components ";
                        }
                        else
                        {
                            tmpStr << ": uninitialized right operand components ";
                        }
                        if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
                        {
                            tmpStr << " and possible lose of data";
                        }
                    }
                    else if(promotionID == 0)
                    {
                        tmpStr << "implicit conversion from right type to left type ";
                        if(details.rightOperandDetails.components > details.leftOperandDetails.components)
                        {
                            tmpStr << ": uninitialized left operand components ";
                        }
                        else
                        {
                            tmpStr << ": uninitialized right operand components ";
                        }
                    }
                    else
                    {
                        assert(0);
                        LOG(LOG_ERROR, "internal error: unexpected promotion ID");
                        return;
                    }
                    if(globals.enableStrongTypeChecking)
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), warningLevel);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                    else
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), warningLevel);
                    }
                }
                break;
            case BT_DiffTypes_EqualComponents:
                if(bOp != BO_Shl && bOp != BO_Shr)
                {
                    std::ostringstream tmpStr;
                    if(promotionID == -1)
                    {
                        tmpStr << "implicit conversion from left type to right type ";
                        if(checkForConversionLose(details.rightOperandDetails, details.leftOperandDetails))
                        {
                            tmpStr << ": possible lose of data";
                        }
                    }
                    else if(promotionID == 1)
                    {
                        tmpStr << "implicit conversion from right type to left type ";
                        if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
                        {
                            tmpStr << ": possible lose of data";
                        }
                    }
                    else if(promotionID == 0)
                    {
                        return;
                    }
                    else
                    {
                        assert(0);
                        LOG(LOG_ERROR, "internal error: unexpected promotion ID");
                        return;
                    }

                    if(globals.enableStrongTypeChecking)
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), warningLevel);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                    else
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), warningLevel);
                    }
                }
                break;
            //! For Same types having unequal components
            case BT_SameTypes_UnequalComponents:
                if(bOp == BO_Shl || bOp == BO_Shr)
                {
                    std::ostringstream tmpStr;
                    if(details.leftOperandDetails.components > details.rightOperandDetails.components)
                    {
                        tmpStr << "shift count not specified for all components";
                    }
                    else
                    {
                        tmpStr << "unused shift count components";
                    }
                    if(globals.enableStrongTypeChecking)
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                    else
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                    }
                }
                else
                {
                    std::ostringstream tmpStr;
                    if(details.rightOperandDetails.components > details.leftOperandDetails.components)
                    {
                        tmpStr << "implicit conversion from right type to left type "
                               << ": uninitialized left operand components ";
                    }
                    else
                    {
                        tmpStr << "implicit conversion from left type to right type "
                               << ": uninitialized right operand components ";
                    }
                    if(globals.enableStrongTypeChecking)
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                    else
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                    }
                }
            case BT_SameTypes_EqualComponents:
                //! do nothing
                break;

        }
    }
}

//! Check semantics of operators which support both integer types and real types
void 
BinaryExpr::semanticsForIntRealOps(BinaryOperandsDetails details)
{
    if(globals.enableStrongTypeChecking)
    {
        if(!checkForTypeAndComponents(details.leftOperandDetails, details.rightOperandDetails))
        {
            RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, "Mismatched operands: both must have same type and same number of components", SemanticChecker::SCWarningLevelOne);
            RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
        }
    }
    else
    {
        int promotionID;
        std::ostringstream tmpStr;
        promotionID = requiredTypePromotion(details.leftOperandDetails, details.rightOperandDetails);
        SemanticChecker::SCWarningLevel warningLevel = SemanticChecker::SCWarningLevelOne;

        switch(details.category)
        {
            //! For Different types 
            case BT_DiffTypes_UnequalComponents:
                //! type warnings
                if(promotionID == -1)
                {
                    tmpStr << "implicit conversion from left type to right type ";
                    if(details.leftOperandDetails.components > details.rightOperandDetails.components)
                    {
                        tmpStr << ": unused left operand components ";
                    }
                    else
                    {
                        tmpStr << ": uninitialized left operand components ";
                    }
                    if(checkForConversionLose(details.rightOperandDetails, details.leftOperandDetails))
                    {
                        tmpStr << " and possible lose of data";
                    }
                }
                else if(promotionID == 1)
                {
                    tmpStr << "implicit conversion from right type to left type ";
                    if(details.rightOperandDetails.components > details.leftOperandDetails.components)
                    {
                        tmpStr << ": unused right operand components ";
                    }
                    else
                    {
                        tmpStr << ": uninitialized right operand components ";
                    }
                    if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        tmpStr << " and possible lose of data";
                    }
                }
                else if(promotionID == 0)
                {
                    tmpStr << "implicit conversion from right type to left type ";
                    if(details.rightOperandDetails.components > details.leftOperandDetails.components)
                    {
                        tmpStr << ": uninitialized left operand components ";
                    }
                    else
                    {
                        tmpStr << ": uninitialized right operand components ";
                    }
                }
                else
                {
                    assert(0);
                    LOG(LOG_ERROR, "internal error: unexpected promotion ID");
                    return;
                }
                
                RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), warningLevel);
                break;
            case BT_DiffTypes_EqualComponents:
                //! Type warnings
                if(promotionID == -1)
                {
                    tmpStr << "implicit conversion from left type to right type ";
                    if(checkForConversionLose(details.rightOperandDetails, details.leftOperandDetails))
                    {
                        tmpStr << ": possible lose of data";
                    }
                }
                else if(promotionID == 1)
                {
                    tmpStr << "implicit conversion from right type to left type ";
                    if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        tmpStr << ": possible lose of data";
                    }
                }
                else if(promotionID == 0)
                {
                    return;
                }
                else
                {
                    assert(0);
                    LOG(LOG_ERROR, "internal error: unexpected promotion ID");
                    return;
                }
                
                RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), warningLevel);
                break;
            //! For Same types having unequal components
            case BT_SameTypes_UnequalComponents:
                //! Components warnings
                if(details.rightOperandDetails.components > details.leftOperandDetails.components)
                {
                    tmpStr << "implicit conversion from right type to left type "
                           << ": uninitialized left operand components ";
                }
                else
                {
                    tmpStr << "implicit conversion from left type to right type "
                           << ": uninitialized right operand components ";
                }
                RecordSemanticWarn(this, SemanticChecker::SCWBinaryExpr, tmpStr.str().c_str(), warningLevel);
                break;
            case BT_SameTypes_EqualComponents:
                //! do nothing
                break;

        }
    }
}

void 
BinaryExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::semanticCheck(%d)\n", isKernel);
    Type *leftType = NULL;
    Type *rightType = NULL;
    if (isKernel)
    {
        if (bOp == BO_PtrMember)
            RecordSemanticError(this, SemanticChecker::SCEKExprOp, " pointer dereference(->)");
    }

    _leftExpr->semanticCheck(isKernel);
    if (bOp != BO_Member)
    {
        _rightExpr->semanticCheck(isKernel);
    }

    if(isKernel)
    {
        //! Left operand type is array type?
        if(_leftExpr->etype == ET_Variable)
        {
            Variable *v = AsVariable(_leftExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as left operand";
                RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str()); 
                return;
            }
        }

        //! Right operand type is array type?
        if(_rightExpr->etype == ET_Variable && bOp != BO_Member)
        {
            Variable *v = AsVariable(_rightExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as right operand";
                RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str()); 
                return;
            }
        }
    }
    if(isKernel && bOp == BO_Member)
    {

        //check against the builtin short vectors, for valid member 
        leftType = _leftExpr->getType();
        if (leftType && _rightExpr->etype == ET_Variable)
        {
            BaseType *strType = leftType->getBase();
            Variable *expVar = AsVariable(_rightExpr);

            if (strType->isStructure())
            {
                //Symbol* sym = ((Variable*)rightExpr())->name;
                //Decl* decl = bt->findComponent( sym, &offset );
            }
            else
            {
                if(isScalarType(getTypeDetails(strType->typemask)))
                {
                    std::ostringstream tmpmsg;
                    tmpmsg << "Illegal to use swizzle on scalar types";
                    RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str());
                }

                if (isVectorSwizzle(strType, expVar->name->name) == false)
                {
                    std::ostringstream tmpmsg;
                    tmpmsg << "unrecognized field/swizzle ";
                    _rightExpr->print(tmpmsg);
                    RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str());
                }
            }                   
        }
        //otherwise, left expr may use undefined variable
    }

    if(isKernel && bOp != BO_Member)
    {
        leftType = _leftExpr->getType();
        rightType = _rightExpr->getType();
        if(leftType != NULL && rightType != NULL)
        {

            BinaryOperandsDetails details = getBinaryOperandsDetails(leftType->getBase()->typemask, 
                                                                      rightType->getBase()->typemask);

            //! Check types only if both are Arithmetic types 
            //! Note: Handle unsigned char, char, unsigned short, short data types
            if(isArithmeticType(details.leftOperandDetails) && isArithmeticType(details.rightOperandDetails))
            {
                if(isIntegerOp())
                {
                    //! Check semantics of integer operators
                    semanticsForIntOps(details);
                }
                else
                {
                    //! Check semantics of operators which support both integer types and real types
                    semanticsForIntRealOps(details);
                }
            }
            else
            {
                if(isArithmeticType(details.leftOperandDetails) == false)
                    RecordSemanticError(this, SemanticChecker::SCEKNull, "left operand is not a Arithmetic type");
                if(isArithmeticType(details.rightOperandDetails) == false)
                    RecordSemanticError(this, SemanticChecker::SCEKNull, "right operand is not a Arithmetic type");
            }
        }
        else
        {
            assert(0);
            std::ostringstream tmpStr;
            print(tmpStr);
            LOG(LOG_ERROR, "%s : %d -> Internal error for %s", __FILE__, __LINE__, tmpStr.str().c_str());
        }
    }
}

#endif

void BinaryExpr::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::findExpr(%p)\n", cb);
    (cb) (this);
    //_leftExpr = (cb)(_leftExpr);
    if (_leftExpr)
        _leftExpr->findExpr(cb);

    //_rightExpr = (cb)(_rightExpr);
    if (_rightExpr)
        _rightExpr->findExpr(cb);
}


void BinaryExpr::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "BinaryExpr::replaceExpr(%p, %p)\n", oldExp, newExp);
    if (oldExp == _rightExpr)
        _rightExpr = newExp;
    else if (oldExp == _leftExpr)
        _leftExpr = newExp;
    else
        assert(0);
    if (newExp)
        newExp->setParent(this);
}


TrinaryExpr::TrinaryExpr(Expression * cExpr,
                         Expression * tExpr, Expression * fExpr, const Location l):Expression(ET_TrinaryExpr, l)
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::TrinaryExpr(%p, %p, %p, %p)\n", cExpr, tExpr, fExpr, &l);
    _condExpr = cExpr;
    _trueExpr = tExpr;
    _falseExpr = fExpr;
    if (cExpr)
        cExpr->setParent(this);
    if (tExpr)
        tExpr->setParent(this);
    if (fExpr)
        fExpr->setParent(this);
}


TrinaryExpr::~TrinaryExpr()
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::~TrinaryExpr()\n");
    delete _condExpr;
    delete _trueExpr;
    delete _falseExpr;
}


Expression *TrinaryExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::dup0()\n");
    TrinaryExpr *ret = new TrinaryExpr(_condExpr->dup(),
                                       _trueExpr->dup(),
                                       _falseExpr->dup(), location);

    ret->type = type;
    return ret;
}


void TrinaryExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::print(%p)\n", &out);
    if(globals.enableSematicCheck == false)
    {
        out << "(" << *condExpr() << ")";

        out << " ? ";
        out << "(" << *trueExpr() << ")";

        out << " : ";
        out << "(" << *falseExpr() << ")";
    }
    else
    {
        out << *condExpr() << " ? " << *trueExpr() << " : "<< *falseExpr();
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}


void TrinaryExpr::printSubExprInfo(std::ostream& out) const
{
    _trueExpr->printExprWithTypeInfo(out);
    _falseExpr->printExprWithTypeInfo(out);
}

#if BROOK_TO_IL_CODE_GEN_PATH
void TrinaryExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::print_il(%p)\n", &out);
    trueExpr()->print_il(out);
    falseExpr()->print_il(out);
    condExpr()->print_il(out);

    bril::store tmp = *condExpr()->get_il_store();

    out << IL_PREFIX_STRING << "cmov_logical " << getILRegName(bril::rs_dst_sel) << ", "<< bril::get_gpr_name(&tmp,
                                                                                                               bril::
                                                                                                               rs_src_sel);
    out << ", "<< trueExpr()->getILRegName(bril::rs_src_sel) << ", "<< falseExpr()->
        getILRegName(bril::rs_src_sel) << "\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH
#ifdef SEMANTIC_CHECK
void 
TrinaryExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::semanticCheck(%d)\n", isKernel);
    _trueExpr->semanticCheck(isKernel);
    _falseExpr->semanticCheck(isKernel);
    _condExpr->semanticCheck(isKernel);
    
    //! semantics of condition expression
    semanticChecker.checkConditionalSemantics(_condExpr, isKernel);

    //! Semantic check for true and false expressions
    if(isKernel)
    {
        Type *trueExprType = _trueExpr->getType();
        Type *falseExprType = _falseExpr->getType();

        if(trueExprType != NULL && falseExprType != NULL)
        {

            BinaryOperandsDetails details = getBinaryOperandsDetails(trueExprType->getBase()->typemask, 
                                                                      falseExprType->getBase()->typemask);
            //! Check types only if both are Arithmetic types 
            //! Note: Handle unsigned char, char, unsigned short, short data types
            if(isArithmeticType(details.leftOperandDetails) && isArithmeticType(details.rightOperandDetails))
            {

                if(globals.enableStrongTypeChecking)
                {
                    if(!checkForTypeAndComponents(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        std::ostringstream warnMsg;
                        warnMsg << "Mismatched types : True expression type and false expression type must have same types and same number of components";
                        RecordSemanticWarn(this, SemanticChecker::SCWTrinaryExpr, warnMsg.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                }
                else
                {
                    Type *trinaryType = getType();
                    BaseTypeInfo *trueExprTypeInfo = getBaseTypeInfo(trueExprType->getBase()->typemask);
                    BaseTypeInfo *falseExprTypeInfo = getBaseTypeInfo(falseExprType->getBase()->typemask);

                    //! find promotion type and make it as left operand
                    Type *promotionType = typeArithmetic(trueExprType, falseExprType);
                    BinaryOperandsDetails tmpDetails = details;
                    std::string warnMsg = "";
                    if(trinaryType->getBase()->typemask !=  promotionType->getBase()->typemask)
                    {
                        warnMsg.append("conversion from ");
                        warnMsg.append(trueExprTypeInfo->brTypeName);
                        warnMsg.append(" to ");
                        warnMsg.append(falseExprTypeInfo->brTypeName);
                        tmpDetails.leftOperandDetails = details.rightOperandDetails;
                        tmpDetails.rightOperandDetails = details.leftOperandDetails;
                    }
                    else
                    {
                        warnMsg.append("conversion from ");
                        warnMsg.append(falseExprTypeInfo->brTypeName);
                        warnMsg.append(" to ");
                        warnMsg.append(trueExprTypeInfo->brTypeName);
                        tmpDetails = tmpDetails;
                    }
                    if(getWarnMsgForBinarayOperands(tmpDetails, warnMsg))
                        RecordSemanticWarn(this, SemanticChecker::SCWTrinaryExpr, warnMsg.c_str(), SemanticChecker::SCWarningLevelOne);
                }
            }
            else
            {
                if(isArithmeticType(details.leftOperandDetails) == false)
                    RecordSemanticError(this, SemanticChecker::SCETrinaryExpr, "left operand is not a Arithmetic type");
                if(isArithmeticType(details.rightOperandDetails) == false)
                    RecordSemanticError(this, SemanticChecker::SCETrinaryExpr, "right operand is not a Arithmetic type");
            }
        }
        else
        {
            assert(0);
            std::ostringstream tmpStr;
            print(tmpStr);
            LOG(LOG_ERROR, "%s : %d -> Internal error for %s", __FILE__, __LINE__, tmpStr.str().c_str());
        }
    }
}
#endif
Type *TrinaryExpr::getType() const
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::getType()\n");
    if (type)
        return type->getRealType();

    return typeArithmetic(condExpr()->getType(), trueExpr()->getType(), trueExpr()->getType());
}


void TrinaryExpr::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::findExpr(%p)\n", cb);
    (cb) (this);
    _condExpr->findExpr(cb);
    //_trueExpr = (cb)(_trueExpr);
    _trueExpr->findExpr(cb);
    //_falseExpr = (cb)(_falseExpr);
    _falseExpr->findExpr(cb);
}


void TrinaryExpr::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "TrinaryExpr::replaceExpr(%p, %p)\n", oldExp, newExp);
    if (oldExp == _condExpr)
        _condExpr = newExp;
    else if (oldExp == _trueExpr)
        _trueExpr = newExp;
    else if (oldExp == _falseExpr)
        _falseExpr = newExp;
    else
        assert(0);
    if (newExp)
        newExp->setParent(this);
}


AssignExpr::AssignExpr(AssignOp _op, Expression * lExpr, Expression * rExpr,
                       const Location l):BinaryExpr(BO_Assign, lExpr, rExpr, l)
{
    LOG(LOG_FN_ENTRY, "AssignExpr::AssignExpr(%d, %p, %p, {%d, %d, %s})\n", _op, lExpr, rExpr,
        l.line, l.column, l.file);
    aOp = _op;
    type = lExpr->type;
}


AssignExpr::~AssignExpr()
{
    LOG(LOG_FN_ENTRY, "AssignExpr::~AssignExpr()\n");
}


Expression *AssignExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "AssignExpr::dup0()\n");
    AssignExpr *ret = new AssignExpr(aOp, _leftExpr->dup(), _rightExpr->dup(), location);

    ret->type = type;
    return ret;
}

void AssignExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "AssignExpr::print(%p)\n", &out);

    bool isSharedArrayThere = false;
    IndexExpr *index = NULL;
    if(lValue()->etype == ET_IndexExpr)
    {
        index = AsIndexExpr(lValue());
        if(index && index->accessType == IndexWriteAccess && globals.enableSematicCheck == false) //! for Array variable
        {
            Type* arrayType = index->array->type;

            if(arrayType)
            {
                if(arrayType->type == TT_Array && arrayType->getQualifiers() == TQ_Shared)
                {
                    isSharedArrayThere = true;
                    std::ostringstream offset;
                    if(index->_subscript->etype == ET_BinaryExpr)
                    {
                        BinaryExpr *mainExpr = static_cast<BinaryExpr *>(index->_subscript);
                        if(mainExpr->op() == BO_Plus)
                        {
                            if(mainExpr->_leftExpr->etype == ET_Constant)
                                mainExpr->_leftExpr->print(offset);
                            else
                                mainExpr->_rightExpr->print(offset);
                        }
                        else
                        {
                            mainExpr = static_cast<BinaryExpr *>(mainExpr->_leftExpr);
                            if(mainExpr->_leftExpr->etype == ET_Constant)
                                mainExpr->_leftExpr->print(offset);
                            else
                                mainExpr->_rightExpr->print(offset);
                        }
                        out << "WriteLds(" << atoi(offset.str().c_str()) * 4 << ", " << *rValue() << ")";
                    }

                }
            }
        }
    }
    if(lValue()->etype == ET_BinaryExpr)
    {
        BinaryExpr *expr = static_cast<BinaryExpr*>(lValue());
        if(expr->op() == BO_Member)
        {
            if(expr->_leftExpr->etype == ET_IndexExpr)
            {
                index = AsIndexExpr(expr->_leftExpr);
                if(index && index->accessType == IndexWriteAccess && globals.enableSematicCheck == false) //! for Array variable
                {
                    isSharedArrayThere = true;
                    Type* arrayType = index->array->type;

                    if(arrayType)
                    {
                        if(arrayType->type == TT_Array && arrayType->getQualifiers() == TQ_Shared)
                        {
                            std::ostringstream offset;
                            if(index->_subscript->etype == ET_BinaryExpr)
                            {
                                BinaryExpr *mainExpr = static_cast<BinaryExpr *>(index->_subscript);
                                if(mainExpr->op() == BO_Plus)
                                {
                                    if(mainExpr->_leftExpr->etype == ET_Constant)
                                        mainExpr->_leftExpr->print(offset);
                                    else
                                        mainExpr->_rightExpr->print(offset);
                                }
                                else
                                {
                                    mainExpr = static_cast<BinaryExpr *>(mainExpr->_leftExpr);
                                    if(mainExpr->_leftExpr->etype == ET_Constant)
                                        mainExpr->_leftExpr->print(offset);
                                    else
                                        mainExpr->_rightExpr->print(offset);
                                }
                                out << "WriteLds(" << atoi(offset.str().c_str()) * 4 << ", " << *rValue() << ")";
                            }
                        }
                    }
                }
            }

            if(isSharedArrayThere == true)
                out << "." << *expr->rightExpr();
        }
    }
    

    if(isSharedArrayThere == false)
    {
        if (lValue()->precedence() < precedence())
            out << "(" << *lValue() << ")";
        else
            out << *lValue();

        out << " ";
        PrintAssignOp(out, aOp);
        out << " ";

        if (rValue()->precedence() < precedence())
            out << "(" << *rValue() << ")";
        else
            out << *rValue();
    }

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}


void AssignExpr::printSubExprInfo(std::ostream& out) const
{
    lValue()->printExprWithTypeInfo(out);
    rValue()->printExprWithTypeInfo(out);
}

#if BROOK_TO_IL_CODE_GEN_PATH
void AssignExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "AssignExpr::print_il(%p)\n", &out);
    rValue()->print_il(out);
    lValue()->print_il(out);

    switch (aOp)
    {
    case AO_Equal:
        bril::emitCopy(out, get_il_store(), rValue()->get_il_store());
        break;

    case AO_PlusEql:
        bril::emitOp(out, this, BO_Plus, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_DivEql:
        bril::emitOp(out, this, BO_Div, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_MinusEql:
        bril::emitOp(out, this, BO_Minus, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_MultEql:
        bril::emitOp(out, this, BO_Mult, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_ModEql:
        bril::emitOp(out, this, BO_Mod, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_ShlEql:
        bril::emitOp(out, this, BO_Shl, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_ShrEql:
        bril::emitOp(out, this, BO_Shr, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_BitAndEql:
        bril::emitOp(out, this, BO_BitAnd, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_BitXorEql:
        bril::emitOp(out, this, BO_BitXor, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    case AO_BitOrEql:
        bril::emitOp(out, this, BO_BitOr, get_il_store(), lValue()->get_il_store(), rValue()->get_il_store());
        break;

    default:
        //fixme: the print_il method is correctly defined as const method
        semanticCheckRecord(AssignConstCast(this), SCROperator, NULL, true);
        break;
    }

    bril::emitCopy(out, lValue()->get_il_store(), get_il_store(), true);
}

#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
//! Check semantics of shared array for write operation
void AssignExpr::checkArrayIndexingSemantics(IndexExpr *index)
{
    //! Do semantics for Gather arrays
    Expression* arrayExp = index->getArrayVariableExpr();

    if(arrayExp && arrayExp->etype == ET_Variable)
    {
        Variable *arrayVar = AsVariable(arrayExp);

        //! return if variable is not declared
        if (arrayVar->name->entry == NULL || arrayVar->name->entry->uVarDecl == NULL)
            return;

        ArrayType *arrayType = AsArrayType(arrayVar->type);

        TypeQual qualifier = arrayType->getQualifiers();
        if(arrayType->isKernelParam())
        {
            //! For non-outputs
            if((qualifier & TQ_Out) == 0 && (qualifier & TQ_Reduce) == 0 )
            {
                if(arrayType->isArray())
                    RecordSemanticError(this, SemanticChecker::SCEAssignExpr, "Gather arrays are read only arrays");
            }
        }
        else
        {
            //! for local constant Array
            if((qualifier & TQ_Const) == true)
                RecordSemanticError(this, SemanticChecker::SCEAssignExpr, "lvalue is a constant type");
        }
    }

    Type* arrayType = index->array->type;

    if(arrayType)
    {
        if(arrayType->type == TT_Array && arrayType->getQualifiers() == TQ_Shared)
        {
            index->accessType = IndexWriteAccess;
            std::ostringstream offset;
            if(index->_subscript->etype == ET_BinaryExpr)
            {
                BinaryExpr *mainExpr = static_cast<BinaryExpr *>(index->_subscript);
                Constant *strideExpr = NULL;
                Expression *threadIdExpr = NULL;
                Constant *offsetExpr = NULL;
                std::ostringstream msg;
                msg << "Shared array index expression for write must be as follows \n"
                    << "\n\tIndex expression = stride * threadId + offset \n"
                    << "\n\t                    or                        \n"
                    << "\n\tIndex expression = threadId * stride + offset \n"
                    << "\n"
                    << "\tWhere stride is a constant integer which is equal to SharedArraySize/GroupSize\n"
                    << "\t      threadId must be a instanceInGroup().x\n"
                    << "\t      offset is a constant integer and range 0 <= offset < stride \n";
                bool isWriteLdsExpr = true;
                if(mainExpr->op() == BO_Plus)
                {
                    if(mainExpr->_leftExpr->etype == ET_Constant)
                    {
                        offsetExpr = static_cast<Constant *>(mainExpr->_leftExpr);
                        BinaryExpr *mulExpr = static_cast<BinaryExpr *>(mainExpr->_rightExpr);
                        if(mulExpr->op() == BO_Mult)
                        {
                            if(mulExpr->_leftExpr->etype == ET_Constant && mulExpr->_rightExpr->etype == ET_Constant)
                            {
                                std::ostringstream errorMsg;
                                errorMsg << "one of the multiplication operand must be instanceInGroup().x \n"
                                         << "\t" << msg.str().c_str();
                                RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                                isWriteLdsExpr = false;
                            }
                            else
                            {
                                if(mulExpr->_leftExpr->etype == ET_Constant)
                                {
                                    strideExpr = static_cast<Constant *>(mulExpr->_leftExpr);
                                    threadIdExpr = mulExpr->_rightExpr;
                                }
                                else
                                {
                                    strideExpr = static_cast<Constant *>(mulExpr->_rightExpr);
                                    threadIdExpr = mulExpr->_leftExpr;
                                }
                            }
                        }
                        else
                        {
                            RecordSemanticError(index, SemanticChecker::SCEKExprOther, msg.str().c_str());
                            isWriteLdsExpr = false;
                        }
                    }
                    else if(mainExpr->_rightExpr->etype == ET_Constant)
                    {
                        offsetExpr = static_cast<Constant *>(mainExpr->_rightExpr);
                        BinaryExpr *mulExpr = static_cast<BinaryExpr *>(mainExpr->_leftExpr);
                        if(mulExpr->op() == BO_Mult)
                        {
                            if(mulExpr->_leftExpr->etype == ET_Constant && mulExpr->_rightExpr->etype == ET_Constant)
                            {
                                std::ostringstream errorMsg;
                                errorMsg << "one of the multiplication operand must be instanceInGroup().x \n"
                                         << "\t" << msg.str().c_str();
                                RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                                isWriteLdsExpr = false;
                            }
                            else
                            {
                                if(mulExpr->_leftExpr->etype == ET_Constant)
                                {
                                    strideExpr = static_cast<Constant *>(mulExpr->_leftExpr);
                                    threadIdExpr = mulExpr->_rightExpr;
                                }
                                else
                                {
                                    strideExpr = static_cast<Constant *>(mulExpr->_rightExpr);
                                    threadIdExpr = mulExpr->_leftExpr;
                                }
                            }
                        }
                        else
                        {
                            RecordSemanticError(index, SemanticChecker::SCEKExprOther, msg.str().c_str());
                            isWriteLdsExpr = false;
                        }
                    }
                    else
                    {
                            std::ostringstream errorMsg;
                            errorMsg << "offset of Shared array index expression must be a constant integer\n"
                                << msg.str().c_str();
                            RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                            isWriteLdsExpr = false;
                    }
                }
                else
                {
                    RecordSemanticError(index, SemanticChecker::SCEKExprOther, msg.str().c_str());
                    isWriteLdsExpr = false;
                }

                //! Check semantics of stride, threadId and offset expressions
                if(isWriteLdsExpr == true)
                {
                    ArrayType *type = static_cast<ArrayType *>(arrayType);

                    //! Skip if array semantics are wrong
                    if(type->isSemanticsOk == false)
                        return;
                        
                    int localDataShareSize = type->getTotalElementCount();
                    int groupSize = type->getGroupSize();
                    std::ostringstream errorMsg;
                    //! Check semantics of offset
                    if(offsetExpr->etype == CT_Int)
                    {
                        IntConstant *intConst = static_cast<IntConstant *>(offsetExpr);
                        if(intConst->lng < 0 || intConst->lng >= (long)(localDataShareSize / groupSize))
                        {
                            std::ostringstream errorMsg;
                            errorMsg << "offset value(" << intConst->lng <<") must be in the range of "
                                    << "0" << " <= offset < (localDataShareSize / groupSize)(" << (localDataShareSize / groupSize) << ")\n" 
                                     << "\t" << msg.str().c_str();
                            RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                        }
                    }
                    else
                    {
                        std::ostringstream errorMsg;
                        errorMsg << "offset must be a integer constant\n" 
                                 << "\t" << msg.str().c_str();
                        RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                    }

                    //! Check semantic of stride expression
                    if(strideExpr->etype == CT_Int)
                    {
                        IntConstant *intConst = static_cast<IntConstant *>(strideExpr);
                        if(intConst->lng != (long)(localDataShareSize / groupSize))
                        {
                            std::ostringstream errorMsg;
                            errorMsg << "stride value must be equal to " << (localDataShareSize / groupSize) << "\n" 
                                     << "\t" << msg.str().c_str();
                            RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                        }
                    }
                    else
                    {
                        std::ostringstream errorMsg;
                        errorMsg << "stride must be a integer constant\n" 
                                 << "\t" << msg.str().c_str();
                        RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                    }

                    //! Check semantic of threadId 
                    if(threadIdExpr->etype == ET_BinaryExpr)
                    {
                        BinaryExpr *expr = static_cast<BinaryExpr *>(threadIdExpr);
                        if(expr->bOp != BO_Member || expr->_leftExpr->etype != ET_BrtInstanceInGroupExpr)
                        {
                            std::ostringstream errorMsg;
                            errorMsg << "Shared array index expression must contain instanceInGroup().x \n"
                                     << "\t" << msg.str().c_str();
                            RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                        }
                    }
                    else
                    {
                        std::ostringstream errorMsg;
                        errorMsg << "Shared array index expression must contain instanceInGroup().x \n"
                                 << "\t" << msg.str().c_str();
                        RecordSemanticError(index, SemanticChecker::SCEKExprOther, errorMsg.str().c_str());
                    }
                }
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
//! \brief  Checks semantics of left expression
/////////////////////////////////////////////////////////////////////////////////////
void 
AssignExpr::lvalueSemantics(bool isKernel)
{
    //! Check inside kernel
    if(isKernel)
    {
        if(_leftExpr->etype == ET_Constant) //! for constant
            RecordSemanticError(this, SemanticChecker::SCEAssignExpr, "lvalue is a constant");
        else if(_leftExpr->etype == ET_FunctionCall) //! for function call
            RecordSemanticError(this, SemanticChecker::SCEAssignExpr, "lvalue is a function call");
        else if(_leftExpr->etype == ET_Variable) //! for variable
        {
            Variable *v = AsVariable(_leftExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as lvalue";
                RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpmsg.str().c_str()); 
            }
            TypeQual qualifier = type->getQualifiers();
            if(v->type->isKernelParam())
            {
                //! For non-outputs
                if((qualifier & TQ_Out) == 0 && (qualifier & TQ_Reduce) == 0 )
                {
                    if(type->isStream())
                        RecordSemanticError(this, SemanticChecker::SCEAssignExpr, "Input streams are read only streams");
                    else
                        RecordSemanticError(this, SemanticChecker::SCEAssignExpr, "Non-stream/Non Array inputs are not allowed to modify inside kernel");
                }
            }
            else
            {
                //! for local constant variable
                if((qualifier & TQ_Const) == true)
                    RecordSemanticError(this, SemanticChecker::SCEAssignExpr, "lvalue is a constant type");
            }

        }
        else if(_leftExpr->etype == ET_IndexExpr) //! for Array variable
        {
            IndexExpr *index = AsIndexExpr(_leftExpr);
            checkArrayIndexingSemantics(index);
        }
        else if(_leftExpr->etype == ET_BinaryExpr)
        {
            BinaryExpr *expr = AsBinaryExpr(_leftExpr);
            if(expr->bOp == BO_Member)
            {
                BaseType *exprType = expr->leftExpr()->getType()->getBase();
                Variable *expVar = AsVariable(expr->rightExpr());

                if (!exprType->isStructure())
                {
                    std::string msg = "";

                    if(isScalarType(getTypeDetails(exprType->typemask)))
                    {
                        std::ostringstream tmpmsg;
                        tmpmsg << "Illegal to use swizzle on scalar types";
                        RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str());
                    }

                    //! check for components repetition in lvalue expression
                    if (isVectorSwizzleRepeated(exprType, expVar->name->name, msg) == false)
                    {
                        std::ostringstream tmpmsg;
                        if(!msg.empty())
                        {
                            tmpmsg << "illegal to use components repetition in lvalue expression :" << msg.c_str() << " in ";
                        }
                        else
                        {
                            tmpmsg << "unrecognized field/swizzle ";
                        }
                        expr->print(tmpmsg);
                        RecordSemanticError(this, SemanticChecker::SCEKExprOther, tmpmsg.str().c_str());
                    }
                }

                if(expr->_leftExpr->etype == ET_IndexExpr)
                {
                    IndexExpr *index = AsIndexExpr(expr->_leftExpr);
                    checkArrayIndexingSemantics(index);
                }
            }
            else
            {
                assert(0);
                std::ostringstream tmpStr;
                print(tmpStr);
                LOG(LOG_ERROR, "%s : %d -> Internal error for %s : Unsupported binary expression as lvalue", __FILE__, __LINE__, tmpStr.str().c_str());
            }
        }
        else
        {
            assert(0);
            std::ostringstream tmpStr;
            print(tmpStr);
            LOG(LOG_ERROR, "%s : %d -> Internal error for %s", __FILE__, __LINE__, tmpStr.str().c_str());
        }
    }
    else
    {
        // Here handle outside kernel assignments
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//! \brief  Check types of lvalue and rvalue and reports proper warning
/////////////////////////////////////////////////////////////////////////////////////
void 
AssignExpr::checkTypeSemantics(bool isKernel)
{
    //! Check inside kernel
    if(isKernel)
    {
        Type* leftType = _leftExpr->getType();
        Type* rightType = _rightExpr->getType();

        //! Left operand type is array type?
        if(_leftExpr->etype == ET_Variable)
        {
            Variable *v = AsVariable(_leftExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as left operand";
                RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpmsg.str().c_str()); 
                return;
            }
        }

        //! Right operand type is array type?
        if(_rightExpr->etype == ET_Variable)
        {
            Variable *v = AsVariable(_rightExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as right operand";
                RecordSemanticError(this, SemanticChecker::SCEAssignExpr, tmpmsg.str().c_str()); 
                return;
            }
        }

        if(leftType != NULL && rightType != NULL)
        {
            if(leftType->isStructure() || rightType->isStructure())
            {
                std::ostringstream tmpStr;
                print(tmpStr);
                LOG(LOG_ERROR, "Structurewise semantics not handled still: %s\n", tmpStr.str().c_str());
                return;
            }
            //! Get left expression type information
            BaseTypeInfo* leftTypeInfo = getBaseTypeInfo(leftType->getBase()->typemask);

            //! Get right expression type information
            BaseTypeInfo* rightTypeInfo = getBaseTypeInfo(rightType->getBase()->typemask);

            //! Get details to compare both left and right types
            BinaryOperandsDetails details = getBinaryOperandsDetails(leftTypeInfo->brTypeID, rightTypeInfo->brTypeID);

            //! Check types only if both are Arithmetic types
            if(isArithmeticType(details.leftOperandDetails) && isArithmeticType(details.rightOperandDetails))
            {
                std::ostringstream tempStr;
                tempStr << "conversion from " << rightTypeInfo->brTypeName << " to " << leftTypeInfo->brTypeName;
                
                SemanticChecker::SCWarningLevel warninglevel = SemanticChecker::SCWarningLevelOne;
                if(globals.enableStrongTypeChecking)
                {
                    if(!checkForTypeAndComponents(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        std::ostringstream warnMsg;
                        warnMsg << "Mismatched types : " << tempStr.str().c_str();
                        RecordSemanticWarn(this, SemanticChecker::SCWAssignExpr, warnMsg.str().c_str(), warninglevel);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                }
                else
                {
                    std::ostringstream msg;
                    switch(details.category)
                    {
                        //! For Different types having unequal components
                        case BT_DiffTypes_UnequalComponents:
                            if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
                            {
                                msg << tempStr.str().c_str() << " : possible lose of data";
                            }
                            if(details.leftOperandDetails.components > details.rightOperandDetails.components)
                            {
                                msg << " and uninitialize components";
                            }
                            else
                            {
                                msg << " and lose of commponents";
                            }
                            
                            RecordSemanticWarn(this, SemanticChecker::SCWAssignExpr, msg.str().c_str(), warninglevel);
                            break;
                        //! For Different types having equal components
                        case BT_DiffTypes_EqualComponents:
                            if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
                            {
                                msg << tempStr.str().c_str() << " : possible lose of data";
                                RecordSemanticWarn(this, SemanticChecker::SCWAssignExpr, msg.str().c_str(), warninglevel);
                            }
                            
                            break;
                        //! For Same types having unequal components
                        case BT_SameTypes_UnequalComponents:
                            if(details.leftOperandDetails.components > details.rightOperandDetails.components)
                            {
                                msg << tempStr.str().c_str() << " : uninitialize components";
                            }
                            else
                            {
                                msg << tempStr.str().c_str() << " : lose of commponents";
                            }

                            RecordSemanticWarn(this, SemanticChecker::SCWAssignExpr, msg.str().c_str(), warninglevel);
                            break;
                        case BT_SameTypes_EqualComponents:
                            break;

                    }
                }
            }
            else
            {
                if(isArithmeticType(details.leftOperandDetails) == false)
                    RecordSemanticError(this, SemanticChecker::SCEKNull, "left operand is not a Arithmetic type");
                if(isArithmeticType(details.rightOperandDetails) == false)
                    RecordSemanticError(this, SemanticChecker::SCEKNull, "right operand is not a Arithmetic type");
            }

        }
        else
        {
            assert(0);
            std::ostringstream tmpStr;
            print(tmpStr);
            LOG(LOG_ERROR, "%s : %d -> Internal error for %s", __FILE__, __LINE__, tmpStr.str().c_str());
        }
    }
    else
    {
        // Here handle outside kernel assignments
    }
}
void 
AssignExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "AssignExpr::semanticCheck(%d)\n", isKernel);
    _leftExpr->semanticCheck(isKernel);   
    _rightExpr->semanticCheck(isKernel);

    //! Check lvalue semantics
    lvalueSemantics(isKernel);

    //! Check type semantics
    checkTypeSemantics(isKernel);
}
#endif
RelExpr::RelExpr(RelOp _op, Expression * lExpr, Expression * rExpr,
                 const Location l):BinaryExpr(BO_Rel, lExpr, rExpr, l)
{
    LOG(LOG_FN_ENTRY, "RelExpr::RelExpr(%d, %p, %p, %p)\n", _op, lExpr, rExpr, &l);
    rOp = _op;
}


RelExpr::~RelExpr()
{
    LOG(LOG_FN_ENTRY, "RelExpr::~RelExpr()\n");
}


Expression *RelExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "RelExpr::dup0()\n");
    RelExpr *ret = new RelExpr(rOp, _leftExpr->dup(), _rightExpr->dup(), location);

    ret->type = type;
    return ret;
}


void RelExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "RelExpr::print(%p)\n", &out);
    if (leftExpr()->precedence() <= precedence())
        out << "(" << *leftExpr() << ")";
    else
        out << *leftExpr();

    out << " ";
    PrintRelOp(out, rOp);
    out << " ";

    if (rightExpr()->precedence() <= precedence())
        out << "(" << *rightExpr() << ")";
    else
        out << *rightExpr();

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}


void RelExpr::printSubExprInfo(std::ostream& out) const
{
    _leftExpr->printExprWithTypeInfo(out);
    _rightExpr->printExprWithTypeInfo(out);
}

#if BROOK_TO_IL_CODE_GEN_PATH
void RelExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "RelExpr::print_il(%p)\n", &out);
    leftExpr()->print_il(out);
    rightExpr()->print_il(out);

    out << IL_PREFIX_STRING;
    PrintILRelOp(out, rOp);
    out << " " << getILRegName(bril::rs_dst_sel) << ", "<< leftExpr()->
        getILRegName(bril::rs_src_sel) << ", "<< rightExpr()->getILRegName(bril::rs_src_sel) << "\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void 
RelExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "RelExpr::semanticCheck(%d)\n", isKernel);
    _leftExpr->semanticCheck(isKernel);
    _rightExpr->semanticCheck(isKernel);

    if(isKernel)
    {
        //! Left operand type is array type?
        if(_leftExpr->etype == ET_Variable)
        {
            Variable *v = AsVariable(_leftExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as left operand";
                RecordSemanticError(this, SemanticChecker::SCEKNull, tmpmsg.str().c_str()); 
                return;
            }
        }

        //! Right operand type is array type?
        if(_rightExpr->etype == ET_Variable)
        {
            Variable *v = AsVariable(_rightExpr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as right operand";
                RecordSemanticError(this, SemanticChecker::SCEKNull, tmpmsg.str().c_str()); 
                return;
            }
        }
    }
    if(isKernel)
    {
        Type* leftType = _leftExpr->getType();
        Type* rightType = _rightExpr->getType();

        if(leftType == NULL)
        {
            std::ostringstream tmpmsg;
            _leftExpr->print(tmpmsg);
            LOG(LOG_ERROR, "Missing left expression type for %s in %s : %d", tmpmsg.str().c_str(), __FILE__, __LINE__);
            return;
        }

        if(rightType == NULL)
        {
            std::ostringstream tmpmsg;
            _rightExpr->print(tmpmsg);
            LOG(LOG_ERROR, "Missing right expression type for %s in %s : %d", tmpmsg.str().c_str(), __FILE__, __LINE__);
            return;
        }

        BinaryOperandsDetails details = getBinaryOperandsDetails(leftType->getBase()->typemask, 
                                                                      rightType->getBase()->typemask);

        //! Check types only if both are Arithmetic types 
        //! Note: Handle unsigned char, char, unsigned short, short data types
        if(isArithmeticType(details.leftOperandDetails) && isArithmeticType(details.rightOperandDetails))
        {
            std::string warnMsg = "";
            BinaryOperandsDetails tmpDetails;
            BaseTypeInfo *leftTypeInfo = getBaseTypeInfo(leftType->getBase()->typemask);
            BaseTypeInfo *rightTypeInfo = getBaseTypeInfo(rightType->getBase()->typemask);
            int promotionID = requiredTypePromotion(details.leftOperandDetails, details.rightOperandDetails);
            if(promotionID == -1)
            {
                tmpDetails.category = details.category;
                tmpDetails.leftOperandDetails = details.rightOperandDetails;
                tmpDetails.rightOperandDetails = details.leftOperandDetails;
                warnMsg.append("conversion from ");
                warnMsg.append(leftTypeInfo->brTypeName);
                warnMsg.append(" to ");
                warnMsg.append(rightTypeInfo->brTypeName);

                if(globals.enableStrongTypeChecking)
                {
                    if(!checkForTypeAndComponents(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        std::ostringstream tmpStr;
                        tmpStr << "Mismatched types : " << warnMsg.c_str();
                        RecordSemanticWarn(this, SemanticChecker::SCWRelExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                }
                else
                {
                    if(getWarnMsgForBinarayOperands(tmpDetails, warnMsg))
                        RecordSemanticWarn(this, SemanticChecker::SCWRelExpr, warnMsg.c_str(), SemanticChecker::SCWarningLevelOne);
                }
            }
            else if(promotionID == 1)
            {
                warnMsg.append("conversion from ");
                warnMsg.append(rightTypeInfo->brTypeName);
                warnMsg.append(" to ");
                warnMsg.append(leftTypeInfo->brTypeName);
                if(globals.enableStrongTypeChecking)
                {
                    if(!checkForTypeAndComponents(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        std::ostringstream tmpStr;
                        tmpStr << "Mismatched types : " << warnMsg.c_str(); 
                        RecordSemanticWarn(this, SemanticChecker::SCWRelExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                }
                else
                {
                    if(getWarnMsgForBinarayOperands(details, warnMsg))
                        RecordSemanticWarn(this, SemanticChecker::SCWRelExpr, warnMsg.c_str(), SemanticChecker::SCWarningLevelOne);
                }
            }
            else if(promotionID == 0)
            {
                return;
            }
            else
            {
                assert(0);
                LOG(LOG_ERROR, "internal error: unexpected promotion ID");
                return;
            }

        }
        else
        {
            if(isArithmeticType(details.leftOperandDetails) == false)
                RecordSemanticError(this, SemanticChecker::SCEKNull, "left operand is not a Arithmetic type");
            if(isArithmeticType(details.rightOperandDetails) == false)
                RecordSemanticError(this, SemanticChecker::SCEKNull, "right operand is not a Arithmetic type");

        }
    }
}
#endif
int RelExpr::precedence() const
{
    LOG(LOG_FN_ENTRY, "RelExpr::precedence()\n");
    switch (rOp)
    {
    case RO_Equal:
    case RO_NotEqual:
        return 9;

    default:
    case RO_Less:
    case RO_LessEql:
    case RO_Grtr:
    case RO_GrtrEql:
        return 10;
    }
}


CastExpr::CastExpr(Type * typeExpr, Expression * operand, const Location l):Expression(ET_CastExpr, l)
{
    LOG(LOG_FN_ENTRY, "CastExpr::CastExpr(%p, %p, %p)\n", typeExpr, operand, &l);
    castTo = typeExpr;
    expr = operand;
    type = typeExpr;
    if (operand)
        operand->setParent(this);
}


CastExpr::~CastExpr()
{
    LOG(LOG_FN_ENTRY, "CastExpr::~CastExpr()\n");
    //delete castTo;
    delete expr;
}


Expression *CastExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "CastExpr::dup0()\n");
    CastExpr *ret = new CastExpr(castTo, expr->dup(), location);

    ret->type = type;
    return ret;
}


void CastExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "CastExpr::print(%p)\n");
    if(globals.enableSematicCheck == false)
    {
        out << "(";
        castTo->printType(out, NULL, true, 0);
        out << ") ";

        out << "(";
        expr->print(out);
        out << ")";
    }
    else
    {
        out << "(";
        castTo->printType(out, NULL, true, 0);
        out << ") ";
        expr->print(out);
    }
#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

void CastExpr::printSubExprInfo(std::ostream& out) const
{
    printExprWithTypeInfo(out);
    expr->printExprWithTypeInfo(out);
}

#if BROOK_TO_IL_CODE_GEN_PATH
void CastExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "CastExpr::print_il(%p)\n", &out);
    expr->print_il(out);

    Type * castFrom = expr->getType();
    bool recognizedCast = false;
    if (castFrom && castFrom->isBaseType() && 
        castTo && castTo->isBaseType())
    {
        BaseType *srcType = static_cast<BaseType*>(castFrom);
        BaseType *dstType = static_cast<BaseType*>(castTo);

        //assume type cast only applied to one component of short vector
        if (dstType->typemask & BT_Float)
        {
            if (srcType->typemask & BT_Double) 
            {
                castTo->allocateILStorage(&il_store);
                bril::emitd2f(out, &il_store, expr->get_il_store());
                recognizedCast = true;
            }
        }
        else if (dstType->typemask & BT_Double)
        {
            if (srcType->typemask & BT_Float)
            {
                castTo->allocateILStorage(&il_store);
                bril::emitf2d(out, &il_store, expr->get_il_store());
                recognizedCast = true;
            }
        }
    }

    if (recognizedCast == false)
        il_store = *(expr->get_il_store());

}

#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void 
CastExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "CastExpr::semanticCheck(%d)\n", isKernel);
    
    //! should we handle more semantics here?
    expr->semanticCheck(isKernel);

    if(isKernel)
    {
        //! casting expression type is array type?
        if(expr->etype == ET_Variable)
        {
            Variable *v = AsVariable(expr);
            
            //! return if variable is not declared
            if (v->name->entry == NULL || v->name->entry->uVarDecl == NULL)
                return;
            if(v->name->entry->uVarDecl->isArray())
            {
                std::ostringstream tmpmsg;
                tmpmsg << "Illegal to use array type as casting expression";
                RecordSemanticError(this, SemanticChecker::SCEKNull, tmpmsg.str().c_str()); 
                return;
            }
        }
    }

    //! Give error if explicit casting having different components
    if(isKernel)
    {
        Type* castType = type;
        Type* exprType = expr->getType();
        if(castType != NULL && exprType != NULL)
        {
            BinaryOperandsDetails details = getBinaryOperandsDetails(castType->getBase()->typemask, 
                                                                      exprType->getBase()->typemask);
            if(details.leftOperandDetails.components != details.rightOperandDetails.components)
            {
                if(globals.enableStrongTypeChecking)
                {
                    RecordSemanticError(this, SemanticChecker::SCEKNull, "explicit casting required to have same no of components");
                    RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                }
                else
                    RecordSemanticWarn(this, SemanticChecker::SCWNull, "explicit casting required to have same no of components");
            }
        }
        else
        {
            assert(0);
            LOG(LOG_ERROR, "Internel error : type info missing for casting expression");
            return;
        }
    }
}
#endif
void CastExpr::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "CastExpr::findExpr(%p)\n", cb);
    (cb) (this);

    expr->findExpr(cb);
}

void CastExpr::replaceExpr(Expression * oldExp, Expression * newExp)
{
    if (oldExp == expr)
        expr = newExp;
    else
        assert(0); //this is to assert internal problem, ok to assert here

    if (newExp)
        newExp->setParent(this);
}

SizeofExpr::SizeofExpr(Expression * operand, const Location l):Expression(ET_SizeofExpr, l)
{
    LOG(LOG_FN_ENTRY, "SizeofExpr::SizeofExpr(%p, %p)\n", operand, &l);
    sizeofType = NULL;
    expr = operand;
    type = new BaseType(BT_UnSigned | BT_Long);
    if (operand)
        operand->setParent(this);
}


SizeofExpr::SizeofExpr(Type * operand, const Location l):Expression(ET_SizeofExpr, l)
{
    LOG(LOG_FN_ENTRY, "SizeofExpr::SizeofExpr(%p, %p)\n", operand, &l);
    sizeofType = operand;
    expr = NULL;
    type = new BaseType(BT_UnSigned | BT_Long);
}

SizeofExpr::~SizeofExpr()
{
    LOG(LOG_FN_ENTRY, "SizeofExpr::~SizeofExpr()\n");
    // delete sizeofType;
    delete expr;
}


Expression *SizeofExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "SizeofExpr::dup0()\n");
    SizeofExpr *ret;

    if (sizeofType != NULL)
        ret = new SizeofExpr(sizeofType, location);
    else
        ret = new SizeofExpr(expr->dup(), location);

    ret->type = type;
    return ret;
}


void SizeofExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "SizeofExpr::print()\n");
    out << "sizeof(";
    if (sizeofType != NULL)
        sizeofType->printType(out, NULL, true, 0);
    else
        expr->print(out);
    out << ") ";

#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

#ifdef SEMANTIC_CHECK
void 
SizeofExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "SizeofExpr::semanticCheck(%d)\n", isKernel);
    if (isKernel)
    {
        RecordSemanticError(this, SemanticChecker::SCEKExprSizeof);
    }
}
#endif
void SizeofExpr::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "SizeofExpr::findExpr(%p)\n", cb);
    (cb) (this);
    if (expr != NULL)
    {
        //expr = (cb)(expr);
        expr->findExpr(cb);
    }
}


IndexExpr::IndexExpr(Expression * _array, Expression * sub, const Location l):Expression(ET_IndexExpr, l)
{
    LOG(LOG_FN_ENTRY, "IndexExpr::IndexExpr(%p, %p, %p)\n", _array, sub, &l);
    array = _array;
    _subscript = sub;
    isParent = false;
    accessType = IndexReadAccess;
    if (array)
        array->setParent(this);
    if (_subscript)
        _subscript->setParent(this);
}
#if BROOK_TO_IL_CODE_GEN_PATH
bril::store * IndexExpr::get_il_store() const
{
    LOG(LOG_FN_ENTRY, "IndexExpr::get_il_store()\n");
    bril::allocate_store(&il_store, _subscript->get_il_store());
    return &il_store;
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

IndexExpr::~IndexExpr()
{
    LOG(LOG_FN_ENTRY, "IndexExpr::~IndexExpr()\n");
    delete array;
    delete _subscript;
}


Expression *IndexExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "IndexExpr::dup0()\n");
    IndexExpr *ret = new IndexExpr(array->dup(), _subscript->dup(), location);

    ret->type = type;
    ret->isParent = isParent;
    ret->accessType = accessType;
    return ret;
}

//! Returns the variable expression of index expression
//! Returns NULL if return expression is not ET_Variable type
Expression *IndexExpr::getArrayVariableExpr() const
{
    IndexExpr *index = NULL;

    if(array->etype == ET_Variable)
        return array;
    else
    {
        if(array->etype != ET_IndexExpr)
            assert(0);
        index = AsIndexExpr(array);
        while(index->etype == ET_IndexExpr)
            index = AsIndexExpr(index->array);
    }
    if(index->etype == ET_Variable)
        return index;
    else
        return NULL;
}

//! Returns size(Dimensions) of index expression
//! Returns zero if variable of index expression is not a variable expression
int IndexExpr::getIndexExprSize() const
{
    IndexExpr *index = NULL;
    int indexExprSize = 1;

    if(array->etype == ET_Variable)
        return indexExprSize;
    else
    {
        if(array->etype != ET_IndexExpr)
            assert(0);
        index = AsIndexExpr(array);
        while(index->etype == ET_IndexExpr)
        {
            index = AsIndexExpr(index->array);
            indexExprSize++;
        }
    }
    if(index->etype == ET_Variable)
        return indexExprSize;
    else
        return 0;
}

//! gets the subscript expression string for given array type
//! basically it converts multi dimensional indexing expressions into single dimension indexing expression
//! Ex :
//!     float x, a[10][11][12];
//!     x = a[i]j[][k]; --> x = a[i * 11 * 12 + j * 12 + k];
void IndexExpr::getSubscriptExprStr(std::ostream & out, ArrayType *arrayType) const
{
    IndexExpr *index = NULL;

    int dim = arrayType->getDimensions();
    int indexExprSize = 1;
    if(dim != 1)
    {
        for(int i = 0; i < dim - 1; ++i)
        {
            index = AsIndexExpr(array);
            for(int j = 0; j < dim - 2 - i; ++j)
            {
                index = AsIndexExpr(index->array);
            }
            index->_subscript->print(out);
            for(int j = 0; j < dim - i - 1; ++j)
            {
                out << " * " << arrayType->getDimensionsSize(j + i + 2);
                if(j == dim - i - 1 - 1)
                    out << " + ";
            }
        }
    }

    _subscript->print(out);
}

//! gets the subscript expression string for given array type
//! basically it converts multi dimensional Non c-style indexing expressions into single dimension indexing expression
//! Ex :
//!     float x, a[10][11][12];
//!     float3 index = float3(k,j,i); 
//!     x = a[index]; --> x = a[[__get3DDepth(index) * 11 * 12 + __get3DHeight(index) * 12 + __get3DWidth(index)];
void IndexExpr::getSubscriptExprStrFromNonCStyleIndexing(std::ostream & out, ArrayType *arrayType) const
{
    IndexExpr *index = NULL;

    int dim = arrayType->getDimensions();
    if(dim == 1)
    {
        _subscript->print(out);
    }
    else if(dim == 2)
    {
        out << "__get2DHeight(";
        _subscript->print(out);
        out << ") * " << arrayType->getDimensionsSize(2) << " + __get2DWidth(";
        _subscript->print(out);
        out << ")";
    }
    else if(dim == 3)
    {
        out << "__get3DDepth(";
        _subscript->print(out);
        out << ") * " << arrayType->getDimensionsSize(2) << " * " << arrayType->getDimensionsSize(3)
            << " + __get3DHeight(";
        _subscript->print(out);
        out << ") * " << arrayType->getDimensionsSize(3)
            << " + __get3DWidth(";
        _subscript->print(out);
        out << ")";
    }
}
//! Prints the Non C-style indexing expression
void IndexExpr::printNonCStyleGatherIndexing(std::ostream & out) const
{
    const Expression* arrayExp = getArrayVariableExpr();
    assert(arrayExp->etype == ET_Variable);
    const Variable *arrayVar = static_cast<const Variable*>(arrayExp);
    ArrayType *arrayType = AsArrayType(arrayVar->type);
    int arrayDim = arrayType->getDimensions();

    if(globals.enableGPUAddressTranslation == false || arrayDim == 2)
    {
        out << "__gather_";
        arrayType->getBase()->printBase(out, 0);
        out << "(";
        arrayVar->print(out);
        out << ", float2(";
        _subscript->print(out);
        out << "))";
    }
    else
    {
        out << "__gather_";
        arrayType->getBase()->printBase(out, 0);
        out << "(";
        arrayVar->print(out);
        out << ", ";
        if(arrayDim == 1)
        {
            out << "__getGatherPosFrom1D(";
            _subscript->print(out);
            out << ", __gatherStreamShape_";
            arrayVar->print(out);
            out << ", __gatherBufferShape_";
            arrayVar->print(out);
            out << ")";
        }
        else if(arrayDim == 3)
        {
            out << "__getGatherPosFrom3D(";
            _subscript->print(out);
            out << ", __gatherStreamShape_";
            arrayVar->print(out);
            out << ", __gatherBufferShape_";
            arrayVar->print(out);
            out << ")";
        }
        out << ")";
    }
}

//! Print C-Style indexing expression
void IndexExpr::printCStyleGatherIndexing(std::ostream & out) const
{
    const Expression* arrayExp = getArrayVariableExpr();

    assert(arrayExp->etype == ET_Variable);
    const Variable *arrayVar = static_cast<const Variable*>(arrayExp);
    ArrayType *arrayType = AsArrayType(arrayVar->type);
    int arrayDim = arrayType->getDimensions();

    //! Get subscript expression as constructor expression style
    std::ostringstream subscriptExprStr;
    const IndexExpr *index = NULL;
    if(arrayDim != 1)
        index = static_cast<const IndexExpr*>(array);
    _subscript->print(subscriptExprStr);
    for(int i = 0; i < arrayDim - 1; ++i)
    {
        subscriptExprStr << ", ";
        index->_subscript->print(subscriptExprStr);
        index = static_cast<const IndexExpr*>(index->array);
    }
    
    if(globals.enableGPUAddressTranslation == false || arrayDim == 2)
    {
        out << "__gather_";
        arrayType->getBase()->printBase(out, 0);
        out << "(";
        arrayVar->print(out);
        out << ", float2(";
        out << subscriptExprStr.str().c_str();
        out << "))";
    }
    else
    {
        out << "__gather_";
        arrayType->getBase()->printBase(out, 0);
        out << "(";
        arrayVar->print(out);
        out << ", ";
        if(arrayDim == 1)
        {
            out << "__getGatherPosFrom1D(";
            out << subscriptExprStr.str().c_str();
            out << ", __gatherStreamShape_";
            arrayVar->print(out);
            out << ", __gatherBufferShape_";
            arrayVar->print(out);
            out << ")";
        }
        else if(arrayDim == 3)
        {
            out << "__getGatherPosFrom3D(";
            out << "float3(";
            out << subscriptExprStr.str().c_str();
            out << ")";
            out << ", __gatherStreamShape_";
            arrayVar->print(out);
            out << ", __gatherBufferShape_";
            arrayVar->print(out);
            out << ")";
        }
        out << ")";
    }
}
void IndexExpr::print(std::ostream & out) const
{
    //! use this function for non-kernel and cal backend kernel code
    LOG(LOG_FN_ENTRY, "IndexExpr::print(%p)\n", &out);
#ifndef DISABLE_C_STYLE_INDEXING
    if(globals.getPresentTarget() == 0)
    {
#endif // DISABLE_C_STYLE_INDEXING
        if (array->precedence() < precedence())
        {
            out << "(";
            array->print(out);
            out << ")";
        }
        else
            array->print(out);

        out << "[";
        _subscript->print(out);
        out << "]";
#ifndef DISABLE_C_STYLE_INDEXING
    }
    else if(globals.getPresentTarget() == TARGET_CAL)
    {
        //! MOVING to code gen level is very good idea
        
        //! skip if parent is index expression
        if(isParent == false)
        {
            return;
        }
        const Expression* arrayExp = getArrayVariableExpr();

        if(arrayExp->etype == ET_Variable)
        {
            const Variable *arrayVar = static_cast<const Variable*>(arrayExp);
            ArrayType *arrayType = AsArrayType(arrayVar->type);

            int arrayDim = arrayType->getDimensions();
            int indexExprSize = getIndexExprSize();
            if(arrayVar->name->entry->IsParamDecl())
            {
                if(arrayVar->getType()->getBase()->qualifier != TQ_Out)
                {
                    //! Generate gather array indexing code 
#ifndef FORCE_C_STYLE_GATHER_INDEXING
                    if(indexExprSize == 1)
                    {
                        if(arrayType->resourceType == ConstBufferType)
                        {
                            //! Generate cosntant buffer array indexing code
                            std::ostringstream subscriptExprStr;

                            getSubscriptExprStrFromNonCStyleIndexing(subscriptExprStr, arrayType);
                            if (array->precedence() < precedence())
                            {
                                out << "(";
                                arrayVar->print(out);
                                out << ")";
                            }
                            else
                                arrayVar->print(out);
                            out << "[";
                            out << subscriptExprStr.str().c_str();
                            out << "]";
                        }
                        else
                        {
                            printNonCStyleGatherIndexing(out);
                        }
                    }
                    else
#endif // FORCE_C_STYLE_GATHER_INDEXING
                    {
                        if(arrayType->resourceType == ConstBufferType)
                        {
                            //! Generate cosntant buffer array indexing code
                            std::ostringstream subscriptExprStr;

                            getSubscriptExprStr(subscriptExprStr, arrayType);
                            if (array->precedence() < precedence())
                            {
                                out << "(";
                                arrayVar->print(out);
                                out << ")";
                            }
                            else
                                arrayVar->print(out);
                            out << "[";
                            out << subscriptExprStr.str().c_str();
                            out << "]";
                        }
                        else
                        {
                            printCStyleGatherIndexing(out);
                        }
                    }
                }
                else
                {
                     //! Generate scatter array indexing code 
                    if (array->precedence() < precedence())
                    {
                        out << "(";
                        arrayVar->print(out);
                        out << ")";
                    }
                    else
                        arrayVar->print(out);

                    if(arrayDim == 1 || (arrayDim == 3 && !globals.enableGPUAddressTranslation))
                    {
                        out << "[int(";
                        _subscript->print(out);
                        out << ")]";
                    }
                    else
                    {
                        std::ostringstream subscriptExprStr;
                        if(indexExprSize == 1)
                        {
                            _subscript->print(subscriptExprStr);
                        }
                        else
                        {
                            //! Get subscript expression as constructor expression style
                            const IndexExpr *index = NULL;
                            index = static_cast<const IndexExpr*>(array);
                            _subscript->print(subscriptExprStr);
                            for(int i = 0; i < arrayDim - 1; ++i)
                            {
                                subscriptExprStr << ", ";
                                index->_subscript->print(subscriptExprStr);
                                index = static_cast<const IndexExpr*>(index->array);
                            }
                        }

                        if(arrayDim == 2)
                        {
                            out << "[__getScatterIndex(int2(";
                            out << subscriptExprStr.str().c_str();
                            out << "), __outputBufferPitch)]";
                        }
                        else if(arrayDim == 3)
                        {
                            out << "[__getScatterIndex(int3(";
                            out << subscriptExprStr.str().c_str();
                            out << "), __outputStreamShape)]";
                        }
                    }
                }
 
            }
            else
            {
                //! Generate local array indexing code

                if(arrayType->getQualifiers() == TQ_Shared)
                {
                    //! Print Shared local array indexing code
                    //! Skip shared local array write(this statement is printed by assinExpr)
                    if(accessType == IndexReadAccess)
                    {

                        std::ostringstream subscriptStr;
                        _subscript->print(subscriptStr);
                        
                        int ldsPerThread = 1;
                        ldsPerThread = arrayType->getTotalElementCount() / arrayType->getGroupSize();
                        std::ostringstream instanceInGroup;
                        instanceInGroup << "((int)(" << subscriptStr.str().c_str() << ") / " << ldsPerThread << ")";

                        std::ostringstream offset;
                        offset << "((int)" << subscriptStr.str().c_str() << ") - (int)(" << instanceInGroup.str().c_str() << " * " << ldsPerThread << ") ";
                        out << "ReadLds(" << instanceInGroup.str().c_str() << ", (" << offset.str().c_str() << ") * " << ldsPerThread <<", false)";
                    }
                }
                else
                {
                    //! Print Non-Shared local array indexing code
                    std::ostringstream subscriptExprStr;

                    getSubscriptExprStr(subscriptExprStr, arrayType);
                    if (array->precedence() < precedence())
                    {
                        out << "(";
                        arrayVar->print(out);
                        out << ")";
                    }
                    else
                        arrayVar->print(out);
                    out << "[";
                    out << subscriptExprStr.str().c_str();
                    out << "]";
                }
            }

        }
    }
    else
    {
        assert(0);
        LOG(LOG_ERROR, "Index expression prining is vaild for non kernel code and CAL backend code");
        return;
    }
#endif // DISABLE_C_STYLE_INDEXING
#ifdef    SHOW_TYPES
    if (type != NULL)
    {
        out << "/* ";
        type->printType(out, NULL, true, 0);
        out << " */";
    }
#endif
}

/////////////////////////////////////////////////////////////////////////////
//! \brief Get the type of index expression
////////////////////////////////////////////////////////////////////////////
Type* IndexExpr::getType() const
{
    if(type)
        return type->getRealType();

    IndexExpr *tmp = AsIndexExpr(array);

    //! Get variable type 
    while(tmp)
    {
        if(tmp->etype == ET_Variable)
        {
            return tmp->getType();
        }
        else if(tmp->etype == ET_IndexExpr)
        {
            tmp = AsIndexExpr(tmp->array);
        }
    }

    //! Just for safe : Is there any more situation where type is NULL?
    std::ostringstream tmpStr;
    print(tmpStr);
    LOG(LOG_ERROR, "%s: %d -> WHY ARRAY IS NOT A VARIABLE in %s", tmpStr.str().c_str());
    assert(type);
    return NULL;
}

#if BROOK_TO_IL_CODE_GEN_PATH
void IndexExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "IndexExpr::print_il(%p)\n", &out);
    _subscript->print_il(out);
    bril::allocate_store(&il_store, _subscript->get_il_store());
    out << IL_PREFIX_STRING << "ftoi r" << il_store.idx << ", r" << il_store.size << std::endl;
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK

//! Checks semantics for subscript of parent index expressions
void IndexExpr::checkSubscriptSemantics(bool isKernel)
{
    if(isKernel)
    {
        Expression * parentExpr = getParentExp();
        if(parentExpr && parentExpr->etype == ET_IndexExpr)
        {
            return;
        }
        isParent = true;

        IndexExpr *index = NULL;

        if(array->etype == ET_Variable)
        {
            Type *subscriptType = _subscript->getType();
            assert(subscriptType);
            BTDetails subscriptDetails = getTypeDetails(subscriptType->getBase()->typemask);
            if(subscriptDetails.components != 1)
                RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "subscript expression must be scalar type");

            //! Type of subscript not handled still
        }
        else
        {
            index = AsIndexExpr(array);
            while(index->etype == ET_IndexExpr)
            {
                Type *subscriptType = index->_subscript->getType();
                assert(subscriptType);
                BTDetails subscriptDetails = getTypeDetails(subscriptType->getBase()->typemask);
                if(subscriptDetails.components != 1)
                    RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "subscript expression must be scalar type");
                //! Type of subscript not handled still

                index = AsIndexExpr(index->array);
            }
        }
    }
}

void 
IndexExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "IndexExpr::semanticCheck(%d)\n", isKernel);
    //gather expr, won't be changed until iredit_3

    array->semanticCheck(isKernel);
    _subscript->semanticCheck(isKernel);

    if (isKernel)
    {
        Expression * parentExpr = getParentExp();
        //! skip if parent is index expression
        if(parentExpr && parentExpr->etype == ET_IndexExpr)
        {
            return;
        }
        isParent = true;
        Expression* arrayExp = getArrayVariableExpr();

        if(arrayExp->etype == ET_Variable)
        {
            Variable *arrayVar = AsVariable(arrayExp);

            //! return if variable is not declared
            if (arrayVar->name->entry == NULL || arrayVar->name->entry->uVarDecl == NULL)
                return;

            if(arrayVar->type->type != TT_Array)
            {
                RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "Variable must be array type");
                return;
            }

            ArrayType *arrayType = AsArrayType(arrayVar->type);

            int arrayDim = arrayType->getDimensions();
            int indexExprSize = getIndexExprSize();
            if(arrayVar->name->entry->IsParamDecl())
            {
                if(arrayDim != indexExprSize)
                {
#ifndef FORCE_C_STYLE_GATHER_INDEXING
                    if(indexExprSize == 1)
                    {
                        RecordSemanticWarn(this, SemanticChecker::SCWNull, "Strongly recommended to use c-style indexing for gather/scatter arrays");
                        Type *subscriptType = _subscript->getType();
                        assert(subscriptType);
                        BTDetails subscriptDetails = getTypeDetails(subscriptType->getBase()->typemask);
                        if(arrayDim != subscriptDetails.components)
                            RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "Gather/scatter array dimensions and subscript vector components must match");

                        //! Type of subscript not handled still
                        

                    }
                    else
                    {
                        RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "Index expression size either 1 or equal to array dimensions");
                    }
#else
                    RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "Gather array variable dimensions and index expression dimensions must be same");
#endif // FORCE_C_STYLE_GATHER_INDEXING
                }
                else 
                {
                    checkSubscriptSemantics(isKernel);
                }
 
            }
            else
            {
                if(arrayDim != indexExprSize)
                    RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "Local array variable dimensions and index expression dimensions must be same");
                else
                {
                    checkSubscriptSemantics(isKernel);
                }
            }

        }
        else
        {
            RecordSemanticError(this, SemanticChecker::SCEKIndexExpr, "Array expression of index expression must be variable type");
        }
        
        //RecordSemanticError(this, SemanticChecker::SCEKExprGather);
    }
}
#endif
void IndexExpr::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "IndexExpr::findExpr(%p)\n", cb);
    (cb) (this);
    //array = (cb)(array);
    array->findExpr(cb);
    //_subscript = (cb)(_subscript);
    _subscript->findExpr(cb);
}


void IndexExpr::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "IndexExpr::replaceExpr(%p, %p)\n", oldExp, newExp);
    if (oldExp == array)
        array = newExp;
    else if (oldExp == _subscript)
        _subscript = newExp;
    else
        assert(0); //this is to assert internal problem, ok to assert here

    if (newExp)
        newExp->setParent(this);
}


ConstructorExpr::ConstructorExpr(BaseType * bType, Expression * exprs[],
                                 const Location l):Expression(ET_ConstructorExpr, l)
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::ConstructorExpr(%p, %p, %p)\n", bType, exprs, &l);
    int nExprs;

    nExprs = FloatDimension(bType->typemask);
    assert(nExprs > 0);
    type = bType;

    _bType = bType;
    _nExprs = nExprs;
    _exprs = new Expression *[nExprs];
    for (int i = 0; i < nExprs; i++)
    {
        _exprs[i] = exprs[i];
        if (exprs[i])
            exprs[i]->setParent(this);
    }
}


ConstructorExpr::~ConstructorExpr()
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::~ConstructorExpr()\n");
    /*
     * Types are tracked in the global project typeList and shouldn't be
     * explicitly delete()'ed.
     delete _bType;
     */

    delete[]_exprs;
}


Expression *ConstructorExpr::dup0() const
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::dup0()\n");
    ConstructorExpr *ret;
    Expression *newExprs[4];    /* Float4 is the largest */

    assert(_nExprs <= sizeof newExprs / sizeof newExprs[0]);
    for (unsigned int i = 0; i < _nExprs; i++)
    {
        newExprs[i] = _exprs[i]->dup();
    }

    ret = new ConstructorExpr((BaseType *) _bType->dup(), newExprs, location);
    ret->type = type;
    return ret;
}


void ConstructorExpr::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::print(%p)\n", &out);
    assert(_nExprs >= 2);

    bType()->printBase(out, 0);
    out << "(";
    for (unsigned int i = 0; i < _nExprs - 1; i++)
    {
        out << *exprN(i) << ",";
    }
    out << *exprN(_nExprs - 1) << ")";
}

#if BROOK_TO_IL_CODE_GEN_PATH
void ConstructorExpr::print_il(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::print_il(%p)\n", &out);
    assert(_nExprs >= 2 && _nExprs <= 4);

    static char const *const c[4] = { "x", "y", "z", "w" };

    for (unsigned int i = 0; i < _nExprs; i++)
        exprN(i)->print_il(out);

    for (unsigned int i = 0; i < _nExprs; i++)
    {
        out << IL_PREFIX_STRING << "mov " << getILRegName(bril::rs_base) << ".";
        if (isDouble(_bType))
            out << c[2*i] << c[2*i+1];
        else
            out << c[i];
        out << ", " << exprN(i)->getILRegName(bril::rs_src_sel) << "\n";
    }
}
#endif // BROOK_TO_IL_CODE_GEN_PATH


#ifdef SEMANTIC_CHECK
void 
ConstructorExpr::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::semanticCheck(%d)\n", isKernel);
    if(isKernel)
    {
        assert(type->getBase());
        BaseTypeInfo *typeInfo = getBaseTypeInfo(type->getBase()->typemask);
        BTDetails details = getTypeDetails(type->getBase()->typemask);

        //! Get category type name for constructor type
        std::string categoryName = std::string(typeInfo->brTypeName);
        assert(details.components != 1);
        categoryName = categoryName.substr(0, categoryName.length() - 1);
       

        for (int i = 0; i < AsInt(_nExprs); i++)
        {
            //! Check semantics of each initializer
            exprN(i)->semanticCheck(isKernel);

            //! Check constructor type and constructor initializers type
            Type *initType = exprN(i)->getType();
            assert(initType->getBase());
            BaseTypeInfo *initTypeInfo = getBaseTypeInfo(initType->getBase()->typemask);
            BTDetails initDetails = getTypeDetails(initType->getBase()->typemask);

            if(details.category != initDetails.category)
            {
                if(globals.enableStrongTypeChecking)
                {
                    if(!checkForTypeAndComponents(details, initDetails))
                    {
                        std::ostringstream tmpStr;
                        tmpStr << "Mismatched type for element " << i << ": " 
                            << "actual type is " << initTypeInfo->brTypeName << " but expected type is " << categoryName.c_str();
                        RecordSemanticWarn(this, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                }
                else
                {
                    if(checkForConversionLose(details, initDetails))
                    {
                        std::ostringstream tmpStr;
                        tmpStr << "possible lose of data for element " << i << ": " 
                            << "conversion from " << initTypeInfo->brTypeName << " to " << categoryName.c_str();
                        RecordSemanticWarn(this, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                    }
                }
            }
        }
    }
}

#endif

void ConstructorExpr::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::findExpr(cb)\n");
    (cb) (this);
    for (unsigned int i = 0; i < _nExprs; i++)
    {
        //_exprs[i] = (cb)(exprN(i));
        exprN(i)->findExpr(cb);
    }
}


void ConstructorExpr::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "ConstructorExpr::replaceExpr(%p, %p)\n", oldExp, newExp);
    bool done = false;

    for (unsigned int i = 0; i < _nExprs; i++)
    {
        if (_exprs[i] == oldExp)
        {
            _exprs[i] = newExp;
            done = true;
        }
    }
    if (newExp)
        newExp->setParent(this);
    assert(done);
}


std::ostream & operator<<(std::ostream & out, const Expression & expr)
{
    expr.print(out);
    return out;
}


Expression *ReverseList(Expression * eList)
{
    LOG(LOG_FN_ENTRY, "ReverseList(%p)\n", eList);
    Expression *head = NULL;

    while (eList != NULL)
    {
        Expression *exp = eList;

        eList = eList->next;

        exp->next = head;
        head = exp;
    }

    return head;
}


#define    SHOW(X)    case X: return #X
const char *nameOfExpressionType(ExpressionType type)
{
    LOG(LOG_FN_ENTRY, "nameOfExpressionType(%d)\n", type);
    switch (type)
    {
    default:
        return "Unknown ExpressionType";

        SHOW(ET_VoidExpr);

        SHOW(ET_Constant);
        SHOW(ET_Variable);
        SHOW(ET_FunctionCall);

        SHOW(ET_AssignExpr);
        SHOW(ET_RelExpr);

        SHOW(ET_UnaryExpr);
        SHOW(ET_BinaryExpr);
        SHOW(ET_TrinaryExpr);

        SHOW(ET_CastExpr);
        SHOW(ET_SizeofExpr);
        SHOW(ET_IndexExpr);

        SHOW(ET_ConstructorExpr);

        SHOW(ET_BrtStreamInitializer);
        SHOW(ET_BrtGatherExpr);
        SHOW(ET_BrtIndexofExpr);
        SHOW(ET_BrtInstanceExpr);
        SHOW(ET_BrtInstanceInGroupExpr);
    }
}


const char *nameOfConstantType(ConstantType type)
{
    LOG(LOG_FN_ENTRY, "nameOfConstantType(%d)\n", type);
    switch (type)
    {
    default:
        return "Unknown ConstantType";

        SHOW(CT_Char);
        SHOW(CT_Int);
        SHOW(CT_Float);
        SHOW(CT_Double);
        SHOW(CT_String);
        SHOW(CT_Array);
    }
}


const char *nameOfAssignOp(AssignOp op)
{
    LOG(LOG_FN_ENTRY, "nameOfAssignOp(%d)\n", op);
    switch (op)
    {
    default:
        return "Unknown AssignOp";

        SHOW(AO_Equal);         //  =

        SHOW(AO_PlusEql);       // +=
        SHOW(AO_MinusEql);      // -=
        SHOW(AO_MultEql);       // *=
        SHOW(AO_DivEql);        // /=
        SHOW(AO_ModEql);        // %=

        SHOW(AO_ShlEql);        // <<=
        SHOW(AO_ShrEql);        // >>=

        SHOW(AO_BitAndEql);     // &=
        SHOW(AO_BitXorEql);     // ^=
        SHOW(AO_BitOrEql);      // |=
    }
}


const char *nameOfRelOp(RelOp op)
{
    LOG(LOG_FN_ENTRY, "nameOfRelOp(%d)\n", op);
    switch (op)
    {
    default:
        return "Unknown RelOp";

        SHOW(RO_Equal);         // ==
        SHOW(RO_NotEqual);      // !=

        SHOW(RO_Less);          // < 
        SHOW(RO_LessEql);       // <=
        SHOW(RO_Grtr);          // > 
        SHOW(RO_GrtrEql);       // >=
    }
}


const char *nameOfUnaryOp(UnaryOp op)
{
    LOG(LOG_FN_ENTRY, "nameOfUnaryOp(%d)\n", op);
    switch (op)
    {
    default:
        return "Unknown UnaryOp";

        SHOW(UO_Plus);          // +
        SHOW(UO_Minus);         // -
        SHOW(UO_BitNot);        // ~
        SHOW(UO_Not);           // !

        SHOW(UO_PreInc);        // ++x
        SHOW(UO_PreDec);        // --x
        SHOW(UO_PostInc);       // x++
        SHOW(UO_PostDec);       // x--

        SHOW(UO_AddrOf);        // &
        SHOW(UO_Deref);         // *
    }
}


const char *nameOfBinaryOp(BinaryOp op)
{
    LOG(LOG_FN_ENTRY, "nameOfBinaryOp(%d)\n", op);
    switch (op)
    {
    default:
        return "Unknown BinaryOp";

        SHOW(BO_Plus);          // +
        SHOW(BO_Minus);         // -
        SHOW(BO_Mult);          // *
        SHOW(BO_Div);           // /
        SHOW(BO_Mod);           // %

        SHOW(BO_Shl);           // <<
        SHOW(BO_Shr);           // >>
        SHOW(BO_BitAnd);        // &
        SHOW(BO_BitXor);        // ^
        SHOW(BO_BitOr);         // |

        SHOW(BO_And);           // &&
        SHOW(BO_Or);            // ||

        SHOW(BO_Comma);         // x,y

        SHOW(BO_Member);        // x.y
        SHOW(BO_PtrMember);     // x->y

        SHOW(BO_Assign);        // An AssignExpr
        SHOW(BO_Rel);           // A RelExpr
    }
}

#undef SHOW

