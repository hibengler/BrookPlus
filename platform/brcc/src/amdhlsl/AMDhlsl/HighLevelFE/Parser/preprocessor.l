%option never-interactive
%option prefix="yypreproc"
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

#include "parser.hpp"
#include "assertlogging.hpp"
#include "positionaware.hpp"
#include "hlfrontendapi.hpp"
#include "preprocessorstate.hpp"
#include "parameter.hpp"

PARAMETER(p_select, "select");

%}

%x comment
%x shader_on
%x shader_off

%{

static std::string       theCurrentShaderName;
static std::istream*     theInputFile = 0;
static PreprocessorState thePreprocessorState;

void SetInputStream(std::istream* input)
{
  YY_NEW_FILE;
  theInputFile = input;
  thePreprocessorState = PreprocessorState();
  
  if(theInputFile!=0)
  {
    theParserHadError = false;
    thePreprocessorState.AtStartOfShader();
    BEGIN(shader_on); 
  }
}

#define YY_DECL int yypreproclex( char* buf, int result, int max_len)
YY_DECL;

const std::string& PreprocScrollToNextShader()
{
  theCurrentShaderName = "";
  BEGIN(INITIAL);
  static char tmp[1024];
  yypreproclex(tmp, 0, sizeof(tmp));
  return theCurrentShaderName;
}

inline int readFromInput(char* buf, int max_size)
{
  DEBUG_CHECK(theInputFile!=0);
  //this needs to be optimized
  int c=0,n=0;
  while( n<max_size && !theInputFile->eof() && (c=theInputFile->get())!=EOF && c!=0)
    buf[n++] = (char) c;
  return n;
}

//redefine some flex macros to redirect output to a provided buffer

#define FORCE_RETURN        { if(result>0) return result; }
#define yyterminate()       return result

#define ASSERT_SPACE_FOR(n) ASSERT_ERROR((int)(n)+result<max_len)(n)(result)(max_len).Text("buffer overflow, line too long!!!")
#define ECHO_STR(str, n)    if(thePreprocessorState.IsActive()){ ASSERT_SPACE_FOR(n); memcpy(buf,str,n); buf+=n; result+=n; }else{}
#define ECHO                ECHO_STR(yytext,yyleng)

#define YY_INPUT(buf,result,max_size) result = readFromInput( buf, max_size );

%}

IDENT     [a-zA-Z_][a-zA-Z0-9_]*
WS        [ \t]+
RESTLINE  ([^\n]|\\\n)*
POUND     ^[ \t]*[#][ \t]*
END       ^end[ \t\r]* 

%%

<shader_on>{
  
  {POUND}define{WS}{IDENT}{RESTLINE}  thePreprocessorState.HandleDefine(yytext);
  {POUND}undef{WS}{IDENT}{RESTLINE}   thePreprocessorState.HandleUndef(yytext);
  {POUND}ifdef{WS}{IDENT}{RESTLINE}   thePreprocessorState.HandleIfdef(yytext);
  {POUND}ifndef{WS}{IDENT}{RESTLINE}  thePreprocessorState.HandleIfndef(yytext);
  {POUND}if{RESTLINE}                 thePreprocessorState.HandleIf(yytext);
  {POUND}else{RESTLINE}               thePreprocessorState.HandleElse(yytext);
  {POUND}endif{RESTLINE}              thePreprocessorState.HandleEndif(yytext);
  {POUND}line{RESTLINE}               thePreprocessorState.HandleLine(yytext);
  {POUND}include{RESTLINE}            thePreprocessorState.HandleInclude(yytext);
  {POUND}{RESTLINE}                   thePreprocessorState.HandleUnknownDirective(yytext);
  
  {END} {
    thePreprocessorState.AtEndOfShader();
    thePreprocessorState = PreprocessorState(); //reset state
    BEGIN(INITIAL); //end of shader
    ASSERT_ERROR(result==0)(result);
    yyterminate();
  }
  
  {IDENT} {
    static int loopCount = 0;
    static PositionAware loopCountPosition = ::GetPosition();
    std::string sym = yytext;
    if(thePreprocessorState.ShouldReplaceSymbol(sym))
    {
      int numReplacements = 0;
      sym = thePreprocessorState.ReplaceSymbol(sym, &yyinput, numReplacements);
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
      ASSERT_ERROR(loopCount++ < 100)(sym)
        .Text("max iterations reached in preprocessor");
    }
    else
    {
      ECHO_STR(sym.c_str(), (int)sym.length());
      if(loopCount>0 && loopCountPosition!=::GetPosition()) 
      {
        loopCount = 0;
        loopCountPosition = ::GetPosition();
      }
    }
  }
  
  \n {
    IncrementGlobalLineno();
    ECHO;
    FORCE_RETURN;
  }

  "/*"                            BEGIN(comment);   
  [ \t]*[#][#][ \t]*              // just swallow up the concat operator
  "//"[^\n]*                      //ignore inside comments
  ["]([^"\n]|\\["])*["]           ECHO; //ignore inside strings
  [ \t\r]+                        ECHO; //'fast-forward' through whitespace
  .                               ECHO; //default case
}

<comment>{
  [*]+[/]                BEGIN(shader_on);  
  [*]+                   /*inside a comment*/
  [^*\n]+                /*inside a comment*/
  <<EOF>>                DIE("expected '*/' before <<EOF>>"); yyterminate();
}

<shader_off>{
  {END}                  BEGIN(INITIAL); //end of shader
  [^\n]+                 /* line inside disabled shader */
}

^[Ss][Hh][Aa][Dd][Ee][Rr]\:[^\n]*\n {
  IncrementGlobalLineno();
  
  const char* t = yytext;
  while(*t!=0 && *t++!=':'); //scroll to "shader[:] foo"
  while(*t!=0 && IsWs(*t)) ++t; //scroll to "shader: [f]oo"
  theCurrentShaderName = Trim(t);
  
  if(!p_select || (p_select == theCurrentShaderName))
  {
    BEGIN(shader_on);
  
    //peek to see if next line starts with "-"
    int c = yyinput();
    if(c!='-') unput(c);
    else
    {//Next line are command line args... lets read (and ignore) them
      std::ostringstream args;
      while ( (c=yyinput()) != '\n' && c!=EOF ) args << (char)c;
      if(c!=EOF) unput((char)c); //put the newline back
      HLFrontendParseFXCArgs(args.str()); //parse the args
    }
    FORCE_RETURN;
  }
  else
  {
    theCurrentShaderName="";
    BEGIN(shader_off);
  }
}


<INITIAL,comment,shader_off>\n            IncrementGlobalLineno();
<INITIAL,shader_on,shader_off><<EOF>>     yyterminate();
[^\n]+                                    /* a line outside the "shader..end" block */


%%
