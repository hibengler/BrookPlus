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

#include "astoperators.hpp"

#include "astdelayedlookup.hpp"
#include "astsymbol.hpp"
#include "convert.hpp"
#include "astliterals.hpp"
#include "astcontrol.hpp"
#include "parameter.hpp"
#include "astbuiltinfunctions.hpp"

PARAMETER(p_strict, "strict");
PARAMETER(p_verbose, "verbose");

const AST::TreeElementPtr& AST::BinaryOperation::Left() const
{
  ASSERT_ERROR(mChildElements.size() == 2)(mChildElements.size());
  return mChildElements[0];
}

const AST::TreeElementPtr& AST::BinaryOperation::Right() const
{
  ASSERT_ERROR(mChildElements.size() == 2)(mChildElements.size());
  return mChildElements[1];
}

const AST::DataTypePtr& AST::BinaryOperation::EffectiveDataType()
{
  return DataType::CombineTypes(Left()->EffectiveDataType(), Right()->EffectiveDataType());
}

const AST::TreeElementPtr& 
AST::UnaryOperation::Child() const
{
  ASSERT_ERROR(mChildElements.size()==1)(mChildElements.size());
  return *Begin();
}

const AST::DataTypePtr& AST::UnaryOperation::EffectiveDataType()
{
  return Child()->EffectiveDataType();
}

int 
AST::UnaryOps::Return::ResolveSymbols( TreeElementPtr& self, SymbolTable*& scope )
{
  STATIC_CONSTSTRING(returnStr,"return");
  SymbolPtr ret = scope->Lookup(returnStr);
  ASSERT_WARNING(ret!=NULL).Text("Return may only be called from within a function");
  if(ret==NULL) return 1;

  //replace "return X;" with "<return>=X; ret_dyn;"
  mChildElements[0] = new BinaryOps::Assign(ret, mChildElements[0]);
  mChildElements[0]->SetPosition(*this);
  return 0;
}

int 
AST::UnaryOps::Return::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  il->ReturnDyn();
  return 0;
}

int 
AST::BinaryOps::Assign::PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il)
{
  //Left()->AssignmentDestRepositionHook(il,self);
  //We DONT want Operator::PostAllocateRegisters to run... it will allocate a unneeded reg for us
  return 0;
}

int 
AST::BinaryOps::Assign::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  ASSERT_ERROR(Left()->EffectiveDataType() != 0);
  ASSERT_ERROR(Right()->EffectiveDataType() != 0);
  //ASSERT_ERROR(Left()->EffectiveDataType()==Right()->EffectiveDataType())
  //  (Left()->EffectiveDataType()->Name())(Right()->EffectiveDataType()->Name())
  //  .Text("Invalid Assignment");

  SetResultSymbol(Left()->GetResultSymbol());
  ASSERT_ERROR(Left()->GetResultSymbol().IsValid())
    .Text("Left of '=' must resolve to a non-temporary symbol");

  if(Left()->GetResultSymbol()->Type()->IsBasicType()
    && Left()->GetResultSymbol()->AsBasicSymbol().IsDynamicSwizzle())
  { //special case for dynamic swizzle
    ILRegister tmp = Right()->GetResultSymbol()->GetRegister();
    Left()->GetResultSymbol()->AsBasicSymbol().DynamicSwizzleWriteValue( il, tmp );
    return 0;
  }
  

  Right()->GetResultSymbol()->HandleUnaryCommand(il, OpType::Assign, Left()->GetResultSymbol());
  return 0;
}

AST::BinaryOps::Assign::Assign( const SymbolPtr& sym, const TreeElementPtr& value )
{
  AddChild(new DelayedLookup(sym));
  AddChild(value);
}

int
AST::Operation::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
    if ( GetResultSymbol().IsValid() )
    {
        return 0;
    }
    STATIC_CONSTSTRING( tmpStr, "<tmp>" );
    SetResultSymbol( Symbol::CreateSymbol( EffectiveDataType(), tmpStr , 0 ) );
    GetResultSymbol()->AllocateRegisters( il );
    return 0;
}

