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

#ifndef AMD_ASTSYMBOL_HPP
#define AMD_ASTSYMBOL_HPP

#include <vector>
#include "assertlogging.hpp"
#include "conststring.hpp"
#include "iltranslator.hpp"
#include "shaderdescription.hpp"
#include "iltranscriber.hpp"
#include "smartpointer.hpp"
#include "hierarchicallookuptable.hpp"
#include "astmodifier.hpp"
#include <set>

namespace AST
{
class ResourceSymbol;
class ArraySymbol;
class FunctionSymbol;
class MultipartSymbol;
class TreeElement;
typedef SmartPtr<TreeElement> TreeElementPtr;
class FunctionDeclaration;
class ArgList;
typedef SmartPtr<FunctionDeclaration> FunctionDeclarationPtr;
typedef std::vector<FunctionDeclarationPtr> FunctionDeclarationList;
class Initializer;

class DataType;
typedef SmartPtr<DataType> DataTypePtr;

class Symbol;
typedef SmartPtr<Symbol>         SymbolPtr;
typedef std::vector<SymbolPtr>   SymbolPtrList;


/**
* represents user defined symbol                                                        
*/
class Symbol : public RefCountedObj
{
public:
  static SymbolPtr CreateSymbol( const DataTypePtr& type , const ConstStringPtr& name , const TreeElementPtr& value);

  ///
  /// Constructor
  Symbol(const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value);
  Symbol(const Symbol& that);

  const ConstStringPtr& Name() const;
  const ConstStringPtr& GetDisplayName() const { return mDisplayName.IsValid() ? mDisplayName : Name(); }
  const AST::DataTypePtr& Type() const;

  virtual std::string ToString() const;

  ///
  /// Return mRegister, allocate if needed
  virtual void AllocateRegisters(ILTranscriber* il) = 0;
  virtual void SetRegister(ILRegister val){};
  ///
  /// Only defined for BasicSymbols
  virtual ILRegister GetRegister() const = 0;
  virtual ILRegister* GetPtrRegister() const{ return (ILRegister*)0;} ;  

  ///
  /// Only defined for MultiPartSymbols
  virtual const SymbolPtr&  GetSubpart(const ConstStringPtr& name) = 0;

  virtual SymbolPtr Clone() const = 0;

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleBinaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , const SymbolPtr& right
    , RegDT::RegDT forceType = RegDT::Invalid ) = 0;

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleUnaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , RegDT::RegDT forceType = RegDT::Invalid ) = 0;


  virtual BasicSymbol& AsBasicSymbol() { DIE("Invalid cast")(ToString()); return *(BasicSymbol*)0; }
  virtual ArraySymbol& AsArraySymbol() { DIE("Invalid cast")(ToString()); return *(ArraySymbol*)0; }
  virtual ResourceSymbol& AsResourceSymbol() { DIE("Invalid cast")(ToString()); return *(ResourceSymbol*)0; }
  virtual FunctionSymbol& AsFunctionSymbol() { DIE("Invalid cast")(ToString()); return *(FunctionSymbol*)0; }
  virtual MultipartSymbol& AsMultipartSymbol() { DIE("Invalid cast")(ToString()); return *(MultipartSymbol*)0; }

  virtual bool IsArraySymbol() const {return false;}
  virtual bool IsMultipartSymbol() const {return false;}

  const AST::TreeElementPtr& GetInitialValue() const { return mInitialValue; }

  virtual void AssertNoInitializers() const;

  //virtual void ExtractInitializers(const TreeElementPtr& e);

  virtual void SetModifiers(const ModifierSet& m);

  virtual void MarkExplicitlyDefined(){mIsExplicitlyDefined=true;}


  virtual int SizeInRegisters() const = 0;

  bool IsFlaggedInput() const;
  bool IsFlaggedOutput() const;
  bool IsFlaggedStatic() const;
  bool IsFlaggedUniform() const;
  bool IsFlaggedColMajor() const;
  bool HasSemantic() const;
  // returns any user requested register assignment
  // or -1 if none
  int RequestedRegister() const;
  
  bool HasFlag(const Modifier& fl) const;

  virtual SymbolPtr CloneArrayIndex(ILRegister& rg) const = 0;

  virtual ILRegister GetMatrixElement(int row, int col) const;
  virtual ILRegister GetMatrixRow(int row) const;
  
  //callbacks for modifiers

  void GenerateModifierILIgnored(ILTranscriber* il, const Modifier& name){}

  void GenerateModifierILUnsupported(ILTranscriber* il, const Modifier& name);

  //assert that a given modifier only occurs in function arg context
  void GenerateModifierILOnlyInArgContext(ILTranscriber* il, const Modifier& name);

  //handle a semantic modifier
  void handleSemantic(ILTranscriber* il, const Modifier& name);

  void handleModifiers(ILTranscriber* il);

  ILSpec::ILInterpMode GetInterpOptions() const;


  virtual std::vector<ILRegister> GetRawRegisters() const;

  virtual void MakeInitializerCode(ILTranscriber* il, const TreeElementPtr& init);


  void TransposeMoveTo(ILTranscriber* il, const SymbolPtr& that);

  virtual void SetSubType(const DataTypePtr& st);

  void UninitializedRegisterFillDefaultValue(ILTranscriber* il) const
  {
// this could would zero out uninitialized data
//     std::vector<ILRegister> regs = GetRawRegisters();
//     for(std::vector<ILRegister>::const_iterator i=regs.begin()
//       ; i!=regs.end()
//       ; ++i)
//       il->Move(*i,il->ConstIntZero(RegDT::Int, i->GetNumSlots()));
  };

  virtual int NumSemanticOrUniformRegisters() const;


  void SetDisplayName(const ConstStringPtr& val) { mDisplayName = val; }


  static void ConvertHook(ILTranscriber* il, SymbolPtr& sym, const DataTypePtr& dstType, OpType::OpType op = OpType::INVALID );

  bool IsExplicitlyDefined() const { return mIsExplicitlyDefined; }
