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

#ifndef AMD_ASTDECLARATIONS_HPP
#define AMD_ASTDECLARATIONS_HPP

#include "astelement.hpp"

namespace AST
{

  /**
  * Baseclass for all declaration datatypes 
  *
  * Declarations exist during parsing but are quickly rolled into their
  * parents symbol table right after parsing finishes
  *
  */
  class Declaration : public TreeElement 
  {
  protected:
    Declaration(const ConstStringPtr& name, const ConstStringPtr& type);

  public:

    const ConstStringPtr& Name() const { return mName; }
    const DataTypePtr& Type() const { return mType;  }
    void SetType(const DataTypePtr& t) { mType=t;  }

    virtual void SetTypeStr(const ConstStringPtr& val, const ConstStringPtr& subType = 0) { mTypeStr = val; }

    virtual const AST::DataTypePtr& EffectiveDataType(){return Type();}

    virtual std::string DebugStatus() const { return (mTypeStr.IsValid() ? std::string(*mTypeStr) : "") + " " + std::string(mName.IsValid() ? std::string(*mName) : ""); }

    ///
    /// Looks up mType
    /*virtual*/ int ResolveDataTypes(TreeElementPtr& self, DataTypeTable*& scope);

  private:
    ///
    /// Name for this for when it gets inserted into lookup table
    ConstStringPtr mName;
    ///
    /// Type for this as a string (before lookup step)
    ConstStringPtr mTypeStr;
    ///
    /// Type for this (after lookup step)
    DataTypePtr mType;
  };


  /**
  * Declaration for a user Variable
  *     child TreeNodes represent initial value
  */
  class VariableDeclaration : public Declaration
  {
    DCL_ASTELEMENT_METHODS(VariableDeclaration);
  public:
    static const TreeElementPtr& UnspecifiedSize();
    static const TreeElementPtr& Single();

    //for declaring arrays
    VariableDeclaration(const ConstStringPtr name, const TreeElementPtr& arrayCount = 0)
      : Declaration(name,0)
      , mArrayCount( arrayCount )
    {}

    ///
    ///Create an entry for this variable in symbol table
    /*virtual*/ int ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope);


