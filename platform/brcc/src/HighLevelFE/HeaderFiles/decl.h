/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
All rights reserved.


The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the
contents of this directory.

****************************************************************************/


/*  o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o

    CTool Library
    Copyright (C) 1998-2001     Shaun Flisakowski

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

/*  o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o
    o+
    o+     File:         decl.h
    o+
    o+     Programmer:   Shaun Flisakowski
    o+     Date:         Aug 9, 1998
    o+
    o+     A high-level view of types / declarations.
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

#ifndef    INC_DECL_H
#define    INC_DECL_H

#include "janitorial.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cassert>

#include "logger.hpp"

#include "symbol.h"
#include "callback.h"
#include "location.h"

#include "dup.h"
#include "types.h"

class Constant;
class Expression;
class EnumConstant;



typedef unsigned long BaseTypeSpec;

// FIXME - long term, this representation isn't going to be enough

const BaseTypeSpec BT_NoType = 0x00000000;      // no type provided
const BaseTypeSpec BT_Void = 0x00000001;        // explicitly no type
//const BaseTypeSpec BT_Char = 0x00000002;
//const BaseTypeSpec BT_Short = 0x00000004;
//const BaseTypeSpec BT_Int = 0x00000008;
const BaseTypeSpec BT_Double = 0x00000010;
const BaseTypeSpec BT_Double2 = 0x00000020;
const BaseTypeSpec BT_Long = 0x00000040;
const BaseTypeSpec BT_Ellipsis = 0x00000080;

/*
 * These have to be adjacent and have these values for the math that parses
 * float2, float3, float4 in lexer.l to work.
 */
const BaseTypeSpec BT_Float = 0x00000100;
const BaseTypeSpec BT_Float2 = 0x00000200;
const BaseTypeSpec BT_Float3 = 0x00000400;
const BaseTypeSpec BT_Float4 = 0x00000800;

const BaseTypeSpec BT_Struct = 0x00001000;
const BaseTypeSpec BT_Union = 0x00002000;
const BaseTypeSpec BT_Enum = 0x00004000;
const BaseTypeSpec BT_UserType = 0x00008000;

const BaseTypeSpec BT_Char = 0x00010000;
const BaseTypeSpec BT_Char2 = 0x00020000;
const BaseTypeSpec BT_Char3 = 0x00040000;
const BaseTypeSpec BT_Char4 = 0x00080000;

const BaseTypeSpec BT_Short = 0x00100000;
const BaseTypeSpec BT_Short2 = 0x00200000;
const BaseTypeSpec BT_Short3 = 0x00400000;
const BaseTypeSpec BT_Short4 = 0x00800000;

const BaseTypeSpec BT_Int =  0x01000000;
const BaseTypeSpec BT_Int2 = 0x02000000;
const BaseTypeSpec BT_Int3 = 0x04000000;
const BaseTypeSpec BT_Int4 = 0x08000000;

const BaseTypeSpec BT_BaseMask = 0x0FFFFFFF;

// Sign indicator
const BaseTypeSpec BT_Signed = 0x10000000;
const BaseTypeSpec BT_UnSigned = 0x20000000;
const BaseTypeSpec BT_SignMask = 0x30000000;

// Space for compiler builtins (FIXME - should be allow this?)

const BaseTypeSpec BT_GCC_BuiltInVAList = 0x40000000;

// And the error flag

const BaseTypeSpec BT_TypeError = 0x80000000;


typedef struct BaseTypeInfo_
{
    const char *brTypeName;     //parse name,
    BaseTypeSpec brTypeID;      //the id for the brook type name
    const char *shTypeName;     //name used to generated ifetch etc
    const char *rtTypeName;     //the runtime type name, the name appears in the output cpp file
    const char *rtTypeIDStr;    //the name for the enum defined in the brook "enum StreamType"
    int component;
    int swizzleComponent;       //in "xyzw"
    bool isComplexType;
} BaseTypeInfo;


//! \brief Short vetcor of same type goes to same category
//!       Ex: float4, float3, float2, float are in the same category
typedef enum _BTCategory
{
    BT_Non_Group = 0,
    BT_Void_Group,
    BT_Char_Group,     //! Integer groups
    BT_UChar_Group,
    BT_Short_Group,
    BT_UShort_Group,
    BT_Int_Group,
    BT_Long_Group,
    BT_UInt_Group,
    BT_Float_Group,    //! Real groups
    BT_Double_Group,
    BT_UserType_Group,  //! User type groups(Typedefs)
    BT_Struct_Group,  //! struct
    BT_Union_Group,  //! union
    BT_Enum_Group  //! enum
}BTCategory;

