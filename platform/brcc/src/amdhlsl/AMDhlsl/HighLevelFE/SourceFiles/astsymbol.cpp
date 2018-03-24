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

#include "astsymbol.hpp"
#include "astcontrol.hpp"
#include "astdatatype.hpp"
#include "astoperators.hpp"
#include "astdelayedlookup.hpp"
#include "astliterals.hpp"
#include "astelement.hpp"
#include "parameter.hpp"
#include "astdeclaration.hpp"
#include <sstream>
#include <string>

PARAMETER(p_case_sensitive_modifiers, "strict" )
PARAMETER(p_strict,                   "strict" )
PARAMETER(p_packarrays,               "packarrays" )
PARAMETER(p_rowmajor,                 "rowmajor" )

typedef std::map<ConstStringPtr, void (AST::Symbol::*)(ILTranscriber*,const AST::Modifier&) > ModifierActionMap;

static inline void _fillVariableDesc(ShaderDescription::VariableDesc& desc, AST::Symbol& sym)
{
  desc.Name     = sym.GetDisplayName();
  sym.Type()->FillDescription(desc.DataType);
  if(sym.IsArraySymbol())
  {
    desc.DataType.ElementSizeInRegisters = sym.AsArraySymbol().ElementSize();
    desc.DataType.NumArrayElements =       sym.AsArraySymbol().ElementCount();
  }
  else
  {
    desc.DataType.ElementSizeInRegisters = sym.SizeInRegisters();
    desc.DataType.NumArrayElements =      -1;
  }
}

ModifierActionMap generateModifierActionMap()
{
#define ADD_MODIFIER_ACTION(name, action) t[ConstString::Lookup(p_case_sensitive_modifiers?name:ToLower(name))] = &AST::Symbol::GenerateModifierIL##action;
  ModifierActionMap t;

  ADD_MODIFIER_ACTION("extern",                     Ignored);
  ADD_MODIFIER_ACTION("nointerpolation",            Unsupported);
  ADD_MODIFIER_ACTION("shared",                     Unsupported);
  ADD_MODIFIER_ACTION("static",                     Ignored);
  ADD_MODIFIER_ACTION("uniform",                    Ignored);
  ADD_MODIFIER_ACTION("volatile",                   Ignored);
  ADD_MODIFIER_ACTION("const",                      Ignored);
  ADD_MODIFIER_ACTION("row_major",                  Ignored);
  ADD_MODIFIER_ACTION("column_major",               Unsupported);
  ADD_MODIFIER_ACTION("inline",                     Ignored);
  ADD_MODIFIER_ACTION("target",                     Unsupported);
  ADD_MODIFIER_ACTION("linear",                     Ignored);
  ADD_MODIFIER_ACTION("in",                         OnlyInArgContext );
  ADD_MODIFIER_ACTION("out",                        OnlyInArgContext );
  ADD_MODIFIER_ACTION("inout",                      OnlyInArgContext );


  ADD_MODIFIER_ACTION("triangleadj",                Ignored);
  ADD_MODIFIER_ACTION("triangle",                   Ignored);
  ADD_MODIFIER_ACTION("lineadj",                    Ignored);
  ADD_MODIFIER_ACTION("line",                       Ignored);
  ADD_MODIFIER_ACTION("pointadj",                   Ignored);
  ADD_MODIFIER_ACTION("point",                      Ignored);


  return t;
}




AST::Symbol::Symbol( const DataTypePtr& type
                    , const ConstStringPtr& name
                    , const TreeElementPtr& value ) 
  : mName(name)
  , mType(type)
  , mInitialValue(value)
  , mIsExplicitlyDefined(false)
{}

AST::Symbol::Symbol( const Symbol& that ) 
  : RefCountedObj()
  , mName(that.mName)
  , mType(that.mType)
  , mInitialValue(0)
  , mModifiers(that.mModifiers)
  , mIsExplicitlyDefined(false)
{

}
const ConstStringPtr& 
AST::Symbol::Name() const
{
  return mName;
}

const AST::DataTypePtr& 
AST::Symbol::Type() const
{
  return mType;
}

std::string 
AST::Symbol::ToString() const
{
  std::ostringstream os;
  os << '('  << mType->ToString() << ')' << *mName;
  return os.str();

}

AST::SymbolPtr AST::Symbol::CreateSymbol( const DataTypePtr& type , const ConstStringPtr& _name , const TreeElementPtr& value )
{
  STATIC_CONSTSTRING(tmpStr,"<tmp>");
  ConstStringPtr name( _name.IsValid() ? _name : tmpStr);

  if(type->IsMultipart())  
    return new MultipartSymbol(type,name,value);
  else if(type->IsResourceType())
  {
    ASSERT_ERROR(value==0)(name)(type->ToString()).Text("Resource types can not have initializers");;
    return new ResourceSymbol(type,name);
  }
  else
    return new BasicSymbol(type,name,value);
}

// void AST::Symbol::ExtractInitializers( const TreeElementPtr& e )
// {
//   if(mInitialValue!=0)
//   {
//     e->AddChild(mInitialValue);
//     mInitialValue=0;
//   }
// }

void AST::Symbol::AssertNoInitializers() const
{
  ASSERT_ERROR(mInitialValue==0)
    (mInitialValue->GetPosition())
    (mInitialValue->DebugClassName())
      .Text("variable initializer unexpected in this context");
}

bool AST::Symbol::HasFlag( const Modifier& fl ) const
{
  return mModifiers.find(fl)!=mModifiers.end();
}

bool 
AST::Symbol::IsFlaggedOutput() const
{
  static Modifier f_out(ConstString::Lookup("out"), Modifier::MOD_KEYWORD);
  static Modifier f_inout(ConstString::Lookup("inout"), Modifier::MOD_KEYWORD);
  return HasFlag(f_out)
    || HasFlag(f_inout);
}

bool 
AST::Symbol::IsFlaggedInput() const
{
  static Modifier f_in(ConstString::Lookup("in"), Modifier::MOD_KEYWORD);
  static Modifier f_inout(ConstString::Lookup("inout"), Modifier::MOD_KEYWORD);
  return HasFlag(f_in)
    || HasFlag(f_inout)
    ||!IsFlaggedOutput();
}
bool AST::Symbol::IsFlaggedStatic() const
{
  static Modifier staticstr(ConstString::Lookup("static"), Modifier::MOD_KEYWORD);
  return HasFlag(staticstr);
}
bool AST::Symbol::IsFlaggedUniform() const
{
  static Modifier staticstr(ConstString::Lookup("uniform"), Modifier::MOD_KEYWORD);
  return HasFlag(staticstr);
}
bool AST::Symbol::IsFlaggedColMajor() const
{
  static Modifier staticstr1(ConstString::Lookup("col_major"), Modifier::MOD_KEYWORD);
  static Modifier staticstr2(ConstString::Lookup("row_major"), Modifier::MOD_KEYWORD);
  bool col_major = HasFlag(staticstr1);
  bool row_major = HasFlag(staticstr1);
  ASSERT_WARNING(col_major+row_major <= 1)(col_major)(row_major)
    .Text("a matrix cant be both col and row major");

  if(col_major) return true;
  if(row_major) return false;
  return ! p_rowmajor; //default value
}
void AST::Symbol::SetModifiers( const ModifierSet& m )
{
  for(ModifierSet::const_iterator i=m.begin(); i!=m.end(); ++i)
    mModifiers.insert(*i);
}

