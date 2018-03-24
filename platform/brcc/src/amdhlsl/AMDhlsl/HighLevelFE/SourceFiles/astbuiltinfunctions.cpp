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

#include "astbuiltinfunctions.hpp"
#include "astcontrol.hpp"
#include "parameter.hpp"
#include "convert.hpp"
#include "conststring.hpp"
#include "iltranslator.hpp"

namespace ILSpec
{
#include "ILTables.h"
}


PARAMETER(p_strict, "strict" )


namespace
{
  //namespace ExpansionTemplate
  //{
    typedef OpType::OpType ILCMD;
    struct Arg
    {
      Arg(const ILRegister& r, char* s="xyzw")
        : reg(r)
      {
        DEBUG_CHECK(strlen(s)==4)(s);
        memcpy(swiz,s,4);
      }
      ILRegister reg;
      char swiz[4];

      //Largest possible width given the available data and swizzle
      int Width() const
      {
        int regW = reg.GetNumSlots();
        if(regW>=4) return 4;
        char stopCh = ILRegister::DEFAULT_SWIZZLE[regW];
        for(int i=0; i<4; ++i)
          if(swiz[i]==stopCh || swiz[i]==0 || swiz[i]==ILRegister::SLOTNOTUSED)
            return i;
        return 4;
      }

      ILRegister ResolveSwizzle(int width) const
      {
        DEBUG_CHECK(width<=4)(width);
        char tmp[4] = { ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED};
        memcpy(tmp, swiz, width);
        ILRegister r = reg;
        r.SetSwizzleBytes(tmp);
        return r;
      }
    };
    struct BCmd
    {
      BCmd(ILCMD c, Arg _d, Arg _l, Arg _r)
        : cmd(c), d(_d), l(_l), r(_r)
      {}
      ILCMD cmd;
      Arg d;
      Arg l;
      Arg r;
    };

    //generate IL from a b BCmd stream
    void FilterAndGenerateIL(ILTranscriber* il, const BCmd* cmd, size_t len)
    {
      ILTranslator tl(il);
      for(;len>0; cmd++,len--)
      {
        if(cmd->cmd!=0 && cmd->d.reg && cmd->r.reg && cmd->l.reg)
        {
          int width = Minimum(cmd->r.Width(), cmd->l.Width());
          ILRegister d = cmd->d.ResolveSwizzle(width);
          ILRegister l = cmd->l.ResolveSwizzle(width);
          ILRegister r = cmd->r.ResolveSwizzle(width);
          tl.BinaryCommand(cmd->cmd,d,l,r);
        }
      }
    }

    ILRegister TryRow(const AST::SymbolPtr& sym, int r)
    {
      return r<sym->Type()->NumRows() ? sym->GetMatrixRow(r) : ILRegister();
    }
  //};
}


template <int numArgs>
void AST::BuiltinFunction<numArgs>::builtinImpl( ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args )
{
  ILRegister retReg = ret->GetRegister();

  DEBUG_CHECK(args.size()==numArgs);
  switch(numArgs)
  {
  case 1:
    {
      ILRegister a0(args[0]->GetRegister());
      typeCheck(il,a0);
      builtinImpl(il, retReg, a0);  
    }
    break;
  case 2:
    {
      ILRegister a0(args[0]->GetRegister()),a1(args[1]->GetRegister());
      typeCheck(il,a0);
      typeCheck(il,a1);
      builtinImpl(il, retReg, a0, a1);  
    }
    break;
  case 3:
    {
      ILRegister a0(args[0]->GetRegister()),a1(args[1]->GetRegister()),a2(args[2]->GetRegister());
      //typeCheck(il,a0);
      //typeCheck(il,a1);
      //typeCheck(il,a2);
      builtinImpl(il, retReg, a0, a1, a2);
      retReg.SetDT(a1.GetDT());
    }
    break;
  default:
    ERROR_UNIMPLEMENTED();
  }

  ret->AsBasicSymbol().SetRegister(retReg);

}

template <int numArgs>
void AST::BuiltinFunction<numArgs>::builtinImpl( ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 )
{
  ERROR_UNIMPLEMENTED()(FunctionName());
}

template <int numArgs>
void AST::BuiltinFunction<numArgs>::builtinImpl( ILTranscriber* il, ILRegister& ret, const ILRegister& arg0, const ILRegister& arg1 )
{
  ERROR_UNIMPLEMENTED()(FunctionName());
}

template <int numArgs>
void AST::BuiltinFunction<numArgs>::builtinImpl( ILTranscriber* il, ILRegister& ret, ILRegister& arg0, ILRegister& arg1, ILRegister& arg2 )
{
  ERROR_UNIMPLEMENTED()(FunctionName());
}

template <int numArgs>
int AST::BuiltinFunction<numArgs>::PostExportToIL( TreeElementPtr& /*self*/, ILTranscriber* il )
{
  checkArgCount();
  SymbolPtrList args;
  for(Iterator i = Begin(); i!=End(); ++i)
    args.push_back((*i)->GetResultSymbol());
  builtinImpl(il, GetResultSymbol(), args);
  return 0;
}

template <int numArgs>
const AST::DataTypePtr& AST::BuiltinFunction<numArgs>::EffectiveDataType()
{
  ASSERT_ERROR(NumChildren()==numArgs)(FunctionName())(numArgs)(NumChildren()).Text("wrong number of arguments to builtin function");
  switch(numArgs)
  {
    case 1: return mChildElements[0]->EffectiveDataType();
    case 2: return DataType::CombineTypes(mChildElements[0]->EffectiveDataType()
                                         ,mChildElements[1]->EffectiveDataType());
    case 3: return DataType::CombineTypes(mChildElements[0]->EffectiveDataType()
                                         ,DataType::CombineTypes(mChildElements[1]->EffectiveDataType()
                                                                ,mChildElements[2]->EffectiveDataType()));
    case 4: return DataType::CombineTypes(mChildElements[0]->EffectiveDataType()
                                         ,DataType::CombineTypes(mChildElements[1]->EffectiveDataType()
                                                                ,DataType::CombineTypes(mChildElements[0]->EffectiveDataType()
                                                                                       ,mChildElements[1]->EffectiveDataType())));
    default:
      ERROR_UNIMPLEMENTED();
      return mChildElements[0]->EffectiveDataType();
  }
}

