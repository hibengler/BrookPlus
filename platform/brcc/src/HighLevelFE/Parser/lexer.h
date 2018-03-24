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

#ifndef LEXER_H
#define LEXER_H

#include <cstdio>

#include "config.h"
#include "decl.h"
#include "express.h"

/**************************************************************************/

class Label;
class Statement;
class DeclStemnt;
class TransUnit;

/**************************************************************************/

/* Maximum length for strings and identifiers */
#define MAX_STRING_LEN      2048

/* If we allow a comment as a token, we need to let them be larger. */
#define MAX_TOKN_LEN        2048

/**************************************************************************/

typedef struct
{
    Location loc;       // This is partly redundant, as some of the other types
                        // contain a loc field, but not all.
    union
    {
        BinaryOp binOp;
        RelOp relOp;
        AssignOp assignOp;

        Symbol *symbol;

        BaseTypeSpec typeSpec;
        TypeQual typeQual;
        StorageType storage;
        GccAttrib *gccAttrib;

        Type *type;
        PtrType *ptr;
        FunctionType *fct;
        BaseType *base;
        Decl *decl;
        StructDef *strDef;
        EnumDef *enDef;

        Expression *value;
        Constant *consValue;
        EnumConstant *enConst;
        ArrayConstant *arrayConst;

        Label *label;
        Statement *stemnt;

        DeclStemnt *declStemnt;
        FunctionDef *functionDef;

        TransUnit *transunit;

        INumList *inumlist;
    };
} lex_union;

extern Location currentLocation;

/******************************************************/

/*    For Flex compatibility  */

#undef  YYSTYPE
#define YYSTYPE lex_union

/******************************************************/

int get_lineno(bool file_ppln, char *txt, Statement ** stement);
void yywarn(char *s);
int yyerror(char *s);
int yyerr(char *s);
int yyerr(char *s, std::string & str);

/******************************************************/

// Indicates whether the identifier being parsed
// could possibly be a type.
extern bool possibleType;

/******************************************************/

#endif /* LEXER_H */
