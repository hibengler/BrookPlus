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

#include "parservariant.hpp"

///
/// Null constructor
ParserVariant::ParserVariant(Type t /*= T_UNSET*/)
  : PositionAware(::GetPosition())
  , mType(t)
{}

///
/// String constructor
ParserVariant::ParserVariant(const char* str) 
: PositionAware(::GetPosition())
, mType(T_STRING)
, mStr(ConstString::Lookup(str))
{}
ParserVariant::ParserVariant(const std::string& str) 
: PositionAware(::GetPosition())
, mType(T_STRING)
, mStr(ConstString::Lookup(str))
{}
ParserVariant::ParserVariant(const ConstStringPtr& str) 
: PositionAware(::GetPosition())
, mType(T_STRING)
, mStr(str)
{}


///
/// Tree Element constructor (start with one element
ParserVariant::ParserVariant(const AST::TreeElementPtr& elmt)
  : PositionAware(::GetPosition())
  , mType(T_TREE_ELEMENTS)
  , mElements(1, elmt)
{
  if(!elmt->IsPositionValid())
    elmt->SetPosition(*this);
}

///
/// Tree Element constructor (start with one element
ParserVariant::ParserVariant(AST::TreeElement* elmt)
  : PositionAware(::GetPosition())
  , mType(T_TREE_ELEMENTS)
  , mElements(1, AST::TreeElementPtr(elmt))
{
  if(!elmt->IsPositionValid())
    elmt->SetPosition(*this);
}

ParserVariant::ParserVariant( const AST::Modifier& mod ) 
  : PositionAware(::GetPosition())
  , mType(T_MODIFIER)
  , mModifier(mod)
{}
///
/// Automatic conversion to string
ParserVariant::operator const std::string& () const 
{
  ASSERT_ERROR(mType == T_STRING)(mType).Text("Invalid conversion to string");
  return *mStr;
}

///
/// Automatic conversion to string
ParserVariant::operator const ConstStringPtr& () const 
{
  ASSERT_ERROR(mType == T_STRING)(mType).Text("Invalid conversion to string");
  return mStr;
}

///
/// Automatic conversion to List
ParserVariant::operator const AST::TreeElementList& () const 
{
  ASSERT_ERROR(mType == T_TREE_ELEMENTS)
    (mType)(*this).Text("Invalid conversion to ASTTreeElement");
  return mElements;
}

///
/// Automatic conversion to single Element
ParserVariant::operator const AST::TreeElementPtr& () const 
{
  ASSERT_ERROR(mType == T_TREE_ELEMENTS && mElements.size() == 1)
    (mType)(mElements.size())(mStr).Text("Invalid conversion to ASTTreeElement");
  return mElements[0];
}

///
/// Append an element to list type
void ParserVariant::Append(const AST::TreeElementPtr& elmt)
{
  ASSERT_ERROR(mType == T_TREE_ELEMENTS)
    (mType)(*this).Text("Cant Append() elements to an non-list variant type");
  mElements.push_back(elmt);
  
  if(!elmt->IsPositionValid())
    elmt->SetPosition(*this);
}


///
/// Generic combination operator, merges two lists
/// Only valid for T_TREE_ELEMENTS
///
/// @TODO 5/9/07 this can likely be optimized to mutate a -jason.ansel 
ParserVariant operator+ (const ParserVariant& _a, const ParserVariant& _b)
{
  //calls the conversion operators
  const AST::TreeElementList& a = _a;
  const AST::TreeElementList& b = _b;

  //empty list optimizations
  if(a.size() == 0) return _b;
  if(b.size() == 0) return _a;

  ParserVariant c(ParserVariant::T_TREE_ELEMENTS);

  //left side
  for(AST::TreeElementList::const_iterator i = a.begin(); i!=a.end(); ++i )
    c.Append(*i);

  //right side
  for(AST::TreeElementList::const_iterator i = b.begin(); i!=b.end(); ++i )
    c.Append(*i);

  return c;
}


///
/// Add a list of child nodes to this;
void ParserVariant::AddChildren(const ParserVariant& _childLst)
{
  //call conversion operators
  const AST::TreeElementPtr& self = *this;
  const AST::TreeElementList& childLst = _childLst;

  for(AST::TreeElementList::const_iterator i = childLst.begin(); i!=childLst.end(); ++i )
    self->AddChild(*i);
}

std::string ParserVariant::ToString() const
{
  switch(mType)
  {
  case T_UNSET: return "<unset>";
  case T_STRING: return "\"" + *mStr + "\"" ;
  case T_TREE_ELEMENTS: return "<element>";
  case T_MODIFIER: return "<modifier:"+*mModifier+">";
  default: return "<?ERR?>";
  }
}