int AST::BinaryOps::MemberAccess::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
    //Resolve Child()
  SymbolPtr obj = Child()->GetResultSymbol();

  if( obj->Type()->IsMatrixType())
  {
    std::vector< ILRegister > regs;
    ASSERT_ERROR(mMemberName->length()%4==0)(mMemberName)(obj->Name()).Text("invalid matrix swizzle");
    for( const char* i = mMemberName->c_str(); *i!=0; i+=4 )
    {
      if(::StartsWith(i, "_m"))
      {
        int row = i[2] - '0';
        int col = i[3] - '0';
        ASSERT_ERROR(Between(0,row,3) && Between(0,col,3))(row)(col)(mMemberName)(obj->Name())
          .Text("invalid matrix swizzle");
        regs.push_back(obj->GetMatrixElement(row,col));
      }
      else
      {
        ASSERT_WARNING(::StartsWith(i, "_m"))
          (i)(mMemberName)(obj->Name())
          .Text("invalid matrix swizzle");
        return 1;
      }
    }

    ASSERT_ERROR(regs.size()>0)(mMemberName)(obj->Name()).Text("invalid swizzle");
    
    if(regs.size()==1) 
    {
      SymbolPtr rslt = new BasicSymbol( EffectiveDataType(), obj->Name(), 0);
      rslt->AsBasicSymbol().SetRegister(regs[0]);    
      SetResultSymbol(rslt);
    }
    else
    {
      //replace this with a typecaster
      self = new TypeCaster( EffectiveDataType(), TreeElementList() ) ;
      for(size_t i=0; i<regs.size(); ++i)
      {
        STATIC_CONSTSTRING(tmpStr,"<tmp>");
        SymbolPtr sym = new BasicSymbol( DataType::LookupSimpleType(regs[i].GetDT()), tmpStr, 0);
        sym->AsBasicSymbol().SetRegister(regs[i]);    
        self->AddChild(new DelayedLookup(sym));
      }
    }
  }
  else if( obj->Type()->IsDoubleType())
  {
    STATIC_CONSTSTRING(xStr,"x");
    STATIC_CONSTSTRING(yStr,"y");
    STATIC_CONSTSTRING(zStr,"z");
    STATIC_CONSTSTRING(wStr,"w");
    SymbolPtr swiz[4] = {0,0,0,0};
    ASSERT_ERROR(mMemberName->size() <= 4)(mMemberName)
      .Text("swizzle too long");
    for(size_t i=0; i<mMemberName->size(); ++i)
    {
      switch(mMemberName->at(i))
      {
      case 'x': case 'r':
        swiz[i] = obj->GetSubpart(xStr);
        break;
      case 'y': case 'g':
        swiz[i] = obj->GetSubpart(yStr);
        break;
      case 'z': case 'b':
        swiz[i] = obj->GetSubpart(zStr);
        break;
      case 'w': case 'a':
        swiz[i] = obj->GetSubpart(wStr);
        break;
      default: 
        ASSERT_WARNING(false)(mMemberName).Text("Expected subset of 'xyzw' to right of '.'");
        return 1;
      }
    }

    SymbolPtr rslt = new MultipartSymbol(EffectiveDataType(), obj->Name(), 0);
    if(swiz[0].IsValid()) rslt->GetSubpart(xStr)->AsBasicSymbol().SetRegister(swiz[0]->GetRegister());
    if(swiz[1].IsValid()) rslt->GetSubpart(yStr)->AsBasicSymbol().SetRegister(swiz[1]->GetRegister());
    if(swiz[2].IsValid()) rslt->GetSubpart(zStr)->AsBasicSymbol().SetRegister(swiz[2]->GetRegister());
    if(swiz[3].IsValid()) rslt->GetSubpart(wStr)->AsBasicSymbol().SetRegister(swiz[3]->GetRegister());
    SetResultSymbol(rslt);
  }
  else if( obj->Type()->IsMultipart() )
  {  
      //handle struct case

      //Lookup right
      SymbolPtr rslt = obj->GetSubpart( mMemberName );
      ASSERT_WARNING( rslt.IsValid() )( obj->ToString() )( mMemberName )
          .Text( "Failed to lookup member of struct" );

      if( ! rslt.IsValid() )
      {
          return 1;
      }

      //Replace this
      //self = new DelayedLookup(rslt);
      //return self->PreAllocateRegisters(self, il)
      //     + self->PostAllocateRegisters(self, il);
      SetResultSymbol( rslt );
  }
  else
  { //must be a .xyzw access

    //update swizzle
    ILRegister reg = obj->AsBasicSymbol().GetRegister();
    ASSERT_ERROR(reg);
    
    ASSERT_WARNING(mMemberName->size() <= 4)(mMemberName).Text("Expected subset of 'xyzw' to right of '.'");
    if(mMemberName->size()>4) return 1;
    
    char swiz[4];
    memset(swiz, ILRegister::SLOTNOTUSED, sizeof(swiz));

    for(size_t i=0; i<mMemberName->size(); ++i)
    {
      switch(mMemberName->at(i))
      {
      case 'x':
      case 'y':
      case 'z':
      case 'w':
        swiz[i] = mMemberName->at(i);
        break;
      case 'r':
        swiz[i] = 'x';
        break;
      case 'g':
        swiz[i] = 'y';
        break;
      case 'b':
        swiz[i] = 'z';
        break;
      case 'a':
        swiz[i] = 'w';
        break;
      default: 
        ASSERT_WARNING(false)(mMemberName).Text("Expected subset of 'xyzw' to right of '.'");
        return 1;
      }
    }
    reg.SetSwizzleBytes(swiz);
    
    SymbolPtr rslt = new BasicSymbol( EffectiveDataType(), obj->Name(), 0);
    rslt->AsBasicSymbol().SetRegister(reg);    
    SetResultSymbol(rslt);

    //A special case: a built-in function can have masks, such as void WriteLds(offset, data)
    if ( Child()->IsDestOnlyBuiltinFunction() ) {
      // It must be a mask, put it into object (builtin_func) symbol
      obj->AsBasicSymbol().SetRegister(reg); 
    }
    
         
  }
  return 0;
}