    //allocates a register
    virtual int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);


    virtual int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);

    VariableDeclaration& AsVarDecl(){return *this;}

    virtual void SetTypeStr(const ConstStringPtr& val, const ConstStringPtr& subType = 0)
    {
      Declaration::SetTypeStr(val);
      mSubType = subType;
    }


    int GetArrayCount() const 
    { 
      if(mArrayCount.IsValid() && mArrayCount->CanSimplifyToInt())
        return mArrayCount->SimplifyToInt();
      else
        return -1;
    }

  private:
    TreeElementPtr mArrayCount;
    ConstStringPtr mSubType;
  };

  /**
  * Declaration for a user datatype... currently only structs are supported
  *   Child tree nodes are VariableDeclarations for struct members
  *
  */
  class StructDeclaration : public Declaration
  {
    DCL_ASTELEMENT_METHODS(StructDeclaration);
  public:
    StructDeclaration(const ConstStringPtr& name);

    ///
    /// Creates a DataType entry for this struct
    /*virtual*/ int ResolveDataTypes(TreeElementPtr& self, DataTypeTable*& scope);

  private:
  };

  /**
  * Declaration for a user function
  *   First mNumArgs child nodes are arge definitions
  *   Child node mNumArgs+1 is a StatementList of function body
  */
  class FunctionDeclaration : public Declaration
  {
    DCL_ASTELEMENT_METHODS(FunctionDeclaration);
    static int NEXT_FUNCTION_ID;
  public:
    static void ResetGlobalState(){NEXT_FUNCTION_ID=1;}

    FunctionDeclaration( const ConstStringPtr& name
      , const ConstStringPtr& return_type
      , int numArgs);

    bool IsMain() const;

    ///
    /// Creates a symbol for this function
    /*virtual*/ int ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope);


    /// (Step 4)
    /// Exports tree to IL language (Called before recursive calls to children)
    /// Returns zero on success
    /*virtual*/ int PreExportToIL(TreeElementPtr& /*self*/, ILTranscriber*& il);

    int PostAllocateRegisters(TreeElementPtr& /*self*/, ILTranscriber* il);

    /// (Step 3.5)
    /// Allocates registers to each VariableDeclaration
    /*virtual*/ int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il);

    /// (Step 4)
    /// Exports tree to IL language (Called after recursive calls to children)
    /// Returns zero on success
    /*virtual*/ int PostExportToIL(TreeElementPtr& /*self*/, ILTranscriber* il);

    ///
    /// Returns what the resulting datatype of this tree of expressions will be
    /// Returns 0 for invalid types
    /*virtual*/ const AST::DataTypePtr& EffectiveDataType();
    
    int FunctionId() const { return mFunctionId; }
    int NumArgs() const { return mNumArgs; }
    bool IsReferenced() const { return mNumRefs > 0; }
    void AddReference() { ++mNumRefs; }
    const SymbolPtr& GetArg(int i) const
    {
      ASSERT_ERROR(i<mNumArgs);
      return mChildElements[i]->GetResultSymbol();
    }

    const DataTypePtr& GetArgType(int i) const
    {
      ASSERT_ERROR(i<mNumArgs);
      return mChildElements[i]->EffectiveDataType();
    }
    
    int GetArgArrayCount(int i) const
    {
      ASSERT_ERROR(i<mNumArgs);
      return mChildElements[i]->AsVarDecl().GetArrayCount();;
    }

    FunctionDeclaration& AsFunction() { return *this; }

    void SetMaxVertexCount(int n) { mMaxVertexCount=n; }
    int GetMaxVertexCount() const { return mMaxVertexCount; }
    
    void SetLdsSize(int n) { mLdsSize=n; }
    int GetLdsSize() const { return mLdsSize; }
    void SetLdsRel(int n) { mLdsRel=n; }
    int GetLdsRel() const { return mLdsRel; }
    void SetLdsAbs(int n) { mLdsAbs=n; }
    int GetLdsAbs() const { return mLdsAbs; }        
    void SetNumThreads(int n1, int n2, int n3) { mNumT[0]=n1; mNumT[1]=n2; mNumT[2]=n3;}
    int GetNumThreadsX() const { return mNumT[0]; }        
    int GetNumThreadsY() const { return mNumT[1]; }        
    int GetNumThreadsZ() const { return mNumT[2]; }        
  private:
    int mFunctionId;
    int mNumArgs;
    int mNumRefs;
    SymbolTable mFunctionArgScope;
    int mMaxVertexCount;
    int mNumT[3]; // num threads, at most 3-dimension
    int mLdsSize;
    int mLdsRel; // sharing mode
    int mLdsAbs;
  };


  class SamplerDeclaration : public Declaration
  {
    DCL_ASTELEMENT_METHODS(SamplerDeclaration);
  public:
    SamplerDeclaration(const ConstStringPtr& name);
    virtual SamplerDeclaration& AsSampler(){ return *this; }

    void SetSamplerAttrb(const ConstStringPtr& name, const TreeElementPtr& value)
    {
      mAttributes[name]=value;
    }

    void SetSamplerArraySize(int count)
    {
      DIE("Sampler arrays not yet supported")(count);
    }

    virtual int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il)
    {
      GetResultSymbol()->AllocateRegisters(il);
      return 0;
    }

    ///
    ///Create an entry for this sampler in symbol table
    /*virtual*/ int ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope)
    {
      DEBUG_CHECK(scope!=0);
      SetResultSymbol(new ResourceSymbol( ResourceDataType::TheSamplerStateType(), Name(), 0));
      scope->Insert(Name(), GetResultSymbol());
      return 0;
    }
  private:
    std::map< ConstStringPtr, TreeElementPtr > mAttributes;
  };


  class ConsBuffScope : public TreeElement
  {
    DCL_ASTELEMENT_METHODS(ConsBuffScope);
  public:
    ConsBuffScope(const ConstStringPtr& name)
      : mName(name)
    {}

    ///Enter scope via il->SetCurrentConstBuff
    virtual int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il);

    ///Leave scope via il->SetCurrentConstBuff
    virtual int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);
  private:
    ConstStringPtr mName;
  };


  class PersistentAllocScope: public TreeElement
  {
    DCL_ASTELEMENT_METHODS(PersistentAllocScope);
  public:
    PersistentAllocScope(const ConstStringPtr& name = 0)
      : mName(name)
    {}

    virtual int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il);
    virtual int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);
  private:
    ConstStringPtr mName;
  };

  class GlobalAllocScope : public TreeElement
  {
    DCL_ASTELEMENT_METHODS(GlobalAllocScope);
  public:
    GlobalAllocScope(const ConstStringPtr& name = 0)
      : mName(name)
    {}

    virtual int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il);
    virtual int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);
  private:
    ConstStringPtr mName;
  };


  class Initializer : public TreeElement
  {
    DCL_ASTELEMENT_METHODS(Initializer);
  public:
    virtual bool IsInitializer() const { return true; }
  };

}

#endif // AMD_ASTLITERALS_HPP