ILRegister AST::Symbol::GetMatrixElement( int row, int col ) const
{
  DIE("matrix operations not supported on this data type")(ToString()); return -1;
}

ILRegister AST::Symbol::GetMatrixRow( int row ) const
{
  ASSERT_ERROR(row==0)(row)(ToString()).Text("out of bounds array access");
  return GetRegister();
}

void AST::Symbol::handleModifiers( ILTranscriber* il )
{
  if(mIsExplicitlyDefined)
  {
    for(ModifierSet::const_iterator i = mModifiers.begin(); i!=mModifiers.end(); ++i)
    {
      static ModifierActionMap theActionMap = generateModifierActionMap();
      
      DEBUG_CHECK(i->IsValid());

      if(i->Type() == Modifier::MOD_KEYWORD)
      {
        ModifierActionMap::const_iterator action = theActionMap.find(*i);
        ASSERT_ERROR(action!=theActionMap.end())(ToString())(**i).Text("unknown modifier");
        (this->*(action->second)) ( il, *i );
      }
      else if(i->Type() == Modifier::MOD_SEMANTIC)
      {
        handleSemantic(il, *i);
      }
      else
        SPAM("Register modifiers are not supported")(*i);
    }

    //Do constant buffer
    if((il->CurrentContext() == ILTranscriber::CTX_MAIN_ARGS && !HasSemantic())
      ||(il->CurrentContext() == ILTranscriber::CTX_GLOBAL && !IsFlaggedStatic()))
    { 
      ShaderDescription::VariableDesc desc;
      _fillVariableDesc(desc,*this);
      il->SourceWithConstBuff(desc, GetRawRegisters());
    }
  }
}

void AST::Symbol::handleSemantic( ILTranscriber* il,const Modifier& _mod )
{
  Modifier mod(_mod);//local copy
  if(il->CurrentContext()==ILTranscriber::CTX_MAIN_ARGS)
  {
    std::vector<ILRegister> regs = GetRawRegisters();

    if( il->GetShaderDescription().IsValid() &&
        regs.size()>0 &&
        mod.LookupSemanticUsage() != ILSpec::IL_IMPORTUSAGE_UNDEFINED  )
    {
      ShaderDescription::InputOutputVariableDesc desc;
      _fillVariableDesc(desc.Variable,*this);
      desc.ImportUsage = mod.LookupSemanticUsage();
      desc.Semantic.Name = mod;
      desc.Semantic.Number = mod.Number();
      if(IsFlaggedInput()) il->GetShaderDescription()->Inputs.push_back(desc);
      if(IsFlaggedOutput()) il->GetShaderDescription()->Outputs.push_back(desc);
    }

    for(std::vector<ILRegister>::const_iterator i=regs.begin(); i!=regs.end(); ++i)
    {
      if(IsFlaggedInput()) il->AddInputSemantic(mod ,GetInterpOptions(), *i);
      if(IsFlaggedOutput()) il->AddOutputSemantic(mod, *i);
      mod.IncrementNumber();
    }
  }
  else if(il->CurrentContext()==ILTranscriber::CTX_GLOBAL)
  {
    SPAM("Semantics not yet supported in global scope")(ToString())(mod);
  }
  else if(il->CurrentContext()==ILTranscriber::CTX_FUNC_ARGS)
  {
    SPAM("Semantics are ignored on functions other than main")(ToString())(mod);
  }
}

void AST::Symbol::GenerateModifierILOnlyInArgContext( ILTranscriber* il, const Modifier& name )
{
  ASSERT_ERROR(il->IsFunctionArgContext())
    (*name).Text("This modifier only expected in a function arg context");
}
void AST::Symbol::GenerateModifierILUnsupported( ILTranscriber*, const Modifier& name )
{
  NOTICE("modifiern not yet supported")(*name);
}

ILSpec::ILInterpMode AST::Symbol::GetInterpOptions() const
{
  using namespace ILSpec;

  ILSpec::ILInterpMode i = IL_INTERPMODE_NOTUSED;

  //TODO: add support for the rest of these

  //  IL_INTERPMODE_CONSTANT,
  static Modifier LINEAR(ConstString::Lookup("linear"), Modifier::MOD_KEYWORD);

  if(HasFlag(LINEAR)) i = IL_INTERPMODE_LINEAR;
  //       IL_INTERPMODE_LINEAR_CENTROID,
  //       IL_INTERPMODE_LINEAR_NOPERSPECTIVE,
  //       IL_INTERPMODE_LINEAR_NOPERSPECTIVE_CENTROID,
  //       IL_INTERPMODE_LINEAR_SAMPLE,
  //       IL_INTERPMODE_LINEAR_NOPERSPECTIVE_SAMPLE,

  return i;
}

std::vector<ILRegister> AST::Symbol::GetRawRegisters() const
{
  return std::vector<ILRegister>(1,GetRegister());
}

void AST::Symbol::MakeInitializerCode(ILTranscriber* il, const TreeElementPtr& init )
{
  DIE("invalid symbol initialization")(ToString());
}

bool AST::Symbol::HasSemantic() const
{
  for(ModifierSet::const_iterator i = mModifiers.begin(); i!=mModifiers.end(); ++i)
    if(i->Type()==Modifier::MOD_SEMANTIC)
      return true;
  return false;
}

// see if user wanted a specific register assigment
// only supported for s and c registers for dx9
int
AST::Symbol::RequestedRegister() const
{
  for(ModifierSet::const_iterator i = mModifiers.begin();
      i!=mModifiers.end(); ++i)
    if(i->Type()==Modifier::MOD_REGISTER) {
      return i->Number();
    }
  return -1;;
}

void 
AST::Symbol::TransposeMoveTo( ILTranscriber* il, const SymbolPtr& that )
{
  int rows = that->Type()->NumRows();
  int cols = that->Type()->NumCols();
  for(int r=0; r<rows; ++r) for(int c=0; c<cols; ++c)
  {
    ILRegister dst = that->GetMatrixElement(c,r);
    ILRegister src = GetMatrixElement(r,c);
    il->Move(dst,src);
  }
}

int
AST::Symbol::NumSemanticOrUniformRegisters() const
{
  return (HasSemantic() | IsFlaggedUniform()) ? SizeInRegisters() : 0;
}

