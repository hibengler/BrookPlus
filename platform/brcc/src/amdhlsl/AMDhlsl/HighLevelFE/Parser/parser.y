%{
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


%}

%token_table

//tokens returned from flex
%token MODIFIER_KEYWORD SAMPLER CBUFFER STREAM TEXTURERXD KW_UNSIGNED KW_VOID
%token KW_MAX_VTXCNT KW_NUM_THREADS KW_LDS_REL KW_LDS_ABS KW_LDS
%token PERSISTENT GLOBAL KW_TRUE KW_FALSE DISCARD
%token IDENT STRING CHAR INTEGER HEX FLOAT DOUBLE STRUCT VECTOR MATRIX
%token EQ NE LE GE LSHFT RSHFT AND OR PLUSPLUS MINUSMINUS 
%token IF ELSE FOR WHILE DO RETURN BREAK CONTINUE REGISTER

//not real ops... used to define precedence
%token PRE_UNARY_OP POST_UNARY_OP PREC_LAST PREC_FIRST BOGUS

%left PREC_LAST
//operator precedence
%left ','
%right '='
%right '?' ':'
%left OR 
%left AND
%left '|'
%left '^'
%left '&'
%left EQ NE
%left '<' '>' LE GE
%left LSHFT RSHFT
%left '+' '-'
%left '*' '/' '%'
%right '!' '~' PLUSPLUS MINUSMINUS PRE_UNARY_OP
%left '.' '[' ']' POST_UNARY_OP 
%right '(' ')'
%left PREC_FIRST

%expect 1

%%

//Catches root of tree and export result to AST::Root::TheCurrentTreeRoot();
root: declaration_list
{
  //Construct root node
  AST::Root* root = new AST::Root();
  //Make sure to keep refcount > 0
  AST::TreeElementPtr rootPtr = root;
  
  //Add all the child nodes
  ParserVariant(rootPtr).AddChildren($1);
  
  //Export it to a global variable (until i find a more elegant way to exit bison)
  AST::Root::TheCurrentTreeRoot() = root; 
};

semicol: ';'    { $$ = $1 }
   //    | error  { DIE("Parse error.  Expected ';'.")("got",$1.ToString()); }
       ;
       
declaration_list: /*empty*/                     { $$ = EMPTY   }
	            | declaration_list declaration  { $$ = $1 + $2 }
	            ;
	            
declaration: multi_var_decl semicol   { $$ = $1    }
           | texture_declaration      { $$ = $1    }
           | function_declaration     { $$ = $1    }
           | struct_declaration       { $$ = $1    }
           | sampler_declaration      { $$ = $1    }
           | specialalloc_declaration { $$ = $1    }
           | semicol                  { $$ = EMPTY }
           | error                    { DIE("Parse error. Expected declaration.")($1); } 
           ;


arg_list: /*empty*/           { $$ = new AST::ArgList(); }
        | arg_list_nonempty   { $$ = $1; }
        ;

arg_list_nonempty: expr               { $$ = new AST::ArgList(); $$.AddChildren($1) }
                 | arg_list ',' expr  { $$ = $1;                 $$.AddChildren($3) }
                 ;
               
//All binary ops are resolved here
//See operator precedence above
binary_expr: 
     expr '=' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::Assign >($1,$3) }
   | expr '&' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::BitAnd >($1,$3) }
   | expr '|' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::BitOr  >($1,$3) }
   | expr '^' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::BitXor >($1,$3) }
   | expr '*' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::Mult   >($1,$3) }
   | expr '/' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::Div    >($1,$3) }
   | expr '%' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::Mod    >($1,$3) }
   | expr '+' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::Plus   >($1,$3) }
   | expr '-' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::Minus  >($1,$3) }
   | expr LSHFT expr { $$ = AST::MakeBinaryOp< AST::BinaryOps::LShift >($1,$3) }
   | expr RSHFT expr { $$ = AST::MakeBinaryOp< AST::BinaryOps::RShift >($1,$3) }
   | expr '<' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::CmpLT  >($1,$3) }
   | expr '>' expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::CmpGT  >($1,$3) }
   | expr LE  expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::CmpLE  >($1,$3) }
   | expr GE  expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::CmpGE  >($1,$3) }
   | expr EQ  expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::CmpEQ  >($1,$3) }
   | expr NE  expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::CmpNE  >($1,$3) }
   | expr AND expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::And    >($1,$3) }
   | expr OR  expr   { $$ = AST::MakeBinaryOp< AST::BinaryOps::Or     >($1,$3) }
   | expr '&' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::BitAnd >($1,$4) }
   | expr '|' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::BitOr  >($1,$4) }
   | expr '^' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::BitXor >($1,$4) }
   | expr '*' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::Mult   >($1,$4) }
   | expr '/' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::Div    >($1,$4) }
   | expr '%' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::Mod    >($1,$4) }
   | expr '+' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::Plus   >($1,$4) }
   | expr '-' '=' expr   %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::Minus  >($1,$4) }
   | expr LSHFT '=' expr %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::LShift >($1,$4) }
   | expr RSHFT '=' expr %prec '=' { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::RShift >($1,$4) } 
         ;
  
