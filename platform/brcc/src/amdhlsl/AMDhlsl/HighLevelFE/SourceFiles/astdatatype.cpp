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

#include "astdatatype.hpp"
#include "astuserdatatype.hpp"
#include "convert.hpp"
#include "astelement.hpp"

namespace
{
  const AST::DataTypePtr& _nullDtPtr() {static AST::DataTypePtr n; return n;}

  STATIC_CONSTSTRING(matrixStr,"matrix");

  /**
   * A matrix is just a 1 to 4 member struct
   */
  class MatrixDataType : public AST::StructDataType
  {
    static AST::SymbolTable makeMatrixMembers(const AST::DataTypePtr& rowType, int numRows )
    {
      static ConstStringPtr rownames [] = {ConstString::Lookup("rowa"), ConstString::Lookup("rowb"), ConstString::Lookup("rowc"), ConstString::Lookup("rowd") };
      AST::SymbolTable table;
      while(numRows-->0)
        table.InsertSymbol( rowType, rownames[numRows], 0);
      return table;
    }
  public:
    MatrixDataType(RegDT::RegDT dt, int rows, int cols)
      : AST::StructDataType( matrixStr, makeMatrixMembers( AST::DataType::LookupSimpleType( dt, cols ), rows) ) 
      , mRows(rows)
      , mCols(cols)
      , mDt(dt)
    {}


    virtual bool IsMatrixType() const { return true; }

    virtual int NumRows() const { return mRows; } 
    virtual int NumCols() const { return mCols; } 

    virtual RegDT::RegDT GetRegDT() const { return  mDt; }
  private:
    int mRows;
    int mCols;
    RegDT::RegDT mDt;
  };



  /**
  * Void is just a struct of 0 elements
  */
  class VoidDataType : public AST::StructDataType
  {
    VoidDataType()
      : AST::StructDataType( ConstString::Lookup("void"), AST::SymbolTable() ) 
    {}
  public:
    static const AST::DataTypePtr& TheInst() 
    {
      static AST::DataTypePtr inst = new VoidDataType();
      return inst;
    }
  };

  /**
   * Simple vector types 
   */
  class BuiltinDatatype : public AST::DataType
  {
  public:
    BuiltinDatatype(const std::string& name, RegDT::RegDT regtype, int numslots)
      : AST::DataType(ConstString::Lookup(name))
      , mRegDT(regtype)
      , mSlots(numslots)
    {}

    virtual RegDT::RegDT GetRegDT() const { return mRegDT; }
    virtual int GetRegSlots() const { return mSlots; }

    virtual int NumRows() const { return 1; } 
    virtual int NumCols() const { return mSlots; } 

    virtual bool IsBasicType() const { return true; }
  private:
    RegDT::RegDT mRegDT;
    int mSlots;
  };

  //a double is a struct of __double
  class DoubleDataType : public AST::StructDataType
  {
    static const AST::DataTypePtr& theDoubleType()
    {
      static AST::DataTypePtr dt = new BuiltinDatatype("__double", RegDT::Double, 2);
      return dt;
    }
    static AST::SymbolTable makeMembers( int cols )
    {
      static ConstStringPtr colnames [] = {ConstString::Lookup("x"), ConstString::Lookup("y"), ConstString::Lookup("z"), ConstString::Lookup("w") };
      AST::SymbolTable table;
      for ( int ii = 0 ; ii < cols; ++ii )
      {
          table.InsertSymbol( theDoubleType(), colnames[ii], 0);
      }
      return table;
    }

    virtual int GetRegSlots() const {  return (int)GetParts().Size(); }
    virtual bool IsDoubleType() const { return true; }

  public:
    DoubleDataType(const char* name,int cols)
      : AST::StructDataType( ConstString::Lookup(name), makeMembers(cols) ) 
    {}

    virtual RegDT::RegDT GetRegDT() const { return  RegDT::Double; }
  };

