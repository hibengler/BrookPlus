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
 * b2ctransform.cpp --
 * code to actually transform kernels to C++ code that can execute said kernels
 */
#ifdef _WIN32
#pragma warning(disable:4786)
//the above warning disables visual studio's annoying habit of warning when using the standard set lib
#endif

#include <set>
#include <fstream>
#include <string>
#include <iostream>

#include "ctool.h"
#include "logger.hpp"
#include "brtdecl.h"
#include "brtstemnt.h"
#include "main.h"
#include "b2ctransform.h"

    using std::cout;
using std::cerr;
using std::endl;

template < class ConverterFunctor > Expression * ConvertToT(Expression * expr);
Expression *ConvertToTConstantExprConverter(Expression *);
Expression *ConvertToTIndexExprConverter(Expression * e);
Expression *ConvertToTMaskConverter(Expression * e);

//set of items we do not want to change to int1 objects.
std::set < Expression * >ArrayBlacklist;

// the following function translates Assign Ops to
// equivalent Binary Ops for mask
BinaryOp TranslatePlusGets(AssignOp ae)
{
    LOG(LOG_FN_ENTRY, "TranslatePlusGets(%d)\n", ae);

    BinaryOp bo = BO_Assign;

    switch (ae)
    {
    case AO_Equal:
        break;
    case AO_PlusEql:
        bo = BO_Plus;
        break;
    case AO_MinusEql:
        bo = BO_Minus;
        break;
    case AO_DivEql:
        bo = BO_Div;
        break;
    case AO_MultEql:
        bo = BO_Mult;
        break;
    case AO_ModEql:
        bo = BO_Mod;
        break;
    case AO_ShlEql:
        bo = BO_Shl;
        break;
    case AO_ShrEql:
        bo = BO_Shr;
        break;
    case AO_BitAndEql:
        bo = BO_BitAnd;
        break;
    case AO_BitXorEql:
        bo = BO_BitXor;
        break;
    case AO_BitOrEql:
        bo = BO_BitOr;
        break;
    }
    return bo;
}


//the following function sees if the member operator target looks like a mask
bool looksLikeMask(std::string s)
{
    LOG(LOG_FN_ENTRY, "looksLikeMask(%s)\n", s.c_str());

    if (s.length() <= 4)
    {
        for (unsigned int i = 0; i < s.length(); ++i)
        {
            if (s[i] != 'x' && s[i] != 'y' && s[i] != 'z' && s[i] != 'w' &&
                s[i] != 'r' && s[i] != 'g' && s[i] != 'b' && s[i] != 'a')
            {
                return false;
            }
        }
        return true;
    }
    return false;
}


//the following translates a letter for a swizzle into a number
int translateSwizzle(char mask)
{
    LOG(LOG_FN_ENTRY, "translateSwizzle(%c)\n", mask);

    switch (mask)
    {
    case 'y':
    case 'g':
        return 1;
    case 'z':
    case 'b':
        return 2;
    case 'w':
    case 'a':
        return 3;
    default:
        return 0;
    }
}


//the following translates a mask into a mask.
std::string translateMask(int swizzle)
{
    LOG(LOG_FN_ENTRY, "translateMask(%d)\n", swizzle);

    switch (swizzle)
    {
    case 3:
        return "::brt::maskW";
    case 2:
        return "::brt::maskZ";
    case 1:
        return "::brt::maskY";
    default:
        return "::brt::maskX";
    }
}


//The following class replaces a lval.mask = rval into lval.mask(rval)
class MaskExpr:public AssignExpr
{
public:
    std::string mask;
    Expression *emask;
    MaskExpr(Expression * lExpr, Expression * rExpr, std::string mask, const Location l, Expression * emask = NULL):
        AssignExpr(AO_Equal, lExpr, rExpr, l)
        {
            LOG(LOG_FN_ENTRY, "MaskExpr::MaskExpr(%p, %p, %s, %p, %p)\n", lExpr, rExpr, mask.c_str(), emask);
            this->mask = mask;
            this->emask = emask;
        }

    ~MaskExpr()
        {
            LOG(LOG_FN_ENTRY, "MaskExpr::~MaskExpr()\n");
            if (emask)
                delete emask;
        }

    void findExpr(fnExprCallback cb)
        {
            LOG(LOG_FN_ENTRY, "MaskExpr::findExpr(%p)\n", cb);
            this->BinaryExpr::findExpr(cb);
            if (emask)
            {
                emask = (cb) (emask);
                emask->findExpr(cb);
            }
        }

    Expression *lValue() const
        {
            return leftExpr();
        }
    Expression *rValue() const
        {
            return rightExpr();
        }

    int precedence() const
        {
            return emask ? 16 : 16; /*maybe left expr's prec */
        }

