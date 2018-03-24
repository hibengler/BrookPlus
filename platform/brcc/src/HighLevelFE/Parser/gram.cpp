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
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENT = 258,
     TAG_NAME = 259,
     LABEL_NAME = 260,
     TYPEDEF_NAME = 261,
     STRING = 262,
     LSTRING = 263,
     ASM = 264,
     CHAR_CONST = 265,
     LCHAR_CONST = 266,
     INUM = 267,
     RNUM = 268,
     PP_DIR = 269,
     PP_LINE = 270,
     INVALID = 271,
     CONST = 272,
     VOLATILE = 273,
     OUT = 274,
     SHARED = 275,
     REDUCE = 276,
     VOUT = 277,
     ITER = 278,
     KERNEL = 279,
     RESTRICT = 280,
     AUTO = 281,
     EXTRN = 282,
     REGISTR = 283,
     STATIC = 284,
     TYPEDEF = 285,
     VOID = 286,
     CHAR = 287,
     CHAR2 = 288,
     CHAR3 = 289,
     CHAR4 = 290,
     SHORT = 291,
     SHORT2 = 292,
     SHORT3 = 293,
     SHORT4 = 294,
     INT = 295,
     INT2 = 296,
     INT3 = 297,
     INT4 = 298,
     LONG = 299,
     SGNED = 300,
     UNSGNED = 301,
     GCC_BUILTIN = 302,
     FLOAT = 303,
     FLOAT2 = 304,
     FLOAT3 = 305,
     FLOAT4 = 306,
     DOUBLE = 307,
     DOUBLE2 = 308,
     ENUM = 309,
     STRUCT = 310,
     UNION = 311,
     BREAK = 312,
     CASE = 313,
     CONT = 314,
     DEFLT = 315,
     DO = 316,
     ELSE = 317,
     IF = 318,
     FOR = 319,
     GOTO = 320,
     RETURN = 321,
     SWITCH = 322,
     WHILE = 323,
     TARGET = 324,
     ASM_SCRATCH = 325,
     ASM_READ = 326,
     ASM_WRITE = 327,
     PLUS_EQ = 328,
     MINUS_EQ = 329,
     STAR_EQ = 330,
     DIV_EQ = 331,
     MOD_EQ = 332,
     B_AND_EQ = 333,
     B_OR_EQ = 334,
     B_XOR_EQ = 335,
     L_SHIFT_EQ = 336,
     R_SHIFT_EQ = 337,
     EQUAL = 338,
     LESS_EQ = 339,
     GRTR_EQ = 340,
     NOT_EQ = 341,
     RPAREN = 342,
     RBRCKT = 343,
     LBRACE = 344,
     RBRACE = 345,
     SEMICOLON = 346,
     COMMA = 347,
     ELLIPSIS = 348,
     LB_SIGN = 349,
     DOUB_LB_SIGN = 350,
     BACKQUOTE = 351,
     AT = 352,
     CONSTRUCTOR_FLOAT = 353,
     CONSTRUCTOR_DOUBLE = 354,
     CONSTRUCTOR_CHAR = 355,
     CONSTRUCTOR_UCHAR = 356,
     CONSTRUCTOR_SHORT = 357,
     CONSTRUCTOR_USHORT = 358,
     CONSTRUCTOR_INT = 359,
     CONSTRUCTOR_UINT = 360,
     ATTRIBUTE = 361,
     KERNEL_ATTRIBUTE = 362,
     ALIGNED = 363,
     PACKED = 364,
     CDECL = 365,
     MODE = 366,
     FORMAT = 367,
     NORETURN = 368,
     NOTHROW = 369,
     NONNULL = 370,
     PURE = 371,
     GROUPSIZE = 372,
     UNKNOWN_GCC_ATTRIBUTE = 373,
     UNKNOWN_KERNEL_ATTRIBUTE = 374,
     MALLOC = 375,
     WARN_UNUSED_RESULT = 376,
     COMMA_OP = 377,
     ASSIGN = 378,
     EQ = 379,
     COMMA_SEP = 380,
     COLON = 381,
     QUESTMARK = 382,
     OR = 383,
     AND = 384,
     B_OR = 385,
     B_XOR = 386,
     B_AND = 387,
     COMP_EQ = 388,
     GRTR = 389,
     LESS = 390,
     COMP_GRTR = 391,
     COMP_LESS = 392,
     COMP_ARITH = 393,
     R_SHIFT = 394,
     L_SHIFT = 395,
     MINUS = 396,
     PLUS = 397,
     MOD = 398,
     DIV = 399,
     STAR = 400,
     CAST = 401,
     DECR = 402,
     INCR = 403,
     SYNC_GROUP = 404,
     INSTANCE_IN_GROUP = 405,
     INSTANCE = 406,
     INDEXOF = 407,
     SIZEOF = 408,
     B_NOT = 409,
     NOT = 410,
     UNARY = 411,
     HYPERUNARY = 412,
     LBRCKT = 413,
     LPAREN = 414,
     DOT = 415,
     ARROW = 416
   };
#endif
/* Tokens.  */
#define IDENT 258
#define TAG_NAME 259
#define LABEL_NAME 260
#define TYPEDEF_NAME 261
#define STRING 262
#define LSTRING 263
#define ASM 264
#define CHAR_CONST 265
#define LCHAR_CONST 266
#define INUM 267
#define RNUM 268
#define PP_DIR 269
#define PP_LINE 270
#define INVALID 271
#define CONST 272
#define VOLATILE 273
#define OUT 274
#define SHARED 275
#define REDUCE 276
#define VOUT 277
#define ITER 278
#define KERNEL 279
#define RESTRICT 280
#define AUTO 281
#define EXTRN 282
#define REGISTR 283
#define STATIC 284
#define TYPEDEF 285
#define VOID 286
#define CHAR 287
#define CHAR2 288
#define CHAR3 289
#define CHAR4 290
#define SHORT 291
#define SHORT2 292
#define SHORT3 293
#define SHORT4 294
#define INT 295
#define INT2 296
#define INT3 297
#define INT4 298
#define LONG 299
#define SGNED 300
#define UNSGNED 301
#define GCC_BUILTIN 302
#define FLOAT 303
#define FLOAT2 304
#define FLOAT3 305
#define FLOAT4 306
#define DOUBLE 307
#define DOUBLE2 308
#define ENUM 309
#define STRUCT 310
#define UNION 311
#define BREAK 312
#define CASE 313
#define CONT 314
#define DEFLT 315
#define DO 316
#define ELSE 317
#define IF 318
#define FOR 319
#define GOTO 320
#define RETURN 321
#define SWITCH 322
#define WHILE 323
#define TARGET 324
#define ASM_SCRATCH 325
#define ASM_READ 326
#define ASM_WRITE 327
#define PLUS_EQ 328
#define MINUS_EQ 329
#define STAR_EQ 330
#define DIV_EQ 331
#define MOD_EQ 332
#define B_AND_EQ 333
#define B_OR_EQ 334
#define B_XOR_EQ 335
#define L_SHIFT_EQ 336
#define R_SHIFT_EQ 337
#define EQUAL 338
#define LESS_EQ 339
#define GRTR_EQ 340
#define NOT_EQ 341
#define RPAREN 342
#define RBRCKT 343
#define LBRACE 344
#define RBRACE 345
#define SEMICOLON 346
#define COMMA 347
#define ELLIPSIS 348
#define LB_SIGN 349
#define DOUB_LB_SIGN 350
#define BACKQUOTE 351
#define AT 352
#define CONSTRUCTOR_FLOAT 353
#define CONSTRUCTOR_DOUBLE 354
#define CONSTRUCTOR_CHAR 355
#define CONSTRUCTOR_UCHAR 356
#define CONSTRUCTOR_SHORT 357
#define CONSTRUCTOR_USHORT 358
#define CONSTRUCTOR_INT 359
#define CONSTRUCTOR_UINT 360
#define ATTRIBUTE 361
#define KERNEL_ATTRIBUTE 362
#define ALIGNED 363
#define PACKED 364
#define CDECL 365
#define MODE 366
#define FORMAT 367
#define NORETURN 368
#define NOTHROW 369
#define NONNULL 370
#define PURE 371
#define GROUPSIZE 372
#define UNKNOWN_GCC_ATTRIBUTE 373
#define UNKNOWN_KERNEL_ATTRIBUTE 374
#define MALLOC 375
#define WARN_UNUSED_RESULT 376
#define COMMA_OP 377
#define ASSIGN 378
#define EQ 379
#define COMMA_SEP 380
#define COLON 381
#define QUESTMARK 382
#define OR 383
#define AND 384
#define B_OR 385
#define B_XOR 386
#define B_AND 387
#define COMP_EQ 388
#define GRTR 389
#define LESS 390
#define COMP_GRTR 391
#define COMP_LESS 392
#define COMP_ARITH 393
#define R_SHIFT 394
#define L_SHIFT 395
#define MINUS 396
#define PLUS 397
#define MOD 398
#define DIV 399
#define STAR 400
#define CAST 401
#define DECR 402
#define INCR 403
#define SYNC_GROUP 404
#define INSTANCE_IN_GROUP 405
#define INSTANCE 406
#define INDEXOF 407
#define SIZEOF 408
#define B_NOT 409
#define NOT 410
#define UNARY 411
#define HYPERUNARY 412
#define LBRCKT 413
#define LPAREN 414
#define DOT 415
#define ARROW 416




/* Copy the first part of user declarations.  */
#line 41 "gram.y"

/* grammar File for C - Shaun Flisakowski and Patrick Baudin */
/* Grammar was constructed with the assistance of:
    "C - A Reference Manual" (Fourth Edition),
    by Samuel P Harbison, and Guy L Steele Jr. */

#ifdef    NO_ALLOCA
#define   alloca  __builtin_alloca
#endif

#ifdef _WIN32
// When using CL to compile, the malloc and free declarations that Bison adds can conflict with stdlib
#define YYMALLOC malloc
#define YYFREE   free
/* Don't complain about switch() statements that only have a 'default' */
#pragma warning( disable : 4065 )
#endif

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <setjmp.h>
#include <iostream>

#include "lexer.h"
#include "symbol.h"
#include "token.h"
#include "stemnt.h"
#include "location.h"
#include "project.h"
#include "brtexpress.h"
extern int err_cnt;
int yylex(YYSTYPE *lvalp);

extern int err_top_level;
/* Cause the `yydebug' variable to be defined.  */
#define YYDEBUG 1
void baseTypeFixup(BaseType *, Decl * decl) {
  BaseType * b = decl->form->getBase();
  while ((decl=decl->next)) {
    BaseType *nb = decl->form->getBase();
    if (memcmp(nb,b,sizeof(BaseType))!=0) {
      decl->form = decl->form->dup();
      *decl->form->getBase()=*b;
    }
  }

}


Location currentLocation;

/*  int  yydebug = 1;  */

/* ###################################################### */
#line 280 "gram.y"