void AST::Symbol::ConvertHook( ILTranscriber* il, SymbolPtr& sym, const DataTypePtr& dstType, OpType::OpType op /*= OpType::INVALID */ )
{
  if(sym->Type() != dstType)
  {
    STATIC_CONSTSTRING(tmpStr, "tmp"); 
    static ConstStringPtr xyzw[] = { ConstString::Lookup("x"), ConstString::Lookup("y"), ConstString::Lookup("z"), ConstString::Lookup("w") };

    
    if(sym->Type()->IsDoubleType() && dstType->IsBasicType() )
    {// DOUBLE ==> FLOAT (==> XXX)?

      //first convert DOUBLE ==> FLOAT per channel
      ILRegister tmp = il->AllocTempRegister();
      tmp.SetNumSlots(dstType->GetRegSlots());
      tmp.SetDT(RegDT::Float);

      for(int i=0; i< dstType->GetRegSlots(); ++i)
      {
        SymbolPtr part = sym->GetSubpart(xyzw[i]);
        ILRegister tmpPart = tmp;
        char swiz[4] = {tmp.SwizzleBytes()[i] , ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED};
        tmpPart.SetSwizzleBytes(swiz);
        il->DToF(tmpPart,part->GetRegister());
      }

      //now convert FLOAT ==> XXXX
      tmp = ILTranslator(il).TmpConvertInput( dstType->GetRegDT() , tmp);
      
      //now package it all up
      sym = new BasicSymbol( dstType, tmpStr, 0 );
      
      sym->AsBasicSymbol().SetRegister( tmp );
    }
    else if(sym->Type()->IsBasicType() && dstType->IsDoubleType() )
    {// (XXX ==>)? FLOAT ==> DOUBLE

      ILTranslator tl(il);

      //first convert to FLOAT
      ILRegister tmp = tl.TmpConvertInput(RegDT::Float, sym->GetRegister());

      if(tmp.GetNumSlots()==1) tmp.WidenSwizzleTo( dstType->GetRegSlots() );

      //allocate double
      sym = Symbol::CreateSymbol( dstType, tmpStr, 0);
      sym->AllocateRegisters(il);

      // convert FLOAT ==> DOUBLE per channel
      for(int i=0; i< dstType->GetRegSlots(); ++i)
      {
        SymbolPtr part = sym->GetSubpart(xyzw[i]);
        ILRegister tmpPart = tmp;
        char swiz[4] = {tmp.SwizzleBytes()[i] , ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED};
        tmpPart.SetSwizzleBytes(swiz);
        il->FToD(part->GetRegister(),tmpPart);
      }
    }
    else if( sym->Type()->IsDoubleType() && dstType->IsDoubleType() && OpType::DotProduct != op )
    { //need to play some games with number of channels
      
      SymbolPtr reslotted = Symbol::CreateSymbol( dstType, tmpStr, 0 );

      // re-arrange elements to create desired type
      for(int i=0; i< dstType->GetRegSlots(); ++i)
      {
          reslotted->GetSubpart( xyzw[i] )->AsBasicSymbol().SetRegister(
              sym->GetSubpart( xyzw[i % sym->Type()->GetRegSlots()] )->GetRegister() );
      }

      sym = reslotted;
    }
    else if(sym->Type()->IsMatrixType() && dstType->IsMatrixType())
    { //need to resize a matrix
      TreeElementPtr converter = new TypeCaster(dstType, new DelayedLookup(sym));
      converter->PreAllocateRegisters(converter, il);
      converter->PostAllocateRegisters(converter, il);
      converter->PreExportToIL(converter, il);
      converter->PostExportToIL(converter, il);
      sym = converter->GetResultSymbol();
      DEBUG_CHECK(sym.IsValid());
    }
  }
}

void AST::Symbol::SetSubType( const DataTypePtr& st )
{
  DIE("subtype not supported here")(ToString());
}

void 
AST::SymbolTable::AllocateRegisters( ILTranscriber* il )
{
  for(Iterator i = Begin(); i!=End(); ++i)
  {
    i->second->AllocateRegisters(il);
  }
}

AST::SymbolTable AST::SymbolTable::Clone() const
{
  SymbolTable tb;
  for(Iterator i = Begin(); i!=End(); ++i)
    tb.Insert(i->first,i->second->Clone());
  return tb;
}

AST::SymbolPtr 
AST::SymbolTable::InsertSymbol( const DataTypePtr& type , const ConstStringPtr& name , const TreeElementPtr& value )
{
  return InsertSymbol(Symbol::CreateSymbol(type,name,value));
}
AST::SymbolPtr 
AST::SymbolTable::InsertSymbol( const SymbolPtr& sym )
{
  Insert(sym->Name(),sym);
  return sym;
}

std::string AST::SymbolTable::ToString() const
{
  if(Size()==0) return "";
  std::ostringstream os;
  for(ConstIterator i = Begin(); i!=End(); ++i)
    os << ' ' << i->second->ToString();
  return os.str().c_str() + 1;
}

void 
AST::SymbolTable::AssertNoInitializers() const
{
  for(ConstIterator i = Begin(); i!=End(); ++i)
    i->second->AssertNoInitializers();
}

void AST::SymbolTable::MarkExplicitlyDefined()
{
  for(ConstIterator i = Begin(); i!=End(); ++i)
    i->second->MarkExplicitlyDefined();
}

int AST::SymbolTable::NumSemanticOrUniformRegisters() const
{
  int sum=0;
  for(ConstIterator i = Begin(); i!=End(); ++i)
    sum+=i->second->NumSemanticOrUniformRegisters();
  return sum;
}

int AST::SymbolTable::NumRegisters() const
{
  int sum=0;
  for(ConstIterator i = Begin(); i!=End(); ++i)
    sum+=i->second->SizeInRegisters();
  return sum;
}
// void AST::SymbolTable::ExtractInitializers( const TreeElementPtr& e )
// {
//   for(ConstIterator i = Begin(); i!=End(); ++i)
//     i->second->ExtractInitializers(e);
// }
void 
AST::BasicSymbol::AllocateRegisters( ILTranscriber* il )
{
  if(mRegister) return;
  if(Type()==DataType::TheFunctionDataType()) return;

  mRegister = il->AllocTempRegister();
  mRegister.SetDT(Type()->GetRegDT());
  mRegister.SetNumSlots(Type()->GetRegSlots());

  handleModifiers(il);



  //if(il->CurrentContext()==ILTranscriber::CTX_MAIN_ARGS)
  //  ASSERT_WARNING(mHasSemantic)(ToString()).Text("arguments to main must have a semantic");
}

ILRegister AST::BasicSymbol::GetRegister() const
{
  ASSERT_ERROR(mRegister);
  return mRegister;
}

const AST::SymbolPtr& AST::BasicSymbol::GetSubpart( const ConstStringPtr& member_name )
{
  static SymbolPtr invalid;
  DIE("Expected a object of type 'struct'")(Name())(member_name);
  return invalid;
}

AST::BasicSymbol::BasicSymbol( const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value ) 
: Symbol(type, name, value)
{}

AST::SymbolPtr AST::BasicSymbol::Clone() const
{
  ASSERT_WARNING(! mRegister)(Name()).Text("Expected mRegister to be unset at clone time");
  return new BasicSymbol(*this);
}

std::string AST::BasicSymbol::ToString() const
{
    return Symbol::ToString() + "[" + mRegister.ToString() + "]";
}

