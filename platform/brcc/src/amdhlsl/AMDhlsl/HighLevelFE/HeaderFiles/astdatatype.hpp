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

#ifndef AMD_ASTDATATYPE_HPP
#define AMD_ASTDATATYPE_HPP

#include "smartpointer.hpp"
#include "shaderdescription.hpp"
#include "assertlogging.hpp"
#include "hierarchicallookuptable.hpp"
#include "iltranslator.hpp"

namespace AST
{
class Symbol;
typedef SmartPtr<Symbol>      SymbolPtr;
class SymbolTable;

class DataType;
typedef SmartPtr<DataType>       DataTypePtr;
typedef HierarchicalLookupTable<ConstStringPtr, DataTypePtr> DataTypeTable;

/**
 * baseclass for data types (both user defined and builtin)                                                         
 */
class DataType : public RefCountedObj
{
public:

  static const DataTypePtr& TheFunctionDataType();

  ///
  /// Singleton instance of the table of built-in datatypes
  static const DataTypeTable* TheBuiltinDataTypes();

  ///
  /// Constructor
  DataType(const ConstStringPtr& name) : mName(name) {}

  static const AST::DataTypePtr& LookupSimpleType(RegDT::RegDT t, int slots = 1);
  static const AST::DataTypePtr& LookupMatrixType(RegDT::RegDT, int rows, int cols);
  static const AST::DataTypePtr& TheVoidType();

  const std::string& ToString() const;

  virtual bool IsMultipart() const;
  ///
  /// IsMultipart() must be true before this can be called
  virtual const SymbolTable& GetParts() const;

  const ConstStringPtr& Name() const;

  ///
  /// register type, not valid if IsMultipart()
  virtual RegDT::RegDT GetRegDT() const { DIE("Expected simple datatype here")(ToString()); return RegDT::Invalid; }

  void SetName(const ConstStringPtr& val) { mName = val; }

  ///
  /// register type, not valid if IsMultipart()
  virtual int GetRegSlots() const {  DIE("Expected simple datatype here")(ToString()); return 0; }

  virtual bool IsDoubleType() const { return false; }

  ///
  /// true if this is a ResourceDataType
  virtual bool IsResourceType() const { return false; }
  
  virtual ILSpec::ILPixTexUsage ResourceUsageType() const {  DIE("Expected resource datatype here")(ToString()); return ILSpec::IL_USAGE_PIXTEX_UNKNOWN; }
  

  virtual bool IsMatrixType() const { return false; }
  virtual bool IsBasicType() const { return false; }

  virtual int NumRows() const { return -1; } 
  virtual int NumCols() const { return -1; } 


  

  static const AST::DataTypePtr& CombineTypes(const DataTypePtr& leftType, const DataTypePtr& rightType);

  void FillDescription(ShaderDescription::DataTypeDesc& desc) const
  {
    desc.Name = Name();
    desc.IsUserDefinedType = !TheBuiltinDataTypes()->Lookup(Name()).IsValid();
    desc.ElementSizeInRegisters = 0;
    desc.NumArrayElements = -1;
  }


private:
  ConstStringPtr mName;
  
};


class ResourceDataType : public DataType
{
  ResourceDataType(ILSpec::ILPixTexUsage ut, const ConstStringPtr& name)
    : DataType(name), mUsageType(ut)
  {}
public:
  static DataTypePtr GetResourcePtrByType(ILSpec::ILPixTexUsage t);
  static DataTypePtr TheSamplerStateType()
  {
    return GetResourcePtrByType(ILSpec::IL_USAGE_PIXTEX_UNKNOWN);
  }

  bool IsResourceType() const { return true; }
  ILSpec::ILPixTexUsage ResourceUsageType() const { return mUsageType; }

private:
  ILSpec::ILPixTexUsage mUsageType;

};


}//namespace AST



#endif
