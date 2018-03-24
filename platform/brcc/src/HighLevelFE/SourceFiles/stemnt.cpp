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
    o+     File:         stemnt.cpp
    o+
    o+     Programmer:   Shaun Flisakowski
    o+     Date:         Aug 9, 1998
    o+
    o+     A high-level view of statements.
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */
#ifdef _WIN32
#pragma warning(disable:4786)
//the above warning disables visual studio's annoying habit of warning when using the standard set lib
#endif

#include <cstring>
#include <cassert>
#include <sstream>

#include "janitorial.hpp"

#include "main.h"
#include "stemnt.h"
#include "symbol.h"
#include "decl.h"
#include "project.h"

#include "gram.h"

//#define PRINT_LOCATION


//  indent - 2 spaces per level.
void indent(std::ostream & out, int level)
{
    if (level > 0)
    {
        for (int j = level; j > 0; j--)
            out << "  ";
    }
}


void printNull(std::ostream & out, int level)
{
    indent(out, level);
    out << ";\n";
}


void printBlock(std::ostream & out, int level, Statement * block)
{
    if (block == NULL)
        printNull(out, level + 1);
    else if (block->isBlock())
        block->print(out, level);
    else
    {
        block->print(out, level + 1);
        if (block->needSemicolon())
            out << ";";
    }
}


Statement *ReverseList(Statement * sList)
{
    Statement *head = NULL;

    while (sList != NULL)
    {
        Statement *ste = sList;

        sList = sList->next;

        ste->next = head;
        head = ste;
    }

    return head;
}


Label::Label(LabelType labelType)
{
    type = labelType;
    begin = NULL;
    end = NULL;
}

Label::Label(Expression * expr)
{
    type = LT_Case;
    begin = expr;
    end = NULL;
}

Label::Label(Expression * _begin, Expression * _end)
{
    type = LT_CaseRange;
    begin = _begin;
    end = _end;
}

Label::Label(Symbol * sym)
{
    type = LT_Goto;
    name = sym;
    end = NULL;
}


Label::~Label()
{
    switch (type)
    {
    case LT_Case:
    case LT_CaseRange:
        delete begin;
        delete end;

        break;

    case LT_Goto:
        delete name;
        break;

    default:
        break;
    }
}


Label *Label::dup() const
{
    Label *ret = this ? dup0() : NULL;

    return ret;
}

Label *Label::dup0() const
{
    Label *ret = new Label(type);

    switch (type)
    {
    default:
    case LT_None:
    case LT_Default:
        break;

    case LT_CaseRange:
        ret->end = end->dup();
    case LT_Case:
        ret->begin = begin->dup();
        break;

    case LT_Goto:
        ret->name = name->dup();
        break;
    }

    return ret;
}


void Label::print(std::ostream & out, int level) const
{
    indent(out, level - 1);

    switch (type)
    {
    case LT_None:
        assert(0);
        break;

    case LT_Default:
        out << "default";
        break;

    case LT_Case:
        assert(begin);
        out << "case " << *begin;
        break;

    case LT_CaseRange:
        assert(begin);
        assert(end);
        out << "case " << *begin << " ... " << *end;
        break;

    case LT_Goto:
        assert(name);
        out << *name;
        break;
    }

    out << ":\n";
}


void Label::findExpr(fnExprCallback cb)
{
    switch (type)
    {
    default:
        break;

    case LT_CaseRange:
        //end = (cb)(end);
        end->findExpr(cb);
    case LT_Case:
        //begin = (cb)(begin);
        begin->findExpr(cb);
        break;
    }
}


Statement::Statement(StatementType stemntType, const Location l):location(l)
{
    type = stemntType;
    next = NULL;
}


Statement::~Statement()
{
    LabelVector::iterator j;

    for (j = labels.begin(); j != labels.end(); j++)
    {
        delete *j;
    }
}


void Statement::addLabel(Label * lbl)
{
    labels.push_back(lbl);

    // Hook the label's symtable entry back to this statement.
    if (lbl->type == LT_Goto)
    {
        if (lbl->name->entry != NULL)
            lbl->name->entry->u2LabelPosition = this;
    }
}

void Statement::addHeadLabel(Label * lbl)
{
    labels.insert(labels.begin(), lbl);

    // Hook the label's symtable entry back to this statement.
    if (lbl->type == LT_Goto)
    {
        if (lbl->name->entry != NULL)
        {
            lbl->name->entry->u2LabelPosition = this;
            lbl->name->entry->uLabelDef = lbl;
        }
    }
}


