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

#ifndef AMD_ASTCONTROL_HPP
#define AMD_ASTCONTROL_HPP

#include "astelement.hpp"

#include "astdeclaration.hpp"

namespace AST
{

/**
* A collection of statements in the body of a if, while, function, etc
*/
class StatementList : public SymbolScope
{
  DCL_ASTELEMENT_METHODS(StatementList);
};

/**
 * A statement list wrapped in simple commands                                                   
 */
template< ILTranscriber::MemberFunction before
        , ILTranscriber::MemberFunction after = &ILTranscriber::Null
        , typename Base = TreeElement>
class WrappedBlock : public Base
{
  DCL_ASTELEMENT_METHODS(WrappedBlock);
  /// (Step 4)
  /// Exports tree to IL language (Called before recursive calls to children)
  /// Returns zero on success
  virtual int PreExportToIL(TreeElementPtr& self, ILTranscriber*& il)
  {
    (il->*before)();
    return Base::PreExportToIL(self,il);
  }

  /// (Step 4)
  /// Exports tree to IL language (Called after recursive calls to children)
  /// Returns zero on success
  virtual int PostExportToIL(TreeElementPtr& self, ILTranscriber* il)
  {
    (il->*after)();
    return Base::PostExportToIL(self,il);
  }
};

typedef WrappedBlock<&ILTranscriber::Break, &ILTranscriber::Null> BreakStatement;
typedef WrappedBlock<&ILTranscriber::Discard, &ILTranscriber::Null> DiscardStatement;
typedef WrappedBlock<&ILTranscriber::Continue, &ILTranscriber::Null> ContinueStatement;


/**
 * A list of expression statements                                                                    
 */
class ArgList : public TreeElement
{
  DCL_ASTELEMENT_METHODS(ArgList);
public:
  ArgList(){}
  ArgList(const TreeElementPtr& child)
  {
    AddChild(child);
  }

  virtual const ArgList& AsArgList() const;

  int LoadFunctionArgs(ILTranscriber* il, const FunctionDeclaration& func) const;

  int UnloadFunctionArgs(ILTranscriber* il, const FunctionDeclaration& func) const;

  bool IsMatchWith(const FunctionDeclaration& func) const;

  
};

/**
* Wraps a call to a given function   
* First child must be function symbol 
* Second child is ArgList
*/
class FunctionCall : public TreeElement
{
  DCL_ASTELEMENT_METHODS(FunctionCall);
public:

  virtual int ResolveDataTypes(TreeElementPtr& self, DataTypeTable*& scope);

  // Propagates return result symbol of function being called to FunctionCall node.
  virtual int PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il );
  /// (Step 3.5)
  /// Allocates registers to each VariableDeclaration
  virtual int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il);

  /// (Step 4)
  /// Exports tree to IL language (Called after recursive calls to children)
  /// Returns zero on success
  virtual int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);

  virtual const AST::DataTypePtr& EffectiveDataType()
  {
    return LookupTargetFunction().Type();
  }

protected:

  const FunctionDeclaration& LookupTargetFunction();
private:
  FunctionDeclarationPtr mTargetFunction;
};


/**
* Wraps a switch statment
* First child is arg to switch
* rest is body
*/
class SwitchStatement : public StatementList
{
  DCL_ASTELEMENT_METHODS(SwitchStatement);
  class StartSwitch : public TreeElement
  {
    DCL_ASTELEMENT_METHODS(StartSwitch);
    //not implemented yet...
  };
public:
};

/**
* if( {expr} ) {foo} else {bar}
* - First child is (IfCondition) {expr}
* - Second child is (StatementList) {foo}
* - Third child is [optional] (ElseStatement) {bar}
*/
class IfStatement : public WrappedBlock< &ILTranscriber::Null
                                       , &ILTranscriber::EndIf>
{
  DCL_ASTELEMENT_METHODS(IfStatement);
  //helper class
  class IfCondition : public TreeElement
  {
    DCL_ASTELEMENT_METHODS(IfCondition);
  public:
    IfCondition(const TreeElementPtr& cond);
    ///break if our one Child is false
    virtual int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);
  };
  //helper class
  typedef WrappedBlock<&ILTranscriber::Else, &ILTranscriber::Null> ElseStatement;
public:

  IfStatement( const TreeElementPtr& cond
             , const TreeElementPtr& thenbody
             , const TreeElementPtr& elsebody = 0
             , bool needReturnValue = false);
  
  /// Add a return register if needed
  virtual int PreAllocateRegisters(TreeElementPtr& /*self*/, ILTranscriber*& /*il*/);


  virtual const AST::DataTypePtr& EffectiveDataType();
private:
  bool mNeedReturnValue;
};

/**
* while( {expr} ) {foo}
* statement representation
* a list of statements wrapped in whileloop/endloop with a break somewhere in it
*/
class LoopStatement : public WrappedBlock< &ILTranscriber::WhileLoop
                                         , &ILTranscriber::EndLoop>
{
  DCL_ASTELEMENT_METHODS(LoopStatement);
  //helper class
  class BreakIfFalse : public TreeElement
  {
    DCL_ASTELEMENT_METHODS(BreakIfFalse);
  public:
    BreakIfFalse(const TreeElementPtr& cond);
    ///break if our one Child is false
    virtual int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);
  };

  //private constructor
  LoopStatement(){}
public:
  static TreeElementPtr MakeWhile( const TreeElementPtr& cond
                                 , const TreeElementPtr& body);
  
  static TreeElementPtr MakeDo( const TreeElementPtr& body
                              , const TreeElementPtr& cond);
  
  static TreeElementPtr MakeFor( const TreeElementPtr& init
                               , const TreeElementPtr& cond
                               , const TreeElementPtr& inc
                               , const TreeElementPtr& body);

};


/**
 * Constantizer is a hook for compiling pieces of code that require a value
 * to resolve to a constant value within a small enumeration.  This is used to 
 * allow compilation of texture fetches where the resource number is not known
 * and for dynamic swizzle operations.
 * It is implemented a switch over dynamic values
 *
 * First child is dynamic expression that we wish to transform to constant value
 * Rest of children are possible cases for dynamic value
 */
// class Constantizer : public TreeElement
// {
//   DCL_ASTELEMENT_METHODS(Constantizer);
// public:
//   Constantizer(const TreeElementPtr& dynamicExpr, const DataTypePtr& type);
//   void AddCase( int constantValue , const TreeElementPtr& expression );
// 
//   //Fill mResultsSymbol
//   int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);
//  
//   int PreExportToIL(TreeElementPtr& self, ILTranscriber*& il);
//   
//   const AST::DataTypePtr& EffectiveDataType();
// 
// 
//   //this is a dest of an assignment... we must fix this problem
//   //void AssignmentDestRepositionHook( ILTranscriber* il, TreeElementPtr& assignment );
// 
// private:
//   TreeElementList mCaseValueLiterals;
//   DataTypePtr mType;
//   bool  mHaveAddedIfs;
// };


}

#endif // AMD_ASTCONTROL_HPP
