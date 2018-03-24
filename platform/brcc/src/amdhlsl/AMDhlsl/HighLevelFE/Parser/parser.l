%option never-interactive
%option prefix="zz"

%option noyylineno
%option nostdinit
%option noyywrap
%option noinput
%option nounput
%option noyy_scan_buffer
%option noyy_scan_bytes
%option noyy_scan_string


%{
#include <stdio.h>
#include <string.h>
#include "parameter.hpp"
#include "parser.hpp"
#include "positionaware.hpp"
#include "assertlogging.hpp"
#include <fstream>


std::ostream* theDumpCppFile = 0;
PARAMETER_MUTABLE(p_dumpcpp, "dumpcpp");
inline static void debug_cpp_output(const char* buf, int len)
{
  if(p_dumpcpp)
  {
    if(theDumpCppFile==0) theDumpCppFile = new std::ofstream(p_dumpcpp.ValueStr());
    ASSERT_ERROR(theDumpCppFile->good())(p_dumpcpp).Text("Usage: ... --dumpcpp=filename ...");
    theDumpCppFile->write(buf,len);
  }
}

int yypreproclex( char* buf, int result, int max_len);

//this callback makes flex read from preprocessor rather than from input
#define YY_INPUT(buf,result,max_size) { result = yypreproclex(buf, 0, max_size); debug_cpp_output(buf,result); }
 
#define YY_USER_ACTION zzlval=yytext;


%}

KW_MODIFIERS extern|nointerpolation|shared|static|uniform|volatile|const|row_major|column_major|inline|target|linear|in|out|inout|triangleadj|triangle|point|line
KW_STREAMS   PointStream|LineStream|TriangleStream
KW_SAMPLERS  SamplerState|sampler|sampler_state|RasterizerState|BlendState|DepthStencilState|SamplerComparisonState|samplerCUBE

%%
{KW_MODIFIERS}                   return MODIFIER_KEYWORD;
{KW_STREAMS}                     return STREAM;
{KW_SAMPLERS}                    return SAMPLER;
[?:;,[\](){}]                    return *yytext; //special chars
[!~=.&|^%<>*/+-]                 return *yytext; //operators    
"<<"                             return LSHFT;
">>"                             return RSHFT;
"=="                             return EQ;
"!="                             return NE;
">="                             return GE;
"<="                             return LE;
"&&"                             return AND;
"||"                             return OR;
"++"                             return PLUSPLUS;
"--"                             return MINUSMINUS;
"return"                         return RETURN;
"struct"                         return STRUCT;
"if"                             return IF;
"else"                           return ELSE;
"for"                            return FOR;
"while"                          return WHILE;
"do"                             return DO;
"break"                          return BREAK;
"continue"                       return CONTINUE;
"register"                       return REGISTER;
"vector"                         return VECTOR;
"matrix"                         return MATRIX;
"persistent"                     return PERSISTENT;
"global"                         return GLOBAL;
"true"                           return KW_TRUE;
"false"                          return KW_FALSE;
"unsigned"                       return KW_UNSIGNED;
"discard"                        return DISCARD;
"void"                           return KW_VOID;
"maxvertexcount"                 return KW_MAX_VTXCNT;
"NumThreads"                     return KW_NUM_THREADS;
"LocalDataShareRel"              return KW_LDS_REL;
"LocalDataShareAbs"              return KW_LDS_ABS;
"LocalDataShare"                 return KW_LDS;
Texture[123]D"Array"?|Buffer     return TEXTURERXD;
TextureCube                      return TEXTURERXD;
[ct]buffer                       return CBUFFER;
0[Xx][0-9ABCDEFabcdef]+[Uu]?     return HEX;
[0-9]+[LlUu]?                    return INTEGER;
[0-9]+[.][0-9]+([Ee][+-]?[0-9]+)?[fF]   return FLOAT;
[0-9]+[.]([Ee][+-]?[0-9]+)?[fF]         return FLOAT;
[.][0-9]+([Ee][+-]?[0-9]+)?[fF]         return FLOAT;
[0-9]+([Ee][+-]?[0-9]+)[fF]             return FLOAT;
[0-9]+[.][0-9]+([Ee][+-]?[0-9]+)?   return DOUBLE;
[0-9]+[.]([Ee][+-]?[0-9]+)?         return DOUBLE;
[.][0-9]+([Ee][+-]?[0-9]+)?         return DOUBLE;
[0-9]+([Ee][+-]?[0-9]+)             return DOUBLE;
[a-zA-Z_][a-zA-Z0-9_]*           return IDENT;
["]([^"\n]|\\\")*["]             return STRING;
[']([^']|\\.)[']                 return CHAR;
[ \t\r\n]+                       /* whitespace */;
.                                NOTICE("lex missed something???")((const char*)yytext);

%%