//! \brief Each brook type has category and components
//!       Ex: int type has BR_Int_Group as category, 1 as component
typedef struct _BTDetails
{
    _BTDetails()
        :category(BT_Int_Group), components(1)
    {
    }
    BTCategory category;
    int components;
}BTDetails;

//! Returns true if given category is a integer type 
bool isIntegerType(BTDetails baseTypeDetails);

//! Returns true if given category is a real type
bool isRealType(BTDetails baseTypeDetails);

//! Returns true if base type is either integer type or real type
bool isArithmeticType(BTDetails baseTypeDetails);

//! Returns true if given type details hav one component
bool isScalarType(BTDetails baseTypeDetails);


//! Returns true if given type mask is 128 bit base data type
bool is128bitBaseType(BaseTypeSpec typeMask);

//! \brief Binary operands category
//!        float2(left operand) and int2(right operand) is example of BR_DiffTypes_EqualComponents category
typedef enum _BinaryOperandsCategory
{
    BT_DiffTypes_UnequalComponents = 0,
    BT_DiffTypes_EqualComponents,
    BT_SameTypes_UnequalComponents,
    BT_SameTypes_EqualComponents
}BinaryOperandsCategory;

//! \brief Binary operands details
//!        category, left/right operand type details
typedef struct _BinaryOperandsDetails
{
    _BinaryOperandsDetails()
        :category(BT_SameTypes_EqualComponents)
    {
    }
    BinaryOperandsCategory category;
    BTDetails leftOperandDetails;
    BTDetails rightOperandDetails;
}BinaryOperandsDetails;

BTDetails getTypeDetails(BaseTypeSpec baseTypeID);
std::string getScalarTypeStr(BTCategory category);
BinaryOperandsDetails getBinaryOperandsDetails(BaseTypeSpec leftBaseTypeID, BaseTypeSpec rightBaseTypeID);
bool getWarnMsgForBinarayOperands(BinaryOperandsDetails details, std::string &warnMsg);
bool checkForTypeAndComponents(BTDetails leftOperand, BTDetails rightOperand);
bool checkForConversionLose(BTDetails promotionType, BTDetails actualType);
int requiredTypePromotion(BTDetails leftOperand, BTDetails rightOperand);

std::string getCurrentKernelName();
void setCurrentKernelName(std::string kernelName);
void resetCurrentKernelName();


enum BaseTypeNameKind
{
    BaseTypeBRName = 0,
    BaseTypeSHName,
    BaseTypeRTName,
    BaseTypeRTEnumName
};

extern const char *getBaseTypeName(BaseTypeSpec typemask, BaseTypeNameKind, bool * pIsComplex = NULL);
extern BaseTypeSpec getBaseTypeSpec(std::string baseTypeName);
extern bool isSupportedDoubleMathOps(std::string funcName);
extern bool isRelationalBuiltIn(std::string funcName);

struct TypeQual
{
    unsigned short tq;
    Expression *vout;
    bool operator ==(unsigned int x) const
    {
        return x == tq;
    }
    bool operator ==(const TypeQual & x) const
    {
        return x.tq == tq;
    }
    bool operator !=(const TypeQual & x) const
    {
        return tq != x.tq;
    }
    bool operator !=(const unsigned int x) const
    {
        return tq != x;
    }

    void init(unsigned short t)
    {
        tq = t;
        vout = NULL;
    }
    void init(unsigned short t, Expression * e)
    {
        tq = t;
        vout = e;
    }
    TypeQual operator &(unsigned int x) const
    {
        TypeQual ret(*this);
                 ret &= x;
                 return ret;
    }
    TypeQual operator |(unsigned int x) const
    {
        TypeQual ret(*this);
                 ret |= x;
                 return ret;
    }
    TypeQual operator &(const TypeQual & x) const
    {
        TypeQual ret(*this);
                 ret &= x;
                 return ret;
    }
    TypeQual operator |(const TypeQual & x) const
    {
        TypeQual ret(*this);
                 ret |= x;
                 return ret;
    }
    TypeQual & operator |=(unsigned int x)
    {
        tq |= x;
        return *this;
    }
    TypeQual & operator &=(unsigned int x)
    {
        tq &= x;
        return *this;
    }
    TypeQual operator ~() const
    {
        TypeQual ret(*this);
                 ret.tq = ~ret.tq;
                 return ret;
    }
    TypeQual & operator |=(const TypeQual & x)
    {
        tq |= x.tq;
        if (x.vout && !vout)
        {
            vout = x.vout;
        }
        return *this;
    }
    TypeQual & operator &=(const TypeQual & x)
    {
        tq &= x.tq;
        return *this;
    }

