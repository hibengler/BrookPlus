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

#include "astdeclaration.hpp"
#include "astuserdatatype.hpp"
#include "astoperators.hpp"
#include "astdelayedlookup.hpp"
#include "astbuiltinfunctions.hpp"
#include "parameter.hpp"

PARAMETER(p_mainfunc, "main");

int AST::FunctionDeclaration::NEXT_FUNCTION_ID=1;

AST::FunctionDeclaration::FunctionDeclaration( const ConstStringPtr& name 
                                             , const ConstStringPtr& return_type 
                                             , int numArgs ) 
  : Declaration(name,return_type)
  , mFunctionId(NEXT_FUNCTION_ID++)
  , mNumArgs(numArgs)
  , mNumRefs(0)
  , mMaxVertexCount(-1)
  , mLdsSize(0)
  , mLdsRel(0)
  , mLdsAbs(0)
{
  mNumT[0] = mNumT[1] = mNumT[2] = 0;
  ASSERT_ERROR(LookupBuiltinFunction(name)==0)(name)
    .Text("Name collides with intrinsic function");
}

///
/// Looks up mType
/*virtual*/ int 
AST::Declaration::ResolveDataTypes(TreeElementPtr& /*self*/, DataTypeTable*& scope)
{
  ASSERT_ERROR(scope!=NULL);
  if(mType.IsValid()) return 0;

  //do the lookup
  mType = scope->Lookup(mTypeStr);
  ASSERT_WARNING(mType != NULL)(mTypeStr).Text("Unknown DataType");

  //return 1 if type is unknown
  return mType==NULL ;
}

AST::Declaration::Declaration( const ConstStringPtr& name, const ConstStringPtr& type ) : mName(name)
, mTypeStr(type)
{
  if(mTypeStr==0)
  {
    STATIC_CONSTSTRING(invdStr,"<invalid>");
    mTypeStr = invdStr;
  }
}
///
///Create an entry for this variable in symbol table
/*virtual*/ int 
AST::VariableDeclaration::ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope)
{
  ASSERT_ERROR(scope!=0);
  ASSERT_ERROR(Type()!=0);
  SPAM("Declaring symbol")(Name());
  TreeElementPtr initializer = 0;
  if(mChildElements.size()>0)
  {
    ASSERT_ERROR(mChildElements.size()==1)(mChildElements.size())
      .Text("variable declaration may only have one initializer");
    initializer = mChildElements[0];
  }
  if(mArrayCount.IsValid())
  {
    DEBUG_CHECK(mArrayCount!=Single());

    int count;
    if(mArrayCount==UnspecifiedSize())
      count = 4096;
    else
      count = mArrayCount->SimplifyToInt();

    SetResultSymbol(scope->InsertSymbol(new ArraySymbol(count, Type(), Name(), initializer)));
  }
  else
    SetResultSymbol(scope->InsertSymbol(Type(), Name(), initializer));

  if(mSubType.IsValid())
  {
    DataTypePtr st = DataType::TheBuiltinDataTypes()->Lookup(mSubType);
    ASSERT_ERROR(st.IsValid() && st->IsBasicType())(mSubType)
      .Text("Invalid subtype for Texture<...>");
    GetResultSymbol()->SetSubType(st);
  }
  GetResultSymbol()->SetModifiers(mModifiers);
  GetResultSymbol()->MarkExplicitlyDefined();

  // if this is a resouce (dx9 sampler) see if has a register request
  // if it does prevent hlsl from allocating the same sampler number
  DataTypePtr my_type = Type();
  if(my_type.IsValid() && my_type->IsResourceType() && p_dx9){
	  int r = GetResultSymbol()->RequestedRegister();
	  if(r>=0){
		  // mark that the register is in use
		  AST::Root::TheCurrentTreeRoot()->RecordForcedSampler(r);
	  }
  }
  return 0;
}

