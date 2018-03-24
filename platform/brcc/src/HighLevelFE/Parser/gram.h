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




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