    Expression *dup0() const
        {
            LOG(LOG_FN_ENTRY, "MaskExpr::dup0()\n");
            MaskExpr *ret = new MaskExpr(_leftExpr->dup(),
                                         _rightExpr->dup(),
                                         mask,
                                         location,
                                         emask ? emask->dup() : NULL);
            ret->type = type;
            return ret;
        }

    void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "MaskExpr::print(%p)\n", &out);
            if (lValue()->precedence() < precedence())
                out << "(" << *lValue() << ")";
            else
                out << *lValue();
            size_t length = 1;
            if     (!emask)
                length = mask.length();
            out << ".mask" << length << "(";
            int commaprecedence = BinaryExpr(BO_Comma, 0, 0, location).precedence();
            if  (rValue()->precedence() < commaprecedence /*comma */ )
                out << "(" << *rValue() << ")";
            else
                out << *rValue();
            for (unsigned int i = 0; i < 4 && i < length; ++i)
            {
                out << ",";
                if (emask)
                {
                    out << *emask;
                }
                else
                {
                    out << translateMask(translateSwizzle(mask[i]));
                }
            }
            out << ")";
        }

};


// the following class translates the Trinary ?: expr into a function call
class NewQuestionColon:public TrinaryExpr
{
public:
    NewQuestionColon(Expression * c, Expression * t, Expression * f, const Location l):TrinaryExpr(c, t, f, l)
        {
            LOG(LOG_FN_ENTRY, "NewQuestionColon::NewQuestionColon(%p, %p, %p, %p)\n", c, t, f, &l);
        }

    Expression *dup0() const
        {
            LOG(LOG_FN_ENTRY, "NewQuestionColon::dup0()\n");
            return new NewQuestionColon(condExpr()->dup(), trueExpr()->dup(), falseExpr()->dup(), location);
        }
    int precedence() const
        {
            return 3;
        }
    void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "NewQuestionColon::print(%p)\n", &out);
            int memberprecedence = BinaryExpr(BO_Member, 0, 0, location).precedence();
            if  (condExpr()->precedence() < memberprecedence)       /*member */
                out << "(";
            out << *condExpr();
            if  (condExpr()->precedence() < memberprecedence)       /*member */
                out << ")";
            out << ".questioncolon(";
            out << *trueExpr();
            out << ",";
            out << *falseExpr();
            out << ")";
        }

};


// The following functor actually finds trinary expressions and converts them into function calls
class QuestionColonConverter
{
public:
    Expression * operator() (Expression * e)
        {
            LOG(LOG_FN_ENTRY, "QuestionColonConverter::operator()(%p)\n", e);
            TrinaryExpr *te;
            NewQuestionColon *ret = NULL;
            if               (e->etype == ET_TrinaryExpr && (te = static_cast < TrinaryExpr * >(e)))
            {
                ret = new NewQuestionColon(te->condExpr()->dup(),
                                           te->trueExpr()->dup(), te->falseExpr()->dup(), te->location);
            }
            return ret;
        }
};


//The following class prints out base types with the BRT equivalents
class BaseType1:public BaseType
{
public:


    // Do not recusively translate an integer
    BaseType1(const BaseType & t):BaseType(t)
        {
            LOG(LOG_FN_ENTRY, "BaseType1::BaseType1(%p)\n", &t);
            ArrayBlacklist.insert((Expression *) this);
        }


    // Upon destruction, delete it from the blacklist
    ~BaseType1()
        {
            LOG(LOG_FN_ENTRY, "BaseType1::~BaseType1()\n");
            std::set < Expression * >::iterator i = ArrayBlacklist.find((Expression *) this);
            if (i != ArrayBlacklist.end())
                ArrayBlacklist.erase(i);
        }


    // to duplicate, follow the duplication procedure of the normal base type.
    Type *dup0() const
        {
            LOG(LOG_FN_ENTRY, "BaseType1::dup0()\n");
            BaseType *ret = new BaseType1(*this);
            ret->storage = storage;
            ret->qualifier = qualifier;
            ret->typemask = typemask;
            ret->tag = tag->dup();
            ret->typeName = typeName->dup();
            ret->stDefn = stDefn->dup();
            ret->enDefn = enDefn->dup();
            return ret;
        }