    static TypeQual ReturnNone()
    {
        TypeQual ret;

        ret.init(0x0000);
        return ret;
    }
    static TypeQual ReturnConst()
    {
        TypeQual ret;

        ret.init(0x0001);
        return ret;
    }
    static TypeQual ReturnVolatile()
    {
        TypeQual ret;

        ret.init(0x0002);
        return ret;
    }
    static TypeQual ReturnOut()
    {
        TypeQual ret;

        ret.init(0x0004);
        return ret;
    }
    static TypeQual ReturnReduce()
    {
        TypeQual ret;

        ret.init(0x0008);
        return ret;
    }
    static TypeQual ReturnVout()
    {
        TypeQual ret;

        ret.init(0x0010);
        return ret;
    }
    static TypeQual ReturnIter()
    {
        TypeQual ret;

        ret.init(0x0020);
        return ret;
    }
    static TypeQual ReturnKernel()
    {
        TypeQual ret;

        ret.init(0x0040);
        return ret;
    }
    static TypeQual ReturnGeneratedIndexOf()
    {
        TypeQual ret;

        ret.init(0x0080);
        return ret;
    }
    static TypeQual ReturnGeneratedGatherConst()
    {
        TypeQual ret;

        ret.init(0x0100);
        return ret;
    }
    static TypeQual ReturnRestrict()
    {
        TypeQual ret;
        ret.init(0x0200);
        return ret;
    }
    static TypeQual ReturnShared()
    {
        TypeQual ret;
        ret.init(0x0400);
        return ret;
    }
};

const TypeQual TQ_None = TypeQual::ReturnNone();
const TypeQual TQ_Const = TypeQual::ReturnConst();
const TypeQual TQ_Volatile = TypeQual::ReturnVolatile();
const TypeQual TQ_Out = TypeQual::ReturnOut();
const TypeQual TQ_Reduce = TypeQual::ReturnReduce();
const TypeQual TQ_Vout = TypeQual::ReturnVout();
const TypeQual TQ_Iter = TypeQual::ReturnIter();
const TypeQual TQ_Kernel = TypeQual::ReturnKernel();
const TypeQual TQ_GeneratedIndexOf = TypeQual::ReturnGeneratedIndexOf();
const TypeQual TQ_GeneratedGatherConst = TypeQual::ReturnGeneratedGatherConst();
const TypeQual TQ_Restrict = TypeQual::ReturnRestrict();
const TypeQual TQ_Shared = TypeQual::ReturnShared();

enum StorageType
{
    ST_None = 0,
    ST_Auto,
    ST_Extern,
    ST_Register,
    ST_Static,
    ST_Typedef,
};


enum TypeKind
{
    TK_Base = 0,
    TK_TypeDef,
    TK_UserType
};


enum TypeType
{
    TT_Base,                    // a simple base type, T
    TT_Pointer,                 // pointer to T
    TT_Array,                   // an array of T
    TT_Stream,                  // a stream / iter of T
    TT_BitField,                // a bitfield
    TT_Function,                // <args> -> <result>

    TT_BrtStream,               // Converted stream declaration
    TT_BrtIter,                 // Converted iter declaration

    /*    These are now considered TT_Base
       TT_Struct,
       TT_Union,
       TT_Enum
     */
};


// For gcc extension __attribute__
enum GccAttribType
{
    GCC_Unsupported = 0,        // Not supported by cTool

    GCC_Aligned,
    GCC_Packed,
    GCC_CDecl,
    GCC_Mode,
    GCC_Format,

    GCC_Const,
    GCC_NoReturn,
    GCC_NoThrow,
    GCC_NonNull,
    GCC_Pure,
    GCC_Malloc,
    Kernel_GroupSize,
    GCC_WarnUnusedResult
};



class Decl;


class StructDef
{
  public:
    StructDef(bool _is_union = false);
    ~StructDef();

    bool isUnion() const
    {
        return _isUnion;
    }

    StructDef *dup0() const;
    StructDef *dup() const;     // deep-copy