int AST::BasicSymbol::HandleBinaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dstsym , const SymbolPtr& _right , RegDT::RegDT forceType )
{
  //convert doubles correctly
  SymbolPtr self = this;
  SymbolPtr rightsym = _right;
  ConvertHook(il, self,     dstsym->Type(), op);
  ConvertHook(il, rightsym, dstsym->Type(), op);
  //a change was made, need to restart
  if( self != this )
  {
      return self->HandleBinaryCommand( il, op, dstsym, rightsym, forceType );
  }

  ILTranslator tl(il);

  ASSERT_ERROR(mRegister)(ToString());
  ASSERT_ERROR(rightsym->AsBasicSymbol().mRegister)(rightsym->ToString());
  ASSERT_ERROR(dstsym->AsBasicSymbol().mRegister)(dstsym->ToString());

  ILRegister left = this->mRegister;
  ILRegister right = rightsym->AsBasicSymbol().mRegister;
  ILRegister& dst = dstsym->AsBasicSymbol().mRegister;

  //Allow scalar to operate with vector
  if(left.GetNumSlots() == 1)  left.WidenSwizzleTo(right.GetNumSlots());
  if(right.GetNumSlots() == 1) right.WidenSwizzleTo(left.GetNumSlots());

  if(p_strict)
  {
    ASSERT_ERROR(left.GetNumSlots() == right.GetNumSlots())
      (ToString())(rightsym->ToString())(dstsym->ToString())
      .Text("Invalid operation with mismatched number of slots");
  }
  else 
  {
    int targetSlotsCnt = dst.GetNumSlots();

    if(op == OpType::DotProduct)//DP is special
      targetSlotsCnt = Minimum(left.GetNumSlots(),right.GetNumSlots());

    //if one of the inputs is short on slots, or is a fancy swizzle, we should
    // reslot it just to be safe
    left  = tl.TmpReslotInput(targetSlotsCnt, left);
    right = tl.TmpReslotInput(targetSlotsCnt, right);
  }

  //Handle implicit datatype conversions
  if(forceType!=RegDT::Invalid)
  { //we most force both args to given type
    if(left.GetDT()!=forceType)
      left = tl.TmpConvertInput(forceType,left);
    if(right.GetDT()!=forceType)
      right = tl.TmpConvertInput(forceType,right);
  }
  else if(left.GetDT()!=right.GetDT())
  { //Promote one of the types automatically
    // shifts are special uint >> int gives uint 
	  if(( op == OpType::RShift) || (op == OpType::LShift) ) {
      right = tl.TmpConvertInput(left.GetDT(),right);      
	  } else {
      if(left.GetDT()<right.GetDT())
        left = tl.TmpConvertInput(right.GetDT(),left);
      else
        right = tl.TmpConvertInput(left.GetDT(),right);
    }
  }

  // now run the command
  tl.BinaryCommand(op,dst,left,right);

  return 0;
}

int AST::BasicSymbol::HandleUnaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dstsym , RegDT::RegDT forceType )
{
  //convert doubles correctly
  SymbolPtr self = this;
  ConvertHook(il, self, dstsym->Type(), op);
  //a change was made, need to restart
  if(self != this) return self->HandleUnaryCommand(il,op,dstsym,forceType);

  ILTranslator tl(il);
  
  ASSERT_ERROR(mRegister);
  ASSERT_ERROR(dstsym->AsBasicSymbol().mRegister);
  
  ILRegister src = this->mRegister;
  ILRegister& dst = dstsym->AsBasicSymbol().mRegister;
  
  if(src.GetNumSlots() == 1) src.WidenSwizzleTo(dst.GetNumSlots());


  if(p_strict)
  {
    ASSERT_ERROR(src.GetNumSlots() == dst.GetNumSlots())/*(src.GetNumSlots())(dst.GetNumSlots())*/
      (ToString())(dstsym->ToString())
      .Text("Invalid operation with mismatched number of slots");
  }
  else
  {
    src = tl.TmpReslotInput(dst.GetNumSlots(), src);
  }

  //special case for assign
  if(op == OpType::Assign && forceType==RegDT::Invalid)
  {
    forceType = dstsym->AsBasicSymbol().Type()->GetRegDT();
  }

  //Handle implicit datatype conversions
  if(forceType!=RegDT::Invalid)
  { //we most force both arg to given type
    if(src.GetDT()!=forceType)
      src = tl.TmpConvertInput(forceType,src);
  }

  //now run the command
  tl.UnaryCommand(op,dst,src);

  return 0;
}


AST::SymbolPtr AST::BasicSymbol::CloneArrayIndex( ILRegister& rg ) const
{
  SymbolPtr copy = Clone();
  rg.SetDT(Type()->GetRegDT());
  rg.SetNumSlots(Type()->GetRegSlots());
  copy->AsBasicSymbol().SetRegister(rg);
  rg.IncrementOffset(SizeInRegisters());
  return copy;
}

ILRegister AST::BasicSymbol::GetMatrixElement( int row, int col ) const
{
  if(row!=0 && col==0) Swap(row,col); //support sloppy addressing of vectors
  ASSERT_ERROR(row==0 && col < mRegister.GetNumSlots())(row)(col)(mRegister.GetNumSlots())
    .Text("out of bounds matrix access");
  ILRegister tmp = mRegister;
  char swiz[] = { tmp.SwizzleBytes()[col], ILRegister::SLOTNOTUSED
    , ILRegister::SLOTNOTUSED, ILRegister::SLOTNOTUSED};
  tmp.SetSwizzleBytes(swiz);
  return tmp;
}

void AST::BasicSymbol::MakeInitializerCode( ILTranscriber* il, const TreeElementPtr& init )
{
  if(init->IsInitializer())
  {
    TreeElementPtr tmp = new TypeCaster(Type(),init->GetChildren());
    tmp->PostAllocateRegisters(tmp,il);
    tmp->PostAllocateRegisters(tmp,il);
    tmp->PostExportToIL(tmp,il);
    tmp->PostExportToIL(tmp,il);
    tmp->GetResultSymbol()->HandleUnaryCommand(il, OpType::Assign, this);
  }
  else
  {
    init->GetResultSymbol()->HandleUnaryCommand(il, OpType::Assign, this);
  }
}

void AST::BasicSymbol::DynamicSwizzleReadValue( ILTranscriber* il )
{
  ILRegister reg  = mDynSwizSource;
  ILRegister _reg  = reg;
  char swiz[4] = {ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED};
  ILRegister idx  = mDynSwizIndex;

  ILCOMMENT(il) << "dynamic swizzle read:";

  for(int i=0; i<_reg.GetNumSlots(); ++i)
  {
    il->IfIntEqual(idx, il->LookupLiteral(i, RegDT::Int, 1));
    swiz[0] = _reg.SwizzleBytes()[i];
    reg.SetSwizzleBytes(swiz);
    ILTranslator(il).UnaryCommand(OpType::Assign,mRegister,reg);
    il->EndIf();
  }
}