Statement *Statement::dup0() const
{
    Statement *ret = new Statement(type, location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void Statement::print(std::ostream & out, int level) const
{

    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* Statement:";
        location.printLocation(out);
        out << " */";
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    switch (type)
    {
    default:
        out << __PRETTY_FUNCTION__ << std::endl;
        out << nameOfStatementType(type) << std::endl;
        break;

    case ST_NullStemnt: // The null statement.
        out << ";";
        break;

    case ST_ContinueStemnt:
        out << "continue";
        break;

    case ST_BreakStemnt:
        out << "break";
        break;
    }
}

#if BROOK_TO_IL_CODE_GEN_PATH
void Statement::print_il(std::ostream & out, int UNUSED(level)) const
{
    switch (type)
    {
    default:
        out << IL_PREFIX_STRING << __PRETTY_FUNCTION__ << std::endl;
        out << nameOfStatementType(type) << std::endl;
        break;

    case ST_NullStemnt: // The null statement.
        out << ";";
        break;

    case ST_ContinueStemnt:
        out << "continue";
        break;

    case ST_BreakStemnt:
        out << "break";
        break;
    }
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

void Statement::findExpr(fnExprCallback cb)
{
    LabelVector::iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
    {
        (*j)->findExpr(cb);
    }
}


FileLineStemnt::FileLineStemnt(const std::string & incl, int lino, const Location l):Statement(ST_FileLineStemnt, l),
filename(incl), linenumber(lino)
{
}


FileLineStemnt::~FileLineStemnt()
{
}


Statement *FileLineStemnt::dup0() const
{
    FileLineStemnt *ret = new FileLineStemnt(filename, linenumber, location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void FileLineStemnt::print(std::ostream & out, int UNUSED(level)) const
{
#ifdef PRINT_LOCATION
    out << "$<FileLineStemnt:";
    location.printLocation(out);
    out << ">$";
#endif

    if (linenumber > 0)
        out << "#line " << linenumber;
    else
        out << "#file";

    out << " \"" << filename << "\"" << std::endl;
}


InclStemnt::InclStemnt(const std::string & incl, const Location l):Statement(ST_InclStemnt, l)
{
    static const char *NrmPath[] = { "./", NULL };

    int j;

    isStandard = false;
    filename = incl;

    for (j = 0; StdPath[j]; j++)
    {
        if (strncmp(filename.c_str(), StdPath[j], strlen(StdPath[j])) == 0)
        {
            isStandard = true;
            filename = &(filename.c_str()[strlen(StdPath[j])]);
        }
    }

    for (j = 0; NrmPath[j]; j++)
    {
        if (strncmp(filename.c_str(), NrmPath[j], strlen(NrmPath[j])) == 0)
        {
            filename = &(filename.c_str()[strlen(NrmPath[j])]);
        }
    }
}


InclStemnt::~InclStemnt()
{
}


Statement *InclStemnt::dup0() const
{
    InclStemnt *ret = new InclStemnt(filename, location);

    ret->isStandard = isStandard;

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void InclStemnt::print(std::ostream & out, int UNUSED(level)) const
{
#ifdef PRINT_LOCATION
    out << "$<InclStemnt:";
    location.printLocation(out);
    out << ">$";
#endif

    out << "#include " << (isStandard ? '<' : '"');
    out << filename << (isStandard ? '>' : '"') << std::endl;
}


EndInclStemnt::EndInclStemnt(const Location l):Statement(ST_EndInclStemnt, l)
{
}


EndInclStemnt::~EndInclStemnt()
{
}


Statement *EndInclStemnt::dup0() const
{
    EndInclStemnt *ret = new EndInclStemnt(location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void EndInclStemnt::print(std::ostream & UNUSED(out), int UNUSED(level)) const
{
#ifdef PRINT_LOCATION
    out << "$<EndInclStemnt:";
    location.printLocation(out);
    out << ">$";
#endif
}


ExpressionStemnt::ExpressionStemnt(Expression * expr, const Location l):Statement(ST_ExpressionStemnt, l)
{
    expression = expr;
    if (expr)
        expr->setParent(this);
}


ExpressionStemnt::~ExpressionStemnt()
{
    delete expression;
}


Statement *ExpressionStemnt::dup0() const
{
    ExpressionStemnt *ret = new ExpressionStemnt(expression->dup(), location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void ExpressionStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* ExpressionStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);
    assert(expression);
    out << *expression;
}

#if BROOK_TO_IL_CODE_GEN_PATH
void ExpressionStemnt::print_il(std::ostream & out, int UNUSED(level)) const
{
    assert(expression);
    expression->print_il(out);
}
#endif //BROOK_TO_IL_CODE_GEN_PATH
#ifdef SEMANTIC_CHECK

void ExpressionStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "ExpressionStemnt::semanticCheck(%d)\n", isKernel);
    expression->semanticCheck(isKernel);
}
#endif //SEMANTIC_CHECK

void ExpressionStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);

    //expression = (cb)(expression);
    expression->findExpr(cb);
}


void ExpressionStemnt::replaceExpr(Expression * oldExp, Expression * newExp)
{
    assert(oldExp == expression);
    expression = newExp;
    if (newExp)
        newExp->setParent(this);
}


IfStemnt::IfStemnt(Expression * _cond, Statement * _thenBlk, const Location l, Statement * _elseBlk /* =NULL */ )
:Statement(ST_IfStemnt, l)
{
    cond = _cond;
    thenBlk = _thenBlk;
    elseBlk = _elseBlk;
    if (cond)
        cond->setParent(this);
}


IfStemnt::~IfStemnt()
{
    delete cond;
    delete thenBlk;
    delete elseBlk;
}


Statement *IfStemnt::dup0() const
{
    IfStemnt *ret = new IfStemnt(cond->dup(), thenBlk->dup(), location, elseBlk->dup());

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void IfStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* IfStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    out << "if (" << *cond << ")\n";
    printBlock(out, level, thenBlk);

    if (elseBlk)
    {
        out << std::endl;
        indent(out, level);
        out << "else\n";
        printBlock(out, level, elseBlk);
    }
}

#if BROOK_TO_IL_CODE_GEN_PATH
void IfStemnt::print_il(std::ostream & out, int level) const
{
    cond->print_il(out);

    bril::store tmp = *cond->get_il_store();
    // tmp.abs = 1;

    out << IL_PREFIX_STRING << "ifc_relop(ne) " << bril::get_gpr_name(&tmp, bril::rs_src_sel) << ", r0.0000\n";

    thenBlk->print_il(out, level);

    if (elseBlk)
    {
        out << IL_PREFIX_STRING << "else\n";
        elseBlk->print_il(out, level);
    }

    out << IL_PREFIX_STRING << "endif\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void IfStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "IfStemnt::semanticCheck(%d)\n", isKernel);
    if (cond)
        cond->semanticCheck(isKernel);

    if (thenBlk)
        thenBlk->semanticCheck(isKernel);

    if (elseBlk)
        elseBlk->semanticCheck(isKernel);

    //! check semantics of conditional expressions
    semanticChecker.checkConditionalSemantics(cond, isKernel);

}
#endif //SEMANTIC_CHECK
void IfStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);

    //cond = (cb)(cond);
    cond->findExpr(cb);

    thenBlk->findExpr(cb);

    if (elseBlk)
    {
        elseBlk->findExpr(cb);
    }
}


void IfStemnt::replaceExpr(Expression * oldExp, Expression * newExp)
{
    assert(oldExp == cond);
    cond = newExp;
    if (newExp)
        newExp->setParent(this);
}


void IfStemnt::findStemnt(fnStemntCallback cb)
{
    (cb) (this);

    thenBlk->findStemnt(cb);

    if (elseBlk)
        elseBlk->findStemnt(cb);
}

#ifdef __ASMIL__
ASMStemnt::ASMStemnt(Expression * _body, const Location l) 
 :Statement(ST_ASMStemnt, l)
{
  body = processDirectives(_body, l);
}

ASMStemnt::~ASMStemnt()
{
    delete body;
}

#define isWhiteSpace(c) ((c) == '\n' || (c) == '\r' || (c) == '\t' || (c) == ' ')
#define stripWhiteSpace(s) while ((s) != buff.end() && isWhiteSpace(*(s))) (s)++;

#define readIdent(i,s) while ((i) != buff.end() && !isWhiteSpace(*(i))) { s.append(1,*i++); }

#define ScratchKeyword  ".scratch"
#define InputKeyword    ".input"
#define OutputKeyword   ".output"

#define RegTypeLit "lit"
#define RegTypeCB  "cb"
#define RegTypeReg "reg"

enum AsmDirective
{
  ASM_Unknown = -1,
  ASM_Scratch = 0,
  ASM_Input,
  ASM_Output,
};

#define asmDirective(i,result) { result = ASM_Unknown;  if (std::string(i, i+strlen(ScratchKeyword)) == ScratchKeyword) {result = ASM_Scratch; i += strlen(ScratchKeyword)+1; } else if (std::string(i, i+strlen(InputKeyword)) == InputKeyword) {result = ASM_Input; i += strlen(InputKeyword)+1;} else if (std::string(i, i+strlen(OutputKeyword)) == OutputKeyword) {result = ASM_Output; i += strlen(OutputKeyword)+1;}}
   
/*
 * Process ASM IL directives, which are of the form:
 *
 *     [
 *       .scratch identifier
 *       .input   register identifier
 *       .ouput   identifier register
 *     ]
 *     body
 */
Expression * ASMStemnt::processDirectives(Expression * _body, const Location l)
{
  std::string buff = ((StringConstant *)_body)->buff;
  
  std::string :: iterator i;
  i = buff.begin(); 
  stripWhiteSpace(i);
  
  if (i != buff.end() && *i == '[') // process directives
  {
    i++;
    stripWhiteSpace(i);
    for (; i != buff.end() && *i != ']';)
    {
      // now we expect a .directive
      AsmDirective directive;
      asmDirective(i,directive);
      
      if (directive == ASM_Unknown)
      {
	  // FIXME: Add proper error messages...
	std::cerr << "Parser error in ASM directive";
	exit(1);
      }

      stripWhiteSpace(i);
      // process variable name
      std::string ident;

      readIdent(i,ident);

      switch (directive)
      {
      case ASM_Scratch:
	scratch.push_back(new StringConstant(ident,l,((StringConstant *)_body)->wide));
	break;
      case ASM_Input:
	{
	  stripWhiteSpace(i);
	
	  std::string ident2;
	  readIdent(i,ident2);
	
	  input.push_back(std::pair<Expression *,Expression *>
			  (new StringConstant(ident,l,((StringConstant *)_body)->wide),
			   new StringConstant(ident2,l,((StringConstant *)_body)->wide)));
	}
	break;
      case ASM_Output:
	{
	  stripWhiteSpace(i);

	  std::string reg;
	  readIdent(i,reg);
	  
	  output.push_back(std::pair<Expression *, Expression *>
			   (new StringConstant(ident,l,((StringConstant *)_body)->wide),
			    new StringConstant(reg,l,((StringConstant *)_body)->wide)));
	}
	break;
      default:
	// should never get here
	break;
      }
      stripWhiteSpace(i);
    }
  }

  if (i != buff.end()) //move passed ']'
    i++;
  
  stripWhiteSpace(i);
  std::string instrs(i,buff.end());
  
  StringConstant * result = new StringConstant(instrs,l,((StringConstant *)_body)->wide);

  // Avoid space leak; this would be better if we used reference counters
  delete _body;

  return result;  
}


Statement *ASMStemnt::dup0() const
{
    // First dup body
    ASMStemnt *ret = new ASMStemnt(body->dup(), location);

    // dup scratch directives
    std::list<Expression *> stmp = scratch; // we should not need this!
    std::list<Expression *> :: iterator i;

    for (i = stmp.begin(); i != stmp.end(); i++)
    {
      ret->scratch.push_back((*i)->dup());
    }

    // dup input directives
    std::list<std::pair<Expression *,Expression *> > tmp = input;
    std::list<std::pair<Expression *,Expression *> > :: iterator it;

    for (it = tmp.begin(); it != tmp.end(); it++)
    {
      ret->input.push_back(std::pair<Expression *,
			             Expression *>((*it).first->dup(),(*it).second->dup()));
    }

    // dup input directives
    tmp = output;

    for (it = tmp.begin(); it != tmp.end(); it++)
    {
      ret->output.push_back(std::pair<Expression *,
			              Expression *>((*it).first->dup(),(*it).second->dup()));
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void ASMStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* ASMStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    out << "asm {" << "\n" << "[\n";

    // print scratch directives
    std::list<Expression *> stmp = scratch; // we should not need this!
    std::list<Expression *> :: iterator i;

    for (i = stmp.begin(); i != stmp.end(); i++)
    {
      out << ".scratch ";
      (*i)->print(out);
      out << "\n";
    }

    // print input directives
    std::list<std::pair<Expression *,Expression *> > tmp = input;
    std::list<std::pair<Expression *,Expression *> > :: iterator it;

    for (it = tmp.begin(); it != tmp.end(); it++)
    {
      out << ".input ";
      (*it).first->print(out);
      out << " ";
      (*it).second->print(out);
      out << "\n";
    }

    // print output directives 
    tmp = output;
    for (it = tmp.begin(); it != tmp.end(); it++)
    {
      out << ".output ";
      (*it).first->print(out);
      out << " ";
      (*it).second->print(out);
      out << "\n";
    }

    out << "]\n";

    body->print(out);
    out << "\n}\n";
}
#if BROOK_TO_IL_CODE_GEN_PATH
void ASMStemnt::print_il(std::ostream & out, int level) const
{
    out << IL_PREFIX_STRING << "asm " ;

    // FIXME: Need to add code for directives...

    body->print_il(out);

    out << IL_PREFIX_STRING << "endasm\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK

void 
ASMStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "ASMStemnt::semanticCheck(%d)\n", isKernel);

    // if (!isKernel)
    //  recordError(this, SemanticChecker::SCEKStmtASM, "");
}
#endif // SEMANTIC_CHECK
#endif /* __ASMIL__ */

SwitchStemnt::SwitchStemnt(Expression * _cond, Statement * _block, const Location l):Statement(ST_SwitchStemnt, l)
{
    cond = _cond;
    block = _block;
    if (cond)
        cond->setParent(this);
}


SwitchStemnt::~SwitchStemnt()
{
    delete cond;
    delete block;
}


Statement *SwitchStemnt::dup0() const
{
    SwitchStemnt *ret = new SwitchStemnt(cond->dup(), block->dup(), location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void SwitchStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* SwitchStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    out << "switch (" << *cond << ")\n";

    //! Printing function block not required for sematic check
    if(globals.enableSematicCheck == false)
        printBlock(out, level, block);
}

#ifdef SEMANTIC_CHECK


void 
SwitchStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "SwitchStemnt::semanticCheck(%d)\n");

    if (cond)
        cond->semanticCheck(false);
    if (block)
        block->semanticCheck(false);
}
#endif // SEMANTIC_CHECK

void SwitchStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);
    //cond = (cb)(cond);
    cond->findExpr(cb);
    block->findExpr(cb);
}


