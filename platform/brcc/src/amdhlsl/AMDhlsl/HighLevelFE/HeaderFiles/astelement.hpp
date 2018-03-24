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

#ifndef AMD_ASTELEMENT_HPP
#define AMD_ASTELEMENT_HPP

#include "astdatatype.hpp"
#include "astsymbol.hpp"
#include "positionaware.hpp"
#include "iltranscriber.hpp"
#include "astmodifier.hpp"
#include <set>
#include <vector>

//macro to declare DebugClassName()
#define DCL_ASTELEMENT_DEBUGCLASSNAME(s) \
  const std::string& DebugClassName() const {static std::string n(s); return n;}

//macro to declare Clone()
#define DCL_ASTELEMENT_CLONE(T) \
  AST::TreeElementPtr Clone() const { return new T(*this); } 

//alias for the above to macros
#define DCL_ASTELEMENT_METHODS(T) \
  DCL_ASTELEMENT_CLONE(T) \
  DCL_ASTELEMENT_DEBUGCLASSNAME(#T)

namespace AST 
{

class TreeElement;
typedef SmartPtr<TreeElement> TreeElementPtr;
typedef std::vector<TreeElementPtr> TreeElementList;
class FunctionDeclaration;
class ArgList;
class VariableDeclaration;
class DelayedLookup;
class SamplerDeclaration;
class Initializer;

/**
* Base class for all elements in Abstract Syntax Tree
*/
class TreeElement : public RefCountedObj, public PositionAware
{
public:
  virtual AST::TreeElementPtr Clone() const = 0;
  virtual const std::string& DebugClassName() const = 0;

  //support for iterating over child elements
  typedef TreeElementList::iterator Iterator;
  typedef TreeElementList::const_iterator ConstIterator;
  Iterator Begin() { return mChildElements.begin(); }
  Iterator End()   { return mChildElements.end(); }
  ConstIterator Begin() const { return mChildElements.begin(); }
  ConstIterator End()   const { return mChildElements.end(); }

  ///
  /// Add a child node below this one in tree
  void AddChild(const TreeElementPtr& that){ mChildElements.push_back(that); }

  ///
  /// Add a given modifier to this
  virtual void AddModifier(const Modifier& name);

  ///
  /// Returns what the resulting datatype of this tree of expressions will be
  /// Returns 0 for invalid types
  virtual const AST::DataTypePtr& EffectiveDataType();

  ///
  /// Print a ascii representation of this AST to cerr
  void DebugPrint(std::ostream& os, int level = 0);

  //extra status string
  virtual std::string DebugStatus() const;

  //here are the args to WalkTree()

  /// (Step 1)
  /// Fills datatype tables and performs delayedlookups
  /// Returns number of unknown types
  virtual int ResolveDataTypes(TreeElementPtr& self, DataTypeTable*& scope);

  /// (Step 2)
  /// Fills symbol tables and performs delayedlookups
  /// Returns number of unknown symbols
  virtual int ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope);

  /// (Step 3)
  /// Reformats tree replacing constructs that dont translate to IL easily
  /// Returns number of simplifications performed
  //virtual int PreSimplifyTree(TreeElementPtr& self, TreeElement*& parent);

  /// (Step 3)
  /// Reformats tree replacing constructs that dont translate to IL easily
  /// Returns number of simplifications performed
  //virtual int PostSimplifyTree(TreeElementPtr& self, TreeElement* parent);

  /// (Step 3.5)
  /// Allocates registers to each VariableDeclaration
  virtual int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il);

  /// (Step 3.5)
  /// Allocates registers to each VariableDeclaration
  virtual int PostAllocateRegisters(TreeElementPtr& self, ILTranscriber* il);
  

  /// (Step 4)
  /// Exports tree to IL language (Called before recursive calls to children)
  /// Returns zero on success
  virtual int PreExportToIL(TreeElementPtr& self, ILTranscriber*& il);

  /// (Step 4)
  /// Exports tree to IL language (Called after recursive calls to children)
  /// Returns zero on success
  virtual int PostExportToIL(TreeElementPtr& self, ILTranscriber* il);

  template< typename ArgT >
  int NullPostAction(TreeElementPtr&, ArgT*){ return 0; }

  ///
  /// This method walks the tree calling *action on each node
  /// The action method gets a pointer to itself that it is allowed to modify
  ///   in order to change it's parent's pointer  
  /// The action method gets an arbitrary arg
  /// The action method returns an int which will be summed up by the tree walker
  /// postAction is like action but called after the recursive calls to child nodes
  template< typename ArgT
    , int (TreeElement::*action)(TreeElementPtr&, ArgT*&)
    , int (TreeElement::*postAction)(TreeElementPtr&, ArgT*) >
  int WalkTree(TreeElementPtr& self,  ArgT* value = 0 )
  {
    TreeElementPtr __keep_self_allocated_in_scope = self;

    //any errors generated at this level should report this node's position
    PositionAware oldPos = ::GetPosition();
    if(this->IsPositionValid()) ::SetGlobalPosition(*this);
    else this->SetPosition(oldPos); /*fill in missing metadata*/

    DEBUG_CHECK(self == this);

    mSkipChildren = false;
    
    // Call *action, it may modify either of its args.
    // If it modifies self, parent WalkTree() will handle it.
    // If it modifies value, modified value passed to child calls.
    int sum = (this->*action)(self, value);

    if(self!=this)
    {
      if(self==0) return sum;
      else return sum + self->WalkTree<ArgT, action, postAction>(self,value);
    }

    if ( !mSkipChildren )
    {
        size_t numDeletedChildren = 0;

        //recursive call on each child element
        for(Iterator i = Begin(); i!=End(); ++i)
        {
            //make copy of smartptr so element wont be deleted if child sets self=0
            TreeElementPtr childptr = *i;
            DEBUG_CHECK(childptr!=NULL)(DebugClassName());

            //recursive call
            sum += childptr->WalkTree<ArgT, action, postAction>(childptr, value);

            //store any changes made to childptr
            if(childptr == NULL) numDeletedChildren++;
            *i = childptr;
        }

        if ( 0 < numDeletedChildren )
        {
            size_t newSize = mChildElements.size()-numDeletedChildren;
            size_t n = 0;
            //Create a new list of non-deleted elements
            TreeElementList newList(newSize, TreeElementPtr());
            for(Iterator i = Begin(); i!=End(); ++i)
                if(*i != NULL)
                    newList[n++] = *i;
            ASSERT_ERROR(n == newSize)(n)(newSize)(DebugClassName());
            //Replace our list
            mChildElements = newList;

            // Note: This technique is faster than deleting as we go because it
            // requires only a single O(n) creation of a new list as opposed to
            // many shifts of every element in the list ( O(n*n) )
        }
    }

    // Call postaction
    sum += (this->*postAction)(self, value);   
   
    ::SetGlobalPosition(oldPos);

    return sum;
  }

  const SymbolPtr& GetResultSymbol() const { return mResultSymbol; }


  virtual FunctionDeclaration& AsFunction();
  virtual const ArgList& AsArgList() const;
  virtual DelayedLookup& AsDelayedLookup();
  virtual SamplerDeclaration& AsSampler();
  virtual VariableDeclaration& AsVarDecl();
  virtual bool IsInitializer() const { return false; }

  int NumChildren() const { return (int)mChildElements.size(); }

  ///
  /// Return a representation of this (to stratch register if needed) that may be 
  /// passed to a conditional
  ILRegister ScratchBooleanValue(ILTranscriber* il)  const
  {
    SymbolPtr sym = GetResultSymbol();
    ASSERT_ERROR(sym!=0).Text("Expected expression evaluating to boolean");
    ILRegister reg;
    if ( sym->IsMultipartSymbol() && sym->Type()->IsDoubleType() )
    {
        reg = sym->AsMultipartSymbol().GetSubpart(ConstString::Lookup("x"))->GetRegister();
    }
    else
    {
        reg = sym->AsBasicSymbol().GetRegister();
    }
    if(reg.GetDT()!=RegDT::Bool)
    {
      ILRegister scratch = il->Scratch();
      ILTranslator(il).ConvertTo(RegDT::Bool,scratch,reg);
      return scratch;
    }
    else
      return reg;
  }

  //At compiletime resolve an expression to an integer
  virtual int SimplifyToInt() const { ASSERT_ERROR(CanSimplifyToInt())(DebugClassName()).Text("Expression can not simplify to integer"); return -1; }
  virtual bool CanSimplifyToInt() const { return false; }

  const TreeElementList& GetChildren() const{ return mChildElements; }
  void SetChildren(const AST::TreeElementList& val) { mChildElements = val; }


  void PrependChild(const TreeElementPtr& child)
  {
    mChildElements.insert(mChildElements.begin(), child);
  }

  virtual bool IsDestOnlyBuiltinFunction() {return false;}   

  //Called on the destinaton of an assignment
  //virtual void AssignmentDestRepositionHook(  ILTranscriber* il, TreeElementPtr& assignment ) {}

protected:

  void SetResultSymbol(const SymbolPtr& val) { mResultSymbol = val; }

protected:
  ///
  /// Collection of child elements of this node in the tree
  TreeElementList mChildElements;
  
  ///
  /// the evaluated result of this tree might be store here
  SymbolPtr mResultSymbol;

  ///
  /// Collection of all modifiers to this node
  ModifierSet mModifiers;
        bool mSkipChildren;
};

/**
 * This level in the tree introduces a new local scope for symbols                                                                    
 */
class SymbolScope : public TreeElement
{
public:
  /// 
  /// Adds a level to the SymbolTable stack and replaces scope with mSymbolTable
  /*virtual*/ int ResolveSymbols(TreeElementPtr& self, SymbolTable*& scope);

  /*virtual*/ int PreAllocateRegisters(TreeElementPtr& self, ILTranscriber*& il);
private:
  SymbolTable     mSymbolTable;
};

/**
 * This level in the tree introduces a new local scope for datatypes
 */
class DataTypeScope : public TreeElement
{
public:
  /// 
  /// Adds a level to the DataTypeTable stack and replaces scope with mDataTypeTable
  /*virtual*/ int ResolveDataTypes(TreeElementPtr& self, DataTypeTable*& scope);
private:
  DataTypeTable   mDataTypeTable;
};


class NullTreeElement : public TreeElement
{
  DCL_ASTELEMENT_METHODS(NullTreeElement);
};


}

#endif