int AST::BinaryOps::MemberAccess::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  // Re assigne target registers if they are modified by temporaries...
  //Resolve Child()
  SymbolPtr obj = Child()->GetResultSymbol();

  if( obj->Type()->IsMatrixType())
  {
    std::vector< ILRegister > regs;
    ASSERT_ERROR(mMemberName->length()%4==0)(mMemberName)(obj->Name()).Text("invalid matrix swizzle");
    for( const char* i = mMemberName->c_str(); *i!=0; i+=4 )
    {
      if(::StartsWith(i, "_m"))
      {
        int row = i[2] - '0';
        int col = i[3] - '0';
        ASSERT_ERROR(Between(0,row,3) && Between(0,col,3))(row)(col)(mMemberName)(obj->Name())
          .Text("invalid matrix swizzle");
        regs.push_back(obj->GetMatrixElement(row,col));
      }
      else
      {
        ASSERT_WARNING(::StartsWith(i, "_m"))
          (i)(mMemberName)(obj->Name())
          .Text("invalid matrix swizzle");
        return 1;
      }
    }

    ASSERT_ERROR(regs.size()>0)(mMemberName)(obj->Name()).Text("invalid swizzle");
    
    if(regs.size()==1) 
    {
      SymbolPtr rslt = new BasicSymbol( EffectiveDataType(), obj->Name(), 0);
      rslt->AsBasicSymbol().SetRegister(regs[0]);    
      SetResultSymbol(rslt);
    }
    else
    {
      //replace this with a typecaster
      self = new TypeCaster( EffectiveDataType(), TreeElementList() ) ;
      for(size_t i=0; i<regs.size(); ++i)
      {
        STATIC_CONSTSTRING(tmpStr,"<tmp>");
        SymbolPtr sym = new BasicSymbol( DataType::LookupSimpleType(regs[i].GetDT()), tmpStr, 0);
        sym->AsBasicSymbol().SetRegister(regs[i]);    
        self->AddChild(new DelayedLookup(sym));
      }
    }
  }
  else if( obj->Type()->IsDoubleType())
  {
    STATIC_CONSTSTRING(xStr,"x");
    STATIC_CONSTSTRING(yStr,"y");
    STATIC_CONSTSTRING(zStr,"z");
    STATIC_CONSTSTRING(wStr,"w");
    SymbolPtr swiz[4] = {0,0,0,0};
    ASSERT_ERROR(mMemberName->size() <= 4)(mMemberName)
      .Text("swizzle too long");
    for(size_t i=0; i<mMemberName->size(); ++i)
    {
      switch(mMemberName->at(i))
      {
      case 'x': case 'r':
        swiz[i] = obj->GetSubpart(xStr);
        break;
      case 'y': case 'g':
        swiz[i] = obj->GetSubpart(yStr);
        break;
      case 'z': case 'b':
        swiz[i] = obj->GetSubpart(zStr);
        break;
      case 'w': case 'a':
        swiz[i] = obj->GetSubpart(wStr);
        break;
      default: 
        ASSERT_WARNING(false)(mMemberName).Text("Expected subset of 'xyzw' to right of '.'");
        return 1;
      }
    }

    SymbolPtr rslt = new MultipartSymbol(EffectiveDataType(), obj->Name(), 0);
    if(swiz[0].IsValid()) rslt->GetSubpart(xStr)->AsBasicSymbol().SetRegister(swiz[0]->GetRegister());
    if(swiz[1].IsValid()) rslt->GetSubpart(yStr)->AsBasicSymbol().SetRegister(swiz[1]->GetRegister());
    if(swiz[2].IsValid()) rslt->GetSubpart(zStr)->AsBasicSymbol().SetRegister(swiz[2]->GetRegister());
    if(swiz[3].IsValid()) rslt->GetSubpart(wStr)->AsBasicSymbol().SetRegister(swiz[3]->GetRegister());
    SetResultSymbol(rslt);
  }
  else if( obj->Type()->IsMultipart() )
  {  
      //handle struct case

      //Lookup right
      SymbolPtr rslt = obj->GetSubpart( mMemberName );
      ASSERT_WARNING( rslt.IsValid() )( obj->ToString() )( mMemberName )
          .Text( "Failed to lookup member of struct" );

      if( ! rslt.IsValid() )
      {
          return 1;
      }

      //Replace this
      //self = new DelayedLookup(rslt);
      //return self->PreAllocateRegisters(self, il)
      //     + self->PostAllocateRegisters(self, il);
      SetResultSymbol( rslt );
  }
  else
  { //must be a .xyzw access

    //update swizzle
    ILRegister reg = obj->AsBasicSymbol().GetRegister();
    ASSERT_ERROR(reg);
    
    ASSERT_WARNING(mMemberName->size() <= 4)(mMemberName).Text("Expected subset of 'xyzw' to right of '.'");
    if(mMemberName->size()>4) return 1;
    
    char swiz[4];
    memset(swiz, ILRegister::SLOTNOTUSED, sizeof(swiz));

    for(size_t i=0; i<mMemberName->size(); ++i)
    {
      switch(mMemberName->at(i))
      {
      case 'x':
      case 'y':
      case 'z':
      case 'w':
        swiz[i] = mMemberName->at(i);
        break;
      case 'r':
        swiz[i] = 'x';
        break;
      case 'g':
        swiz[i] = 'y';
        break;
      case 'b':
        swiz[i] = 'z';
        break;
      case 'a':
        swiz[i] = 'w';
        break;
      default: 
        ASSERT_WARNING(false)(mMemberName).Text("Expected subset of 'xyzw' to right of '.'");
        return 1;
      }
    }
    reg.SetSwizzleBytes(swiz);
    
    SymbolPtr rslt = new BasicSymbol( EffectiveDataType(), obj->Name(), 0);
    rslt->AsBasicSymbol().SetRegister(reg);    
    SetResultSymbol(rslt);

    //A special case: a built-in function can have masks, such as void WriteLds(offset, data)
    if ( Child()->IsDestOnlyBuiltinFunction() ) {
      // It must be a mask, put it into object (builtin_func) symbol
      obj->AsBasicSymbol().SetRegister(reg); 
    }
    
         
  }
  return 0;
}