//now all the unary ops       
unary_expr: '!' expr               %prec PRE_UNARY_OP 
  { $$ = new AST::UnaryOps::LogNot();   $$.AddChildren($2); };
         
unary_expr: '~' expr               %prec PRE_UNARY_OP 
  { $$ = new AST::UnaryOps::BitNot();   $$.AddChildren($2); };
                
unary_expr: '-' expr               %prec PRE_UNARY_OP 
   { $$ = new AST::UnaryOps::Negate();   $$.AddChildren($2); };
                 
unary_expr: '+' expr               %prec PRE_UNARY_OP 
   { $$=$1 };
                
unary_expr:  PLUSPLUS expr          %prec PRE_UNARY_OP 
  { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::Plus >($2, AST::MakeIntegerLiteral("1")) };
                
unary_expr:  MINUSMINUS expr        %prec PRE_UNARY_OP 
  { $$ = AST::MakeBinaryOpAssign< AST::BinaryOps::Minus >($2, AST::MakeIntegerLiteral("1")) };
                
unary_expr:  expr PLUSPLUS         %prec POST_UNARY_OP 
  { 
    $$ = AST::MakeBinaryOpPostAssign< AST::BinaryOps::Plus >($1, AST::MakeIntegerLiteral("1"));
  };
                
unary_expr:  expr MINUSMINUS       %prec POST_UNARY_OP 
  { 
$$ = AST::MakeBinaryOpPostAssign< AST::BinaryOps::Minus >($1, AST::MakeIntegerLiteral("1"));
  };
                
//unary_expr:  '(' data_type ')' expr %prec PRE_UNARY_OP 
//    { $$ = $2; };
         
          
snglexpr: KW_TRUE       { $$ = new AST::NumericLiteral<unsigned int, RegDT::Bool>(0xFFFFFFFF) }
        | KW_FALSE      { $$ = new AST::NumericLiteral<unsigned int, RegDT::Bool>(0x00000000) }
        | INTEGER       { $$ = AST::MakeIntegerLiteral($1); }
        | HEX           { $$ = AST::MakeHexLiteral($1); }
        | FLOAT         { $$ = AST::MakeFloatLiteral($1);   }
        | DOUBLE        { $$ = AST::MakeDoubleLiteral($1);  }
        | dlIDENT       { $$ = $1 }       
        | STRING        { ERROR_UNIMPLEMENTED().Text("Strings"); }
        | CHAR          { $$ = AST::MakeCharLiteral($1);   }  
        | '(' expr ')'  { $$=$2 }      
        | func_call     { $$=$1 }   
;

expr: snglexpr                { $$ = $1 }
    | binary_expr             { $$ = $1 }
    | unary_expr              { $$ = $1 }
    | expr '?' expr ':' expr  { $$ = new AST::FunctionCall();
				YYSTYPE al = new AST::ArgList();
                                al.AddChildren( $1 );
                                al.AddChildren( $3 );
                                al.AddChildren( $5 );
				$$.AddChildren( new AST::DelayedLookup( ConstString::Lookup( "cmov" ) ) + al );
			      }
    | '(' expr ')' expr       { $$ = new AST::FunctionCall(); $$.AddChildren($2 + new AST::ArgList($4)); }
    | error { DIE("Parse error. Invalid expression.")($1.ToString()); }  
    ;

dlIDENT: IDENT { $$ = new AST::DelayedLookup($1); };

