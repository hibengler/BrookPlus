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
    o+     File:         stemnt.h
    o+
    o+     Programmer:   Shaun Flisakowski
    o+     Date:         Aug 9, 1998
    o+
    o+     A high-level view of statements.
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

#ifndef    STEMNT_H
#define    STEMNT_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <list>
#include <utility>

#include "symbol.h"
#include "express.h"
#include "decl.h"
#include "callback.h"
#include "location.h"
#include "dup.h"
#include "cgTool.h"
    enum StatementType
    {
        ST_NullStemnt = 0,          // The null statement.

        ST_DeclStemnt,
        ST_TypedefStemnt,
        ST_ExpressionStemnt,

        ST_IfStemnt,
        ST_SwitchStemnt,

        ST_ForStemnt,
        ST_WhileStemnt,
        ST_DoWhileStemnt,

        ST_ContinueStemnt,
        ST_BreakStemnt,
        ST_GotoStemnt,
        ST_ReturnStemnt,
#ifdef __ASMIL__
	ST_ASMStemnt,
#endif /* __ASMIL */
        ST_Block,
        ST_BRTKernel,
        ST_BRTScatter,

        ST_FileLineStemnt,          // #line #file
        ST_InclStemnt,              // #include
        ST_EndInclStemnt,
        ST_PPDirective
    };


enum LabelType
{
    LT_None = 0,                // No label - invalid.

    LT_Default,                 // default:
    LT_Case,                    // case <expr>:
    LT_CaseRange,               // case <expr> ... <expr>: (gcc extension)

    LT_Goto                     // A named label (goto destination).
};


class Label
{
public:
    Label(LabelType labelType);
    Label(Expression * _expr);
    Label(Expression * _begin, Expression * _end);
    Label(Symbol * sym);
    ~Label();

    Label *dup0() const;
    Label *dup() const;
    void print(std::ostream & out, int level) const;

    void findExpr(fnExprCallback cb);

    LabelType type;

    union
    {
        Expression *begin;      // for LT_Case, LT_CaseRange
        Symbol *name;           // for LT_Goto
    };

    Expression *end;            // for LT_CaseRange

    SymEntry *syment;
};



typedef std::vector < Label * >LabelVector;



class Statement;
typedef Dup < Statement > DupableStatement;

class Statement:public DupableStatement
{
public:
    Statement(StatementType stemntType, const Location location);
    virtual ~ Statement();

    void addLabel(Label * lbl); // Add a label to this statement.
    void addHeadLabel(Label * lbl);

    virtual bool isBlock() const
        {
            return false;
        }
    virtual bool isFuncDef() const
        {
            return false;
        }
    virtual bool isDeclaration() const
        {
            return false;
        }
    virtual bool isTypedef() const
        {
            return false;
        }

    virtual bool needSemicolon() const
        {
            return (type != ST_NullStemnt);
        }

    virtual bool isFileLine() const
        {
            return false;
        }
    virtual bool isInclude() const
        {
            return false;
        }
    virtual bool isEndInclude() const
        {
            return false;
        }

    virtual Statement *dup0() const;
    virtual void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out, int level) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    virtual void semanticCheck(bool isKernel) {}
#endif

    virtual void findExpr(fnExprCallback cb);
    virtual void findStemnt(fnStemntCallback cb)
        {
            (cb) (this);
        }
    virtual void replaceExpr(UNUSED(Expression * oldExp), UNUSED(Expression * newExp))
        {
            assert(0);
        }

    StatementType type;
    LabelVector labels;
    Location location;

    Statement *next;            // For making a list of statements.

    friend std::ostream & operator<<(std::ostream & out, const Statement & stemnt);
};


class FileLineStemnt:public Statement
{
public:
    FileLineStemnt(const std::string & incl, int lino, const Location location);
    virtual ~ FileLineStemnt();

    bool isFileline() const
        {
            return true;
        }
    Statement *dup0() const;
    void print(std::ostream & out, int level) const;

    std::string filename;
    int linenumber;
};

class InclStemnt:public Statement
{
public:
    InclStemnt(const std::string & incl, const Location location);
    virtual ~ InclStemnt();

    bool isInclude() const
        {
            return true;
        }
    Statement *dup0() const;
    void print(std::ostream & out, int level) const;

    bool isStandard;            // use brackets
    std::string filename;
};

class EndInclStemnt:public Statement
{
public:
    EndInclStemnt(const Location location);
    virtual ~ EndInclStemnt();

    bool isEndInclude() const
        {
            return true;
        }
    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
};


class ExpressionStemnt:public Statement
{
public:
    ExpressionStemnt(Expression * expr, const Location location);
    virtual ~ ExpressionStemnt();

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out, int level) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *expression;
};


class IfStemnt:public Statement
{
public:
    IfStemnt(Expression * _cond, Statement * _thenBlk, const Location location, Statement * _elseBlk = NULL);
    ~IfStemnt();

    bool needSemicolon() const
        {
            return false;
        }

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out, int level) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void findStemnt(fnStemntCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *cond;

    Statement *thenBlk;
    Statement *elseBlk;         // Can be NULL
};

#ifdef __ASMIL__
class ASMStemnt:public Statement
{
public:
    ASMStemnt(Expression * _body, const Location location);
    ~ASMStemnt();

    bool needSemicolon() const
        {
            return false;
        }

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out, int level) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif
    Expression *body;
    std::list<Expression *> scratch;
    std::list<std::pair<Expression *, Expression *> > input;
    std::list<std::pair<Expression *, Expression *> > output;
 private:
    Expression * processDirectives(Expression * _body, const Location l);
};
#endif /* __ASMIL__ */