const AST::DataTypePtr& AST::BinaryOps::MemberAccess::EffectiveDataType()
{
  //Resolve Child()
  //SymbolPtr obj = Child()->GetResultSymbol();
  DataTypePtr type = Child()->EffectiveDataType();
  ASSERT_ERROR(type.IsValid())(Child()->DebugClassName())(mMemberName)
    .Text("Failed to lookup object type");
  if(type->IsMatrixType())
  {
    int slots = 0;
    for( const char* i = mMemberName->c_str(); *i!=0; ++i )
      if(::StartsWith(i, "_m"))  
        ++slots;
    ASSERT_ERROR(Between(0,slots,3))(slots)(mMemberName)(type->ToString())
      .Text("invalid matrix swizzle");
    return DataType::LookupSimpleType(type->GetRegDT(), slots);
  }
  else if( type->IsMultipart() && !type->IsDoubleType())
  {//handle struct case
    SymbolPtr rslt = type->GetParts().Lookup(mMemberName);
    ASSERT_ERROR(rslt.IsValid())(type->ToString())(mMemberName)
      .Text("Failed to lookup member of struct");
    return rslt->Type();
  }
  else
  { //must be a .xyzw access
    int slots = 0;
    for(size_t i=0; i<mMemberName->size(); ++i)
    {
      switch(mMemberName->at(i))
      {
      case 'x': case 'y': case 'z': case 'w':
      case 'r': case 'g': case 'b': case 'a':
        slots++;
      }
    }
    ASSERT_ERROR(slots >= 1 && slots <= 4)(slots)(mMemberName)
      .Text("invalid swizzle string");
    return DataType::LookupSimpleType(type->GetRegDT(),slots);
  }
}
int AST::ArrayIndex::PostExportToIL( TreeElementPtr& /*self*/, ILTranscriber* il )
{
  if(Left()->GetResultSymbol()->IsArraySymbol() )
    Left()->GetResultSymbol()->AsArraySymbol().DoArrayIndexIL(il,mBase,Right()->GetResultSymbol()); 
  else if(Left()->GetResultSymbol()->Type()->IsMatrixType())
  {
    ILTranslator tl(il);
    ILRegister idx = Right()->GetResultSymbol()->GetRegister();
    ASSERT_WARNING(idx.GetNumSlots()==1)(idx.ToString()).Text("expected a single slot index for operator[]");
    ILRegister row0 = Left()->GetResultSymbol()->GetMatrixRow(0);
    ILRegister row0_base = row0.IsArrayType() ? row0.GetBase() : il->ConstIntZero(RegDT::Int,1);
    //fill base with calculated new base that incorperates user offset
    tl.BinaryCommand(OpType::Plus, mBase, idx, row0_base);
    //and make sure it doesn't over/under flow
    tl.BinaryCommand(OpType::Min, mBase, mBase, il->ConstIntThree(RegDT::Int,1));
    tl.BinaryCommand(OpType::Max, mBase, mBase, il->ConstIntZero(RegDT::Int,1));
  }
  else if(Left()->GetResultSymbol()->Type()->IsBasicType())
  { 

    ILRegister reg  = Left()->GetResultSymbol()->GetRegister();
    ILRegister idx  = Right()->GetResultSymbol()->GetRegister();
    ASSERT_WARNING(idx.GetNumSlots()==1)(idx.ToString()).Text("expected a single slot index for operator[]");

    GetResultSymbol()->AsBasicSymbol().SetDynamicSwizzle(reg, idx);
    GetResultSymbol()->AsBasicSymbol().DynamicSwizzleReadValue(il);
  }
  return 0;
}