func_call: dlIDENT '(' arg_list ')'        { $$ = new AST::FunctionCall(); $$.AddChildren($1 + $3) }    
         | snglexpr '.' IDENT { $$ = new AST::BinaryOps::MemberAccess($3); $$.AddChildren($1) }
         | snglexpr '[' expr ']'       { $$ = new AST::ArrayIndex();   $$.AddChildren($1 + $3) }  
         | snglexpr '.' IDENT '(' arg_list ')'
           {
           //this syntax is to support .Load(), Sample(), etc for DX10 Textures
           //rather than adding full support for method calls we merely transform
           // a.Foo(b,c) to __objcall_Foo(a,b,c)
           $$ = new AST::FunctionCall();
           $$.AddChildren(new AST::DelayedLookup(ConstString::Lookup("__objcall_" + $3.Str())));
           $5.Elmt()->PrependChild($1);
           $$.AddChildren($5);
           }
         ;

param_list_item: var_decl             { $$ = $1 }
               | SAMPLER sampler_name { $$ = $2 }
               ;

param_list: /* empty */          { $$ = EMPTY }     
          | KW_VOID              { $$ = EMPTY }     
          | param_list_nonempty  { $$ = $1 }
          ;

param_list_nonempty: param_list_item                { $$ = $1 }
                   | param_list ',' param_list_item { $$ = $1 + $3 }
          ;
          
variable_declaration_list: /* empty */ { $$ = EMPTY }
        | variable_declaration_list var_decl semicol { $$ = $1 + $2 }
                         ;
           
return_statement: RETURN expr { $$ = new AST::UnaryOps::Return(); $$.AddChildren($2) }
                | RETURN 
{ 
  $$ = new AST::UnaryOps::Return();
  $$.AddChildren(new AST::TypeCaster(AST::DataType::TheVoidType(), AST::TreeElementList() ));
};
discard_statement: DISCARD   { $$ = new AST::DiscardStatement(); };
break_statement: BREAK { $$ = new AST::BreakStatement(); };
continue_statement: CONTINUE { $$ = new AST::ContinueStatement(); };

if_statement: IF '(' expr ')' loopbody 
{
  $$ = new AST::IfStatement($3,$5);
};

if_statement: IF '(' expr ')' loopbody ELSE loopbody
{
  $$ = new AST::IfStatement($3,$5,$7);
};

expr_or_true: expr          { $$ = $1 }
             | /* empty */  { $$ = AST::MakeIntegerLiteral("1") }
             ;
             
expr_or_vdcl: arg_list  { $$ = $1 }
            | var_decl  { $$ = $1 }
;

loop_statement: FOR '(' expr_or_vdcl semicol expr_or_true semicol arg_list ')' loopbody
{
  $$ = AST::LoopStatement::MakeFor($3,$5,$7,$9);
};

loop_statement: WHILE '(' expr ')' loopbody
{
  $$ = AST::LoopStatement::MakeWhile($3,$5);
};

loop_statement: DO loopbody WHILE '(' expr ')' semicol 
{
  $$ = AST::LoopStatement::MakeDo($2, $5);
};
           
statement_list: /* empty */                 { $$ = new AST::StatementList(); }
              | statement_list multi_var_decl semicol { $$ = $1; $$.AddChildren($2) }
              | statement_list loopbody     { $$ = $1; $$.AddChildren($2) }
;              

loopbody: expr semicol                { $$ = $1 }
        | return_statement semicol    { $$ = $1 }
        | discard_statement semicol   { $$ = $1 }
        | break_statement semicol     { $$ = $1 }
        | continue_statement semicol  { $$ = $1 }
        | cmplr_opt if_statement      { $$ = $2 }
        | cmplr_opt loop_statement    { $$ = $2 }
        | semicol                     { $$ = new AST::NullTreeElement() }
        | '{' statement_list '}'      { $$ = $2 }
        ;
        

lds_info_list: lds_info
             | lds_info ',' lds_info_list
             ;

lds_info: KW_LDS '(' expr ')' 
          {
            gLdsInfo.lds_size = ($3.Elmt()->SimplifyToInt());                 
          }
        | KW_LDS_ABS 
          { 
            gLdsInfo.lds_abs = 1;
          }
        | KW_LDS_REL 
          { 
            gLdsInfo.lds_rel = 1;
          }
        | KW_NUM_THREADS '(' expr ')' 
          {
            gLdsInfo.numT[0] = ($3.Elmt()->SimplifyToInt());
            gLdsInfo.numT[1] = 1;
            gLdsInfo.numT[2] = 1;
          }
        | KW_NUM_THREADS '('  expr ',' expr ')' 
          {
            gLdsInfo.numT[0] = ($3.Elmt()->SimplifyToInt());
            gLdsInfo.numT[1] = ($5.Elmt()->SimplifyToInt());
            gLdsInfo.numT[2] = 1;
          }
        | KW_NUM_THREADS '('  expr ',' expr ',' expr ')' 
          {
            gLdsInfo.numT[0] = ($3.Elmt()->SimplifyToInt());
            gLdsInfo.numT[1] = ($5.Elmt()->SimplifyToInt());
            gLdsInfo.numT[2] = ($7.Elmt()->SimplifyToInt());
          }
        ;