  static const AST::DataTypeTable* generateTheBuiltinTypes()
  {
    static AST::DataTypeTable theTable;
    ASSERT_ERROR(theTable.Size()==0).Text("should not be called more than once");

    //matrix/vector types
    for(int dt = RegDT::Invalid+1; dt<RegDT::_count; ++dt)
    {
      for(int col=1; col<=4; ++col)
      {
        //Add the simple type
        AST::DataTypePtr st = AST::DataType::LookupSimpleType((RegDT::RegDT)dt, col);
        theTable.Insert(st->Name(), st);

        //fill in the matrix types
        for(int row=1; row<=4; ++row)  
        {
          ConstStringPtr name = ConstString::Lookup(*AST::DataType::LookupSimpleType((RegDT::RegDT)dt)->Name() + ToString(row) + "x" + ToString(col));
          AST::DataTypePtr type = AST::DataType::LookupMatrixType((RegDT::RegDT)dt,row,col);
          if(row>1) type->SetName(name);
          theTable.Insert(name, type);
        }
      }
    }

    //fill in all the resource types
    for(int i=0; i<ILSpec::IL_USAGE_PIXTEX_LAST; ++i)
    {
      AST::DataTypePtr rdt = AST::ResourceDataType::GetResourcePtrByType(ILSpec::ILPixTexUsage(i));
      if(rdt.IsValid()) theTable.Insert(rdt->Name(), rdt);
    }


#define ALIAS_BUILTIN_TYPE(newname, oldname)\
    theTable.Insert(ConstString::Lookup(newname), theTable.Lookup( ConstString::Lookup(oldname) ) );

    //void
    theTable.Insert(VoidDataType::TheInst()->Name(), VoidDataType::TheInst());

    //Half aliases
    ALIAS_BUILTIN_TYPE("half4", "float4");
    ALIAS_BUILTIN_TYPE("half3", "float3");
    ALIAS_BUILTIN_TYPE("half2", "float2");
    ALIAS_BUILTIN_TYPE("half",  "float");
    //*1 aliases
    ALIAS_BUILTIN_TYPE("bool1",   "bool");
    ALIAS_BUILTIN_TYPE("uchar1",  "uchar");
    ALIAS_BUILTIN_TYPE("char1",   "char");
    ALIAS_BUILTIN_TYPE("ushort1", "ushort");
    ALIAS_BUILTIN_TYPE("short1",  "short");
    ALIAS_BUILTIN_TYPE("int1",    "int");
    ALIAS_BUILTIN_TYPE("uint1",   "uint");
    ALIAS_BUILTIN_TYPE("float1",  "float");
    ALIAS_BUILTIN_TYPE("double1", "double");
    //dx9 texture aliases
    ALIAS_BUILTIN_TYPE("sampler1D","Texture1D");
    ALIAS_BUILTIN_TYPE("sampler2D","Texture2D");
    ALIAS_BUILTIN_TYPE("sampler3D","Texture3D");
    ALIAS_BUILTIN_TYPE("Sampler1D","Texture1D");
    ALIAS_BUILTIN_TYPE("Sampler2D","Texture2D");
    ALIAS_BUILTIN_TYPE("Sampler3D","Texture3D");

       

    return &theTable;
  }

}//file local namespace

/*static*/ const AST::DataTypeTable* 
AST::DataType::TheBuiltinDataTypes() 
{
  static const AST::DataTypeTable* instanceGeneratedOnce = generateTheBuiltinTypes();
  return instanceGeneratedOnce;
}

/*static*/ const AST::DataTypePtr& AST::DataType::TheFunctionDataType()
{
  static DataTypePtr f = new DataType(ConstString::Lookup("function"));
  return f;
}

const std::string&
AST::DataType::ToString() const
{
  return *mName;
}

const AST::SymbolTable& AST::DataType::GetParts() const
{
  DIE("Non-Multipart Datatype does not have parts!!!"); 
  return *(SymbolTable*)0;
}

bool AST::DataType::IsMultipart() const
{
  return false;
}

const ConstStringPtr& AST::DataType::Name() const
{
  return mName;
}