    void print(std::ostream & out, Symbol * name, int level) const;
    bool printStructureStreamHelper(std::ostream & out, bool raw) const;
    bool printStructureStreamShape(std::ostream & out);

    void findExpr(fnExprCallback cb);

    void addComponent(Decl * comp);

    void registerComponents();
    Decl *findComponent(Symbol * sym, u32 * offset);

    // Lookup this symbol in this struct/union and set its entry
    // if its a component of it.
    bool lookup(Symbol * sym) const;

    bool _isUnion;
    Symbol *tag;

    int size;                   // size of the array.
    int nComponents;
    Decl **components;

  private:
    Decl * lastDecl();
};


class EnumDef
{
  public:
    EnumDef();
    ~EnumDef();

    EnumDef *dup0() const;
    EnumDef *dup() const;       // deep-copy

    void print(std::ostream & out, Symbol * name, int level) const;
    void findExpr(fnExprCallback cb);

    void addElement(Symbol * nme, Expression * val = NULL);

    void addElement(EnumConstant * ec);

    Symbol *tag;

    int size;                   // size of the arrays.
    int nElements;
    Symbol **names;
    Expression **values;
};



class Type;
typedef Dup < Type > DupableType;

class Type:public DupableType
{
  public:
    Type(TypeType _type = TT_Base);
    virtual ~ Type();
    virtual Type **getSubType()
    {
        return NULL;
    }
    virtual int precedence() const
    {
        return 16;
    }
    virtual Type *dup0() const = 0;     // deep-copy

    virtual Type *extend(Type * UNUSED(extension))
    {
        assert(0);
        return NULL;
    }

    // This function handles the complexity of printing a type.
    virtual void printType(std::ostream & out, Symbol * name,
                           bool showBase, int level, bool rawType = false /*prevents cpu-specific transforms */ )const;

    virtual bool printStructureStreamHelperType(std::ostream & out, const std::string & name, bool raw) const = 0;
    virtual bool printStructureStreamShape(std::ostream & UNUSED(out))
    {
        return false;
    }

    virtual void printBase(std::ostream & UNUSED(out), int UNUSED(level)) const
    {
    }
    virtual void printRawBase(std::ostream & out, int level) const
    {
        printBase(out, level);
    }
    virtual void printBefore(std::ostream & UNUSED(out), Symbol * UNUSED(name), int UNUSED(level)) const
    {
    }
    virtual void printAfter(std::ostream & UNUSED(out)) const
    {
    }

    virtual void printForm(std::ostream & out) const
    {
        out << "-[ABT]";
    }

    virtual void registerComponents()
    {
    }
    virtual Decl *findComponent(Symbol * sym, u32 * offset)
    {
        if (getRealType() != this)
            return getRealType()->findComponent(sym, offset);
        else
            return NULL;
    }

    virtual void findExpr(fnExprCallback UNUSED(cb))
    {
    }

    virtual bool lookup(Symbol * UNUSED(sym)) const
    {
        return false;
    }

    virtual TypeQual getQualifiers(void) const = 0;
    virtual BaseType *getBase(void) = 0;

    Type *getRealType() const;

    void tokenize(const std::string& str, std::vector<std::string>& tokens, 
                  const std::string& delimiters) const;

    bool isBaseType() const
    {
        return (type == TT_Base);
    }
    bool isPointer() const
    {
        return (type == TT_Pointer);
    }
    bool isFunction() const
    {
        return (type == TT_Function);
    }
    bool isArray() const
    {
        return (type == TT_Array);
    }
    bool isStream() const
    {
        return (type == TT_Stream);
    }
    bool isReduce() const
    {
        return (getQualifiers() & TQ_Reduce) != 0;
    }
    bool isKernel() const
    {
        return ((getQualifiers() & (TQ_Reduce | TQ_Kernel)) != 0);
    }

    virtual bool isStructure() const
    {
        return false;
    }
    virtual bool isBuitinStreamElementType() const
    {
        return false;
    }
    
    //! Return true if this type is used as kernel parameter
    bool isKernelParam() const
    {
        return kernelParam;
    }

    bool isBuitinStreamType();

    // Delete all types stored in this list.
    static void DeleteTypeList(Type * typelist);
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void allocateILStorage(bril::store * r) const;
#endif
    TypeType type;

    //! Set this to true in kernel definition while doing 
    //! semantic check on kernel parameter declarations
    //! Default value is false
    bool kernelParam; 