void AST::BasicSymbol::DynamicSwizzleWriteValue( ILTranscriber* il, ILRegister src )
{
  ILRegister reg  = mDynSwizSource;
  ILRegister _reg  = reg;
  char swiz[4] = {ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED,ILRegister::SLOTNOTUSED};
  ILRegister idx  = mDynSwizIndex;

  ILCOMMENT(il) << "dynamic swizzle write:";

  for(int i=0; i<_reg.GetNumSlots(); ++i)
  {
    il->IfIntEqual(idx, il->LookupLiteral(i, RegDT::Int, 1));
    swiz[0] = _reg.SwizzleBytes()[i];
    reg.SetSwizzleBytes(swiz);
    ILTranslator(il).UnaryCommand(OpType::Assign,reg,src);
    il->EndIf();
  }    

  ILTranslator(il).UnaryCommand(OpType::Assign,mRegister,src);
}

void AST::BasicSymbol::SetDynamicSwizzle( const ILRegister& reg, const ILRegister& idx )
{
  mDynSwizSource=reg;
  mDynSwizIndex=idx;
}
void 
AST::MultipartSymbol::AllocateRegisters( ILTranscriber* il )
{
  if(il->CurrentContext()==ILTranscriber::CTX_MAIN_ARGS 
    && Type()->IsMatrixType() 
    && IsFlaggedColMajor() 
    && HasSemantic())
  {
    //TODO: special case where we must transpose matrix
  }

  mMembers.AllocateRegisters(il);
}

AST::MultipartSymbol::MultipartSymbol( const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value ) 
: Symbol(type, name, value)
, mMembers(type->GetParts().Clone())
{}

AST::MultipartSymbol::MultipartSymbol( const MultipartSymbol& that ) : Symbol(that)
, mMembers(that.mMembers.Clone())
{}

AST::SymbolPtr 
AST::MultipartSymbol::Clone() const
{
  return new MultipartSymbol(*this);
}

const AST::SymbolPtr& AST::MultipartSymbol::GetSubpart( const ConstStringPtr& name )
{
  return mMembers.Lookup(name);
}

std::string AST::MultipartSymbol::ToString() const
{
    return Symbol::ToString()+"["+mMembers.ToString()+"]";;
}

int AST::MultipartSymbol::HandleBinaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , const SymbolPtr& _right , RegDT::RegDT forceType )
{
  //convert doubles correctly
  SymbolPtr self = this;
  SymbolPtr right = _right;
  ConvertHook(il, self, dst->Type(), op);
  ConvertHook(il, right, dst->Type(), op);
  //a change was made, need to restart
  if(self != this) return self->HandleBinaryCommand(il,op,dst,right,forceType);

  DEBUG_CHECK(dst!=0);
  DEBUG_CHECK(right!=0);
  ASSERT_ERROR(forceType == RegDT::Invalid)(forceType)(Type()->ToString())
    .Text("Don't know how to implicitly convert Type() to primitive type");
  ASSERT_ERROR(right->Type()==Type())(right->Type()->ToString())(Type()->ToString())
    .Text("Cant perform operation with mismatched types");

  int sum = 0;

  if ( OpType::DotProduct == op && dst->Type()->IsDoubleType() )
  {
      // special special case of dot product of doubles
      SymbolPtr interm = Symbol::CreateSymbol( self->Type(), 0, 0 );
      interm->AllocateRegisters( il );
      SymbolTable::ConstIterator ii = mMembers.Begin();
      for( ; ii != mMembers.End(); ++ii )
      {
          sum += ii->second->
              HandleBinaryCommand( il, OpType::Mult,
                                   interm->GetSubpart( ii->first ),
                                   right->GetSubpart( ii->first ),
                                   forceType );
      }
      SymbolTable::ConstIterator jj = mMembers.Begin();
      il->Move( dst->GetSubpart( jj->first )->GetRegister(), interm->GetSubpart( jj->first )->GetRegister() );
      ii = mMembers.Begin();
      ii++;
      for( ; ii != mMembers.End(); ++ii )
      {
          sum += dst->GetSubpart( jj->first )->
              HandleBinaryCommand( il, OpType::Plus,
                                   dst->GetSubpart( jj->first ),
                                   interm->GetSubpart( ii->first ),
                                   forceType );
      }
  }
  else
  {
      ASSERT_ERROR(dst->Type()==Type())(dst->Type()->ToString())(Type()->ToString())
          .Text("Cant perform operation with mismatched types");
      for( SymbolTable::ConstIterator ii = mMembers.Begin(); ii != mMembers.End(); ++ii )
      {
          sum += ii->second->
              HandleBinaryCommand( il, op,
                                   dst->GetSubpart( ii->first ),
                                   right->GetSubpart( ii->first ),
                                   forceType );
      }
  }
  return sum;
}

int AST::MultipartSymbol::HandleUnaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , RegDT::RegDT forceType )
{
  //convert doubles correctly
  SymbolPtr self = this;
  ConvertHook(il, self, dst->Type(), op);
  //a change was made, need to restart
  if(self != this) return self->HandleUnaryCommand(il,op,dst,forceType);

  DEBUG_CHECK(dst!=0);
  ASSERT_ERROR(forceType == RegDT::Invalid)(forceType)(Type()->ToString())
    .Text("Don't know how to implicitly convert Type() to primitive type");
  ASSERT_ERROR(dst->Type()==Type())(dst->Type()->ToString())(Type()->ToString())
    .Text("Cant perform operation with mismatched types");

  int sum=0;

  for(SymbolTable::ConstIterator i=mMembers.Begin(); i!=mMembers.End(); ++i)
    sum+=i->second->HandleUnaryCommand( il, op, dst->GetSubpart(i->first), forceType);

  return sum;
}

void AST::MultipartSymbol::AssertNoInitializers() const
{
  mMembers.AssertNoInitializers();
  Symbol::AssertNoInitializers();
}

void AST::MultipartSymbol::SetModifiers( const ModifierSet& m )
{
  ModifierSet mods = m;
  if(m.size()==0) return;
  Symbol::SetModifiers(mods);
  for(SymbolTable::ConstIterator i=mMembers.Begin(); i!=mMembers.End();)
  {
    i->second->SetModifiers(mods);
   
    ++i;
    if(i != mMembers.End()) //this is expensive, so we only do it if we will go around again
      mods = IncrementModifierSet(mods);
  }
}

void AST::MultipartSymbol::MarkExplicitlyDefined()
{
  mMembers.MarkExplicitlyDefined();
  Symbol::MarkExplicitlyDefined();
}

int AST::MultipartSymbol::SizeInRegisters() const
{
  int t = 0;
  for(SymbolTable::ConstIterator i=mMembers.Begin(); i!=mMembers.End(); ++i)
    t += i->second->SizeInRegisters();
  return t;
}

AST::SymbolPtr
AST::MultipartSymbol::CloneArrayIndex( ILRegister& rg ) const
{
    MultipartSymbol* copy = new MultipartSymbol( *this );
    copy->mMembers.Clear();
    for( SymbolTable::ConstIterator ii = mMembers.Begin(); ii != mMembers.End(); ++ii )
    {
        copy->mMembers.Insert( ii->first, ii->second->CloneArrayIndex( rg ) );
    }
    return copy;
}