    // to print out the base type, see if it falls into the set
    // of classes with BRT replacements
    void hackedPrintBase(std::ostream & out, int level, bool raw) const
        {
            LOG(LOG_FN_ENTRY, "BaseType1::hackedPrintBase(%p, %d, %d)\n", &out, level, raw);
            int special =
                BT_Char | BT_Char2 | BT_Char3 | BT_Char4 |
                BT_Short | BT_Short2 | BT_Short3 | BT_Short4 |
                BT_Int | BT_Int2 | BT_Int3 | BT_Int4 |
                BT_Float |
                BT_Float2 |
                BT_Float3 |
                BT_Float4 |
                BT_Double |
                BT_Double2 |
                BT_Long | BT_UserType | BT_Struct;

            if  ((typemask & special) != 0)
            {
                if ((qualifier & TQ_Const) != 0)
                    out << "const ";
                if ((qualifier & TQ_Volatile) != 0)
                    out << "volatile ";

                if (typemask & BT_UserType)
                {
                    if (raw)
                    {
                        out << "__castablestruct_" << *typeName;
                    }
                    else
                    {
                        out << "__cpustruct_" << *typeName;
                    }
                }
                else if (typemask & BT_Struct)
                {
                    if (raw)
                    {
                        // TIM: hope they didn't define the struct inline...
                        out << "struct " << *tag;
                    }
                    else
                    {
                        out << "struct __cpustruct_" << *tag;
                    }
                }
                else
                {
                    const char *outstr = getBaseTypeName(typemask, BaseTypeRTName);

                    if (outstr == NULL)
                    {
                        cerr << "ERROR the following data type can't be stream element type: "
                             << getBaseTypeName(typemask, BaseTypeBRName) << std::endl;
                        exit(-1);
                    }
                    out << outstr << " ";
                    // out << "__BrtInt1 ";
                }

                if ((qualifier & TQ_Out) != 0)
                {
                    //out << "&";
                }
            }
            else
            {
                //no BRT equivalent fallback
                BaseType::printBase(out, level);
            }

        }

    virtual void printRawBase(std::ostream & out, int level) const
        {
            LOG(LOG_FN_ENTRY, "BaseType1::printRawBase(%p, %d)\n", &out, level);
            hackedPrintBase(out, level, true);
            /*
              BaseType *b = static_cast<BaseType*>(this->dup());
              b->qualifier=TQ_None;
              b->BaseType::printBase(out,level);
              delete b;
            */
        }
    virtual void printBase(std::ostream & out, int level) const
        {
            LOG(LOG_FN_ENTRY, "BaseType1::printBase(%p, %d)\n", &out, level);
            hackedPrintBase(out, level, false);
        }
};


// insert an expression into the blacklist, yet do not alter it...
Expression *ArrayBlacklister(Expression * e)
{
    LOG(LOG_FN_ENTRY, "ArrayBlacklister(%p)\n", e);
    ArrayBlacklist.insert(e);
    return e;
}


// this function makes a type's size expressions off limits
// so that they are not transformed
void BlacklistType(Type ** t);


// this function does the same as above but since they require **,
// it needs to look at a stored BaseType
void BlacklistBaseType(BaseType ** t)
{
    LOG(LOG_FN_ENTRY, "BlacklistBaseType(%p)\n", t);
    *t = new BaseType1(**t);
}


// This function goes through a type looking for array
// bounds to mark those to not be touched
void BlacklistType(Type ** t)
{
    LOG(LOG_FN_ENTRY, "BlacklistType(%p)\n", t);
    if (!t)
        return;
    if (!(*t))
        return;

    BaseType *basei;

    if ((*t)->type == TT_Base && (basei = static_cast < BaseType * >(*t)))
    {
        *t = new BaseType1(*basei);
    }

    ArrayType *at;

    if ((*t)->type == TT_Array && (at = static_cast < ArrayType * >(*t)))
    {
        if (at->size)
        {
            ArrayBlacklister(at->size);
            at->size->findExpr(ArrayBlacklister);
        }
    }

    //this takes care of Streams as well as arrays
    if ((*t)->type == TT_Array || (*t)->type == TT_Stream)
    {
        BlacklistType(&(static_cast < ArrayType * >(*t)->subType));
    }

    if ((*t)->type == TT_Pointer)
        BlacklistType(&(static_cast < PtrType * >(*t)->subType));

    if ((*t)->type == TT_BitField)
        BlacklistType(&(static_cast < BitFieldType * >(*t)->subType));

    BrtStreamType *st;

    if ((*t)->type == TT_BrtStream && (st = static_cast < BrtStreamType * >(*t)))
    {
        BlacklistBaseType(&st->base);
    }
    FunctionType *ft;

    if ((*t)->type == TT_Function && (ft = static_cast < FunctionType * >(*t)))
    {
        for (unsigned int i = 0; i < ft->nArgs; ++i)
        {
            BlacklistType(&ft->args[i]->form);
        }
        BlacklistType(&ft->subType);
    }

}

Expression *ModifyConstructorType(Expression * e)
{
    LOG(LOG_FN_ENTRY, "ModifyConstructorType(%p)\n", e);
    if (e->etype == ET_ConstructorExpr)
    {
        ConstructorExpr *ce = static_cast < ConstructorExpr * >(e);

        if (ce)
        {
            BlacklistBaseType(&ce->_bType);
        }
    }
    return e;
}