void SwitchStemnt::replaceExpr(Expression * oldExp, Expression * newExp)
{
    assert(oldExp == cond);
    cond = newExp;
    if (newExp)
        newExp->setParent(this);
}


void SwitchStemnt::findStemnt(fnStemntCallback cb)
{
    (cb) (this);

    block->findStemnt(cb);
}


ForStemnt::ForStemnt(Expression * _init, Expression * _cond,
                     Expression * _incr, const Location l, Statement * _block /* =NULL */ )
:Statement(ST_ForStemnt, l)
{
    init = _init;
    cond = _cond;
    incr = _incr;
    if (init)
        init->setParent(this);
    if (cond)
        cond->setParent(this);
    if (incr)
        incr->setParent(this);

    block = _block;
}


ForStemnt::~ForStemnt()
{
    delete init;
    delete cond;
    delete incr;

    delete block;
}


Statement *ForStemnt::dup0() const
{
    ForStemnt *ret = new ForStemnt(init->dup(), cond->dup(), incr->dup(),
                                   location, block->dup());

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void ForStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* ForStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    out << "for (";

    if (init)
        out << *init;
    out << "; ";

    if (cond)
        out << *cond;
    out << "; ";

    if (incr)
        out << *incr;
    out << ")\n";

    printBlock(out, level, block);
}

#if BROOK_TO_IL_CODE_GEN_PATH
void ForStemnt::print_il(std::ostream & out, int level) const
{
    if (init)
        init->print_il(out);

    out << IL_PREFIX_STRING << "whileloop\n";

    if (cond)
    {
        cond->print_il(out);

        bril::store tmp = *cond->get_il_store();

        out << IL_PREFIX_STRING << "break_logicalz " << bril::get_gpr_name(&tmp, bril::rs_src_sel) << "\n";
    }

    block->print_il(out, level);

    if (incr)
        incr->print_il(out);

    out << IL_PREFIX_STRING << "endloop\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK

//! Returns ture if continue statement found in the given block and makes a block of statements
//! which contains continue statement and increment block of for and adds this block at appropriate 
//! place. Appropriate place is where continue is there.
bool 
ForStemnt::searchContinueStemnt(Block* blockStemnt)
{
    bool continueFound = false;
    Statement *preStemnt = blockStemnt->head;
    unsigned int i = 0;
    for(Statement *stmt = blockStemnt->head; stmt != NULL; stmt = stmt->next)
    {
        //! Track pre statement
        if(i >= 2)
            preStemnt = preStemnt->next;
        i++;


        //! Skip if statement is any loop type
        if(stmt->type == ST_ForStemnt || stmt->type == ST_DoWhileStemnt || stmt->type == ST_WhileStemnt)
            continue;

        if(stmt->type == ST_Block)
        {
            Block *innerBlock = static_cast<Block*>(stmt);
            bool result = false;
            result = searchContinueStemnt(innerBlock);
            continueFound = continueFound ? continueFound : result;
        }
        else
        {

            //! replace continue statement with a block which contains increment block of for and continue statement
            if(stmt->type == ST_ContinueStemnt)
            {
                continueFound = true;
                Block *newBlockStemnt = new Block(preStemnt->next->location);
                ExpressionStemnt *incrExprStemnt = new ExpressionStemnt(incr->dup0(), incr->location);
                newBlockStemnt->add(incrExprStemnt);
                newBlockStemnt->add(preStemnt->next);

                newBlockStemnt->next = preStemnt->next->next;
                preStemnt->next = newBlockStemnt;
            }
            else if(stmt->type == ST_IfStemnt)
            {
                //! Search in if statement
                IfStemnt *tmpIfStemnt = static_cast<IfStemnt *>(stmt);
                if(tmpIfStemnt->thenBlk)
                {
                    if(tmpIfStemnt->thenBlk->type == ST_Block)
                    {
                        Block *innerBlock = static_cast<Block*>(tmpIfStemnt->thenBlk);
                        bool result = false;
                        result = searchContinueStemnt(innerBlock);
                        continueFound = continueFound ? continueFound : result;
                    }
                    else 
                    {
                        if(tmpIfStemnt->thenBlk->type == ST_ContinueStemnt)
                        {
                            continueFound = true;
                            Block *newBlockStemnt = new Block(tmpIfStemnt->thenBlk->location);
                            ExpressionStemnt *incrExprStemnt = new ExpressionStemnt(incr->dup0(), incr->location);
                            newBlockStemnt->add(incrExprStemnt);
                            newBlockStemnt->add(tmpIfStemnt->thenBlk);
                            tmpIfStemnt->thenBlk = newBlockStemnt;
                        }
                    }
                }
                if(tmpIfStemnt->elseBlk)
                {
                    if(tmpIfStemnt->elseBlk->type == ST_Block)
                    {
                        Block *innerBlock = static_cast<Block*>(tmpIfStemnt->elseBlk);
                        bool result = false;
                        result = searchContinueStemnt(innerBlock);
                        continueFound = continueFound ? continueFound : result;
                    }
                    else 
                    {
                        if(tmpIfStemnt->elseBlk->type == ST_ContinueStemnt)
                        {
                            continueFound = true;
                            Block *newBlockStemnt = new Block(tmpIfStemnt->elseBlk->location);
                            ExpressionStemnt *incrExprStemnt = new ExpressionStemnt(incr->dup0(), incr->location);
                            newBlockStemnt->add(incrExprStemnt);
                            newBlockStemnt->add(tmpIfStemnt->elseBlk);
                            tmpIfStemnt->elseBlk = newBlockStemnt;
                        }
                    }
                }
            }
        }
    }

    return continueFound;
}


//! Find the continue statement in the block
//! and groups the continue and increment expression as block of statement if continue
//! WHY: To fix HLSL code generation issue (for loop with continue  )
void 
ForStemnt::groupContinueAndIncrExpr(bool isKernel)
{
    if(isKernel)
    {
        //! Find continue statement
        if(block->type == ST_Block)
        {
            bool continueFound = false;
            Block *tmpBlock = static_cast<Block*>(block);
            continueFound = searchContinueStemnt(tmpBlock);
            if(continueFound)
            {
                ExpressionStemnt *incrExprStemnt = new ExpressionStemnt(incr->dup0(), incr->location);
                tmpBlock->tail->next = incrExprStemnt;

                //! Initialize to null 
                delete incr;
                incr = NULL;
            }
        }
    }

}
void
ForStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "ForStemnt::semanticCheck(%d)\n", isKernel);
    if (init)
        init->semanticCheck(isKernel);
    if (cond)
        cond->semanticCheck(isKernel);
    if (incr)
        incr->semanticCheck(isKernel);
    if (block)
        block->semanticCheck(isKernel);

    //! check semantics of conditional expressions
    semanticChecker.checkConditionalSemantics(cond, isKernel);

    groupContinueAndIncrExpr(isKernel);
}