int AST::VariableDeclaration::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  GetResultSymbol()->AllocateRegisters(il);

  //need to handle initializing the globals a little earlier
  if(il->CurrentContext()==ILTranscriber::CTX_GLOBAL && mChildElements.size()>0)
  {
    //need to run the initialization code early because this is a global variable
    WalkTree<ILTranscriber, &TreeElement::PreExportToIL, &TreeElement::PostExportToIL> ( self, il );
    
    //now make sure this code isn't run again later
    self=0;
  }

  return 0;
}

const AST::TreeElementPtr& AST::VariableDeclaration::UnspecifiedSize()
{
  static TreeElementPtr inst = new NullTreeElement();//here we just have an untyped node as it is special cased
  return inst;
}

const AST::TreeElementPtr& AST::VariableDeclaration::Single()
{
  static TreeElementPtr inst = new NullTreeElement();//here we just have an untyped node as it is special cased
  return inst;
}

int AST::VariableDeclaration::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  if(mChildElements.size()>0)
  {
    ILCOMMENT(il) << "initializer for " << Name();
    GetResultSymbol()->MakeInitializerCode(il, mChildElements[0]);
  }
  return 0;
}
///
/// Creates a DataType entry for this struct
/*virtual*/ int 
AST::StructDeclaration::ResolveDataTypes(TreeElementPtr& self, DataTypeTable*& scope)
{
  ASSERT_ERROR(scope!=0);
  SPAM("defining user struct")(Name());

  int undefinedCount = 0;
  
  //Resolve the datatypes of every struct member
  for(Iterator i=Begin(); i!=End(); ++i)
    undefinedCount += (*i)->ResolveDataTypes(*i, scope);


  if(undefinedCount>0) return undefinedCount;

  SymbolTable structMembers;
  SymbolTable* structMembersPtr = &structMembers;

  //Fill structMembers SymbolTable
  for(Iterator i=Begin(); i!=End(); ++i)
    undefinedCount += (*i)->ResolveSymbols(*i, structMembersPtr);

  //We don't expect child to define scopes...
  DEBUG_CHECK(structMembersPtr == &structMembers);
  structMembers.AssertNoInitializers();

  //Now create the datatype
  scope->Insert( Name() , new StructDataType(Name(), structMembers) );

  //this node in this tree can now be removed (children should not be walked)
  mChildElements.clear();
  self = 0;

  return undefinedCount;
}

STATIC_CONSTSTRING(structStr,"struct");
AST::StructDeclaration::StructDeclaration( const ConstStringPtr& name ) 
  : Declaration(name, structStr)
{}


///
/// Creates a symbol for this function
/*virtual*/ int 
AST::FunctionDeclaration::ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope)
{
  ASSERT_ERROR(scope!=0);
  SPAM("Declaring Function")(Name());

  SymbolPtr sym;
  if(scope->HasKeyLocally(Name()))
  {
    sym = scope->Lookup(Name());
    ASSERT_ERROR(!IsMain())(Name())
      .Text("entry point can not be overloaded");
  }
  else
  {
    sym = new FunctionSymbol(Name());
    scope->Insert(Name(), sym);
    DEBUG_CHECK(scope->HasKeyLocally(Name()));
  }

  sym->AsFunctionSymbol().AddOverload(this);

  //now add level to scope
  mFunctionArgScope.SetParent(scope);
  scope = &mFunctionArgScope;
  
  //Add symbol to store return value
  STATIC_CONSTSTRING(returnStr,"return");
  static Modifier outMod(ConstString::Lookup("out"), Modifier::MOD_KEYWORD);
  SetResultSymbol(mFunctionArgScope.InsertSymbol(Type(), returnStr, 0));
  ModifierSet mods = mModifiers;
  mods.insert(outMod);
  GetResultSymbol()->SetModifiers(mods);
  mFunctionArgScope.MarkExplicitlyDefined();
  return 0;
}

const AST::DataTypePtr& 
AST::FunctionDeclaration::EffectiveDataType()
{
  return DataType::TheFunctionDataType();
}

