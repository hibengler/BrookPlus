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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse zzparse
#define yylex   zzlex
#define yyerror zzerror
#define yylval  zzlval
#define yychar  zzchar
#define yydebug zzdebug
#define yynerrs zznerrs
#define yylloc zzlloc

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




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

/* ============================================================
Copyright (c) 2007 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the 
following conditions:
 
Redistributions must retain the above copyright notice and all terms of 
this license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE 
REVOCATION OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERATION, OR THAT IT IS 
FREE FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - 
WHETHER EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, 
ANY IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE, ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR 
NON-INFRINGEMENT. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY 
COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, PUNITIVE, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
REVENUE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED 
ON ANY THEORY OF LIABILITY ARISING IN ANY WAY RELATED TO THIS MATERIAL, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE 
LIABILITY OF ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT HOLDERS AND 
CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS (US $10.00). ANYONE REDISTRIBUTING 
OR ACCESSING OR USING THIS MATERIAL ACCEPTS THIS ALLOCATION OF RISK AND AGREES 
TO RELEASE ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT HOLDERS AND 
CONTRIBUTORS FROM ANY AND ALL LIABILITIES, OBLIGATIONS, CLAIMS, OR DEMANDS IN 
EXCESS OF TEN DOLLARS (US $10.00). THE FOREGOING ARE ESSENTIAL TERMS OF THIS 
LICENSE AND, IF ANY OF THESE TERMS ARE CONSTRUED AS UNENFORCEABLE, FAIL IN 
ESSENTIAL PURPOSE, OR BECOME VOID OR DETRIMENTAL TO ADVANCED MICRO DEVICES, 
INC. OR ANY COPYRIGHT HOLDERS OR CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS 
TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL SHALL TERMINATE IMMEDIATELY. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF THIS 
LICENSE OR ANY AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS 
SUCH AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS 
AS DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS,
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is provided 
with “RESTRICTED RIGHTS” and/or “LIMITED RIGHTS” as applicable to computer 
software and technical data, respectively. Use, duplication, distribution or 
disclosure by the U.S. Government and/or DOD agencies is subject to the full 
extent of restrictions in all applicable regulations, including those found at 
FAR52.227 and DFARS252.227 et seq. and any successor regulations thereof. Use 
of this material by the U.S. Government and/or DOD agencies is acknowledgment 
of the proprietary rights of any copyright holders and contributors, including 
those of Advanced Micro Devices, Inc., as well as the provisions of FAR52.227-14 
through 23 regarding privately developed and/or commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, 
title, or interest in, or relating to, this material. No terms of this license 
can be modified or waived, and no breach of this license can be excused, unless 
done so in a writing signed by all affected parties. Each term of this license 
is separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.
============================================================ */
#include <stdio.h>
#include <string.h>
#include "parser.hpp"
#include "astroot.hpp"
#include "astdeclaration.hpp"
#include "astliterals.hpp"
#include "astuserdatatype.hpp"
#include "astcontrol.hpp"
#include "astoperators.hpp"
#include "astdelayedlookup.hpp"
#include "assertlogging.hpp"

#define EMPTY (ParserVariant::EmptyList());
#define yylex zzlex

int zzlex();

bool theParserHadError = false;


void yyerror(const char *message)
{
    theParserHadError = true;
	NOTICE("Parse error")(message);
}

struct LdsInfo 
 {
   LdsInfo() { reset(); }
   void reset() {
     memset( this, 0, sizeof(LdsInfo) );
   }
   int lds_size;
   int lds_rel;
   int lds_abs;
   int numT[3];
 };
static LdsInfo gLdsInfo;




