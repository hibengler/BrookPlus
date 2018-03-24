%option never-interactive
%option prefix="ppyy"
%option noyylineno
%option nostdinit
%option noyywrap
%option noyy_scan_buffer
%option noyy_scan_bytes
%option noyy_scan_string

%{
#include <stdio.h>
#include <string.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

#include "preprocessor.h"
#include "project.h"

%}

%x comment
%x brook_file_on
%x brook_file_off

%{

extern std::istream *yyinstream;
static Preprocessor thePreprocessor;

void SetBrccInputStream(std::istream* input)
{
  YY_NEW_FILE;
  yyinstream = input;
  thePreprocessor = Preprocessor();
  
  if(yyinstream!=0)
  {
    thePreprocessor.AtStartOfBrFile();
    BEGIN(brook_file_on); 
  }
}

#define YY_DECL int yypplex( char* buf, int result, int max_len)
YY_DECL;

inline int readFromBrFile(char* buf, int max_size)
{
  if(yyinstream == NULL)
  {
    LOG(LOG_ERROR, "Internal Error : yyinstream is not initialized \n File(%s): %s \n", __LINE__, __FILE__);
    return 0;
  }
  
  //this needs to be optimized
  int c=0,n=0;
  while( n<max_size && !yyinstream->eof() && (c=yyinstream->get())!=EOF && c!=0)
    buf[n++] = (char) c;
  return n;
}

//redefine some flex macros to redirect output to a provided buffer

#define FORCE_RETURN        { if(result>0) return result; }
#define yyterminate()       return result

#define ASSERT_SPACE_FOR(n) if((int)n + result >= max_len) \
                            { \
                                LOG(LOG_ERROR, "Internal Error : buffer overflow, line too long!!!\n File(%s): %s \n", __LINE__, __FILE__); \
                            }
#define ECHO_STR(str, n)    if(thePreprocessor.IsActive()) \
                            { \
                                ASSERT_SPACE_FOR(n); \
                                memcpy(buf,str,n); \
                                buf+=n; \
                                result+=n; \
                            } \
                            else \
                            { \
                            }
#define ECHO                ECHO_STR(yytext,yyleng)

#define YY_INPUT(buf,result,max_size) result = readFromBrFile( buf, max_size );

%}
DIGITS     [0-9]
INT_NUM    ([0-9]{DIGITS}*)
IDENT     [a-zA-Z_][a-zA-Z0-9_]*
WS        [ \t]+
RESTLINE  ([^\n]|\\\n)*
POUND     ^[ \t]*[#][ \t]*
END       ^end[ \t\r]* 

%%

<brook_file_on>{
  
  {POUND}define{WS}{IDENT}{RESTLINE}  thePreprocessor.HandleDefine(yytext);
  {POUND}undef{WS}{IDENT}{RESTLINE}   thePreprocessor.HandleUndef(yytext);
  {POUND}ifdef{WS}{IDENT}{RESTLINE}   thePreprocessor.HandleIfdef(yytext);
  {POUND}ifndef{WS}{IDENT}{RESTLINE}  thePreprocessor.HandleIfndef(yytext);
  {POUND}if{WS}{INT_NUM}{RESTLINE}    thePreprocessor.HandleIf(yytext, BRPP_ConstIntType);
  {POUND}if{WS}{IDENT}{RESTLINE}      thePreprocessor.HandleIf(yytext, BRPP_IdentifierType);
  {POUND}else{RESTLINE}               thePreprocessor.HandleElse(yytext);
  {POUND}endif{RESTLINE}              thePreprocessor.HandleEndif(yytext);
  {POUND}line{RESTLINE}               thePreprocessor.HandleLine(yytext);
  {POUND}include{RESTLINE}            thePreprocessor.HandleInclude(yytext); 
  {POUND}{RESTLINE}                   thePreprocessor.HandleUnknownDirective(yytext);
  
  {END} {
    thePreprocessor.AtEndOfBrFile();
    thePreprocessor = Preprocessor(); //reset state
    BEGIN(INITIAL); //end of shader
    if(result != 0)
    {
        LOG(LOG_ERROR, "Internal Error : result = %d \n File(%s): %s \n", result, __LINE__, __FILE__); 
    }
    yyterminate();
  }
  
  {IDENT} {
    static int loopCount = 0;
    static int loopCountPosition = gProject->Parse_TOS->yylineno;
    std::string sym = yytext;
    if(thePreprocessor.ShouldReplaceSymbol(sym))
    {
      int numReplacements = 0;
      sym = thePreprocessor.ReplaceSymbol(sym, &yyinput, numReplacements);
      if(numReplacements == 0)
      { //no changes made... don't reprocess it
        ECHO_STR(sym.c_str(),(int)sym.length());
      }
      else
      {
        for( int i = (int)sym.length()-1; i>=0; --i)
        {
          unput(sym[i]); //reprocess the output as input
        }
      }
      if(loopCount++ >= 100)
      {
        LOG(LOG_ERROR, "Internal Error : max iterations reached in preprocessor\n File(%s): %s \n", __LINE__, __FILE__); 
      }
    }
    else
    {
      ECHO_STR(sym.c_str(), (int)sym.length());
      if(loopCount > 0 && loopCountPosition != gProject->Parse_TOS->yylineno) 
      {
        loopCount = 0;
        loopCountPosition = gProject->Parse_TOS->yylineno;
      }
    }
  }
  
  \n {
        if(thePreprocessor.IsActive())
        {
            ECHO;
            FORCE_RETURN;      

        }
        else
        {
            gProject->Parse_TOS->yylineno++;
            gProject->Parse_TOS->yynxtcol = 0;    
        }
    }

  "/*"                            BEGIN(comment);   
  [ \t]*[#][#][ \t]*              // just swallow up the concat operator
  "//"[^\n]*                      //ignore inside comments
  ["]([^"\n]|\\["])*["]           ECHO; //ignore inside strings
  [ \t\r]+                        ECHO; //'fast-forward' through whitespace
  .                               ECHO; //default case
}

<comment>{
  [*]+[/]                BEGIN(brook_file_on);  
  [*]+                   /*inside a comment*/
  [^*\n]+                /*inside a comment*/
  <<EOF>>                {
                            LOG(LOG_ERROR, "Internal Error : expected '*/' before end of the file \n File(%s): %s \n", __LINE__, __FILE__); 
                            yyterminate();
                         }
}

<brook_file_off>{
  {END}                  BEGIN(INITIAL); //end of shader
  [^\n]+                 /* line inside disabled shader */
}

<INITIAL,comment,brook_file_off>\n            {
                                                gProject->Parse_TOS->yylineno++;
                                                gProject->Parse_TOS->yynxtcol = 0;
                                              }
<INITIAL,brook_file_on,brook_file_off><<EOF>>     yyterminate();
[^\n]+                                    /* a line outside the "shader..end" block */


%%