// This finds all types so that sizes may not be altered
void FindTypesDecl(Statement * s)
{
    LOG(LOG_FN_ENTRY, "FindTypesDecl(%p)\n", s);
    DeclStemnt *ds;

    s->findExpr(&ModifyConstructorType);

    if (s->isDeclaration() && (ds = static_cast < DeclStemnt * >(s)))
    {
        for (unsigned int i = 0; i < ds->decls.size(); ++i)
        {
            BlacklistType(&ds->decls[i]->form);
        }
    }

    FunctionDef *fd;

    if (s->isFuncDef() && (fd = static_cast < FunctionDef * >(s)))
    {
        BlacklistType(&fd->decl->form);
    }
}

extern bool recursiveIsGather(Type * t);


// This class overrides the index expr with one that prints [(int)<lookupExpr>]
// It is no longer needed for the current transformation since we now demand float4 lookups for 4d gather
class NewIndexExpr:public IndexExpr
{
public:
    bool isGather;
    Variable *findVariable(Expression * e)
        {
            LOG(LOG_FN_ENTRY, "NewIndexExpr::findVariable(%p)\n", e);
            if (e->etype == ET_Variable)
                return static_cast < Variable * >(e);
            if (e->etype == ET_IndexExpr)
                return findVariable(static_cast < IndexExpr * >(e)->array);
            return NULL;
        }
    NewIndexExpr(Expression * a, Expression * s, const Location l):IndexExpr(a, s, l)
        {
            LOG(LOG_FN_ENTRY, "NewIndexExpr::NewIndexExpr(%p, %p, %p)\n", a, s, &l);

            isGather = false;
            Variable *v = findVariable(a);

            if (v)
                if (v->name->entry)
                    if (v->name->entry->type == ParamDeclEntry)
                        if (v->name->entry->uVarDecl)
                            if (v->name->entry->uVarDecl->form)
                                if ((v->name->entry->uVarDecl->form->getQualifiers() & TQ_Reduce) == 0)
                                    if (v->name->entry->uVarDecl->form->type == TT_Array)
                                        isGather = true;
        }

    Expression *dup0() const
        {
            LOG(LOG_FN_ENTRY, "NewIndexExpr::dup0()\n");
            return new NewIndexExpr(array->dup(), _subscript->dup(), location);
        }
    void printIndex(std::ostream & out, bool printCast) const
        {
            LOG(LOG_FN_ENTRY, "NewIndexExpr::printIndex(%p, %d)\n", &out, printCast);
            if (array->precedence() < precedence())
            {
                out << "(";
            }
            if    (array->etype == ET_IndexExpr)
            {
                static_cast < NewIndexExpr * >(array)->printIndex(out, false);
            }
            else
            {
                array->print(out);
            }
            if (array->precedence() < precedence())
            {
                out << ")";
            }

            out << "[";

            Expression* arrayExp = getArrayVariableExpr();

			//! skip casting for shared array index expression
            bool skipCasting = false;
            if(arrayExp->etype == ET_Variable)
            {
                Variable *arrayVar = AsVariable(arrayExp);
                ArrayType *arrayType = AsArrayType(arrayVar->type);

                if(arrayType->getQualifiers() == TQ_Shared)
                    skipCasting = true;

            }
            if(skipCasting == false)
            {
                if(!isGather)
                    out << "(int)";
            }
            /*
              int castprecedence= CastExpr(NULL,NULL,location).precedence();
              if (_subscript->precedence()<=castprecedence)
              out <<"(";
            */
            _subscript->print(out);
            /*
              if (_subscript->precedence()<=castprecedence)
              out <<")";
            */
            out << "]";
            if (printCast && isGather)
            {
#ifdef MULTIPLE_ARRAY_BOUNDS_LOOKUPS
                out << ".gather()";
#endif
            }
        }