const AST::DataTypePtr& AST::DataType::LookupSimpleType( RegDT::RegDT dt, int slots )
{
  //Fast compiletime table
  static DataTypePtr typetable[9][4] = {
  {new BuiltinDatatype("bool",  RegDT::Bool,  1), new BuiltinDatatype("bool2",  RegDT::Bool,  2), new BuiltinDatatype("bool3",  RegDT::Bool,  3), new BuiltinDatatype("bool4",  RegDT::Bool,  4)},
  {new BuiltinDatatype("char",  RegDT::Char,  1), new BuiltinDatatype("char2",  RegDT::Char,  2), new BuiltinDatatype("char3",  RegDT::Char,  3), new BuiltinDatatype("char4",  RegDT::Char,  4)},
  {new BuiltinDatatype("uchar",  RegDT::UChar,  1), new BuiltinDatatype("uchar2",  RegDT::UChar,  2), new BuiltinDatatype("uchar3",  RegDT::UChar,  3), new BuiltinDatatype("uchar4",  RegDT::UChar,  4)},
  {new BuiltinDatatype("short",  RegDT::Short,  1), new BuiltinDatatype("short2",  RegDT::Short,  2), new BuiltinDatatype("short3",  RegDT::Short,  3), new BuiltinDatatype("short4",  RegDT::Short,  4)},
  {new BuiltinDatatype("ushort",  RegDT::UShort,  1), new BuiltinDatatype("ushort2",  RegDT::UShort,  2), new BuiltinDatatype("ushort3",  RegDT::UShort,  3), new BuiltinDatatype("ushort4",  RegDT::UShort,  4)},
  {new BuiltinDatatype("int",   RegDT::Int,   1), new BuiltinDatatype("int2",   RegDT::Int,   2), new BuiltinDatatype("int3",   RegDT::Int,   3), new BuiltinDatatype("int4",   RegDT::Int,   4)},
  {new BuiltinDatatype("uint",  RegDT::UInt,  1), new BuiltinDatatype("uint2",  RegDT::UInt,  2), new BuiltinDatatype("uint3",  RegDT::UInt,  3), new BuiltinDatatype("uint4",  RegDT::UInt,  4)},
  {new BuiltinDatatype("float", RegDT::Float, 1), new BuiltinDatatype("float2", RegDT::Float, 2), new BuiltinDatatype("float3", RegDT::Float, 3), new BuiltinDatatype("float4", RegDT::Float, 4)},
  {new DoubleDataType( "double",1),               new DoubleDataType( "double2",2),               new DoubleDataType( "double3", 3),              new DoubleDataType( "double4",4)}
  };

  ASSERT_ERROR(slots>=1 && slots<=4)(slots);

  switch(dt){                                    
      case RegDT::Bool:   return typetable[0][slots-1];
      case RegDT::UChar:  return typetable[1][slots-1];
      case RegDT::Char:  return typetable[2][slots-1];
      case RegDT::Short:  return typetable[3][slots-1];
      case RegDT::UShort:  return typetable[4][slots-1];
      case RegDT::Int:    return typetable[5][slots-1];
      case RegDT::UInt:   return typetable[6][slots-1];
      case RegDT::Float:  return typetable[7][slots-1];
      case RegDT::Double: return typetable[8][slots-1];
      default: 
        DIE("Unknown type")(dt)(slots);
        return _nullDtPtr();
  }
}

AST::DataTypePtr AST::ResourceDataType::GetResourcePtrByType( ILSpec::ILPixTexUsage t )
{
  switch(t)
  {
  case ILSpec::IL_USAGE_PIXTEX_BUFFER:  { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("Buffer"));         return inst; }
  case ILSpec::IL_USAGE_PIXTEX_1D:      { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("Texture1D"));      return inst; }
  case ILSpec::IL_USAGE_PIXTEX_1DARRAY: { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("Texture1DArray")); return inst; }
  case ILSpec::IL_USAGE_PIXTEX_2D:      { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("Texture2D"));      return inst; }
  case ILSpec::IL_USAGE_PIXTEX_2DARRAY: { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("Texture2DArray")); return inst; }
  case ILSpec::IL_USAGE_PIXTEX_3D:      { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("Texture3D"));      return inst; }
  case ILSpec::IL_USAGE_PIXTEX_CUBEMAP: { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("TextureCube"));    return inst; }
  //Texture2DMS???? Texture2DMSArray?????
  case ILSpec::IL_USAGE_PIXTEX_UNKNOWN: { static DataTypePtr inst = new ResourceDataType(t,ConstString::Lookup("<Unknown>"));      return inst; }
  default: 
    return _nullDtPtr();
  }
}