int AST::FunctionDeclaration::PreExportToIL( TreeElementPtr& /*self*/, ILTranscriber*& il )
{
    if ( !IsReferenced() && !IsMain() )
    {
        mSkipChildren = true;
        return 0;
    }
 mFunctionArgScope.AssertNoInitializers();
 ASSERT_ERROR(il!=0);
 ASSERT_ERROR(GetResultSymbol() != 0);
 ILCOMMENT(il) << "Function: " << Name();
 ILCOMMENT(il) << "Args: " << mFunctionArgScope.ToString();
 il->BeginFunction(mFunctionId);
 il->SetCurrentContext(ILTranscriber::CTX_LOCAL);
 return 0;
}

int AST::FunctionDeclaration::PostExportToIL( TreeElementPtr& /*self*/, ILTranscriber* il )
{
    if ( !IsReferenced() && mSkipChildren )
    {
        mSkipChildren = false;
        return 0;
    }
  il->EndFunction();
  il->SetCurrentContext(ILTranscriber::CTX_GLOBAL);
  return 0;
}

int AST::FunctionDeclaration::PostAllocateRegisters( TreeElementPtr& /*self*/, ILTranscriber* il )
{
  il->SetCurrentContext(ILTranscriber::CTX_GLOBAL);
  return 0;
}
int AST::FunctionDeclaration::PreAllocateRegisters( TreeElementPtr& self, ILTranscriber*& il )
{
  il->SetCurrentContext(IsMain() ? ILTranscriber::CTX_MAIN_ARGS : ILTranscriber::CTX_FUNC_ARGS);
  mFunctionArgScope.AllocateRegisters(il);
  il->SetCurrentContext(ILTranscriber::CTX_LOCAL);

  if(il->GetShaderDescription().IsValid())
  {
    ShaderDescription::FunctionDesc fd;
    fd.Name = Name();
    fd.FunctionNumber = mFunctionId;
    fd.IsEntryPoint = IsMain();
    fd.NumRegisterArgs = mFunctionArgScope.NumRegisters();
    fd.NumRegisterArgsSemanticOrUniform = mFunctionArgScope.NumSemanticOrUniformRegisters();
    il->GetShaderDescription()->Functions.push_back(fd);
  }

  return 0;
}

bool AST::FunctionDeclaration::IsMain() const
{
  static ConstStringPtr mainName = ConstString::Lookup(p_mainfunc);
  //handle changing parameters
  if(*mainName != p_mainfunc ) mainName = ConstString::Lookup(p_mainfunc);
  return Name()==mainName;
}

static const ConstStringPtr strSamplerState = ConstString::Lookup("<SamplerState>");
AST::SamplerDeclaration::SamplerDeclaration(const ConstStringPtr& name)
    : Declaration ( name, strSamplerState )
{
  SetType(ResourceDataType::TheSamplerStateType());
}
int AST::ConsBuffScope::PreAllocateRegisters( TreeElementPtr& self, ILTranscriber*& il )
{
  ASSERT_ERROR(il->CurrentContext()==ILTranscriber::CTX_GLOBAL)
    .Text("cbuffer {...} construct must appear in global scope");
  il->SetCurrentConstBuff(mName);
  return 0;
}
int AST::ConsBuffScope::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  return 0;
}


int AST::PersistentAllocScope::PreAllocateRegisters( TreeElementPtr& self, ILTranscriber*& il )
{
  ASSERT_ERROR(il->CurrentContext()==ILTranscriber::CTX_GLOBAL)
    .Text("persistent {...} construct must appear in global scope");
  il->SetSpecialAllocMode(ILTranscriber::ALLOC_PERSISTENT);
  return 0;
}

int AST::PersistentAllocScope::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  il->SetSpecialAllocMode(ILTranscriber::ALLOC_NORMAL);
  return 0;
}

int AST::GlobalAllocScope::PreAllocateRegisters( TreeElementPtr& self, ILTranscriber*& il )
{
  ASSERT_ERROR(il->CurrentContext()==ILTranscriber::CTX_GLOBAL)
    .Text("global {...} construct must appear in global scope");
  il->SetSpecialAllocMode(ILTranscriber::ALLOC_GLOBAL);
  return 0;
}

int AST::GlobalAllocScope::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  il->SetSpecialAllocMode(ILTranscriber::ALLOC_NORMAL);
  return 0;
}