AST::SymbolPtr
AST::MultipartSymbol::CloneArrayIndexGlobal( ILRegister& rg ) const
{
    MultipartSymbol* copy = new MultipartSymbol( *this );
    copy->mMembers.Clear();
    int jj = 0, kk = 0;
    for ( SymbolTable::ConstIterator ii = mMembers.Begin();
          ii != mMembers.End(); ++ii )
    {
        SymbolPtr member = ii->second->Clone();
        rg.SetDT( ii->second->Type()->GetRegDT());
        rg.SetNumSlots( ii->second->Type()->GetRegSlots() );
        rg.IncrementOffset( kk );
        if ( jj++ & 1 )
        {
            rg.SetSwizzleBytes( "__zw" );
            ++kk;
        }
        member->AsBasicSymbol().SetRegister( rg );
        copy->mMembers.Insert( ii->first, member );
    }
    return copy;
}

ILRegister AST::MultipartSymbol::GetMatrixElement( int row, int col ) const
{
  ASSERT_ERROR(Type()->IsMatrixType())(ToString())
    .Text("expected a matrix type here");
  ASSERT_ERROR(row<Type()->NumRows() && col<Type()->NumCols())(row)(col)(Type()->NumRows())(Type()->NumCols())
    .Text("out of bounds matrix access");
  SymbolTable::ConstIterator i = mMembers.Begin();
  for(; row>0; --row) ++i; //not great.. but this will run at most 3 times
  return i->second->GetMatrixElement(0, col);
}

ILRegister AST::MultipartSymbol::GetMatrixRow( int row ) const
{
  ASSERT_ERROR(Type()->IsMatrixType())(ToString())
    .Text("expected a matrix type here");
  ASSERT_ERROR(row<Type()->NumRows())(row)(Type()->NumRows())
    .Text("out of bounds matrix access");
  SymbolTable::ConstIterator i = mMembers.Begin();
  for(; row>0; --row) ++i; //not great.. but this will run at most 3 times
  return i->second->GetRegister();
}

std::vector<ILRegister> AST::MultipartSymbol::GetRawRegisters() const
{
  std::vector<ILRegister> rgs;
  for(SymbolTable::ConstIterator i = mMembers.Begin(); i!=mMembers.End(); ++i)
  {
    std::vector<ILRegister> tmp = i->second->GetRawRegisters();
    rgs.insert(rgs.end(), tmp.begin(), tmp.end());
  }
  return rgs;
}

void AST::MultipartSymbol::MakeInitializerCode( ILTranscriber* il, const TreeElementPtr& init )
{
  if(init->IsInitializer())
  {
    if(Type()->IsMatrixType())
    {
      int totalMatrixSize = Type()->NumCols()*Type()->NumRows();
      if(init->NumChildren() == totalMatrixSize)
      { //special case to allow:
        //  float3x3 a = { 1,2,3,
        //                 4,5,6,
        //                 7,8,9 }

        SymbolTable::ConstIterator s = mMembers.Begin(); 
        TreeElement::ConstIterator v = init->Begin();

        for(; s!=mMembers.End(); ++s)
        {
          TreeElementPtr tmp = new Initializer();
          for(int i=0; i<Type()->NumCols(); ++i) tmp->AddChild(*v++);
          s->second->MakeInitializerCode(il,tmp);
        }

        return;
      }
    }

    ASSERT_ERROR((int)mMembers.Size() == init->NumChildren())
      (mMembers.Size())(init->NumChildren())(Type()->ToString())
      .Text("initializer size does not match struct size");

    SymbolTable::ConstIterator s = mMembers.Begin(); 
    TreeElement::ConstIterator v = init->Begin();

    for(; s!=mMembers.End(), v!=init->End() ; ++s,++v)
      s->second->MakeInitializerCode(il,*v);
  }
  else
  {
    init->GetResultSymbol()->HandleUnaryCommand(il, OpType::Assign, this);
  }
}

int AST::MultipartSymbol::NumSemanticOrUniformRegisters() const
{
  return (HasSemantic() | IsFlaggedUniform()) ? SizeInRegisters() : mMembers.NumSemanticOrUniformRegisters();
}

// void AST::MultipartSymbol::ExtractInitializers( const TreeElementPtr& e )
// {
//   mMembers.ExtractInitializers(e);
//   Symbol::ExtractInitializers(e);
// }

AST::ArraySymbol::ArraySymbol(int count, const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value ) 
: Symbol(type,name,value)
, mFirstEntry(Symbol::CreateSymbol(type,name,value))
, mCount(count)
{
  mElementSize = mFirstEntry->SizeInRegisters();
}

void AST::ArraySymbol::SetModifiers( const ModifierSet& m )
{
  if(m.size()==0) return;
  mFirstEntry->SetModifiers(m);
  Symbol::SetModifiers(m);
}

void AST::ArraySymbol::MarkExplicitlyDefined()
{
  mFirstEntry->MarkExplicitlyDefined();
  Symbol::MarkExplicitlyDefined();
}

void AST::ArraySymbol::AssertNoInitializers() const
{
  mFirstEntry->AssertNoInitializers();
  Symbol::AssertNoInitializers();
}

AST::SymbolPtr AST::ArraySymbol::StaticArrayIndex( int i )
{
  ILRegister tmp = mRegister;
  tmp.SetOffset( tmp.GetOffset() + mElementSize*i);
  return mFirstEntry->CloneArrayIndex(tmp);
}

int AST::ArraySymbol::HandleUnaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , RegDT::RegDT forceType )
{
  int sum = 0;
  if ( mFirstEntry->Type()->IsResourceType() ) {
    // for resource, only need the first entry, rest will be expanded based on the first one
    // when they are used.
    mFirstEntry->AsResourceSymbol().HandleUnaryCommand(il, op, dst->AsArraySymbol().mFirstEntry, forceType );
  }
  else {
    for(int i=0; i<mCount; ++i)
      sum+= StaticArrayIndex(i) -> HandleUnaryCommand(il,op, dst->AsArraySymbol().StaticArrayIndex(i), forceType);
  }
  return sum;
}

int AST::ArraySymbol::HandleBinaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , const SymbolPtr& right , RegDT::RegDT forceType )
{
  int sum = 0;
  for(int i=0; i<mCount; ++i)
    sum+= StaticArrayIndex(i) -> HandleBinaryCommand(il,op, dst->AsArraySymbol().StaticArrayIndex(i), right->AsArraySymbol().StaticArrayIndex(i), forceType);
  return sum;
}

AST::SymbolPtr AST::ArraySymbol::Clone() const
{
  STATIC_CONSTSTRING(tmpStr,"<tmp>");
  SymbolPtr tmp = new ArraySymbol(mCount, Type(), tmpStr, 0);
  tmp->SetModifiers(mModifiers);
  return tmp;
}

std::string AST::ArraySymbol::ToString() const
{
  if ( mFirstEntry->Type()->IsResourceType() ) {
    return "array["+::ToString(mCount)+", s["+ mRegister.GetBase().ToString()+".x]]("+mFirstEntry->ToString()+")";
  }
  else {
    return "array["+::ToString(mCount)+","+mRegister.ToString()+"]("+mFirstEntry->ToString()+")";
  }
}