/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 1
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 352 "parser.tab.cpp"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYLTYPE_IS_TRIVIAL) && YYLTYPE_IS_TRIVIAL \
             && defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2420

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  56
/* YYNRULES -- Number of rules. */
#define YYNRULES  187
/* YYNRULES -- Number of states. */
#define YYNSTATES  376

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    68,     2,     2,     2,    67,    60,     2,
      73,    74,    65,    63,    54,    64,    70,    66,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    57,    75,
      61,    55,    62,    56,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,    58,    77,    69,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     7,     8,    11,    14,    16,    18,
      20,    22,    24,    26,    28,    29,    31,    33,    37,    41,
      45,    49,    53,    57,    61,    65,    69,    73,    77,    81,
      85,    89,    93,    97,   101,   105,   109,   113,   118,   123,
     128,   133,   138,   143,   148,   153,   158,   163,   166,   169,
     172,   175,   178,   181,   184,   187,   189,   191,   193,   195,
     197,   199,   201,   203,   205,   209,   211,   213,   215,   217,
     223,   228,   230,   232,   237,   241,   246,   253,   255,   258,
     259,   261,   263,   265,   269,   270,   274,   277,   279,   281,
     283,   285,   291,   299,   301,   302,   304,   306,   316,   322,
     330,   331,   335,   338,   341,   344,   347,   350,   353,   356,
     359,   361,   365,   367,   371,   376,   378,   380,   385,   392,
     401,   409,   414,   417,   423,   425,   428,   433,   435,   442,
     444,   453,   455,   460,   463,   465,   467,   471,   474,   476,
     480,   483,   486,   488,   491,   494,   497,   499,   502,   503,
     506,   511,   512,   514,   516,   520,   523,   525,   529,   532,
     536,   537,   543,   549,   556,   560,   563,   567,   569,   572,
     578,   583,   586,   592,   600,   609,   610,   612,   619,   625,
     629,   635,   639,   640,   647,   651,   653,   656
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
      79,     0,    -1,    81,    -1,    75,    -1,    -1,    81,    82,
      -1,   115,    80,    -1,   125,    -1,   109,    -1,   124,    -1,
     126,    -1,   131,    -1,    80,    -1,     1,    -1,    -1,    84,
      -1,    88,    -1,    83,    54,    88,    -1,    88,    55,    88,
      -1,    88,    60,    88,    -1,    88,    58,    88,    -1,    88,
      59,    88,    -1,    88,    65,    88,    -1,    88,    66,    88,
      -1,    88,    67,    88,    -1,    88,    63,    88,    -1,    88,
      64,    88,    -1,    88,    34,    88,    -1,    88,    35,    88,
      -1,    88,    61,    88,    -1,    88,    62,    88,    -1,    88,
      32,    88,    -1,    88,    33,    88,    -1,    88,    30,    88,
      -1,    88,    31,    88,    -1,    88,    36,    88,    -1,    88,
      37,    88,    -1,    88,    60,    55,    88,    -1,    88,    58,
      55,    88,    -1,    88,    59,    55,    88,    -1,    88,    65,
      55,    88,    -1,    88,    66,    55,    88,    -1,    88,    67,
      55,    88,    -1,    88,    63,    55,    88,    -1,    88,    64,
      55,    88,    -1,    88,    34,    55,    88,    -1,    88,    35,
      55,    88,    -1,    68,    88,    -1,    69,    88,    -1,    64,
      88,    -1,    63,    88,    -1,    38,    88,    -1,    39,    88,
      -1,    88,    38,    -1,    88,    39,    -1,    17,    -1,    18,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    89,
      -1,    21,    -1,    22,    -1,    73,    88,    74,    -1,    90,
      -1,    87,    -1,    85,    -1,    86,    -1,    88,    56,    88,
      57,    88,    -1,    73,    88,    74,    88,    -1,     1,    -1,
      20,    -1,    89,    73,    83,    74,    -1,    87,    70,    20,
      -1,    87,    71,    88,    72,    -1,    87,    70,    20,    73,
      83,    74,    -1,   117,    -1,     4,   127,    -1,    -1,     9,
      -1,    93,    -1,    91,    -1,    92,    54,    91,    -1,    -1,
      94,   117,    80,    -1,    45,    88,    -1,    45,    -1,    19,
      -1,    46,    -1,    47,    -1,    40,    73,    88,    74,   104,
      -1,    40,    73,    88,    74,   104,    41,   104,    -1,    88,
      -1,    -1,    83,    -1,   117,    -1,    42,    73,   101,    80,
     100,    80,    83,    74,   104,    -1,    43,    73,    88,    74,
     104,    -1,    44,   104,    43,    73,    88,    74,    80,    -1,
      -1,   103,   115,    80,    -1,   103,   104,    -1,    88,    80,
      -1,    95,    80,    -1,    96,    80,    -1,    97,    80,    -1,
      98,    80,    -1,   132,    99,    -1,   132,   102,    -1,    80,
      -1,    76,   103,    77,    -1,   106,    -1,   106,    54,   105,
      -1,    14,    73,    88,    74,    -1,    13,    -1,    12,    -1,
      11,    73,    88,    74,    -1,    11,    73,    88,    54,    88,
      74,    -1,    11,    73,    88,    54,    88,    54,    88,    74,
      -1,    71,    10,    73,    88,    74,    72,   107,    -1,    71,
     105,    72,   107,    -1,   133,   107,    -1,   110,   111,    73,
      92,    74,    -1,   107,    -1,   108,   133,    -1,   108,    76,
     103,    77,    -1,    20,    -1,    28,    61,    20,    54,    23,
      62,    -1,    28,    -1,    29,    61,    20,    54,    23,    54,
      23,    62,    -1,    29,    -1,     6,    61,    20,    62,    -1,
       8,    20,    -1,     9,    -1,    20,    -1,    20,   122,   118,
      -1,    20,   118,    -1,   112,    -1,   113,    54,   112,    -1,
     114,   133,    -1,   110,   113,    -1,   114,    -1,   133,   115,
      -1,   116,   133,    -1,   110,   112,    -1,   116,    -1,   133,
     117,    -1,    -1,    55,    88,    -1,    55,    76,   119,    77,
      -1,    -1,   120,    -1,   121,    -1,   120,    54,   121,    -1,
     120,    54,    -1,    88,    -1,    76,   119,    77,    -1,    71,
      72,    -1,    71,    88,    72,    -1,    -1,    57,    48,    73,
      20,    74,    -1,    27,    20,    76,    94,    77,    -1,     7,
      61,    20,    62,   112,    80,    -1,     7,   112,    80,    -1,
     129,    77,    -1,     4,   127,    80,    -1,    20,    -1,     4,
     127,    -1,     4,   127,    71,    88,    72,    -1,     4,   127,
      55,     4,    -1,   128,    76,    -1,   129,    20,    55,    88,
      75,    -1,   129,    20,    55,    61,    89,    62,    75,    -1,
     129,    20,    71,    88,    72,    55,    88,    75,    -1,    -1,
      20,    -1,     5,    20,   123,    76,    81,    77,    -1,    15,
     130,    76,    81,    77,    -1,    15,   117,    80,    -1,    16,
     130,    76,    81,    77,    -1,    16,   117,    80,    -1,    -1,
      71,    20,    73,    88,    74,    72,    -1,    71,    20,    72,
      -1,     3,    -1,    57,    20,    -1,    57,    48,    73,    20,
      74,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   169,   169,   183,   187,   188,   191,   192,   193,   194,
     195,   196,   197,   198,   202,   203,   206,   207,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   245,   248,   251,
     254,   257,   260,   263,   268,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   290,   291,   292,   293,
     300,   301,   304,   306,   307,   308,   309,   321,   322,   325,
     326,   327,   330,   331,   334,   335,   338,   339,   344,   345,
     346,   348,   353,   358,   359,   362,   363,   366,   371,   376,
     381,   382,   383,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   398,   399,   402,   406,   410,   414,   420,   426,
     434,   439,   451,   456,   462,   463,   469,   475,   476,   477,
     478,   479,   480,   481,   482,   485,   488,   494,   500,   501,
     503,   504,   510,   511,   515,   516,   517,   518,   521,   522,
     523,   525,   526,   528,   529,   531,   533,   534,   537,   538,
     540,   541,   543,   550,   551,   559,   560,   565,   568,   569,
     571,   575,   576,   577,   578,   581,   582,   584,   590,   596,
     602,   607,   613,   614,   615,   618,   619,   620
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MODIFIER_KEYWORD", "SAMPLER", "CBUFFER",
  "STREAM", "TEXTURERXD", "KW_UNSIGNED", "KW_VOID", "KW_MAX_VTXCNT",
  "KW_NUM_THREADS", "KW_LDS_REL", "KW_LDS_ABS", "KW_LDS", "PERSISTENT",
  "GLOBAL", "KW_TRUE", "KW_FALSE", "DISCARD", "IDENT", "STRING", "CHAR",
  "INTEGER", "HEX", "FLOAT", "DOUBLE", "STRUCT", "VECTOR", "MATRIX", "EQ",
  "NE", "LE", "GE", "LSHFT", "RSHFT", "AND", "OR", "PLUSPLUS",
  "MINUSMINUS", "IF", "ELSE", "FOR", "WHILE", "DO", "RETURN", "BREAK",
  "CONTINUE", "REGISTER", "PRE_UNARY_OP", "POST_UNARY_OP", "PREC_LAST",
  "PREC_FIRST", "BOGUS", "','", "'='", "'?'", "':'", "'|'", "'^'", "'&'",
  "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'.'",
  "'['", "']'", "'('", "')'", "';'", "'{'", "'}'", "$accept", "root",
  "semicol", "declaration_list", "declaration", "arg_list",
  "arg_list_nonempty", "binary_expr", "unary_expr", "snglexpr", "expr",
  "dlIDENT", "func_call", "param_list_item", "param_list",
  "param_list_nonempty", "variable_declaration_list", "return_statement",
  "discard_statement", "break_statement", "continue_statement",
  "if_statement", "expr_or_true", "expr_or_vdcl", "loop_statement",
  "statement_list", "loopbody", "lds_info_list", "lds_info",
  "func_decl_hdr2", "func_decl_hdr", "function_declaration", "data_type",
  "function_name", "variable_name", "variable_name_list",
  "multi_var_decl_root", "multi_var_decl", "var_decl_root", "var_decl",
  "initializer", "initalizer_list", "initalizer_list_ne",
  "initalizer_list_item", "array_size", "optional_register_spec",
  "struct_declaration", "texture_declaration", "sampler_declaration",
  "sampler_name", "sampler_dcl_prefix", "sampler_dcl_body",
  "optional_ident", "specialalloc_declaration", "cmplr_opt", "modifier", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,    44,    61,    63,    58,   124,    94,
      38,    60,    62,    43,    45,    42,    47,    37,    33,   126,
      46,    91,    93,    40,    41,    59,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    78,    79,    80,    81,    81,    82,    82,    82,    82,
      82,    82,    82,    82,    83,    83,    84,    84,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    86,    86,    86,
      86,    86,    86,    86,    86,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    88,    88,    88,    88,
      88,    88,    89,    90,    90,    90,    90,    91,    91,    92,
      92,    92,    93,    93,    94,    94,    95,    95,    96,    97,
      98,    99,    99,   100,   100,   101,   101,   102,   102,   102,
     103,   103,   103,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   105,   105,   106,   106,   106,   106,   106,   106,
     107,   107,   107,   107,   108,   108,   109,   110,   110,   110,
     110,   110,   110,   110,   110,   111,   112,   112,   113,   113,
     114,   114,   115,   115,   116,   116,   117,   117,   118,   118,
     118,   119,   119,   120,   120,   120,   121,   121,   122,   122,
     123,   123,   124,   125,   125,   126,   126,   127,   128,   128,
     128,   129,   129,   129,   129,   130,   130,   131,   131,   131,
     131,   131,   132,   132,   132,   133,   133,   133
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     0,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     2,     2,     2,
       2,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     5,
       4,     1,     1,     4,     3,     4,     6,     1,     2,     0,
       1,     1,     1,     3,     0,     3,     2,     1,     1,     1,
       1,     5,     7,     1,     0,     1,     1,     9,     5,     7,
       0,     3,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     3,     1,     3,     4,     1,     1,     4,     6,     8,
       7,     4,     2,     5,     1,     2,     4,     1,     6,     1,
       8,     1,     4,     2,     1,     1,     3,     2,     1,     3,
       2,     2,     1,     2,     2,     2,     1,     2,     0,     2,
       4,     0,     1,     1,     3,     2,     1,     3,     2,     3,
       0,     5,     5,     6,     3,     2,     3,     1,     2,     5,
       4,     2,     5,     7,     8,     0,     1,     6,     5,     3,
       5,     3,     0,     6,     3,     1,     2,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       4,     0,     0,     1,    13,   185,     0,     0,     0,     0,
       0,   134,   175,   175,   127,     0,   129,   131,     0,     0,
       3,    12,     5,   124,     0,     8,     0,   142,     0,     9,
       7,    10,     0,     0,    11,     0,   167,   168,   160,     0,
     148,     0,     0,   133,   127,     0,   146,     0,     0,     0,
       0,     0,     0,     0,     0,   186,     0,     0,     0,   116,
     115,     0,     0,   112,   100,   125,   148,     0,   138,   141,
     140,     6,   171,     0,   165,   122,   143,     0,     0,   166,
       0,     0,     0,     0,     0,   137,   148,     0,   164,   145,
     144,   179,     4,   147,   181,     4,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,     0,
     170,    71,    55,    56,    72,    62,    63,    57,    58,    59,
      60,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      66,     0,    61,    65,     0,     4,   132,     0,   149,   158,
       0,   136,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   121,     0,     0,   113,    88,    72,     0,    89,
      90,     0,   100,   126,   110,     0,     0,     0,     0,     0,
     102,     0,     0,     0,     0,     0,    80,    82,     0,    81,
      77,   139,     0,     0,     0,    51,    52,    50,    49,    47,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   169,     0,     0,     0,
       0,   156,     0,   152,   153,   159,     0,   178,   180,   162,
       0,     0,     0,   187,     0,     0,   117,   114,   135,    86,
       0,     0,   103,   104,   105,   106,   107,   101,     0,     0,
       0,     0,   108,   109,    78,     0,   123,     0,   172,     0,
       0,    74,     0,    33,    34,    31,    32,     0,    27,     0,
      28,    35,    36,    18,     0,     0,    20,     0,    21,     0,
      19,    29,    30,     0,    25,     0,    26,     0,    22,     0,
      23,     0,    24,     0,    15,    16,     0,   177,     0,   150,
       0,   163,    85,   128,     0,     0,     0,   184,     0,   111,
       0,     0,     0,     0,    83,     0,     0,    70,     0,    75,
      45,    46,     0,    38,    39,    37,    43,    44,    40,    41,
      42,     0,    73,   161,   157,   154,     0,   120,     0,   118,
       0,     0,    95,     0,    96,     0,     0,   173,     0,     0,
      69,    17,   130,     0,     0,     0,     0,     0,     0,   174,
      76,   119,   183,    91,    93,     0,    98,     0,     0,     0,
       0,    92,     0,    99,     0,    97
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,   164,     2,    22,   293,   294,   128,   129,   130,
     165,   132,   133,   177,   178,   179,   145,   166,   167,   168,
     169,   252,   365,   343,   253,   105,   170,    62,    63,    23,
      24,    25,    45,    67,    68,    69,    27,    28,    46,   180,
      85,   222,   223,   224,    86,    81,    29,    30,    31,    37,
      32,    33,    48,    34,   173,    49
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -302
static const short int yypact[] =
{
    -302,    24,   786,  -302,  -302,  -302,     7,    29,   -10,     3,
      42,  -302,    67,    67,  -302,    66,    28,    44,     2,   169,
    -302,  -302,  -302,  -302,     6,  -302,    95,     8,    41,  -302,
    -302,  -302,    53,    -8,  -302,  1089,  -302,   -42,    70,   110,
     -25,   112,    41,  -302,    60,   117,     8,    41,    64,  1173,
      41,    73,    74,   123,   132,  -302,    81,    83,    84,  -302,
    -302,    87,    89,   108,  -302,  -302,    80,   101,  -302,   122,
    -302,  -302,  -302,   -18,  -302,  -302,  -302,   173,  1440,  -302,
     138,   118,   133,   358,   961,  -302,   141,   135,  -302,  -302,
    -302,  -302,  -302,  -302,  -302,  -302,  -302,   144,   145,   180,
    1440,  1440,  1440,  1089,    86,   529,   251,   117,   993,  1440,
    -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,
    -302,  1440,  1440,  1440,  1440,  1440,  1440,  1440,  -302,  -302,
     -51,  2080,   128,  -302,   131,  -302,  -302,   667,  2209,  -302,
    2095,  -302,   117,    51,   287,    82,   179,   184,   136,  1720,
    1630,  1765,  -302,   191,  1089,  -302,  -302,   192,   875,  -302,
    -302,   194,  -302,  -302,  -302,  1492,    41,    41,    41,    41,
    -302,   117,    41,   129,  1173,     7,   195,  -302,   -40,  -302,
    -302,  -302,   196,  1538,  2138,    -7,    -7,    -7,    -7,    -7,
      -7,  1810,   198,  1440,  1440,  1440,  1440,  1440,  1020,  1047,
    1440,  1440,  -302,  -302,  1440,  1440,  1104,  1131,  1188,  1440,
    1440,  1215,  1272,  1299,  1356,  1383,  -302,   934,   199,   693,
     667,  2209,   152,   168,  -302,  -302,    41,  -302,  -302,  -302,
      41,   170,   183,  -302,   166,  1440,  -302,  -302,  -302,  2209,
      11,   606,  -302,  -302,  -302,  -302,  -302,  -302,   185,   188,
     190,   736,  -302,  -302,  -302,  1464,  -302,   177,  -302,   186,
     454,   193,  2153,  1242,  1242,   385,   385,  1440,    54,  1440,
      54,  2295,  2257,  2209,  2196,  1440,  2305,  1440,  2343,  1440,
    2353,   385,   385,  1440,   -23,  1440,   -23,  1440,    -7,  1440,
      -7,  1440,    -7,    20,  -302,  2209,   182,  -302,   176,  -302,
     167,  -302,  -302,  -302,   241,  1089,  1675,  -302,  1440,  -302,
    1440,   816,  1440,   222,  -302,   197,  1440,  -302,   934,  -302,
    2209,  2209,  1440,  2209,  2209,  2209,  2209,  2209,  2209,  2209,
    2209,  1440,  -302,  -302,  -302,  -302,   205,  -302,  1440,  -302,
    1855,  1900,   214,    41,  -302,  1945,   200,  -302,  1584,    27,
    2247,  2209,  -302,  1990,   203,   736,   902,   736,  1440,  -302,
    -302,  -302,  -302,   228,  2209,    41,  -302,  2035,   736,   934,
      41,  -302,    59,  -302,   736,  -302
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -302,  -302,    -2,   -74,  -302,  -301,  -302,  -302,  -302,  -302,
     125,    88,  -302,    21,  -302,  -302,  -302,  -302,  -302,  -302,
    -302,  -302,  -302,  -302,  -302,   115,  -243,   174,  -302,   -31,
    -302,  -302,     4,  -302,    -4,  -302,  -302,   -28,  -302,   -11,
     201,    61,  -302,   -16,  -302,  -302,  -302,  -302,  -302,   107,
    -302,  -302,   272,  -302,  -302,     1
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -183
static const short int yytable[] =
{
      21,    47,    50,    35,    75,    42,    26,    76,   313,     5,
     342,     5,    73,    77,   255,   202,   203,   349,   143,   192,
     193,   144,    55,    40,     3,    65,    71,    36,    70,    78,
      83,   202,   203,    20,   256,    79,    35,   108,    93,    26,
      88,    89,   213,   214,   215,    91,    84,    90,    94,    38,
      56,    39,     4,   109,     5,     6,     7,     8,     9,    10,
      11,   219,    43,    18,    41,    18,    12,    13,   372,    74,
       5,    14,   152,     8,   331,    10,    11,   172,    15,    16,
      17,   331,    64,   307,   308,     5,    52,    44,     8,    53,
      10,    11,   202,   203,   332,    16,    17,    58,    59,    60,
      61,   360,    14,   181,   154,    54,   174,   153,    18,   171,
      16,    17,   363,   331,   366,    66,    20,   211,   212,   213,
     214,   215,    19,    75,    18,   371,    20,    80,   227,    72,
      82,   375,    87,   374,   230,    83,  -176,    40,   226,    18,
      92,    21,    21,    97,    35,    35,    76,    26,    26,    95,
      96,    84,    98,  -135,    99,   154,   100,   101,   153,   229,
     102,   103,   104,   242,   243,   244,   245,   246,   111,   248,
     247,   249,   250,   251,   106,   174,   107,   110,   171,    57,
      58,    59,    60,    61,   112,   113,   134,   114,   115,   116,
     117,   118,   119,   120,   135,   136,    83,   142,   146,   147,
     148,   217,   231,   131,   218,   121,   122,   232,   138,   140,
     233,   238,  -127,   172,   240,  -134,   114,    21,   261,   296,
      35,  -155,   300,    26,   301,   149,   150,   151,   302,   299,
     123,   124,   303,   183,   184,   125,   126,   304,   305,   315,
     127,   316,   174,   220,  -155,   171,   185,   186,   187,   188,
     189,   190,   191,   334,     5,   175,   333,     8,   310,    10,
     176,   311,   221,   312,   336,   346,   318,   352,   331,   368,
     257,    14,   347,   358,   337,   362,   314,   241,   155,    16,
      17,   298,   254,   239,   335,    51,     0,   141,     4,     0,
       5,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     344,     0,    12,    13,     0,     0,   154,    14,    18,   153,
       0,     0,     0,     0,    15,    16,    17,     0,   262,   263,
     264,   265,   266,   268,   270,   271,   272,     0,     0,   273,
     274,   276,   278,   280,   281,   282,   284,   286,   288,   290,
     292,   356,   295,     0,    18,   221,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    19,   111,
     306,     0,    20,   369,   228,     0,     0,     0,   373,     0,
       0,     0,     0,     0,     0,   112,   113,     0,   114,   115,
     116,   117,   118,   119,   120,   317,     0,     0,     0,     0,
       0,     0,   320,     0,   321,     0,   121,   122,     0,     0,
     323,     0,   324,     0,   325,     0,     0,     0,   326,     0,
     327,     0,   328,     0,   329,     0,   330,     0,     0,   198,
     199,   123,   124,   202,   203,   221,   125,   126,     0,     0,
       0,   127,     0,   340,   137,   341,   295,   345,     0,     0,
       0,   348,     0,   295,     0,     0,     0,   350,   211,   212,
     213,   214,   215,     0,     0,   111,   351,   -64,     0,     0,
       0,     0,     0,   353,     0,     0,     0,     0,     0,     0,
       0,   112,   113,     0,   114,   115,   116,   117,   118,   119,
     120,   364,     0,   367,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   -64,   -64,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -64,   -64,
     -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,   -64,
     -64,   -64,   125,   126,   -64,   -64,   -64,   127,   -64,   -64,
     111,   -64,     5,     0,     0,     8,     0,    10,    11,     0,
       0,     0,     0,     0,     0,     0,   112,   113,   156,   157,
     115,   116,   117,   118,   119,   120,     0,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,   121,   122,  -182,
       0,  -182,  -182,  -182,   158,   159,   160,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
       0,     0,   123,   124,     0,     0,     0,   125,   126,     0,
     161,     0,   127,     0,    20,   162,   163,   111,     0,     5,
       0,     0,     8,     0,    10,    11,     0,     0,     0,     0,
       0,     0,     0,   112,   113,   156,   157,   115,   116,   117,
     118,   119,   120,     0,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,  -182,     0,  -182,  -182,
    -182,   158,   159,   160,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    18,     0,     0,     0,     0,   111,   123,
     124,     0,     0,     0,   125,   126,     0,   161,     0,   127,
       0,    20,   162,   309,   112,   113,     0,   114,   115,   116,
     117,   118,   119,   120,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,     0,   121,   122,     0,    12,    13,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
     123,   124,     0,     0,     0,   125,   126,   111,     0,     0,
     127,     0,     0,   220,  -151,     0,     0,     0,     0,     0,
      18,     0,     0,   112,   113,   156,   114,   115,   116,   117,
     118,   119,   120,     0,    19,     0,     0,     0,    20,     0,
     297,     0,     0,     0,   121,   122,  -182,     0,  -182,  -182,
    -182,   158,   159,   160,     0,     0,    -2,     4,     0,     5,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   123,
     124,    12,    13,     0,   125,   126,    14,   161,     0,   127,
       0,    20,   162,    15,    16,    17,     0,   111,     0,     5,
       0,     0,     8,     0,    10,    11,     0,     0,     0,     0,
       0,     0,     0,   112,   113,     0,   157,   115,   116,   117,
     118,   119,   120,    18,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,    19,     0,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
     -14,     0,     0,    18,     0,     0,   111,     0,     0,   123,
     124,     0,     0,     0,   125,   126,     0,     0,     0,   127,
       0,   -14,   112,   113,     0,   114,   115,   116,   117,   118,
     119,   120,     0,   111,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   121,   122,     0,     0,     0,     0,   112,
     113,     0,   114,   115,   116,   117,   118,   119,   120,     0,
       0,     0,     0,     0,     0,   111,     0,     0,   123,   124,
     121,   122,     0,   125,   126,     0,     0,     0,   127,     0,
     -87,   112,   113,     0,   114,   115,   116,   117,   118,   119,
     120,     0,   111,     0,     0,   123,   124,     0,     0,     0,
     125,   126,   121,   122,     0,   127,     0,   -94,   112,   113,
       0,   114,   115,   116,   117,   118,   119,   120,   -14,     0,
       0,     0,     0,     0,   111,     0,     0,   123,   124,   121,
     122,     0,   125,   126,     0,     0,     0,   127,   -14,     0,
     112,   113,     0,   114,   115,   116,   117,   118,   119,   120,
       0,   111,     0,     0,   123,   124,     0,     0,     0,   125,
     126,   121,   122,   139,   127,     0,     0,   112,   113,     0,
     114,   115,   116,   117,   118,   119,   120,     0,   111,     0,
       0,     0,     0,     0,   182,     0,   123,   124,   121,   122,
       0,   125,   126,     0,   112,   113,   127,   114,   115,   116,
     117,   118,   119,   120,     0,   267,     0,     0,     0,     0,
       0,     0,     0,   123,   124,   121,   122,     0,   125,   126,
       0,     0,     5,   127,     0,     8,     0,    10,    11,     0,
       0,     0,   269,     0,     0,   111,     0,     0,     0,    14,
     123,   124,     0,     0,     0,   125,   126,    16,    17,     0,
     127,   112,   113,     0,   114,   115,   116,   117,   118,   119,
     120,     0,   111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   121,   122,     0,     0,    18,     0,   112,   113,
       0,   114,   115,   116,   117,   118,   119,   120,     0,   275,
      19,     0,     0,     0,     0,     0,     0,   123,   124,   121,
     122,     0,   125,   126,     0,     0,     5,   127,     0,     8,
       0,    10,    11,     0,     0,     0,   277,     0,     0,   111,
       0,     0,     0,    14,   123,   124,     0,     0,     0,   125,
     126,    16,    17,     0,   127,   112,   113,     0,   114,   115,
     116,   117,   118,   119,   120,     0,   111,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   121,   122,     0,     0,
      18,     0,   112,   113,     0,   114,   115,   116,   117,   118,
     119,   120,     0,   279,     0,     0,     0,     0,     0,     0,
       0,   123,   124,   121,   122,     0,   125,   126,     0,     0,
       0,   127,     0,     0,     0,     0,     0,     0,     0,     0,
     283,     0,     0,   111,   196,   197,   198,   199,   123,   124,
     202,   203,     0,   125,   126,     0,     0,     0,   127,   112,
     113,     0,   114,   115,   116,   117,   118,   119,   120,     0,
     111,     0,     0,   209,   210,   211,   212,   213,   214,   215,
     121,   122,     0,     0,     0,     0,   112,   113,     0,   114,
     115,   116,   117,   118,   119,   120,     0,   285,     0,     0,
       0,     0,     0,     0,     0,   123,   124,   121,   122,     0,
     125,   126,     0,     0,     0,   127,     0,     0,     0,     0,
       0,     0,     0,     0,   287,     0,     0,   111,     0,     0,
       0,     0,   123,   124,     0,     0,     0,   125,   126,     0,
       0,     0,   127,   112,   113,     0,   114,   115,   116,   117,
     118,   119,   120,     0,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,     0,     0,     0,     0,
     112,   113,     0,   114,   115,   116,   117,   118,   119,   120,
       0,   289,     0,     0,     0,     0,     0,     0,     0,   123,
     124,   121,   122,     0,   125,   126,     0,     0,     0,   127,
       0,     0,     0,     0,     0,     0,     0,     0,   291,     0,
       0,   111,     0,     0,     0,     0,   123,   124,     0,     0,
       0,   125,   126,     0,     0,     0,   127,   112,   113,     0,
     114,   115,   116,   117,   118,   119,   120,     5,   175,     0,
       8,     0,    10,    11,     0,     0,     0,     0,   121,   122,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,    16,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   124,     0,     0,     0,   125,   126,
       0,     0,     0,   127,     0,     0,     0,     0,     0,     0,
       0,    18,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   204,   205,     0,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
       0,     0,     0,     0,     0,     0,     0,    20,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   204,   205,     0,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,     0,     0,     0,     0,
       0,     0,     0,   258,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   204,
     205,     0,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,     0,     0,     0,     0,     0,     0,     0,   359,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,   204,   205,     0,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,     0,
       0,     0,     0,     0,   236,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   338,
     204,   205,     0,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,     0,     0,     0,     0,     0,     0,   339,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   204,   205,     0,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,     0,
       0,     0,     0,     0,   234,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,   205,     0,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,     0,     0,     0,     0,     0,     0,   237,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   204,   205,     0,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,     0,
       0,     0,     0,     0,   260,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,   205,     0,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,     0,     0,     0,     0,     0,     0,   354,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   204,   205,     0,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,     0,
       0,     0,     0,     0,   355,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,   205,     0,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,     0,     0,     0,     0,     0,     0,   357,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   204,   205,     0,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,     0,
       0,     0,     0,     0,   361,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     204,   205,     0,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,     0,     0,     0,     0,     0,     0,   370,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
       0,     0,     0,     0,     0,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,     0,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,     0,     0,
     204,   205,   216,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,     0,     0,     0,     0,   225,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,     0,     0,
       0,     0,     0,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,     0,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,     0,     0,   204,   205,
     259,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,     0,     0,     0,     0,   319,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,     0,     0,     0,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,     0,
       0,   204,   205,   322,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   204,   205,     0,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   194,   195,   196,
     197,   198,   199,   200,     0,   202,   203,     0,     0,     0,
       0,     0,     0,   205,     0,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   194,   195,   196,   197,   198,
     199,     0,     0,   202,   203,   194,   195,   196,   197,   198,
     199,     0,     0,   202,   203,     0,     0,     0,     0,     0,
       0,     0,     0,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,     0,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   194,   195,   196,   197,   198,   199,     0,
       0,   202,   203,   194,   195,   196,   197,   198,   199,     0,
       0,   202,   203,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,   209,   210,   211,   212,   213,   214,
     215,     0,     0,     0,   209,   210,   211,   212,   213,   214,
     215
};

static const short int yycheck[] =
{
       2,    12,    13,     2,    35,     9,     2,    35,   251,     3,
     311,     3,    20,    55,    54,    38,    39,   318,    92,    70,
      71,    95,    20,    20,     0,    24,    28,    20,    27,    71,
      55,    38,    39,    75,    74,    37,    35,    55,    49,    35,
      42,    45,    65,    66,    67,    47,    71,    46,    50,    20,
      48,    61,     1,    71,     3,     4,     5,     6,     7,     8,
       9,   135,    20,    57,    61,    57,    15,    16,   369,    77,
       3,    20,   103,     6,    54,     8,     9,   105,    27,    28,
      29,    54,    76,    72,    73,     3,    20,    20,     6,    61,
       8,     9,    38,    39,    74,    28,    29,    11,    12,    13,
      14,    74,    20,   107,   103,    61,   105,   103,    57,   105,
      28,    29,   355,    54,   357,    20,    75,    63,    64,    65,
      66,    67,    71,   154,    57,   368,    75,    57,    77,    76,
      20,   374,    20,    74,   145,    55,    76,    20,   142,    57,
      76,   143,   144,    20,   143,   144,   174,   143,   144,    76,
      76,    71,    20,    73,    73,   154,    73,    73,   154,    77,
      73,    72,    54,   165,   166,   167,   168,   169,     1,    40,
     172,    42,    43,    44,    73,   174,    54,     4,   174,    10,
      11,    12,    13,    14,    17,    18,    48,    20,    21,    22,
      23,    24,    25,    26,    76,    62,    55,    62,    54,    54,
      20,    73,    23,    78,    73,    38,    39,    23,    83,    84,
      74,    20,    20,   241,    20,    20,    20,   219,    20,    20,
     219,    54,    54,   219,   226,   100,   101,   102,   230,    77,
      63,    64,    62,   108,   109,    68,    69,    54,    72,    62,
      73,    55,   241,    76,    77,   241,   121,   122,   123,   124,
     125,   126,   127,    77,     3,     4,    74,     6,    73,     8,
       9,    73,   137,    73,    23,    43,    73,    62,    54,    41,
     182,    20,    75,    73,   305,    72,   255,   162,   104,    28,
      29,   220,   175,   158,   300,    13,    -1,    86,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    -1,    -1,    -1,
     311,    -1,    15,    16,    -1,    -1,   305,    20,    57,   305,
      -1,    -1,    -1,    -1,    27,    28,    29,    -1,   193,   194,
     195,   196,   197,   198,   199,   200,   201,    -1,    -1,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   343,   217,    -1,    57,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,     1,
     235,    -1,    75,   365,    77,    -1,    -1,    -1,   370,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,   260,    -1,    -1,    -1,    -1,
      -1,    -1,   267,    -1,   269,    -1,    38,    39,    -1,    -1,
     275,    -1,   277,    -1,   279,    -1,    -1,    -1,   283,    -1,
     285,    -1,   287,    -1,   289,    -1,   291,    -1,    -1,    34,
      35,    63,    64,    38,    39,   300,    68,    69,    -1,    -1,
      -1,    73,    -1,   308,    76,   310,   311,   312,    -1,    -1,
      -1,   316,    -1,   318,    -1,    -1,    -1,   322,    63,    64,
      65,    66,    67,    -1,    -1,     1,   331,     3,    -1,    -1,
      -1,    -1,    -1,   338,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,   356,    -1,   358,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   369,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
       1,    77,     3,    -1,    -1,     6,    -1,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    28,    29,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    69,    -1,
      71,    -1,    73,    -1,    75,    76,    77,     1,    -1,     3,
      -1,    -1,     6,    -1,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,     1,    63,
      64,    -1,    -1,    -1,    68,    69,    -1,    71,    -1,    73,
      -1,    75,    76,    77,    17,    18,    -1,    20,    21,    22,
      23,    24,    25,    26,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    -1,    -1,    38,    39,    -1,    15,    16,
      -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    68,    69,     1,    -1,    -1,
      73,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    -1,
      57,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    71,    -1,    -1,    -1,    75,    -1,
      77,    -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,
      44,    45,    46,    47,    -1,    -1,     0,     1,    -1,     3,
       4,     5,     6,     7,     8,     9,    -1,    -1,    -1,    63,
      64,    15,    16,    -1,    68,    69,    20,    71,    -1,    73,
      -1,    75,    76,    27,    28,    29,    -1,     1,    -1,     3,
      -1,    -1,     6,    -1,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    57,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    71,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    -1,    -1,    57,    -1,    -1,     1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    73,
      -1,    75,    17,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,    63,    64,
      38,    39,    -1,    68,    69,    -1,    -1,    -1,    73,    -1,
      75,    17,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    -1,     1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    69,    38,    39,    -1,    73,    -1,    75,    17,    18,
      -1,    20,    21,    22,    23,    24,    25,    26,    54,    -1,
      -1,    -1,    -1,    -1,     1,    -1,    -1,    63,    64,    38,
      39,    -1,    68,    69,    -1,    -1,    -1,    73,    74,    -1,
      17,    18,    -1,    20,    21,    22,    23,    24,    25,    26,
      -1,     1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,
      69,    38,    39,    72,    73,    -1,    -1,    17,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    -1,     1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    63,    64,    38,    39,
      -1,    68,    69,    -1,    17,    18,    73,    20,    21,    22,
      23,    24,    25,    26,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    38,    39,    -1,    68,    69,
      -1,    -1,     3,    73,    -1,     6,    -1,     8,     9,    -1,
      -1,    -1,    55,    -1,    -1,     1,    -1,    -1,    -1,    20,
      63,    64,    -1,    -1,    -1,    68,    69,    28,    29,    -1,
      73,    17,    18,    -1,    20,    21,    22,    23,    24,    25,
      26,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    38,    39,    -1,    -1,    57,    -1,    17,    18,
      -1,    20,    21,    22,    23,    24,    25,    26,    -1,    55,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    38,
      39,    -1,    68,    69,    -1,    -1,     3,    73,    -1,     6,
      -1,     8,     9,    -1,    -1,    -1,    55,    -1,    -1,     1,
      -1,    -1,    -1,    20,    63,    64,    -1,    -1,    -1,    68,
      69,    28,    29,    -1,    73,    17,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    -1,     1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    38,    39,    -1,    -1,
      57,    -1,    17,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    38,    39,    -1,    68,    69,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,     1,    32,    33,    34,    35,    63,    64,
      38,    39,    -1,    68,    69,    -1,    -1,    -1,    73,    17,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    -1,
       1,    -1,    -1,    61,    62,    63,    64,    65,    66,    67,
      38,    39,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      21,    22,    23,    24,    25,    26,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    38,    39,    -1,
      68,    69,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    -1,    -1,     1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    69,    -1,
      -1,    -1,    73,    17,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    -1,     1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    21,    22,    23,    24,    25,    26,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    38,    39,    -1,    68,    69,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,     1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    68,    69,    -1,    -1,    -1,    73,    17,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,     3,     4,    -1,
       6,    -1,     8,     9,    -1,    -1,    -1,    -1,    38,    39,
      -1,    -1,    -1,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,    69,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    -1,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    -1,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,    -1,    -1,    -1,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    55,    56,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    -1,    -1,
      55,    56,    72,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    72,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
      -1,    -1,    -1,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    55,    56,    -1,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    -1,    -1,    55,    56,
      72,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    72,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    -1,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    55,    56,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    30,    31,    32,    33,    34,
      35,    -1,    -1,    38,    39,    30,    31,    32,    33,    34,
      35,    -1,    -1,    38,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    30,    31,    32,    33,    34,    35,    -1,
      -1,    38,    39,    30,    31,    32,    33,    34,    35,    -1,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,    61,    62,    63,    64,    65,    66,
      67
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    79,    81,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    15,    16,    20,    27,    28,    29,    57,    71,
      75,    80,    82,   107,   108,   109,   110,   114,   115,   124,
     125,   126,   128,   129,   131,   133,    20,   127,    20,    61,
      20,    61,   112,    20,    20,   110,   116,   117,   130,   133,
     117,   130,    20,    61,    61,    20,    48,    10,    11,    12,
      13,    14,   105,   106,    76,   133,    20,   111,   112,   113,
     133,    80,    76,    20,    77,   107,   115,    55,    71,    80,
      57,   123,    20,    55,    71,   118,   122,    20,    80,   112,
     133,    80,    76,   117,    80,    76,    76,    20,    20,    73,
      73,    73,    73,    72,    54,   103,    73,    54,    55,    71,
       4,     1,    17,    18,    20,    21,    22,    23,    24,    25,
      26,    38,    39,    63,    64,    68,    69,    73,    85,    86,
      87,    88,    89,    90,    48,    76,    62,    76,    88,    72,
      88,   118,    62,    81,    81,    94,    54,    54,    20,    88,
      88,    88,   107,   110,   133,   105,    19,    20,    45,    46,
      47,    71,    76,    77,    80,    88,    95,    96,    97,    98,
     104,   110,   115,   132,   133,     4,     9,    91,    92,    93,
     117,   112,    61,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    70,    71,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    55,    56,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    72,    73,    73,    81,
      76,    88,   119,   120,   121,    72,   112,    77,    77,    77,
     117,    23,    23,    74,    74,    54,    74,    74,    20,    88,
      20,   103,    80,    80,    80,    80,    80,    80,    40,    42,
      43,    44,    99,   102,   127,    54,    74,    89,    75,    72,
      74,    20,    88,    88,    88,    88,    88,    55,    88,    55,
      88,    88,    88,    88,    88,    55,    88,    55,    88,    55,
      88,    88,    88,    55,    88,    55,    88,    55,    88,    55,
      88,    55,    88,    83,    84,    88,    20,    77,   119,    77,
      54,    80,    80,    62,    54,    72,    88,    72,    73,    77,
      73,    73,    73,   104,    91,    62,    55,    88,    73,    72,
      88,    88,    57,    88,    88,    88,    88,    88,    88,    88,
      88,    54,    74,    74,    77,   121,    23,   107,    54,    74,
      88,    88,    83,   101,   117,    88,    43,    75,    88,    83,
      88,    88,    62,    88,    74,    74,    80,    74,    73,    75,
      74,    74,    72,   104,    88,   100,   104,    88,    41,    80,
      74,   104,    83,    80,    74,   104
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value, Location);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");

# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;
  (void) yylocationp;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended. */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
  *++yylsp = yylloc;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, yylsp - yylen, yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 170 "parser.y"
    {
  //Construct root node
  AST::Root* root = new AST::Root();
  //Make sure to keep refcount > 0
  AST::TreeElementPtr rootPtr = root;
  
  //Add all the child nodes
  ParserVariant(rootPtr).AddChildren((yyvsp[0]));
  
  //Export it to a global variable (until i find a more elegant way to exit bison)
  AST::Root::TheCurrentTreeRoot() = root; 
;}
    break;

  case 3:
#line 183 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 4:
#line 187 "parser.y"
    { (yyval) = EMPTY   ;}
    break;

  case 5:
#line 188 "parser.y"
    { (yyval) = (yyvsp[-1]) + (yyvsp[0]) ;}
    break;

  case 6:
#line 191 "parser.y"
    { (yyval) = (yyvsp[-1])    ;}
    break;

  case 7:
#line 192 "parser.y"
    { (yyval) = (yyvsp[0])    ;}
    break;

  case 8:
#line 193 "parser.y"
    { (yyval) = (yyvsp[0])    ;}
    break;

  case 9:
#line 194 "parser.y"
    { (yyval) = (yyvsp[0])    ;}
    break;

  case 10:
#line 195 "parser.y"
    { (yyval) = (yyvsp[0])    ;}
    break;

  case 11:
#line 196 "parser.y"
    { (yyval) = (yyvsp[0])    ;}
    break;

  case 12:
#line 197 "parser.y"
    { (yyval) = EMPTY ;}
    break;

  case 13:
#line 198 "parser.y"
    { DIE("Parse error. Expected declaration.")((yyvsp[0])); ;}
    break;

  case 14:
#line 202 "parser.y"
    { (yyval) = new AST::ArgList(); ;}
    break;

  case 15:
#line 203 "parser.y"
    { (yyval) = (yyvsp[0]); ;}
    break;

  case 16:
#line 206 "parser.y"
    { (yyval) = new AST::ArgList(); (yyval).AddChildren((yyvsp[0])) ;}
    break;

  case 17:
#line 207 "parser.y"
    { (yyval) = (yyvsp[-2]);                 (yyval).AddChildren((yyvsp[0])) ;}
    break;

  case 18:
#line 213 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::Assign >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 19:
#line 214 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::BitAnd >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 20:
#line 215 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::BitOr  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 21:
#line 216 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::BitXor >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 22:
#line 217 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::Mult   >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 23:
#line 218 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::Div    >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 24:
#line 219 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::Mod    >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 25:
#line 220 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::Plus   >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 26:
#line 221 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::Minus  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 27:
#line 222 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::LShift >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 28:
#line 223 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::RShift >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 29:
#line 224 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::CmpLT  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 30:
#line 225 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::CmpGT  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 31:
#line 226 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::CmpLE  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 32:
#line 227 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::CmpGE  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 33:
#line 228 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::CmpEQ  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 34:
#line 229 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::CmpNE  >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 35:
#line 230 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::And    >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 36:
#line 231 "parser.y"
    { (yyval) = AST::MakeBinaryOp< AST::BinaryOps::Or     >((yyvsp[-2]),(yyvsp[0])) ;}
    break;

  case 37:
#line 232 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::BitAnd >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 38:
#line 233 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::BitOr  >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 39:
#line 234 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::BitXor >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 40:
#line 235 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::Mult   >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 41:
#line 236 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::Div    >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 42:
#line 237 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::Mod    >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 43:
#line 238 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::Plus   >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 44:
#line 239 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::Minus  >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 45:
#line 240 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::LShift >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 46:
#line 241 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::RShift >((yyvsp[-3]),(yyvsp[0])) ;}
    break;

  case 47:
#line 246 "parser.y"
    { (yyval) = new AST::UnaryOps::LogNot();   (yyval).AddChildren((yyvsp[0])); ;}
    break;

  case 48:
#line 249 "parser.y"
    { (yyval) = new AST::UnaryOps::BitNot();   (yyval).AddChildren((yyvsp[0])); ;}
    break;

  case 49:
#line 252 "parser.y"
    { (yyval) = new AST::UnaryOps::Negate();   (yyval).AddChildren((yyvsp[0])); ;}
    break;

  case 50:
#line 255 "parser.y"
    { (yyval)=(yyvsp[-1]) ;}
    break;

  case 51:
#line 258 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::Plus >((yyvsp[0]), AST::MakeIntegerLiteral("1")) ;}
    break;

  case 52:
#line 261 "parser.y"
    { (yyval) = AST::MakeBinaryOpAssign< AST::BinaryOps::Minus >((yyvsp[0]), AST::MakeIntegerLiteral("1")) ;}
    break;

  case 53:
#line 264 "parser.y"
    { 
    (yyval) = AST::MakeBinaryOpPostAssign< AST::BinaryOps::Plus >((yyvsp[-1]), AST::MakeIntegerLiteral("1"));
  ;}
    break;

  case 54:
#line 269 "parser.y"
    { 
(yyval) = AST::MakeBinaryOpPostAssign< AST::BinaryOps::Minus >((yyvsp[-1]), AST::MakeIntegerLiteral("1"));
  ;}
    break;

  case 55:
#line 277 "parser.y"
    { (yyval) = new AST::NumericLiteral<unsigned int, RegDT::Bool>(0xFFFFFFFF) ;}
    break;

  case 56:
#line 278 "parser.y"
    { (yyval) = new AST::NumericLiteral<unsigned int, RegDT::Bool>(0x00000000) ;}
    break;

  case 57:
#line 279 "parser.y"
    { (yyval) = AST::MakeIntegerLiteral((yyvsp[0])); ;}
    break;

  case 58:
#line 280 "parser.y"
    { (yyval) = AST::MakeHexLiteral((yyvsp[0])); ;}
    break;

  case 59:
#line 281 "parser.y"
    { (yyval) = AST::MakeFloatLiteral((yyvsp[0]));   ;}
    break;

  case 60:
#line 282 "parser.y"
    { (yyval) = AST::MakeDoubleLiteral((yyvsp[0]));  ;}
    break;

  case 61:
#line 283 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 62:
#line 284 "parser.y"
    { ERROR_UNIMPLEMENTED().Text("Strings"); ;}
    break;

  case 63:
#line 285 "parser.y"
    { (yyval) = AST::MakeCharLiteral((yyvsp[0]));   ;}
    break;

  case 64:
#line 286 "parser.y"
    { (yyval)=(yyvsp[-1]) ;}
    break;

  case 65:
#line 287 "parser.y"
    { (yyval)=(yyvsp[0]) ;}
    break;

  case 66:
#line 290 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 67:
#line 291 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 68:
#line 292 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 69:
#line 293 "parser.y"
    { (yyval) = new AST::FunctionCall();
				YYSTYPE al = new AST::ArgList();
                                al.AddChildren( (yyvsp[-4]) );
                                al.AddChildren( (yyvsp[-2]) );
                                al.AddChildren( (yyvsp[0]) );
				(yyval).AddChildren( new AST::DelayedLookup( ConstString::Lookup( "cmov" ) ) + al );
			      ;}
    break;

  case 70:
#line 300 "parser.y"
    { (yyval) = new AST::FunctionCall(); (yyval).AddChildren((yyvsp[-2]) + new AST::ArgList((yyvsp[0]))); ;}
    break;

  case 71:
#line 301 "parser.y"
    { DIE("Parse error. Invalid expression.")((yyvsp[0]).ToString()); ;}
    break;

  case 72:
#line 304 "parser.y"
    { (yyval) = new AST::DelayedLookup((yyvsp[0])); ;}
    break;

  case 73:
#line 306 "parser.y"
    { (yyval) = new AST::FunctionCall(); (yyval).AddChildren((yyvsp[-3]) + (yyvsp[-1])) ;}
    break;

  case 74:
#line 307 "parser.y"
    { (yyval) = new AST::BinaryOps::MemberAccess((yyvsp[0])); (yyval).AddChildren((yyvsp[-2])) ;}
    break;

  case 75:
#line 308 "parser.y"
    { (yyval) = new AST::ArrayIndex();   (yyval).AddChildren((yyvsp[-3]) + (yyvsp[-1])) ;}
    break;

  case 76:
#line 310 "parser.y"
    {
           //this syntax is to support .Load(), Sample(), etc for DX10 Textures
           //rather than adding full support for method calls we merely transform
           // a.Foo(b,c) to __objcall_Foo(a,b,c)
           (yyval) = new AST::FunctionCall();
           (yyval).AddChildren(new AST::DelayedLookup(ConstString::Lookup("__objcall_" + (yyvsp[-3]).Str())));
           (yyvsp[-1]).Elmt()->PrependChild((yyvsp[-5]));
           (yyval).AddChildren((yyvsp[-1]));
           ;}
    break;

  case 77:
#line 321 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 78:
#line 322 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 79:
#line 325 "parser.y"
    { (yyval) = EMPTY ;}
    break;

  case 80:
#line 326 "parser.y"
    { (yyval) = EMPTY ;}
    break;

  case 81:
#line 327 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 82:
#line 330 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 83:
#line 331 "parser.y"
    { (yyval) = (yyvsp[-2]) + (yyvsp[0]) ;}
    break;

  case 84:
#line 334 "parser.y"
    { (yyval) = EMPTY ;}
    break;

  case 85:
#line 335 "parser.y"
    { (yyval) = (yyvsp[-2]) + (yyvsp[-1]) ;}
    break;

  case 86:
#line 338 "parser.y"
    { (yyval) = new AST::UnaryOps::Return(); (yyval).AddChildren((yyvsp[0])) ;}
    break;

  case 87:
#line 340 "parser.y"
    { 
  (yyval) = new AST::UnaryOps::Return();
  (yyval).AddChildren(new AST::TypeCaster(AST::DataType::TheVoidType(), AST::TreeElementList() ));
;}
    break;

  case 88:
#line 344 "parser.y"
    { (yyval) = new AST::DiscardStatement(); ;}
    break;

  case 89:
#line 345 "parser.y"
    { (yyval) = new AST::BreakStatement(); ;}
    break;

  case 90:
#line 346 "parser.y"
    { (yyval) = new AST::ContinueStatement(); ;}
    break;

  case 91:
#line 349 "parser.y"
    {
  (yyval) = new AST::IfStatement((yyvsp[-2]),(yyvsp[0]));
;}
    break;

  case 92:
#line 354 "parser.y"
    {
  (yyval) = new AST::IfStatement((yyvsp[-4]),(yyvsp[-2]),(yyvsp[0]));
;}
    break;

  case 93:
#line 358 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 94:
#line 359 "parser.y"
    { (yyval) = AST::MakeIntegerLiteral("1") ;}
    break;

  case 95:
#line 362 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 96:
#line 363 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 97:
#line 367 "parser.y"
    {
  (yyval) = AST::LoopStatement::MakeFor((yyvsp[-6]),(yyvsp[-4]),(yyvsp[-2]),(yyvsp[0]));
;}
    break;

  case 98:
#line 372 "parser.y"
    {
  (yyval) = AST::LoopStatement::MakeWhile((yyvsp[-2]),(yyvsp[0]));
;}
    break;

  case 99:
#line 377 "parser.y"
    {
  (yyval) = AST::LoopStatement::MakeDo((yyvsp[-5]), (yyvsp[-2]));
;}
    break;

  case 100:
#line 381 "parser.y"
    { (yyval) = new AST::StatementList(); ;}
    break;

  case 101:
#line 382 "parser.y"
    { (yyval) = (yyvsp[-2]); (yyval).AddChildren((yyvsp[-1])) ;}
    break;

  case 102:
#line 383 "parser.y"
    { (yyval) = (yyvsp[-1]); (yyval).AddChildren((yyvsp[0])) ;}
    break;

  case 103:
#line 386 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 104:
#line 387 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 105:
#line 388 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 106:
#line 389 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 107:
#line 390 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 108:
#line 391 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 109:
#line 392 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 110:
#line 393 "parser.y"
    { (yyval) = new AST::NullTreeElement() ;}
    break;

  case 111:
#line 394 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 114:
#line 403 "parser.y"
    {
            gLdsInfo.lds_size = ((yyvsp[-1]).Elmt()->SimplifyToInt());                 
          ;}
    break;

  case 115:
#line 407 "parser.y"
    { 
            gLdsInfo.lds_abs = 1;
          ;}
    break;

  case 116:
#line 411 "parser.y"
    { 
            gLdsInfo.lds_rel = 1;
          ;}
    break;

  case 117:
#line 415 "parser.y"
    {
            gLdsInfo.numT[0] = ((yyvsp[-1]).Elmt()->SimplifyToInt());
            gLdsInfo.numT[1] = 1;
            gLdsInfo.numT[2] = 1;
          ;}
    break;

  case 118:
#line 421 "parser.y"
    {
            gLdsInfo.numT[0] = ((yyvsp[-3]).Elmt()->SimplifyToInt());
            gLdsInfo.numT[1] = ((yyvsp[-1]).Elmt()->SimplifyToInt());
            gLdsInfo.numT[2] = 1;
          ;}
    break;

  case 119:
#line 427 "parser.y"
    {
            gLdsInfo.numT[0] = ((yyvsp[-5]).Elmt()->SimplifyToInt());
            gLdsInfo.numT[1] = ((yyvsp[-3]).Elmt()->SimplifyToInt());
            gLdsInfo.numT[2] = ((yyvsp[-1]).Elmt()->SimplifyToInt());
          ;}
    break;

  case 120:
#line 435 "parser.y"
    {
                 (yyval) = (yyvsp[0]);
                 (yyval).Elmt()->AsFunction().SetMaxVertexCount((yyvsp[-3]).Elmt()->SimplifyToInt());
               ;}
    break;

  case 121:
#line 440 "parser.y"
    {
                 (yyval) = (yyvsp[0]);
                 (yyval).Elmt()->AsFunction().SetLdsSize(gLdsInfo.lds_size);
                 (yyval).Elmt()->AsFunction().SetLdsAbs(gLdsInfo.lds_abs);
                 (yyval).Elmt()->AsFunction().SetLdsRel(gLdsInfo.lds_rel);
                 (yyval).Elmt()->AsFunction().SetNumThreads(gLdsInfo.numT[0],
                                                       gLdsInfo.numT[1],
                                                       gLdsInfo.numT[2]  );
               ;}
    break;

  case 122:
#line 452 "parser.y"
    {
  ((yyval)=(yyvsp[0])).Elmt()->AddModifier((yyvsp[-1]))
;}
    break;

  case 123:
#line 457 "parser.y"
    {
  (yyval) = new AST::FunctionDeclaration((yyvsp[-3]), (yyvsp[-4]), (yyvsp[-1]).Size());
  (yyval).AddChildren((yyvsp[-1]));
;}
    break;

  case 124:
#line 462 "parser.y"
    {(yyval)=(yyvsp[0]); gLdsInfo.reset();;}
    break;

  case 125:
#line 464 "parser.y"
    {
              ((yyval)=(yyvsp[-1])).Elmt()->AddModifier((yyvsp[0]))
            ;}
    break;

  case 126:
#line 469 "parser.y"
    { ((yyval)=(yyvsp[-3])).AddChildren((yyvsp[-1])); ;}
    break;

  case 127:
#line 475 "parser.y"
    { (yyval) = (yyvsp[0]);  ;}
    break;

  case 128:
#line 476 "parser.y"
    { (yyval) = (yyvsp[-3]).Str() + (yyvsp[-1]).Str(); ;}
    break;

  case 129:
#line 477 "parser.y"
    { (yyval) = "float4";}
    break;

  case 130:
#line 478 "parser.y"
    { (yyval) = (yyvsp[-5]).Str() + (yyvsp[-3]).Str() + "x" + (yyvsp[-1]).Str(); ;}
    break;

  case 131:
#line 479 "parser.y"
    { (yyval) = "float4x4"; ;}
    break;

  case 132:
#line 480 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 133:
#line 481 "parser.y"
    { (yyval) = "u" + (yyvsp[0]).Str(); ;}
    break;

  case 134:
#line 482 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 135:
#line 485 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 136:
#line 489 "parser.y"
    {
  (yyval) = new AST::VariableDeclaration((yyvsp[-2]), (yyvsp[-1]));
  (yyval).AddChildren((yyvsp[0]));
;}
    break;

  case 137:
#line 495 "parser.y"
    {
  (yyval) = new AST::VariableDeclaration((yyvsp[-1]));
  (yyval).AddChildren((yyvsp[0]));
;}
    break;

  case 138:
#line 500 "parser.y"
    {(yyval)=(yyvsp[0]);}
    break;

  case 139:
#line 501 "parser.y"
    {(yyval)=(yyvsp[-2])+(yyvsp[0]);}
    break;

  case 140:
#line 503 "parser.y"
    { ((yyval)=(yyvsp[-1])).Elmt()->AddModifier((yyvsp[0])) ;}
    break;

  case 141:
#line 505 "parser.y"
    { 
                        const AST::TreeElementList& lst = (yyval) = (yyvsp[0]);
                        for(AST::TreeElementList::const_iterator i = lst.begin(); i != lst.end(); ++i)
                          (*i)->AsVarDecl().SetTypeStr((yyvsp[-1]));
                      ;}
    break;

  case 142:
#line 510 "parser.y"
    {  (yyval)=(yyvsp[0]) ;}
    break;

  case 143:
#line 511 "parser.y"
    { ((yyval)=(yyvsp[0])).Elmt()->AddModifier((yyvsp[-1])) ;}
    break;

  case 144:
#line 515 "parser.y"
    { ((yyval)=(yyvsp[-1])).Elmt()->AddModifier((yyvsp[0])) ;}
    break;

  case 145:
#line 516 "parser.y"
    { ((yyval)=(yyvsp[0])).Elmt()->AsVarDecl().SetTypeStr((yyvsp[-1])); ;}
    break;

  case 146:
#line 517 "parser.y"
    {  (yyval)=(yyvsp[0]) ;}
    break;

  case 147:
#line 518 "parser.y"
    { ((yyval)=(yyvsp[0])).Elmt()->AddModifier((yyvsp[-1])) ;}
    break;

  case 148:
#line 521 "parser.y"
    { (yyval) = EMPTY ;}
    break;

  case 149:
#line 522 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 150:
#line 523 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 151:
#line 525 "parser.y"
    {(yyval) = new AST::Initializer();;}
    break;

  case 152:
#line 526 "parser.y"
    {(yyval) = (yyvsp[0]);}
    break;

  case 153:
#line 528 "parser.y"
    { (yyval) = new AST::Initializer(); (yyval).AddChildren((yyvsp[0])) ;}
    break;

  case 154:
#line 529 "parser.y"
    { (yyval) = (yyvsp[-2]); (yyval).AddChildren((yyvsp[0])) ;}
    break;

  case 155:
#line 531 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 156:
#line 533 "parser.y"
    { (yyval) = (yyvsp[0]) ;}
    break;

  case 157:
#line 534 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 158:
#line 537 "parser.y"
    { (yyval)=AST::VariableDeclaration::UnspecifiedSize(); ;}
    break;

  case 159:
#line 538 "parser.y"
    { (yyval)=(yyvsp[-1]); ;}
    break;

  case 160:
#line 540 "parser.y"
    { (yyval)="" ;}
    break;

  case 161:
#line 541 "parser.y"
    { (yyval)="" ;}
    break;

  case 162:
#line 544 "parser.y"
    {
  (yyval) = new AST::StructDeclaration((yyvsp[-3]));
  (yyval).AddChildren((yyvsp[-1]));
;}
    break;

  case 163:
#line 550 "parser.y"
    { ((yyval)=(yyvsp[-1])).Elmt()->AsVarDecl().SetTypeStr((yyvsp[-5]), (yyvsp[-3])); ;}
    break;

  case 164:
#line 552 "parser.y"
    { 
                      STATIC_CONSTSTRING(f4,"float4");
                      ((yyval)=(yyvsp[-1])).Elmt()->AsVarDecl().SetTypeStr((yyvsp[-2]), f4); 
                   ;}
    break;

  case 165:
#line 559 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 166:
#line 560 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 167:
#line 565 "parser.y"
    { (yyval) = new AST::SamplerDeclaration((yyvsp[0])); ;}
    break;

  case 168:
#line 568 "parser.y"
    {(yyval)=(yyvsp[0]);}
    break;

  case 169:
#line 569 "parser.y"
    {(yyval)=(yyvsp[-3]); (yyval).Elmt()->AsSampler().SetSamplerArraySize((yyvsp[-1]).Elmt()->SimplifyToInt()); ;}
    break;

  case 170:
#line 571 "parser.y"
    {(yyval)=(yyvsp[-2]);}
    break;

  case 171:
#line 575 "parser.y"
    {(yyval)=(yyvsp[-1]); ;}
    break;

  case 172:
#line 576 "parser.y"
    {(yyval)=(yyvsp[-4]); (yyval).Elmt()->AsSampler().SetSamplerAttrb((yyvsp[-3]),(yyvsp[-1])); ;}
    break;

  case 173:
#line 577 "parser.y"
    {(yyval)=(yyvsp[-6]); (yyval).Elmt()->AsSampler().SetSamplerAttrb((yyvsp[-5]),(yyvsp[-2])); ;}
    break;

  case 174:
#line 578 "parser.y"
    {(yyval)=(yyvsp[-7]); (yyval).Elmt()->AsSampler().SetSamplerAttrb((yyvsp[-6]),(yyvsp[-1])); ;}
    break;

  case 175:
#line 581 "parser.y"
    { (yyval)="" ;}
    break;

  case 176:
#line 582 "parser.y"
    { (yyval)=(yyvsp[0]) ;}
    break;

  case 177:
#line 585 "parser.y"
    { 
    (yyval) = new AST::ConsBuffScope((yyvsp[-4]));
    (yyval).AddChildren((yyvsp[-1]));
  ;}
    break;

  case 178:
#line 591 "parser.y"
    {
    (yyval) = new AST::PersistentAllocScope((yyvsp[-3]));
    (yyval).AddChildren((yyvsp[-1]));
  ;}
    break;

  case 179:
#line 597 "parser.y"
    {
  (yyval) = new AST::PersistentAllocScope();
  (yyval).AddChildren((yyvsp[-1]));
;}
    break;

  case 180:
#line 603 "parser.y"
    {
  (yyval) = new AST::GlobalAllocScope((yyvsp[-3]));
  (yyval).AddChildren((yyvsp[-1]));
;}
    break;

  case 181:
#line 608 "parser.y"
    {
  (yyval) = new AST::GlobalAllocScope();
  (yyval).AddChildren((yyvsp[-1]));
;}
    break;

  case 182:
#line 613 "parser.y"
    { (yyval) = "" ;}
    break;

  case 183:
#line 614 "parser.y"
    { (yyval) = (yyvsp[-4]) ;}
    break;

  case 184:
#line 615 "parser.y"
    { (yyval) = (yyvsp[-1]) ;}
    break;

  case 185:
#line 618 "parser.y"
    { (yyval) = AST::Modifier((yyvsp[0]), AST::Modifier::MOD_KEYWORD);  ;}
    break;

  case 186:
#line 619 "parser.y"
    { (yyval) = AST::Modifier((yyvsp[0]), AST::Modifier::MOD_SEMANTIC); ;}
    break;

  case 187:
#line 620 "parser.y"
    { (yyval) = AST::Modifier((yyvsp[-1]), AST::Modifier::MOD_REGISTER); ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 3155 "parser.tab.cpp"

  yyvsp -= yylen;
  yyssp -= yylen;
  yylsp -= yylen;

  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval, &yylloc);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  yylsp -= yylen;
  yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping", yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though. */
  YYLLOC_DEFAULT (yyloc, yyerror_range - 1, 2);
  *++yylsp = yyloc;

  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}