class SwitchStemnt:public Statement
{
public:
    SwitchStemnt(Expression * _cond, Statement * _block, const Location location);
    ~SwitchStemnt();

    bool needSemicolon() const
        {
            return false;
        }

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void findStemnt(fnStemntCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *cond;

    Statement *block;
};

//! Forward declaration
class Block;
class ForStemnt:public Statement
{
public:
    ForStemnt(Expression * _init, Expression * _cond, Expression * _incr,
              const Location location, Statement * _block = NULL);
    ~ForStemnt();

    bool needSemicolon() const
        {
            return false;
        }

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out, int level) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    bool searchContinueStemnt(Block* blockStement);
    void groupContinueAndIncrExpr(bool isKernel);
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void findStemnt(fnStemntCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *init;
    Expression *cond;
    Expression *incr;

    Statement *block;           // Can be NULL
};


class WhileStemnt:public Statement
{
public:
    WhileStemnt(Expression * _cond, Statement * _block, const Location location);
    ~WhileStemnt();

    bool needSemicolon() const
        {
            return false;
        }

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    void print_il(std::ostream & out, int level) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void findStemnt(fnStemntCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *cond;
    Statement *block;
};


class DoWhileStemnt:public Statement
{
public:
    DoWhileStemnt(Expression * _cond, Statement * _block, const Location location);
    ~DoWhileStemnt();

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void findStemnt(fnStemntCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *cond;
    Statement *block;
};


class GotoStemnt:public Statement
{
public:
    GotoStemnt(Symbol * _dest, const Location location);
    ~GotoStemnt();

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    Symbol *dest;
};


class ReturnStemnt:public Statement
{
public:
    ReturnStemnt(Expression * _result, const Location location);
    ~ReturnStemnt();

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out, int level) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExp, Expression * newExp);

    Expression *result;         // Can be NULL.
};


class DeclStemnt:public Statement
{
public:
    DeclStemnt(const Location location, StatementType stype = ST_DeclStemnt);
    virtual ~ DeclStemnt();

    // Declarations print their own semicolon.
    bool needSemicolon() const
        {
            return false;
        }
    bool isDeclaration() const
        {
            return true;
        }

    Statement *dup0() const;
    virtual void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out, int level) const;
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

    // Only converts if necessary.
    DeclStemnt *convertToTypedef();

    void addDecl(Decl * decl);
    void addDecls(Decl * decls);

    void findExpr(fnExprCallback cb);

    // The type(s).
    DeclVector decls;
};

class TypedefStemnt:public DeclStemnt
{
public:
    TypedefStemnt(const Location location);
    ~TypedefStemnt();

    bool isTypedef() const
        {
            return true;
        }

    void print(std::ostream & out, int level) const;

#ifdef SEMANTIC_CHECK
    void semanticCheck(bool isKernel);
#endif

};


class Block:public Statement
{
public:
    Block(const Location location);
    virtual ~ Block();

    void add(Statement * stemnt);

    void addStatements(Statement * stemnt);
    void addDeclStemnts(DeclStemnt * decls);

    virtual bool isBlock() const
        {
            return true;
        }
    bool needSemicolon() const
        {
            return false;
        }

    virtual Statement *dup0() const;
    virtual void print(std::ostream & out, int level) const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out, int level) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
    virtual void semanticCheck(bool isKernel);
#endif

    virtual void findExpr(fnExprCallback cb);
    void findStemnt(fnStemntCallback cb);

    void insert(Statement * stemnt, Statement * after = NULL);

    Statement *head;            // List of statements in this block.
    Statement *tail;
};

//! Shader type 
typedef enum _ShaderType
{
    PixelShader = 0,
    ComputeShader,
    ShaderCount
}ShaderType;

// Not really true, but this is very convienent.
class FunctionDef:public Block
{
public:
    FunctionDef(const Location location);
    ~FunctionDef();

    bool isFuncDef() const
        {
            return true;
        }

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;
    ShaderType getShaderType() const {return shaderType;};
    GccAttrib* getGroupSize() const;
    int getLocalDataShareSize() const {return localDataShareSize;}
    int getSharedArrayTypeComponents() const {return sharedArrayTypeComponents;}

#ifdef SEMANTIC_CHECK
    void checkKernelAttributeSemantics(bool isKernel);
    void checkKernelBuiltinFuncUsage(bool isKernel);
    static Expression* setBuiltinMask(Expression *expr);
    void setShaderType(ShaderType type);
    void semanticCheck(bool isKernel);
#endif
#if BROOK_TO_IL_CODE_GEN_PATH
    bril::store * get_il_store() const;

    mutable bril::store il_store;
#endif // BROOK_TO_IL_CODE_GEN_PATH
    void findExpr(fnExprCallback cb);

    bool returnsVoid() const;

    Symbol *FunctionName() const;

    Decl *decl;                 // The declaration.

    GccAttrib *kernelAttribute; // For kernel attributes
    ShaderType shaderType;
    std::vector<int> groupSizeValues;
    int localDataShareSize;
    int sharedArrayTypeComponents;
};


class PPDirective:public Statement
{
public:
    PPDirective(const std::string & line, const Location location);
    ~PPDirective();

    Statement *dup0() const;
    void print(std::ostream & out, int level) const;

    std::string directive;
};



Statement *ReverseList(Statement * sList);

void indent(std::ostream & out, int level);
void printNull(std::ostream & out, int level);
void printBlock(std::ostream & out, int level, Statement * block);

// For debugging.
const char *nameOfStatementType(StatementType type);
const char *nameOfLabelType(LabelType type);



#endif /* STEMNT_H */
