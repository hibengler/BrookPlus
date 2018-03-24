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

#ifndef AMD_ASTLITERALS_HPP
#define AMD_ASTLITERALS_HPP

#include "astelement.hpp"
#include "convert.hpp"

namespace AST
{
  template<typename T, RegDT::RegDT type>
  class NumericLiteral : public TreeElement
  {
    DCL_ASTELEMENT_METHODS(NumericLiteral);
  public:
    NumericLiteral(const std::string& value)
    {
      ASSERT_ERROR(value.size()>0);
      char lastCh = value[value.size()-1];
      if(lastCh >= '0' && lastCh <= '9') //last char a digit
      {
        mValue = FromString<T>( value );
      }
      else if(type == RegDT::Char)
      {
          const char* temp = value.c_str(); 
          mValue = temp[1];
      }
      else
      {
        //trim off trailing 'F' or 'L'
        ASSERT_ERROR(value.size()>1);
        std::string copy = value;
        copy.resize(copy.size()-1);
        mValue = FromString<T>( copy );
      }
    }

    NumericLiteral(T value)
      : mValue(value)
    {}

    /// (Step 3.5)
    /// Allocates registers to each VariableDeclaration
    virtual int PreAllocateRegisters(TreeElementPtr& /*self*/, ILTranscriber*& il)
    {
      if(GetResultSymbol().IsValid()) return 0;
      if(type == RegDT::Double)
      {
          ILRegister reg = il->LookupLiteral(*(long long unsigned int*)&mValue, type, 2);
          STATIC_CONSTSTRING(literalStr,"<literal>");
          MultipartSymbol* sym = new MultipartSymbol(EffectiveDataType(), literalStr, 0);
          sym->SetLiteralRegister(reg);
          SetResultSymbol(sym);
      }
      else
      {
          ILRegister reg = il->LookupLiteral(*(long long unsigned int*)&mValue, type, 1);
          STATIC_CONSTSTRING(literalStr,"<literal>");
          BasicSymbol* sym = new BasicSymbol(EffectiveDataType(), literalStr, 0);
          sym->SetRegister(reg);
          SetResultSymbol(sym);
      }
      
      return 0;
    }

    const AST::DataTypePtr& EffectiveDataType()
    {
      static DataTypePtr tp = DataType::LookupSimpleType(type);
      return tp;
    }

    virtual int SimplifyToInt() const 
    { 
      if(!CanSimplifyToInt()) return TreeElement::SimplifyToInt();
      else return (int)mValue;
    }
    virtual bool CanSimplifyToInt() const 
    { 
      //poor man's T===int
      return ((T)0.1) == 0; 
    }

    virtual std::string DebugStatus() const { return ::ToString(mValue); }
  private:
    T mValue;
  };

  inline TreeElementPtr MakeIntegerLiteral(const std::string& v)
  {
    return new NumericLiteral<int, RegDT::Int>(v);
  }


  inline TreeElementPtr MakeHexLiteral(const std::string& str)
  {
    // hex literals are 0x... or 0x...u
    // trailing u indicates that this is unsigned
    
    int v = 0;
    for(size_t i = 2; i<str.length(); ++i) //"0x" skip two digits
    {
      switch(str[i])
      {
      case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        v*=16;//shift next digit
        v += str[i] - '0';
        break;
      case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
        v*=16;//shift next digit        
        v += str[i] + 10 - 'a';
        break;
      case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
        v*=16;//shift next digit                
        v += str[i] + 10 - 'A';
        break;
      case 'U': case 'u':
        return new NumericLiteral<int, RegDT::UInt>(v);        
      default:
        ERROR_UNIMPLEMENTED()(str)(i)(str[i]);
      }
    }
    return new NumericLiteral<int, RegDT::Int>(v);
  }


  inline TreeElementPtr MakeFloatLiteral(const std::string& v)
  {
    return new NumericLiteral<float, RegDT::Float>(v);    
  }

  inline TreeElementPtr MakeDoubleLiteral(const std::string& v)
  {      
    return new NumericLiteral<double, RegDT::Double>(v);    
  }

  inline TreeElementPtr MakeCharLiteral(const std::string& v)
  {      
    return new NumericLiteral<int, RegDT::Char>(v);    
  }

}

#endif // AMD_ASTLITERALS_HPP

