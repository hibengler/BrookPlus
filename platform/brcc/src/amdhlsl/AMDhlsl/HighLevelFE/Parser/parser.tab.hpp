/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     MODIFIER_KEYWORD = 258,
     SAMPLER = 259,
     CBUFFER = 260,
     STREAM = 261,
     TEXTURERXD = 262,
     KW_UNSIGNED = 263,
     KW_VOID = 264,
     KW_MAX_VTXCNT = 265,
     KW_NUM_THREADS = 266,
     KW_LDS_REL = 267,
     KW_LDS_ABS = 268,
     KW_LDS = 269,
     PERSISTENT = 270,
     GLOBAL = 271,
     KW_TRUE = 272,
     KW_FALSE = 273,
     DISCARD = 274,
     IDENT = 275,
     STRING = 276,
     CHAR = 277,
     INTEGER = 278,
     HEX = 279,
     FLOAT = 280,
     DOUBLE = 281,
     STRUCT = 282,
     VECTOR = 283,
     MATRIX = 284,
     EQ = 285,
     NE = 286,
     LE = 287,
     GE = 288,
     LSHFT = 289,
     RSHFT = 290,
     AND = 291,
     OR = 292,
     PLUSPLUS = 293,
     MINUSMINUS = 294,
     IF = 295,
     ELSE = 296,
     FOR = 297,
     WHILE = 298,
     DO = 299,
     RETURN = 300,
     BREAK = 301,
     CONTINUE = 302,
     REGISTER = 303,
     PRE_UNARY_OP = 304,
     POST_UNARY_OP = 305,
     PREC_LAST = 306,
     PREC_FIRST = 307,
     BOGUS = 308
   };
#endif
/* Tokens.  */
#define MODIFIER_KEYWORD 258
#define SAMPLER 259
#define CBUFFER 260
#define STREAM 261
#define TEXTURERXD 262
#define KW_UNSIGNED 263
#define KW_VOID 264
#define KW_MAX_VTXCNT 265
#define KW_NUM_THREADS 266
#define KW_LDS_REL 267
#define KW_LDS_ABS 268
#define KW_LDS 269
#define PERSISTENT 270
#define GLOBAL 271
#define KW_TRUE 272
#define KW_FALSE 273
#define DISCARD 274
#define IDENT 275
#define STRING 276
#define CHAR 277
#define INTEGER 278
#define HEX 279
#define FLOAT 280
#define DOUBLE 281
#define STRUCT 282
#define VECTOR 283
#define MATRIX 284
#define EQ 285
#define NE 286
#define LE 287
#define GE 288
#define LSHFT 289
#define RSHFT 290
#define AND 291
#define OR 292
#define PLUSPLUS 293
#define MINUSMINUS 294
#define IF 295
#define ELSE 296
#define FOR 297
#define WHILE 298
#define DO 299
#define RETURN 300
#define BREAK 301
#define CONTINUE 302
#define REGISTER 303
#define PRE_UNARY_OP 304
#define POST_UNARY_OP 305
#define PREC_LAST 306
#define PREC_FIRST 307
#define BOGUS 308




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE zzlval;

#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE zzlloc;