const AST::SymbolPtr& AST::ArraySymbol::GetSubpart( const ConstStringPtr& name )
{
  DIE("Cant extract member parts from an array")(name);
  static SymbolPtr invld;
  return invld;
}

ILRegister AST::ArraySymbol::GetRegister() const
{
  DIE("Array type unexpected in this location");
  return -1;
}

void AST::ArraySymbol::AllocateRegisters( ILTranscriber* il )
{
  if(mRegister) return;
  
  if ( mFirstEntry->Type()->IsResourceType() ) {
    mRegister = il->AllocTempRegister();
    mFirstEntry->AsResourceSymbol().AllocateRegisters( il );
    if ( mFirstEntry->AsResourceSymbol().IsDynamicResource() ) {
      mRegister.SetBase( mFirstEntry->AsResourceSymbol().GetDynamicResourceId() );
    }
    else {
      mRegister.SetBase( il->LookupLiteral( mFirstEntry->AsResourceSymbol().ResourceId(), 
                                            RegDT::UInt, 
                                            1 ) );
    }
    if ( !mFirstEntry->AsResourceSymbol().IsSamplerState() && 
         !mFirstEntry->AsResourceSymbol().IsDynamicResource() ) {
      for ( int k = 1; k < mCount; k++ ) {
        int id = il->AllocResource( mFirstEntry->AsResourceSymbol().Name(), 
                                    mFirstEntry->AsResourceSymbol().Type()->ResourceUsageType(),
                                    -1 );
      }
    }
  }
  else {
    mRegister = il->AllocArrayStructure(SizeInRegisters());
    handleModifiers(il);
  }
}

int AST::ArraySymbol::SizeInRegisters() const
{
  return mCount*mElementSize;
}

AST::SymbolPtr AST::ArraySymbol::CloneArrayIndex( ILRegister& rg ) const
{
    ArraySymbol* copy = new ArraySymbol(*this);
    copy->mRegister = rg;
    rg.IncrementOffset(SizeInRegisters());
    return copy;
}

AST::SymbolPtr AST::ArraySymbol::DoArrayIndexAlloc(ILTranscriber* il, ILRegister& base, const SymbolPtr& idx)
{
  if ( mFirstEntry->Type()->IsDoubleType()
       && ILSpec::IL_REGTYPE_GLOBAL == mRegister.Type() )
  {
      // This is a silly corner case, has to do with how we represent doubleX
      // and how global data is represented.  Normally doubleX is allocate a
      // register for each component of doubleX.  However global data is packed
      // 2 components into each float4 element of g[].  Hence we need to divide
      // the number of registers by two and take the ceiling of the result.
      mElementSize = ( mElementSize + 1 ) / 2;
  }
  mElementSizeLiteral = new NumericLiteral<int, RegDT::UInt>( mElementSize );
  mElementSizeLiteral->PreAllocateRegisters(mElementSizeLiteral, il);
  mElementSizeLiteral->PostAllocateRegisters(mElementSizeLiteral, il);
  base = il->AllocTempRegister();
  base.SetNumSlots(1);
  base.SetDT(RegDT::UInt);
  ILRegister rg = mRegister;
  rg.SetBase(base);
  SymbolPtr copy;
  if ( mFirstEntry->Type()->IsDoubleType()
       && ILSpec::IL_REGTYPE_GLOBAL == mRegister.Type() )
  {
      copy = mFirstEntry->AsMultipartSymbol().CloneArrayIndexGlobal( rg );
  }
  else
  {
      copy = mFirstEntry->CloneArrayIndex( rg );
  }
  if ( copy->Type()->IsResourceType() )
  {
      copy->AsResourceSymbol().SetDynamicResourceId( base );
  }
  return copy;
}


void AST::ArraySymbol::DoArrayIndexIL(ILTranscriber* il, const ILRegister& base, const SymbolPtr& idx)
{
  mElementSizeLiteral->PreExportToIL(mElementSizeLiteral, il);
  mElementSizeLiteral->PostExportToIL(mElementSizeLiteral, il);
  ILRegister idxr = ILTranslator(il).TmpConvertInput(RegDT::Int,idx->AsBasicSymbol().GetRegister());
  ILRegister elementSize = mElementSizeLiteral->GetResultSymbol()->AsBasicSymbol().GetRegister();
  il->UMul(base, elementSize, idxr);
  il->IAdd(base, base, mRegister.GetBase());
}

std::vector<ILRegister> AST::ArraySymbol::GetRawRegisters() const
{
  //first get an "example" entry by creating a temporary instance of the first
  ILRegister tmp = mRegister;
  SymbolPtr entry = mFirstEntry->CloneArrayIndex(tmp);
  std::vector<ILRegister> rgs = entry->GetRawRegisters();
  DEBUG_CHECK((int)rgs.size() == mElementSize)(rgs.size())(mElementSize)(entry->ToString());

  //now fill in the rest of the table by making copies of the first
  int numregs = SizeInRegisters();
  for(int i=mElementSize; i<numregs; ++i)
  {
    ILRegister tmp = rgs[i-mElementSize]; //grab element from X[n-1] entry
   
    if(!p_packarrays && tmp.GetNumSlots()<4)
      tmp.SetNumSlots(4);

    tmp.IncrementOffset(mElementSize);
    rgs.push_back(tmp);
  }
  DEBUG_CHECK((int)rgs.size()==numregs)(rgs.size())(numregs)(entry->ToString());
  return rgs;
}

void AST::ArraySymbol::MakeInitializerCode( ILTranscriber* il,  const TreeElementPtr& init )
{
  ASSERT_ERROR(init->NumChildren()==mCount)(mCount)(init->NumChildren())
    .Text("initializer size does not match array");
  ASSERT_ERROR(init->IsInitializer())(ToString()).Text("expected initializer to be of the form '{...}'");

  ILRegister rg = mRegister;
  for(int i=0; i<mCount; ++i)
  {
    mFirstEntry->CloneArrayIndex(rg)->MakeInitializerCode(il,init->GetChildren()[i]);
  }
}

int AST::ArraySymbol::NumSemanticOrUniformRegisters() const
{
  return (HasSemantic() | IsFlaggedUniform()) 
          ? SizeInRegisters() 
          : (mFirstEntry->NumSemanticOrUniformRegisters() * mCount);
}

AST::ResourceSymbol::ResourceSymbol( const DataTypePtr& type, const ConstStringPtr& name , const TreeElementPtr& value) 
: Symbol(type, name, value), mResourceId(-1), mIsDynamicResource(false)
{}

