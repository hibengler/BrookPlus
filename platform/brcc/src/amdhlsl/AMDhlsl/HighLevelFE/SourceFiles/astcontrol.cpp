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

#include "astcontrol.hpp"
#include "astdelayedlookup.hpp"
#include "astbuiltinfunctions.hpp"
#include "astliterals.hpp"
#include "astoperators.hpp"

AST::LoopStatement::BreakIfFalse::BreakIfFalse( const TreeElementPtr& cond )
{
  AddChild(cond);
}

int AST::LoopStatement::BreakIfFalse::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  ASSERT_ERROR(mChildElements.size()==1)(mChildElements.size());
  il->BreakIfNot((*Begin())->ScratchBooleanValue(il));
  return 0;
}

AST::TreeElementPtr AST::LoopStatement::MakeWhile( const TreeElementPtr& cond, const TreeElementPtr& body )
{
  TreeElementPtr lp = new LoopStatement();
  lp->AddChild(new BreakIfFalse(cond));
  lp->AddChild(body);
  return lp;
}

AST::TreeElementPtr AST::LoopStatement::MakeDo( const TreeElementPtr& body, const TreeElementPtr& cond )
{
  TreeElementPtr lp = new LoopStatement();
  lp->AddChild(body);
  lp->AddChild(new BreakIfFalse(cond));
  return lp;  
}

AST::TreeElementPtr
AST::LoopStatement::MakeFor( const TreeElementPtr& init,
                             const TreeElementPtr& cond,
                             const TreeElementPtr& inc,
                             const TreeElementPtr& body )
{
  // yeah... lets just draw a picture
  //
  //       cond
  //         |                         
  //      BrkIfFls  body  inc  
  //           \   /       /
  //    init   Loop ------/
  //      \    /
  //      outer

  TreeElementPtr outer = new StatementList();
  TreeElementPtr loop = new LoopStatement();
  loop->AddChild(new BreakIfFalse(cond));
  loop->AddChild(body);
  loop->AddChild(inc);
  outer->AddChild(init);
  outer->AddChild(loop);
  return outer;
}

AST::IfStatement::IfCondition::IfCondition( const TreeElementPtr& cond )
{
  AddChild(cond);
}

int AST::IfStatement::IfCondition::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  ASSERT_ERROR(mChildElements.size()==1)(mChildElements.size());
  il->If((*Begin())->ScratchBooleanValue(il));
  return 0;
}

AST::IfStatement::IfStatement( const TreeElementPtr& cond 
                             , const TreeElementPtr& thenbody 
                             , const TreeElementPtr& elsebody /*= 0 */
                             , bool needReturnValue /*= false*/ )
  :mNeedReturnValue(needReturnValue)
{
  AddChild(new IfCondition(cond));
  AddChild(thenbody);
  if(elsebody!=0)
  {
    AddChild(new ElseStatement());
    AddChild(elsebody);
  }
}

int AST::IfStatement::PreAllocateRegisters( TreeElementPtr& /*self*/, ILTranscriber*& il )
{
  if(mNeedReturnValue)
  {
    ASSERT_ERROR(mChildElements.size() == 4)(mChildElements.size());
    TreeElementPtr& thenbody = mChildElements[1];
    TreeElementPtr& elsebody = mChildElements[3];

    { //error checking
      DataTypePtr thentype = thenbody->EffectiveDataType();
      DataTypePtr elsetype = elsebody->EffectiveDataType();
      ASSERT_ERROR(thenbody!=0);
      ASSERT_ERROR(elsebody!=0);
      ASSERT_ERROR(!(thentype->IsMultipart()||elsetype->IsMultipart()) || thentype==elsetype)
        (thentype->ToString())(elsetype->ToString())
        .Text("Types of 'b' and 'c' in 'a ? b : c' must match");
    }

    //create a tmp value for return
    STATIC_CONSTSTRING(tmpStr,"<tmp>");
    SymbolPtr rv = Symbol::CreateSymbol(thenbody->EffectiveDataType(), tmpStr, 0);
    SetResultSymbol(rv);
    rv->AllocateRegisters(il);
    //replace each {body} with "tmp = {body}"
    thenbody = new AST::BinaryOps::Assign(rv, thenbody);
    elsebody = new AST::BinaryOps::Assign(rv, elsebody);
  }
  return 0;
}

