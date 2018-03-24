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

#ifndef AMD_ASTOPERATORS_HPP
#define AMD_ASTOPERATORS_HPP

#include "astelement.hpp"
#include "iltranslator.hpp"


namespace AST
{
  class DataType;

  class Operation : public TreeElement 
  {
  public:
    ///
    /// Creates a temporary place to put result
    int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);
  };

  class BinaryOperation : public Operation
  {
  public:

    const AST::TreeElementPtr& Left() const;
    const AST::TreeElementPtr& Right() const;

    ///
    /// Returns what the resulting datatype of this tree of expressions will be
    /// Returns 0 for invalid types
    virtual const AST::DataTypePtr& EffectiveDataType();


  };

  class UnaryOperation : public Operation
  {
  public:

    const AST::TreeElementPtr&  Child() const;

    ///
    /// Returns what the resulting datatype of this tree of expressions will be
    /// Returns 0 for invalid types
    virtual const AST::DataTypePtr& EffectiveDataType();

  };

  /**
  * Generic definition for binary opcodes using ILTranslator                                                                    
  */
  template < OpType::OpType op, RegDT::RegDT forceType = RegDT::Invalid >
  class BasicBinaryOperation : public BinaryOperation
  {
    DCL_ASTELEMENT_METHODS(BasicBinaryOperation);

    /// 
    /// Convert all inputs to bools and call base
    int PostExportToIL(TreeElementPtr& self, ILTranscriber* il)
    {
      return Left()->GetResultSymbol()->HandleBinaryCommand(il, op, GetResultSymbol(), Right()->GetResultSymbol(), forceType);
    }

    virtual int SimplifyToInt() const 
    { 
      if(forceType == RegDT::Bool)
        return ILTranslator::RunOpStatically(op, Left()->SimplifyToInt()!=0, Right()->SimplifyToInt()!=0);
      else
        return ILTranslator::RunOpStatically(op, Left()->SimplifyToInt(), Right()->SimplifyToInt());
    }

    virtual bool CanSimplifyToInt() const 
    { 
      return Left()->CanSimplifyToInt() && Right()->CanSimplifyToInt();
    }
  };

  /**
  * Generic definition for unary opcodes using ILTranslator                                                                    
  */
  template < OpType::OpType op, RegDT::RegDT forceType = RegDT::Invalid >
  class BasicUnaryOperation : public UnaryOperation
  {
    DCL_ASTELEMENT_METHODS(BasicUnaryOperation);

    virtual int SimplifyToInt() const 
    { 
      return ILTranslator::RunUnaryOpStatically(op, Child()->SimplifyToInt());
    }
    virtual bool CanSimplifyToInt() const 
    { 
      return Child()->CanSimplifyToInt();
    }

    /// 
    /// Convert all inputs to bools and call base
    int PostExportToIL(TreeElementPtr& self, ILTranscriber* il)
    {
      return Child()->GetResultSymbol()->HandleUnaryCommand(il, op, GetResultSymbol(), forceType);
    }

  };

  namespace BinaryOps
  {
    //Generic ops
    typedef BasicBinaryOperation<OpType::And>     BitAnd;
    typedef BasicBinaryOperation<OpType::Or>      BitOr; 
    typedef BasicBinaryOperation<OpType::Xor>     BitXor;
    typedef BasicBinaryOperation<OpType::Mult>    Mult;  
    typedef BasicBinaryOperation<OpType::Div>     Div;   
    typedef BasicBinaryOperation<OpType::Mod>     Mod;   
    typedef BasicBinaryOperation<OpType::Plus>    Plus;  
    typedef BasicBinaryOperation<OpType::Minus>   Minus; 
    typedef BasicBinaryOperation<OpType::CmpLT>   CmpLT; 
    typedef BasicBinaryOperation<OpType::CmpGE>   CmpGE; 
    typedef BasicBinaryOperation<OpType::CmpEQ>   CmpEQ; 
    typedef BasicBinaryOperation<OpType::CmpNE>   CmpNE; 
    typedef BasicBinaryOperation<OpType::CmpLE>   CmpLE;   
    typedef BasicBinaryOperation<OpType::LShift>  LShift;
    typedef BasicBinaryOperation<OpType::RShift>  RShift;
    typedef BasicBinaryOperation<OpType::CmpGT>   CmpGT; 
    typedef BasicBinaryOperation<OpType::And, RegDT::Bool> And;
    typedef BasicBinaryOperation<OpType::Or, RegDT::Bool>  Or; 

    /**
    * Member access operator
    * Logically a binary op but member name is not allowed to be an expression
    */
    class MemberAccess : public UnaryOperation 
    {
      DCL_ASTELEMENT_METHODS(MemberAccess);
    public:
      MemberAccess(const ConstStringPtr& name)
        : mMemberName(name)
      {}

      MemberAccess(const TreeElementPtr& child, const ConstStringPtr& name)
        : mMemberName(name)
      {
        AddChild(child);
      }

      /// 
      /// Replace this with DelayedLookup to temporary
      int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* /*il*/);
      int PostExportToIL( TreeElementPtr& self, ILTranscriber* il );

      virtual const AST::DataTypePtr& EffectiveDataType();


       virtual std::string DebugStatus() const { return *mMemberName; }
    private:
      ConstStringPtr mMemberName;
    };

    /**
    * Set the value of Left() to the value of Right();
    * This is custom so as to insert a return value
    */
    class Assign : public BinaryOperation 
    { 
      DCL_ASTELEMENT_METHODS(Assign);
    public:
      Assign(const SymbolPtr& sym, const TreeElementPtr& value);
      Assign(){};

      //We DONT want Operator::PostAllocateRegisters to run
      int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);

      /// (Step 4)
      /// Exports tree to IL language (Called after recursive calls to children)
      /// Returns zero on success
      /*virtual*/ int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);
    };

    /**
    * store the initial value of Child() and return it                                                                    
    */
    class PostAssign : public Assign
    {
      DCL_ASTELEMENT_METHODS(PostAssign);
    public:
      PostAssign(const SymbolPtr& sym, const TreeElementPtr& value)
        : Assign(sym,value)
      {}
      PostAssign(){}

      int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);

      /// (Step 4)
      /// Hide away initial value
      /*virtual*/ int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);
    };

    /**
    * Ignores result of left, and just return right 
    */
    class PrecomputeLeftReturnRight : public BinaryOperation 
    { 
      DCL_ASTELEMENT_METHODS(PrecomputeLeftReturnRight);
    public:
      int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);
    };
  }



  namespace UnaryOps
  {
    typedef BasicUnaryOperation<OpType::Not>             BitNot;
    typedef BasicUnaryOperation<OpType::Not,RegDT::Bool> LogNot;
    typedef BasicUnaryOperation<OpType::Negate>          Negate;

    class Return : public UnaryOperation 
    {
      DCL_ASTELEMENT_METHODS(Return);
    public:

      /// (Step 2)
      /// Fills symbol tables and performs delayedlookups
      /// Returns number of unknown symbols
      /*virtual*/ int ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope);

      /// (Step 4)
      /// Exports tree to IL language (Called after recursive calls to children)
      /// Returns zero on success
      /*virtual*/ int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);
    };    


  }

  /**
  * a[b], get the b'th element out of a
  */
  class ArrayIndex : public BinaryOperation 
  { 
    DCL_ASTELEMENT_METHODS(ArrayIndex);
  public:


    int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);


    int PostExportToIL(TreeElementPtr& /*self*/, ILTranscriber* il);


    virtual const AST::DataTypePtr& EffectiveDataType();

  private:
    ILRegister mBase;
  };

  class TypeCaster : public UnaryOperation
  {
    DCL_ASTELEMENT_METHODS(TypeCaster);
  public:
    TypeCaster(const DataTypePtr& targetType, const TreeElementList& args) 
      : mTargetType(targetType) 
    {
      mChildElements = args;
    }

    TypeCaster(const DataTypePtr& targetType, const TreeElementPtr& arg) 
      : mTargetType(targetType) 
    {
      AddChild(arg);
    }

    const AST::DataTypePtr& EffectiveDataType();

    //Figure out where to put result 
    int PostAllocateRegisters( TreeElementPtr& self, ILTranscriber* il );

    //perform the actual casting
    int PostExportToIL( TreeElementPtr& self, ILTranscriber* il );

    virtual std::string DebugStatus() const { return mTargetType->ToString(); }
  private:
    DataTypePtr mTargetType;
  };


  template<typename T> inline TreeElementPtr 
    MakeBinaryOp(const TreeElementPtr& left, const TreeElementPtr& right)
  {
    TreeElementPtr t = new T();
    t->AddChild(left);
    t->AddChild(right);
    return t;
  }

  template<typename T> inline TreeElementPtr 
    MakeBinaryOpAssign(const TreeElementPtr& left, const TreeElementPtr& right)
  {
    return MakeBinaryOp<BinaryOps::Assign>(left,MakeBinaryOp<T>(left,right));
  }

  template<typename T> inline TreeElementPtr 
    MakeBinaryOpPostAssign(const TreeElementPtr& left, const TreeElementPtr& right)
  {
    TreeElementPtr t = new BinaryOps::PostAssign();
    t->AddChild(left);
    t->AddChild(MakeBinaryOp<T>(left,right));
    return t;
  }


}

#endif // AMD_ASTOPERATORS_HPP