/* 1 if we explained undeclared var errors.  */
/*  static int undeclared_variable_notice = 0;  */


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
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 479 "gram.tab.c"

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
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  87
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3616

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  162
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  213
/* YYNRULES -- Number of rules. */
#define YYNRULES  476
/* YYNRULES -- Number of states. */
#define YYNSTATES  765

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   416

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    15,    16,    18,
      20,    22,    24,    27,    31,    35,    38,    42,    46,    47,
      53,    56,    57,    59,    62,    63,    66,    69,    71,    76,
      79,    80,    82,    85,    87,    89,    91,    94,    97,   101,
     103,   105,   107,   109,   111,   113,   115,   117,   119,   121,
     123,   125,   127,   129,   132,   135,   139,   141,   143,   145,
     147,   149,   155,   158,   161,   165,   169,   171,   177,   185,
     187,   195,   201,   211,   213,   215,   217,   219,   222,   227,
     229,   231,   237,   239,   243,   247,   248,   250,   252,   253,
     255,   257,   259,   263,   265,   269,   272,   274,   278,   280,
     284,   286,   290,   293,   295,   300,   302,   306,   308,   312,
     314,   318,   320,   324,   326,   330,   332,   334,   336,   338,
     340,   342,   344,   346,   348,   350,   352,   354,   356,   363,
     372,   383,   388,   393,   398,   404,   412,   422,   428,   434,
     442,   452,   458,   466,   476,   482,   490,   500,   506,   514,
     524,   530,   538,   548,   554,   562,   572,   576,   580,   584,
     588,   592,   596,   600,   604,   606,   608,   615,   620,   622,
     624,   626,   628,   630,   632,   634,   636,   638,   640,   645,
     648,   651,   656,   660,   664,   668,   671,   674,   677,   680,
     683,   686,   688,   692,   694,   696,   698,   700,   704,   708,
     710,   712,   714,   716,   718,   720,   722,   724,   726,   728,
     733,   735,   737,   740,   743,   745,   749,   753,   758,   759,
     761,   763,   765,   769,   773,   775,   777,   779,   781,   783,
     785,   787,   789,   791,   793,   795,   797,   799,   801,   803,
     805,   807,   809,   811,   812,   813,   816,   817,   820,   821,
     822,   825,   827,   830,   834,   837,   840,   843,   846,   849,
     850,   852,   854,   855,   858,   860,   863,   865,   867,   869,
     871,   873,   875,   877,   879,   881,   882,   884,   887,   888,
     892,   895,   896,   898,   899,   903,   906,   908,   909,   913,
     915,   919,   920,   922,   924,   926,   930,   932,   934,   938,
     940,   942,   944,   949,   950,   952,   954,   956,   958,   960,
     962,   964,   966,   968,   970,   975,   977,   980,   981,   983,
     985,   987,   989,   991,   993,   995,   997,   999,  1001,  1003,
    1005,  1007,  1009,  1011,  1013,  1015,  1017,  1019,  1021,  1023,
    1025,  1027,  1029,  1031,  1033,  1035,  1037,  1039,  1041,  1043,
    1045,  1047,  1050,  1053,  1056,  1059,  1064,  1069,  1072,  1077,
    1082,  1085,  1090,  1095,  1096,  1098,  1101,  1102,  1105,  1106,
    1108,  1110,  1112,  1116,  1118,  1122,  1124,  1125,  1126,  1130,
    1133,  1136,  1140,  1143,  1146,  1148,  1150,  1151,  1155,  1156,
    1162,  1164,  1166,  1168,  1169,  1174,  1176,  1177,  1179,  1181,
    1183,  1186,  1188,  1190,  1192,  1196,  1198,  1200,  1205,  1210,
    1214,  1219,  1224,  1226,  1228,  1232,  1234,  1235,  1237,  1241,
    1244,  1246,  1249,  1251,  1252,  1255,  1257,  1261,  1263,  1265,
    1267,  1269,  1270,  1271,  1274,  1275,  1278,  1280,  1284,  1286,
    1290,  1291,  1294,  1297,  1300,  1302,  1304,  1306,  1309,  1311,
    1315,  1319,  1324,  1328,  1333,  1334,  1336,  1338,  1341,  1348,
    1350,  1352,  1354,  1356,  1358,  1360,  1365,  1370,  1379,  1384,
    1386,  1388,  1390,  1395,  1397,  1402,  1404
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     163,     0,    -1,    -1,   164,    -1,     1,    -1,   166,   165,
      -1,   164,   166,   165,    -1,    -1,   269,    -1,   167,    -1,
      14,    -1,    15,    -1,     1,    91,    -1,     1,    90,   165,
      -1,   372,   168,   169,    -1,   168,   169,    -1,   273,   339,
     261,    -1,   272,   338,   261,    -1,    -1,    89,   170,   263,
     171,    90,    -1,     1,    90,    -1,    -1,   172,    -1,   180,
     173,    -1,    -1,   173,   174,    -1,   173,    15,    -1,   179,
      -1,    89,   263,   176,    90,    -1,     1,    90,    -1,    -1,
     177,    -1,   180,   178,    -1,   180,    -1,   179,    -1,    15,
      -1,   178,   179,    -1,   178,    15,    -1,   178,     1,    91,
      -1,   180,    -1,   181,    -1,   182,    -1,   183,    -1,   175,
      -1,   184,    -1,   194,    -1,   185,    -1,   186,    -1,   187,
      -1,   188,    -1,   189,    -1,   190,    -1,   191,    -1,   224,
      91,    -1,   207,    91,    -1,   198,   126,   179,    -1,   192,
      -1,   193,    -1,   195,    -1,   196,    -1,   197,    -1,    67,
     159,   207,    87,   179,    -1,    57,    91,    -1,    59,    91,
      -1,    66,   206,    91,    -1,    65,     5,    91,    -1,    91,
      -1,    63,   159,   207,    87,   179,    -1,    63,   159,   207,
      87,   179,    62,   179,    -1,     9,    -1,    61,   179,    68,
     159,   207,    87,    91,    -1,    68,   159,   207,    87,   179,
      -1,    64,   159,   206,    91,   206,    91,   206,    87,   179,
      -1,   199,    -1,   200,    -1,   201,    -1,   353,    -1,    58,
     205,    -1,    58,   205,    93,   205,    -1,    60,    -1,   208,
      -1,   208,   127,   207,   126,   202,    -1,   202,    -1,   228,
     259,   203,    -1,   228,   259,   224,    -1,    -1,   205,    -1,
     207,    -1,    -1,   207,    -1,   240,    -1,   209,    -1,   208,
     128,   209,    -1,   211,    -1,   209,   129,   211,    -1,   155,
     215,    -1,   212,    -1,   211,   130,   212,    -1,   213,    -1,
     212,   131,   213,    -1,   216,    -1,   213,   132,   216,    -1,
     154,   215,    -1,   228,    -1,   159,   277,    87,   215,    -1,
     217,    -1,   216,   256,   217,    -1,   218,    -1,   217,   257,
     218,    -1,   219,    -1,   218,   258,   219,    -1,   220,    -1,
     219,   254,   220,    -1,   215,    -1,   220,   255,   215,    -1,
      49,    -1,    41,    -1,    53,    -1,    33,    -1,    37,    -1,
      50,    -1,    42,    -1,    34,    -1,    38,    -1,    51,    -1,
      43,    -1,    35,    -1,    39,    -1,   221,   159,   203,    92,
     203,    87,    -1,   222,   159,   203,    92,   203,    92,   203,
      87,    -1,   223,   159,   203,    92,   203,    92,   203,    92,
     203,    87,    -1,   221,   159,     1,    87,    -1,   222,   159,
       1,    87,    -1,   223,   159,     1,    87,    -1,    98,   203,
      92,   203,    87,    -1,    98,   203,    92,   203,    92,   203,
      87,    -1,    98,   203,    92,   203,    92,   203,    92,   203,
      87,    -1,    99,   203,    92,   203,    87,    -1,   104,   203,
      92,   203,    87,    -1,   104,   203,    92,   203,    92,   203,
      87,    -1,   104,   203,    92,   203,    92,   203,    92,   203,
      87,    -1,   105,   203,    92,   203,    87,    -1,   105,   203,
      92,   203,    92,   203,    87,    -1,   105,   203,    92,   203,
      92,   203,    92,   203,    87,    -1,   102,   203,    92,   203,
      87,    -1,   102,   203,    92,   203,    92,   203,    87,    -1,
     102,   203,    92,   203,    92,   203,    92,   203,    87,    -1,
     103,   203,    92,   203,    87,    -1,   103,   203,    92,   203,
      92,   203,    87,    -1,   103,   203,    92,   203,    92,   203,
      92,   203,    87,    -1,   100,   203,    92,   203,    87,    -1,
     100,   203,    92,   203,    92,   203,    87,    -1,   100,   203,
      92,   203,    92,   203,    92,   203,    87,    -1,   101,   203,
      92,   203,    87,    -1,   101,   203,    92,   203,    92,   203,
      87,    -1,   101,   203,    92,   203,    92,   203,    92,   203,
      87,    -1,    98,     1,    87,    -1,    99,     1,    87,    -1,
     100,     1,    87,    -1,   101,     1,    87,    -1,   102,     1,
      87,    -1,   103,     1,    87,    -1,   104,     1,    87,    -1,
     105,     1,    87,    -1,   203,    -1,   224,    -1,    23,   159,
     226,    92,   226,    87,    -1,    23,   159,     1,    87,    -1,
     243,    -1,   229,    -1,   234,    -1,   235,    -1,   210,    -1,
     214,    -1,   236,    -1,   237,    -1,   238,    -1,   239,    -1,
     153,   159,   275,    87,    -1,   153,   228,    -1,   152,   353,
      -1,   152,   159,   353,    87,    -1,   151,   159,    87,    -1,
     150,   159,    87,    -1,   149,   159,    87,    -1,   141,   215,
      -1,   142,   215,    -1,   132,   215,    -1,   145,   215,    -1,
     148,   228,    -1,   147,   228,    -1,   203,    -1,   240,    92,
     203,    -1,   353,    -1,   242,    -1,   260,    -1,   225,    -1,
     159,   207,    87,    -1,   159,     1,    87,    -1,   241,    -1,
     244,    -1,   245,    -1,   251,    -1,   246,    -1,   247,    -1,
     230,    -1,   231,    -1,   232,    -1,   233,    -1,   243,   158,
     207,    88,    -1,   249,    -1,   250,    -1,   243,   148,    -1,
     243,   147,    -1,   355,    -1,   243,   160,   248,    -1,   243,
     161,   248,    -1,   243,   159,   252,    87,    -1,    -1,   253,
      -1,   203,    -1,   224,    -1,   253,    92,   203,    -1,   253,
      92,   224,    -1,   142,    -1,   141,    -1,   145,    -1,   144,
      -1,   143,    -1,   133,    -1,   138,    -1,   137,    -1,   136,
      -1,   140,    -1,   139,    -1,   124,    -1,   123,    -1,    12,
      -1,    13,    -1,    10,    -1,    11,    -1,     7,    -1,     8,
      -1,    -1,    -1,   262,   267,    -1,    -1,   264,   265,    -1,
      -1,    -1,   266,   267,    -1,   268,    -1,   267,   268,    -1,
     267,     1,    91,    -1,   271,    91,    -1,   270,    91,    -1,
     271,    91,    -1,   272,   293,    -1,   273,   293,    -1,    -1,
     278,    -1,   365,    -1,    -1,   276,   277,    -1,   278,    -1,
     278,   274,    -1,   284,    -1,    27,    -1,    29,    -1,    30,
      -1,    26,    -1,    28,    -1,   279,    -1,   280,    -1,   304,
      -1,    -1,   284,    -1,   281,   283,    -1,    -1,   282,   285,
     283,    -1,   300,   283,    -1,    -1,   287,    -1,    -1,   304,
     288,   286,    -1,   300,   286,    -1,   287,    -1,    -1,   338,
     291,   368,    -1,   290,    -1,   290,   124,   296,    -1,    -1,
     294,    -1,   295,    -1,   292,    -1,   295,    92,   292,    -1,
     298,    -1,   298,    -1,   297,    92,   298,    -1,   203,    -1,
     224,    -1,   227,    -1,    89,   297,   299,    90,    -1,    -1,
      92,    -1,   301,    -1,    17,    -1,    18,    -1,    19,    -1,
      20,    -1,    21,    -1,    23,    -1,    24,    -1,    25,    -1,
      22,   158,   204,    88,    -1,   301,    -1,   302,   301,    -1,
      -1,   302,    -1,   315,    -1,   311,    -1,   313,    -1,   309,
      -1,   307,    -1,   308,    -1,   305,    -1,    31,    -1,    32,
      -1,    33,    -1,    34,    -1,    35,    -1,    36,    -1,    37,
      -1,    38,    -1,    39,    -1,    40,    -1,    41,    -1,    42,
      -1,    43,    -1,    47,    -1,    44,    -1,    48,    -1,    49,
      -1,    50,    -1,    51,    -1,    52,    -1,    53,    -1,    45,
      -1,    46,    -1,     6,    -1,     4,    -1,    55,   306,    -1,
      56,   306,    -1,    54,   306,    -1,    55,   306,    -1,    55,
      89,   316,    90,    -1,   310,    89,   316,    90,    -1,    56,
     306,    -1,    56,    89,   316,    90,    -1,   312,    89,   316,
      90,    -1,    54,   306,    -1,    54,    89,   317,    90,    -1,
     314,    89,   317,    90,    -1,    -1,   323,    -1,    14,   323,
      -1,    -1,   319,   318,    -1,    -1,    92,    -1,   320,    -1,
     321,    -1,   319,    92,   321,    -1,   322,    -1,   322,   124,
     203,    -1,   355,    -1,    -1,    -1,   324,   325,   326,    -1,
     327,    91,    -1,   326,    91,    -1,   326,   327,    91,    -1,
     326,    14,    -1,   289,   328,    -1,   289,    -1,   329,    -1,
      -1,   330,   332,   368,    -1,    -1,   329,    92,   331,   332,
     368,    -1,   333,    -1,   334,    -1,   338,    -1,    -1,   337,
     126,   335,   336,    -1,   202,    -1,    -1,   338,    -1,   340,
      -1,   340,    -1,   349,   341,    -1,   341,    -1,   342,    -1,
     353,    -1,   159,   340,    87,    -1,   343,    -1,   344,    -1,
     342,   159,   358,    87,    -1,   342,   159,   350,    87,    -1,
     342,   159,    87,    -1,   342,   158,   204,    88,    -1,   342,
     137,   346,   136,    -1,   260,    -1,   251,    -1,   159,   207,
      87,    -1,   353,    -1,    -1,   345,    -1,   346,    92,   345,
      -1,   145,   303,    -1,   347,    -1,   348,   347,    -1,   348,
      -1,    -1,   351,   352,    -1,   353,    -1,   352,    92,   353,
      -1,     3,    -1,     6,    -1,   353,    -1,   354,    -1,    -1,
      -1,   357,   360,    -1,    -1,   359,   360,    -1,   361,    -1,
     361,    92,    93,    -1,   362,    -1,   361,    92,   362,    -1,
      -1,   363,   364,    -1,   278,   338,    -1,   278,   365,    -1,
     278,    -1,   349,    -1,   366,    -1,   349,   366,    -1,   367,
      -1,   159,   365,    87,    -1,   158,   204,    88,    -1,   367,
     158,   204,    88,    -1,   159,   356,    87,    -1,   367,   159,
     356,    87,    -1,    -1,   369,    -1,   370,    -1,   369,   370,
      -1,   106,   159,   159,   371,    87,    87,    -1,   118,    -1,
     109,    -1,   110,    -1,    17,    -1,   113,    -1,   114,    -1,
     108,   159,    12,    87,    -1,   111,   159,   353,    87,    -1,
     112,   159,   353,    92,    12,    92,    12,    87,    -1,   115,
     159,   374,    87,    -1,   116,    -1,   120,    -1,   121,    -1,
     107,   158,   373,    88,    -1,   119,    -1,   117,   159,   374,
      87,    -1,    12,    -1,   374,    92,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   292,   292,   299,   310,   318,   323,   330,   338,   342,
     346,   350,   354,   358,   365,   395,   423,   452,   486,   485,
     503,   510,   513,   516,   535,   538,   547,   564,   567,   577,
     584,   587,   590,   606,   612,   616,   620,   629,   638,   644,
     648,   654,   655,   656,   657,   658,   659,   660,   661,   662,
     663,   664,   665,   691,   697,   703,   716,   717,   720,   721,
     722,   725,   731,   737,   743,   749,   755,   761,   767,   773,
     784,   790,   796,   803,   804,   805,   808,   816,   820,   826,
     841,   842,   848,   849,   853,   860,   863,   866,   870,   873,
     876,   879,   880,   886,   887,   893,   899,   900,   906,   907,
     913,   914,   920,   926,   927,   933,   934,   940,   941,   947,
     948,   954,   955,   961,   962,   967,   967,   967,   967,   967,
     968,   968,   968,   968,   969,   969,   969,   969,   971,   976,
     982,   988,   992,   996,  1001,  1007,  1014,  1022,  1030,  1036,
    1043,  1052,  1058,  1065,  1074,  1080,  1087,  1096,  1102,  1109,
    1118,  1124,  1131,  1140,  1146,  1153,  1160,  1165,  1170,  1175,
    1180,  1185,  1190,  1195,  1201,  1202,  1205,  1219,  1225,  1226,
    1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1237,  1241,
    1247,  1251,  1270,  1276,  1282,  1288,  1294,  1301,  1307,  1313,
    1319,  1325,  1326,  1332,  1336,  1337,  1341,  1347,  1351,  1357,
    1358,  1359,  1360,  1361,  1362,  1363,  1364,  1365,  1366,  1370,
    1376,  1377,  1380,  1386,  1392,  1396,  1418,  1440,  1452,  1455,
    1458,  1459,  1460,  1465,  1472,  1473,  1476,  1477,  1478,  1481,
    1484,  1485,  1486,  1489,  1490,  1493,  1494,  1497,  1498,  1499,
    1500,  1501,  1502,  1511,  1520,  1520,  1541,  1541,  1554,  1558,
    1558,  1574,  1578,  1588,  1594,  1608,  1614,  1621,  1639,  1665,
    1671,  1680,  1684,  1684,  1695,  1704,  1727,  1736,  1737,  1738,
    1741,  1742,  1745,  1746,  1756,  1760,  1763,  1766,  1782,  1782,
    1919,  1940,  1943,  1946,  1946,  1956,  1971,  1981,  1980,  1991,
    1992,  2000,  2003,  2006,  2009,  2013,  2028,  2030,  2035,  2042,
    2043,  2044,  2045,  2052,  2055,  2066,  2069,  2070,  2071,  2072,
    2073,  2074,  2075,  2076,  2077,  2085,  2086,  2095,  2098,  2105,
    2106,  2107,  2108,  2109,  2110,  2111,  2112,  2113,  2114,  2115,
    2116,  2117,  2118,  2119,  2120,  2121,  2122,  2123,  2124,  2125,
    2126,  2127,  2128,  2129,  2130,  2131,  2132,  2133,  2134,  2137,
    2144,  2152,  2162,  2172,  2182,  2192,  2197,  2219,  2229,  2235,
    2258,  2268,  2273,  2295,  2298,  2299,  2306,  2309,  2315,  2318,
    2328,  2331,  2336,  2343,  2355,  2369,  2376,  2381,  2376,  2395,
    2400,  2407,  2412,  2418,  2423,  2434,  2438,  2438,  2446,  2445,
    2456,  2474,  2485,  2489,  2488,  2508,  2512,  2515,  2522,  2528,
    2534,  2539,  2542,  2545,  2550,  2554,  2555,  2556,  2570,  2584,
    2606,  2618,  2629,  2633,  2637,  2641,  2648,  2651,  2655,  2664,
    2670,  2671,  2678,  2685,  2685,  2700,  2705,  2717,  2721,  2733,
    2734,  2743,  2747,  2747,  2756,  2756,  2769,  2770,  2779,  2780,
    2788,  2788,  2798,  2814,  2831,  2844,  2848,  2852,  2859,  2862,
    2866,  2870,  2880,  2884,  2901,  2904,  2907,  2908,  2914,  2920,
    2925,  2929,  2933,  2937,  2941,  2945,  2957,  2962,  2982,  2987,
    2991,  2995,  3001,  3007,  3012,  3019,  3024
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENT", "TAG_NAME", "LABEL_NAME",
  "TYPEDEF_NAME", "STRING", "LSTRING", "ASM", "CHAR_CONST", "LCHAR_CONST",
  "INUM", "RNUM", "PP_DIR", "PP_LINE", "INVALID", "CONST", "VOLATILE",
  "OUT", "SHARED", "REDUCE", "VOUT", "ITER", "KERNEL", "RESTRICT", "AUTO",
  "EXTRN", "REGISTR", "STATIC", "TYPEDEF", "VOID", "CHAR", "CHAR2",
  "CHAR3", "CHAR4", "SHORT", "SHORT2", "SHORT3", "SHORT4", "INT", "INT2",
  "INT3", "INT4", "LONG", "SGNED", "UNSGNED", "GCC_BUILTIN", "FLOAT",
  "FLOAT2", "FLOAT3", "FLOAT4", "DOUBLE", "DOUBLE2", "ENUM", "STRUCT",
  "UNION", "BREAK", "CASE", "CONT", "DEFLT", "DO", "ELSE", "IF", "FOR",
  "GOTO", "RETURN", "SWITCH", "WHILE", "TARGET", "ASM_SCRATCH", "ASM_READ",
  "ASM_WRITE", "PLUS_EQ", "MINUS_EQ", "STAR_EQ", "DIV_EQ", "MOD_EQ",
  "B_AND_EQ", "B_OR_EQ", "B_XOR_EQ", "L_SHIFT_EQ", "R_SHIFT_EQ", "EQUAL",
  "LESS_EQ", "GRTR_EQ", "NOT_EQ", "RPAREN", "RBRCKT", "LBRACE", "RBRACE",
  "SEMICOLON", "COMMA", "ELLIPSIS", "LB_SIGN", "DOUB_LB_SIGN", "BACKQUOTE",
  "AT", "CONSTRUCTOR_FLOAT", "CONSTRUCTOR_DOUBLE", "CONSTRUCTOR_CHAR",
  "CONSTRUCTOR_UCHAR", "CONSTRUCTOR_SHORT", "CONSTRUCTOR_USHORT",
  "CONSTRUCTOR_INT", "CONSTRUCTOR_UINT", "ATTRIBUTE", "KERNEL_ATTRIBUTE",
  "ALIGNED", "PACKED", "CDECL", "MODE", "FORMAT", "NORETURN", "NOTHROW",
  "NONNULL", "PURE", "GROUPSIZE", "UNKNOWN_GCC_ATTRIBUTE",
  "UNKNOWN_KERNEL_ATTRIBUTE", "MALLOC", "WARN_UNUSED_RESULT", "COMMA_OP",
  "ASSIGN", "EQ", "COMMA_SEP", "COLON", "QUESTMARK", "OR", "AND", "B_OR",
  "B_XOR", "B_AND", "COMP_EQ", "GRTR", "LESS", "COMP_GRTR", "COMP_LESS",
  "COMP_ARITH", "R_SHIFT", "L_SHIFT", "MINUS", "PLUS", "MOD", "DIV",
  "STAR", "CAST", "DECR", "INCR", "SYNC_GROUP", "INSTANCE_IN_GROUP",
  "INSTANCE", "INDEXOF", "SIZEOF", "B_NOT", "NOT", "UNARY", "HYPERUNARY",
  "LBRCKT", "LPAREN", "DOT", "ARROW", "$accept", "program", "trans_unit",
  "top_level_exit", "top_level_decl", "func_def", "func_spec",
  "cmpnd_stemnt", "@1", "opt_stemnt_list", "stemnt_list", "stemnt_list2",
  "stemnt", "cmpnd_stemnt_reentrance", "opt_stemnt_list_reentrance",
  "stemnt_list_reentrance", "stemnt_list_reentrance2", "stemnt_reentrance",
  "non_constructor_stemnt", "constructor_stemnt", "expr_stemnt",
  "labeled_stemnt", "cond_stemnt", "iter_stemnt", "switch_stemnt",
  "break_stemnt", "continue_stemnt", "return_stemnt", "goto_stemnt",
  "null_stemnt", "if_stemnt", "if_else_stemnt", "asm_stemnt", "do_stemnt",
  "while_stemnt", "for_stemnt", "label", "named_label", "case_label",
  "deflt_label", "cond_expr", "assign_expr", "opt_const_expr",
  "const_expr", "opt_expr", "expr", "log_or_expr", "log_and_expr",
  "log_neg_expr", "bitwise_or_expr", "bitwise_xor_expr",
  "bitwise_and_expr", "bitwise_neg_expr", "cast_expr", "equality_expr",
  "relational_expr", "shift_expr", "additive_expr", "mult_expr",
  "constructor_op2", "constructor_op3", "constructor_op4",
  "constructor_expr", "const_constructor_expr", "iter_constructor_arg",
  "iter_constructor_expr", "unary_expr", "sizeof_expr", "indexof_expr",
  "instance_expr", "instance_in_group_expr", "sync_group_expr",
  "unary_minus_expr", "unary_plus_expr", "addr_expr", "indirection_expr",
  "preinc_expr", "predec_expr", "comma_expr", "prim_expr", "paren_expr",
  "postfix_expr", "subscript_expr", "comp_select_expr", "postinc_expr",
  "postdec_expr", "field_ident", "direct_comp_select",
  "indirect_comp_select", "func_call", "opt_expr_list", "expr_list",
  "add_op", "mult_op", "equality_op", "relation_op", "shift_op",
  "assign_op", "constant", "opt_KnR_declaration_list", "@2",
  "opt_declaration_list", "@3", "opt_declaration_list_actual", "@4",
  "declaration_list", "declaration_single", "decl_stemnt",
  "old_style_declaration", "declaration", "no_decl_specs", "decl_specs",
  "abs_decl", "type_name", "@5", "type_name_bis",
  "decl_specs_reentrance_bis", "local_or_global_storage_class",
  "local_storage_class", "storage_class", "type_spec",
  "opt_decl_specs_reentrance", "decl_specs_reentrance", "@6",
  "opt_comp_decl_specs", "comp_decl_specs_reentrance", "@7",
  "comp_decl_specs", "decl", "@8", "init_decl", "opt_init_decl_list",
  "init_decl_list", "init_decl_list_reentrance", "initializer",
  "initializer_list", "initializer_reentrance", "opt_comma", "type_qual",
  "type_qual_token", "type_qual_list", "opt_type_qual_list",
  "type_spec_reentrance", "typedef_name", "tag_ref", "struct_tag_ref",
  "union_tag_ref", "enum_tag_ref", "struct_tag_def", "struct_type_define",
  "union_tag_def", "union_type_define", "enum_tag_def", "enum_type_define",
  "struct_or_union_definition", "enum_definition", "opt_trailing_comma",
  "enum_def_list", "enum_def_list_reentrance", "enum_const_def",
  "enum_constant", "field_list", "@9", "@10", "field_list_reentrance",
  "comp_decl", "comp_decl_list", "comp_decl_list_reentrance", "@11", "@12",
  "comp_declarator", "simple_comp", "bit_field", "@13", "width",
  "opt_declarator", "declarator", "func_declarator",
  "declarator_reentrance_bis", "direct_declarator_reentrance_bis",
  "direct_declarator_reentrance", "array_decl", "stream_decl",
  "dimension_constraint", "comma_constants", "pointer_start",
  "pointer_reentrance", "pointer", "ident_list", "@14",
  "ident_list_reentrance", "ident", "typename_as_ident", "any_ident",
  "opt_param_type_list", "@15", "param_type_list", "@16",
  "param_type_list_bis", "param_list", "param_decl", "@17",
  "param_decl_bis", "abs_decl_reentrance",
  "direct_abs_decl_reentrance_bis", "direct_abs_decl_reentrance",
  "opt_gcc_attrib", "gcc_attrib", "gcc_attrib_single", "gcc_inner",
  "kernel_attrib_single", "kernel_inner", "inum_list", 0
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   162,   163,   163,   163,   164,   164,   165,   166,   166,
     166,   166,   166,   166,   167,   167,   168,   168,   170,   169,
     169,   171,   171,   172,   173,   173,   173,   174,   175,   175,
     176,   176,   177,   177,   178,   178,   178,   178,   178,   179,
     179,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   181,   182,   183,   184,   184,   185,   185,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   198,   198,   199,   200,   200,   201,
     202,   202,   203,   203,   203,   204,   204,   205,   206,   206,
     207,   208,   208,   209,   209,   210,   211,   211,   212,   212,
     213,   213,   214,   215,   215,   216,   216,   217,   217,   218,
     218,   219,   219,   220,   220,   221,   221,   221,   221,   221,
     222,   222,   222,   222,   223,   223,   223,   223,   224,   224,
     224,   224,   224,   224,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   227,   227,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   229,   229,
     230,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   240,   241,   241,   241,   241,   242,   242,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   244,
     245,   245,   246,   247,   248,   249,   250,   251,   252,   252,
     253,   253,   253,   253,   254,   254,   255,   255,   255,   256,
     257,   257,   257,   258,   258,   259,   259,   260,   260,   260,
     260,   260,   260,   261,   262,   261,   264,   263,   265,   266,
     265,   267,   267,   267,   268,   269,   269,   270,   271,   272,
     273,   274,   276,   275,   277,   277,   278,   279,   279,   279,
     280,   280,   281,   281,   282,   283,   283,   284,   285,   284,
     284,   286,   286,   288,   287,   287,   289,   291,   290,   292,
     292,   293,   293,   294,   295,   295,   296,   297,   297,   298,
     298,   298,   298,   299,   299,   300,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   302,   302,   303,   303,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   305,
     306,   307,   308,   309,   310,   311,   311,   312,   313,   313,
     314,   315,   315,   316,   316,   316,   317,   317,   318,   318,
     319,   320,   320,   321,   321,   322,   324,   325,   323,   326,
     326,   326,   326,   327,   327,   328,   330,   329,   331,   329,
     332,   332,   333,   335,   334,   336,   337,   337,   338,   339,
     340,   340,   341,   342,   342,   342,   342,   342,   342,   342,
     343,   344,   345,   345,   345,   345,   346,   346,   346,   347,
     348,   348,   349,   351,   350,   352,   352,   353,   354,   355,
     355,   356,   357,   356,   359,   358,   360,   360,   361,   361,
     363,   362,   364,   364,   364,   365,   365,   365,   366,   367,
     367,   367,   367,   367,   368,   368,   369,   369,   370,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   372,   373,   373,   374,   374
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     1,     1,     2,     3,     0,     1,     1,
       1,     1,     2,     3,     3,     2,     3,     3,     0,     5,
       2,     0,     1,     2,     0,     2,     2,     1,     4,     2,
       0,     1,     2,     1,     1,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     3,     1,     1,     1,     1,
       1,     5,     2,     2,     3,     3,     1,     5,     7,     1,
       7,     5,     9,     1,     1,     1,     1,     2,     4,     1,
       1,     5,     1,     3,     3,     0,     1,     1,     0,     1,
       1,     1,     3,     1,     3,     2,     1,     3,     1,     3,
       1,     3,     2,     1,     4,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     8,
      10,     4,     4,     4,     5,     7,     9,     5,     5,     7,
       9,     5,     7,     9,     5,     7,     9,     5,     7,     9,
       5,     7,     9,     5,     7,     9,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     6,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     2,
       2,     4,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     1,     3,     1,     1,     1,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     1,     2,     2,     1,     3,     3,     4,     0,     1,
       1,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     0,     2,     0,     2,     0,     0,
       2,     1,     2,     3,     2,     2,     2,     2,     2,     0,
       1,     1,     0,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     2,     0,     3,
       2,     0,     1,     0,     3,     2,     1,     0,     3,     1,
       3,     0,     1,     1,     1,     3,     1,     1,     3,     1,
       1,     1,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     4,     4,     2,     4,     4,
       2,     4,     4,     0,     1,     2,     0,     2,     0,     1,
       1,     1,     3,     1,     3,     1,     0,     0,     3,     2,
       2,     3,     2,     2,     1,     1,     0,     3,     0,     5,
       1,     1,     1,     0,     4,     1,     0,     1,     1,     1,
       2,     1,     1,     1,     3,     1,     1,     4,     4,     3,
       4,     4,     1,     1,     3,     1,     0,     1,     3,     2,
       1,     2,     1,     0,     2,     1,     3,     1,     1,     1,
       1,     0,     0,     2,     0,     2,     1,     3,     1,     3,
       0,     2,     2,     2,     1,     1,     1,     2,     1,     3,
       3,     4,     3,     4,     0,     1,     1,     2,     6,     1,
       1,     1,     1,     1,     1,     4,     4,     8,     4,     1,
       1,     1,     4,     1,     4,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,     4,   349,    10,    11,   306,   307,   308,   309,   310,
       0,   311,   312,   313,   270,   267,   271,   268,   269,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   340,   347,   348,   339,   341,   342,   343,   344,
     345,   346,     0,     0,     0,     0,     0,     0,     7,     9,
       0,     8,     0,     0,   291,   291,   260,   272,   273,   275,
     278,   266,   275,   305,   274,   325,   323,   324,   322,     0,
     320,     0,   321,     0,   319,   259,     7,    12,    85,   350,
     366,   353,   376,   351,   376,   352,     0,     1,     0,     7,
       5,     0,    18,    15,   255,   256,   427,   317,     0,   289,
     294,   257,   292,   293,   244,   398,   401,   402,   405,   406,
     420,   422,     0,   403,   258,   287,   244,   399,   277,   276,
     275,   280,   376,   376,   366,     0,     0,     0,    13,   241,
     242,   239,   240,   237,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,   191,     0,
      86,    87,    80,    91,   172,    93,    96,    98,   173,   113,
     100,   105,   107,   109,   111,   196,   103,   169,   205,   206,
     207,   208,   170,   171,   174,   175,   176,   177,    90,   199,
     194,   168,   200,   201,   203,   204,   210,   211,   202,   195,
     193,   428,     0,   368,   370,   371,   373,   429,   430,   375,
     376,     0,   364,   377,     0,     0,   473,     0,     6,    20,
     246,   315,   318,   419,     0,     0,     0,    17,     0,   454,
     416,    85,   434,   421,   400,    16,   279,     0,     0,     0,
      14,   244,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   187,
     103,   185,   186,   188,     0,   190,   189,     0,     0,     0,
       0,   180,     0,   179,   102,    95,     0,     0,     0,   264,
     314,     0,     0,     0,     0,     0,     0,   229,     0,   232,
     231,   230,     0,   234,   233,     0,   225,   224,     0,   228,
     227,   226,     0,   236,   235,     0,     0,   213,   212,     0,
     218,     0,     0,   361,   369,   367,     0,   365,   355,     0,
     358,     0,   472,     0,   248,   316,   404,     0,   118,   122,
     126,   119,   123,   127,   116,   121,   125,   115,   120,   124,
     117,     0,   299,     0,     0,     0,   300,   301,   290,   296,
     295,     0,   251,     0,   291,     0,   288,   455,   456,     0,
       0,   202,   195,   417,     0,   193,     0,   409,     0,     0,
       0,   440,   356,   359,   362,   156,     0,   157,     0,   158,
       0,   159,     0,   160,     0,   161,     0,   162,     0,   163,
       0,   184,   183,   182,     0,     0,     0,   198,   197,     0,
      85,   432,   265,   445,   261,   446,   448,     0,    92,    94,
      97,    99,   101,   106,   108,   110,   112,   114,    83,    84,
     192,     0,   220,   221,     0,   219,   215,   214,   216,   372,
     374,   286,   386,   281,   283,   378,     0,   475,     0,     0,
      69,     0,     0,     0,    79,     0,     0,     0,     0,    88,
       0,     0,   246,    66,     0,    22,    43,    24,    41,    42,
      44,    46,    47,    48,    49,    50,    51,    52,    56,    57,
      45,    58,    59,    60,     0,    73,    74,    75,     0,   193,
     247,     0,     0,   303,   297,     0,     0,     0,     0,   252,
     254,     0,   457,     0,     0,   411,   410,   408,   424,   425,
     407,   435,   436,   438,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   178,   263,   104,     0,     0,   440,
       0,   447,    85,   432,     0,   209,   217,     0,   383,   385,
     396,   285,   282,   281,   382,   380,     0,   379,   474,     0,
      29,    62,    77,    63,     0,    39,    40,     0,     0,    88,
       0,     0,    89,     0,     0,     0,    19,     0,     0,    54,
       0,     0,   164,   165,     0,   304,     0,     0,     0,     0,
       0,     0,     0,   253,     0,   197,   418,     0,   440,   444,
     441,   134,     0,   137,   150,     0,   153,     0,   144,     0,
     147,     0,   138,     0,   141,     0,   450,   452,   433,   449,
       0,     0,    81,   222,   223,   388,   454,   390,   391,     0,
     392,   284,   381,   476,     0,     0,    53,     0,     0,    65,
      64,     0,     0,     0,    31,     0,    26,    25,    27,    55,
     167,     0,   298,   302,   131,     0,   132,     0,   133,     0,
     462,     0,   460,   461,     0,     0,   463,   464,     0,   469,
     459,   470,   471,     0,   426,   437,   439,   432,   442,   445,
     443,     0,     0,     0,     0,     0,     0,     0,   451,   453,
     396,   387,   393,    78,     0,     0,    88,     0,     0,    28,
      35,     0,    34,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   135,     0,   151,     0,   154,     0,   145,     0,
     148,     0,   139,     0,   142,     0,   454,     0,     0,    67,
       0,    61,    71,     0,    37,    36,   166,   128,     0,     0,
       0,     0,     0,     0,   458,     0,     0,     0,     0,     0,
       0,     0,   389,   395,   394,     0,     0,    88,    38,     0,
       0,   465,   466,     0,   468,   136,   152,   155,   146,   149,
     140,   143,    70,    68,     0,   129,     0,     0,     0,     0,
       0,    72,   130,     0,   467
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    46,    47,    90,    48,    49,    50,    93,   220,   454,
     455,   557,   627,   456,   623,   624,   681,   544,   545,   546,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     157,   158,   159,   160,   551,   478,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   343,
     344,   345,   547,   175,   564,   347,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   426,   196,   197,   198,
     424,   425,   298,   302,   288,   292,   295,   305,   199,   227,
     228,   323,   324,   480,   481,   351,   352,    51,    52,   353,
      54,   354,   402,   395,   396,   278,    56,    57,    58,    59,
      60,   118,    61,   120,   531,   431,   533,   432,    99,   229,
     100,   114,   102,   103,   348,   483,   349,   566,    62,    63,
     222,   223,    64,    65,    81,    66,    67,    68,    69,    70,
      71,    72,    73,    74,   211,   202,   315,   203,   204,   205,
     206,   212,   213,   319,   435,   436,   528,   529,   530,   670,
     606,   607,   608,   707,   734,   609,   115,   116,   105,   106,
     107,   108,   109,   363,   364,   110,   111,   112,   368,   369,
     498,   200,   208,   209,   518,   519,   370,   371,   501,   502,
     503,   504,   580,   520,   405,   406,   356,   357,   358,   653,
      75,   217,   438
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -562
static const short int yypact[] =
{
     715,    12,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
     -89,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,    49,    50,    51,   -80,   158,   858,  -562,  -562,
      48,  -562,    71,    82,    34,    34,  -562,  -562,  -562,  3458,
    -562,  -562,  3458,  -562,  -562,  -562,  -562,  -562,  -562,    97,
    -562,   117,  -562,   139,  -562,  3458,  -562,  -562,  3066,  -562,
      73,   161,    45,   170,    45,   193,    17,  -562,    12,  -562,
    -562,   155,  -562,  -562,  -562,  -562,  -562,   396,    34,   171,
    -562,  -562,  -562,   129,   120,  -562,  -562,   -69,  -562,  -562,
    -562,   102,    38,  -562,  -562,  -562,    55,    98,  -562,  -562,
    3458,  -562,    45,    45,    73,    48,    34,    34,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,   676,   831,  2181,  2286,  2348,
    2412,  2474,  2538,  3066,  3066,  3066,  3066,  3090,  3090,   138,
     142,   143,    44,  3169,  3066,  3066,  1156,  -562,  -562,   208,
    -562,  -562,     3,   169,  -562,   173,   168,   172,  -562,  -562,
     176,   137,    26,    83,   149,  -562,   132,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,   213,  -562,
    -562,   118,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,   220,   219,  -562,  -562,   188,  -562,  -562,  -562,
    -562,   223,  -562,  -562,   224,   159,  -562,   231,  -562,  -562,
    -562,  -562,   396,  -562,   233,  2913,    34,  -562,  3458,   215,
     227,  3066,    59,  -562,  -562,  -562,  -562,   232,   243,   244,
    -562,    55,  -562,   236,   256,   248,   260,   249,   262,   269,
     266,   273,   272,   280,   276,   282,   278,   284,   283,  -562,
    -562,  -562,  -562,  -562,  2600,  -562,  -562,   285,   287,   293,
     378,  -562,  1311,  -562,  -562,  -562,   295,   296,   297,   -74,
    -562,  3066,  3066,  3066,  3066,  3066,  3066,  -562,  3066,  -562,
    -562,  -562,  3066,  -562,  -562,  3066,  -562,  -562,  3066,  -562,
    -562,  -562,  3066,  -562,  -562,  2986,  3066,  -562,  -562,  3066,
    2986,    73,    73,  -562,    73,  -562,  3066,  -562,  -562,  3560,
    -562,   375,  -562,  2008,  3509,  -562,  -562,   234,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  2913,  -562,   235,   237,   239,  -562,  -562,  -562,  -562,
    -562,  3324,  -562,   299,    34,   240,  -562,   215,  -562,  2600,
     118,   -28,   -27,  -562,   -11,   -10,   300,  -562,   305,   378,
     313,  -562,  -562,  -562,  -562,  -562,  3066,  -562,  3066,  -562,
    3066,  -562,  3066,  -562,  3066,  -562,  3066,  -562,  3066,  -562,
    3066,  -562,  -562,  -562,   316,   321,  3458,  -562,  -562,  3066,
    3066,    36,  -562,    24,  -562,  -562,   103,   298,   169,   173,
     168,   172,   176,   137,    26,    83,   149,  -562,  -562,  -562,
    -562,   322,  -562,  -562,   324,   317,  -562,  -562,  -562,  -562,
    -562,  -562,   331,  3560,  -562,  3383,   332,  -562,     1,   335,
    -562,   339,  3066,   343,  -562,  1903,   267,   279,   434,  3066,
     289,   290,  -562,  -562,   351,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,   325,  -562,  -562,  -562,   359,   326,
    -562,  3458,   394,   350,  -562,  2664,  2726,  2790,   362,  -562,
    -562,   304,  -562,   367,   227,  -562,  -562,  -562,   363,  -562,
    -562,  -562,   364,  -562,  3458,     5,   371,     7,     8,     9,
      41,    69,    77,  -562,  -562,  -562,  -562,   376,   372,  -562,
     379,  -562,  3066,   380,  3066,  -562,  -562,  2986,  -562,   373,
      34,  -562,  -562,  3560,  -562,  -562,   377,  -562,  -562,   457,
    -562,  -562,   381,  -562,   402,  -562,  -562,   384,  3066,  3066,
     389,   390,  -562,  3066,  3066,  2113,  -562,  1466,  1903,  -562,
    1001,   395,  -562,  -562,   391,  2913,   397,   398,   392,   399,
     408,   401,   413,  -562,   229,    -9,  -562,   378,   414,    40,
    -562,  -562,  3066,  -562,  -562,  3066,  -562,  3066,  -562,  3066,
    -562,  3066,  -562,  3066,  -562,  3066,  -562,  -562,  -562,  -562,
     383,   403,  -562,  -562,  -562,  -562,   215,  -562,  -562,   365,
     382,  -562,  -562,  -562,  3066,   330,  -562,   419,   418,  -562,
    -562,   423,   424,   422,  -562,  1621,  -562,  -562,  -562,  -562,
    -562,  2986,  -562,  -562,  -562,  3066,  -562,  3066,  -562,  3066,
    -562,   354,  -562,  -562,   355,   356,  -562,  -562,   357,  -562,
    -562,  -562,  -562,   430,  -562,  -562,  -562,    33,  -562,    43,
    -562,    88,   121,   123,   127,   131,   140,   144,  -562,  -562,
      34,  -562,  -562,  -562,  3066,  1903,  3066,  1903,  1903,  -562,
    -562,  1776,  -562,   431,   432,   428,   429,   510,   378,   378,
     375,   437,  -562,  3066,  -562,  3066,  -562,  3066,  -562,  3066,
    -562,  3066,  -562,  3066,  -562,  3066,   215,  3066,   440,   467,
     439,  -562,  -562,   177,  -562,  -562,  -562,  -562,  3066,  3066,
     444,   445,   441,   156,  -562,   447,   450,   463,   465,   468,
     469,   471,  -562,  -562,  -562,   470,  1903,  3066,  -562,   472,
     462,  -562,  -562,   551,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,   478,  -562,  3066,   475,  1903,   482,
     561,  -562,  -562,   487,  -562
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -562,  -562,  -562,    -2,   528,  -562,   501,   452,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -507,  -302,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -490,   -25,  -216,  -407,  -517,   -76,  -562,   301,  -562,   302,
     294,   303,  -562,  -115,   306,   291,   288,   286,   292,  -562,
    -562,  -562,  -219,  -562,   -49,  -562,  -135,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,
    -562,  -208,  -562,  -562,  -562,  -562,   274,  -562,  -562,  -207,
    -562,  -562,  -562,  -562,  -562,  -562,  -562,  -562,  -206,   473,
    -562,   135,  -562,  -562,  -562,   110,  -318,  -562,  -562,    60,
     509,    57,  -562,  -562,  -562,   197,  -142,  -562,  -562,  -562,
    -562,    -1,     4,  -562,    61,  -391,  -562,  -562,  -562,  -562,
     369,   542,  -562,  -562,  -562,  -562,  -316,  -562,  -292,   -45,
    -562,  -562,  -281,  -562,   246,  -562,  -562,  -562,  -562,  -562,
    -562,  -562,  -562,  -562,   -17,   477,  -562,  -562,  -562,   308,
    -562,   387,  -562,  -562,  -562,   164,  -562,  -562,  -562,  -562,
     -68,  -562,  -562,  -562,  -562,  -562,   -51,  -562,   -50,  -108,
    -562,  -562,  -562,   114,  -562,   493,  -562,  -272,  -562,  -562,
    -562,   -54,  -562,   -41,    86,  -562,  -562,  -562,    92,  -562,
      35,  -562,  -562,  -262,  -387,  -562,  -561,  -562,   258,  -562,
    -562,  -562,   -73
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -432
static const short int yytable[] =
{
     113,   113,   161,   104,   234,   117,   346,   403,   260,   260,
     260,   260,   265,   266,   279,   366,   521,   404,   273,   260,
     260,   457,   360,   361,   362,   484,   207,   433,   259,   261,
     262,   263,   618,   489,   602,   542,    96,    96,   434,   274,
     275,    96,   532,    96,   113,   671,    96,    96,   224,    91,
     628,   629,   221,    79,    79,    79,  -243,    55,   113,   210,
      53,   121,  -423,   119,  -413,  -412,   119,   214,   230,    78,
     207,    97,   113,   113,   128,   241,    96,   242,    86,   201,
     277,   494,  -415,  -414,   400,   401,   419,   218,   538,   231,
     232,   423,   581,   539,   584,   586,   588,   582,   271,   585,
     587,   589,    76,    77,    55,   237,   238,    53,  -413,  -412,
     244,   246,   248,   250,   252,   254,   256,   258,   682,   236,
    -431,  -243,   346,  -431,   119,   495,  -415,  -414,   590,   403,
     281,   282,   127,   591,   215,  -363,   216,    92,    80,    82,
      84,   433,   532,   433,  -243,   732,   367,   260,   260,   260,
     260,   260,   434,   260,   434,   161,   592,   260,    87,   710,
     260,   593,    94,   260,   594,   293,   294,   260,   709,   595,
     711,   712,   113,    95,   715,   692,   365,   325,    97,    97,
     693,    97,   400,   401,   517,    97,   122,   417,   277,  -398,
    -398,   400,   657,    98,   400,   401,   277,    98,   400,   657,
     342,   400,   657,   270,  -398,   407,   123,   673,   694,  -243,
     696,  -287,  -287,   695,   698,   697,   394,   733,   700,   699,
     754,   226,  -398,   701,   296,   297,  -287,   702,   124,   753,
      96,   704,   703,   421,   129,   130,   705,   131,   132,   133,
     134,   433,   489,   744,  -287,   219,   640,    97,   539,   632,
    -360,   761,   434,   625,   279,   303,   304,   207,   207,  -354,
     207,   522,   523,   563,   260,   307,   308,   540,   738,   479,
     427,   427,   521,   289,   290,   291,   309,   310,   311,   312,
     418,   420,  -357,   493,   516,   422,   360,   361,   362,    83,
      85,   430,   299,   300,   301,   225,   280,   267,   283,   285,
     113,   268,   269,   284,   286,   306,   600,   659,   604,   287,
     313,   314,   316,   318,   320,   499,   342,   660,   321,   322,
     326,   355,   372,   375,   161,   135,   136,   137,   138,   139,
     140,   141,   142,   373,   374,   377,   379,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   346,   650,   376,   651,
     652,   505,   378,   506,   380,   507,   381,   508,   382,   509,
     383,   510,   579,   511,   384,   512,   161,   385,   386,   387,
     388,   389,   391,   552,   392,   390,   149,   150,   151,   152,
     393,    96,   397,   398,   399,   659,   359,   437,   496,   260,
     490,   479,   497,   482,   485,   561,   486,    96,   487,   491,
     500,   129,   130,   513,   131,   132,   133,   134,   514,   527,
     525,   526,   563,     5,     6,     7,     8,     9,    10,    11,
      12,    13,  -384,   537,   524,   540,   548,   328,   329,   330,
     541,   331,   332,   333,   543,   334,   335,   336,   549,   550,
     365,   556,   565,   337,   338,   339,   161,   340,   553,   554,
     559,   558,   -76,   573,   575,   577,   578,   562,   583,   597,
     568,   570,   572,   574,   596,   605,   599,  -431,   612,   613,
     615,   668,   617,   552,   614,   616,   113,   621,   622,   610,
     619,   620,   630,   631,   635,   634,   636,   633,   638,   674,
     669,   672,   135,   136,   137,   138,   139,   140,   141,   142,
     637,   479,   603,   479,   479,   639,   675,   655,  -397,   676,
     677,   678,   679,   687,   688,   689,   690,   691,   716,   717,
     718,   719,   720,   654,   724,   113,   143,   735,   658,   736,
     737,   741,   742,   743,   745,   144,   145,   746,   161,   146,
     342,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     747,   234,   748,   156,   756,   749,   750,   661,   751,   755,
     662,   752,   663,   757,   664,   758,   665,   760,   666,   762,
     667,   479,   260,   763,   764,    89,   125,   240,   410,   413,
     414,   415,   683,   408,   126,   409,   428,   555,   411,   235,
     416,   560,   412,   515,   611,   350,   101,   317,   708,   536,
     552,   239,   706,   113,   233,   113,   562,   224,   576,   601,
     684,   598,   685,   656,   686,   492,   113,   723,     0,   610,
       0,   479,   429,   479,   479,     0,     0,   479,     0,     0,
       0,     0,     0,     0,   721,   722,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   552,     0,     0,     0,     0,     0,     0,   725,     0,
     726,     0,   727,     0,   728,     0,   729,   243,   730,    96,
     731,     0,   479,   129,   130,     0,   131,   132,   133,   134,
       0,     0,     0,   739,   740,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    -2,     1,     0,  -259,     0,
       0,     2,     0,     0,     0,     0,     0,     0,     0,     3,
       4,   759,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,     0,     0,   135,   136,   137,   138,   139,   140,
     141,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -259,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,     0,
       0,   146,    45,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   245,     0,    96,   156,     0,     0,   129,   130,
       0,   131,   132,   133,   134,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    -3,    88,
    -259,  -259,     0,     0,     2,     0,     0,     0,     0,     0,
       0,     0,     3,     4,  -259,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
     136,   137,   138,   139,   140,   141,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -259,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,     0,    45,     0,     0,     0,     0,
       0,     0,   144,   145,     0,     0,   146,     0,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     0,     0,     0,
     156,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   488,  -259,  -250,     0,     0,     2,  -250,  -250,
    -250,  -250,  -250,  -250,  -250,     0,     0,  -259,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,  -250,  -250,
    -250,  -250,  -250,     0,  -250,  -250,  -250,  -250,  -250,  -250,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -250,  -250,  -250,     0,     0,     0,     0,     0,     0,  -250,
    -250,  -250,  -250,  -250,  -250,  -250,  -250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -250,     0,     0,     0,     0,     0,     0,
       0,     0,  -250,  -250,     0,     0,  -250,     0,  -250,  -250,
    -250,  -250,  -250,  -250,  -250,  -250,  -250,   276,     0,    96,
    -250,     0,     2,   129,   130,     0,   131,   132,   133,   134,
       0,     0,     0,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,     0,
       0,   146,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   276,     0,    96,   156,     0,  -262,   129,   130,
       0,   131,   132,   133,   134,     0,     0,     0,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,
    -262,  -262,  -262,  -262,  -262,  -262,  -262,  -262,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   135,
     136,   137,   138,   139,   140,   141,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   144,   145,     0,     0,   146,     0,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   439,     0,    96,
     156,     0,     0,   129,   130,   440,   131,   132,   133,   134,
       0,   626,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
       0,     0,     0,     0,     0,   337,   338,   339,     0,   340,
       0,     0,     0,   441,   442,   443,   444,   445,     0,   446,
     447,   448,   449,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   452,   -23,   453,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,     0,
       0,   146,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   439,     0,    96,   156,     0,     0,   129,   130,
     440,   131,   132,   133,   134,     0,   680,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   328,   329,   330,     0,   331,   332,
     333,     0,   334,   335,   336,     0,     0,     0,     0,     0,
     337,   338,   339,     0,   340,     0,     0,     0,   441,   442,
     443,   444,   445,     0,   446,   447,   448,   449,   450,   451,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     452,   -33,   453,     0,     0,     0,     0,     0,     0,   135,
     136,   137,   138,   139,   140,   141,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   144,   145,     0,     0,   146,     0,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   713,     0,    96,
     156,     0,     0,   129,   130,   440,   131,   132,   133,   134,
       0,   714,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
       0,     0,     0,     0,     0,   337,   338,   339,     0,   340,
       0,     0,     0,   441,   442,   443,   444,   445,     0,   446,
     447,   448,   449,   450,   451,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   452,   -32,   453,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   439,     0,    96,     0,   143,     0,
     129,   130,   440,   131,   132,   133,   134,   144,   145,     0,
       0,   146,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,   156,   328,   329,   330,     0,
     331,   332,   333,     0,   334,   335,   336,     0,     0,     0,
       0,     0,   337,   338,   339,     0,   340,     0,     0,     0,
     441,   442,   443,   444,   445,     0,   446,   447,   448,   449,
     450,   451,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   452,     0,   453,     0,     0,     0,     0,     0,
       0,   135,   136,   137,   138,   139,   140,   141,   142,   439,
       0,    96,     0,     0,     0,   129,   130,   440,   131,   132,
     133,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   144,   145,     0,     0,   146,     0,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     0,
       0,     0,   156,     0,     0,   441,   442,   443,   444,   445,
       0,   446,   447,   448,   449,   450,   451,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   452,   -21,   453,
       0,     0,     0,     0,     0,     0,   135,   136,   137,   138,
     139,   140,   141,   142,   439,     0,    96,     0,     0,     0,
     129,   130,   440,   131,   132,   133,   134,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     143,     0,     0,     0,     0,     0,     0,     0,     0,   144,
     145,     0,     0,   146,     0,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,     0,   156,     0,     0,
     441,   442,   443,   444,   445,     0,   446,   447,   448,   449,
     450,   451,   247,     0,    96,     0,     0,     0,   129,   130,
       0,   131,   132,   133,   134,     0,     0,     0,     0,     0,
       0,     0,   452,   -30,   453,     0,     0,     0,     0,     0,
       0,   135,   136,   137,   138,   139,   140,   141,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   144,   145,     0,     0,   146,     0,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     0,
       0,     0,   156,     0,     0,     0,     0,     0,     0,   135,
     136,   137,   138,   139,   140,   141,   142,   249,     0,    96,
       0,     0,     0,   129,   130,     0,   131,   132,   133,   134,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   143,     0,     0,     0,     0,     0,     0,
       0,     0,   144,   145,     0,     0,   146,     0,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     0,     0,     0,
     156,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,    96,     0,     0,     0,   129,   130,     0,   131,   132,
     133,   134,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   253,     0,    96,     0,     0,   143,   129,
     130,     0,   131,   132,   133,   134,     0,   144,   145,     0,
       0,   146,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,   156,   135,   136,   137,   138,
     139,   140,   141,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   255,     0,    96,     0,     0,
     143,   129,   130,     0,   131,   132,   133,   134,     0,   144,
     145,     0,     0,   146,     0,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,     0,   156,     0,     0,
     135,   136,   137,   138,   139,   140,   141,   142,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   257,
       0,    96,     0,     0,   143,   129,   130,     0,   131,   132,
     133,   134,     0,   144,   145,     0,     0,   146,     0,   147,
     148,   149,   150,   151,   152,   153,   154,   155,     0,     0,
       0,   156,   135,   136,   137,   138,   139,   140,   141,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   276,     0,    96,     0,     0,   143,   129,   130,     0,
     131,   132,   133,   134,     0,   144,   145,     0,     0,   146,
       0,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,   156,     0,     0,   135,   136,   137,   138,
     139,   140,   141,   142,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,     0,    96,     0,     0,
     143,   129,   130,     0,   131,   132,   133,   134,     0,   144,
     145,     0,     0,   146,     0,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,     0,   156,   135,   136,
     137,   138,   139,   140,   141,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   569,     0,    96,
       0,     0,   143,   129,   130,     0,   131,   132,   133,   134,
       0,   144,   145,     0,     0,   146,     0,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,     0,   156,
       0,     0,   135,   136,   137,   138,   139,   140,   141,   142,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,     0,    96,     0,     0,   143,   129,   130,     0,
     131,   132,   133,   134,     0,   144,   145,     0,     0,   146,
       0,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,     0,     0,   156,   135,   136,   137,   138,   139,   140,
     141,   142,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,     0,
       0,   146,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,   156,     0,     0,   135,   136,
     137,   138,   139,   140,   141,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,     0,     0,     0,
     129,   130,   143,   131,   132,   133,   134,     0,     0,     0,
       0,   144,   145,     0,     0,   146,   327,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   328,   329,   330,   156,
     331,   332,   333,     0,   334,   335,   336,     0,     0,     0,
       0,     0,   337,   338,   339,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
       0,     0,     0,   129,   130,     0,   131,   132,   133,   134,
       0,     0,   341,     0,     0,     0,     0,     0,     0,     0,
       0,   135,   136,   137,   138,   139,   140,   141,   142,   328,
     329,   330,     0,   331,   332,   333,     0,   334,   335,   336,
       0,     0,     0,     0,     0,   337,   338,   339,     0,   340,
       0,     0,     0,     0,     0,   143,     0,     0,     0,     0,
       0,     0,     0,     0,   144,   145,     0,     0,   146,     0,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    96,
       0,     0,   156,   129,   130,     0,   131,   132,   133,   134,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,   142,     0,    96,     0,     0,     0,   129,   130,     0,
     131,   132,   133,   134,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,     0,
       0,   146,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     0,     0,     0,   156,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   135,   136,   137,   138,   139,   140,
     141,   142,    96,     0,     0,     0,   129,   130,     0,   131,
     132,   133,   134,     0,     0,     0,     0,     0,   135,   136,
     137,   138,   139,   140,   141,   142,     0,     0,   143,     0,
       0,     0,     0,     0,     0,     0,     0,   144,   145,     0,
       0,   146,     0,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   143,     0,     0,   156,     0,     0,     0,     0,
       0,   144,   145,     0,     0,   146,     0,   147,   148,   149,
     150,   151,   152,   153,   154,   155,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   135,   136,   137,
     138,   139,   140,   141,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,     0,     0,     0,
     144,   145,     0,     0,   146,     0,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   488,     0,     0,   272,     0,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       0,     0,     0,     0,     0,     0,     0,   534,     0,     0,
       5,     6,     7,     8,     9,    10,    11,    12,    13,     0,
       0,     0,     0,  -245,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,  -249,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -249,  -249,  -249,  -249,
    -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,
    -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,
    -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,  -249,
    -249,  -249,  -249,  -249,  -249,  -249,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     0,     0,     0,     0,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44
};

static const short int yycheck[] =
{
      54,    55,    78,    54,   112,    55,   225,   279,   143,   144,
     145,   146,   147,   148,   156,   231,   403,   279,   153,   154,
     155,   323,   230,   230,   230,   341,    80,   319,   143,   144,
     145,   146,   549,   351,   524,   442,     3,     3,   319,   154,
     155,     3,   433,     3,    98,   606,     3,     3,    98,     1,
     557,   558,    97,     4,     4,     4,     1,     0,   112,    14,
       0,    62,     3,    59,    92,    92,    62,    84,   137,   158,
     124,   145,   126,   127,    76,   126,     3,   127,   158,     6,
     156,    92,    92,    92,   158,   159,   305,    89,    87,   158,
     159,   310,    87,    92,    87,    87,    87,    92,   152,    92,
      92,    92,    90,    91,    47,   122,   123,    47,   136,   136,
     135,   136,   137,   138,   139,   140,   141,   142,   625,   120,
      87,     1,   341,    87,   120,   136,   136,   136,    87,   401,
     127,   128,    75,    92,   117,    90,   119,    89,    89,    89,
      89,   433,   533,   435,    89,   706,    87,   282,   283,   284,
     285,   286,   433,   288,   435,   231,    87,   292,     0,   676,
     295,    92,    91,   298,    87,   139,   140,   302,   675,    92,
     677,   678,   226,    91,   681,    87,   230,   222,   145,   145,
      92,   145,   158,   159,   400,   145,    89,   302,   264,    91,
      92,   158,   159,   159,   158,   159,   272,   159,   158,   159,
     225,   158,   159,   159,   106,   281,    89,   614,    87,    89,
      87,    91,    92,    92,    87,    92,   270,   707,    87,    92,
     737,    92,   124,    92,   141,   142,   106,    87,    89,   736,
       3,    87,    92,   309,     7,     8,    92,    10,    11,    12,
      13,   533,   560,    87,   124,    90,    17,   145,    92,   565,
      89,   758,   533,   555,   396,   123,   124,   311,   312,    89,
     314,   158,   159,   482,   399,   147,   148,    90,    91,   323,
     311,   312,   659,   136,   137,   138,   158,   159,   160,   161,
     305,   306,    89,   359,   399,   310,   494,   494,   494,    43,
      44,   316,   143,   144,   145,   124,    88,   159,   129,   131,
     354,   159,   159,   130,   132,    92,   522,   579,   527,   133,
      90,    92,   124,    90,    90,   369,   341,   579,   159,    88,
      87,   106,    90,    87,   400,    98,    99,   100,   101,   102,
     103,   104,   105,    90,    90,    87,    87,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   565,   118,    92,   120,
     121,   376,    92,   378,    92,   380,    87,   382,    92,   384,
      87,   386,   504,   388,    92,   390,   442,    87,    92,    87,
      92,    87,    87,   449,    87,    92,   149,   150,   151,   152,
      87,     3,    87,    87,    87,   657,   159,    12,    88,   524,
      91,   445,    87,   159,   159,     1,   159,     3,   159,   159,
      87,     7,     8,    87,    10,    11,    12,    13,    87,    92,
      88,    87,   631,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    91,    91,   126,    90,   159,    33,    34,    35,
      91,    37,    38,    39,    91,    41,    42,    43,   159,     5,
     494,    90,    92,    49,    50,    51,   522,    53,   159,   159,
      91,   126,   126,    91,    87,    92,    92,   482,    87,    87,
     485,   486,   487,   159,    88,    92,    87,    87,    91,    12,
      68,    88,   548,   549,    93,    91,   530,   553,   554,   530,
      91,    91,    87,    92,    92,    87,    87,    90,    87,   159,
      87,   126,    98,    99,   100,   101,   102,   103,   104,   105,
      92,   555,   527,   557,   558,    92,    87,    93,   126,    91,
      87,    87,    90,   159,   159,   159,   159,    87,    87,    87,
      92,    92,    12,   577,    87,   579,   132,    87,   579,    62,
      91,    87,    87,    92,    87,   141,   142,    87,   614,   145,
     565,   147,   148,   149,   150,   151,   152,   153,   154,   155,
      87,   659,    87,   159,    92,    87,    87,   582,    87,    87,
     585,    91,   587,    12,   589,    87,   591,    92,   593,    87,
     595,   625,   707,    12,    87,    47,    75,   125,   284,   288,
     292,   295,   631,   282,    75,   283,   312,   452,   285,   116,
     298,   481,   286,   396,   533,   226,    54,   210,   674,   435,
     676,   124,   670,   657,   111,   659,   631,   657,   494,   523,
     635,   519,   637,   578,   639,   357,   670,   690,    -1,   670,
      -1,   675,   314,   677,   678,    -1,    -1,   681,    -1,    -1,
      -1,    -1,    -1,    -1,   688,   689,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   737,    -1,    -1,    -1,    -1,    -1,    -1,   693,    -1,
     695,    -1,   697,    -1,   699,    -1,   701,     1,   703,     3,
     705,    -1,   736,     7,     8,    -1,    10,    11,    12,    13,
      -1,    -1,    -1,   718,   719,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   758,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,    -1,
      -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,   756,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,   145,   107,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     1,    -1,     3,   159,    -1,    -1,     7,     8,
      -1,    10,    11,    12,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,
     145,     3,    -1,    -1,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    14,    15,   159,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,   145,     3,    -1,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,   159,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     1,    -1,     3,
     159,    -1,     6,     7,     8,    -1,    10,    11,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     1,    -1,     3,   159,    -1,     6,     7,     8,
      -1,    10,    11,    12,    13,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     1,    -1,     3,
     159,    -1,    -1,     7,     8,     9,    10,    11,    12,    13,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    35,    -1,    37,    38,    39,    -1,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,
      -1,    -1,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,     1,    -1,     3,   159,    -1,    -1,     7,     8,
       9,    10,    11,    12,    13,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    34,    35,    -1,    37,    38,
      39,    -1,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    -1,    53,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    -1,    63,    64,    65,    66,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     1,    -1,     3,
     159,    -1,    -1,     7,     8,     9,    10,    11,    12,    13,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    35,    -1,    37,    38,    39,    -1,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,
      -1,    -1,    -1,    57,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,   132,    -1,
       7,     8,     9,    10,    11,    12,    13,   141,   142,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,    -1,   159,    33,    34,    35,    -1,
      37,    38,    39,    -1,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    53,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    66,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,     1,
      -1,     3,    -1,    -1,    -1,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,    -1,    57,    58,    59,    60,    61,
      -1,    63,    64,    65,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,     1,    -1,     3,    -1,    -1,    -1,
       7,     8,     9,    10,    11,    12,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,    -1,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    -1,    -1,    -1,   159,    -1,    -1,
      57,    58,    59,    60,    61,    -1,    63,    64,    65,    66,
      67,    68,     1,    -1,     3,    -1,    -1,    -1,     7,     8,
      -1,    10,    11,    12,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,    -1,
      -1,    -1,   159,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,   100,   101,   102,   103,   104,   105,     1,    -1,     3,
      -1,    -1,    -1,     7,     8,    -1,    10,    11,    12,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,    -1,   145,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,    -1,    -1,    -1,
     159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,    -1,     7,     8,    -1,    10,    11,
      12,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,    -1,   132,     7,
       8,    -1,    10,    11,    12,    13,    -1,   141,   142,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,    -1,   159,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,
     132,     7,     8,    -1,    10,    11,    12,    13,    -1,   141,
     142,    -1,    -1,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    -1,    -1,    -1,   159,    -1,    -1,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,   132,     7,     8,    -1,    10,    11,
      12,    13,    -1,   141,   142,    -1,    -1,   145,    -1,   147,
     148,   149,   150,   151,   152,   153,   154,   155,    -1,    -1,
      -1,   159,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,    -1,    -1,   132,     7,     8,    -1,
      10,    11,    12,    13,    -1,   141,   142,    -1,    -1,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   154,   155,
      -1,    -1,    -1,   159,    -1,    -1,    98,    99,   100,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,
     132,     7,     8,    -1,    10,    11,    12,    13,    -1,   141,
     142,    -1,    -1,   145,    -1,   147,   148,   149,   150,   151,
     152,   153,   154,   155,    -1,    -1,    -1,   159,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
      -1,    -1,   132,     7,     8,    -1,    10,    11,    12,    13,
      -1,   141,   142,    -1,    -1,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,    -1,   159,
      -1,    -1,    98,    99,   100,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,    -1,    -1,   132,     7,     8,    -1,
      10,    11,    12,    13,    -1,   141,   142,    -1,    -1,   145,
      -1,   147,   148,   149,   150,   151,   152,   153,   154,   155,
      -1,    -1,    -1,   159,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,    -1,   159,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
       7,     8,   132,    10,    11,    12,    13,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,   145,    23,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    33,    34,    35,   159,
      37,    38,    39,    -1,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,     7,     8,    -1,    10,    11,    12,    13,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,   100,   101,   102,   103,   104,   105,    33,
      34,    35,    -1,    37,    38,    39,    -1,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    -1,    53,
      -1,    -1,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,    -1,   145,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,     3,
      -1,    -1,   159,     7,     8,    -1,    10,    11,    12,    13,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,    -1,     3,    -1,    -1,    -1,     7,     8,    -1,
      10,    11,    12,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,   102,   103,
     104,   105,     3,    -1,    -1,    -1,     7,     8,    -1,    10,
      11,    12,    13,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,   102,   103,   104,   105,    -1,    -1,   132,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
      -1,   145,    -1,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   132,    -1,    -1,   159,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,    -1,   145,    -1,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,    -1,   159,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,    -1,   145,    -1,   147,   148,   149,   150,
     151,   152,   153,   154,   155,     1,    -1,    -1,   159,    -1,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    89,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     1,     6,    14,    15,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,   107,   163,   164,   166,   167,
     168,   269,   270,   271,   272,   273,   278,   279,   280,   281,
     282,   284,   300,   301,   304,   305,   307,   308,   309,   310,
     311,   312,   313,   314,   315,   372,    90,    91,   158,     4,
      89,   306,    89,   306,    89,   306,   158,     0,     1,   166,
     165,     1,    89,   169,    91,    91,     3,   145,   159,   290,
     292,   293,   294,   295,   338,   340,   341,   342,   343,   344,
     347,   348,   349,   353,   293,   338,   339,   340,   283,   284,
     285,   283,    89,    89,    89,   168,   272,   273,   165,     7,
       8,    10,    11,    12,    13,    98,    99,   100,   101,   102,
     103,   104,   105,   132,   141,   142,   145,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   159,   202,   203,   204,
     205,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   225,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   249,   250,   251,   260,
     353,     6,   317,   319,   320,   321,   322,   353,   354,   355,
      14,   316,   323,   324,   316,   117,   119,   373,   165,    90,
     170,   301,   302,   303,   340,   124,    92,   261,   262,   291,
     137,   158,   159,   347,   341,   261,   283,   316,   316,   317,
     169,   338,   340,     1,   203,     1,   203,     1,   203,     1,
     203,     1,   203,     1,   203,     1,   203,     1,   203,   215,
     228,   215,   215,   215,   159,   228,   228,   159,   159,   159,
     159,   353,   159,   228,   215,   215,     1,   207,   277,   278,
      88,   127,   128,   129,   130,   131,   132,   133,   256,   136,
     137,   138,   257,   139,   140,   258,   141,   142,   254,   143,
     144,   145,   255,   123,   124,   259,    92,   147,   148,   158,
     159,   160,   161,    90,    92,   318,   124,   323,    90,   325,
      90,   159,    88,   263,   264,   301,    87,    23,    33,    34,
      35,    37,    38,    39,    41,    42,    43,    49,    50,    51,
      53,    89,   203,   221,   222,   223,   224,   227,   296,   298,
     292,   267,   268,   271,   273,   106,   368,   369,   370,   159,
     243,   251,   260,   345,   346,   353,   204,    87,   350,   351,
     358,   359,    90,    90,    90,    87,    92,    87,    92,    87,
      92,    87,    92,    87,    92,    87,    92,    87,    92,    87,
      92,    87,    87,    87,   353,   275,   276,    87,    87,    87,
     158,   159,   274,   349,   365,   366,   367,   207,   209,   211,
     212,   213,   216,   217,   218,   219,   220,   215,   203,   224,
     203,   207,   203,   224,   252,   253,   248,   355,   248,   321,
     203,   287,   289,   300,   304,   326,   327,    12,   374,     1,
       9,    57,    58,    59,    60,    61,    63,    64,    65,    66,
      67,    68,    89,    91,   171,   172,   175,   180,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   207,   353,
     265,   266,   159,   297,   298,   159,   159,   159,     1,   268,
      91,   159,   370,   207,    92,   136,    88,    87,   352,   353,
      87,   360,   361,   362,   363,   203,   203,   203,   203,   203,
     203,   203,   203,    87,    87,   277,   215,   204,   356,   357,
     365,   366,   158,   159,   126,    88,    87,    92,   328,   329,
     330,   286,   287,   288,    14,    91,   327,    91,    87,    92,
      90,    91,   205,    91,   179,   180,   181,   224,   159,   159,
       5,   206,   207,   159,   159,   263,    90,   173,   126,    91,
     267,     1,   203,   224,   226,    92,   299,     1,   203,     1,
     203,     1,   203,    91,   159,    87,   345,    92,    92,   278,
     364,    87,    92,    87,    87,    92,    87,    92,    87,    92,
      87,    92,    87,    92,    87,    92,    88,    87,   360,    87,
     204,   356,   202,   203,   224,    92,   332,   333,   334,   337,
     338,   286,    91,    12,    93,    68,    91,   207,   206,    91,
      91,   207,   207,   176,   177,   180,    15,   174,   179,   179,
      87,    92,   298,    90,    87,    92,    87,    92,    87,    92,
      17,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     118,   120,   121,   371,   353,    93,   362,   159,   338,   349,
     365,   203,   203,   203,   203,   203,   203,   203,    88,    87,
     331,   368,   126,   205,   159,    87,    91,    87,    87,    90,
      15,   178,   179,   226,   203,   203,   203,   159,   159,   159,
     159,    87,    87,    92,    87,    92,    87,    92,    87,    92,
      87,    92,    87,    92,    87,    92,   332,   335,   207,   179,
     206,   179,   179,     1,    15,   179,    87,    87,    92,    92,
      12,   353,   353,   374,    87,   203,   203,   203,   203,   203,
     203,   203,   368,   202,   336,    87,    62,    91,    91,   203,
     203,    87,    87,    92,    87,    87,    87,    87,    87,    87,
      87,    87,    91,   179,   206,    87,    92,    12,    87,   203,
      92,   179,    87,    12,    87
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
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
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
                  Type, Value);	\
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
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

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
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

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



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


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


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

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

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 292 "gram.y"
    {
            if (err_cnt == 0)
              *gProject->Parse_TOS->yyerrstream
              << "Warning: ANSI/ISO C forbids an empty source file.\n";
            gProject->Parse_TOS->transUnit = (TransUnit*) NULL;
            (yyval.transunit) = (TransUnit*) NULL;
        ;}
    break;

  case 3:
#line 300 "gram.y"
    {
            if (err_cnt)
            {
                *gProject->Parse_TOS->yyerrstream
                << err_cnt << " errors found.\n";
                gProject->Parse_TOS->transUnit = (TransUnit*) NULL;
            } else {
                gProject->Parse_TOS->transUnit = (yyval.transunit);
            }
        ;}
    break;

  case 4:
#line 311 "gram.y"
    {
            *gProject->Parse_TOS->yyerrstream << "Errors - Aborting parse.\n";
            gProject->Parse_TOS->transUnit = (TransUnit*) NULL;
            YYACCEPT;
        ;}
    break;

  case 5:
#line 319 "gram.y"
    {
            (yyval.transunit) = gProject->Parse_TOS->transUnit;
            (yyval.transunit)->add((yyvsp[-1].stemnt));
        ;}
    break;

  case 6:
#line 324 "gram.y"
    {
            (yyval.transunit)->add((yyvsp[-1].stemnt));
        ;}
    break;

  case 7:
#line 330 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->ReinitializeCtxt();
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ExitScopes(FILE_SCOPE);
            err_top_level = 0;
        ;}
    break;

  case 8:
#line 339 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].declStemnt);
        ;}
    break;

  case 9:
#line 343 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].functionDef);
        ;}
    break;

  case 10:
#line 347 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].stemnt);
        ;}
    break;

  case 11:
#line 351 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].stemnt);
        ;}
    break;

  case 12:
#line 355 "gram.y"
    {
            (yyval.stemnt) = (Statement*) NULL;
        ;}
    break;

  case 13:
#line 359 "gram.y"
    {
            (yyval.stemnt) = (Statement*) NULL;
        ;}
    break;

  case 14:
#line 366 "gram.y"
    {
            if ((yyvsp[0].stemnt) != NULL)
            {
                (yyval.functionDef) = new FunctionDef((yyvsp[0].stemnt)->location);
                Block *blk = (Block*) (yyvsp[0].stemnt);

                (yyval.functionDef)->decl = (yyvsp[-1].decl);

                if ((yyvsp[-1].decl)->name &&
                    (yyvsp[-1].decl)->name->entry)
                    (yyvsp[-1].decl)->name->entry->u2FunctionDef = (yyval.functionDef);

                // Steal internals of the compound statement
                (yyval.functionDef)->head = blk->head;
                (yyval.functionDef)->tail = blk->tail;
                
                //! Set kernel attributes
                (yyval.functionDef)->kernelAttribute = (yyvsp[-2].gccAttrib);

                blk->head = blk->tail = (Statement*) NULL;
                delete (yyvsp[0].stemnt);
            }
            else
            {
                    delete (yyvsp[-2].gccAttrib);
                    delete (yyvsp[-1].decl);
                    (yyval.functionDef) = (FunctionDef*) NULL;
            }
        ;}
    break;

  case 15:
#line 396 "gram.y"
    {
            if ((yyvsp[0].stemnt) != NULL)
            {
                (yyval.functionDef) = new FunctionDef((yyvsp[0].stemnt)->location);
                Block *blk = (Block*) (yyvsp[0].stemnt);

                (yyval.functionDef)->decl = (yyvsp[-1].decl);

                if ((yyvsp[-1].decl)->name &&
                    (yyvsp[-1].decl)->name->entry)
                    (yyvsp[-1].decl)->name->entry->u2FunctionDef = (yyval.functionDef);

                // Steal internals of the compound statement
                (yyval.functionDef)->head = blk->head;
                (yyval.functionDef)->tail = blk->tail;

                blk->head = blk->tail = (Statement*) NULL;
                delete (yyvsp[0].stemnt);
            }
            else
            {
                    delete (yyvsp[-1].decl);
                    (yyval.functionDef) = (FunctionDef*) NULL;
            }
        ;}
    break;

  case 16:
#line 424 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->ResetDeclCtxt();

            possibleType = true;
            (yyval.decl) = (yyvsp[-1].decl);

            if ((yyval.decl)->form != NULL)
            {
                assert(err_top_level ||
                       (yyval.decl)->form->type == TT_Function );

                (yyval.decl)->extend((yyvsp[-2].base));

                /* This is adding K&R-style declarations if $3 exists */
                if ((yyvsp[0].declStemnt) != NULL)
                {
                    FunctionType *fnc = (FunctionType*) ((yyval.decl)->form);
                    fnc->KnR_decl = true;
                    Statement *param = (yyvsp[0].declStemnt);
                    while (param != NULL)
                    {
                        Statement *next= param->next;
                        delete param ;
                        param = next;
                    }
                }
            }
        ;}
    break;

  case 17:
#line 453 "gram.y"
    {

            gProject->Parse_TOS->parseCtxt->ResetDeclCtxt();

            (yyval.decl) = (yyvsp[-1].decl);

            if ((yyval.decl)->form != NULL)
            {
                assert(err_top_level ||
                       (yyval.decl)->form->type == TT_Function );
                (yyval.decl)->extend((yyvsp[-2].base));

                /* This is adding K&R-style declarations if $3 exists */
                if ((yyvsp[0].declStemnt) != NULL)
                {
                    FunctionType *fnc = (FunctionType*) ((yyval.decl)->form);
                    fnc->KnR_decl = true;
                    Statement *param = (yyvsp[0].declStemnt);
                    while (param != NULL)
                    {
                        Statement *next= param->next;
                        delete param ;
                        param = next;
                    }
                }
            }
        ;}
    break;

  case 18:
#line 486 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ReEnterScope();
        ;}
    break;

  case 19:
#line 491 "gram.y"
    {
            Block *block = new Block((yyvsp[-4].loc));
            (yyval.stemnt) = block;
            block->addDeclStemnts((yyvsp[-2].declStemnt));
            block->addStatements(ReverseList((yyvsp[-1].stemnt)));
            if (gProject->Parse_TOS->transUnit)
            {
                yyCheckLabelsDefinition(gProject->Parse_TOS->transUnit->contxt.labels);
                gProject->Parse_TOS->transUnit->contxt.ExitScope();
                gProject->Parse_TOS->transUnit->contxt.ExitScope();
            }
        ;}
    break;

  case 20:
#line 504 "gram.y"
    {
            (yyval.stemnt) = NULL;
        ;}
    break;

  case 21:
#line 510 "gram.y"
    {
            (yyval.stemnt) = (Statement*) NULL;
        ;}
    break;

  case 23:
#line 517 "gram.y"
    {
            /*
             * All the statements are expected in a reversed list (because
             * of how we parse stemnt_list2) so we need to take the
             * non_constructor statement at the end.
             */
            if ((yyvsp[0].stemnt))
            {
                Statement *s;

                for (s = (yyvsp[0].stemnt); s->next; s = s->next) /* Traverse to the end */;
                s->next = (yyvsp[-1].stemnt);
                (yyval.stemnt) = (yyvsp[0].stemnt);
            }
        ;}
    break;

  case 24:
#line 535 "gram.y"
    {
           (yyval.stemnt) = (Statement *) NULL;
        ;}
    break;

  case 25:
#line 539 "gram.y"
    {
            /* Hook them up backwards, we'll reverse them later. */
            if ((yyvsp[0].stemnt))
            {
                (yyvsp[0].stemnt)->next = (yyvsp[-1].stemnt);
                (yyval.stemnt) = (yyvsp[0].stemnt);
            }
        ;}
    break;

  case 26:
#line 548 "gram.y"
    {    /* preprocessor #line directive */
            /* Hook them up backwards, we'll reverse them later. */
            if ((yyvsp[0].stemnt))
            {
                (yyvsp[0].stemnt)->next = (yyvsp[-1].stemnt);
                (yyval.stemnt) = (yyvsp[0].stemnt);
            }

        ;}
    break;

  case 28:
#line 568 "gram.y"
    {
            Block *block = new Block((yyvsp[-3].loc));
            (yyval.stemnt) = block;
            block->addDeclStemnts((yyvsp[-2].declStemnt));
            block->addStatements(ReverseList((yyvsp[-1].stemnt)));

            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ExitScope();
        ;}
    break;

  case 29:
#line 578 "gram.y"
    {
            (yyval.stemnt) = NULL;
        ;}
    break;

  case 30:
#line 584 "gram.y"
    {
            (yyval.stemnt) = (Statement*) NULL;
        ;}
    break;

  case 32:
#line 591 "gram.y"
    {
            /*
             * All the statements are expected in a reversed list (because
             * of how we parse stemnt_list_reentrance2) so we need to take
             * the non_constructor statement at the end.
             */
            if ((yyvsp[0].stemnt))
            {
                Statement *s;

                for (s = (yyvsp[0].stemnt); s->next; s = s->next) /* Traverse to the end */;
                s->next = (yyvsp[-1].stemnt);
                (yyval.stemnt) = (yyvsp[0].stemnt);
            }
        ;}
    break;

  case 33:
#line 607 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].stemnt);
        ;}
    break;

  case 34:
#line 613 "gram.y"
    {
                (yyval.stemnt) = (yyvsp[0].stemnt);
            ;}
    break;

  case 35:
#line 617 "gram.y"
    {
                (yyval.stemnt) = (yyvsp[0].stemnt);
            ;}
    break;

  case 36:
#line 621 "gram.y"
    {
            /* Hook them up backwards, we'll reverse them later. */
            if ((yyvsp[0].stemnt))
            {
                (yyvsp[0].stemnt)->next = (yyvsp[-1].stemnt);
                (yyval.stemnt) = (yyvsp[0].stemnt);
            }
        ;}
    break;

  case 37:
#line 630 "gram.y"
    {    /* preprocessor #line directive */
            /* Hook them up backwards, we'll reverse them later. */
            if ((yyvsp[0].stemnt))
            {
                (yyvsp[0].stemnt)->next = (yyvsp[-1].stemnt);
                (yyval.stemnt) = (yyvsp[0].stemnt);
            }
        ;}
    break;

  case 38:
#line 639 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[-2].stemnt);
        ;}
    break;

  case 39:
#line 645 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].stemnt);
         ;}
    break;

  case 40:
#line 649 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].stemnt);
         ;}
    break;

  case 53:
#line 692 "gram.y"
    {
            (yyval.stemnt) = new ExpressionStemnt((yyvsp[-1].value),(yyvsp[0].loc));
        ;}
    break;

  case 54:
#line 698 "gram.y"
    {
            (yyval.stemnt) = new ExpressionStemnt((yyvsp[-1].value),(yyvsp[0].loc));
        ;}
    break;

  case 55:
#line 704 "gram.y"
    {
            (yyval.stemnt) = (yyvsp[0].stemnt);
            if ((yyval.stemnt) == NULL)
            {
              /* Sorry, we must have a statement here. */
              yyerr("Error: Can't have a label at the end of a block! ");
              (yyval.stemnt) = new Statement(ST_NullStemnt, (yyvsp[-1].loc));
            }
            (yyval.stemnt)->addHeadLabel((yyvsp[-2].label));
        ;}
    break;

  case 61:
#line 726 "gram.y"
    {
            (yyval.stemnt) = new SwitchStemnt((yyvsp[-2].value),(yyvsp[0].stemnt),(yyvsp[-4].loc));
        ;}
    break;

  case 62:
#line 732 "gram.y"
    {
            (yyval.stemnt) = new Statement(ST_BreakStemnt, (yyvsp[-1].loc));
        ;}
    break;

  case 63:
#line 738 "gram.y"
    {
            (yyval.stemnt) = new Statement(ST_ContinueStemnt, (yyvsp[-1].loc));
        ;}
    break;

  case 64:
#line 744 "gram.y"
    {
            (yyval.stemnt) = new ReturnStemnt((yyvsp[-1].value), (yyvsp[-2].loc));
        ;}
    break;

  case 65:
#line 750 "gram.y"
    {
            (yyval.stemnt) = new GotoStemnt((yyvsp[-1].symbol), (yyvsp[-2].loc));
        ;}
    break;

  case 66:
#line 756 "gram.y"
    {
            (yyval.stemnt) = new Statement(ST_NullStemnt, (yyvsp[0].loc));
        ;}
    break;

  case 67:
#line 762 "gram.y"
    {
            (yyval.stemnt) = new IfStemnt((yyvsp[-2].value), (yyvsp[0].stemnt), (yyvsp[-4].loc));
        ;}
    break;

  case 68:
#line 768 "gram.y"
    {
            (yyval.stemnt) = new IfStemnt((yyvsp[-4].value), (yyvsp[-2].stemnt), (yyvsp[-6].loc), (yyvsp[0].stemnt));
        ;}
    break;

  case 69:
#line 774 "gram.y"
    {
#ifdef __ASMIL__
            (yyval.stemnt) = new ASMStemnt((yyvsp[0].consValue), (yyvsp[0].consValue)->location);;
#else
            (yyval.stemnt) = NULL; /* We should never get here as asm supporte disabled in lexer. */
#endif /* __ASMIL__ */
        ;}
    break;

  case 70:
#line 785 "gram.y"
    {
            (yyval.stemnt) = new DoWhileStemnt((yyvsp[-2].value), (yyvsp[-5].stemnt), (yyvsp[-6].loc));
        ;}
    break;

  case 71:
#line 791 "gram.y"
    {
            (yyval.stemnt) = new WhileStemnt((yyvsp[-2].value), (yyvsp[0].stemnt), (yyvsp[-4].loc));
        ;}
    break;

  case 72:
#line 798 "gram.y"
    {
            (yyval.stemnt) = new ForStemnt((yyvsp[-6].value), (yyvsp[-4].value), (yyvsp[-2].value), (yyvsp[-8].loc), (yyvsp[0].stemnt));
        ;}
    break;

  case 76:
#line 809 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                (yyval.label) = gProject->Parse_TOS->parseCtxt->Mk_named_label((yyvsp[0].symbol),
                                gProject->Parse_TOS->transUnit->contxt.labels);
        ;}
    break;

  case 77:
#line 817 "gram.y"
    {
            (yyval.label) = new Label((yyvsp[0].value));
        ;}
    break;

  case 78:
#line 821 "gram.y"
    {
            (yyval.label) = new Label((yyvsp[-2].value),(yyvsp[0].value));
        ;}
    break;

  case 79:
#line 827 "gram.y"
    {
            (yyval.label) = new Label(LT_Default);
        ;}
    break;

  case 81:
#line 843 "gram.y"
    {
            (yyval.value) = new TrinaryExpr((yyvsp[-4].value), (yyvsp[-2].value), (yyvsp[0].value), (yyvsp[-3].loc));
        ;}
    break;

  case 83:
#line 850 "gram.y"
    {
            (yyval.value) = new AssignExpr((yyvsp[-1].assignOp),(yyvsp[-2].value),(yyvsp[0].value),currentLocation);
        ;}
    break;

  case 84:
#line 854 "gram.y"
    {
            (yyval.value) = new AssignExpr((yyvsp[-1].assignOp),(yyvsp[-2].value),(yyvsp[0].value),currentLocation);
        ;}
    break;

  case 85:
#line 860 "gram.y"
    {
            (yyval.value) = (Expression*) NULL;
        ;}
    break;

  case 88:
#line 870 "gram.y"
    {
           (yyval.value) = (Expression*) NULL;
        ;}
    break;

  case 92:
#line 881 "gram.y"
    {
            (yyval.value) = new BinaryExpr(BO_Or, (yyvsp[-2].value), (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 94:
#line 888 "gram.y"
    {
            (yyval.value) = new BinaryExpr(BO_And, (yyvsp[-2].value), (yyvsp[0].value),(yyvsp[-1].loc));
        ;}
    break;

  case 95:
#line 894 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_Not, (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 97:
#line 901 "gram.y"
    {
            (yyval.value) = new BinaryExpr(BO_BitOr, (yyvsp[-2].value), (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 99:
#line 908 "gram.y"
    {
            (yyval.value) = new BinaryExpr(BO_BitXor, (yyvsp[-2].value), (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 101:
#line 915 "gram.y"
    {
            (yyval.value) = new BinaryExpr(BO_BitAnd, (yyvsp[-2].value), (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 102:
#line 921 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_BitNot, (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 104:
#line 928 "gram.y"
    {
            (yyval.value) = new CastExpr((yyvsp[-2].type), (yyvsp[0].value), (yyvsp[-3].loc));
        ;}
    break;

  case 106:
#line 935 "gram.y"
    {
            (yyval.value) = new RelExpr((yyvsp[-1].relOp), (yyvsp[-2].value), (yyvsp[0].value), currentLocation);
        ;}
    break;

  case 108:
#line 942 "gram.y"
    {
            (yyval.value) = new RelExpr((yyvsp[-1].relOp), (yyvsp[-2].value), (yyvsp[0].value), currentLocation);
        ;}
    break;

  case 110:
#line 949 "gram.y"
    {
            (yyval.value) = new BinaryExpr((yyvsp[-1].binOp), (yyvsp[-2].value), (yyvsp[0].value), currentLocation);
        ;}
    break;

  case 112:
#line 956 "gram.y"
    {
            (yyval.value) = new BinaryExpr((yyvsp[-1].binOp), (yyvsp[-2].value), (yyvsp[0].value), currentLocation);
        ;}
    break;

  case 114:
#line 963 "gram.y"
    {
            (yyval.value) = new BinaryExpr((yyvsp[-1].binOp), (yyvsp[-2].value), (yyvsp[0].value), currentLocation);
        ;}
    break;

  case 128:
#line 972 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            (yyval.value) = new ConstructorExpr((yyvsp[-5].base), exprs, currentLocation);
        ;}
    break;

  case 129:
#line 978 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            (yyval.value) = new ConstructorExpr((yyvsp[-7].base), exprs, currentLocation);
        ;}
    break;

  case 130:
#line 984 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            (yyval.value) = new ConstructorExpr((yyvsp[-9].base), exprs, currentLocation);
        ;}
    break;

  case 131:
#line 989 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 132:
#line 993 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 133:
#line 997 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 134:
#line 1002 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Float2);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 135:
#line 1009 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Float3);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 136:
#line 1016 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Float4);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 137:
#line 1023 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Double2);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 138:
#line 1031 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Int2);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 139:
#line 1038 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Int3);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 140:
#line 1045 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Int4);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 141:
#line 1053 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Int2 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 142:
#line 1060 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Int3 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 143:
#line 1067 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Int4 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 144:
#line 1075 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Short2);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 145:
#line 1082 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Short3);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 146:
#line 1089 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Short4);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 147:
#line 1097 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Short2 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 148:
#line 1104 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Short3 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 149:
#line 1111 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Short4 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 150:
#line 1119 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Char2);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 151:
#line 1126 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Char3);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 152:
#line 1133 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Char4);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 153:
#line 1141 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Char2 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 154:
#line 1148 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Char3 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 155:
#line 1155 "gram.y"
    {
            Expression *exprs[] = { (yyvsp[-7].value), (yyvsp[-5].value), (yyvsp[-3].value), (yyvsp[-1].value) };
            BaseType *bType = new BaseType(BT_Char4 | BT_UnSigned);
            (yyval.value) = new ConstructorExpr(bType, exprs, currentLocation);
        ;}
    break;

  case 156:
#line 1161 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 157:
#line 1166 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 158:
#line 1171 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 159:
#line 1176 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 160:
#line 1181 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 161:
#line 1186 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 162:
#line 1191 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 163:
#line 1196 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 166:
#line 1207 "gram.y"
    {
           Symbol *sym = new Symbol();
           Variable *var;

           sym->name = strdup("iter");
           var = new Variable(sym, (yyvsp[-4].loc));
           (yyval.value) = new FunctionCall(var, (yyvsp[-4].loc));

           ((FunctionCall *) (yyval.value))->addArg((yyvsp[-3].value));
           ((FunctionCall *) (yyval.value))->addArg((yyvsp[-1].value));

        ;}
    break;

  case 167:
#line 1220 "gram.y"
    {
           (yyval.value) = (Expression *) NULL;
        ;}
    break;

  case 178:
#line 1238 "gram.y"
    {
            (yyval.value) = new SizeofExpr((yyvsp[-1].type), (yyvsp[-3].loc));
        ;}
    break;

  case 179:
#line 1242 "gram.y"
    {
            (yyval.value) = new SizeofExpr((yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 180:
#line 1248 "gram.y"
    {
          (yyval.value) = new BrtIndexofExpr(new Variable((yyvsp[0].symbol), (yyvsp[-1].loc)), (yyvsp[-1].loc));
        ;}
    break;

  case 181:
#line 1252 "gram.y"
    {
          (yyval.value) = new BrtIndexofExpr(new Variable((yyvsp[-1].symbol), (yyvsp[-3].loc)), (yyvsp[-3].loc));
        ;}
    break;

  case 182:
#line 1271 "gram.y"
    {
          (yyval.value) = new BrtInstanceExpr((yyvsp[-2].loc));
        ;}
    break;

  case 183:
#line 1277 "gram.y"
    {
          (yyval.value) = new BrtInstanceInGroupExpr((yyvsp[-2].loc));
        ;}
    break;

  case 184:
#line 1283 "gram.y"
    {
          (yyval.value) = new BrtSyncGroupExpr((yyvsp[-2].loc));
        ;}
    break;

  case 185:
#line 1289 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_Minus, (yyvsp[0].value), currentLocation);
        ;}
    break;

  case 186:
#line 1295 "gram.y"
    {
            /* Unary plus is an ISO addition (for symmetry) - ignore it */
            (yyval.value) = (yyvsp[0].value);
        ;}
    break;

  case 187:
#line 1302 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_AddrOf, (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 188:
#line 1308 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_Deref, (yyvsp[0].value), currentLocation);
        ;}
    break;

  case 189:
#line 1314 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_PreInc, (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 190:
#line 1320 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_PreDec, (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 192:
#line 1327 "gram.y"
    {
            (yyval.value) = new BinaryExpr(BO_Comma, (yyvsp[-2].value), (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 193:
#line 1333 "gram.y"
    {
            (yyval.value) = new Variable((yyvsp[0].symbol), currentLocation);
        ;}
    break;

  case 195:
#line 1338 "gram.y"
    {
            (yyval.value) = (yyvsp[0].consValue);
        ;}
    break;

  case 196:
#line 1342 "gram.y"
    {
            (yyval.value) = (yyvsp[0].value);
        ;}
    break;

  case 197:
#line 1348 "gram.y"
    {
            (yyval.value) = (yyvsp[-1].value);
        ;}
    break;

  case 198:
#line 1352 "gram.y"
    {
            (yyval.value) = (Expression*) NULL;
        ;}
    break;

  case 209:
#line 1371 "gram.y"
    {
            (yyval.value) = new IndexExpr((yyvsp[-3].value), (yyvsp[-1].value), (yyvsp[-2].loc));
        ;}
    break;

  case 212:
#line 1381 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_PostInc, (yyvsp[-1].value), (yyvsp[0].loc));
        ;}
    break;

  case 213:
#line 1387 "gram.y"
    {
            (yyval.value) = new UnaryExpr(UO_PostDec, (yyvsp[-1].value), (yyvsp[0].loc));
        ;}
    break;

  case 215:
#line 1397 "gram.y"
    {
            Variable *var = new Variable((yyvsp[0].symbol), (yyvsp[-1].loc));
            BinaryExpr *be = new BinaryExpr(BO_Member, (yyvsp[-2].value), var, (yyvsp[-1].loc));
            (yyval.value) = be;

            // Lookup the component in its struct
            // if possible.
            if ((yyvsp[-2].value)->etype == ET_Variable)
            {
                Variable  *var = (Variable*) (yyvsp[-2].value);
                Symbol    *varName = var->name;
                SymEntry  *entry = varName->entry;

                if (entry && entry->uVarDecl)
                {
                    entry->uVarDecl->lookup((yyvsp[0].symbol));
                }
            }
        ;}
    break;

  case 216:
#line 1419 "gram.y"
    {
            Variable *var = new Variable((yyvsp[0].symbol), (yyvsp[-1].loc));
            BinaryExpr *be = new BinaryExpr(BO_PtrMember, (yyvsp[-2].value), var, (yyvsp[-1].loc));
            (yyval.value) = be;

            // Lookup the component in its struct
            // if possible.
            if ((yyvsp[-2].value)->etype == ET_Variable)
            {
                Variable  *var = (Variable*) (yyvsp[-2].value);
                Symbol    *varName = var->name;
                SymEntry  *entry = varName->entry;

                if (entry && entry->uVarDecl)
                {
                    entry->uVarDecl->lookup((yyvsp[0].symbol));
                }
            }
        ;}
    break;

  case 217:
#line 1441 "gram.y"
    {
            FunctionCall* fc = new FunctionCall((yyvsp[-3].value), (yyvsp[-2].loc));

            /* add function args */
            fc->addArgs(ReverseList((yyvsp[-1].value)));

            (yyval.value) = fc;
        ;}
    break;

  case 218:
#line 1452 "gram.y"
    {
            (yyval.value) = (Expression*) NULL;
        ;}
    break;

  case 222:
#line 1461 "gram.y"
    {
            (yyval.value) = (yyvsp[0].value);
            (yyval.value)->next = (yyvsp[-2].value);
        ;}
    break;

  case 223:
#line 1466 "gram.y"
    {
            (yyval.value) = (yyvsp[0].value);
            (yyval.value)->next = (yyvsp[-2].value);
        ;}
    break;

  case 243:
#line 1511 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ReEnterScope();

            (yyval.declStemnt) = (DeclStemnt*) NULL;
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ExitScope();
        ;}
    break;

  case 244:
#line 1520 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ReEnterScope();
            gProject->Parse_TOS->parseCtxt->ResetDeclCtxt();

            gProject->Parse_TOS->parseCtxt->SetVarParam(1, !err_top_level, 0);
            gProject->Parse_TOS->parseCtxt->SetIsKnR(true);
        ;}
    break;

  case 245:
#line 1529 "gram.y"
    {   (yyval.declStemnt) = (yyvsp[0].declStemnt);
            gProject->Parse_TOS->parseCtxt->SetIsKnR(false);
            gProject->Parse_TOS->parseCtxt->SetVarParam(0, !err_top_level, 1);

            // Exit, but will allow re-enter for a function.
            // Hack, to handle parameters being in the function's scope.
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ExitScope(true);
        ;}
    break;

  case 246:
#line 1541 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.EnterScope();
            gProject->Parse_TOS->parseCtxt->ResetDeclCtxt();
        ;}
    break;

  case 247:
#line 1547 "gram.y"
    {
            (yyval.declStemnt) = (yyvsp[0].declStemnt);
        ;}
    break;

  case 248:
#line 1554 "gram.y"
    {
            (yyval.declStemnt) = (DeclStemnt*) NULL;
        ;}
    break;

  case 249:
#line 1558 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->SetVarParam(0, !err_top_level, 0);
        ;}
    break;

  case 250:
#line 1562 "gram.y"
    {   (yyval.declStemnt) = (yyvsp[0].declStemnt);
            gProject->Parse_TOS->parseCtxt->SetVarParam(0, !err_top_level, 0);
        ;}
    break;

  case 251:
#line 1575 "gram.y"
    {
            (yyval.declStemnt) = (yyvsp[0].declStemnt);
        ;}
    break;

  case 252:
#line 1579 "gram.y"
    {
            (yyval.declStemnt) = (yyvsp[-1].declStemnt);

                        Statement*      appendDecl = (yyvsp[-1].declStemnt);
                        while (appendDecl->next != NULL)
                                appendDecl = appendDecl->next;

            appendDecl->next = (yyvsp[0].declStemnt);
        ;}
    break;

  case 253:
#line 1589 "gram.y"
    {
            (yyval.declStemnt) = (yyvsp[-2].declStemnt);
        ;}
    break;

  case 254:
#line 1595 "gram.y"
    {
            (yyval.declStemnt) = new DeclStemnt((yyvsp[0].loc));
            (yyval.declStemnt)->addDecls(ReverseList((yyvsp[-1].decl)));
        ;}
    break;

  case 255:
#line 1609 "gram.y"
    {
            (yyval.declStemnt) = new DeclStemnt((yyvsp[0].loc));
            (yyval.declStemnt)->addDecls(ReverseList((yyvsp[-1].decl)));
        ;}
    break;

  case 256:
#line 1615 "gram.y"
    {
            (yyval.declStemnt) = new DeclStemnt((yyvsp[0].loc));
            (yyval.declStemnt)->addDecls(ReverseList((yyvsp[-1].decl)));
        ;}
    break;

  case 257:
#line 1622 "gram.y"
    {
            assert (err_top_level ||
                    (yyvsp[-1].base) == gProject->Parse_TOS->parseCtxt->curCtxt->decl_specs);
            gProject->Parse_TOS->parseCtxt->ResetDeclCtxt();

            yywarn("Warning: old-style declaration or incorrect type");

            possibleType = true;
            (yyval.decl) = (yyvsp[0].decl);

            if ((yyval.decl) == NULL)
            {
                (yyval.decl) = new Decl((yyvsp[-1].base));
            }
        ;}
    break;

  case 258:
#line 1640 "gram.y"
    {
            assert (1||err_top_level ||
                    (yyvsp[-1].base) == gProject->Parse_TOS->parseCtxt->curCtxt->decl_specs);
            if ((yyvsp[-1].base)!=gProject->Parse_TOS->parseCtxt->curCtxt->decl_specs) {
              if (!err_top_level) {
                baseTypeFixup((yyvsp[-1].base),(yyvsp[0].decl));
              }
            }
            gProject->Parse_TOS->parseCtxt->ResetDeclCtxt();

            possibleType = true;
            (yyval.decl) = (yyvsp[0].decl);

            if ((yyval.decl) == NULL)
            {
                (yyval.decl) = new Decl((yyvsp[-1].base));
            }
        ;}
    break;

  case 259:
#line 1665 "gram.y"
    {
            (yyval.base) = new BaseType(BT_Int);
            gProject->Parse_TOS->parseCtxt->SetDeclCtxt((yyval.base));
        ;}
    break;

  case 262:
#line 1684 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->PushCtxt();
            gProject->Parse_TOS->parseCtxt->ResetVarParam();
        ;}
    break;

  case 263:
#line 1689 "gram.y"
    {
            (yyval.type) = (yyvsp[0].type);
            gProject->Parse_TOS->parseCtxt->PopCtxt(false);
        ;}
    break;

  case 264:
#line 1696 "gram.y"
    {
            assert ((yyvsp[0].base) == gProject->Parse_TOS->parseCtxt->curCtxt->decl_specs);

            possibleType = true;
            (yyval.type) = (yyvsp[0].base);
            if ((yyval.type)->isFunction())
                yyerr("Error: Function type not allowed as type name");
        ;}
    break;

  case 265:
#line 1705 "gram.y"
    {
            assert ((yyvsp[-1].base) == gProject->Parse_TOS->parseCtxt->curCtxt->decl_specs);

            possibleType = true;
            (yyval.type) = (yyvsp[0].type);

            Type * extended = (yyval.type)->extend((yyvsp[-1].base));
            if ((yyval.type)->isFunction())
                yyerr("Error: Function type not allowed as type name");
            else if (extended &&
                (yyvsp[-1].base) && (yyvsp[-1].base)->isFunction() &&
                ! extended->isPointer())
                yyerr("Error: Wrong type combination") ;

        ;}
    break;

  case 266:
#line 1728 "gram.y"
    {
            if ((yyvsp[0].base)->typemask == BT_Signed || (yyvsp[0].base)->typemask == BT_UnSigned)
                (yyvsp[0].base)->typemask |= BT_Int;
            (yyval.base) = (yyvsp[0].base);
            gProject->Parse_TOS->parseCtxt->SetDeclCtxt((yyval.base));
        ;}
    break;

  case 273:
#line 1747 "gram.y"
    {
            if (! gProject->Parse_TOS->transUnit ||
                gProject->Parse_TOS->transUnit->contxt.syms->current->level >= FUNCTION_SCOPE)
                 (yyval.storage) = (yyvsp[0].storage) ;
             else
                 (yyval.storage) = ST_None ;
        ;}
    break;

  case 275:
#line 1760 "gram.y"
    {
            (yyval.base) = (BaseType*) NULL;
        ;}
    break;

  case 277:
#line 1767 "gram.y"
    {
            (yyval.base) = (yyvsp[0].base);

            if (!(yyval.base))
            {
                (yyval.base) = new BaseType();
            }

            if ((yyvsp[-1].storage) == ST_None)
                 yyerr("Error: Invalid use of local storage type");
            else if ((yyval.base)->storage != ST_None)
                 yywarn("Warning: Overloading previous storage type specification");
            else
                 (yyval.base)->storage = (yyvsp[-1].storage);
        ;}
    break;

  case 278:
#line 1782 "gram.y"
    { possibleType = false; ;}
    break;

  case 279:
#line 1783 "gram.y"
    {
            (yyval.base) = (yyvsp[-2].base);

            // FIXME - this all needs reworking in a more general and correct manner
            // FIXME - needs to fail for "signed unsigned <foo>" and similar cases

            if ((yyvsp[0].base))
            {
                bool typeerr = false;
                if ((yyval.base)->typemask == BT_Signed || (yyval.base)->typemask == BT_UnSigned)
                {
                    (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                }

                if ((yyvsp[0].base)->typemask & BT_Long)
                {
                    if ((yyval.base)->typemask & BT_Long)
                    {
                        // we map "long long" back to "long" for now
                        // FIXME - support 64 bit long long
                        // FIXME - what about signed/unsigned long long ?
                        yywarn("Warning: \"long long\" not currently supported - remapping to \"long\"");
                    }
                    else if ((yyval.base)->typemask & BT_Int)
                    {
                        (yyval.base)->typemask = (yyvsp[0].base)->typemask;
                    }
                    else if ((yyval.base)->typemask == BT_Signed || (yyval.base)->typemask == BT_UnSigned)
                    {
                        (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                    }
                    else
                    {
                        typeerr = true;
                    }
                }
                else if ((yyvsp[0].base)->typemask & BT_Int)
                {
                    if ((yyval.base)->typemask & BT_Long
                     || (yyval.base)->typemask & BT_Short
                     || (yyval.base)->typemask & BT_UnSigned
                     || (yyval.base)->typemask & BT_Signed)
                    {
                        ; // these combinations OK
                    }
                    else if ((yyval.base)->typemask == BT_Signed || (yyval.base)->typemask == BT_UnSigned)
                    {
                        (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                    }
                    else
                    {
                        typeerr = true;
                    }
                }
                else if ((yyvsp[0].base)->typemask & BT_Short)
                {
                    if ((yyval.base)->typemask & BT_Int
                     || (yyval.base)->typemask & BT_UnSigned
                     || (yyval.base)->typemask & BT_Signed)
                    {
                        (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                    }
                    else
                    {
                        typeerr = true;
                    }
                }
                else if ((yyvsp[0].base)->typemask & BT_Char)
                {
                    if ((yyval.base)->typemask & BT_Int
                     || (yyval.base)->typemask & BT_UnSigned
                     || (yyval.base)->typemask & BT_Signed)
                    {
                        (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                    }
                    else
                    {
                        typeerr = true;
                    }
                }
                else if ((yyvsp[0].base)->typemask & BT_Float)
                {
                    if ((yyval.base)->typemask & BT_Int
                        || (yyval.base)->typemask & BT_Short
                        || (yyval.base)->typemask & BT_Long
                        || (yyval.base)->typemask & BT_UnSigned
                        || (yyval.base)->typemask & BT_Signed
                        || (yyval.base)->typemask & BT_Double)
                    {
                        typeerr = true;
                    }
                    else
                    {
                        (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                    }
                }
                else if ((yyvsp[0].base)->typemask & BT_Double)
                {
                    if ((yyval.base)->typemask & BT_Int
                        || (yyval.base)->typemask & BT_Short
                        || (yyval.base)->typemask & BT_Long
                        || (yyval.base)->typemask & BT_UnSigned
                        || (yyval.base)->typemask & BT_Signed
                        || (yyval.base)->typemask & BT_Float)
                    {
                        typeerr = true;
                    }
                    else
                    {
                        (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                    }
                }
                if (typeerr)
                {
                    if (0)  // Debug code, turned off by default
                    {
                        printf("\n$$->typemask = %08x\n$3->typemask = %08x\n",
                               (unsigned int)(yyval.base)->typemask,
                               (unsigned int)((yyvsp[0].base)->typemask));
                    }
                    yyerr("Error: problem with type declaration");
                }
                if ((yyvsp[0].base)->storage != ST_None)
                    (yyval.base)->storage = (yyvsp[0].base)->storage;

                // delete $3;
            }

            /*
            std::cout << "In decl_spec: ";
            $$->printBase(std::cout,0);
            if ($$->storage == ST_Typedef)
                std::cout << "(is a typedef)";
            std::cout << std::endl;
            */
        ;}
    break;

  case 280:
#line 1920 "gram.y"
    {
            (yyval.base) = (yyvsp[0].base);

            if (!(yyval.base))
            {
                (yyval.base) = new BaseType();
            }

            if (TQ_None != ((yyval.base)->qualifier & (yyvsp[-1].typeQual)))
                yywarn("Warning: qualifier already specified");

            (yyval.base)->qualifier |= (yyvsp[-1].typeQual);

        ;}
    break;

  case 281:
#line 1940 "gram.y"
    {
           (yyval.base) = (BaseType*) NULL;
        ;}
    break;

  case 283:
#line 1946 "gram.y"
    { possibleType = false; ;}
    break;

  case 284:
#line 1947 "gram.y"
    {
            (yyval.base) = (yyvsp[-2].base);

            if ((yyvsp[0].base))
            {
                (yyval.base)->typemask |= (yyvsp[0].base)->typemask;
                // delete $3;
            }
        ;}
    break;

  case 285:
#line 1957 "gram.y"
    {
            (yyval.base) = (yyvsp[0].base);

            if (!(yyval.base))
            {
                (yyval.base) = new BaseType();
            }

            if (TQ_None != ((yyval.base)->qualifier & (yyvsp[-1].typeQual)))
                yywarn("Warning: qualifier already specified");
            (yyval.base)->qualifier |= (yyvsp[-1].typeQual);
        ;}
    break;

  case 286:
#line 1972 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->SetDeclCtxt((yyval.base));
        ;}
    break;

  case 287:
#line 1981 "gram.y"
    {
           (yyvsp[0].decl)->extend(gProject->Parse_TOS->parseCtxt->UseDeclCtxt());
        ;}
    break;

  case 288:
#line 1985 "gram.y"
    {
           (yyvsp[-2].decl)->attrib = (yyvsp[0].gccAttrib);
           (yyval.decl) = (yyvsp[-2].decl);
        ;}
    break;

  case 290:
#line 1993 "gram.y"
    {
           (yyvsp[-2].decl)->addInitializer((yyvsp[0].value));
           (yyval.decl) = (yyvsp[-2].decl);
        ;}
    break;

  case 291:
#line 2000 "gram.y"
    {
          (yyval.decl) = (Decl*) NULL;
        ;}
    break;

  case 294:
#line 2010 "gram.y"
    {
            (yyval.decl) = (yyvsp[0].decl);
        ;}
    break;

  case 295:
#line 2014 "gram.y"
    {
            (yyval.decl) = (yyvsp[-2].decl);

                        Decl*   appendDecl = (yyvsp[-2].decl);
                        while (appendDecl->next != NULL)
                                appendDecl = appendDecl->next;

            appendDecl->next = (yyvsp[0].decl);
        ;}
    break;

  case 297:
#line 2031 "gram.y"
    {
            (yyval.arrayConst) = new ArrayConstant(currentLocation);
            (yyval.arrayConst)->addElement((yyvsp[0].value));
        ;}
    break;

  case 298:
#line 2036 "gram.y"
    {
            (yyval.arrayConst) = (yyvsp[-2].arrayConst);
            (yyval.arrayConst)->addElement((yyvsp[0].value));
        ;}
    break;

  case 302:
#line 2046 "gram.y"
    {
            (yyval.value) = (yyvsp[-2].arrayConst);
        ;}
    break;

  case 303:
#line 2052 "gram.y"
    {
            (yyval.loc) = Location::null_location;
        ;}
    break;

  case 304:
#line 2056 "gram.y"
    {
            (yyval.loc) = Location::null_location;
        ;}
    break;

  case 314:
#line 2078 "gram.y"
    {
           TypeQual r((yyvsp[-3].typeQual));
           r.vout=(yyvsp[-1].value);
           (yyval.typeQual) = r;
        ;}
    break;

  case 316:
#line 2087 "gram.y"
    {
            (yyval.typeQual) = (yyvsp[-1].typeQual) | (yyvsp[0].typeQual);
            if (TQ_None != ((yyvsp[0].typeQual) & (yyvsp[-1].typeQual)))
                yywarn("Warning: qualifier already specified");
        ;}
    break;

  case 317:
#line 2095 "gram.y"
    {
            (yyval.typeQual) = TQ_None;
        ;}
    break;

  case 349:
#line 2138 "gram.y"
    {
            (yyval.base) = new BaseType(BT_UserType);
            (yyval.base)->typeName = (yyvsp[0].symbol);
        ;}
    break;

  case 350:
#line 2145 "gram.y"
    {
            assert ((! (yyval.symbol)->entry) ||
                    (yyval.symbol)->entry->IsTagDecl()) ;
        ;}
    break;

  case 351:
#line 2153 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                (yyval.base) = gProject->Parse_TOS->parseCtxt->Mk_tag_ref((yyvsp[-1].typeSpec), (yyvsp[0].symbol),
                                                                gProject->Parse_TOS->transUnit->contxt.tags);
            else
                (yyval.base) = NULL;
        ;}
    break;

  case 352:
#line 2163 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                (yyval.base) = gProject->Parse_TOS->parseCtxt->Mk_tag_ref((yyvsp[-1].typeSpec), (yyvsp[0].symbol),
                                                                gProject->Parse_TOS->transUnit->contxt.tags);
            else
                (yyval.base) = NULL;
        ;}
    break;

  case 353:
#line 2173 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                (yyval.base) = gProject->Parse_TOS->parseCtxt->Mk_tag_ref((yyvsp[-1].typeSpec), (yyvsp[0].symbol),
                                                                gProject->Parse_TOS->transUnit->contxt.tags);
            else
                (yyval.base) = NULL;
        ;}
    break;

  case 354:
#line 2183 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                (yyval.base) = gProject->Parse_TOS->parseCtxt->Mk_tag_def((yyvsp[-1].typeSpec), (yyvsp[0].symbol),
                                                            gProject->Parse_TOS->transUnit->contxt.tags);
            else
                (yyval.base) = NULL;
        ;}
    break;

  case 355:
#line 2193 "gram.y"
    {
            (yyval.base) = new BaseType((yyvsp[-1].strDef));
            (yyvsp[-1].strDef)->_isUnion = false;
        ;}
    break;

  case 356:
#line 2198 "gram.y"
    {
            (yyval.base) = (yyvsp[-3].base);
            assert (! (yyval.base)->stDefn);
            (yyval.base)->stDefn = (yyvsp[-1].strDef);
            (yyvsp[-1].strDef)->tag = (yyvsp[-3].base)->tag->dup();
            (yyvsp[-1].strDef)->_isUnion = false;

            // Overload the incomplete definition
            (yyval.base)->tag->entry->uStructDef = (yyval.base) ;

//             std::cout << "struct/union/enum_type_define:"
//                          "The definition of:"
//                       << "(uStructDef:" << $1->tag->entry->uStructDef << ")"
//                       << "(uStructDef->stDefn:" << $1->tag->entry->uStructDef->stDefn << ")"
//                       << "(" << $1->tag->entry << ")" << $1->tag->name  << "$" ;
//             $1->tag->entry->scope->ShowScopeId(std::cout);
//             std::cout << " has been completed" << endl;

        ;}
    break;

  case 357:
#line 2220 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                (yyval.base) = gProject->Parse_TOS->parseCtxt->Mk_tag_def((yyvsp[-1].typeSpec), (yyvsp[0].symbol),
                                                            gProject->Parse_TOS->transUnit->contxt.tags);
            else
              (yyval.base) = NULL ;
        ;}
    break;

  case 358:
#line 2230 "gram.y"
    {
            (yyval.base) = new BaseType((yyvsp[-1].strDef));
            (yyvsp[-1].strDef)->_isUnion = true;

        ;}
    break;

  case 359:
#line 2236 "gram.y"
    {
            (yyval.base) = (yyvsp[-3].base);
            assert (! (yyval.base)->stDefn);
            (yyval.base)->stDefn = (yyvsp[-1].strDef);
            (yyvsp[-1].strDef)->tag = (yyvsp[-3].base)->tag->dup();
            (yyvsp[-1].strDef)->_isUnion = true;

            // Overload the incomplete definition
            (yyval.base)->tag->entry->uStructDef = (yyval.base) ;

//             std::cout << "struct/union/enum_type_define:"
//                          "The definition of:"
//                       << "(uStructDef:" << $1->tag->entry->uStructDef << ")"
//                       << "(uStructDef->stDefn:" << $1->tag->entry->uStructDef->stDefn << ")"
//                       << "(" << $1->tag->entry << ")" << $1->tag->name  << "$" ;
//             $1->tag->entry->scope->ShowScopeId(std::cout);
//             std::cout << " has been completed" << endl;


        ;}
    break;

  case 360:
#line 2259 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                (yyval.base) = gProject->Parse_TOS->parseCtxt->Mk_tag_def((yyvsp[-1].typeSpec),(yyvsp[0].symbol),
                                                            gProject->Parse_TOS->transUnit->contxt.tags);
            else
              (yyval.base) = NULL;
        ;}
    break;

  case 361:
#line 2269 "gram.y"
    {
            (yyval.base) = new BaseType((yyvsp[-1].enDef));

        ;}
    break;

  case 362:
#line 2274 "gram.y"
    {
            (yyval.base) = (yyvsp[-3].base);
            assert (! (yyval.base)->enDefn);
            (yyval.base)->enDefn = (yyvsp[-1].enDef);
            (yyvsp[-1].enDef)->tag = (yyvsp[-3].base)->tag->dup();

            // Overload the incomplete definition
            (yyval.base)->tag->entry->uStructDef = (yyval.base) ;

//             std::cout << "struct/union/enum_type_define:"
//                          "The definition of:"
//                       << "(uStructDef:" << $1->tag->entry->uStructDef << ")"
//                       << "(uStructDef->stDefn:" << $1->tag->entry->uStructDef->stDefn << ")"
//                       << "(" << $1->tag->entry << ")" << $1->tag->name  << "$" ;
//             $1->tag->entry->scope->ShowScopeId(std::cout);
//             std::cout << " has been completed" << endl;

        ;}
    break;

  case 363:
#line 2295 "gram.y"
    {  (yyval.strDef) = new StructDef();
           yywarn("ANSI/ISO C prohibits empty struct/union");
        ;}
    break;

  case 365:
#line 2300 "gram.y"
    {
            (yyval.strDef) = (yyvsp[0].strDef);
        ;}
    break;

  case 366:
#line 2306 "gram.y"
    {  (yyval.enDef) = new EnumDef();
           yywarn("ANSI/ISO C prohibits empty enum");
        ;}
    break;

  case 367:
#line 2310 "gram.y"
    {  (yyval.enDef) = (yyvsp[-1].enDef);
        ;}
    break;

  case 368:
#line 2315 "gram.y"
    {
            (yyval.loc) = Location::null_location;
        ;}
    break;

  case 369:
#line 2319 "gram.y"
    {
          yywarn("Trailing comma in enum type definition");
        ;}
    break;

  case 371:
#line 2332 "gram.y"
    {
            (yyval.enDef) = new EnumDef();
            (yyval.enDef)->addElement((yyvsp[0].enConst));
        ;}
    break;

  case 372:
#line 2337 "gram.y"
    {
            (yyval.enDef) = (yyvsp[-2].enDef);
            (yyval.enDef)->addElement((yyvsp[0].enConst));
        ;}
    break;

  case 373:
#line 2344 "gram.y"
    {
            (yyval.enConst) = new EnumConstant((yyvsp[0].symbol),NULL,currentLocation);
            if (gProject->Parse_TOS->transUnit)
            {
              if (gProject->Parse_TOS->transUnit->contxt.syms->IsDefined((yyvsp[0].symbol)->name))
                 yyerr("Duplicate enumeration constant");

              (yyvsp[0].symbol)->entry = gProject->Parse_TOS->transUnit->contxt.syms->Insert(
                                  mk_enum_const((yyvsp[0].symbol)->name, (yyval.enConst)));
            }
        ;}
    break;

  case 374:
#line 2356 "gram.y"
    {
            (yyval.enConst) = new EnumConstant((yyvsp[-2].symbol),(yyvsp[0].value),currentLocation);
            if (gProject->Parse_TOS->transUnit)
            {
              if (gProject->Parse_TOS->transUnit->contxt.syms->IsDefined((yyvsp[-2].symbol)->name))
                 yyerr("Duplicate enumeration constant");

              (yyvsp[-2].symbol)->entry = gProject->Parse_TOS->transUnit->contxt.syms->Insert(
                                  mk_enum_const((yyvsp[-2].symbol)->name, (yyval.enConst)));
            }
        ;}
    break;

  case 376:
#line 2376 "gram.y"
    {
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.EnterScope();
            gProject->Parse_TOS->parseCtxt->PushCtxt();
        ;}
    break;

  case 377:
#line 2381 "gram.y"
    {
            assert (!err_top_level || possibleType);
             /* Safety precaution! */
             possibleType=true;
        ;}
    break;

  case 378:
#line 2387 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->PopCtxt(false);
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ExitScope();
            (yyval.strDef) = (yyvsp[0].strDef) ;
        ;}
    break;

  case 379:
#line 2396 "gram.y"
    {
            (yyval.strDef) = new StructDef();
            (yyval.strDef)->addComponent(ReverseList((yyvsp[-1].decl)));
        ;}
    break;

  case 380:
#line 2401 "gram.y"
    {
            // A useful gcc extension:
            //   naked semicolons in struct/union definitions.
            (yyval.strDef) = (yyvsp[-1].strDef);
            yywarn ("Empty declaration");
        ;}
    break;

  case 381:
#line 2408 "gram.y"
    {
            (yyval.strDef) = (yyvsp[-2].strDef);
            (yyval.strDef)->addComponent(ReverseList((yyvsp[-1].decl)));
        ;}
    break;

  case 382:
#line 2413 "gram.y"
    {
            (yyval.strDef) = (yyvsp[-1].strDef);
        ;}
    break;

  case 383:
#line 2419 "gram.y"
    {
            possibleType = true;
            (yyval.decl) = (yyvsp[0].decl);
        ;}
    break;

  case 384:
#line 2424 "gram.y"
    {
            possibleType = true;
            (yyval.decl) = new Decl ((yyvsp[0].base));
            yywarn ("No field declarator");
        ;}
    break;

  case 386:
#line 2438 "gram.y"
    {   gProject->Parse_TOS->parseCtxt->SetIsFieldId(true);
        ;}
    break;

  case 387:
#line 2441 "gram.y"
    {
            (yyval.decl) = (yyvsp[-1].decl);
            (yyval.decl)->attrib = (yyvsp[0].gccAttrib);
        ;}
    break;

  case 388:
#line 2446 "gram.y"
    {   gProject->Parse_TOS->parseCtxt->SetIsFieldId(true);
        ;}
    break;

  case 389:
#line 2449 "gram.y"
    {
            (yyval.decl) = (yyvsp[-1].decl);
            (yyval.decl)->attrib = (yyvsp[0].gccAttrib);
            (yyval.decl)->next = (yyvsp[-4].decl);
        ;}
    break;

  case 390:
#line 2457 "gram.y"
    {
           gProject->Parse_TOS->parseCtxt->SetIsFieldId(false);
           Type * decl = gProject->Parse_TOS->parseCtxt->UseDeclCtxt();
           Type * extended = (yyval.decl)->extend(decl);
           if ((yyval.decl)->form &&
               (yyval.decl)->form->isFunction())
               yyerr("Error: Function type not allowed as field");
           else if ((yyval.decl)->form &&
                    (yyval.decl)->form->isArray() &&
                    ! ((ArrayType *) (yyval.decl)->form)->size)
               yyerr("Error: Unsized array not allowed as field");
           else if (extended &&
               decl && decl->isFunction() &&
               ! extended->isPointer())
               yyerr("Error: Wrong type combination") ;

        ;}
    break;

  case 391:
#line 2475 "gram.y"
    {
           Type * decl = gProject->Parse_TOS->parseCtxt->UseDeclCtxt();
           (yyval.decl)->extend(decl);
           if (! decl)
               yyerr("Error: No type specifier for bit field") ;
           else if (!(yyval.decl)->form)
               yyerr("Error: Wrong type combination") ;
        ;}
    break;

  case 393:
#line 2489 "gram.y"
    {   gProject->Parse_TOS->parseCtxt->SetIsFieldId(false);
        ;}
    break;

  case 394:
#line 2492 "gram.y"
    {
            BitFieldType  *bf = new BitFieldType((yyvsp[0].value));
            (yyval.decl) = (yyvsp[-3].decl);

            if ((yyval.decl) == NULL)
            {
                (yyval.decl) = new Decl(bf);
            }
            else
            {
                bf->subType = (yyval.decl)->form;
                (yyval.decl)->form = bf;
            }
        ;}
    break;

  case 396:
#line 2512 "gram.y"
    {
           (yyval.decl) = (Decl*) NULL;
        ;}
    break;

  case 398:
#line 2523 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->Mk_declarator ((yyval.decl));
        ;}
    break;

  case 399:
#line 2529 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->Mk_func_declarator ((yyval.decl));
        ;}
    break;

  case 400:
#line 2535 "gram.y"
    {
            (yyval.decl) = (yyvsp[0].decl);
            (yyval.decl)->extend((yyvsp[-1].ptr));
        ;}
    break;

  case 403:
#line 2546 "gram.y"
    {  if (gProject->Parse_TOS->transUnit)
                (yyval.decl) = gProject->Parse_TOS->parseCtxt->Mk_direct_declarator_reentrance ((yyvsp[0].symbol),
                gProject->Parse_TOS->transUnit->contxt.syms);
        ;}
    break;

  case 404:
#line 2551 "gram.y"
    {
            (yyval.decl) = (yyvsp[-1].decl);
        ;}
    break;

  case 407:
#line 2557 "gram.y"
    {
            (yyval.decl) = (yyvsp[-3].decl);
            FunctionType * ft = new FunctionType(ReverseList((yyvsp[-1].decl)));
            Type * extended = (yyval.decl)->extend(ft);
            if (extended && ! extended->isPointer())
                yyerr("Error: Wrong type combination") ;

            // Exit, but will allow re-enter for a function.
            // Hack, to handle parameters being in the function's scope.
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ExitScope(true);

        ;}
    break;

  case 408:
#line 2571 "gram.y"
    {
            (yyval.decl) = (yyvsp[-3].decl);
            FunctionType * ft = new FunctionType(ReverseList((yyvsp[-1].decl)));
            Type * extended = (yyval.decl)->extend(ft);
            if (extended && ! extended->isPointer())
                yyerr("Wrong type combination") ;

            // Exit, but will allow re-enter for a function.
            // Hack, to handle parameters being in the function's scope.
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.ExitScope(true);

        ;}
    break;

  case 409:
#line 2585 "gram.y"
    {
            (yyval.decl) = (yyvsp[-2].decl);

                        if ((yyval.decl) != NULL)
                        {
                                FunctionType* ft = new FunctionType();
                                Type* extended = (yyval.decl)->extend(ft);
                                if (extended && ! extended->isPointer())
                            yyerr("Wrong type combination") ;
                        }

            if (gProject->Parse_TOS->transUnit)
            {
                gProject->Parse_TOS->transUnit->contxt.EnterScope();
                // Exit, but will allow re-enter for a function.
                // Hack, to handle parameters being in the function's scope.
                gProject->Parse_TOS->transUnit->contxt.ExitScope(true);
            }
        ;}
    break;

  case 410:
#line 2607 "gram.y"
    {
            (yyval.decl) = (yyvsp[-3].decl);
            ArrayType * at = new ArrayType(TT_Array, (yyvsp[-1].value));
            Type * extended = (yyval.decl)->extend(at);
            if (extended &&
                extended->isFunction())
                yyerr("Wrong type combination") ;

        ;}
    break;

  case 411:
#line 2619 "gram.y"
    {
            (yyval.decl) = (yyvsp[-3].decl);
            ArrayType * at = new ArrayType(TT_Stream, (yyvsp[-1].value));
            Type * extended = (yyval.decl)->extend(at);

            if (extended &&
                extended->isFunction())
                yyerr("Wrong type combination") ;
        ;}
    break;

  case 412:
#line 2630 "gram.y"
    {
            (yyval.value) = (yyvsp[0].consValue);
        ;}
    break;

  case 413:
#line 2634 "gram.y"
    {
            (yyval.value) = (yyvsp[0].value);
        ;}
    break;

  case 414:
#line 2638 "gram.y"
    {
           (yyval.value) = (yyvsp[-1].value);
        ;}
    break;

  case 415:
#line 2642 "gram.y"
    {
            (yyval.value) = new Variable ((yyvsp[0].symbol),currentLocation);
        ;}
    break;

  case 416:
#line 2648 "gram.y"
    {
           (yyval.value) = NULL;
        ;}
    break;

  case 417:
#line 2652 "gram.y"
    {
            (yyval.value) = (yyvsp[0].value);
        ;}
    break;

  case 418:
#line 2656 "gram.y"
    {
            (yyval.value) = new BinaryExpr(BO_Comma,(yyvsp[-2].value), (yyvsp[0].value), (yyvsp[-1].loc));
        ;}
    break;

  case 419:
#line 2665 "gram.y"
    {
            (yyval.ptr) = new PtrType((yyvsp[0].typeQual));
        ;}
    break;

  case 421:
#line 2672 "gram.y"
    {
            (yyval.ptr) = (yyvsp[0].ptr);
            (yyval.ptr)->subType = (yyvsp[-1].ptr);
        ;}
    break;

  case 423:
#line 2685 "gram.y"
    {  gProject->Parse_TOS->parseCtxt->IncrVarParam(1);
          if (gProject->Parse_TOS->transUnit)
              gProject->Parse_TOS->transUnit->contxt.EnterScope();
          gProject->Parse_TOS->parseCtxt->PushCtxt();
        ;}
    break;

  case 424:
#line 2691 "gram.y"
    {
          // Exit, but will allow re-enter for a function.
          // Hack, to handle parameters being in the function's scope.
          gProject->Parse_TOS->parseCtxt->PopCtxt(true);
          gProject->Parse_TOS->parseCtxt->IncrVarParam(-1);
          (yyval.decl) = (yyvsp[0].decl);
       ;}
    break;

  case 425:
#line 2701 "gram.y"
    {  if (gProject->Parse_TOS->transUnit)
               (yyval.decl) = gProject->Parse_TOS->parseCtxt->Mk_direct_declarator_reentrance ((yyvsp[0].symbol),
                gProject->Parse_TOS->transUnit->contxt.syms);
        ;}
    break;

  case 426:
#line 2706 "gram.y"
    {  (yyval.decl) = (yyvsp[-2].decl);
           if (gProject->Parse_TOS->transUnit)
           {
              (yyval.decl) = gProject->Parse_TOS->parseCtxt->Mk_direct_declarator_reentrance ((yyvsp[0].symbol),
                gProject->Parse_TOS->transUnit->contxt.syms);
              (yyval.decl)->next = (yyvsp[-2].decl);
           }
        ;}
    break;

  case 428:
#line 2722 "gram.y"
    {
            /* Convert a TYPEDEF_NAME back into a normal IDENT */
            (yyval.symbol) = (yyvsp[0].symbol);
            (yyval.symbol)->entry = (SymEntry*) NULL;
        ;}
    break;

  case 431:
#line 2743 "gram.y"
    {
           (yyval.decl) = (Decl*) NULL;
        ;}
    break;

  case 432:
#line 2747 "gram.y"
    { gProject->Parse_TOS->parseCtxt->IncrVarParam(1);
        ;}
    break;

  case 433:
#line 2750 "gram.y"
    { gProject->Parse_TOS->parseCtxt->IncrVarParam(-1);
           (yyval.decl) = (yyvsp[0].decl);
        ;}
    break;

  case 434:
#line 2756 "gram.y"
    {   gProject->Parse_TOS->parseCtxt->IncrVarParam(1);
            if (gProject->Parse_TOS->transUnit)
                gProject->Parse_TOS->transUnit->contxt.EnterScope();
            gProject->Parse_TOS->parseCtxt->PushCtxt();
        ;}
    break;

  case 435:
#line 2762 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->PopCtxt(true);
            gProject->Parse_TOS->parseCtxt->IncrVarParam(-1);
            (yyval.decl) = (yyvsp[0].decl) ;
        ;}
    break;

  case 437:
#line 2771 "gram.y"
    {
            BaseType *bt = new BaseType(BT_Ellipsis);

            (yyval.decl) = new Decl(bt);
            (yyval.decl)->next = (yyvsp[-2].decl);
        ;}
    break;

  case 439:
#line 2781 "gram.y"
    {
            (yyval.decl) = (yyvsp[0].decl);
            (yyval.decl)->next = (yyvsp[-2].decl);
        ;}
    break;

  case 440:
#line 2788 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->PushCtxt();
        ;}
    break;

  case 441:
#line 2792 "gram.y"
    {
            gProject->Parse_TOS->parseCtxt->PopCtxt(true);
            (yyval.decl) = (yyvsp[0].decl);
        ;}
    break;

  case 442:
#line 2799 "gram.y"
    {
            assert (err_top_level ||
                    (yyvsp[-1].base) == gProject->Parse_TOS->parseCtxt->curCtxt->decl_specs);
            possibleType = true;
            (yyval.decl) = (yyvsp[0].decl);
            Type * decl = gProject->Parse_TOS->parseCtxt->UseDeclCtxt();
            Type * extended = (yyval.decl)->extend(decl);
            if ((yyval.decl)->form &&
                (yyval.decl)->form->isFunction())
                yyerr("Function type not allowed");
            else if (extended &&
                decl && decl->isFunction() &&
                ! extended->isPointer())
                yyerr("Wrong type combination") ;
        ;}
    break;

  case 443:
#line 2815 "gram.y"
    {
            assert (err_top_level ||
                    (yyvsp[-1].base) == gProject->Parse_TOS->parseCtxt->curCtxt->decl_specs);
            possibleType = true;
            (yyval.decl) = new Decl((yyvsp[0].type));

            Type * decl = gProject->Parse_TOS->parseCtxt->UseDeclCtxt();
            Type * extended = (yyval.decl)->extend(decl);
            if ((yyval.decl)->form &&
                (yyval.decl)->form->isFunction())
                yyerr("Function type not allowed for parameter");
            else if (extended &&
                decl && decl->isFunction() &&
                ! extended->isPointer())
                yyerr("Wrong type combination") ;
        ;}
    break;

  case 444:
#line 2832 "gram.y"
    {
            possibleType = true;
            (yyval.decl) = new Decl((yyvsp[0].base));
            if ((yyval.decl)->form &&
                (yyval.decl)->form->isFunction())
                yyerr("Function type not allowed for parameter");
        ;}
    break;

  case 445:
#line 2845 "gram.y"
    {
            (yyval.type) = (yyvsp[0].ptr);
        ;}
    break;

  case 446:
#line 2849 "gram.y"
    {
            (yyval.type) = (yyvsp[0].type);
        ;}
    break;

  case 447:
#line 2853 "gram.y"
    {
            (yyval.type) = (yyvsp[0].type);
            (yyval.type)->extend((yyvsp[-1].ptr));
        ;}
    break;

  case 449:
#line 2863 "gram.y"
    {
            (yyval.type) = (yyvsp[-1].type);
        ;}
    break;

  case 450:
#line 2867 "gram.y"
    {
            (yyval.type) = new ArrayType(TT_Array, (yyvsp[-1].value));
        ;}
    break;

  case 451:
#line 2871 "gram.y"
    {
            ArrayType *at = new ArrayType(TT_Array, (yyvsp[-1].value));
            (yyval.type) = (yyvsp[-3].type);
            (yyval.type)->extend(at);
            Type * extended = (yyval.type)->extend(at) ;
            if (extended &&
                extended->isFunction())
                yyerr("Error: Wrong type combination") ;
        ;}
    break;

  case 452:
#line 2881 "gram.y"
    {
            (yyval.type) = new FunctionType(ReverseList((yyvsp[-1].decl)));
        ;}
    break;

  case 453:
#line 2885 "gram.y"
    {
            FunctionType * ft = new FunctionType(ReverseList((yyvsp[-1].decl)));
            (yyval.type) = (yyvsp[-3].type);
            Type * extended = (yyval.type)->extend(ft) ;
            if (extended &&
                ! extended->isPointer())
                yyerr("Error: Wrong type combination") ;

        ;}
    break;

  case 454:
#line 2901 "gram.y"
    {
            (yyval.gccAttrib) = (GccAttrib*) NULL;
        ;}
    break;

  case 457:
#line 2909 "gram.y"
    {
    (yyvsp[-1].gccAttrib)->next = (yyvsp[0].gccAttrib);
    (yyval.gccAttrib) = (yyvsp[-1].gccAttrib);
;}
    break;

  case 458:
#line 2915 "gram.y"
    {
                (yyval.gccAttrib) = (yyvsp[-2].gccAttrib);
            ;}
    break;

  case 459:
#line 2921 "gram.y"
    {
                yywarn("Unsupported GCC attribute");
                (yyval.gccAttrib) = new GccAttrib( GCC_Unsupported);
            ;}
    break;

  case 460:
#line 2926 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_Packed );
            ;}
    break;

  case 461:
#line 2930 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_CDecl );
            ;}
    break;

  case 462:
#line 2934 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_Const );
            ;}
    break;

  case 463:
#line 2938 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_NoReturn );
            ;}
    break;

  case 464:
#line 2942 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_NoThrow );
            ;}
    break;

  case 465:
#line 2946 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_Aligned );

                if ((yyvsp[-1].consValue)->ctype == CT_Int)
                {
                    IntConstant    *iCons = (IntConstant*) (yyvsp[-1].consValue);

                    (yyval.gccAttrib)->value = iCons->lng;
                }

            ;}
    break;

  case 466:
#line 2958 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_Mode );
                (yyval.gccAttrib)->mode = (yyvsp[-1].symbol);
            ;}
    break;

  case 467:
#line 2963 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_Format );

                (yyval.gccAttrib)->mode = (yyvsp[-5].symbol);

                if ((yyvsp[-3].consValue)->ctype == CT_Int)
                {
                    IntConstant    *iCons = (IntConstant*) (yyvsp[-3].consValue);

                    (yyval.gccAttrib)->strIdx = iCons->lng;
                }

                if ((yyvsp[-1].consValue)->ctype == CT_Int)
                {
                    IntConstant    *iCons = (IntConstant*) (yyvsp[-1].consValue);

                    (yyval.gccAttrib)->first = iCons->lng;
                }
            ;}
    break;

  case 468:
#line 2983 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( GCC_NonNull );
                (yyval.gccAttrib)->ns = (yyvsp[-1].inumlist);
            ;}
    break;

  case 469:
#line 2988 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib(GCC_Pure);
            ;}
    break;

  case 470:
#line 2992 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib(GCC_Malloc);
            ;}
    break;

  case 471:
#line 2996 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib(GCC_WarnUnusedResult);
            ;}
    break;

  case 472:
#line 3002 "gram.y"
    {
                (yyval.gccAttrib) = (yyvsp[-1].gccAttrib);
            ;}
    break;

  case 473:
#line 3008 "gram.y"
    {
                yywarn("Unsupported GCC attribute");
                (yyval.gccAttrib) = new GccAttrib( GCC_Unsupported);
            ;}
    break;

  case 474:
#line 3013 "gram.y"
    {
                (yyval.gccAttrib) = new GccAttrib( Kernel_GroupSize );
                (yyval.gccAttrib)->ns = (yyvsp[-1].inumlist);
            ;}
    break;

  case 475:
#line 3020 "gram.y"
    {
                  IntConstant *iCons = (IntConstant*) (yyvsp[0].consValue);
                  (yyval.inumlist) = new INumList(iCons->lng);
              ;}
    break;

  case 476:
#line 3025 "gram.y"
    {
                  IntConstant *iCons = (IntConstant*) (yyvsp[0].consValue);
                  (yyval.inumlist) = new INumList(iCons->lng);
                  (yyval.inumlist)->next = (yyvsp[-2].inumlist);
              ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 5770 "gram.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


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
	  yydestruct ("Error: discarding", yytoken, &yylval);
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 3032 "gram.y"


/*******************************************************/