const AST::DataTypePtr& AST::IfStatement::EffectiveDataType()
{
  ASSERT_ERROR(mChildElements.size() == 4)(mChildElements.size());
  TreeElementPtr& thenbody = mChildElements[1];
  TreeElementPtr& elsebody = mChildElements[3];
  return DataType::CombineTypes(thenbody->EffectiveDataType(), elsebody->EffectiveDataType());
}

int AST::FunctionCall::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  il->SetCurrentContext(ILTranscriber::CTX_FUNC_ARGS);
  ASSERT_ERROR(mChildElements.size()==2)(mChildElements.size());
  const ArgList& args = mChildElements[1]->AsArgList();
  const FunctionDeclaration& func = LookupTargetFunction();
  int sum = 0;
  sum+=args.LoadFunctionArgs(il,func);
  il->CallFunction(func.FunctionId(), func.Name());
  sum+=args.UnloadFunctionArgs(il,func);

  SymbolPtr return_result = func.GetResultSymbol();

  if ( !return_result->IsArraySymbol() && 
       ( return_result->Type()->IsMultipart() || 
         return_result->Type()->IsBasicType() ) ) {
    STATIC_CONSTSTRING(tmpStr, "tmp"); 
    return_result = Symbol::CreateSymbol( return_result->Type(), tmpStr, 0);
    return_result->AllocateRegisters( il );
    std::vector<ILRegister> dst = return_result->GetRawRegisters();
    std::vector<ILRegister> src = func.GetResultSymbol()->GetRawRegisters();
    std::vector<ILRegister>::const_iterator dst_i;
    std::vector<ILRegister>::const_iterator src_i;
    for( src_i = src.begin(), dst_i = dst.begin(); 
         src_i != src.end() && dst_i != dst.end(); 
         ++src_i, ++dst_i ) {
       il->Move(*dst_i, *src_i);
    }
  }

  SetResultSymbol( return_result );

  il->SetCurrentContext(ILTranscriber::CTX_LOCAL);
  return sum;
}

int AST::FunctionCall::PreAllocateRegisters( TreeElementPtr& self, ILTranscriber*& il )
{
  //perhaps this is not a function call but a cast?
  if(mChildElements[0]->AsDelayedLookup().IsDataTypeSymbol())
  {
    ASSERT_ERROR(mChildElements.size() == 2)(mChildElements.size());
    DataTypePtr type = mChildElements[0]->AsDelayedLookup().DataTypeValue();
    //replace this with a typecaster
    self = new TypeCaster(type, mChildElements[1]->GetChildren());
    self->SetPosition(GetPosition());
  }
  return 0;
}

// FIXME!  This is completely inconsistent. This data is normally used
// in PostExportToIL stage, when the result of a function is copied
// into a bunch of temporaries.  HOWEVER, MemberAccess needs this
// information before PostExportToIL, at PostAllocateRegisters stage,
// when it queries the symbol of the result and replaces it with the
// same register with proper swizzling mask.  the compiler will still
// generate a bunch of IL moves for copying the results of a func call
// into temporaries, but they are superceeded by the MemberAccess copy
// instructions.

int
AST::FunctionCall::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
    if ( GetResultSymbol().IsValid() )
    {
        return 0;
    }
    const FunctionDeclaration& func = LookupTargetFunction();
    SymbolPtr return_result = func.GetResultSymbol();
    SetResultSymbol( return_result );
    mTargetFunction->AddReference();
    return 0;
}