private:
  ConstStringPtr mName;
  ConstStringPtr mDisplayName;
  DataTypePtr    mType;
  TreeElementPtr mInitialValue;
protected:
  ModifierSet    mModifiers;
  bool           mIsExplicitlyDefined; 
};

class SymbolTable : public HierarchicalLookupTable<ConstStringPtr, SymbolPtr> 
{
public:
  void AllocateRegisters( ILTranscriber* il );

  SymbolTable Clone() const;

  SymbolPtr InsertSymbol( const DataTypePtr& type , const ConstStringPtr& name , const TreeElementPtr& value);
  SymbolPtr InsertSymbol( const SymbolPtr& sym);

  std::string ToString() const;

  void AssertNoInitializers() const;
  //void ExtractInitializers(const TreeElementPtr& e);
  void MarkExplicitlyDefined();

  int NumSemanticOrUniformRegisters() const;
  int NumRegisters() const;
};

/**
* Represents a user defined symbol of a simple (int,float,etc) type                                                                 
* Can fit in a single register
*/
class BasicSymbol : public Symbol
{
public:
  BasicSymbol(const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value);

  virtual void AllocateRegisters(ILTranscriber* il);
  
  virtual ILRegister GetRegister() const;
  virtual ILRegister* GetPtrRegister() const { return (ILRegister*)&mRegister;};  
  
  virtual const SymbolPtr& GetSubpart(const ConstStringPtr& member_name);
  virtual std::string ToString() const;
  virtual SymbolPtr Clone() const;
  virtual BasicSymbol& AsBasicSymbol() { return *this; }

  virtual int SizeInRegisters() const { return 1; }

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleBinaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , const SymbolPtr& right
    , RegDT::RegDT forceType );

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleUnaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , RegDT::RegDT forceType );

  void SetRegister(ILRegister val) { mRegister = val; }


  virtual void MakeInitializerCode(ILTranscriber* il,const TreeElementPtr& init);

  virtual SymbolPtr CloneArrayIndex(ILRegister& rg) const;

  ILRegister GetMatrixElement(int row, int col) const;
 

  ///mark this as the result of a dynamic swizzle operation
  void SetDynamicSwizzle(const ILRegister& reg, const ILRegister& idx);


  bool IsDynamicSwizzle() const { return mDynSwizSource && mDynSwizIndex; }

  void DynamicSwizzleReadValue(ILTranscriber* il);
  void DynamicSwizzleWriteValue(ILTranscriber* il, ILRegister src);

private:
  ILRegister mRegister;
  ILRegister mDynSwizSource;
  ILRegister mDynSwizIndex;
};


/**
* Represents compound type (IE struct)
*/
class MultipartSymbol : public Symbol
{
public:
  MultipartSymbol(const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value);
  MultipartSymbol(const MultipartSymbol& that);

  virtual void AllocateRegisters(ILTranscriber* il);
  virtual const SymbolPtr& GetSubpart(const ConstStringPtr& name);
  virtual std::string ToString() const;
  virtual SymbolPtr Clone() const;

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleBinaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , const SymbolPtr& right
    , RegDT::RegDT forceType );

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleUnaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , RegDT::RegDT forceType );

  virtual void AssertNoInitializers() const;
  void MarkExplicitlyDefined();
  //virtual void ExtractInitializers(const TreeElementPtr& e);

  virtual int SizeInRegisters() const;

  virtual void SetModifiers(const ModifierSet& m);

  virtual SymbolPtr CloneArrayIndex(ILRegister& rg) const;
  SymbolPtr CloneArrayIndexGlobal( ILRegister& rg ) const;
  virtual bool IsMultipartSymbol() const {return true;}
  virtual MultipartSymbol& AsMultipartSymbol() 
  {
      return *this;
  }
  ILRegister GetMatrixElement(int row, int col) const;
  ILRegister GetMatrixRow(int row) const;

  std::vector<ILRegister> GetRawRegisters() const;

  virtual void MakeInitializerCode(ILTranscriber* il, const TreeElementPtr& init);

  virtual int NumSemanticOrUniformRegisters() const;
  
  ILRegister GetRegister() const
  {
    ILRegister val;

    for(SymbolTable::ConstIterator i=mMembers.Begin(); i!=mMembers.End(); ++i)
    {
        val = i->second->GetRegister();
    }

    return val;
  }
  // Only needed for literals.
  void SetLiteralRegister(ILRegister val)
  {
      for(SymbolTable::ConstIterator i=mMembers.Begin(); i!=mMembers.End(); ++i)
      {
        i->second->SetRegister(val);
      }
  }
  