    void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "print(%p)\n", &out);
            Expression* arrayExp = getArrayVariableExpr();

            if(arrayExp->etype == ET_Variable)
            {
                Variable *arrayVar = AsVariable(arrayExp);

                //! return if variable is not declared
                if (arrayVar->name->entry == NULL || arrayVar->name->entry->uVarDecl == NULL)
                    return;

                 ArrayType *arrayType = AsArrayType(arrayVar->type);

                int arrayDim = arrayType->getDimensions();
                int indexExprSize = getIndexExprSize();
                if(arrayVar->name->entry->IsParamDecl() || arrayType->getQualifiers() == TQ_Shared)
                {
                    if(arrayDim == indexExprSize && indexExprSize != 1)
                    {
                        std::ostringstream subscriptExprStr;
                        std::ostringstream constructorStr;
                        constructorStr << "__BrtInt4( (int)";

                        IndexExpr *index = NULL;

                        if(_subscript->etype == ET_BinaryExpr)
                        {
                            BinaryExpr* binaryExpr = AsBinaryExpr(_subscript);
                            if(binaryExpr->bOp == BO_Member)
                            {
                                _subscript->print(constructorStr);
                                constructorStr << ".getAt(0)";
                            }
                            else
                            {
                                constructorStr << "(int)(";
                                _subscript->print(constructorStr);
                                constructorStr << ")";
                                
                            }
                        }
                        else
                        {
                            constructorStr << "(int)(";
                            _subscript->print(constructorStr);
                            constructorStr << ")";
                        }
                        index = AsIndexExpr(array);
                        for(int i = 1; i < arrayDim ; ++i)
                        {
                            constructorStr << ", (int)";
                            if(index->_subscript->etype == ET_BinaryExpr)
                            {
                                BinaryExpr* binaryExpr = AsBinaryExpr(_subscript);
                                if(binaryExpr->bOp == BO_Member)
                                {
                                    index->_subscript->print(constructorStr);
                                    constructorStr << ".getAt(" << i << ")";
                                }
                                else
                                {
                                    constructorStr << "(int)(";
                                    index->_subscript->print(constructorStr);
                                    constructorStr << ")";                                    
                                }
                            }
                            else
                            {
                                constructorStr << "(int)(";
                                index->_subscript->print(constructorStr);
                                constructorStr << ")";
                            }
                            index = AsIndexExpr(index->array);
                        }

                        if(arrayDim == 2)
                            constructorStr << ", 0, 0)";
                        if(arrayDim == 3)
                            constructorStr << ", 0)";

                        subscriptExprStr << constructorStr.str().c_str();

                        //! add swizzle
                        subscriptExprStr << ".swizzle" << arrayDim;

                        if(arrayDim == 2)
                            subscriptExprStr << "(::brt::maskX, ::brt::maskY)";
                        if(arrayDim == 3)
                            subscriptExprStr << "(::brt::maskX, ::brt::maskY, ::brt::maskZ)";;

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
                        printIndex(out, true);
                    }

                }
                else
                {
                    printIndex(out, true);
                }
            }  
        }
};


// This class finds the indices to convert from [E] to [(int)E]
class IndexExprConverter
{
public:
    Expression * operator() (Expression * e)
        {
            LOG(LOG_FN_ENTRY, "IndexExprConverter::operator()(%p)\n", e);
            IndexExpr *ie;
            IndexExpr *ret = NULL;
            if        (e->etype == ET_IndexExpr && (ie = static_cast < IndexExpr * >(e)))
            {
                ret = new NewIndexExpr(ie->array->dup(), ie->_subscript->dup(), e->location);
                ret->array->findExpr(&ConvertToTIndexExprConverter);
                ret->_subscript->findExpr(&ConvertToTIndexExprConverter);
            }
            CastExpr *ce;

            if (e->etype == ET_CastExpr && (ce = static_cast < CastExpr * >(e)))
            {
                BlacklistType(&ce->castTo);
            }
            return ret;
        }
};


// This int constant is specifies its type as a brt type
class NewIntConstant:public IntConstant
{
public:
    NewIntConstant(long val, const Location l, NumericForm printForm = BR_Decimal_Form):IntConstant(val, l, printForm)
        {
            LOG(LOG_FN_ENTRY, "NewIntConstant::NewIntConstant(%d, %p)\n", val, &l);
        }
    virtual void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "NewIntConstant::print(%p)\n", &out);     
            out << "__BrtInt1((int)";
            (this)->IntConstant::print(out);
            out << ")";
        }
};


// This unsigned int constant is specifies its type as a brt type
class NewUIntConstant:public UIntConstant
{
public:
    NewUIntConstant(unsigned int val, const Location l, NumericForm printForm = BR_Decimal_Form):UIntConstant(val, l, printForm)
        {
            LOG(LOG_FN_ENTRY, "NewUIntConstant(%d, %p)\n", val, &l);
        }
    virtual void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "NewUIntConstant::print(%p)\n", &out);
            out << "__BrtUInt1((unsigned int)";
            (this)->UIntConstant::print(out);
            out << ")";
        }
};


// This char constant is specifies its type as a brt type
class NewCharConstant:public CharConstant
{
public:
    NewCharConstant(char val, const Location l):CharConstant(val, l)
        {
            LOG(LOG_FN_ENTRY, "NewCharConstant::NewCharConstant(%c, %p)\n", val, &l);
        }
    virtual void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "NewCharConstant::print(%p)\n", &out);
            out << "__BrtChar1((char)";
            (this)->CharConstant::print(out);
            out << ")";
        }
};