int AST::FunctionCall::ResolveDataTypes( TreeElementPtr& self, DataTypeTable*& scope )
{
    // a function call could be either a call of a user defined function,
    // a builtin function, or a cast

    // mchildelements[0] is the function name,
    // mchildelements[1] is the argument string
    TreeElementPtr func = mChildElements[0];
    func->ResolveDataTypes(func, scope);

    //perhaps this is not a function call, but a builtin function?
    TreeElementPtr bival = LookupBuiltinFunction(func->AsDelayedLookup().Name());
    if ( bival.IsValid() )
    {
        TreeElementList args = mChildElements[1]->GetChildren();
        bival->SetChildren( args );
        //replace this with a builtin func
        self = bival;
        self->SetPosition( GetPosition() );
    } 
    //perhaps this is not a function call but a cast?
    else if ( func->AsDelayedLookup().IsDataTypeSymbol() )
    {
        ASSERT_ERROR(mChildElements.size() == 2)(mChildElements.size());
        DataTypePtr type = mChildElements[0]->AsDelayedLookup().DataTypeValue();
        //replace this with a typecaster
        self = new TypeCaster(type, mChildElements[1]->GetChildren());
        self->SetPosition(GetPosition());
    }
    return 0;
}

const AST::FunctionDeclaration& AST::FunctionCall::LookupTargetFunction()
{
  if(!mTargetFunction.IsValid()) 
  {
    const ArgList& args = mChildElements[1]->AsArgList();
    mTargetFunction = mChildElements[0]->GetResultSymbol()->AsFunctionSymbol().ResolveOverloads(args);
  }
  ASSERT_ERROR(mTargetFunction.IsValid())(mChildElements[0]->DebugStatus())
    .Text("unknown function");
  return *mTargetFunction;
}



// AST::Constantizer::Constantizer( const TreeElementPtr& dynamicExpr,const DataTypePtr& type )
//   : mType(type)
//   , mHaveAddedIfs(false)
// {
//   AddChild(dynamicExpr); 
//   mCaseValueLiterals.push_back(new NullTreeElement());
// }
// 
// void AST::Constantizer::AddCase( int constantValue , const TreeElementPtr& expression )
// {
//   AddChild(expression);
//   mCaseValueLiterals.push_back(new NumericLiteral<int, RegDT::Int>(constantValue));
//   DEBUG_CHECK(mCaseValueLiterals.size() ==
//           mChildElements.size())(mCaseValueLiterals.size())(mChildElements.size());
// }
// 
// const AST::DataTypePtr& AST::Constantizer::EffectiveDataType()
// {
//   return mType;
// }
// 
// int AST::Constantizer::PostAllocateRegisters( TreeElementPtr& /*self*/, ILTranscriber* il )
// {
//   //make sure our literals get allocated early
//   for(TreeElementList::iterator i=mCaseValueLiterals.begin()
//      ;i!=mCaseValueLiterals.end()
//      ;++i)
//   {
//      (*i)->PreAllocateRegisters(*i,il);
//      (*i)->PostAllocateRegisters(*i,il);
//   }
//   SetResultSymbol(Symbol::CreateSymbol(EffectiveDataType(),0,0));
//   GetResultSymbol()->AllocateRegisters(il);
//   return 0;
// }
// 
// int AST::Constantizer::PreExportToIL( TreeElementPtr& /*self*/, ILTranscriber*& il )
// {
//   if(mHaveAddedIfs) 
//     return 0; 
//   else 
//     mHaveAddedIfs = true;
// 
//   DEBUG_CHECK(mChildElements.size()>=2);
//   TreeElementPtr dynamicVal = new DelayedLookup(mChildElements[0]->GetResultSymbol());
//   dynamicVal->PreAllocateRegisters(dynamicVal, il);
//   dynamicVal->PostAllocateRegisters(dynamicVal, il);
// 
//   //replace our child elements each with a IfStatement 
//   for(size_t i=1; i<mChildElements.size(); ++i)
//   {
//     TreeElementPtr resultSym = new DelayedLookup(GetResultSymbol());
//     resultSym->PreAllocateRegisters(resultSym, il);
//     resultSym->PostAllocateRegisters(resultSym, il);
//     TreeElementPtr cond   = MakeBinaryOp<BinaryOps::CmpEQ>( dynamicVal,  mCaseValueLiterals[i] );
//     TreeElementPtr action = MakeBinaryOp<BinaryOps::Assign>( resultSym , mChildElements[i]);
//     cond->PreAllocateRegisters(cond, il);
//     cond->PostAllocateRegisters(cond, il);
//     action->PreAllocateRegisters(action, il);
//     action->PostAllocateRegisters(action, il);
//     mChildElements[i] = new IfStatement(cond, action);
//   }
//   return 0;
// }
//
// void AST::Constantizer::AssignmentDestRepositionHook(  ILTranscriber* il, TreeElementPtr& assignment )
// {
//   ASSERT_ERROR(!mHaveAddedIfs);
//   TreeElementPtr assignmentValue = assignment->GetChildren()[1];
//   TreeElementPtr assignmentValueRslt = new DelayedLookup(assignmentValue->GetResultSymbol());
//   assignmentValueRslt->PreAllocateRegisters(assignmentValueRslt,il);
//   assignmentValueRslt->PostAllocateRegisters(assignmentValueRslt,il);
//   assignment = MakeBinaryOp<BinaryOps::PrecomputeLeftReturnRight>( assignmentValue, this );
//   assignment->PreAllocateRegisters(assignment,il);
//   assignment->PostAllocateRegisters(assignment,il);
//   
//   
//   for(size_t i=1; i<mChildElements.size(); ++i)
//   {
//     mChildElements[i] = MakeBinaryOp<BinaryOps::Assign>(mChildElements[i], assignmentValueRslt);
//     mChildElements[i]->PreAllocateRegisters( mChildElements[i] ,il);
//     mChildElements[i]->PostAllocateRegisters( mChildElements[i] ,il);
//   }
// }