private:
    SymbolTable mMembers;
};

/**
* Represents array type
*/
class ArraySymbol : public Symbol
{
public:
  ArraySymbol(int count, const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value);

  virtual void AllocateRegisters(ILTranscriber* il);
  virtual ILRegister GetRegister() const;
  virtual const SymbolPtr& GetSubpart(const ConstStringPtr& name);
  virtual std::string ToString() const;
  virtual SymbolPtr Clone() const;

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleBinaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , const SymbolPtr& right
    , RegDT::RegDT forceType );

  ///
  /// Run the requested command, performing all needed conversions and translation
  virtual int HandleUnaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , RegDT::RegDT forceType );

  virtual void AssertNoInitializers() const;

  void MarkExplicitlyDefined();

  virtual void SetModifiers(const ModifierSet& m);

  virtual int SizeInRegisters() const;

  virtual SymbolPtr CloneArrayIndex(ILRegister& rg) const;

  virtual ArraySymbol& AsArraySymbol() 
  {
    return *this;
  }

  virtual bool IsArraySymbol() const {return true;}

  SymbolPtr DoArrayIndexAlloc(ILTranscriber* il, ILRegister& base, const SymbolPtr& idx);
  void      DoArrayIndexIL(ILTranscriber* il, const ILRegister& base, const SymbolPtr& idx);

  virtual std::vector<ILRegister> GetRawRegisters() const;

  virtual void MakeInitializerCode(ILTranscriber* il,const TreeElementPtr& init);

  SymbolPtr StaticArrayIndex(int i);

  virtual int NumSemanticOrUniformRegisters() const;

  int ElementCount() const { return mCount; }
  int ElementSize() const { return mElementSize; }

private:
  SymbolPtr mFirstEntry;
  int mCount;
  int mElementSize;
  ILRegister mRegister;
  TreeElementPtr mElementSizeLiteral;
};

class ResourceSymbol : public Symbol
{
public:
  ResourceSymbol(const DataTypePtr& type, const ConstStringPtr& name, const TreeElementPtr& value = 0);

  ///
  /// Return mRegister, allocate if needed
  virtual void AllocateRegisters(ILTranscriber* il);

  virtual SymbolPtr Clone() const;

  virtual ResourceSymbol& AsResourceSymbol();

  int ResourceId() const;

  /// True if this represents a samplerstate
  bool IsSamplerState() const;

  //the rest of the methods throw errors
  virtual ILRegister GetRegister() const;
  virtual int SizeInRegisters() const;
  virtual SymbolPtr CloneArrayIndex(ILRegister& rg) const; 
  virtual const SymbolPtr& GetSubpart(const ConstStringPtr& name); 
  virtual int HandleBinaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , const SymbolPtr& right
    , RegDT::RegDT forceType = RegDT::Invalid );
  virtual int HandleUnaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , RegDT::RegDT forceType = RegDT::Invalid );

  void SetSubType(const DataTypePtr& st);
  const AST::DataTypePtr& GetSubType() const { return mSubType; }

  ILRegister GetDynamicResourceId() const { return mDynamicResourceId; }
  bool IsDynamicResource() const { return mIsDynamicResource; }

  void SetStaticResourceId(int x)
  { 
    mDynamicResourceId = 0; 
    mResourceId=x; 
    mIsDynamicResource = false; 
  }

  void SetDynamicResourceId(ILRegister x)
  { 
    mDynamicResourceId = x; 
    mIsDynamicResource = true; 
  }
private:
  int         mResourceId;
  DataTypePtr mSubType;
  ILRegister  mDynamicResourceId;
  bool        mIsDynamicResource;
};


class FunctionSymbol : public Symbol
{
public:
  FunctionSymbol(const ConstStringPtr& name);

  //most of these throw errors:
  virtual void AllocateRegisters(ILTranscriber* il);
  virtual int SizeInRegisters() const;
  virtual ILRegister GetRegister() const;
  virtual const SymbolPtr&  GetSubpart(const ConstStringPtr& name);
  virtual int HandleBinaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , const SymbolPtr& right
    , RegDT::RegDT forceType = RegDT::Invalid );
  virtual int HandleUnaryCommand( ILTranscriber* il
    , OpType::OpType op
    , const SymbolPtr& dst
    , RegDT::RegDT forceType = RegDT::Invalid );
  virtual SymbolPtr CloneArrayIndex(ILRegister& rg) const;
  virtual SymbolPtr Clone() const;


  virtual FunctionSymbol& AsFunctionSymbol();

  void AddOverload(const FunctionDeclarationPtr& func);

  FunctionDeclarationPtr ResolveOverloads(const ArgList& args) const;
  FunctionDeclarationPtr ResolveOverloads() const;

private:
  FunctionDeclarationList mOverloads;
};



}//namespace AST



#endif
