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

#include "astmodifier.hpp"
#include "convert.hpp"
#include "parameter.hpp"
#include "astelement.hpp"

// command line parameters used in this file
PARAMETER(p_case_sensitive_modifiers,"strict");
PARAMETER(p_dx9, "dx9" );


AST::Modifier::Modifier( const ConstStringPtr& name, ModifierType type ) 
: ConstStringPtr(name)
, mType(type)
, mNumber(0)
{
  std::string modName = *name;

  if(!p_case_sensitive_modifiers)
    modName = ::ToLower(modName);

  if(mType==MOD_SEMANTIC)
  {
    for(int i=1; Between('0',modName[modName.length()-1],'9'); i*=10)
    {
      mNumber += i * (modName[modName.length()-1] - '0'); 
      modName.resize(modName.length()-1);
    }
  }

  if(modName != *name) {
    ConstStringPtr::operator= ( ConstString::Lookup(modName) );
  }
  

  // in dx9 mode s and c registers assingments can be forced
  // with a :register(name) declaration
  if(mType == MOD_REGISTER){
    // hlsl seems not to complain in dx10, instead ms hlsl just ignores
    // the declaration
	  if(p_dx9){
      for(int i=1; Between('0',modName[i],'9'); i++) {
        mNumber = 10 * mNumber + (modName[i] - '0'); 
      }
	  }
  }
}

static std::map<ConstStringPtr, ILSpec::ILImportUsage> _generateSemanticUsageTable()
{
  using namespace ILSpec;
  std::map<ConstStringPtr, ILSpec::ILImportUsage> table;
  
#define INSERT_SEMANTIC(key, val) \
  table[ ConstString::Lookup( p_case_sensitive_modifiers ? key : ToLower(key) ) ] = val;

  INSERT_SEMANTIC("SV_ClipDistance",            IL_IMPORTUSAGE_CLIPDISTANCE);
  INSERT_SEMANTIC("SV_CullDistance",            IL_IMPORTUSAGE_CULLDISTANCE);
//  INSERT_SEMANTIC("SV_Depth",                   IL_IMPORTUSAGE_GENERIC);
  INSERT_SEMANTIC("SV_InstanceID",              IL_IMPORTUSAGE_INSTANCEID);
  INSERT_SEMANTIC("SV_IsFrontFace",             IL_IMPORTUSAGE_ISFRONTFACE);
  INSERT_SEMANTIC("SV_PrimitiveID",             IL_IMPORTUSAGE_PRIMITIVEID);
  INSERT_SEMANTIC("SV_RenderTargetArrayIndex",  IL_IMPORTUSAGE_RENDERTARGET_ARRAY_INDEX);
//  INSERT_SEMANTIC("SV_Target",                  IL_IMPORTUSAGE_GENERIC);
  INSERT_SEMANTIC("SV_VertexID",                IL_IMPORTUSAGE_VERTEXID);
  INSERT_SEMANTIC("SV_ViewportArrayIndex",      IL_IMPORTUSAGE_VIEWPORT_ARRAY_INDEX);
//  INSERT_SEMANTIC("SV_Depth",                   IL_IMPORTUSAGE_GENERIC);
  INSERT_SEMANTIC("SV_Position",                IL_IMPORTUSAGE_POS);
  INSERT_SEMANTIC("SV_RelThreadId",             IL_IMPORTUSAGE_UNDEFINED); 
  INSERT_SEMANTIC("SV_AbsThreadId",             IL_IMPORTUSAGE_UNDEFINED); 
  INSERT_SEMANTIC("SV_ThreadGroupId",           IL_IMPORTUSAGE_UNDEFINED); 
//   INSERT_SEMANTIC("VVIEW",                      IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("BINORMAL",                   IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("BLENDINDICES",               IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("BLENDWEIGHT",                IL_IMPORTUSAGE_GENERIC);
  INSERT_SEMANTIC("COLOR",                      IL_IMPORTUSAGE_COLOR);
//  INSERT_SEMANTIC("NORMAL",                     IL_IMPORTUSAGE_GENERIC);
  INSERT_SEMANTIC("POSITION",                   IL_IMPORTUSAGE_POS);
  INSERT_SEMANTIC("POS",                        IL_IMPORTUSAGE_POS);
//   INSERT_SEMANTIC("POSITIONT",                  IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("PSIZE",                      IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("TANGENT",                    IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("TEXCOORD",                   IL_IMPORTUSAGE_GENERIC);
  INSERT_SEMANTIC("FOG",                        IL_IMPORTUSAGE_FOG);
//   INSERT_SEMANTIC("PSIZE",                      IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("TESSFACTOR",                 IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("TEXCOORD",                   IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("VFACE",                      IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("VPOS",                       IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("DEPTH",                      IL_IMPORTUSAGE_GENERIC);
//   INSERT_SEMANTIC("WORLDVIEWPROJECTION",        IL_IMPORTUSAGE_GENERIC);

  return table;
}

ILSpec::ILImportUsage AST::Modifier::LookupSemanticUsage() const
{
  DEBUG_CHECK(Type()==MOD_SEMANTIC);
  static std::map<ConstStringPtr, ILSpec::ILImportUsage> theTable = _generateSemanticUsageTable();
  std::map<ConstStringPtr, ILSpec::ILImportUsage>::const_iterator i = theTable.find(*this);
  if(i==theTable.end()) return ILSpec::IL_IMPORTUSAGE_GENERIC;
  return i->second;
}

void AST::Modifier::IncrementNumber( int offset /*= 1*/ )
{
  if(mType==MOD_SEMANTIC)
    mNumber += offset;
}

AST::ModifierSet AST::IncrementModifierSet(const ModifierSet& src)
{
  ModifierSet dst;
  for(ModifierSet::const_iterator i=src.begin(); i!=src.end(); ++i)
  {
    Modifier m = *i;
    m.IncrementNumber();
    dst.insert(m);
  }
  return dst;
}


// for input semantics
static std::map<ConstStringPtr, ILSpec::ILRegType> _generateInputSemanticRegTable()
{
  using namespace ILSpec;
  std::map<ConstStringPtr, ILSpec::ILRegType> table;
  
#define INSERT_SEMANTIC(key, val) \
  table[ ConstString::Lookup( p_case_sensitive_modifiers ? key : ToLower(key) ) ] = val;
  
  INSERT_SEMANTIC("SV_RelThreadId",	IL_REGTYPE_THREAD_ID); 
  INSERT_SEMANTIC("SV_AbsThreadId",	IL_REGTYPE_ABSOLUTE_THREAD_ID); 
  INSERT_SEMANTIC("SV_ThreadGroupId",	IL_REGTYPE_THREAD_GROUP_ID); // MS group = block 
  INSERT_SEMANTIC("SV_Position",	IL_REGTYPE_INPUT);
  INSERT_SEMANTIC("POSITION",		IL_REGTYPE_WINCOORD);
  // all others will use default IL_REGTYPE_INPUT
  return table;
}
ILSpec::ILRegType AST::Modifier::LookupInputSemanticRegister() const
{
  DEBUG_CHECK(Type()==MOD_SEMANTIC);
  static std::map<ConstStringPtr, ILSpec::ILRegType> regTable = _generateInputSemanticRegTable();
  std::map<ConstStringPtr, ILSpec::ILRegType>::const_iterator i = regTable.find(*this);
  if(i==regTable.end()) return ILSpec::IL_REGTYPE_INPUT; // default
  return i->second;
}