const AST::ArgList& AST::ArgList::AsArgList() const
{
  return *this;
}

int AST::ArgList::LoadFunctionArgs( ILTranscriber* il, const FunctionDeclaration& func ) const
{
  ASSERT_ERROR(func.NumArgs() == (int)mChildElements.size())
    (func.NumArgs())(mChildElements.size())
    .Text("Invalid function call");

  int sum = 0;
  for(int i=0; i<func.NumArgs(); ++i)
  {
    if(func.GetArg(i)->IsFlaggedInput())
    {
      sum += mChildElements[i]->GetResultSymbol()->HandleUnaryCommand( 
        il,
        OpType::Assign, 
        func.GetArg(i)
        );
    }
    else
    {
      //zero out the memory, to avoid uninitialized
      func.GetArg(i)->UninitializedRegisterFillDefaultValue(il);
    }
  }

  return sum;
}

int AST::ArgList::UnloadFunctionArgs( ILTranscriber* il, const FunctionDeclaration& func ) const
{
  ASSERT_ERROR(func.NumArgs() == (int)mChildElements.size())
    (func.NumArgs())(mChildElements.size())
    .Text("Invalid function call");

  int sum = 0;
  for(int i=0; i<func.NumArgs(); ++i)
    if(func.GetArg(i)->IsFlaggedOutput())
      sum += func.GetArg(i)->HandleUnaryCommand( 
      il,
      OpType::Assign, 
      mChildElements[i]->GetResultSymbol()
      );
  return sum;
}

bool AST::ArgList::IsMatchWith( const FunctionDeclaration& func ) const
{
  if(func.NumArgs() != static_cast<int>(mChildElements.size())) return false;
  for(int i=0; i<func.NumArgs(); ++i)
  {
    //same base type?
    if(func.GetArgType(i) != mChildElements[i]->EffectiveDataType())
      return false;

    // some tree_elems like func_call don't have  symbols. (the next clause is only for those with symbols)     
    if ( mChildElements[i]->GetResultSymbol().IsValid() ) {

      //both either array or not array?
      if ( (func.GetArgArrayCount(i)>0) != mChildElements[i]->GetResultSymbol()->IsArraySymbol() )
        return false;
      
      //same number of array elements?
      if ( mChildElements[i]->GetResultSymbol()->IsArraySymbol() &&
           mChildElements[i]->GetResultSymbol()->AsArraySymbol().ElementCount() != func.GetArgArrayCount(i) )
        return false;

      // add more special case checks here ...
    }

  }
  return true;
}