#endif // SEMANTIC_CHECK
void ForStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);

    if (init != NULL)
    {
        //init = (cb)(init);
        init->findExpr(cb);
    }
    if (cond != NULL)
    {
        //cond = (cb)(cond);
        cond->findExpr(cb);
    }
    if (incr != NULL)
    {
        //incr = (cb)(incr);
        incr->findExpr(cb);
    }
    if (block != NULL)
    {
        block->findExpr(cb);
    }
}


void ForStemnt::replaceExpr(Expression * oldExp, Expression * newExp)
{
    if (oldExp == init)
        init = newExp;
    else if (oldExp == cond)
        cond = newExp;
    else if (oldExp == incr)
        incr = newExp;
    else
        assert(0);

    if (newExp)
        newExp->setParent(this);
}


void ForStemnt::findStemnt(fnStemntCallback cb)
{
    (cb) (this);

    if (block)
        block->findStemnt(cb);
}


WhileStemnt::WhileStemnt(Expression * _cond, Statement * _block, const Location l):Statement(ST_WhileStemnt, l)
{
    cond = _cond;
    block = _block;
    if (cond)
        cond->setParent(this);
}


WhileStemnt::~WhileStemnt()
{
    delete cond;
    delete block;
}


Statement *WhileStemnt::dup0() const
{
    WhileStemnt *ret = new WhileStemnt(cond->dup(), block->dup(), location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void WhileStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* WhileStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    out << "while (" << *cond << ")\n";

    printBlock(out, level, block);
}

#if BROOK_TO_IL_CODE_GEN_PATH
void WhileStemnt::print_il(std::ostream & out, int level) const
{
    out << IL_PREFIX_STRING << "whileloop\n";
    cond->print_il(out);

    bril::store tmp = *cond->get_il_store();

    out << IL_PREFIX_STRING << "break_logicalz " << bril::get_gpr_name(&tmp, bril::rs_src_sel) << "\n";

    block->print_il(out, level);

    out << IL_PREFIX_STRING << "endloop\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void 
WhileStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "WhileStemnt::semanticCheck(%d)\n", isKernel);
    if (cond)
        cond->semanticCheck(isKernel);

    if (block)
        block->semanticCheck(isKernel);   

    //! check semantics of conditional expressions
    semanticChecker.checkConditionalSemantics(cond, isKernel);
}

#endif // SEMANTIC_CHECK
void WhileStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);

    //cond = (cb)(cond);
    cond->findExpr(cb);

    if (block)
    {
        block->findExpr(cb);
    }
}


void WhileStemnt::replaceExpr(Expression * oldExp, Expression * newExp)
{
    assert(oldExp == cond);
    cond = newExp;
    if (newExp)
        newExp->setParent(this);
}


void WhileStemnt::findStemnt(fnStemntCallback cb)
{
    (cb) (this);

    if (block)
        block->findStemnt(cb);
}


DoWhileStemnt::DoWhileStemnt(Expression * _cond, Statement * _block, const Location l):Statement(ST_DoWhileStemnt, l)
{
    cond = _cond;
    block = _block;
    if (cond)
        cond->setParent(this);
}


DoWhileStemnt::~DoWhileStemnt()
{
    delete cond;
    delete block;
}


Statement *DoWhileStemnt::dup0() const
{
    DoWhileStemnt *ret = new DoWhileStemnt(cond->dup(), block->dup(), location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void DoWhileStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* DoWhileStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);
    out << "do ";

    if (!block->isBlock())
        out << std::endl;

    printBlock(out, level, block);

    if (!block->isBlock())
        out << std::endl;

    indent(out, level);
    out << "while (" << *cond << ")";
}

#ifdef SEMANTIC_CHECK
void DoWhileStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "DoWhileStemnt::semanticCheck(%d)\n", isKernel);
    if (cond)
        cond->semanticCheck(isKernel);

    if (block)
        block->semanticCheck(isKernel);

    //! check semantics of conditional expressions
    semanticChecker.checkConditionalSemantics(cond, isKernel);
}
#endif

void DoWhileStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);

    //cond = (cb)(cond);
    cond->findExpr(cb);

    if (block)
    {
        block->findExpr(cb);
    }
}


void DoWhileStemnt::replaceExpr(Expression * oldExp, Expression * newExp)
{
    assert(oldExp == cond);
    cond = newExp;
    if (newExp)
        newExp->setParent(this);
}


void DoWhileStemnt::findStemnt(fnStemntCallback cb)
{
    (cb) (this);

    if (block)
        block->findStemnt(cb);
}


GotoStemnt::GotoStemnt(Symbol * _dest, const Location l):Statement(ST_GotoStemnt, l)
{
    dest = _dest;
}


GotoStemnt::~GotoStemnt()
{
    delete dest;
}


Statement *GotoStemnt::dup0() const
{
    GotoStemnt *ret = new GotoStemnt(dest->dup(), location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void GotoStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* GotoStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);
    out << "goto " << *dest;
}
#ifdef SEMANTIC_CHECK
void GotoStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "GotoStemnt::semanticCheck(%d)\n", isKernel);
    if (isKernel)
        RecordSemanticError(this, SemanticChecker::SCEKStmtControlFlow);
}
#endif

ReturnStemnt::ReturnStemnt(Expression * _result, const Location l):Statement(ST_ReturnStemnt, l)
{
    result = _result;
    if (result)
        result->setParent(this);
}


ReturnStemnt::~ReturnStemnt()
{
    delete result;
}


Statement *ReturnStemnt::dup0() const
{
    ReturnStemnt *ret = new ReturnStemnt(result->dup(), location);

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void ReturnStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* ReturnStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);
    out << "return";

    if (result)
        out << " " << *result;
}


#if BROOK_TO_IL_CODE_GEN_PATH
FunctionDef *get_current_kernel();