namespace
{
  template < RegDT::RegDT dt, int rows >
  const AST::DataTypePtr& lookupMatrixTypeDtRow( int cols )
  {
    //this single declaration will generate 15 different linkable functions 
    // (5 datatype * 3 row values)
    // each of those 15 linkable function will have 4 static DataTypePr instances
    // thus generating our 60 DataTypePtr "table" of matrix types
    switch(cols)
    {
    case 1: { static AST::DataTypePtr inst = new MatrixDataType(dt, rows, 1); return inst; }
    case 2: { static AST::DataTypePtr inst = new MatrixDataType(dt, rows, 2); return inst; }
    case 3: { static AST::DataTypePtr inst = new MatrixDataType(dt, rows, 3); return inst; }
    case 4: { static AST::DataTypePtr inst = new MatrixDataType(dt, rows, 4); return inst; }
    default: DIE("invalid matrix")(rows)(cols);
      return _nullDtPtr();
    }
  }

  template < RegDT::RegDT  dt>
  const AST::DataTypePtr& lookupMatrixTypeDt( int rows, int cols )
  {
    switch(rows)
    {
    case 2: return lookupMatrixTypeDtRow< dt, 2 >(cols);
    case 3: return lookupMatrixTypeDtRow< dt, 3 >(cols);
    case 4: return lookupMatrixTypeDtRow< dt, 4 >(cols);
    default: DIE("invalid matrix")(rows)(cols);
      return _nullDtPtr();
    }
  }
}

const AST::DataTypePtr& AST::DataType::LookupMatrixType( RegDT::RegDT dt, int rows, int cols )
{
  if(rows==1) return LookupSimpleType(dt,cols);
  switch(dt){                                    
      case RegDT::Bool:   return lookupMatrixTypeDt<RegDT::Bool>(rows,cols);
      case RegDT::UChar:  return lookupMatrixTypeDt<RegDT::UChar>(rows,cols);
      case RegDT::Char:   return lookupMatrixTypeDt<RegDT::Char>(rows,cols);
      case RegDT::UShort: return lookupMatrixTypeDt<RegDT::UShort>(rows,cols);
      case RegDT::Short:  return lookupMatrixTypeDt<RegDT::Short>(rows,cols);
      case RegDT::Int:    return lookupMatrixTypeDt<RegDT::Int>(rows,cols);
      case RegDT::UInt:   return lookupMatrixTypeDt<RegDT::UInt>(rows,cols);
      case RegDT::Float:  return lookupMatrixTypeDt<RegDT::Float>(rows,cols);
      case RegDT::Double: return lookupMatrixTypeDt<RegDT::Double>(rows,cols);
      default: 
        DIE("Unknown type")(dt)(rows)(cols);
        return _nullDtPtr();
  }
}

const AST::DataTypePtr& AST::DataType::CombineTypes( const DataTypePtr& leftType, const DataTypePtr& rightType )
{
  if(leftType!=rightType)
  {
    //hmm.. type mismatch lets try to handle it

    //we can combine basics types with a max
    if( (leftType->IsBasicType()  || leftType->IsDoubleType()) 
     && (rightType->IsBasicType() || rightType->IsDoubleType()))
    {
      int slts = Minimum(leftType->GetRegSlots(),rightType->GetRegSlots());

      //special case when one side is a single slotted
      if(leftType->GetRegSlots()==1) slts = rightType->GetRegSlots();
      if(rightType->GetRegSlots()==1) slts = leftType->GetRegSlots();
       
      return DataType::LookupSimpleType(Maximum(leftType->GetRegDT(), rightType->GetRegDT()) ,slts);
    }

    //prefer matrix types
    if(rightType->IsMatrixType())
      return rightType;
  }
  return leftType;
}

const AST::DataTypePtr& AST::DataType::TheVoidType()
{
  return VoidDataType::TheInst();
}