func_decl_hdr2: '[' KW_MAX_VTXCNT '(' expr ')' ']'  func_decl_hdr2
               {
                 $$ = $7;
                 $$.Elmt()->AsFunction().SetMaxVertexCount($4.Elmt()->SimplifyToInt());
               }
              | '[' lds_info_list ']'  func_decl_hdr2
               {
                 $$ = $4;
                 $$.Elmt()->AsFunction().SetLdsSize(gLdsInfo.lds_size);
                 $$.Elmt()->AsFunction().SetLdsAbs(gLdsInfo.lds_abs);
                 $$.Elmt()->AsFunction().SetLdsRel(gLdsInfo.lds_rel);
                 $$.Elmt()->AsFunction().SetNumThreads(gLdsInfo.numT[0],
                                                       gLdsInfo.numT[1],
                                                       gLdsInfo.numT[2]  );
               }
              ;

func_decl_hdr2: modifier func_decl_hdr2 
{
  ($$=$2).Elmt()->AddModifier($1)
};

func_decl_hdr2: data_type function_name '(' param_list ')' 
{
  $$ = new AST::FunctionDeclaration($2, $1, $4.Size());
  $$.AddChildren($4);
}; 

func_decl_hdr: func_decl_hdr2 {$$=$1; gLdsInfo.reset();} 
             | func_decl_hdr modifier
            {
              ($$=$1).Elmt()->AddModifier($2)
            };


function_declaration: func_decl_hdr '{' statement_list '}' { ($$=$1).AddChildren($3); };
                     




data_type: IDENT { $$ = $1;  }
         | VECTOR '<' IDENT ',' INTEGER '>' { $$ = $3.Str() + $5.Str(); }
         | VECTOR { $$ = "float4"}
         | MATRIX '<' IDENT ',' INTEGER ',' INTEGER '>' { $$ = $3.Str() + $5.Str() + "x" + $7.Str(); }
         | MATRIX { $$ = "float4x4"; }
         | STREAM '<' IDENT '>' { $$ = $3 }
         | KW_UNSIGNED IDENT    { $$ = "u" + $2.Str(); }
         | KW_VOID              { $$ = $1 }
         ;

function_name: IDENT { $$ = $1 };


variable_name: IDENT array_size initializer
{
  $$ = new AST::VariableDeclaration($1, $2);
  $$.AddChildren($3);
};

variable_name: IDENT initializer
{
  $$ = new AST::VariableDeclaration($1);
  $$.AddChildren($2);
};

variable_name_list: variable_name {$$=$1}
                  | variable_name_list ',' variable_name {$$=$1+$3};

multi_var_decl_root: multi_var_decl_root modifier { ($$=$1).Elmt()->AddModifier($2) }
                   | data_type variable_name_list        
                      { 
                        const AST::TreeElementList& lst = $$ = $2;
                        for(AST::TreeElementList::const_iterator i = lst.begin(); i != lst.end(); ++i)
                          (*i)->AsVarDecl().SetTypeStr($1);
                      };
multi_var_decl: multi_var_decl_root            {  $$=$1 }
              | modifier multi_var_decl { ($$=$2).Elmt()->AddModifier($1) };
        
        
//Single variable declaration
var_decl_root: var_decl_root modifier { ($$=$1).Elmt()->AddModifier($2) }
             | data_type variable_name       { ($$=$2).Elmt()->AsVarDecl().SetTypeStr($1); };
var_decl: var_decl_root            {  $$=$1 }
        | modifier var_decl { ($$=$2).Elmt()->AddModifier($1) };

                         
initializer: /*empty*/                    { $$ = EMPTY }
           | '=' expr                     { $$ = $2 } 
           | '=' '{' initalizer_list '}'  { $$ = $3 };
           