void ReturnStemnt::print_il(std::ostream & out, int UNUSED(level)) const
{
    if (result)
    {
        FunctionDef *fd = get_current_kernel();

        assert(!fd->returnsVoid());

        result->print_il(out);
        bril::emitCopy(out, fd->get_il_store(), result->get_il_store());
    }

    // This needs to be ret_dyn (which is only on R600 - using R500 right now). 
    // Otherwise, we have a bunch more work to do.

    // out << "ret_dyn\n";
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void ReturnStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "ReturnStemnt::semanticCheck(%d)\n", isKernel);
    if (result)
        result->semanticCheck(isKernel);
}
#endif
void ReturnStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);

    if (result)
    {
        //result = (cb)(result);
        result->findExpr(cb);
    }
}


void ReturnStemnt::replaceExpr(Expression * oldExp, Expression * newExp)
{
    assert(oldExp == result);
    result = newExp;
    if (newExp)
        newExp->setParent(this);
}


DeclStemnt::DeclStemnt(const Location l, StatementType stype /* =ST_DeclStemnt */ )
:Statement(stype, l)
{
}


DeclStemnt::~DeclStemnt()
{
    DeclVector::iterator j;

    for (j = decls.begin(); j != decls.end(); j++)
        delete *j;
}


void DeclStemnt::addDecl(Decl * decl)
{
    decls.push_back(decl);
}

void DeclStemnt::addDecls(Decl * decls)
{
    Decl *decl = decls;

    while (decls != NULL)
    {
        decls = decls->next;
        decl->next = NULL;
        //std::cout << "Decl is: ";
        //decl->print(std::cout,true);
        //std::cout << std::endl;
        addDecl(decl);
        decl = decls;
    }
}


Statement *DeclStemnt::dup0() const
{
    DeclStemnt *ret;

    if (type == ST_TypedefStemnt)
        ret = new TypedefStemnt(location);
    else
        ret = new DeclStemnt(location);

    DeclVector::const_iterator j;
    for (j = decls.begin(); j != decls.end(); j++)
        ret->addDecl((*j)->dup());

    LabelVector::const_iterator k;
    for (k = labels.begin(); k != labels.end(); k++)
        ret->addLabel((*k)->dup());

    return ret;
}


DeclStemnt *DeclStemnt::convertToTypedef()
{
    // Nothing to do?
    if (isTypedef())
        return this;

    TypedefStemnt *ret = new TypedefStemnt(location);

    // Since we are really the same thing,
    // let's just steal the insides.
    LabelVector::iterator k;
    for (k = labels.begin(); k != labels.end(); k++)
    {
        ret->addLabel(*k);
    }
    labels.clear();

    ret->next = next;
    next = NULL;

    DeclVector::iterator j;

    for (j = decls.begin(); j != decls.end(); j++)
    {
        ret->addDecl(*j);
    }

    decls.clear();

    delete this;                // Danger - Will Robinson!

    return ret;
}


void DeclStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* DeclStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    if (!decls.empty())
    {
        DeclVector::const_iterator j = decls.begin();

        //! Skip array is a shared array for both cpu and cal backends
        if((*j)->form->getQualifiers() != TQ_Shared)
            (*j)->print(out, true, level);
        else
        {
            out << "//";
            (*j)->print(out, true, level);
            out << "\t//Removed shared array from declaration";
        }


        for (j++; j != decls.end(); j++)
        {
            if((*j)->form->getQualifiers() == TQ_Shared)
                continue;

            // It is ugly fix to make the new C++ API working
            if((*j)->isStream())
            {
                out << ";" << std::endl << "  ";
                (*j)->print(out, true, level);
            }
            else
            {
                out << ", ";
                (*j)->print(out, false, level);
            }
        }
    }

    out << ";";

    // TIM: add mangled declarations as needed:
    DeclVector::const_iterator k = decls.begin();
    for (; k != decls.end(); ++k)
    {
        (*k)->printStructureStreamHelpers(out);
        // TIM: broken const-ness, but I don't care
        // to fix it... :)
        Decl *decl = const_cast < Decl * >(*k);

        decl->printStructureStreamShape(out);
    }
}

#if BROOK_TO_IL_CODE_GEN_PATH
void DeclStemnt::print_il(std::ostream & out, int level) const
{
    if (!decls.empty())
    {
        DeclVector::const_iterator j = decls.begin();

        (*j)->print_il(out, level);
        for (j++; j != decls.end(); j++)
        {
            (*j)->print_il(out, level);
        }
    }

#if 0
    // TIM: add mangled declarations as needed:
    DeclVector::const_iterator k = decls.begin();
    for (; k != decls.end(); ++k)
    {
        (*k)->printStructureStreamHelpers(out);

        // TIM: broken const-ness, but I don't care
        // to fix it... :)
        Decl *decl = const_cast < Decl * >(*k);

        decl->printStructureStreamShape(out);
    }
#endif
}

#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void 
DeclStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "DeclStemnt::semanticCheck(%d)\n", isKernel);
    for (DeclVector::const_iterator k = decls.begin(); k != decls.end(); ++k)
    {
        semanticChecker.checkDecl((*k), this, isKernel);
    }
}
#endif
void DeclStemnt::findExpr(fnExprCallback cb)
{
    Statement::findExpr(cb);

    DeclVector::iterator j;

    for (j = decls.begin(); j != decls.end(); j++)
    {
        (*j)->findExpr(cb);
    }
}


TypedefStemnt::TypedefStemnt(const Location l):DeclStemnt(l, ST_TypedefStemnt)
{
}

TypedefStemnt::~TypedefStemnt()
{
}


void TypedefStemnt::print(std::ostream & out, int level) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* TypedefStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);

    out << "typedef ";

    if (!decls.empty())
    {
        DeclVector::const_iterator j = decls.begin();

        (*j)->print(out, true, level);
        for (j++; j != decls.end(); j++)
        {
            out << ", ";
            (*j)->print(out, false, level);
        }
    }

    out << ";";
}
#ifdef SEMANTIC_CHECK
void 
TypedefStemnt::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "TypedefStemnt::semanticCheck(%d)\n", isKernel);
}
#endif

Block::Block(const Location l):Statement(ST_Block, l)
{
    head = tail = NULL;
}


Block::~Block()
{
    Statement *stemnt, *prevStemnt = NULL;

    for (stemnt = head; stemnt; stemnt = stemnt->next)
    {
        delete prevStemnt;

        prevStemnt = stemnt;
    }
    delete prevStemnt;
}


void Block::add(Statement * stemnt)
{
    if (stemnt)
    {
        stemnt->next = NULL;

        if (tail)
            tail->next = stemnt;
        else
            head = stemnt;

        tail = stemnt;
    }
}


void Block::addStatements(Statement * stemnts)
{
    Statement *stemnt;

    while (stemnts != NULL)
    {
        stemnt = stemnts;
        stemnts = stemnts->next;

        stemnt->next = NULL;
        add(stemnt);
    }
}

void Block::addDeclStemnts(DeclStemnt * decls)
{
    DeclStemnt *decl = decls;

    while (decls != NULL)
    {
        decls = static_cast<DeclStemnt*>(decls->next);
        decl->next = NULL;
        add(decl);
        decl = decls;
    }
}