// This float constant is specifies its type as a brt type
class NewFloatConstant:public FloatConstant
{
public:
    NewFloatConstant(std::string str, const Location l):FloatConstant(str, l)
        {
            LOG(LOG_FN_ENTRY, "NewFloatConstant::NewFloatConstant(%s, %p)\n", str.c_str(), &l);
        }
    virtual void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "NewFloatConstant::print(%p)\n", &out);
            out << "__BrtFloat1(";
            (this)->FloatConstant::print(out);
            out << ")";
        }
};

// This double constant is specifies its type as a brt type
class NewDoubleConstant:public DoubleConstant
{
public:
    NewDoubleConstant(std::string str, const Location l):DoubleConstant(str, l)
        {
        }
    virtual void print(std::ostream & out) const
        {
            out << "__BrtDouble1((double)";  //fixme: float2
            (this)->DoubleConstant::print(out);
            out << ")";
        }
};

// This array constant is specifies its type as a constructor for a brt type
class NewArrayConstant:public ArrayConstant
{
public:
    NewArrayConstant(const Location l):ArrayConstant(l)
        {
            LOG(LOG_FN_ENTRY, "NewArrayConstant::NewArrayConstant(%p)\n", &l);
        }
    virtual void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "print(%p)\n", &out);
            out << "__BrtFloat" << items.size() << "(";
            for (unsigned int i = 0; i < items.size(); ++i)
            {
                items[i]->print(out);
                if (i != items.size() - 1)
                {
                    out << ", ";
                }
            }
            out << ")";
        }
};


class ConstantExprConverter
{
public:
    Expression * operator() (Expression * e)
        {
            LOG(LOG_FN_ENTRY, "ConstantExprConverter::operator(%p)\n", e);
            Constant *con;
            Constant *ret = NULL;
            if(e->etype == ET_Constant && (con = static_cast < Constant * >(e)))
            {
                switch (con->ctype)
                {
                case CT_Char:
                {
                    CharConstant *cc = static_cast < CharConstant * >(con);
                    ret = new NewCharConstant(cc->ch, cc->location);
                    break;
                }
                case  CT_Int:
                {
                    IntConstant *cc = static_cast < IntConstant * >(con);

                    ret = new NewIntConstant(cc->lng, cc->location, cc->numericForm);
                    break;
                }
                case CT_UInt:
                {
                    UIntConstant *cc = static_cast < UIntConstant * >(con);

                    ret = new NewUIntConstant(cc->ulng, cc->location, cc->numericForm);
                    break;
                }
                case CT_Float:
                {
                    FloatConstant *cc = static_cast < FloatConstant * >(con);

                    ret = new NewFloatConstant(cc->strVal, cc->location);
                    break;
                }
                case CT_Double:
                {
                    DoubleConstant *cc = static_cast < DoubleConstant * >(con);

                    ret = new NewDoubleConstant(cc->strVal, cc->location);
                    break;
                }
                case CT_Array:
                {
                    ArrayConstant *ac = static_cast < ArrayConstant * >(con);
                    ArrayConstant *aret = new NewArrayConstant(ac->location);
                    ret = aret;
                    for (unsigned int i = 0; i < ac->items.size(); ++i)
                    {
                        Expression *expr = ac->items[i]->dup();

                        aret->addElement(expr);
                        expr->findExpr(ConvertToTConstantExprConverter);
                    }
                    break;
                }
                default:
                    break;
                }
            }
            return ret;
        }
};