int AST::ArrayIndex::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  STATIC_CONSTSTRING(name,"<tmp>");

  if(Left()->GetResultSymbol()->IsArraySymbol())
  {//Normal case
    SetResultSymbol(Left()->GetResultSymbol()->AsArraySymbol().DoArrayIndexAlloc(il,mBase,Right()->GetResultSymbol()));   
    return 0;
  }
  else if(Left()->GetResultSymbol()->Type()->IsMatrixType())
  {//special case for indexing matrix types
    if(Right()->CanSimplifyToInt())
    {
      int idx = Right()->SimplifyToInt();
      ASSERT_ERROR(Between(0,idx,Left()->GetResultSymbol()->Type()->NumRows()))(idx)(Left()->GetResultSymbol()->Type()->NumRows())
        .Text("out of bounds access");
    }

    mBase = il->AllocTempRegister();
    mBase.SetDT(RegDT::Int);
    mBase.SetNumSlots(1);
    ILRegister row0 = Left()->GetResultSymbol()->GetMatrixRow(0);
    //and assign a base 
    row0.SetBase(mBase);
    SymbolPtr sm = Symbol::CreateSymbol(DataType::LookupSimpleType(row0.GetDT(),row0.GetNumSlots()), name, 0 );
    sm->AsBasicSymbol().SetRegister(row0);
    SetResultSymbol(sm);
  }
  else if(Left()->GetResultSymbol()->Type()->IsBasicType())
  {
    if(Right()->CanSimplifyToInt())
    {
      int idx = Right()->SimplifyToInt();
      ASSERT_ERROR(Between(0,idx,Left()->GetResultSymbol()->Type()->NumCols()))(idx)(Left()->GetResultSymbol()->Type()->NumCols())
        .Text("out of bounds access");
    }

    ILRegister reg = Left()->GetResultSymbol()->GetRegister();
    mBase = il->AllocTempRegister();
    mBase.SetDT(reg.GetDT());
    mBase.SetNumSlots(1);
    SymbolPtr sm = Symbol::CreateSymbol(DataType::LookupSimpleType(reg.GetDT(),1), name, 0 );
    sm->AsBasicSymbol().SetRegister(mBase);
    SetResultSymbol(sm);
  }
  else
  {
    DIE("operator[] is unknown for this type")(Left()->EffectiveDataType()->ToString());
  }
  return 0;
}