Statement *Block::dup0() const
{
    Block *ret = new Block(location);

    for (Statement * stemnt = head; stemnt; stemnt = stemnt->next)
        ret->add(stemnt->dup());

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void Block::print(std::ostream & out, int level) const
{
    bool isDecl;
    Statement *stemnt;

    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* BlockStemnt:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        (*j)->print(out, level);

    indent(out, level);
    out << "{\n";

    // TIM:
    // the Cg compiler apparently hates empty functions
    // so we make this change here... this is a terrible
    // place to do it, but I can't think of a better solution

    // Ian:  I took this out.  We should not be allowing empty
    // kernels anyway.

#if 0
    if (head == NULL)
    {
        out << "int __bogus=0; // to make empty function non-empty" << std::endl;
    }
#endif

    isDecl = (head != NULL) ? head->isDeclaration() : false;
    for (stemnt = head; stemnt; stemnt = stemnt->next)
    {
        if (isDecl && !stemnt->isDeclaration())
        {
            isDecl = false;
            out << std::endl;
        }

        stemnt->print(out, level + 1);

        if (stemnt->needSemicolon())
            out << ";";
        out << std::endl;
    }

    indent(out, level);
    out << "}\n";
}

#if BROOK_TO_IL_CODE_GEN_PATH
void Block::print_il(std::ostream & out, int level) const
{
    Statement *stemnt;

    for (stemnt = head; stemnt; stemnt = stemnt->next)
        stemnt->print_il(out, level + 1);
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

#ifdef SEMANTIC_CHECK
void
Block::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "Block::semanticCheck(%d)\n", isKernel);
    for (Statement *stmt = head; stmt; stmt = stmt->next)
    {
        stmt->semanticCheck(isKernel);
    }
}
#endif

void Block::findExpr(fnExprCallback cb)
{
    Statement *stemnt;

    Statement::findExpr(cb);

    for (stemnt = head; stemnt; stemnt = stemnt->next)
    {
        stemnt->findExpr(cb);
    }
}


void Block::findStemnt(fnStemntCallback cb)
{
    Statement *stemnt;

    (cb) (this);

    for (stemnt = head; stemnt; stemnt = stemnt->next)
    {
        stemnt->findStemnt(cb);
    }
}


void Block::insert(Statement * stemnt, Statement * after /* =NULL */ )
{
    if (stemnt)
    {
        stemnt->next = NULL;

        if (tail)
        {
            if (after)
            {
                stemnt->next = after->next;
                after->next = stemnt;
            }
            else
            {
                stemnt->next = head;
                head = stemnt;
            }

            if (stemnt->next == NULL)
                tail = stemnt;
        }
        else
            head = tail = stemnt;
    }
}


FunctionDef::FunctionDef(const Location l):Block(l)
{
    decl = NULL;
    kernelAttribute = NULL;
    shaderType = PixelShader;
    localDataShareSize = 0;
    sharedArrayTypeComponents = 0;
}

FunctionDef::~FunctionDef()
{
    delete decl;
    delete kernelAttribute;
}


bool FunctionDef::returnsVoid() const
{
    FunctionType *ft = static_cast < FunctionType * >(decl->form);

    return (decl->form->getBase()->typemask & BT_Void) && ft->subType->isBaseType();
}
#if BROOK_TO_IL_CODE_GEN_PATH
bril::store * FunctionDef::get_il_store() const
{
    decl->form->getBase()->allocateILStorage(&il_store);
    return &il_store;
}
#endif //BROOK_TO_IL_CODE_GEN_PATH

Statement *FunctionDef::dup0() const
{
    FunctionDef *ret = new FunctionDef(location);

    ret->decl = decl->dup();
    ret->kernelAttribute = kernelAttribute->dup();
    ret->shaderType = shaderType;
    ret->localDataShareSize = localDataShareSize;
    ret->sharedArrayTypeComponents = sharedArrayTypeComponents;

    for(unsigned int i = 0; i < groupSizeValues.size(); ++i)
    {
        ret->groupSizeValues.push_back(groupSizeValues[i]);
    }

    for (Statement * stemnt = head; stemnt; stemnt = stemnt->next)
        ret->add(stemnt->dup());

    LabelVector::const_iterator j;
    for (j = labels.begin(); j != labels.end(); j++)
        ret->addLabel((*j)->dup());

    return ret;
}


void FunctionDef::print(std::ostream & out, int) const
{
    if (globals.printLineDirectives)
        location.printLineDirective(out);

    if (Project::gDebug)
    {
        out << "/* FunctionDef:";
        location.printLocation(out);
        out << " */" << std::endl;
    }

    decl->print(out, true);
    out << std::endl;

    //! Print kernel attributes if requires
    if(globals.enableSematicCheck == true && kernelAttribute)
    {
        GccAttrib* tempAttrib = kernelAttribute;
        while(tempAttrib)
        {
            tempAttrib->print(out);
            tempAttrib = tempAttrib->next;
        }
    }

    //! Printing function block not required for sematic check
    if(globals.enableSematicCheck == false)
        Block::print(out, 0);
}

#ifdef SEMANTIC_CHECK
void FunctionDef::checkKernelAttributeSemantics(bool isKernel)
{
    if(isKernel)
    {
        if(kernelAttribute)
        {
            bool isComputeKernel = false;
            GccAttrib *tempAttrib = kernelAttribute;
            bool moreGroupSizes = false;
            while(tempAttrib)
            {
                if(tempAttrib->type == Kernel_GroupSize)
                {
                    isComputeKernel = true;
                    if(moreGroupSizes)
                    {
                        std::ostringstream msg;
                        msg << " More than one GroupSize specified(";
                        tempAttrib->print(msg);
                        msg << ")";
                        msg << ": only one GroupSize allowed";
                        RecordSemanticError(this, SemanticChecker::SCEKNull, msg.str().c_str());
                        break;
                    }
                    moreGroupSizes = true;
                    if(tempAttrib->ns)
                    {
                        INumList* tempNumList = tempAttrib->ns;
                        unsigned int groupSize = 0;
                        //std::vector<int> values;
                        while(tempNumList)
                        {
                            groupSizeValues.push_back(tempNumList->value);
                            tempNumList = tempNumList->next;
                        }
                        
                        if(groupSizeValues.size() > 4)
                        {
                            std::ostringstream msg;
                            msg << "More than 4 componensts not allowed in GroupSize(";
                            tempAttrib->print(msg);
                            msg << ")";
                            RecordSemanticError(this, SemanticChecker::SCEKNull, msg.str().c_str());
                        }

                        for(unsigned int i = 0; i < groupSizeValues.size() - 1; ++i)
                        {
                            if(groupSizeValues[i] != 1)
                            {
                                std::ostringstream msg;
                                msg << groupSizeValues.size() - i << " dimension of GroupSize must be 1(";
                                tempAttrib->print(msg);
                                msg << ")";
                                RecordSemanticError(this, SemanticChecker::SCEKNull, msg.str().c_str());
                            }
                        }
                        if(groupSizeValues[groupSizeValues.size() - 1] > 1024)
                        {
                            std::ostringstream msg;
                            msg << " First dimension of GroupSize must be <= 1024 (";
                            tempAttrib->print(msg);
                            msg << ")";
                            RecordSemanticError(this, SemanticChecker::SCEKNull, msg.str().c_str());
                        }

                    }
                    else
                    {
                        std::ostringstream msg;
                        msg << "GroupSize must have values(";
                        tempAttrib->print(msg);
                        msg << ")";
                        RecordSemanticError(this, SemanticChecker::SCEKNull, msg.str().c_str());
                    }
                    

                }
                else
                {
                    std::ostringstream msg;
                    msg << "Unsupported kernel Attribute(";
                    kernelAttribute->print(msg);
                    msg << ")";
                    RecordSemanticError(this, SemanticChecker::SCEKNull, msg.str().c_str());
                }

                tempAttrib = tempAttrib->next;
            }

            if(isComputeKernel)
                setShaderType(ComputeShader);
        }
    }
}

//! Returns appropriate GCCAtrrib only after Semantic check.
GccAttrib* FunctionDef::getGroupSize() const
{
    if(kernelAttribute)
    {
        GccAttrib *tempAttrib = kernelAttribute;
        while(tempAttrib)
        {
            if(tempAttrib->type == Kernel_GroupSize)
            {
                return tempAttrib;
            }
            tempAttrib = tempAttrib->next;
        }
    }
    return NULL;
}
void FunctionDef::setShaderType(ShaderType type)
{
    shaderType = type;
}

static unsigned int builtinMask = 0x00000000;
static unsigned int indexofMask = 0x00000001;
static unsigned int instanceMask = 0x00000002;
static unsigned int instanceInGroupMask = 0x00000004;
static unsigned int syncGroupMask = 0x00000008;