    // Temporary - is moved into the declaration (Decl).
    StorageType storage;

  private:
    Type * link;                // For linking all type classes togather.
};

StructDef *findStructureDef(Type const *inType);


class BaseType:public Type
{
  public:
    BaseType(BaseTypeSpec = BT_NoType);
    BaseType(StructDef * sd);
    BaseType(EnumDef * ed);
    ~BaseType();

    Type *dup0() const;         // deep-copy

    Type *extend(Type * UNUSED(extension))
    {
        assert(0);
        return NULL;
    }

    bool printStructureStreamHelperType(std::ostream & out, const std::string & name, bool raw) const;
    bool printStructureStreamShape(std::ostream & out);

    void printBase(std::ostream & out, int level) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const;
    void printAfter(std::ostream & out) const;

    void printForm(std::ostream & out) const;

    void registerComponents();
    virtual Decl *findComponent(Symbol * sym, u32 * offset);

    bool lookup(Symbol * sym) const;

    TypeQual getQualifiers(void) const;
    BaseType *getBase(void);
    BaseType & operator =(const BaseType & b);

    BaseTypeSpec typemask;
    int getNumComponent();

    virtual bool isStructure() const
    {
        return findStructureDef(this) ? true : false;
    }

    virtual bool isBuitinStreamElementType() const;
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void allocateILStorage(bril::store * r) const;
#endif

    TypeQual qualifier;

    Symbol *tag;                // tag for struct/union/enum
    Symbol *typeName;           // typedef name for a UserType

    StructDef *stDefn;          // optional definition of struct/union
    EnumDef *enDefn;            // optional definition of enum

};


class PtrType:public Type
{
  public:
  PtrType(TypeQual qual = TQ_None):Type(TT_Pointer), qualifier(qual), subType(NULL)
    {
    };

    ~PtrType()
    {
    };
    virtual Type **getSubType()
    {
        return &subType;
    }
    int precedence() const
    {
        return 15;
    }

    Type *dup0() const;         // deep-copy

    Type *extend(Type * extension);

    bool printStructureStreamHelperType(std::ostream & UNUSED(out), const std::string & UNUSED(name), bool UNUSED(raw)) const
    {
        return false;
    }

    void printBase(std::ostream & out, int level) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const;
    void printAfter(std::ostream & out) const;

    void printForm(std::ostream & out) const;

    void registerComponents()
    {
        if (subType)
            subType->registerComponents();
    }
    Decl *findComponent(Symbol * sym, u32 * offset)
    {
        if (subType)
            return subType->findComponent(sym, offset);
        return NULL;
    }

    void findExpr(fnExprCallback cb);

    bool lookup(Symbol * sym) const;

    TypeQual getQualifiers(void) const;
    BaseType *getBase(void);

    TypeQual qualifier;
    Type *subType;
};

typedef enum _ArrayResourceType
{
    TextureType = 0, //! stream or gather or scatter
    ConstBufferType
}ArrayResourceType;

class ArrayType:public Type
{
  public:
    ArrayType(TypeType t, Expression * s, ArrayResourceType resType = TextureType);

    ~ArrayType();

    Type *dup0() const;         // deep-copy

    Type *extend(Type * extension);
    virtual Type **getSubType()
    {
        return &subType;
    }
    bool printStructureStreamHelperType(std::ostream & UNUSED(out), const std::string & UNUSED(name), bool UNUSED(raw)) const
    {
        return false;
    }

    void printBase(std::ostream & out, int level) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const;
    void printAfter(std::ostream & out) const;

    void printForm(std::ostream & out) const;

    void registerComponents()
    {
        if (subType)
            subType->registerComponents();
    }
    Decl *findComponent(Symbol * sym, u32 * offset)
    {
        if (subType)
            return subType->findComponent(sym, offset);
        return NULL;
    }

    void findExpr(fnExprCallback cb);

    bool lookup(Symbol * sym) const;

    //! Returns Dimensions
    int getDimensions();

    //! Return GroupSize
    int getGroupSize();

    //! Returns size of given dimension
    //! dim = 1 means leftmost index size
    int getDimensionsSize(int dim);

    //! Returns total number of elements
    int getTotalElementCount();

    TypeQual getQualifiers(void) const;
    BaseType *getBase(void);

    Type *subType;

    Expression *size;

    //! Resource type of Array 
    ArrayResourceType resourceType;

