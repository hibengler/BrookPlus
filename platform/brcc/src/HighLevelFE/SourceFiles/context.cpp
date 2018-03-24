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
    o+     File:         context.cpp
    o+
    o+     Programmer:   Patrick Baudin
    o+     Date:         Oct 30, 2000
    o+
    o+     Manage parsing context for SymEntry insertion.
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

#include <iostream>

#include "config.h"

#include "logger.hpp"
#include "janitorial.hpp"
#include "parseenv.h"
#include "context.h"
#include "symbol.h"
#include "project.h"
#include "decl.h"
#include "stemnt.h"

//#define STATISTICS

#define PARSE_CTXT_TBL_SIZE (16)

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */

EXTERN int yyerr ARGS((char *s, std::string & str));
EXTERN int yyerr ARGS((char *s));
EXTERN void yywarn ARGS((char *s));
EXTERN int err_top_level;

// Quick wrappers for the yyerr/yywarn functions to compensate for
// gcc (correctly) interpreting strings as "const char[]" but the
// other tools/platforms not being quite as up-to-date

int cyyerr(const char *s, std::string &str)
{
  return yyerr((char *)s, str);
}

int cyyerr(const char *s)
{
  return yyerr((char *)s);
}

void cyywarn(const char *s)
{
  yywarn((char *)s);
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void yyCheckLabelsDefinition(SymTbl * labels)
{
    LOG(LOG_FN_ENTRY, "yyCheckLabelsDefinition(%p)\n", labels);
    if (labels->clevel == FUNCTION_SCOPE && labels->current)
    {
        HashTbl *htab = labels->current->htab;

        if (htab)
            for (int j = 0; j < htab->tsize; j++)
                for (SymEntry * curr = htab->tab[j]; curr; curr = curr->next)
                {
                    assert(curr->IsLabelDecl());
                    if (!curr->u2LabelPosition)
                        cyyerr("undefined label - ", curr->name);
                }
    }
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
ParseCtxt::ParseCtxt(void):size(PARSE_CTXT_TBL_SIZE), isFieldId(false)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::ParseCtxt()\n");
    tabCtxt = new ParseEnvCtxt[size];
    curCtxt = tabCtxt;
    ResetDeclCtxt();
    curCtxt->varParam = 0;
    curCtxt->possibleDuplication = NULL;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
ParseCtxt::~ParseCtxt(void)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::~ParseCtxt()\n");
    delete[]tabCtxt;
#ifdef STATISTICS
    std::cout << "ParseCtxt::size=" << size << std::endl;
#endif
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::SetDeclCtxt(BaseType * decl_specsCtxt)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::SetDeclCtxt(%p)\n", decl_specsCtxt);
#ifdef DECL_DEBUG
    std::cout << "SetDeclCtxt: ";
    if (curCtxt->decl_specs)
    {
        std::cout << "(Overloading previous setting!: ";
        curCtxt->decl_specs->printBase(std::cout, 0);
        std::cout << ", used: " << curCtxt->nb_decl_uses << ")";
    }
#endif

    curCtxt->decl_specs = decl_specsCtxt;
    curCtxt->nb_decl_uses = 0;

#ifdef DECL_DEBUG
    curCtxt->decl_specs->printBase(std::cout, 0);
    std::cout << "\n";
#endif
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::ResetDeclCtxt(void)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::ResetDeclCtxt()\n");
    curCtxt->decl_specs = (BaseType *) NULL;
    curCtxt->nb_decl_uses = 0;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
Type *ParseCtxt::UseDeclCtxt(void)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::UseDeclCtxt()\n");
    curCtxt->nb_decl_uses++;
    if (curCtxt->nb_decl_uses == 1)
        return curCtxt->decl_specs;
    else
        return curCtxt->decl_specs->dup();
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::PushCtxt(void)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::PushCtxt()\n");
    int varParam = curCtxt->varParam;

    curCtxt++;
    if (curCtxt == tabCtxt + size)
    {
        int old_size = size;

        size += size;
        ParseEnvCtxt *old_tabCtxt = tabCtxt;

        tabCtxt = new ParseEnvCtxt[size];
        for (int j = 0; j < old_size; j++)
            tabCtxt[j] = old_tabCtxt[j];
        delete[]old_tabCtxt;
        curCtxt = tabCtxt + old_size;
    }
    curCtxt->varParam = varParam;
    curCtxt->possibleDuplication = NULL;
    curCtxt->isKnR = false;
    ResetDeclCtxt();
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::PopCtxt(bool assert_checking)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::PopCtxt(%d)\n", assert_checking);
    assert(curCtxt != tabCtxt);

    int varParam = curCtxt->varParam;

    curCtxt--;

    //assert(!assert_checking || curCtxt->varParam == varParam);
    if (assert_checking && curCtxt->varParam != varParam)
        cyyerr("Problem with the parse context: ");
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::SetVarParam(int val, bool assert_checking, int should_be)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::SetVarParam(%d, %d, %d)\n", val, assert_checking, should_be);
    //assert(!assert_checking || should_be == curCtxt->varParam);
    if (assert_checking && curCtxt->varParam != should_be)
        cyyerr("Problem with the parse context: ");

    curCtxt->varParam = val;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::ReinitializeCtxt(void)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::ReinitializeCtxt()\n");
    curCtxt = tabCtxt;
    ResetDeclCtxt();
    isFieldId = false;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
BaseType *ParseCtxt::Mk_tag_ref(BaseTypeSpec tagType, Symbol * tagSym, SymTbl * tags)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::Mk_tag_ref(%d, %p, %p)\n", tagType, tagSym, tags);
    if (!tagSym)
        return NULL;

    BaseType *result = new BaseType(tagType);

    result->tag = tagSym;

    if (tagSym->entry)
    {
//     std::cout << "struct/union/enum_tag_ref:"
//       "There is a previous tag entry:"
//               << "(" << tagSym->entry << ")" << tagSym->name << "$";
//     tagSym->entry->scope->ShowScopeId(std::cout);
//     std::cout << " which have to be consistent" << endl;

        if (!tagSym->entry->uStructDef || tagSym->entry->uStructDef->typemask != tagType)
        {
            // ... which isn't compatible.
            cyyerr("Unconsistant tag use: ", tagSym->name);
            tagSym->entry = tags->Insert(mk_tag(tagSym->name, result));
        }
    }
    else
    {
        tagSym->entry = tags->Insert(mk_tag(tagSym->name, result));

//     std::cout << "struct/union/enum_tag_ref:"
//       "There is no tag entry:"
//               << "(" << tagSym->entry << ")" << tagSym->name  << "$";
//     tagSym->entry->scope->ShowScopeId(std::cout);
//     std::cout << " has been created" << endl;
    }

    return result;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
BaseType *ParseCtxt::Mk_tag_def(BaseTypeSpec tagType, Symbol * tagSym, SymTbl * tags)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::Mk_tag_def(%d, %p, %p)\n", tagType, tagSym, tags);
    if (!tagSym)
        return NULL;

    BaseType *result = new BaseType(tagType);

    result->tag = tagSym;

    if (tagSym->entry)
    {

//     std::cout << "struct/union/enum_tag_ref:"
//                      "There is a previous tag entry:"
//               << "(uStructDef:" << tagSym->entry->uStructDef << ")"
//               << "(uStructDef->stDefn:" << tagSym->entry->uStructDef->stDefn << ")"
//               << "(uStructDef->enDefn:" << tagSym->entry->uStructDef->enDefn << ")"
//               << "(" << tagSym->entry << ")" << tagSym->name << "$";
//     tagSym->entry->scope->ShowScopeId(std::cout);
//     std::cout << " which have to be consistent" << endl;

        if (!tagSym->entry->uStructDef || tagSym->entry->uStructDef->typemask != tagType)
        {
            // ... which isn't compatible.
            cyyerr("Unconsistant tag use: ", tagSym->name);
            tagSym->entry = tags->Insert(mk_tag(tagSym->name, result));
        }
        else if ((tagSym->entry->uStructDef->stDefn || tagSym->entry->uStructDef->enDefn)
                 && tagSym->entry->scope->level == tags->clevel)
        {
            // ... has alreay a definition at this level.
            cyyerr("struct/union/enum tag redeclared: ", tagSym->name);
            tagSym->entry = tags->Insert(mk_tag(tagSym->name, result));
        }
    }
    else
    {
        tagSym->entry = tags->Insert(mk_tag(tagSym->name, result));

//     std::cout << "struct/union/enum_tag_ref:"
//       "There is no tag entry:"
//               << "(" << tagSym->entry << ")" << tagSym->name  << "$";
//     tagSym->entry->scope->ShowScopeId(std::cout);
//     std::cout << " has been created" << endl;

    }

    return result;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
Decl *ParseCtxt::Mk_direct_declarator_reentrance(Symbol * declSym, SymTbl * syms)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::Mk_direct_declarator_reentrance(%p, %p)\n", declSym, syms);
    if (!declSym)
        return NULL;

    if (curCtxt->possibleDuplication != NULL)
    {
        cyyerr("Warning: Duplicate name: ", declSym->name);
    }

    Decl *result = new Decl(declSym);

//   std::cout << "isTypedefCtxt=" << IsTypedefDeclCtxt()
//             << ", Var/Param=" << curCtxt->varParam
//             << ", isKnR=" << curCtxt->isKnR
//             << ", isFieldId=" << isFieldId
//             << ", Declarator=" << declSym->name
//             << ", Type Entry=" << (declSym->entry ? declSym->entry->type : -1)
//             << ", Entry level=" << (declSym->entry ? declSym->entry->scope->level : -1)
//             << ", Parsing level=" << syms->clevel
//             << std::endl;

    if (curCtxt->varParam == 0)
    {
        if (IsTypedefDeclCtxt())
        {
            //assert(err_top_level || !isFieldId);
            if (!err_top_level && isFieldId)
                cyyerr("Problem with the parse context: ");
            if (declSym->entry && (declSym->entry->scope->level == syms->clevel))
                // There is a previous entry defined at the same level.
                cyyerr("Duplicate typedef name: ", declSym->name);

            declSym->entry = syms->Insert(mk_typedef(declSym->name, result));
        }
        else
        {
            if (declSym->entry && (declSym->entry->scope->level == syms->clevel))
            {
                if (!declSym->entry->IsFctDecl())
                    cyyerr("Symbol name duplicated: ", declSym->name);
                else
                    curCtxt->possibleDuplication = declSym->entry;
            }

            if (isFieldId)
                declSym->entry = syms->Insert(mk_component(declSym->name, result));
            else
                declSym->entry = syms->Insert(mk_varfctdecl(declSym->name, result));
        }
    }
    else
    {
        //assert(err_top_level || !IsTypedefDeclCtxt());
        if (!err_top_level && IsTypedefDeclCtxt())
            cyyerr("Problem with the parse context: ");

        if (curCtxt->isKnR && curCtxt->varParam == 1)
        {
            if (!(declSym->entry && (declSym->entry->scope->level == syms->clevel)))
            {
                // There is not a previous entry defined at the same level.
                cyyerr("Unknown parameter: ", declSym->name);
                declSym->entry = syms->Insert(mk_paramdecl(declSym->name, result));
            }
            else if (declSym->entry->uVarDecl->form)
            {
                //assert(declSym->entry->IsParamDecl());
                cyyerr("Duplicate type declaration: ", declSym->name);
            }
        }
        else if (declSym->entry && (declSym->entry->scope->level == syms->clevel))
        {
            // There is a previous entry defined at the same level.
            cyyerr("Duplicate parameter name: ", declSym->name);
            declSym->entry = syms->Insert(mk_paramdecl(declSym->name, result));
        }
        else
            declSym->entry = syms->Insert(mk_paramdecl(declSym->name, result));
    }

//     std::cout << "isTypedefCtxt=" << IsTypedefDeclCtxt()
//               << ", Var/Param=" << curCtxt->varParam
//               << ", isKnR=" << curCtxt->isKnR
//               << ", isFieldId=" << isFieldId
//               << ", Declarator=" << declSym->name
//               << ", Type Entry=" << (declSym->entry ? declSym->entry->type : -1)
//               << ", Entry level=" << (declSym->entry ? declSym->entry->scope->level : -1)
//               << ", Parsing level=" << syms->clevel
//               << ", Duplication=" << (curCtxt->possibleDuplication ? curCtxt->possibleDuplication->name : "NO")
//               << std::endl;

    return result;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::Mk_declarator(Decl * decl)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::Mk_declarator(%p)\n", decl);
    if (!decl)
        return;

    Symbol *ident = decl->name;

    //assert(!curCtxt->possibleDuplication || ident->entry);
    if (curCtxt->possibleDuplication && ident->entry == NULL)
        cyyerr("Problem with the parse context: ");

    if (ident && ident->entry)
    {
        if (curCtxt->possibleDuplication)
        {
            //assert(curCtxt->possibleDuplication->IsFctDecl());
            if (curCtxt->possibleDuplication->IsFctDecl() == false)
                cyyerr("Problem with the parse context: ");
            //assert(curCtxt->possibleDuplication->scope->level
            //              == gProject->Parse_TOS->transUnit->contxt.syms->clevel);

            if (!decl->form || (decl->form->type != TT_Function))
                cyyerr("Duplicate function name: ", ident->name);
            /*
               else
               cyywarn ("TO DO: checking prototype consistency and eventually delete the new fct symbol");
             */

            curCtxt->possibleDuplication = NULL;
        }

        if (ident->entry->type == VarFctDeclEntry)
        {
            if (decl->form && (decl->form->type == TT_Function))
                ident->entry->type = FctDeclEntry;
            else
                ident->entry->type = VarDeclEntry;
        }
    }
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
void ParseCtxt::Mk_func_declarator(Decl * decl)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::Mk_func_declarator(%p)\n", decl);
    if (!decl)
        return;

    Symbol *ident = decl->name;

    //assert(!curCtxt->possibleDuplication || ident->entry);
    //assert(!err_top_level || decl->form->type == TT_Function);
    if ((curCtxt->possibleDuplication && !ident->entry) ||
        (err_top_level && decl->form->type != TT_Function))
    {
        cyyerr("Problem with the parse context: ");
    }
    

    if (ident && ident->entry)
    {
        ident->entry->type = FctDeclEntry;
        if (curCtxt->possibleDuplication)
        {
            assert(curCtxt->possibleDuplication->IsFctDecl());
            assert(curCtxt->possibleDuplication->scope->level == gProject->Parse_TOS->transUnit->contxt.syms->clevel);

            if (curCtxt->possibleDuplication->u2FunctionDef)
                cyyerr("Duplicate function name: ", ident->name);
            /*
               else
               cyywarn("TO DO: checking prototype consistency and eventually delete the new fct symbol");
             */

            curCtxt->possibleDuplication = NULL;
        }
    }
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
Label *ParseCtxt::Mk_named_label(Symbol * labelSym, SymTbl * labels)
{
    LOG(LOG_FN_ENTRY, "ParseCtxt::Mk_named_label(%p, %p)\n", labelSym, labels);
    if (!labelSym)
        return NULL;

    labelSym->entry = labels->LookupAt(labelSym->name, FUNCTION_SCOPE);

    Label *result = new Label(labelSym);

    if (labelSym->entry)
    {
        if (labelSym->entry->u2LabelPosition)
        {
            cyyerr("Duplicate label: ", labelSym->name);
            labelSym->entry = mk_label(labelSym->name, result);
            labels->InsertAt(labelSym->entry, FUNCTION_SCOPE);
        }
    }
    else
    {
        labelSym->entry = mk_label(labelSym->name, result);
        labels->InsertAt(labelSym->entry, FUNCTION_SCOPE);
    }

    result->syment = labelSym->entry;
    return result;
}

/* o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o */