class SwizzleConverter
{
public:
    Expression * operator() (Expression * e)
        {
            LOG(LOG_FN_ENTRY, "SwizzleConverter::operator(%p)\n", e);
            BinaryExpr *be;
            if         (e->etype == ET_BinaryExpr && (be = static_cast < BinaryExpr * >(e)))
            {
                if (be->op() == BO_Member)
                {
                    Variable *vswiz;
                    Expression *leftExp = be->leftExpr();

                    //fixme: ET_BrtIndexofExpr, ET_IndexExpr, ET_FunctionCall, ET_BinaryExpr(==NULL)
                    //       these three types of exp do not have "type " set up properly
                    //       for now, assume they are of BuitinStreamElementType
                    if         ((leftExp->etype == ET_BrtIndexofExpr ||
                                 leftExp->etype == ET_BrtInstanceExpr ||
                                 leftExp->etype == ET_BrtInstanceInGroupExpr ||
                                 leftExp->type == NULL ||
                                 leftExp->type->isBuitinStreamElementType() ||
                                 leftExp->type->isBuitinStreamType()) &&
                                be->rightExpr()->etype == ET_Variable &&
                                (vswiz = static_cast < Variable * >(be->rightExpr())))
                    {

                        StructDef *structure = NULL;
                        if(leftExp->etype == ET_IndexExpr)
                        {
                            IndexExpr* indexExpr = static_cast<IndexExpr *>(leftExp);
                            Expression* arrayExp = indexExpr->getArrayVariableExpr();

                            if(arrayExp && arrayExp->etype == ET_Variable)
                            {
                                Variable *arrayVar = AsVariable(arrayExp);
                                Type* varType = arrayVar->getType();
                            
                                structure = findStructureDef(varType);
                            }
                        }

                        //! Skip if type is a structure
                        if (structure == NULL && looksLikeMask(vswiz->name->name))
                        {
                            size_t len = vswiz->name->name.length();
                            char swizlength[2] = { (char) len + '0', 0 };

                            std::string rez = std::string("swizzle") + swizlength + "(";
                            for (unsigned int i = 0; i < len; ++i)
                            {
                                if (i != 0)
                                    rez += ", ";
                                rez += translateMask(translateSwizzle(vswiz->name->name[i]));
                            }
                            rez += ")";
                            vswiz->name->name = rez;
                        }
                    }
                }
            }
            return NULL;
        }
};


class MaskConverter
{
public:
    void PropogatePlusGets(AssignExpr * ae)
        {
            LOG(LOG_FN_ENTRY, "MaskConverter::PropogatePlusGets(%p)\n", ae);
            BinaryOp bo = TranslatePlusGets(ae->op());
            if       (bo != BO_Assign)
            {
                //now we need to move the lval to the right
                ae->_rightExpr = new BinaryExpr(bo, ae->lValue()->dup(), ae->_rightExpr, ae->location);
                ae->aOp = AO_Equal;
            }
        }
    Type *getSymEntryType(SymEntry * se)
        {
            LOG(LOG_FN_ENTRY, "MaskConverter::getSymEntryType(%p)\n", se);
            switch (se->type)
            {
            case TypedefEntry:
            case VarDeclEntry:
            case ParamDeclEntry:
            case VarFctDeclEntry:
            case FctDeclEntry:
                return se->uVarDecl->form;
            case EnumConstEntry:
            default:
                return NULL;
            }
        }
    bool IndexExprDepthGreaterType(IndexExpr * i)
        {
            LOG(LOG_FN_ENTRY, "MaskConverter::IndexExprDepthGreaterType(%p)\n", i);
            int count = 1;

            while (i->array->etype == ET_IndexExpr)
            {
                i = static_cast < IndexExpr * >(i->array);
                count++;
            }
            if (i->array->etype == ET_Variable)
            {
                Variable *v = static_cast < Variable * >(i->array);
                Type *t = getSymEntryType(v->name->entry);

                if (t)
                {
                    int vcount = 0;

                    if (t->type == TT_Stream)
                        t = static_cast < ArrayType * >(t)->subType;
                    while (t->type == TT_Array)
                    {
                        t = static_cast < ArrayType * >(t)->subType;
                        vcount++;
                    }
                    return vcount < count;
                }
            }
            return false;
        }
    Expression *operator () (Expression * e)
        {
            LOG(LOG_FN_ENTRY, "MaskConverter::operator()", e);
            AssignExpr *ae;
            BinaryExpr *ret = NULL;
            Variable *vmask = NULL;

            if (e->etype == ET_BinaryExpr
                && static_cast < BinaryExpr * >(e)->op() == BO_Assign && (ae = static_cast < AssignExpr * >(e)))
            {
                //now lets identify what expression is to the left...
                //if it's a dot then we go!

                if (ae->lValue()->etype == ET_IndexExpr)
                {
                    IndexExpr *lval = static_cast < IndexExpr * >(ae->lValue());

                    if (IndexExprDepthGreaterType(lval))
                    {
                        //FIXME: what if it's an structure component mask,
                        //rather than an assignment to an array
                        PropogatePlusGets(ae);
                        ret = new MaskExpr(lval->array->dup(),
                                           ae->rValue()->dup(), "x", lval->location, lval->_subscript->dup());
                        ret->findExpr(&ConvertToTMaskConverter);
                    }
                }
                BinaryExpr *lval;

                if (ae->lValue()->etype == ET_BinaryExpr && (lval = static_cast < BinaryExpr * >(ae->lValue())))
                {
                    if (lval->op() == BO_Member)
                    {
                        if (lval->rightExpr()->etype == ET_Variable
                            && (vmask = static_cast < Variable * >(lval->rightExpr())))
                        {
                            if (looksLikeMask(vmask->name->name) &&
                                lval->leftExpr()->getType() != NULL &&
                                lval->leftExpr()->getType()->isStructure()==false)
                            {
                                PropogatePlusGets(ae);
                                ret = new MaskExpr(lval->leftExpr()->dup(),
                                                   ae->rValue()->dup(), vmask->name->name, lval->location);

                                ret->findExpr(&ConvertToTMaskConverter);
                            }
                        }
                    }
                }
            }
            return ret;

        }
};