template <int numArgs>
void AST::FloatsOnlyBuiltinFunction<numArgs>::typeCheck( ILTranscriber* il, ILRegister& t)
{
  t=ILTranslator(il).TmpConvertInput(RegDT::Float,t);
}

// Implementing type promotion to avoid unnecessary implicit type cast.
template <int numArgs>
void AST::FloatsOnlyBuiltinFunction<numArgs>::typeCheck( ILTranscriber* il, ILRegister& t1, ILRegister& t2)
{
    if(t1.GetDT() == t2.GetDT())
    {
        return;
    }
    else
    {
        if((t1.GetDT() == RegDT::Double)||(t2.GetDT() == RegDT::Double))
        {
            t1=ILTranslator(il).TmpConvertInput(RegDT::Double, t1);
            t2=ILTranslator(il).TmpConvertInput(RegDT::Double, t2);
            return;
        }
        
        if((t1.GetDT() == RegDT::Float)||(t2.GetDT() == RegDT::Float))
        {
            t1=ILTranslator(il).TmpConvertInput(RegDT::Float, t1);
            t2=ILTranslator(il).TmpConvertInput(RegDT::Float, t2);
            return;
        }
        
        if((t1.GetDT() == RegDT::UInt)||(t2.GetDT() == RegDT::UInt))
        {
            t1=ILTranslator(il).TmpConvertInput(RegDT::UInt, t1);
            t2=ILTranslator(il).TmpConvertInput(RegDT::UInt, t2);
            return;
        }
    }
}

template <int numArgs>
const AST::DataTypePtr& AST::FloatsOnlyBuiltinFunction<numArgs>::EffectiveDataType()
{
  return DataType::LookupSimpleType(RegDT::Float, BuiltinFunction<numArgs>::EffectiveDataType()->GetRegSlots());
}


using namespace AST;

namespace
{
  typedef AST::TreeElementPtr (*BIFFactoryFunctionPtr)();

  static std::map<ConstStringPtr, BIFFactoryFunctionPtr >&  theBIFunctionTable() 
  { 
    static std::map<ConstStringPtr, BIFFactoryFunctionPtr >  inst;
    return inst;
  }
  static std::map<BIFFactoryFunctionPtr , ConstStringPtr >&  theBINameTable() 
  { 
    static std::map<BIFFactoryFunctionPtr , ConstStringPtr > inst;
    return inst;
  }
  int addToBIFunctionTable(const char* name, BIFFactoryFunctionPtr factory)
  {
    ConstStringPtr k = ConstString::Lookup(name);
    theBIFunctionTable()[k] = factory;
    theBINameTable()[factory] = k;
    return 0;
  }

