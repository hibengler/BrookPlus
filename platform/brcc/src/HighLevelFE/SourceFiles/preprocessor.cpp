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

#include "preprocessor.h"
#include "project.h"

extern void increParserErrCnt();

template<typename T>
inline bool Between(const T& a, const T& b, const T& c) { return a<=b && b<=c; }


std::map<std::string, std::string> commandMacroMap;

bool isSignedIntStr(std::string str)
{
    size_t found = str.find_first_not_of("0123456789");
    if(found != std::string::npos || str[0] == '0' && str.length() != 1)
    {
        return false;
    }
    return true;
}

//! Process marcro input coming from command line options and store macros for preprocessor
bool processDefineMacro(const char *macroInput, std::string &errorMsg)
{
    std::string numericChars = "0123456789";
    std::string macroName = "";
    std::string macroValue = "";
    std::string macro = macroInput;
    bool hasValue = false;
    size_t eqPos = macro.find('=');
    macroName.append(macro.substr(0, eqPos).c_str());
    if(eqPos < macro.length())
        macroValue.append(macro.substr(eqPos + 1));
    
    //! Check macro value semantics
    if(macroValue.empty() && eqPos != std::string::npos)
    {
        errorMsg.append("Error:: Macro value missing\n");
        return false;
    }

    size_t found = macroValue.find_first_not_of("0123456789");
    if(found != std::string::npos || macroValue[0] == '0' && macroValue.length() != 1)
    {
        errorMsg.append("Error:: Follow signed integer semantics(only decimal format) for macro value\n");
        return false;
    }

    //! check macro Name semantics

    // Check specical characters in macro name
    found = macroName.find_first_not_of("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    if(found != std::string::npos)
    {
        errorMsg.append("Error:: Special characters(%c) are not allowed\n", macroName[found]);
        return false;
    }
     
    // Check first character 
    found = numericChars.find(macroName[0]);
    if(found != std::string::npos)
    {
        errorMsg.append("Error:: Numeric not allowed as first character for Macro name\n");
        return false;
    }
    
    if(!commandMacroMap.empty())
    {

        std::map<std::string, std::string>::const_iterator i = commandMacroMap.find(macroName);
        if(i != commandMacroMap.end())
        {
            std::ostringstream msg;
            msg << "Error :: " << macroName << ": Macro redefinition \n"
                << "\tSee previous command line definition of '" << macroName << "' \n";
            errorMsg.append(msg.str().c_str());
            return false;
        }
    }
    commandMacroMap[macroName] = macroValue;
    return true;

}
Preprocessor::StringPair Preprocessor::parsePreprocessorLine( const char* cInput )
{

    std::string tempStr = std::string(cInput);
 
    //! Remove the comments
    size_t startPos = 0;
    size_t found = 0;
    found = tempStr.find('/', startPos);
    while(found != std::string::npos)
    {
        if(tempStr[found + 1] == '/')
        {
            tempStr[found] = '\0';
            break;
        }
        else if(tempStr[found + 1] == '*')
        {
            size_t endFound = tempStr.find("*/", found + 2);
            if(endFound != std::string::npos)
            {
                for(size_t i = found; i < endFound + 2; i++)
                {
                    tempStr[i] = ' ';
                }
                startPos = endFound + 2;
            }
            else
            {
                std::ostringstream msg;
                msg << "Expecting end of comment \n"
                    << "\tStatement: " << cInput
                    << "\n\tNote: comment should end in the same line if it is in the preprocessor directive line";
                PrintErrorMsg(msg.str().c_str());
                break;
            }            
        }
        else
        {
            startPos = found + 1;
        }
        found = tempStr.find('/', startPos);        
    }

    //! Tokenize preprocessor tokens
    const char* c = tempStr.c_str();
    std::ostringstream curName;
    std::ostringstream curVal;

    //scroll to " #[ ]define    aaa bbb"
    while(*c!=0 && *c++ != '#');

    //scroll to " # [d]efine   aaa bbb"
    while(*c!=0 && IsWs(*c)) c++;

    //scroll to " # define[ ]  aaa bbb""
    while(*c!=0 && !IsWs(*c)) c++;

    //scroll to " # define   [a]aa bbb""
    while(*c!=0 && IsWs(*c)) c++;

    //record name " # define   aaa[ ]bbb""
    while(*c!=0 && !IsWs(*c) && *c!='('){ curName << *c++;}

    //record rest
    while(*c!=0)
    { 
      //normalize whitespace
      if(IsWs(*c)) 
        curVal<<' '; 
      else if(c[0]=='\\' && c[1]=='\n')
      {
        curVal<<' '; 
        gProject->Parse_TOS->yylineno++;    //make sure we dont miss newlines
        gProject->Parse_TOS->yynxtcol = 0;  
      }
      else
        curVal<<*c; 
      c++;
    }

    return StringPair(curName.str(),curVal.str());
}

void Preprocessor::HandleDefine( const char* yytext )
{
    StringPair line = parsePreprocessorLine(yytext);

    //! Check #define syntax
    if(line.first.length() <= 0)
    {
        std::ostringstream msg;
        msg << "Expecting token after '#define' \n"
            << "\tStatement: " << yytext;
        PrintErrorMsg(msg.str().c_str());
        return;     
    }
    if(IsActive()) 
    {
        if(!mDefinedSymbols.empty())
        {
           StringMap::const_iterator i = mDefinedSymbols.find(line.first);
           if(i != mDefinedSymbols.end())
           {
               std::ostringstream msg;
               msg << line.first << " : Macro redefinition \n"
                   << "\tSee previous definition of '" << line.first << "' \n"
                   << "\tand latest one considered as macro \n";
               PrintWarningMsg(msg.str().c_str());
           }
        }
        mDefinedSymbols[line.first] = line.second;
    }
}

void Preprocessor::HandleUndef( const char* yytext )
{
    StringPair line = parsePreprocessorLine(yytext);
    
    //! Check #undef syntax
    bool isSyntaxOk = true;
    if(line.first.length() <= 0)
    {
        std::ostringstream msg;
        msg << "Expecting token after '#undef' \n"
            << "\tStatement: " << yytext;
        PrintErrorMsg(msg.str().c_str());
        isSyntaxOk = false;
    }

    if(Trim(line.second).length() != 0)
    {
        std::ostringstream msg;
        msg << "Unexpected token following preprocessor directive (#undef" << line.first << ") -- expected a newline \n"
            << "\tStatement: " << yytext;
        PrintWarningMsg(msg.str().c_str());
    }

    if(isSyntaxOk == false)
        return;

    if(IsActive()) mDefinedSymbols.erase(line.first);
}

void Preprocessor::HandleIf( const char* yytext, IfTokenType tokenType )
{
    StringPair line = parsePreprocessorLine(yytext);

    //! Check #if syntax
    bool isSyntaxOk = true;
    int value = 0;
    if(tokenType == BRPP_ConstIntType)
    {
        if(isSignedIntStr(Trim(line.first)))
            value = atoi(line.first.c_str());
        else
        {
            std::ostringstream msg;
            msg << "Follow signed integer semantics(only decimal format) for macro value\n"
                << "\tStatement: " << yytext;
            PrintErrorMsg(msg.str().c_str());
            isSyntaxOk = false;
        }
    }
    else if(tokenType == BRPP_IdentifierType)
    {
        StringMap::const_iterator i = mDefinedSymbols.find(line.first);
        if(i == mDefinedSymbols.end())
        {
            std::ostringstream msg;
            msg << "Undefined preprocessor directive(" << line.first <<")\n"
                << "\tStatement: " << yytext;
            PrintErrorMsg(msg.str().c_str());
            isSyntaxOk = false;
        }
        else
        {
            if(isSignedIntStr(Trim((*i).second)))
                value = atoi((*i).second.c_str());
            else
            {
                std::ostringstream msg;
                msg << "Follow signed integer semantics(only decimal format) for macro value\n"
                    << "\tStatement: " << yytext;
                PrintErrorMsg(msg.str().c_str());
                isSyntaxOk = false;
            }
        }

    }
    else
    {
        std::ostringstream msg;
        msg << "Unsupported #if syntax\n"
            << "\tStatement: " << yytext;
        PrintErrorMsg(msg.str().c_str());
        isSyntaxOk = false;
    }
    if(isSyntaxOk == false)
        return;

    if(value != 0)
    {
        HandleIf(true);
    }
    else
    {
        HandleIf(false);
    }
}

void Preprocessor::HandleIf( bool evalRst )
{
    ++mNestedIfDefCount;
    if(!IsActive())
      ++mDisabledCount;
    else if(!evalRst)
      ++mDisabledCount;
}

void Preprocessor::HandleIfdef( const char* yytext )
{
    StringPair line = parsePreprocessorLine(yytext);
    
    //! Check #ifdef syntax
    bool isSyntaxOk = true;
    if(line.first.length() <= 0)
    {
        std::ostringstream msg;
        msg << "Expecting token after '#ifdef' \n"
            << "\tStatement: " << yytext;
        PrintErrorMsg(msg.str().c_str());
        isSyntaxOk = false;
    }

    if(Trim(line.second).length() != 0)
    {
        std::ostringstream msg;
        msg << "Unexpected token following preprocessor directive (#ifdef " << line.first << ") -- expected a newline \n"
            << "\tStatement: " << yytext;
        PrintWarningMsg(msg.str().c_str());
    }

    if(isSyntaxOk == false)
        return;

    HandleIf(ShouldReplaceSymbol(line.first));
}    

void Preprocessor::HandleIfndef( const char* yytext )
{
    
    StringPair line = parsePreprocessorLine(yytext);

    //! Check #ifndef syntax
    bool isSyntaxOk = true;

    if(line.first.length() > 0)
    {
        std::ostringstream msg;
        msg << "Expecting token after '#ifndef' \n"
            << "\tStatement: " << yytext;
        PrintErrorMsg(msg.str().c_str());
        isSyntaxOk = false;
    }

    if(Trim(line.second).length() != 0)
    {
        std::ostringstream msg;
        msg << "Unexpected token following preprocessor directive (#ifndef " << line.first << ") -- expected a newline \n"
            << "\tStatement: " << yytext;
        PrintWarningMsg(msg.str().c_str());
    }

    if(isSyntaxOk == false)
        return;

    HandleIf(!ShouldReplaceSymbol(line.first));
}

void Preprocessor::HandleElse( const char* yytext )
{
    StringPair line = parsePreprocessorLine(yytext);

    //! Check #else syntax
    if(line.first.length() != 0)
    {
        std::ostringstream msg;
        msg << "Unexpected token following preprocessor directive (#else) -- expected a newline \n"
            << "\tStatement: " << yytext;
        PrintWarningMsg(msg.str().c_str());
    }

    if(mDisabledCount==0)
      mDisabledCount = 1;
    else if(mDisabledCount==1)
      mDisabledCount = 0;
    else
      { /*leave it as it is*/ }
}

void Preprocessor::HandleEndif( const char* yytext )
{
    StringPair line = parsePreprocessorLine(yytext);

    //! Check #endif syntax
    if(line.first.length() != 0)
    {
        std::ostringstream msg;
        msg << "Unexpected token following preprocessor directive (#endif) -- expected a newline \n"
            << "\tStatement: " << yytext;
        PrintWarningMsg(msg.str().c_str());
    }
    --mNestedIfDefCount;
    if(mDisabledCount>0) --mDisabledCount;
}

void Preprocessor::HandleLine( const char* yytext )
{
    std::ostringstream msg;
    msg << "'#line' is not yet supported\n"
        << "\tStatement: " << yytext;
    PrintErrorMsg(msg.str().c_str());
}

void Preprocessor::HandleInclude( const char* yytext )
{
    std::ostringstream msg;
    msg << "'#include' is not yet supported\n"
        << "\tStatement: " << yytext;
    PrintErrorMsg(msg.str().c_str());
}

void Preprocessor::HandleUnknownDirective( const char* yytext )
{
    std::ostringstream msg;
    msg << "Unsupported preprocessor directive syntax\n"
        << "\tStatement: " << yytext;
    PrintErrorMsg(msg.str().c_str());
}

void Preprocessor::PrintErrorMsg(const char* errorMsg)
{
    //! Print error message with line information
    *gProject->Parse_TOS->yyerrstream << gProject->Parse_TOS->transUnit->filename 
                                      << "(" << gProject->Parse_TOS->yylineno
                                      << ") : ERROR-- " << errorMsg << std::endl;
    //! Increment parser error count
    increParserErrCnt();
}

void Preprocessor::PrintWarningMsg(const char* errorMsg)
{
    //! Print warning message with line information
    *gProject->Parse_TOS->yyerrstream << gProject->Parse_TOS->transUnit->filename 
                                      << "(" << gProject->Parse_TOS->yylineno
                                      << ") : WARN-- " << errorMsg << std::endl;
}

void Preprocessor::AtStartOfBrFile()
{
    //! Add command line macro pairs to macro table
    std::map<std::string, std::string>::iterator i;
    for(i = commandMacroMap.begin(); i != commandMacroMap.end(); ++i)
    {
        mDefinedSymbols[(*i).first] = (*i).second;
    }
}

void Preprocessor::AtEndOfBrFile()
{
    if(mDisabledCount != 0)
    {
        LOG(LOG_ERROR, "Internal Error : mDisableCount = %d \n File(%s): %s \n", mDisabledCount, __LINE__, __FILE__);
    }

    if(mNestedIfDefCount != 0)
    {
        std::ostringstream msg;
        msg << "Expecting '#endif' before end of file\n";
        PrintErrorMsg(msg.str().c_str());
    }
}

bool Preprocessor::ShouldReplaceSymbol( const std::string& sym ) const
{
    return mDefinedSymbols.find(sym) != mDefinedSymbols.end();
}

std::string Preprocessor::ReplaceSymbol( const std::string& sym, int (*getNextChar ) (), int& numReplacements ) const
{
    //lookup symbol
    StringMap::const_iterator i = mDefinedSymbols.find(sym);

    //undefined symbol case, don't translate
    if(i==mDefinedSymbols.end()) return sym;

    //not a macro, basic translation
    const std::string& val = i->second;
    if(val.length()>0 && val[0]!='(')
    {
      ++numReplacements;
      return val;
    }

    //cant proceed without a input stream
    if(getNextChar==0) return sym; 

    //now comes macro translation...
    char c;
    std::stringstream tmp, readData;

    //scroll to a '('
    for(c=0; IsWs(c);)
    {
      c = (*getNextChar)();
      if(c==EOF || c==0) return sym + readData.str(); //end of file... nevermind
      readData << c;
      
      if(!IsWs(c) && c!='(')
        return sym + readData.str(); //oops.... not a macro after all
    }

    std::list<std::string> argValues;
    //now read the arg values 
    //TODO: handle strings correctly
    int parenCnt = 0;
    for(c=0; parenCnt>=0;)
    {
      c = (*getNextChar)();
      if(c==EOF || c==0) return sym + readData.str(); //end of file... nevermind
      readData << c;
      if(c=='(') parenCnt++;
      else if(c==')') parenCnt--;

      if((c==',' && parenCnt==0) || parenCnt==-1)
      {
        argValues.push_back(tmp.str());
        tmp.str("");
      }
      else
        tmp << c;
    }
    const char* macroiterator = val.c_str();
    StringMap table = buildMacroTable(macroiterator, argValues);
    return translateMacroValue(macroiterator, table, numReplacements);
}

Preprocessor::StringMap Preprocessor::buildMacroTable( const char*& i , const std::list<std::string>& argValues ) const
{

    /////////ASSERT_ERROR(*i++ == '(');

    std::stringstream tmp;
    std::list<std::string> argNames;
    for(; *i!=0 && i[-1]!=')'; ++i)
    {
      if(*i==',' || *i==')')
      {
        argNames.push_back(tmp.str());
        tmp.str("");
      }
      else
        tmp << *i;
    }
    
    /////////ASSERT_ERROR(*i!=0).Text("malformed macro");
    /////////ASSERT_ERROR(argNames.size() == argValues.size())(argNames.size())(argValues.size())
    /////////  .Text("Incorrect number of arguments to macro");

    
    {
      StringMap argMap;
      std::list<std::string>::const_iterator n,v;
      for(n=argNames.begin(),v=argValues.begin(); n!=argNames.end() && v!=argValues.end(); ++n,++v )
        argMap[*n] = *v;
      return argMap;
    }
}

static bool _isAlphaNum(char c) { return Between('a',c,'z') || Between('A',c,'Z') || Between('0',c,'9'); } 

std::string Preprocessor::translateMacroValue( const char*& macroiterator, const StringMap& argTable, int& numReplacements ) const
{
    //TODO: handle strings correctly
    std::stringstream os, curIdent;
    for(;; ++macroiterator)
    {
      if(_isAlphaNum(*macroiterator))
        curIdent << *macroiterator; //collect identifier
      else
      {
        //end of an identifier?
        std::string ident = curIdent.str();
        curIdent.str("");
        if(ident.length() >0)
        {
          StringMap::const_iterator i = argTable.find(ident);
          if(i!=argTable.end())
            os << i->second;
          else
            os << ReplaceSymbol(ident, 0, numReplacements);
        }
        if(*macroiterator==0) break;
        else os << *macroiterator;
      }
    }
    ++numReplacements;
    return os.str();
}