class ChainExpression:public Expression
{
public:
    ChainExpression(Expression * e):Expression(e->etype, e->location)
        {
            LOG(LOG_FN_ENTRY, "ChainExpression::ChainExpression(%p)\n", e);
            next = e;
        }
    virtual void print(std::ostream & out) const
        {
            LOG(LOG_FN_ENTRY, "ChainExpression::print(%p)\n", &out);
            next->print(out);
        }
    virtual Expression *dup0() const
        {
            LOG(LOG_FN_ENTRY, "ChainExpression::dup0()\n");
            return new ChainExpression(next);
        }
    virtual void findExpr(fnExprCallback cb)
        {
            LOG(LOG_FN_ENTRY, "findExpr(%p)\n", cb);
            next->findExpr(cb);
        }
};

#define CONVERTTO(NAME) Expression * ConvertToT##NAME(Expression * eexpr){ \
        Expression * expression = eexpr; \
        if (ArrayBlacklist.find(expression)==ArrayBlacklist.end()) { \
                Expression * e = NAME()(expression); \
        if (e) { \
            expression->replace(e); \
                        return e; \
        } \
        } \
        return expression; \
}


CONVERTTO(MaskConverter)

    void FindMask(Statement * s)
{
    LOG(LOG_FN_ENTRY, "FindMask(%p)\n", s);
    s->findExpr(&ConvertToTMaskConverter);
}


CONVERTTO(SwizzleConverter)

    void FindSwizzle(Statement * s)
{
    LOG(LOG_FN_ENTRY, "FindSwizzle(%s)\n", s);
    s->findExpr((fnExprCallback) & ConvertToTSwizzleConverter);
}


CONVERTTO(QuestionColonConverter)

    void FindQuestionColon(Statement * s)
{
    LOG(LOG_FN_ENTRY, "FindQuestionColon(%p)\n", s);
    s->findExpr((fnExprCallback) & ConvertToTQuestionColonConverter);
}


CONVERTTO(IndexExprConverter)

    void FindIndexExpr(Statement * s)
{
    LOG(LOG_FN_ENTRY, "FindIndexExpr(%p)\n", s);
    s->findExpr((fnExprCallback) & ConvertToTIndexExprConverter);
}


CONVERTTO(ConstantExprConverter)

    void FindConstantExpr(Statement * s)
{
    LOG(LOG_FN_ENTRY, "FindConstantExpr(%p)\n", s);
    Expression *(*tmp) (class Expression *) = &ConvertToTConstantExprConverter;
    s->findExpr(tmp);
}


void RestoreTypes(FunctionDef * fDef)
{
    LOG(LOG_FN_ENTRY, "RestoreTypes(%p)\n", fDef);
    ArrayBlacklist.clear();
    fDef->findStemnt(FindTypesDecl);
}


Expression *ChangeFunctionTarget(Expression * e)
{
    LOG(LOG_FN_ENTRY, "ChangeFunctionTarget(%p)\n", e);
    if (e->etype == ET_FunctionCall)
    {
        Expression *k = static_cast < FunctionCall * >(e)->function;

        if (k->etype == ET_Variable)
        {
            Symbol *s = static_cast < Variable * >(k)->name;

            if (!(s->name.find("__") == 0 && s->name.find("_cpu_inner") != std::string::npos))
            {
                s->name = "__" + s->name + "_cpu_inner";
            }
        }
    }
    return e;
}


void FindFunctionCall(Statement * s)
{
    LOG(LOG_FN_ENTRY, "FindFunctionCall(%p)\n", s);
    s->findExpr(ChangeFunctionTarget);
}


void Brook2Cpp_ConvertKernel(FunctionDef * fDef)
{
    LOG(LOG_FN_ENTRY, "ConvertKernel(%p)\n", fDef);
    changeFunctionCallForIndexOf(fDef);
    RestoreTypes(fDef);
    fDef->findStemnt(&FindMask);
    RestoreTypes(fDef);
    fDef->findStemnt(&FindSwizzle);
    RestoreTypes(fDef);
    fDef->findStemnt(&FindQuestionColon);
    RestoreTypes(fDef);
    fDef->findStemnt(&FindIndexExpr);
    RestoreTypes(fDef);
    fDef->findStemnt(&FindConstantExpr);
    RestoreTypes(fDef);
    fDef->findStemnt(&FindFunctionCall);
    RestoreTypes(fDef);
}