const AST::DataTypePtr& AST::ArrayIndex::EffectiveDataType()
{
  const DataTypePtr& dt = Left()->EffectiveDataType();
  if(dt->IsMatrixType())
  {//special case for indexing matrix types
    return DataType::LookupSimpleType(dt->GetRegDT(), dt->NumCols());
  }
  else if(dt->IsBasicType() || dt->IsDoubleType())
  {
    return DataType::LookupSimpleType(dt->GetRegDT());
  }
  else
  {
    return dt;
  }
}
int AST::BinaryOps::PostAssign::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  ASSERT_ERROR(GetResultSymbol().IsValid());
  ASSERT_ERROR(Left()->GetResultSymbol().IsValid());
  Left()->GetResultSymbol()->HandleUnaryCommand(il, OpType::Assign, GetResultSymbol());
  SymbolPtr rs = GetResultSymbol();
  int t=Assign::PostExportToIL(self,il);
  SetResultSymbol(rs);
  return t;
}

int AST::BinaryOps::PostAssign::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  TreeElementPtr _self = self;
  //Left()->AssignmentDestRepositionHook(il,_self);
  ASSERT_ERROR(_self == self)
    .Text("Post assignment is not supported here");
  return Operation::PostAllocateRegisters(self,il);
}
const AST::DataTypePtr& AST::TypeCaster::EffectiveDataType()
{
  return mTargetType;
}

int AST::TypeCaster::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  // for now always allocate a temporary
  // later we may want to reuse the existing register
  return Operation::PostAllocateRegisters(self,il);
}