  //we play some tricks with macros here to make declaring lots of these things easy;
#define BEGIN_BUILTIN(name, baseclass) \
  namespace _MK_UNIQUE_NAME(builtin_func_impl) { \
  static const char* NAME = name; \
  AST::TreeElementPtr factoryFunc(); \
  int registrationResult = addToBIFunctionTable(name, & factoryFunc ); \
  AST::TreeElementPtr factoryFunc () { \
    class localBIFClass : public baseclass { \
      DCL_ASTELEMENT_CLONE(localBIFClass);     \
      DCL_ASTELEMENT_DEBUGCLASSNAME(name "(" #baseclass ")");     \
      std::string FunctionName() const {return NAME;}
      

#define END_BUILTIN }; return new localBIFClass(); } }

    BEGIN_BUILTIN( "cmov", FloatsOnlyBuiltinFunction<3> )
    void builtinImpl( ILTranscriber* il,
                      ILRegister& ret,
                      ILRegister& arg0,
                      ILRegister& arg1,
                      ILRegister& arg2 )
    {
        typeCheck(il,arg1,arg2);
        arg0.WidenSwizzleTo(arg1.GetNumSlots());
        il->CMovLogical( ret, arg0, arg1, arg2 );
    }
    END_BUILTIN

  BEGIN_BUILTIN("abs", BuiltinFunction<1> )
    void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    args[0]->HandleUnaryCommand(il,OpType::AbsoluteValue, ret);
  }
  END_BUILTIN

  BEGIN_BUILTIN("sign", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FSign(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("sin", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FSin(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("cos", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FCos(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("tan", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FTan(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("asin", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
      // for each arg0 slot copy the value to tmp.w and compute asin of it,
      // move tmp.x to the corresponding ret slot.
      ILRegister tmpRet = il->AllocTempRegister();
      tmpRet.SetNumSlots( 4 );
      ILRegister w = il->Scratch();
      w.SetNumSlots( 4 );
      w.SetSwizzleBytes( "___w" );
      for ( int ii = 0; ii < ret.GetNumSlots(); ++ii )
      {
          ILRegister dst = ret;
          ILRegister src = arg0;
          {
              char swiz[4] = { ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED,
                               ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED };
              swiz[0] = dst.SwizzleBytes()[ii];
              dst.SetSwizzleBytes(swiz);
              swiz[0] = ILRegister::SLOTNOTUSED;
              swiz[3] = src.SwizzleBytes()[ii];
              src.SetSwizzleBytes(swiz);
          }
          il->Move( w, src );
          il->FASin( tmpRet, w );
          il->Move( dst, tmpRet );
      }
  }
  END_BUILTIN

  BEGIN_BUILTIN("acos", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
      ILRegister tmpRet = il->AllocTempRegister();
      tmpRet.SetNumSlots( 4 );
      ILRegister w = il->Scratch();
      w.SetNumSlots( 4 );
      w.SetSwizzleBytes( "___w" );
      for ( int ii = 0; ii < ret.GetNumSlots(); ++ii )
      {
          ILRegister dst = ret;
          ILRegister src = arg0;
          {
              char swiz[4] = { ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED,
                               ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED };
              swiz[0] = dst.SwizzleBytes()[ii];
              dst.SetSwizzleBytes(swiz);
              swiz[0] = ILRegister::SLOTNOTUSED;
              swiz[3] = src.SwizzleBytes()[ii];
              src.SetSwizzleBytes(swiz);
          }
          il->Move( w, src );
          il->FACos( tmpRet, w );
          il->Move( dst, tmpRet );
      }
  }
  END_BUILTIN

  BEGIN_BUILTIN("atan", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
      ILRegister tmpRet = il->AllocTempRegister();
      tmpRet.SetNumSlots( 4 );
      ILRegister w = il->Scratch();
      w.SetNumSlots( 4 );
      w.SetSwizzleBytes( "___w" );
      for ( int ii = 0; ii < ret.GetNumSlots(); ++ii )
      {
          ILRegister dst = ret;
          ILRegister src = arg0;
          {
              char swiz[4] = { ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED,
                               ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED };
              swiz[0] = dst.SwizzleBytes()[ii];
              dst.SetSwizzleBytes(swiz);
              swiz[0] = ILRegister::SLOTNOTUSED;
              swiz[3] = src.SwizzleBytes()[ii];
              src.SetSwizzleBytes(swiz);
          }
          il->Move( w, src );
          il->FATan( tmpRet, w );
          il->Move( dst, tmpRet );
      }
  }
  END_BUILTIN

  BEGIN_BUILTIN("log", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FLog2(ret,arg0);
    il->FMult(ret, ret , il->ConstFloatLn2());
  }
  END_BUILTIN

  BEGIN_BUILTIN("log2", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FLog2(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("log10", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FLog2(ret,arg0);
    il->FMult(ret, ret , il->ConstFloatLog2());
  }
  END_BUILTIN

  typedef ReturnFixedTypeBuiltinFunction<RegDT::Bool,1> ReturnFixedTypeBuiltinFunctionBool1;
  BEGIN_BUILTIN("all", ReturnFixedTypeBuiltinFunctionBool1 )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    ILRegister a = ILTranslator(il).TmpConvertInput(RegDT::Bool, arg0);
    ILRegister b = a;
    switch(a.GetNumSlots())
    {
    case 4:
    case 3:
      a.SetSwizzleBytes("xy__");
      b.SetSwizzleBytes(a.GetNumSlots()==4 ? "zw__" : "zy__");
      ret.SetNumSlots(2);
      il->And(ret,a,b);
      a = b = ret; //fall through to next case
    case 2:
      a.SetSwizzleBytes("x___");
      b.SetSwizzleBytes("y___");
      ret.SetNumSlots(1);
      il->And(ret,a,b);
    case 1:
      return;
    }
  }
  END_BUILTIN

  BEGIN_BUILTIN("any", ReturnFixedTypeBuiltinFunctionBool1 )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    ILRegister a = ILTranslator(il).TmpConvertInput(RegDT::Bool, arg0);
    ILRegister b = a;
    switch(a.GetNumSlots())
    {
    case 4:
    case 3:
      a.SetSwizzleBytes("xy__");
      b.SetSwizzleBytes(a.GetNumSlots()==4 ? "zw__" : "zy__");
      ret.SetNumSlots(2);
      il->Or(ret,a,b);
      a = b = ret; //fall through to next case
    case 2:
      a.SetSwizzleBytes("x___");
      b.SetSwizzleBytes("y___");
      ret.SetNumSlots(1);
      il->Or(ret,a,b);
      return;
    case 1:
      il->Move(ret, a);
      return;
    }
  }
  END_BUILTIN

  BEGIN_BUILTIN("asfloat", ReturnFixedTypeBuiltinFunction<RegDT::Float> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->Move(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("asint", ReturnFixedTypeBuiltinFunction<RegDT::Int> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->Move(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("splitdouble", ReturnFixedTypeBuiltinFunction<RegDT::Int> )
    const DataTypePtr& EffectiveDataType()
  {
    return DataType::LookupSimpleType(RegDT::UInt, 2);
  }
  
    int PostExportToIL( TreeElementPtr& self, ILTranscriber* il)
  {
    // target type is int, source should be double
    std::vector<ILRegister> dst = GetResultSymbol()->GetRawRegisters();
    std::vector<ILRegister> src =
       mChildElements[0]->GetResultSymbol()->GetRawRegisters();

    il->Move(dst[0], src[0]);
	return 0;
  }    
    
  END_BUILTIN    

  BEGIN_BUILTIN("asdouble", ReturnFixedTypeBuiltinFunction<RegDT::Double> )
    int PostExportToIL( TreeElementPtr& self, ILTranscriber* il)
  {
    // target type is double, which is multipart
    std::vector<ILRegister> dst = GetResultSymbol()->GetRawRegisters();
    std::vector<ILRegister> src =
       mChildElements[0]->GetResultSymbol()->GetRawRegisters();
    // set all the double results to the converted value
    for (int count = 0; count <2; count++){
      if (dst[count].GetNumSlots() > 0){
        il->Move(dst[count], src[0]);
      }
    }
	return 0;
  }
  
  END_BUILTIN
  

  BEGIN_BUILTIN("asuint", ReturnFixedTypeBuiltinFunction<RegDT::UInt> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->Move(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("round", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FRound(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("ceil", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FCeil(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("floor", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FFloor(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("min", BuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    args[0]->HandleBinaryCommand(il,OpType::Min, ret, args[1]);
  }
  END_BUILTIN

  BEGIN_BUILTIN("max", BuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    args[0]->HandleBinaryCommand(il,OpType::Max, ret, args[1]);
  }
  END_BUILTIN

  BEGIN_BUILTIN("clamp", BuiltinFunction<3> )
  void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    args[0]->HandleBinaryCommand(il,OpType::Max, ret, args[1] );
    ret->HandleBinaryCommand(il,    OpType::Min, ret, args[2] );
  }
  END_BUILTIN

  BEGIN_BUILTIN("fmod", FloatsOnlyBuiltinFunction<2> )
  void builtinImpl( ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args )
  {
      args[0]->HandleBinaryCommand( il, OpType::Mod, ret, args[1] );
  }
  END_BUILTIN

  BEGIN_BUILTIN("normalize", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    //\todo IL nrm is broken, if it is fixed just do this: -ja 7.27.07
    //il->Normalize(ret, arg0);
   
    ILRegister tmp = il->Scratch();
    tmp.SetNumSlots(1);
    il->FDot(tmp,arg0,arg0);
    il->FSqrt(tmp,tmp);
    tmp.SetSwizzleBytes("xxxx");
    il->FDiv(ret,arg0,tmp);
  }
  END_BUILTIN

  BEGIN_BUILTIN("isfinite", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->And(ret, arg0, il->ConstFloatExpBitMask(arg0.GetDT(),arg0.GetNumSlots()));
    il->INe(ret, ret,  il->ConstFloatExpBitMask(arg0.GetDT(),arg0.GetNumSlots()));
    il->And(ret, ret,  il->ConstIntOne(arg0.GetDT(),arg0.GetNumSlots()));
    il->IToF(ret, ret);
  }
  END_BUILTIN

  BEGIN_BUILTIN("isinf", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->And(ret, arg0, il->ConstFloatExpFracBitMask(arg0.GetDT(),arg0.GetNumSlots()));
    il->IEq(ret, ret,  il->ConstFloatExpBitMask(arg0.GetDT(),arg0.GetNumSlots()));
    il->And(ret, ret,  il->ConstIntOne(arg0.GetDT(),arg0.GetNumSlots()));
    il->IToF(ret, ret);
  }
  END_BUILTIN

  BEGIN_BUILTIN("isnan", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->And(ret, arg0, il->ConstFloatExpFracBitMask(arg0.GetDT(),arg0.GetNumSlots()));
    il->ULt(ret, il->ConstFloatExpBitMask(arg0.GetDT(),arg0.GetNumSlots()), ret);
    il->And(ret, ret,  il->ConstIntOne(arg0.GetDT(),arg0.GetNumSlots()));
    il->IToF(ret, ret);
  }
  END_BUILTIN

  BEGIN_BUILTIN("dot", BuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    args[0]->HandleBinaryCommand(il,OpType::DotProduct, ret, args[1]);
  }
  const AST::DataTypePtr& EffectiveDataType()
  {
    //always 1 slot
    return DataType::LookupSimpleType(BuiltinFunction<2>::EffectiveDataType()->GetRegDT(),1);
  }
  END_BUILTIN

  BEGIN_BUILTIN("mul", BuiltinFunction<2> )

  int DimensionOfChild(int n)
  {
    ASSERT_ERROR((int)mChildElements.size()>n)(mChildElements.size())(n);
    int rows = mChildElements[n]->GetResultSymbol()->Type()->NumRows();
    int cols = mChildElements[n]->GetResultSymbol()->Type()->NumCols();
    if(cols==1 && rows == 1) return 0;
    if(rows>1) return 2;
    return 1;
  }

  const AST::DataTypePtr& EffectiveDataType()
  {
    int d0 = DimensionOfChild(0);
    int d1 = DimensionOfChild(1);
    const DataTypePtr& t0 = mChildElements[0]->EffectiveDataType();
    const DataTypePtr& t1 = mChildElements[1]->EffectiveDataType();

    //handle scalar * ANY cases
    if(d0==0) return t1;
    if(d1==0) return t0;

    //handle vector * vector case
    if(d0 == 1 && d1 == 1) return DataType::LookupSimpleType(t0->GetRegDT(), 1);

    //handle matrix * matrix case
    if(d0 == 2 && d1 == 2)
    {
      int outrows = Minimum(t0->NumRows(), t1->NumRows());
      int outcols = Minimum(t0->NumCols(), t1->NumCols());
      return DataType::LookupMatrixType(t0->GetRegDT(), outrows, outcols);
    }

    //handle vector * matrix cases
    if(d0 == 2) return DataType::LookupSimpleType(t0->GetRegDT(), Minimum(t0->NumRows(), t1->NumCols()));
    if(d1 == 2) return DataType::LookupSimpleType(t0->GetRegDT(), Minimum(t1->NumCols(), t0->NumCols()));

    DIE("this should be unreachable");
    static DataTypePtr ndt;
    return ndt;
  }
  void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    ILTranslator tl(il);
    ASSERT_ERROR(args.size()==2)(args.size()).Text("mul() takes 2 args");
    int d0 = DimensionOfChild(0);
    int d1 = DimensionOfChild(1);
    SymbolPtr s0 = args[0];
    SymbolPtr s1 = args[1];

    if(d0==0 || d1==0)
    {//handle scalar * ANY cases
      if(d1==0){ Swap(d0,d1); Swap(s0,s1); }
      ILRegister scalar = s0->GetRegister();
      DEBUG_CHECK(scalar.GetNumSlots()==1)(scalar.GetNumSlots());

      //using namespace ExpansionTemplate;
      BCmd tmpl[] = {
        BCmd(OpType::Mult, TryRow(ret,0), Arg(scalar,"xxxx"), TryRow(s1,0)),
        BCmd(OpType::Mult, TryRow(ret,1), Arg(scalar,"xxxx"), TryRow(s1,1)),
        BCmd(OpType::Mult, TryRow(ret,2), Arg(scalar,"xxxx"), TryRow(s1,2)),
        BCmd(OpType::Mult, TryRow(ret,3), Arg(scalar,"xxxx"), TryRow(s1,3)),
      };
      FilterAndGenerateIL(il, tmpl, sizeof(tmpl)/sizeof(BCmd));
    }
    else if(d1==1)
    { //special case where we need to flip way we index the first array
      //using namespace ExpansionTemplate;
      BCmd tmpl[] = {
        BCmd(OpType::DotProduct, Arg(TryRow(ret,0),"x___"), TryRow(s0,0), TryRow(s1,0)),
        BCmd(OpType::DotProduct, Arg(TryRow(ret,0),"y___"), TryRow(s0,1), TryRow(s1,0)),
        BCmd(OpType::DotProduct, Arg(TryRow(ret,0),"z___"), TryRow(s0,2), TryRow(s1,0)),
        BCmd(OpType::DotProduct, Arg(TryRow(ret,0),"w___"), TryRow(s0,3), TryRow(s1,0)),
      };
      FilterAndGenerateIL(il, tmpl, sizeof(tmpl)/sizeof(BCmd));
    }
    else
    { //generic case (this command seq may be filtered depending on the available data)
      //using namespace ExpansionTemplate;
      BCmd tmpl[] = {
        BCmd(OpType::Mult,         TryRow(ret,0), TryRow(s1,0), Arg(TryRow(s0,0),"xxxx")),
        BCmd(OpType::MultAddToDst, TryRow(ret,0), TryRow(s1,1), Arg(TryRow(s0,0),"yyyy")),
        BCmd(OpType::MultAddToDst, TryRow(ret,0), TryRow(s1,2), Arg(TryRow(s0,0),"zzzz")),
        BCmd(OpType::MultAddToDst, TryRow(ret,0), TryRow(s1,3), Arg(TryRow(s0,0),"wwww")),
        BCmd(OpType::Mult,         TryRow(ret,1), TryRow(s1,0), Arg(TryRow(s0,1),"xxxx")),
        BCmd(OpType::MultAddToDst, TryRow(ret,1), TryRow(s1,1), Arg(TryRow(s0,1),"yyyy")),
        BCmd(OpType::MultAddToDst, TryRow(ret,1), TryRow(s1,2), Arg(TryRow(s0,1),"zzzz")),
        BCmd(OpType::MultAddToDst, TryRow(ret,1), TryRow(s1,3), Arg(TryRow(s0,1),"wwww")),
        BCmd(OpType::Mult,         TryRow(ret,2), TryRow(s1,0), Arg(TryRow(s0,2),"xxxx")),
        BCmd(OpType::MultAddToDst, TryRow(ret,2), TryRow(s1,1), Arg(TryRow(s0,2),"yyyy")),
        BCmd(OpType::MultAddToDst, TryRow(ret,2), TryRow(s1,2), Arg(TryRow(s0,2),"zzzz")),
        BCmd(OpType::MultAddToDst, TryRow(ret,2), TryRow(s1,3), Arg(TryRow(s0,2),"wwww")),
        BCmd(OpType::Mult,         TryRow(ret,3), TryRow(s1,0), Arg(TryRow(s0,3),"xxxx")),
        BCmd(OpType::MultAddToDst, TryRow(ret,3), TryRow(s1,1), Arg(TryRow(s0,3),"yyyy")),
        BCmd(OpType::MultAddToDst, TryRow(ret,3), TryRow(s1,2), Arg(TryRow(s0,3),"zzzz")),
        BCmd(OpType::MultAddToDst, TryRow(ret,3), TryRow(s1,3), Arg(TryRow(s0,3),"wwww")),
      };
      FilterAndGenerateIL(il, tmpl, sizeof(tmpl)/sizeof(BCmd));
    }
  }
  END_BUILTIN

  BEGIN_BUILTIN("transpose", BuiltinFunction<1> )
  const AST::DataTypePtr& EffectiveDataType()
  {
    int rows = mChildElements[0]->GetResultSymbol()->Type()->NumRows();
    int cols = mChildElements[0]->GetResultSymbol()->Type()->NumCols();
    const DataTypePtr& t = mChildElements[0]->EffectiveDataType();
    return DataType::LookupMatrixType(t->GetRegDT(), cols, rows);
  }
  void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    args[0]->TransposeMoveTo(il,ret);
  }
  END_BUILTIN



  BEGIN_BUILTIN("cross", FloatsOnlyBuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0, const ILRegister& arg1)
  {
    ILTranslator tl(il);
    if(p_strict)
    {
      ASSERT_ERROR(arg0.GetNumSlots()==3 && arg1.GetNumSlots()==3)
        (arg0.SrcILString())(arg1.SrcILString())
        .Text("Cross product requires inputs to have exactly 3 slots");
    }
    else
    {
      ASSERT_ERROR(arg0.GetNumSlots()>=3 && arg1.GetNumSlots()>=3)
        (arg0.SrcILString())(arg1.SrcILString())
        .Text("Cross product requires inputs to have 3 slots");
    }
    il->FCrossProduct(ret,tl.TmpReslotInput(3, arg0),tl.TmpReslotInput(3, arg1));
  }
  END_BUILTIN

  BEGIN_BUILTIN("pow", FloatsOnlyBuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0, const ILRegister& arg1)
  {
    //need to use tmp in case arg1==ret
    ILRegister tmp = il->Scratch();
    tmp.SetNumSlots(arg0.GetNumSlots());
    il->FLog2(tmp,arg0);
    il->FMult(tmp,tmp,arg1);
    il->FExp2(ret,tmp);
    
    //TODO: this handles negatives wrong
  }
  END_BUILTIN

  BEGIN_BUILTIN("exp", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FDiv(ret,arg0,il->ConstFloatLn2());
    il->FExp2(ret,ret);
  }
  END_BUILTIN

  BEGIN_BUILTIN("exp2", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FExp2(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("sqrt", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FSqrt(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("rsqrt", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FRsqrt(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("saturate", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    ILTranslator tl(il);
    tl.BinaryCommand(OpType::Min, ret, arg0, il->ConstFloatOne(RegDT::Float, arg0.GetNumSlots()));
    tl.BinaryCommand(OpType::Max, ret, ret, il->ConstIntZero(RegDT::Float, arg0.GetNumSlots()));
  }
  END_BUILTIN

  typedef ReturnFixedTypeBuiltinFunction<RegDT::Float, 1, FloatsOnlyBuiltinFunction<1> > ReturnFixedTypeBuiltinFunctionFloat1FloatsOnly;
  BEGIN_BUILTIN("length", ReturnFixedTypeBuiltinFunctionFloat1FloatsOnly )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0)
  {
    il->FLen(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("lerp", FloatsOnlyBuiltinFunction<3> )
  void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    SymbolPtr tmp = new BasicSymbol(ret->Type(),0,0);
    tmp->AllocateRegisters(il);
    const SymbolPtr& x = args[0];
    const SymbolPtr& y = args[1];
    const SymbolPtr& s = args[2];

    y->HandleBinaryCommand(il, OpType::Minus,  tmp, x);
    s->HandleBinaryCommand(il, OpType::Mult, tmp, tmp);
    x->HandleBinaryCommand(il, OpType::Plus, ret, tmp);
  }
  END_BUILTIN

  BEGIN_BUILTIN("reflect", FloatsOnlyBuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 , const ILRegister& arg1)
  {
    il->FReflect(ret, arg0, arg1);
  }
  END_BUILTIN

  BEGIN_BUILTIN("smoothstep", FloatsOnlyBuiltinFunction<3> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 , const ILRegister& arg1 , const ILRegister& arg2 )
  {
    //smoothstep(min, max, x)
    ILRegister tmp = il->Scratch();
    tmp.SetNumSlots(ret.GetNumSlots());
    il->FMax(tmp, arg2, arg0);
    il->FMin(tmp, tmp, arg1);
    il->FSub(tmp, tmp, arg0);
    il->FSub(ret, arg1, arg0);
    il->FDiv(ret, tmp, ret);
  }
  END_BUILTIN

  BEGIN_BUILTIN("ldexp", FloatsOnlyBuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 , const ILRegister& arg1 )
  {
    //smoothstep(min, max, x)
    ILRegister tmp = il->Scratch();
    tmp.SetNumSlots(ret.GetNumSlots());
    il->FExp2(tmp,arg1);
    il->FMult(ret,tmp,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("frac", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 )
  {
    il->FFrac(ret,arg0);
  }
  END_BUILTIN

  //not a official function... but needed in the following opcode
  BEGIN_BUILTIN("__roundzero", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 )
  {
    il->FRoundZero(ret,arg0);
  }
  END_BUILTIN
  
  BEGIN_BUILTIN("modf", FloatsOnlyBuiltinFunction<2> )
  int ResolveDataTypes(TreeElementPtr& self, DataTypeTable*& /*scope*/)
  {
    ASSERT_ERROR(mChildElements.size()==2)(mChildElements.size())
      .Text("modf() takes two args");
    //macro expand to: arg0-(arg1=__roundzero(arg0))
    STATIC_CONSTSTRING(rzStr, "__roundzero");
    TreeElementPtr rounded = LookupBuiltinFunction(rzStr);
    rounded->AddChild(mChildElements[0]);
    self = MakeBinaryOp<BinaryOps::Minus>(mChildElements[0],MakeBinaryOp<BinaryOps::Assign>(mChildElements[1], rounded));
    return 0;
  }
  END_BUILTIN


  BEGIN_BUILTIN("ddx", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 )
  {
    il->FDerivX(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("ddy", FloatsOnlyBuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0 )
  {
    il->FDerivY(ret,arg0);
  }
  END_BUILTIN

  BEGIN_BUILTIN("atan2", FloatsOnlyBuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0, const ILRegister& arg1 )
  {
    ILRegister tmpRet = il->AllocTempRegister();
    tmpRet.SetNumSlots(1);
    ILRegister scratch = il->Scratch();
    scratch.SetNumSlots(1);
    ILRegister zero = il->ConstIntZero();
    zero.SetNumSlots(1);
    ILRegister pi = il->ConstFloatPi();
    zero.SetNumSlots(1);
    ILRegister halfpi = il->ConstFloatHalfPi();
    zero.SetNumSlots(1);
    for(int i=0; i<ret.GetNumSlots();++i)
    {
      ILRegister d = ret;
      ILRegister l = arg0;
      ILRegister r = arg1;
      { //fix swizzles
        char swiz[4] = {ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED};
        swiz[0] = d.SwizzleBytes()[i]; d.SetSwizzleBytes(swiz);
        swiz[0] = l.SwizzleBytes()[i]; l.SetSwizzleBytes(swiz);
        swiz[0] = r.SwizzleBytes()[i]; r.SetSwizzleBytes(swiz);
      }
      
      il->FDiv(tmpRet,l,r);
      il->FATan(tmpRet,tmpRet);
      il->FLt(scratch, l, zero);
      il->If(scratch);
        il->FLt(scratch, r, zero);
        il->If(scratch);
          il->FSub(tmpRet,tmpRet,pi);
        il->Else();
          il->FAdd(tmpRet,tmpRet,pi);
        il->EndIf();
      il->EndIf();
      il->FEq(scratch,l,zero);
      il->If(scratch);
        r.AddFlags(ILRegister::FLAG_SIGN);
        il->FMult(tmpRet, halfpi, r);
      il->EndIf();
      il->Move(d,tmpRet);
    }
  }
  END_BUILTIN



    /*
    sinh(x)
    tanh(x)
    cosh(x)
    atan2(y, x)
    sincos(x, out s, out c)

    rsqrt(x)

    degrees(x)
    radians(x)

    clip(x)
    D3DCOLORtoUBYTE4(x)
    determinant(m)
    distance(a, b)
    faceforward(n, i, ng)
    frac(x)
    frexp(x, exp)
    fwidth(x)

    sign(x)

    lit(n • l, n • h, m)
    noise(x)
    reflect(i, n)
    refract(i, n, R)

    step(a, x)
*/

  typedef TextureBuiltinFunction<0,1,3,4>  TextureBIF_Res_Sam_1or2extra;
  typedef TextureBuiltinFunction<0,1,4,5>  TextureBIF_Res_Sam_2or3extra;
  typedef TextureBuiltinFunction<0,1,5,6>  TextureBIF_Res_Sam_3or4extra;  
  typedef TextureBuiltinFunction<0,-1,2,2> TextureBIF_Res_1extra;
  typedef TextureBuiltinFunction<0,-1,2,3> TextureBIF_Res_1or2extra;  

///////////////////////////////////////////////
/////// Normal texture load functions 

  BEGIN_BUILTIN("tex1D", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex2D", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex3D", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN


  BEGIN_BUILTIN("texCUBE", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN


/////////////////////////////////////////////
/// Level of detail texture loaders

  // Changing TextureSampleL call to TextureSample for next three texture load function
  // SC has a bug for TextureSampleL in case number of inputs >= 64
  BEGIN_BUILTIN("tex1Dlod", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
      il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex2Dlod", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
      il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex3Dlod", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
      il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

///////////////////////////////////////////////
/////// Bias texture load functions

  BEGIN_BUILTIN("tex1Dbias", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex2Dbias", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex3Dbias", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN


  BEGIN_BUILTIN("texCUBEbias", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

///////////////////////////////////////////////
/////// Proj texture load functions

  BEGIN_BUILTIN("tex1Dproj", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex2Dproj", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("tex3Dproj", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN


  BEGIN_BUILTIN("texCUBEproj", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    il->TextureSample(ret, resource, sampler, 0, args[0]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

///////////////////////////////////////////////
/////// Grad texture load functions

/*
tex1Dgrad(s, t, ddx, ddy)
tex2Dgrad(s, t, ddx, ddy)
tex3Dgrad(s, t, ddx, ddy)
texCUBEgrad(s, t, ddx, ddy)
*/

///////////////////////////////////////////////
/////// DX10 syntax sampler

  BEGIN_BUILTIN("__objcall_GetDimensions", TextureBIF_Res_1extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    WARNING_UNIMPLEMENTED()(NAME);
    //il->TextureLoad(ret, arg0.ResourceId(), 0, arg1);
  }
  END_BUILTIN


  BEGIN_BUILTIN("__objcall_Load", TextureBIF_Res_1or2extra )
  virtual void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    SymbolPtr resource = args[0];
    ILSpec::ILPixTexUsage usage = resource->Type()->ResourceUsageType();
    char* sw = "xyzw";
    switch (usage) {
	case ILSpec::IL_USAGE_PIXTEX_1D:
	case ILSpec::IL_USAGE_PIXTEX_BUFFER:
      sw = "xxxy";
      break;
	case ILSpec::IL_USAGE_PIXTEX_1DARRAY:
    case ILSpec::IL_USAGE_PIXTEX_2D:
    case ILSpec::IL_USAGE_PIXTEX_2DMSAA:
      sw = "xyyz";
      break;
    case ILSpec::IL_USAGE_PIXTEX_2DARRAY:
    case ILSpec::IL_USAGE_PIXTEX_4COMP:
    case ILSpec::IL_USAGE_PIXTEX_CUBEMAP:      
    case ILSpec::IL_USAGE_PIXTEX_3D:
      sw = "xyzw";
      break;
    default:
      DIE("unknown texture")("");
    }
	SymbolPtr s = args[1];
	ILRegister * r = s->GetPtrRegister();
    r->SetSwizzleBytes(sw);
	TextureBIF_Res_1or2extra::builtinImpl(il, ret, args);
  }
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)                         
  {
    // args[0] is the register source for the texture
    unsigned int offset = 0;
    if (args.size() == 2){
      SymbolPtr s = args[1];
      ILRegister r = s->GetRegister();
      offset = il->LookupRegister(r);
    }
    
    ILRegister coord = ILTranslator(il).TmpConvertInput(
      RegDT::Int, args[0]->AsBasicSymbol().GetRegister());
    il->TextureLoad( ret , resource, sampler, offset , coord);
  }
  END_BUILTIN


  BEGIN_BUILTIN("__objcall_Sample", TextureBIF_Res_Sam_1or2extra)
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    // args[0] is the register source for the texture
    unsigned int offset = 0;
    if (args.size() == 2){
      SymbolPtr s = args[1];
      ILRegister r = s->GetRegister();
      offset = il->LookupRegister(r);
    }

    il->TextureSample( ret
                         , resource
                         , sampler, offset,
                         args[0]->AsBasicSymbol().GetRegister());      

    
    
  }
  END_BUILTIN


  BEGIN_BUILTIN("__objcall_SampleCmp", TextureBIF_Res_Sam_2or3extra)
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    // args[0] is the register source for the texture
    // args[1] is the comparison value
    unsigned int offset = 0;
    if (args.size() == 3){
      SymbolPtr s = args[2];
      ILRegister r = s->GetRegister();
      offset = il->LookupRegister(r);
    }

 
      il->TextureSampleCmp( ret
                         , resource
                         , sampler, offset
                         , args[0]->AsBasicSymbol().GetRegister()
                         , args[1]->AsBasicSymbol().GetRegister());    
  
    
  }
  END_BUILTIN

  BEGIN_BUILTIN("__objcall_SampleCmpLevelZero", TextureBIF_Res_Sam_2or3extra)
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    // args[0] is the register source for the texture
    // args[1] is the comparison value
    unsigned int offset = 0;
    if (args.size() == 3){
      SymbolPtr s = args[2];
      ILRegister r = s->GetRegister();
      offset = il->LookupRegister(r);
    }

 
      il->TextureSampleCmpLevelZero( ret
                         , resource
                         , sampler, offset
                         , args[0]->AsBasicSymbol().GetRegister()
                         , args[1]->AsBasicSymbol().GetRegister());    
  
    
  }
  END_BUILTIN    

  BEGIN_BUILTIN("__objcall_SampleLevel", TextureBIF_Res_Sam_2or3extra )
  void textureExpand( ILTranscriber* il, const ILRegister& ret, int resource, int sampler, const SymbolPtrList& args)
  {
    unsigned int offset = 0;
    if (args.size() == 3){
      SymbolPtr s = args[2];
      ILRegister r = s->GetRegister();
      offset = il->LookupRegister(r);
    }    
    il->TextureSampleL( ret
      , resource
      , sampler, offset
      , args[0]->AsBasicSymbol().GetRegister()
      , args[1]->AsBasicSymbol().GetRegister());
  }
  END_BUILTIN

  BEGIN_BUILTIN("__objcall_SampleGrad", TextureBIF_Res_Sam_3or4extra )

  void textureExpand( ILTranscriber* il, const ILRegister& ret,
                      int resource, int sampler, const SymbolPtrList& args)
  {

    unsigned int offset = 0;
    if (args.size() == 4){
      SymbolPtr s = args[3];
      ILRegister r = s->GetRegister();
      offset = il->LookupRegister(r);
    } 
	ILRegister coord = args[0]->AsBasicSymbol().GetRegister();
	ILRegister x = args[1]->AsBasicSymbol().GetRegister();
	ILRegister y = args[2]->AsBasicSymbol().GetRegister();
    x.SetSwizzleBytes("xxxx");
    y.SetSwizzleBytes("xxxx");
    il->TextureSampleG( ret
      , resource, sampler, offset
      , coord , x , y);
  }
  END_BUILTIN

  BEGIN_BUILTIN("__objcall_Append", BuiltinFunction<2> )
  void builtinImpl(ILTranscriber* il, const SymbolPtr& /*ret*/, const SymbolPtrList& args)
  {
    args[1]->HandleUnaryCommand(il,OpType::Assign, args[0]);
    il->GSEmit();
  }
  const AST::DataTypePtr& EffectiveDataType()
  {
    return DataType::TheVoidType();
  }
  END_BUILTIN

  BEGIN_BUILTIN("__objcall_RestartStrip", BuiltinFunction<1> )
  void builtinImpl(ILTranscriber* il, const SymbolPtr& /*ret*/, const SymbolPtrList& args)
  {
    il->GSCut();
  }
  const AST::DataTypePtr& EffectiveDataType()
  {
    return DataType::TheVoidType();
  }
  END_BUILTIN

  //typedef ReturnFixedTypeBuiltinFunction<RegDT::Float, 4, BuiltinFunction<3>> ThreeArgsMemoryLoad;
  
  ///////////////// R7xx LDS read/write and synchronization barrier

  // float4  ReadLds(int his_tid, int offset, bool do_neighborExch)
  BEGIN_BUILTIN("ReadLds", BuiltinFunction<3> )
    
    void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    checkArgCount();
    SymbolPtr s1, s2, s3 ;

    s1 = args[0]; s2 = args[1]; s3 = args[2];
    ILRegister r1 = s1->GetRegister();
    ILRegister r2 = s2->GetRegister();
    ILRegister r3 = s3->GetRegister();
    ILRegister r_ret = ret->GetRegister();
    TreeElementPtr  t3;

    // first figure out the bool
    #define NOT_FOUND -99 /* used by LookupRegister() */
    int do_fft = NOT_FOUND; 
    if ( r3.Type() == ILSpec::IL_REGTYPE_LITERAL ) {
      do_fft = il->LookupRegister(r3);
    }
    if ( do_fft <= NOT_FOUND ) {
      if ( s3->IsExplicitlyDefined() ) {
        t3 = s3->GetInitialValue();
        if ( t3->CanSimplifyToInt() ) {
          do_fft = t3->SimplifyToInt();
        }
      }
    }
    // 0xffffffffff or 0x0
    ASSERT_ERROR((do_fft == -1 || do_fft == 0))(do_fft).Text("argument 3 must be literal boolean or static constant");

    // next create Move to combine tid and offset
    ILRegister addr = il->AllocTempRegister();
    addr.SetDT(r1.GetDT()); // same type as tid
    addr.SetNumSlots(1); // addr.x is for Tid
    char broadX[4] = {ILRegister::SLOT0,ILRegister::SLOT0,ILRegister::SLOT0,ILRegister::SLOT0};    
    r1.SetSwizzleBytes(broadX);
    il->Move(addr, r1);

    // addr.y is offset
    char swiz[4] = {ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED};
    swiz[1] = 'y';
    addr.SetSwizzleBytes(swiz);
    r2.SetSwizzleBytes(broadX);
    il->Move(addr, r2);

    // finally create real read_lds instruction
    addr.SetNumSlots(2); // .xy
    il->ReadLds(r_ret, addr, do_fft);
  }

  const AST::DataTypePtr& EffectiveDataType()
  {
    return DataType::LookupSimpleType(RegDT::Float, 4);
  }
  
  END_BUILTIN

  // void WriteLds(int offset, float4 data)
  BEGIN_BUILTIN("WriteLds", BuiltinFunction<2> )
    
    void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    checkArgCount();
    SymbolPtr s1, s2;

    s1 = args[0]; s2 = args[1];
    ILRegister r1 = s1->GetRegister();
    ILRegister r2 = s2->GetRegister();
    ILRegister r_ret = ret->GetRegister();
    TreeElementPtr  t1;
    
    // first figure out static value
    #define NOT_FOUND -99 /* used by LookupRegister() */    
    int offset = NOT_FOUND; 
    if ( r1.Type() == ILSpec::IL_REGTYPE_LITERAL ) {
      offset = il->LookupRegister(r1);
    }
    if ( offset <= NOT_FOUND ) {
      if ( s1->IsExplicitlyDefined() ) {
        t1 = s1->GetInitialValue();
        if ( t1->CanSimplifyToInt() ) {
          offset = t1->SimplifyToInt();
        }
      }
    }
    ASSERT_ERROR((offset >= 0))(offset).Text("argument 1 must be literal integer or static constant");

    // create target mem
    ILRegister mem = ILRegister(0, ILSpec::IL_REGTYPE_GENERIC_MEM, RegDT::Float, 4);
    // it could have mask like zw__ (set by astoperator.cpp), will translate this into __zw
    char swiz[4] = {ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED};
    for (int i = 0; i < 4; i++ ) {
      switch (r_ret.SwizzleBytes()[i]) {
      case 'x':
        swiz[0] = 'x';
        break;
      case 'y':
        swiz[1] = 'y';        
        break;
      case 'z':
        swiz[2] = 'z';        
        break;
      case 'w':
        swiz[3] = 'w';        
        break;          
      default:
        // do nothing
        break;
      }
    }
    mem.SetSwizzleBytes(swiz);
    il->WriteLds(mem, r2, offset);
  }

  const AST::DataTypePtr& EffectiveDataType()
  {
    // Note: Though WriteLds returns nothing. We pretend it is returning a float4, so the write mask
    //      info can be stored in a fake return register. We then process the return register to figure out
    //      the write mask.
    return DataType::LookupSimpleType(RegDT::Float, 4);
  }
  
  bool IsDestOnlyBuiltinFunction() {return true;} 
  
  END_BUILTIN  

  // void SyncThreads(void)
  BEGIN_BUILTIN("SyncThreads", BuiltinFunction<0> )
    
    void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
  {
    checkArgCount();
    il->SyncThreads();
  }

  const AST::DataTypePtr& EffectiveDataType()
  {
    return DataType::TheVoidType();    
  }
  END_BUILTIN
 
} // end of name_space


TreeElementPtr AST::LookupBuiltinFunction(const ConstStringPtr& name)
{
  std::map<ConstStringPtr, BIFFactoryFunctionPtr >::const_iterator i = theBIFunctionTable().find(name);
  if(i!=theBIFunctionTable().end())
    return (*i->second)();
  else
    return 0;
}