initalizer_list: /*empty*/          {$$ = new AST::Initializer();}
               | initalizer_list_ne {$$ = $1};
                  
initalizer_list_ne: initalizer_list_item                     { $$ = new AST::Initializer(); $$.AddChildren($1) }
                  | initalizer_list_ne ',' initalizer_list_item { $$ = $1; $$.AddChildren($3) };
                  //SUPPORT A TYPO IN: leather_dx10sqk_hlsl:5027
                  | initalizer_list_ne ','  { $$ = $1 };
                  
initalizer_list_item: expr                    { $$ = $1 }
                    | '{' initalizer_list '}' { $$ = $2 };
                    
                    
array_size: '[' ']'        { $$=AST::VariableDeclaration::UnspecifiedSize(); }
          | '[' expr ']'   { $$=$2; };
                                
optional_register_spec: /*empty*/                  { $$="" } 
                      | ':' REGISTER '(' IDENT ')' { $$="" };

struct_declaration: STRUCT IDENT '{' variable_declaration_list '}'
{
  $$ = new AST::StructDeclaration($2);
  $$.AddChildren($4);
};


texture_declaration: TEXTURERXD '<' IDENT '>'  variable_name  semicol { ($$=$5).Elmt()->AsVarDecl().SetTypeStr($1, $3); };
                   | TEXTURERXD variable_name semicol                
                   { 
                      STATIC_CONSTSTRING(f4,"float4");
                      ($$=$2).Elmt()->AsVarDecl().SetTypeStr($1, f4); 
                   };



sampler_declaration: sampler_dcl_body '}' { $$ = $1 }
                   | SAMPLER sampler_name semicol { $$ = $2 }
                   ;

            
            
sampler_name: IDENT { $$ = new AST::SamplerDeclaration($1); };
            
                  // dx10 syntax:  
sampler_dcl_prefix: SAMPLER sampler_name                     {$$=$2}
                  | SAMPLER sampler_name '[' expr ']'        {$$=$2; $$.Elmt()->AsSampler().SetSamplerArraySize($4.Elmt()->SimplifyToInt()); }
                  //and the dx9 syntax:
                  | SAMPLER sampler_name '=' SAMPLER {$$=$2}
                  ;
                  
                  
sampler_dcl_body: sampler_dcl_prefix '{'                   {$$=$1; }
                | sampler_dcl_body IDENT '=' expr ';'      {$$=$1; $$.Elmt()->AsSampler().SetSamplerAttrb($2,$4); }
                | sampler_dcl_body IDENT '=' '<' dlIDENT '>' ';' {$$=$1; $$.Elmt()->AsSampler().SetSamplerAttrb($2,$5); }
                | sampler_dcl_body IDENT '[' expr ']' '=' expr ';' {$$=$1; $$.Elmt()->AsSampler().SetSamplerAttrb($2,$7); }
                ;

optional_ident: /*empty*/ { $$="" }
              | IDENT     { $$=$1 };
                
specialalloc_declaration: CBUFFER IDENT optional_register_spec '{' declaration_list '}' 
  { 
    $$ = new AST::ConsBuffScope($2);
    $$.AddChildren($5);
  };
    
specialalloc_declaration: PERSISTENT optional_ident '{' declaration_list '}' 
  {
    $$ = new AST::PersistentAllocScope($2);
    $$.AddChildren($4);
  };
  
specialalloc_declaration: PERSISTENT var_decl semicol
{
  $$ = new AST::PersistentAllocScope();
  $$.AddChildren($2);
};

specialalloc_declaration: GLOBAL optional_ident '{' declaration_list '}' 
{
  $$ = new AST::GlobalAllocScope($2);
  $$.AddChildren($4);
};
specialalloc_declaration: GLOBAL var_decl semicol
{
  $$ = new AST::GlobalAllocScope();
  $$.AddChildren($2);
};

cmplr_opt: /*empty*/                  { $$ = "" } 
         | '[' IDENT '(' expr ')' ']' { $$ = $2 }
         | '[' IDENT ']'              { $$ = $2 };


modifier: MODIFIER_KEYWORD           { $$ = AST::Modifier($1, AST::Modifier::MOD_KEYWORD);  }
        | ':' IDENT                  { $$ = AST::Modifier($2, AST::Modifier::MOD_SEMANTIC); }
        | ':' REGISTER '(' IDENT ')' { $$ = AST::Modifier($4, AST::Modifier::MOD_REGISTER); };