    //! Indicates semantics are ok or not
    bool isSemanticsOk;

    //! groupSize is valid if array type is shared 
    int groupSize;
};

class BitFieldType:public Type
{
  public:
  BitFieldType(Expression * s = NULL):Type(TT_BitField), size(s), subType(NULL)
    {
    };

    ~BitFieldType();

    Type *dup0() const;         // deep-copy
    virtual Type **getSubType()
    {
        return &subType;
    }
    Type *extend(Type * extension);

    bool printStructureStreamHelperType(std::ostream & UNUSED(out), const std::string & UNUSED(name), bool UNUSED(raw)) const
    {
        return false;
    }

    void printBase(std::ostream & out, int level) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const;
    void printAfter(std::ostream & out) const;

    void printForm(std::ostream & out) const;

    void registerComponents()
    {
        if (subType)
            subType->registerComponents();
    }
    Decl *findComponent(Symbol * sym, u32 * offset)
    {
        if (subType)
            return subType->findComponent(sym, offset);
        return NULL;
    }

    void findExpr(fnExprCallback cb);

    bool lookup(Symbol * sym) const;

    TypeQual getQualifiers(void) const;
    BaseType *getBase(void);

    Expression *size;

    Type *subType;
};


class FunctionType:public Type
{
  public:
    FunctionType(Decl * args_list = NULL);
    ~FunctionType();

    Type *dup0() const;         // deep-copy

    Type *extend(Type * extension);
    virtual Type **getSubType()
    {
        return &subType;
    }

    bool printStructureStreamHelperType(std::ostream & UNUSED(out), const std::string & UNUSED(name), bool UNUSED(raw)) const
    {
        return false;
    }

    void printBase(std::ostream & out, int level) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const;
    void printAfter(std::ostream & out) const;

    void printForm(std::ostream & out) const;

    void registerComponents()
    {
        if (subType)
            subType->registerComponents();
    }
    Decl *findComponent(Symbol * sym, u32 * offset)
    {
        if (subType)
            return subType->findComponent(sym, offset);
        return NULL;
    }

    void insertArg(Decl * arg, u32 idx);
    void addArg(Decl * arg);
    void addArgs(Decl * args);

    Decl *findArg(const char *name);

    void findExpr(fnExprCallback cb);

    bool lookup(Symbol * sym) const;

    TypeQual getQualifiers(void) const;
    BaseType *getBase(void);

    bool KnR_decl;              // old-style function declaration?
    unsigned int nArgs;
    unsigned int size;
    Decl **args;

    Type *subType;              // The return type
};


class StreamType:public Type
{
  public:
    StreamType(Expression * s):Type(TT_Stream), subType(NULL), size(s)
    {
    };

    ~StreamType();
    virtual Type **getSubType()
    {
        return &subType;
    }
    Type *dup0() const;         // deep-copy

    Type *extend(Type * extension);

    void printBase(std::ostream & out, int level) const;
    void printBefore(std::ostream & out, Symbol * name, int level) const;
    void printAfter(std::ostream & out) const;

    void printForm(std::ostream & out) const;

    void registerComponents()
    {
        if (subType)
            subType->registerComponents();
    }
    Decl *findComponent(Symbol * sym, u32 * offset)
    {
        if (subType)
            return subType->findComponent(sym, offset);
        return NULL;
    }

    void findExpr(fnExprCallback cb);

    bool lookup(Symbol * sym) const;

    TypeQual getQualifiers(void) const;
    BaseType *getBase(void);
#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void allocateILStorage(bril::store * r) const
    {
        ((StreamType *) this)->getBase()->allocateILStorage(r);
    }
#endif

    Type *subType;

    Expression *size;
};


class INumList;

class GccAttrib
{
  public:
    GccAttrib(GccAttribType gccType);
    ~GccAttrib();

    GccAttrib *dup0() const;
    GccAttrib *dup() const;

    void print(std::ostream & out) const;

    GccAttribType type;

    uint value;                 // For use with GCC_Aligned
    Symbol *mode;               // For use with GCC_Mode, GCC_Format

    uint strIdx;                // For use with GCC_Format
    uint first;                 // For use with GCC_Format

    INumList *ns;               // For use with GCC_NonNull

    GccAttrib *next;            // Can be more than one per declaration
};


class Decl
{
  public:
    Decl(Symbol * sym = (Symbol *) NULL);
    Decl(Type * type);
    virtual ~ Decl();