int AST::TypeCaster::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  ILTranslator tl(il);

  if(mTargetType->IsBasicType() || mTargetType->IsDoubleType())
  { //handle double convert    
    for(size_t i=0; i<mChildElements.size(); ++i)
    {
      SymbolPtr sym = mChildElements[i]->GetResultSymbol();
      SymbolPtr symOrig = sym;
      if(sym->Type()->IsBasicType() || sym->Type()->IsDoubleType())
      {
        Symbol::ConvertHook(il, sym, DataType::LookupSimpleType(mTargetType->GetRegDT(), sym->Type()->GetRegSlots()));
        if(sym != symOrig)
        {
            // a change occurred; apply temporary patch
            TreeElementPtr origElmt = mChildElements[i];
            mChildElements[i] = new DelayedLookup(sym);

            //retry
            int rslt = PostExportToIL(self,il);
          
            //unapply patch
            mChildElements[i] = origElmt;
            return rslt;
        }
      }
    }
  }
  

  //special case for casting to matrix
  if(mTargetType->IsMatrixType())
  {
    if(mChildElements.size()==1)
    {
      ASSERT_ERROR(Child()->EffectiveDataType()->IsMatrixType())
        (Child()->EffectiveDataType()->ToString())(mTargetType->ToString())
        .Text("Casting from non-matrix type to matrix type is not supported");

      int targRows   = mTargetType->NumRows();
      int targCols   = mTargetType->NumCols();
      int sourceRows = Child()->EffectiveDataType()->NumRows();
      int sourceCols = Child()->EffectiveDataType()->NumCols();

      ASSERT_ERROR(targRows <= sourceRows && targCols <= sourceCols)
        (targRows)(sourceRows)(targCols)(sourceCols)
        .Text("Can not cast from smaller to larger matrix type");

      for(int i=0; i<targRows; ++i)
      { 
        ILRegister src = Child()->GetResultSymbol()->GetMatrixRow(i);
        ILRegister dst = GetResultSymbol()->GetMatrixRow(i);
        if(targCols!=sourceCols) src.SetNumSlots(targCols);
        tl.UnaryCommand(OpType::Assign, dst, src);
      }
    }
    else
    {
      int curRow=0;
      for(Iterator i=Begin(); i!=End(); ++i)
      {
        ASSERT_ERROR(curRow < mTargetType->NumRows())(mTargetType->ToString())(curRow)(mTargetType->NumRows())
          .Text("Too much data for cast");
        ILRegister dst = GetResultSymbol()->GetMatrixRow(curRow++);
        ILRegister src = (*i)->GetResultSymbol()->GetRegister();
        tl.UnaryCommand(OpType::Assign, dst, src);
      }
      ASSERT_ERROR(curRow == mTargetType->NumRows())(mTargetType->ToString())(curRow)(mTargetType->NumRows())
        .Text("Too little data for cast");
    }
    return 0;
  }

  if(mTargetType == DataType::TheVoidType())
  { //casting to void is really trivial
    return 0;
  }

  // this should copy multiple elements from src to dst
  if( mTargetType->IsMultipart() )
  {
    std::vector<ILRegister> dst = GetResultSymbol()->GetRawRegisters();
    std::vector<ILRegister> src = mChildElements[0]->GetResultSymbol()->GetRawRegisters();
    for ( int ii = 1; ii < NumChildren(); ++ii )
    {
        std::vector<ILRegister> regs = mChildElements[ii]->GetResultSymbol()->GetRawRegisters();
        for ( std::vector<ILRegister>::iterator jj = regs.begin(); jj != regs.end(); ++jj )
        {
            src.push_back( *jj );
        }
    }
    if(p_verbose) ASSERT_WARNING(dst.size()==src.size())(mTargetType->ToString())(dst.size())(src.size())
      .Text("size mismatch in cast");
    size_t cnt = Minimum(dst.size(),src.size());
    for(size_t i=0; i<cnt; ++i)
    {
      if(p_verbose) ASSERT_WARNING(dst[i].GetNumSlots()==src[i].GetNumSlots())(mTargetType->ToString())(dst[i].GetNumSlots())(src[i].GetNumSlots())(i)
        .Text("size mismatch in cast");
      //fix swizzles
      if(dst[i].GetNumSlots() < src[i].GetNumSlots())
        src[i].SetNumSlots(dst[i].GetNumSlots());
      if(src[i].GetNumSlots() < dst[i].GetNumSlots())
        dst[i].SetNumSlots(src[i].GetNumSlots());
      il->Move(dst[i],src[i]);
    }
    return 0;
  }

  const int neededSlots = mTargetType->GetRegSlots();
  int foundSlots = 0;
  DEBUG_CHECK(GetResultSymbol().IsValid());
  ILRegister dst = GetResultSymbol()->GetRegister();


  for(ConstIterator i = Begin(); i!=End(); ++i)
  {
    ILRegister srcI = tl.TmpConvertInput(dst.GetDT(),(*i)->GetResultSymbol()->GetMatrixRow(0));
    if(neededSlots-foundSlots < srcI.GetNumSlots())
    {
      if(neededSlots-foundSlots == 0) break;
      srcI = tl.TmpReslotInput(neededSlots-foundSlots, srcI);
    }
    

    //calculate swizzle
    char swiz[4];
    memset(swiz, ILRegister::SLOTNOTUSED, sizeof(swiz));
    memcpy(swiz, ILRegister::DEFAULT_SWIZZLE+foundSlots, srcI.GetNumSlots());
    foundSlots += srcI.GetNumSlots();
    dst.SetSwizzleBytes(swiz);
    tl.UnaryCommand(OpType::Assign, dst, srcI );
  }
  
  if(p_strict)
    ASSERT_ERROR(neededSlots==foundSlots)(neededSlots)(foundSlots)(mTargetType->ToString())
      .Text("Unable to perform conversion");
  else
  {
    if(foundSlots < neededSlots) 
    {
      //fill in rest with zeros
      char swiz[4];
      memset(swiz, ILRegister::SLOTNOTUSED, sizeof(swiz));
      memcpy(swiz, ILRegister::DEFAULT_SWIZZLE+foundSlots, neededSlots-foundSlots);
      dst.SetSwizzleBytes(swiz);
      tl.UnaryCommand(OpType::Assign, dst, il->ConstIntZero(dst.GetDT(), neededSlots-foundSlots) );
    }
  }
  return 0;
}



int AST::BinaryOps::PrecomputeLeftReturnRight::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  SetResultSymbol(Right()->GetResultSymbol()); 
  return 0;
}
