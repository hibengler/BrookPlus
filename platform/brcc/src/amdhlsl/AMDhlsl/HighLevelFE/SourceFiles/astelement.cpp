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

#include "astelement.hpp"
#include "astdeclaration.hpp"
#include "astcontrol.hpp"
#include "parameter.hpp"

///
/// Print a ascii representation of this AST to cerr
void 
AST::TreeElement::DebugPrint(std::ostream& os, int level/* = 0*/)
{
  //this node
  if(IsPositionValid()) os << GetPosition() << '\t';
  else os << "????????:??\t";
  for(int i=0; i<level; ++i)
    os << "  ";
  os << DebugClassName() << " "
     << DebugStatus();
  if ( mResultSymbol.IsValid() )
  {
      os << " Result( " << mResultSymbol->ToString() << " )";
  }
  os << std::endl;

  //child nodes
  for(Iterator i=Begin(); i!=End(); ++i)
    (*i)->DebugPrint(os, level+1);
}

std::string AST::TreeElement::DebugStatus() const
{
  return "";
}


/// (Step 1)
/// Fills datatype tables and performs delayedlookups
/// Returns number of unknown types
/*virtual*/ int AST::TreeElement::ResolveDataTypes( TreeElementPtr& self, DataTypeTable*& scope )
{
  return 0;
}

/// (Step 2)
/// Fills symbol tables and performs delayedlookups
/// Returns number of unknown symbols
/*virtual*/ int AST::TreeElement::ResolveSymbols( TreeElementPtr& self, SymbolTable*& scope )
{
  return 0;
}

/// (Step 3)
/// Reformats tree replacing constructs that dont translate to IL easily
/// Returns number of simplifications performed
// virtual int 
// AST::TreeElement::PreSimplifyTree(TreeElementPtr& /*self*/, TreeElement*& parent)
// {
//   parent = this;
//   return 0;
// }

/// (Step 4)
/// Exports tree to IL language
/// Returns zero on sucess
/*virtual*/ int 
AST::TreeElement::PreExportToIL(TreeElementPtr& /*self*/, ILTranscriber*& /*il*/)
{
  return 0;
}

const AST::DataTypePtr& 
AST::TreeElement::EffectiveDataType()
{
  static DataTypePtr invalid; 
  return invalid;
}

int AST::TreeElement::PostExportToIL( TreeElementPtr& self, ILTranscriber* il )
{
  return 0;
}

int AST::TreeElement::PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il )
{
  return 0;
}

int 
AST::TreeElement::PreAllocateRegisters( TreeElementPtr& self, ILTranscriber*& il )
{
  return 0;
}

//int 
// AST::TreeElement::PostSimplifyTree( TreeElementPtr& self, TreeElement* parent )
// {
//   return 0;
// }

AST::FunctionDeclaration& 
AST::TreeElement::AsFunction()
{
  DIE("This is not a function...")(DebugClassName()); 
  return *(FunctionDeclaration*)0;
}

AST::VariableDeclaration& AST::TreeElement::AsVarDecl()
{
  DIE("This is not a variable declaration...")(DebugClassName()); 
  return *(VariableDeclaration*)0;
}

const AST::ArgList& 
AST::TreeElement::AsArgList() const
{
  DIE("This is not a arglist...")(DebugClassName()); 
  return *(const ArgList*)0;
}
AST::DelayedLookup& AST::TreeElement::AsDelayedLookup()
{
  DIE("This is not a DelayedLookup...")(DebugClassName()); 
  return *(DelayedLookup*)0;
}
AST::SamplerDeclaration& AST::TreeElement::AsSampler() 
{
  DIE("This is not a SamplerDeclaration...")(DebugClassName()); 
  return *(SamplerDeclaration*)0;
}

void AST::TreeElement::AddModifier( const Modifier& mod )
{
  mModifiers.insert(mod);
}



/// 
/// Adds a level to the SymbolTable stack and replaces scope with mSymbolTable
/*virtual*/ int 
AST::DataTypeScope::ResolveDataTypes(TreeElementPtr& /*self*/, DataTypeTable*& scope)
{
  mDataTypeTable.SetParent(scope);
  scope = &mDataTypeTable;
  return 0;
}

/// 
/// Adds a level to the DataTypeTable stack and replaces scope with mDataTypeTable
/*virtual*/ int 
AST::SymbolScope::ResolveSymbols(TreeElementPtr& /*self*/, SymbolTable*& scope)
{
  mSymbolTable.SetParent(scope);
  scope = &mSymbolTable;
  return 0;
}


int AST::SymbolScope::PreAllocateRegisters( TreeElementPtr& self, ILTranscriber*& il )
{
  mSymbolTable.AllocateRegisters(il);
  return 0;
}