Expression * FunctionDef::setBuiltinMask(Expression *expr)
{
    LOG(LOG_FN_ENTRY, "CgFunction::impCollectIndexof(%p)\n", expr);
    if (expr->etype == ET_BrtIndexofExpr)
    {
        builtinMask |= indexofMask;
    }
    else if(expr->etype == ET_BrtInstanceExpr)
    {
        builtinMask |= instanceMask;
    }
    else if (expr->etype == ET_BrtInstanceInGroupExpr)
    {
        builtinMask |= instanceInGroupMask;
    }
    else if(expr->etype == ET_BrtSyncGroupExpr)
    {
        builtinMask |= syncGroupMask;
    }

    return expr;

}


//! checks the kernel built-in functions usage
//! Kernel build-in functions: instance(), indexof(), instanceInGroup() and syncGroup()
void 
FunctionDef::checkKernelBuiltinFuncUsage(bool isKernel)
{
    if(isKernel)
    {
        builtinMask = 0x00000000;
        findExpr(setBuiltinMask);

        //! Give error if reduction kernel has any of the following functions
        //! indexof, instance, instanceInGroup and syncGroup
        if (decl->isReduce())
        {
            if (builtinMask)
            {
                if(builtinMask & indexofMask)
                    RecordSemanticError(decl, this, SemanticChecker::SCEKNull, 
                                        "Illegal to use indexof() function in reduction kernel");
                if(builtinMask & instanceMask)
                    RecordSemanticError(decl, this, SemanticChecker::SCEKNull, 
                                        "Illegal to use instance() function in reduction kernel");
                if(builtinMask & instanceInGroupMask)
                    RecordSemanticError(decl, this, SemanticChecker::SCEKNull, 
                                        "Illegal to use instanceInGroup() function in reduction kernel");
                if(builtinMask & syncGroupMask)
                    RecordSemanticError(decl, this, SemanticChecker::SCEKNull, 
                                        "Illegal to use syncGroup() function in reduction kernel");
            }
        }

        if(shaderType == PixelShader)
        {
            if(builtinMask & instanceInGroupMask)
                RecordSemanticError(decl, this, SemanticChecker::SCEKNull, 
                                    "Illegal to use instanceInGroup() function for pixel shader kernel");
            if(builtinMask & syncGroupMask)
                RecordSemanticError(decl, this, SemanticChecker::SCEKNull, 
                                    "Illegal to use syncGroup() function for pixel shader kernel");
        }
    }
}