void AST::ResourceSymbol::AllocateRegisters( ILTranscriber* il )
{
  if(mResourceId < 0)
  {
    if( il->CurrentContext()==ILTranscriber::CTX_GLOBAL
     || il->CurrentContext()==ILTranscriber::CTX_MAIN_ARGS)
    {
      if(IsSamplerState())
      {
        mResourceId = il->AllocSampler(Name());
        if(il->GetShaderDescription().IsValid())
        {
          ShaderDescription::SamplerDesc desc;
          _fillVariableDesc(desc.Variable, *this);
          desc.SamplerNumber = mResourceId;
          il->GetShaderDescription()->Samplers.push_back(desc);
        }
      } else {
        // in dx9, samplers show up here is a resouce
        // if the application wants a speicfic sampler id
        // then this has a mModifer with an mType of register attached
        // of no such modifier, assign the next avaliable one
        int requested_reg_id = this->AsResourceSymbol().RequestedRegister();

        mResourceId = il->AllocResource(Name(), 
                                        Type()->ResourceUsageType(),
                                        requested_reg_id);
        
        if(il->GetShaderDescription().IsValid()) {
          ShaderDescription::TextureDesc desc;
          _fillVariableDesc(desc.Variable, *this);
          if(mSubType.IsValid()) 
            mSubType->FillDescription(desc.ElementType);
          else
          {
            static DataTypePtr f4 = DataType::LookupSimpleType(RegDT::Float, 4);
            f4->FillDescription(desc.ElementType);
          }
          desc.ResourceNumber = mResourceId;
          il->GetShaderDescription()->Textures.push_back(desc);
        }
      }

      mDynamicResourceId = il->LookupLiteral(mResourceId, RegDT::Int, 1);
    }
    else
    {//need a dynamic resource -- allocate a register for this
      mIsDynamicResource = true;
      mDynamicResourceId = il->AllocTempRegister();
      mDynamicResourceId.SetDT(RegDT::Int);
      mDynamicResourceId.SetNumSlots(1);
      mResourceId = 0;
      ILCOMMENT(il) << "virtual resource allocated: " << mDynamicResourceId.ToString();
    }
  }
}

AST::SymbolPtr AST::ResourceSymbol::Clone() const
{
  return new ResourceSymbol(*this);
}

AST::ResourceSymbol& AST::ResourceSymbol::AsResourceSymbol()
{
  return *this;
}

int AST::ResourceSymbol::ResourceId() const
{
  return mResourceId;
}

ILRegister AST::ResourceSymbol::GetRegister() const
{
  DIE("unexpected object of type resource")(ToString()); return -1;
}

int AST::ResourceSymbol::SizeInRegisters() const
{
  return 1;
}

AST::SymbolPtr AST::ResourceSymbol::CloneArrayIndex( ILRegister& rg ) const
{
  SymbolPtr copy = Clone();
  copy->AsResourceSymbol().mResourceId = -1;
  copy->AsResourceSymbol().mIsDynamicResource = false;
  rg.IncrementOffset(SizeInRegisters());
  return copy;
}

const AST::SymbolPtr& AST::ResourceSymbol::GetSubpart( const ConstStringPtr& name )
{
  DIE("unexpected object of type resource")(ToString()); static SymbolPtr nll; return nll;
}

int AST::ResourceSymbol::HandleBinaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , const SymbolPtr& right , RegDT::RegDT forceType /*= RegDT::Invalid */ )
{
  DIE("unexpected object of type resource")(ToString());
  return 1;
}

int AST::ResourceSymbol::HandleUnaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , RegDT::RegDT forceType /*= RegDT::Invalid */ )
{
  //dynamic resources can be assigned
  if(op == OpType::Assign)
  {
    il->Move(dst->AsResourceSymbol().GetDynamicResourceId(), GetDynamicResourceId());
    return 0;
  }
  else
  {
    DIE("unexpected object of type resource")(ToString());
  }
  return 1;
}

bool AST::ResourceSymbol::IsSamplerState() const
{
  return Type()==ResourceDataType::TheSamplerStateType();
}

void AST::ResourceSymbol::SetSubType( const DataTypePtr& st )
{
  mSubType = st;
}


void AST::FunctionSymbol::AllocateRegisters( ILTranscriber* il )
{

}

int AST::FunctionSymbol::SizeInRegisters() const
{
  return 0;
}

ILRegister AST::FunctionSymbol::GetRegister() const
{
  DIE("function unexpected here")(Name()); 
  return ILRegister();
}

const AST::SymbolPtr& AST::FunctionSymbol::GetSubpart( const ConstStringPtr& name )
{
  DIE("function unexpected here")(Name()); 
  static SymbolPtr nl; 
  return nl;
}

int AST::FunctionSymbol::HandleBinaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , const SymbolPtr& right , RegDT::RegDT forceType /*= RegDT::Invalid */ )
{
  DIE("function unexpected here")(Name()); 
  return 0;
}

int AST::FunctionSymbol::HandleUnaryCommand( ILTranscriber* il , OpType::OpType op , const SymbolPtr& dst , RegDT::RegDT forceType /*= RegDT::Invalid */ )
{
  DIE("function unexpected here")(Name()); 
  return 0;
}

AST::SymbolPtr AST::FunctionSymbol::CloneArrayIndex( ILRegister& rg ) const
{
  DIE("function unexpected here")(Name()); return 0;
}

AST::SymbolPtr AST::FunctionSymbol::Clone() const
{
  return new FunctionSymbol(*this);
}

AST::FunctionSymbol::FunctionSymbol( const ConstStringPtr& name ) 
  : Symbol(DataType::TheFunctionDataType(), name, 0)
{}

void AST::FunctionSymbol::AddOverload( const FunctionDeclarationPtr& func )
{
  mOverloads.push_back(func);
}

AST::FunctionSymbol& AST::FunctionSymbol::AsFunctionSymbol()
{
  return *this;
}

AST::FunctionDeclarationPtr AST::FunctionSymbol::ResolveOverloads( ) const
{
  static ArgList empty;
  return ResolveOverloads(empty);
}

AST::FunctionDeclarationPtr AST::FunctionSymbol::ResolveOverloads( const ArgList& args ) const
{
  DEBUG_CHECK(mOverloads.size()>0)(Name());
  
  //if there is only one option... return that option
  if(mOverloads.size()==1) return mOverloads[0];

  FunctionDeclarationList options = mOverloads;
  int numArgs = args.NumChildren();

  //first narrow down options by number of args
  for(int i=0; i<(int)options.size(); ++i)
  {
    if(numArgs != options[i]->NumArgs())
    { 
      //O(1) removal from a vector requires a swap with last element and a resize
      options[i] = options[options.size()-1];
      options.resize(options.size()-1);
      --i;
    }
  }

  //are we done yet?
  if(options.size()==1) return options[0];

  ASSERT_ERROR(options.size()>0)(mOverloads.size())(Name())(numArgs)
    .Text("No overloaded version of function has matching number of arguments");

  for(int i=0; i<(int)options.size(); ++i)
  {
    if(!args.IsMatchWith(*options[i]))
    {
      //O(1) removal from a vector requires a swap with last element and a resize
      options[i] = options[options.size()-1];
      options.resize(options.size()-1);
      --i;
    }
  }
 
  //are we done yet?
  if(options.size()==1) return options[0];

  ASSERT_ERROR(options.size()==1)(mOverloads.size())(Name())(numArgs)(options.size())
    .Text("Failed to determine which function overload was intended");

  return mOverloads[0];
}