    Type *extend(Type * type);

    bool isTypedef() const
    {
        return (storage == ST_Typedef);
    }
    bool isStatic() const
    {
        return (storage == ST_Static);
    }
    bool isReduce() const
    {
        return (form->getQualifiers() & TQ_Reduce) != 0;
    }
    bool isKernel() const
    {
        return form->isKernel();
    }
    bool isStream() const
    {
        return (form->type == TT_Stream || form->type == TT_BrtStream);
    }
    bool isArray() const
    {
        return (form->type == TT_Array);
    }

    void clear();

    Decl *dup0() const;
    Decl *dup() const;          // deep-copy

    void copy(const Decl & decl);       // shallow copy

    void print(std::ostream & out, bool showBase, int level = 0) const;
    void printExprWithTypeInfo(std::ostream& out);
    void printStructureStreamHelpers(std::ostream & out) const;
    bool printStructureStreamInternals(std::ostream & out, bool raw) const;
    bool printStructureStreamShape(std::ostream & out);
    bool printStructureStreamShapeInternals(std::ostream & out);
    void printBase(std::ostream & out, Symbol * name, bool showBase, int level) const;

#if BROOK_TO_IL_CODE_GEN_PATH
    virtual void print_il(std::ostream & out, int level = 0) const;
#endif //BROOK_TO_IL_CODE_GEN_PATH

    void findExpr(fnExprCallback cb);
    void replaceExpr(Expression * oldExpr, Expression * newExpr);
    void addInitializer(Expression *);

    bool lookup(Symbol * sym) const;

    StorageType storage;

    Type *form;                 // i.e., int *x[5]

    Symbol *name;               // The symbol being declared.

    GccAttrib *attrib;          // optional gcc attribute

    Expression *initializer;

    Decl *next;                 // For linking into lists
};



typedef std::vector < Decl * >DeclVector;



Decl *ReverseList(Decl * dList);



/*
 * FloatDimension --
 *
 *      Simple helper function to tell how many elements a FloatN type has
 *      fiven its TypeSpec.  Only annoying because type specs are bitmasks,
 *      not linear.
 */

static inline int FloatDimension(BaseTypeSpec bt)
{
    LOG(LOG_FN_ENTRY, " FloatDimension(%08x)\n", bt);
    if ((bt & BT_Float) || (bt & BT_Double) || (bt & BT_Int) || (bt & BT_Char) || (bt & BT_Short))
        return 1;
    else if ((bt & BT_Float2) || (bt & BT_Double2) || (bt & BT_Int2) || (bt & BT_Char2) || (bt & BT_Short2))
        return 2;
    else if ((bt & BT_Float3) || (bt & BT_Int3) || (bt & BT_Char3) || (bt & BT_Short3))
        return 3;
    else if ((bt & BT_Float4) || (bt & BT_Int4) || (bt & BT_Char4) || (bt & BT_Short4))
        return 4;
    else
        return 0;
}
static inline int FloatGPUDimension(BaseTypeSpec bt)
{
    LOG(LOG_FN_ENTRY, " FloatGPUDimension(%08x)\n", bt);
    if (bt & BT_Double)
        return 2;
    if (bt & BT_Double2)
        return 4;
    return FloatDimension(bt);
}

static inline bool isDouble(Type* type)
{
#define AsBaseType(t) (static_cast<BaseType*>(t))
        if (type && type->isBaseType())
        {
            BaseType* basetype = AsBaseType(type);
            return (basetype->typemask & BT_Double) || (basetype->typemask & BT_Double2);
        }
        return false;
#undef AsBaseType
}

extern BaseTypeInfo *getBaseTypeInfo(BaseTypeSpec intypemask);


// Added for the gcc non_null attribute. Deleting a node in this list deletes its tail also.

class INumList
{
public:
    int value;
    INumList *next;
    INumList(int a)
    {
        value = a;
        next = NULL;
    }
    ~INumList()
    {
        delete next;
    }
    
    INumList *dup() const
    {
        LOG(LOG_FN_ENTRY, "INumList::dup()\n");
        INumList *ret = this ? dup0() : NULL;

        return ret;
    }

    INumList *dup0() const
    {
        LOG(LOG_FN_ENTRY, "INumList::dup0()\n");
        INumList *ret = new INumList(value);
        if (next)
        {
	        ret->next = next->dup();
        }
        return ret;
    }
};


#endif /* INC_DECL_H */