void
FunctionDef::semanticCheck(bool isKernel)
{
    LOG(LOG_FN_ENTRY, "FunctionDef::semanticCheck(%d)\n", isKernel);
    if (decl->isKernel())
    {
        CGASSERT(isKernel==false, "nested function definition is not allowed");
        isKernel = true;
    }

    checkKernelAttributeSemantics(isKernel);
    //check formal parameters declaration
    CGASSERT(decl->form->isFunction(), "internal error");
    FunctionType* funcType = AsFunctionType(decl->form);

    Type* subType = *funcType->getSubType();
    if (subType->isBaseType() && (AsBaseType(subType))->typemask == 0)
        RecordSemanticError(decl, this, SemanticChecker::SCEBOther, "unspecified kernel return type");

    int inStream = 0;
    int outOther = 0;
    int outReduction = 0;
    int outScatter = 0;
    int gatherStream = 0;
    int others = 0;

    //! Increment only if constant buffer used for gather array
    int constBufferCount = 0;
#define MAX_COUNT_ALLOWED_FOR_GATHER_ARRAY_AS_CONST_BUFFERS 10

    for (int i = 0; i < AsInt(funcType->nArgs); i++)   
    { 
        Decl* paramDcl = funcType->args[i];
        semanticChecker.checkDecl(paramDcl, this, isKernel, true);
        Type* dclType = paramDcl->form;

        //! Set kernelParam is true
        //! This information can be used to check semantics of body of definition
        dclType->kernelParam = true;

        if (IsScatterOutType(dclType))
        {
            outScatter ++;
        } 
        else if (IsReductionOut(dclType->getQualifiers()))
        {
            outReduction ++;
        } 
        else if (IsOutputArg(dclType->getQualifiers()))
        {
            outOther ++;
        }
        else if (paramDcl->isStream() && IsOutputArg(dclType->getQualifiers()) == false)
        {
            inStream ++;
        }
        else if (paramDcl->isArray())
        {
            gatherStream ++;
            ArrayType *arrayType = AsArrayType(paramDcl->form);
            if(globals.disableConstBuffer == false)
            {
                if(constBufferCount < MAX_COUNT_ALLOWED_FOR_GATHER_ARRAY_AS_CONST_BUFFERS)
                {
                    constBufferCount++;
                }
                else
                {
				    //! Use stream resource for gather array even it saticifies the constant buffer requirements
                    arrayType->resourceType = TextureType;
                }
            }
            else
            {
                arrayType->resourceType = TextureType;
            }
        }
        else
        {
            others ++;
        }
    }

    if (decl->isReduce())
    {
        if (! (outReduction == 1 && inStream == 1 && 
            (gatherStream + outScatter + outOther) == 0))
        {
            RecordSemanticError(decl, this, SemanticChecker::SCEBOther, 
                "reduction kernel can only have one input stream and one reduction output scalar/stream");
        }
    }

    if (decl->isReduce() == false && outReduction != 0)
    {
        RecordSemanticError(decl, this, SemanticChecker::SCEBOther, 
                "Specify reduce qualifier for kernel");
    }

    if(isKernel)
    {
        if(outScatter > 1)
        {
            RecordSemanticError(decl, this, SemanticChecker::SCEBOther, 
                            "Only one scatter output allowed");
        }
        if(outScatter == 0 && shaderType == ComputeShader || shaderType == ComputeShader && outOther > 0)
        {
            RecordSemanticError(decl, this, SemanticChecker::SCEBOther, 
                            "Output must be a scatter type if you specify GroupSize attribute");
        }
        if(outOther > CAL_MRT && globals.userTargets & TARGET_CAL)
        {
            int passes = outOther / CAL_MRT + 1;
            std::ostringstream tmpStr;
            tmpStr << "kernel required "<< passes << " passes. There could be redundant calculations";
            if(outScatter)
                tmpStr << "\n\tNote: Scatter output will be included in each pass";
            RecordSemanticWarn(decl, this, SemanticChecker::SCWNull, tmpStr.str().c_str());
        }

        //! Compare kernel parameter declarations and kernel local declarations
        //! Actually parser should handle this situation
        for (Statement *stmt = head; stmt; stmt = stmt->next)
        {
            if(stmt->isDeclaration())
            {
                DeclStemnt *declStmt = static_cast<DeclStemnt *>(stmt);
                for (DeclVector::const_iterator k = declStmt->decls.begin(); k != declStmt->decls.end(); ++k)
                {
                    Decl *localDecl = (*k);
                    Type *dclType = localDecl->form;
                    //! Semantic checks specific to shared local array
                    if(dclType->type == TT_Array && dclType->getQualifiers() == TQ_Shared)
                    {
                        if(shaderType != ComputeShader)
                        {
                            RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "Shared array allowed only for computer shaders: specify GroupSize Attribute");

                        }
                        semanticChecker.checkArrayDeclSemantics(localDecl, declStmt, isKernel, false);
                        if(localDataShareSize != 0)
                            RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "Only One Shared local array allowed");

                        ArrayType *arrayType = static_cast<ArrayType *>(dclType);
                        if(is128bitBaseType(arrayType->getBase()->typemask) == false)
                        {
                            RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "Shared array type must be 128 bit type");
                        }
                        else
                        {
                            sharedArrayTypeComponents = 4;
                        }

                        //! Skip GroupSize/SharedArraySize semantic checks if array declaration semantics are wrong
                        if(arrayType->isSemanticsOk == true)
                        {
                            localDataShareSize = arrayType->getTotalElementCount();
                            int groupSize = 1;
                            for(unsigned int i = 0; i < groupSizeValues.size(); ++i)
                            {
                                groupSize *= groupSizeValues[i];
                            }
                            arrayType->groupSize = groupSize;

                            if(groupSize > 1024)
                            {
                                RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "Group size must be <= 1024");
                            }
                            if(localDataShareSize * 4 > 4 * 1024)
                            {
                                RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "Shared array size must be <= 4K 32-bit type");
                            }

                            if(((localDataShareSize * 4) / groupSize) % 4 != 0)
                            {
                                RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "SharedArraySize/GroupSize must be multiples of 4");
                            }
                            if(((localDataShareSize * 4) / groupSize) > 64)
                            {
                                RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "SharedArraySize/GroupSize must be <= 64");
                            }
                            if(((localDataShareSize * 4) % groupSize) != 0)
                            {
                                RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, "SharedArraySize%GroupSize must be zero");
                            }
                        }
                    }
                    for (int i = 0; i < AsInt(funcType->nArgs); i++)   
                    { 
                        Decl* paramDcl = funcType->args[i];
                        if(localDecl->name->name.compare(paramDcl->name->name) == 0)
                        {
                            std::ostringstream tmpStr;
                            tmpStr << "Variable name Duplicated : " << localDecl->name->name.c_str();
                            RecordSemanticError(localDecl, declStmt, SemanticChecker::SCEKNull, tmpStr.str().c_str());
                        }
                        
                    }
                }
            }
        }
    }

    Block::semanticCheck(isKernel);

    //! Check return type semantics
    if(isKernel)
    {
        for (Statement *stmt = head; stmt; stmt = stmt->next)
        {
            if(stmt->type == ST_ReturnStemnt)  //! Check return type 
            {
                ReturnStemnt *returnStmt = static_cast<ReturnStemnt *>(stmt);
                if(returnStmt->result)
                {
                    Type *returnType = returnStmt->result->getType();
                    BTDetails retTypeDetails = getTypeDetails(returnType->getBase()->typemask);
                    BTDetails subTypeDetails = getTypeDetails(subType->getBase()->typemask);

                    BaseTypeInfo *retTypeInfo = getBaseTypeInfo(returnType->getBase()->typemask);
                    BaseTypeInfo *subTypeInfo = getBaseTypeInfo(subType->getBase()->typemask);

                    if(returnType)
                    {
                        if(globals.enableStrongTypeChecking)
                        {
                            if(!checkForTypeAndComponents(subTypeDetails, retTypeDetails))
                            {
                                std::ostringstream tmpStr;
                                tmpStr << "Mismatched type : " 
                                    << "actual type is " << retTypeInfo->brTypeName << " but expected type is " << subTypeInfo->brTypeName;
                                RecordSemanticWarn(returnStmt, SemanticChecker::SCWNull, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                                RecordSemanticNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                            }
                        }
                        else
                        {
                            if(checkForConversionLose(subTypeDetails, retTypeDetails))
                            {
                                std::ostringstream tmpStr;
                                tmpStr << "possible lose of data :" 
                                    << "conversion from " << retTypeInfo->brTypeName << " to " << subTypeInfo->brTypeName;
                                RecordSemanticWarn(returnStmt, SemanticChecker::SCWNull, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                            }
                        }
                    }
                    else
                    {
                        assert(0);
                        LOG(LOG_ERROR, " %s :line %d -> Return type information is not set by parser", __FILE__, __LINE__);
                    }
                }
                else
                {   
                    //! check for kernel return type if returns none 
                    if(subType->getBase()->typemask != BT_Void)
                    {
                        RecordSemanticError(returnStmt, SemanticChecker::SCEKNull, "Must return a value");
                    }
                }
            }
            else if(stmt->type == ST_SwitchStemnt)
            {
                SwitchStemnt *switchStmt = static_cast<SwitchStemnt *>(stmt);
                RecordSemanticError(switchStmt, SemanticChecker::SCEKNull, "Switch not supported inside kernel yet");
            }
            else if(stmt->type == ST_ExpressionStemnt)
            {
                ExpressionStemnt *exprStemnt = static_cast<ExpressionStemnt *>(stmt);
                Expression *expr = exprStemnt->expression;
                if(expr->etype == ET_AssignExpr)
                {
                    AssignExpr *assignExpr = static_cast<AssignExpr *>(expr);
                    IndexExpr *index = NULL;
                    if(assignExpr->lValue()->etype == ET_IndexExpr)
                        index = AsIndexExpr(assignExpr->lValue());
                    
                    if(index && index->accessType == IndexWriteAccess) //! for Array variable
                    {
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
                                        Constant * constExpr = NULL;
                                        if(mainExpr->_leftExpr->etype == ET_Constant)
                                        {
                                            constExpr = static_cast<Constant *>(mainExpr->_leftExpr);
                                            std::ostringstream errorMsg;
                                            if(constExpr->etype == CT_Int)
                                            {
                                                IntConstant *intConst = static_cast<IntConstant *>(constExpr);
                                                ArrayType *type = static_cast<ArrayType *>(arrayType);
                                                localDataShareSize = type->getTotalElementCount();
                                                int groupSize = 1;
                                                for(unsigned int i = 0; i < AsInt(groupSizeValues.size()); ++i)
                                                {
                                                    groupSize *= groupSizeValues[i];
                                                }
                                                if(intConst->lng < 0 || intConst->lng > (long)(localDataShareSize / groupSize))
                                                {
                                                    errorMsg << "offse value(" << intConst->lng <<") must be in the range of "
                                                             << "0" << " <= offset " << (localDataShareSize / groupSize); 
                                                }

                                            }
                                            mainExpr->_leftExpr->print(offset);
                                        }
                                        else
                                        {
                                            constExpr = static_cast<Constant *>(mainExpr->_leftExpr);
                                            mainExpr->_rightExpr->print(offset);

                                        }
                                    }
                                    else
                                    {
                                        mainExpr = static_cast<BinaryExpr *>(mainExpr->_leftExpr);
                                        if(mainExpr->_leftExpr->etype == ET_Constant)
                                            mainExpr->_leftExpr->print(offset);
                                        else
                                            mainExpr->_rightExpr->print(offset);
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }
    }


    checkKernelBuiltinFuncUsage(isKernel);
}
#endif

void FunctionDef::findExpr(fnExprCallback cb)
{
    decl->findExpr(cb);

    Block::findExpr(cb);
}


Symbol *FunctionDef::FunctionName() const
{
    return decl->name;
}


PPDirective::PPDirective(const std::string & line, const Location l):Statement(ST_PPDirective, l)
{
    directive = line;
    if (directive[directive.size() - 1] == '\r')
        directive.erase(directive.size() - 1, 1);
}


PPDirective::~PPDirective()
{
}


Statement *PPDirective::dup0() const
{
    PPDirective *ret = new PPDirective(directive, location);

    return ret;
}


void PPDirective::print(std::ostream & out, int UNUSED(level)) const
{
#ifdef PRINT_LOCATION
    out << "$<PPDirective:";
    location.printLocation(out);
    out << ">$";
#endif

    out << directive << "\n";
}


std::ostream & operator<<(std::ostream & out, const Statement & stemnt)
{
    stemnt.print(out, 0);
    return out;
}



#define  SHOW(X)  case X: return #X

const char *nameOfStatementType(StatementType type)
{
    switch (type)
    {
    default:
        return "Unknown StatementType";

        SHOW(ST_NullStemnt);
        SHOW(ST_DeclStemnt);
        SHOW(ST_ExpressionStemnt);
        SHOW(ST_IfStemnt);
        SHOW(ST_SwitchStemnt);
        SHOW(ST_ForStemnt);
        SHOW(ST_WhileStemnt);
        SHOW(ST_DoWhileStemnt);
        SHOW(ST_ContinueStemnt);
        SHOW(ST_BreakStemnt);
        SHOW(ST_GotoStemnt);
        SHOW(ST_ReturnStemnt);
        SHOW(ST_Block);
        SHOW(ST_InclStemnt);
        SHOW(ST_EndInclStemnt);
    }
}


const char *nameOfLabelType(LabelType type)
{
    switch (type)
    {
    default:
        return "Unknown LabelType";

        SHOW(LT_None);          // No label - invalid.
        SHOW(LT_Default);       // default:
        SHOW(LT_Case);          // case <expr>:
        SHOW(LT_Goto);          // A named label (goto destination).
    }
}

#undef SHOW

