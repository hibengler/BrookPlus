// Copyright (c) 2003, Stanford University
// All rights reserved.

// Copyright (c) 2007, Advanced Micro Devices, Inc.
// All rights reserved.


// The BRCC portion of BrookGPU is derived from the cTool project
// (http://ctool.sourceforge.net) and distributed under the GNU Public License.

// Additionally, see LICENSE.ctool for information on redistributing the 
// contents of this directory.



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
    o+     File:         decl.cpp
    o+
    o+     Programmer:   Shaun Flisakowski
    o+     Date:         Aug 9, 1998
    o+
    o+     A high-level view of declarations.
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */
#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include <cassert>
#include <cstring>
#include <sstream>

#include "logger.hpp"

#include "main.h"
#include "decl.h"
#include "express.h"
#include "stemnt.h"

#include "token.h"
#include "gram.h"
#include "project.h"



//fixme: make "long double" and similar kind as one-word, instead of "long" + double ...
//fixed etc may not be useful ...

BaseTypeInfo allBaseTypeInfo[] = {
    {"void", BT_Void, NULL, NULL, NULL, 1, 0, false},
    {"char", BT_Char, "char", "__BrtChar1", "BR_FORMAT_CHAR_1", 1, 0, true},
    {"char2", BT_Char2, "char2", "__BrtChar2", "BR_FORMAT_CHAR_2", 2, 0, true},
    {"char3", BT_Char3, "char3", "__BrtChar3", "BR_FORMAT_CHAR_3", 3, 0, true},
    {"char4", BT_Char4, "char4", "__BrtChar4", "BR_FORMAT_CHAR_4", 4, 0, true},
    {"short", BT_Short, "short", "__BrtShort1", "BR_FORMAT_SHORT_1", 1, 0, true},
    {"short2", BT_Short2, "short2", "__BrtShort2", "BR_FORMAT_SHORT_2", 2, 0, true},
    {"short3", BT_Short3, "short3", "__BrtShort3", "BR_FORMAT_SHORT_3", 3, 0, true},
    {"short4", BT_Short4, "short4", "__BrtShort4", "BR_FORMAT_SHORT_4", 4, 0, true},
    {"int", BT_Int, "int", "__BrtInt1", "BR_FORMAT_INT_1", 1, 0, true},
    {"int2", BT_Int2, "int2", "__BrtInt2", "BR_FORMAT_INT_2", 2, 0, true},
    {"int3", BT_Int3, "int3", "__BrtInt3", "BR_FORMAT_INT_3", 3, 0, true},
    {"int4", BT_Int4, "int4", "__BrtInt4", "BR_FORMAT_INT_4", 4, 0, true},
    {"double", BT_Double, "float2", "__BrtDouble1", "BR_FORMAT_DOUBLE_1", 2, 0, true},
    {"double2", BT_Double2, "float4", "__BrtDouble2", "BR_FORMAT_DOUBLE_2", 4, 0, true},
    {"long", BT_Long, NULL, NULL, NULL, 1, 0, false},                           //this means "long int"
    {"...", BT_Ellipsis, NULL, NULL, NULL, 0, 0, false},
    {"float", BT_Float, "float", "__BrtFloat1", "BR_FORMAT_FLOAT_1", 1, 0, true},
    {"float2", BT_Float2, "float2", "__BrtFloat2", "BR_FORMAT_FLOAT_2", 2, 2, true},
    {"float3", BT_Float3, "float3", "__BrtFloat3", "BR_FORMAT_FLOAT_3", 3, 3, true},
    {"float4", BT_Float4, "float4", "__BrtFloat4", "BR_FORMAT_FLOAT_4", 4, 4, true},

    {NULL, 0, NULL, NULL, NULL, 0, 0, false}
};

BaseTypeInfo allUnsignedBaseTypeInfo[] = {
    {"uint", BT_Int, "int", "__BrtUInt1", "BR_FORMAT_UINT_1", 1, 0, true},
    {"uint2", BT_Int2, "int2", "__BrtUInt2", "BR_FORMAT_UINT_2", 2, 0, true},
    {"uint3", BT_Int3, "int3", "__BrtUInt3", "BR_FORMAT_UINT_3", 3, 0, true},
    {"uint4", BT_Int4, "int4", "__BrtUInt4", "BR_FORMAT_UINT_4", 4, 0, true},
    {"uchar", BT_Char, "char", "__BrtUChar1", "BR_FORMAT_UCHAR_1", 1, 0, true},
    {"uchar2", BT_Char2, "char2", "__BrtUChar2", "BR_FORMAT_UCHAR_2", 2, 0, true},
    {"uchar3", BT_Char3, "char3", "__BrtUChar3", "BR_FORMAT_UCHAR_3", 3, 0, true},
    {"uchar4", BT_Char4, "char4", "__BrtUChar4", "BR_FORMAT_UCHAR_4", 4, 0, true},
    {"ushort", BT_Short, "short", "__BrtUShort1", "BR_FORMAT_USHORT_1", 1, 0, true},
    {"ushort2", BT_Short2, "short2", "__BrtUShort2", "BR_FORMAT_USHORT_2", 2, 0, true},
    {"ushort3", BT_Short3, "short3", "__BrtUShort3", "BR_FORMAT_USHORT_3", 3, 0, true},
    {"ushort4", BT_Short4, "short4", "__BrtUShort4", "BR_FORMAT_USHORT_4", 4, 0, true},
    {NULL, 0, NULL, NULL, NULL, 0, 0}
};

//! unsupported builtin Double Math Operations
static const char* unsupportedBuiltinDoubleMathOps[] = {
        "abs",
        "acos",
        "asin",
        "clamp",
        "cos",
        "cos_vec",
        "cross",
        "exp",
        "floor",
        "fmod",
        "frac",
        "isfinite",
        "isinf",
        "isnan",
        "lerp",
        "log",
        "max",
        "min",
        "normalize",
        "pow",
        "rsqrt",
        "round",
        "sign",
        "sin",
        "sin_vec",
        "sqrt",
        "sqrt_vec",
        NULL
    };


//! unsupported builtin Double Math Operations
static const char* RelationalBuiltIns[] = {
        "all",
        "any",
        NULL
    };

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Finds wether given math operator supported for double data type.
//!        Returns true if it find functions name in unsupported list   
//!
////////////////////////////////////////////////////////////////////////////////
bool isSupportedDoubleMathOps(std::string funcName)
{
    unsigned int i = 0;
    while(unsupportedBuiltinDoubleMathOps[i] != NULL)
    {
        if(funcName.compare(unsupportedBuiltinDoubleMathOps[i]) == 0)
            return true;
        i++;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Return true if it is supported relational built in special fucntion
//!        
//!
//////////////////////////////////////////////////////////////////////////////// 
bool isRelationalBuiltIn(std::string funcName)
{
    unsigned int i = 0;
    while(RelationalBuiltIns[i] != NULL)
    {
        if(funcName.compare(RelationalBuiltIns[i]) == 0)
            return true;
        i++;
    }
    return false;
}

BaseTypeInfo *getBaseTypeInfo(BaseTypeSpec intypemask)
{
    LOG(LOG_FN_ENTRY, "getBaseTypeInfo(%08x)\n", intypemask);
    static bool init = false;
    static std::map < BaseTypeSpec, BaseTypeInfo * > baseType;
    static std::map < BaseTypeSpec, BaseTypeInfo * > unsignedbaseType;

    BaseTypeSpec typemask = (intypemask & BT_BaseMask);

    if (typemask == 0)
        return NULL;

    assert(((typemask - 1) & typemask) == 0);   //power of 2

    std::map < BaseTypeSpec, BaseTypeInfo * >::iterator it;
    if (init == false)
    {
        int i = 0;

        while (allBaseTypeInfo[i].brTypeName != NULL)
        {
            it = baseType.find(allBaseTypeInfo[i].brTypeID);
            assert(it == baseType.end());
            baseType[allBaseTypeInfo[i].brTypeID] = &allBaseTypeInfo[i];
            i++;
        }

        i = 0;
        while (allUnsignedBaseTypeInfo[i].brTypeName != NULL)
        {
            it = unsignedbaseType.find(allUnsignedBaseTypeInfo[i].brTypeID);
            assert(it == unsignedbaseType.end());
            unsignedbaseType[allUnsignedBaseTypeInfo[i].brTypeID] = &allUnsignedBaseTypeInfo[i];
            i++;
        }

        init = true;
    }

    if ( (intypemask & BT_UnSigned) != 0 )
    {
        it = unsignedbaseType.find(typemask);
        if (it == unsignedbaseType.end())
            return NULL;
    }
    else
    {
        it = baseType.find(typemask);
        if (it == baseType.end())
            return NULL;
    }



    return (*it).second;
}

BTDetails getTypeDetails(BaseTypeSpec baseTypeID)
{
    BaseTypeInfo *typeInfo = getBaseTypeInfo(baseTypeID);
    BTDetails typeDetails;

    if(BT_NoType == baseTypeID)
    {
        typeDetails.category = BT_Non_Group;
        typeDetails.components = 1;
        assert(0);
        LOG(LOG_ERROR, "Invalid situation: requesting details of base type having no type");
        return typeDetails;
    }
    else if(BT_Void == baseTypeID)
    {
        typeDetails.category = BT_Void_Group;
        typeDetails.components = 1;
        LOG(LOG_ERROR, "Invalid situation: requesting details of base type having void type");
        return typeDetails;
    }
    else if(BT_Struct & baseTypeID)
    {
        typeDetails.category = BT_Struct_Group;
        typeDetails.components = 1;   //! components have to meaning
        return typeDetails;
    }
    else if(BT_UserType & baseTypeID)
    {
        typeDetails.category = BT_UserType_Group;
        typeDetails.components = 1; //! components have to meaning
        return typeDetails;
    }
    else if(BT_Union & baseTypeID)
    {
        typeDetails.category = BT_Union_Group;
        typeDetails.components = 1; //! components have to meaning
        assert(0);
        LOG(LOG_ERROR, "required to handle union Types ");
        return typeDetails;
    }
    else if(BT_Enum & baseTypeID)
    {
        typeDetails.category = BT_Enum_Group;
        typeDetails.components = 1; //! components have to meaning
        assert(0);
        LOG(LOG_ERROR, "required to handle enum Types ");
        return typeDetails;
    }
    else
    {
        if(BT_Double == baseTypeID || BT_Double2 == baseTypeID)
            typeDetails.components = typeInfo->component / 2;
        else
            typeDetails.components = typeInfo->component;
    }

    //! Set category of Base type
    if(BT_Void & baseTypeID)
    {
        typeDetails.category = BT_Void_Group;
    }
    else if(BT_Char & baseTypeID || BT_Char2 & baseTypeID || BT_Char3 & baseTypeID || BT_Char4 & baseTypeID)
    {
        if(BT_UnSigned & baseTypeID)
            typeDetails.category = BT_UChar_Group;
        else
            typeDetails.category = BT_Char_Group;
    }
    else if(BT_Short & baseTypeID || BT_Short2 & baseTypeID || BT_Short3 & baseTypeID || BT_Short4 & baseTypeID)
    {
        if(BT_UnSigned & baseTypeID)
            typeDetails.category = BT_UShort_Group;
        else
            typeDetails.category = BT_Short_Group;
    }
    else if(BT_Int & baseTypeID || BT_Int2 & baseTypeID || BT_Int3 & baseTypeID || BT_Int4 & baseTypeID)
    {
        if(BT_UnSigned & baseTypeID)
            typeDetails.category = BT_UInt_Group;
        else
            typeDetails.category = BT_Int_Group;
    }
    else if(BT_Float & baseTypeID || BT_Float2 & baseTypeID || BT_Float3 & baseTypeID || BT_Float4 & baseTypeID)
    {
        typeDetails.category = BT_Float_Group;
    }
    else if(BT_Double & baseTypeID || BT_Double2 & baseTypeID)
    {
        typeDetails.category = BT_Double_Group;
    }
    else if(BT_Struct & baseTypeID || BT_Union & baseTypeID || BT_Enum & baseTypeID || BT_UserType & baseTypeID)
    {
        typeDetails.category = BT_UserType_Group;
    }
    else
    {
        assert(0);
        LOG(LOG_ERROR, "Unknown Base type ID");
    }

    return typeDetails;
}


std::string getScalarTypeStr(BTCategory category)
{
    std::string scalarStr = "";
    switch(category)
    {
        case BT_Char_Group:     //! Integer groups
            scalarStr.append("char");
            break;
        case BT_UChar_Group:
            scalarStr.append("unsigned char");  
            break;
        case BT_Short_Group:
            scalarStr.append("short");
            break;
        case BT_UShort_Group:
            scalarStr.append("unsigned short");
            break;
        case BT_Int_Group:
            scalarStr.append("int");
            break;
        case BT_UInt_Group:
            scalarStr.append("unsigned int");
            break;
        case BT_Float_Group:    //! Real groups
            scalarStr.append("float");
            break;
        case BT_Double_Group:
            scalarStr.append("double");
            break;
        default:
            assert(0);
            LOG(LOG_ERROR, "Internal error :%s, %d : Unsupported type category", __FILE__, __LINE__);

    }

    return scalarStr;
}
BinaryOperandsDetails getBinaryOperandsDetails(BaseTypeSpec leftBaseTypeID, BaseTypeSpec rightBaseTypeID)
{
    BinaryOperandsDetails details;

    details.leftOperandDetails = getTypeDetails(leftBaseTypeID);
    details.rightOperandDetails = getTypeDetails(rightBaseTypeID);

    //! Set operands category
    if(details.leftOperandDetails.category == details.rightOperandDetails.category)
    {
        if(details.leftOperandDetails.components == details.rightOperandDetails.components)
        {
            details.category = BT_SameTypes_EqualComponents;
        }
        else
        {
            details.category = BT_SameTypes_UnequalComponents;
        }
    }
    else
    {
        if(details.leftOperandDetails.components == details.rightOperandDetails.components)
        {
            details.category = BT_DiffTypes_EqualComponents;
        }
        else
        {
            details.category = BT_DiffTypes_UnequalComponents;
        }
    }

    return details;
}

//! Returns true and fills the warning message for given binary operands details
//!  and assuming left operand is a destination type
bool getWarnMsgForBinarayOperands(BinaryOperandsDetails details, std::string &warnMsg)
{
    bool isThereWarning = false;
    switch(details.category)
    {
        //! For Different types 
        case BT_DiffTypes_UnequalComponents:
            //! type warnings
            if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
            {
                warnMsg.append(" : possible lose of data");
                isThereWarning = true;
            }
            if(details.leftOperandDetails.components > details.rightOperandDetails.components)
            {
                warnMsg.append(" and uninitialize components");
            }
            else
            {
                warnMsg.append(" and lose of commponents");
            }
            break;
        case BT_DiffTypes_EqualComponents:
            //! Type warnings
            if(checkForConversionLose(details.leftOperandDetails, details.rightOperandDetails))
            {
                warnMsg.append(" : possible lose of data");
                isThereWarning = true;
            }
            break;
        //! For Same types having unequal components
        case BT_SameTypes_UnequalComponents:
            //! Components warnings
            if(details.leftOperandDetails.components > details.rightOperandDetails.components)
            {
                warnMsg.append(": uninitialize components");
                isThereWarning = true;
            }
            else
            {
                warnMsg.append(": lose of commponents");
                isThereWarning = true;
            }
            break;
        case BT_SameTypes_EqualComponents:
            //! do nothing
            break;
    }

    return isThereWarning;
}

//! Returns true if type or components are matched
bool checkForTypeAndComponents(BTDetails leftOperand, BTDetails rightOperand)
{
    if(leftOperand.category != rightOperand.category || leftOperand.components != rightOperand.components)
        return false;
    return true;
}
//! Returns true if there is lose of data while converting actual type to promotion Type
bool checkForConversionLose(BTDetails promotionType, BTDetails actualType)
{
    switch(promotionType.category)
    {
        case BT_Char_Group: 
        case BT_UChar_Group:
            if(actualType.category != BT_Char_Group && actualType.category != BT_UChar_Group)
                return true;
            break;
        case BT_Short_Group:
        case BT_UShort_Group:
            if(actualType.category == BT_Int_Group || actualType.category == BT_UInt_Group
                || promotionType.category == BT_Float_Group || promotionType.category == BT_Double_Group)
                return true;
            break;
        case BT_Int_Group:
        case BT_UInt_Group:
            if(actualType.category == BT_Float_Group || actualType.category == BT_Double_Group)
                return true;
            break;
        case BT_Long_Group:
            assert(0);
            LOG(LOG_ERROR, "Internal error :%s, %d : Unsupported type category", __FILE__, __LINE__);
            break;
        case BT_Float_Group: 
            if(actualType.category == BT_Int_Group || actualType.category == BT_UInt_Group
                || actualType.category == BT_Double_Group)
                return true;
            break;
        case BT_Double_Group:
            break;
        default:
            assert(0);
            LOG(LOG_ERROR, "Internal error :%s, %d : Unknown type category", __FILE__, __LINE__);
    }      
    return false;
}

//! Returns -1 if left operand type required to be converted to right operand type
//! Returns 0(Zero) if no promotions required
//! Returns 1 if right operand type required to be converted to left operand type
int requiredTypePromotion(BTDetails leftOperand, BTDetails rightOperand)
{
    if(leftOperand.category != rightOperand.category)
    {
        if(leftOperand.category == BT_Double_Group || rightOperand.category == BT_Double_Group)
        {
            if(leftOperand.category == BT_Double_Group)
                return 1;
        }
        else if(leftOperand.category == BT_Float_Group || rightOperand.category == BT_Float_Group)
        {
            if(leftOperand.category == BT_Float_Group)
                return 1;
        }
        else if(leftOperand.category == BT_UInt_Group || rightOperand.category == BT_UInt_Group)
        {
            if(leftOperand.category == BT_UInt_Group)
                return 1;
        }
        else if(leftOperand.category == BT_Int_Group || rightOperand.category == BT_Int_Group)
        {
            if(leftOperand.category == BT_Int_Group)
                return 1;
        }
        else if(leftOperand.category == BT_UShort_Group || rightOperand.category == BT_UShort_Group)
        {
            if(leftOperand.category == BT_UShort_Group)
                return 1;
        }
        else if(leftOperand.category == BT_Short_Group || rightOperand.category == BT_Short_Group)
        {
            if(leftOperand.category == BT_Short_Group)
                return 1;
        }
        else if(leftOperand.category == BT_UChar_Group || rightOperand.category == BT_UChar_Group)
        {
            if(leftOperand.category == BT_UChar_Group)
                return 1;
        }
        else if(leftOperand.category == BT_Char_Group && rightOperand.category == BT_Char_Group)
        {
            return 0;
        }
        else if(leftOperand.category == BT_Void_Group || rightOperand.category == BT_Void_Group)
        {
            //! For void, return 0 just to pass compilation
            return 0;
        }
        else
        {
            assert(0);
            LOG(LOG_ERROR, "unsupported type category");
            return 2;
        }
    }
    else
    {
        //! No promotion required if both are same type but having sign bit 
        return 0;
    }

    return -1;
}

//! Returns true if given category is a integer type
bool isIntegerType(BTDetails baseTypeDetails)
{
    if(baseTypeDetails.category >= BT_Char_Group && baseTypeDetails.category < BT_Float_Group)
        return true;
    return false;
}

//! Returns true if given category is a Real type 
bool isRealType(BTDetails baseTypeDetails)
{
    if(baseTypeDetails.category >= BT_Float_Group && baseTypeDetails.category < BT_UserType_Group)
        return true;
    return false;
}

//! Returns true if base type is either integer type or real type
bool isArithmeticType(BTDetails baseTypeDetails)
{
    if(baseTypeDetails.category >= BT_Char_Group && baseTypeDetails.category < BT_UserType_Group)
        return true;
    return false;
}

//! Returns true if given type details hav one component
bool isScalarType(BTDetails baseTypeDetails)
{
    if(baseTypeDetails.components == 1)
        return true;
    return false;

}

std::string currentKernelName = "?";
std::string getCurrentKernelName()
{
    return currentKernelName;
}
void setCurrentKernelName(std::string kernelName)
{
    currentKernelName = kernelName;
}
void resetCurrentKernelName()
{
    currentKernelName = "?";
}

//! Returns true if given type mask is 128 bit base data type
bool is128bitBaseType(BaseTypeSpec typeMask)
{
    if(typeMask & BT_Float4 || typeMask & BT_Double2 || typeMask & BT_Int4 || typeMask & BT_Char4 || typeMask & BT_Short4)
        return true;
    return false;
}
const char *getBaseTypeName(BaseTypeSpec intypemask, BaseTypeNameKind kind, bool * pIsComplex)
{
    LOG(LOG_FN_ENTRY, "getBaseTypeName(%08x, %d)\n", intypemask, kind);
    BaseTypeInfo *typeInfo = getBaseTypeInfo(intypemask);

    if (pIsComplex)
    {
        if (typeInfo)
            *pIsComplex = typeInfo->isComplexType;
        else
            *pIsComplex = false;
    }

    if (typeInfo)
    {
        switch (kind)
        {
        case BaseTypeBRName:
            return typeInfo->brTypeName;
        case BaseTypeSHName:
            return typeInfo->shTypeName;
        case BaseTypeRTName:
            return typeInfo->rtTypeName;
        case BaseTypeRTEnumName:
            return typeInfo->rtTypeIDStr;
        default:
            fprintf(stderr, "Internal error: getBaseTypeName() called for unknown kind %d.\n", kind);
            exit(-1);
        }
    }

    return NULL;
}

//! Returns the mask value for given baseType name
//! Retruns int mask value if given name is not a base type name
extern BaseTypeSpec getBaseTypeSpec(std::string baseTypeName)
{
    BaseTypeSpec typeMask;
    
    bool found = false;
    for(int i = 0; allBaseTypeInfo[i].brTypeName != NULL; ++i)
    {
        if(baseTypeName.compare(allBaseTypeInfo[i].brTypeName) == 0)
        {
            typeMask = allBaseTypeInfo[i].brTypeID;
            found = true;
            break;
        }
    }

    if(found == false)
    {
        for(int i = 0; allUnsignedBaseTypeInfo[i].brTypeName != NULL; ++i)
        {
            if(baseTypeName.compare(allUnsignedBaseTypeInfo[i].brTypeName) == 0)
            {
                typeMask = allUnsignedBaseTypeInfo[i].brTypeID;
                typeMask = typeMask | BT_UnSigned;
                found = true;
                break;
            }
        }
    }

    if(found == false)
    {
        assert(0);
        LOG(LOG_ERROR, "Internal error: %s, %d: unsupported base type name %s", __FILE__, __LINE__, baseTypeName.c_str());
        return BT_Int;
    }

    return typeMask;
}

static void printStorage(std::ostream & out, StorageType storage)
{
    LOG(LOG_FN_ENTRY, "printStorage(%p, %d)\n", &out, storage);
    switch (storage)
    {
    case ST_None:
        break;

    case ST_Typedef:
        out << "typedef ";
        break;

    case ST_Auto:
        //! print only for cpu backend
        if(globals.getPresentTarget() == TARGET_CPU)
            out << "auto ";
        break;

    case ST_Register:
        //! print only for cpu backend
        if(globals.getPresentTarget() == TARGET_CPU)
            out << "register ";
        break;

    case ST_Static:
        out << "static ";
        break;

    case ST_Extern:
        out << "extern ";
        break;
    }
}


static void printQual(std::ostream & out, TypeQual qualifier)
{
    LOG(LOG_FN_ENTRY, "printQual(%p, %08x)\n", &out, qualifier);
// TIM: complete HACK to make the iterator test case work
// I can't figure out how to remove qualifiers from
// printType, though
//    if ((qualifier & TQ_Iter)!=0)
//        out << "iter ";

    if ((qualifier & TQ_Out) != 0)
        out << "out ";

    if ((qualifier & TQ_Const) != 0)
        out << "const ";

    if ((qualifier & TQ_Volatile) != 0)
        out << "volatile ";
}


Type::Type(TypeType _type /* =TT_Base */ )
{
    LOG(LOG_FN_ENTRY, "Type::Type(%08x)\n", _type);
    type = _type;
    storage = ST_None;

    //! Set this to true in kernel definition while doing 
    //! semantic check on kernel parameter declarations
    kernelParam = false; 

    // Add us into the global list for destruction later.
    link = gProject->typeList;
    gProject->typeList = this;
}


Type::~Type()
{
    LOG(LOG_FN_ENTRY, "Type::~Type()\n");
    // assert(false);
}


void Type::DeleteTypeList(Type * typeList)
{
    LOG(LOG_FN_ENTRY, "Type::DeleteTypeList(%p)\n", typeList);
    Type *prev = NULL;
    Type *curr = typeList;

    while (curr != NULL)
    {
        if (prev != NULL)
            delete prev;

        prev = curr;
        curr = curr->link;
    }

    if (prev != NULL)
        delete prev;
}


void Type::printType(std::ostream & out, Symbol * name, bool showBase, int level, bool raw) const
{
    LOG(LOG_FN_ENTRY, "Type::printType(%p, %p, %d, %d, %d)\n", &out, name, showBase, level, raw);
    if (showBase)
    {
        if (raw)
        {
            printRawBase(out, level);
        }
        else
        {
            printBase(out, level);
        }
        if (name != NULL)
            out << " ";
    }
    printBefore(out, name, level);
    printAfter(out);
}



bool Type::isBuitinStreamType()
{
    LOG(LOG_FN_ENTRY, "Type::isBuitinStreamType()\n");
    if (type == TT_Stream)
        return ((ArrayType *) this)->subType->isBuitinStreamElementType();

    return false;
}


BaseType::BaseType(BaseTypeSpec bt /* =BT_NoType */ )
    :  Type(TT_Base)
{
    LOG(LOG_FN_ENTRY, "BaseType::BaseType(%08x)\n", bt);
    typemask = bt;
    qualifier = TQ_None;

    tag = NULL;
    typeName = NULL;
    stDefn = NULL;
    enDefn = NULL;
}

BaseType::BaseType(StructDef * sd):Type(TT_Base)
{
    LOG(LOG_FN_ENTRY, "BaseType::BaseType(%p)\n", sd);
    typemask = (sd->isUnion())? BT_Union : BT_Struct;
    qualifier = TQ_None;

    tag = sd->tag->dup();

    typeName = NULL;
    stDefn = sd;
    enDefn = NULL;
}

BaseType::BaseType(EnumDef * ed):Type(TT_Base)
{
    LOG(LOG_FN_ENTRY, "BaseType::BaseType(%p)\n", ed);
    typemask = BT_Enum;
    qualifier = TQ_None;

    tag = ed->tag->dup();
    typeName = NULL;
    stDefn = NULL;
    enDefn = ed;
}

BaseType::~BaseType()
{
    LOG(LOG_FN_ENTRY, "BaseType::~BaseType()\n");
    delete tag;
    delete typeName;
    delete stDefn;
    delete enDefn;
}


Type *BaseType::dup0() const
{
    LOG(LOG_FN_ENTRY, "BaseType::dup0()\n");
    BaseType *ret = new BaseType();

    ret->storage = storage;
    ret->qualifier = qualifier;
    ret->typemask = typemask;

    ret->tag = tag->dup();
    ret->typeName = typeName->dup();
    ret->stDefn = stDefn->dup();
    ret->enDefn = enDefn->dup();

    return ret;
}


void
Type::tokenize(const std::string& str, std::vector<std::string>& tokens, 
                  const std::string& delimiters) const
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

Type *Type::getRealType() const
{
    LOG(LOG_FN_ENTRY, "Type::getRealType()\n");
    Type *result = (Type *) this;

    while (1)
    {
        if (result->type == TT_Base && (((BaseType *) result)->typemask & BT_UserType))
            result = ((BaseType *) result)->typeName->entry->uVarDecl->form;
        else if (result->type == TT_Array || result->type == TT_Stream)
            result = ((Type *) this)->getBase();
        else
            break;
    }
    assert(result);
    return result;
}
#if BROOK_TO_IL_CODE_GEN_PATH
void Type::allocateILStorage(bril::store * r) const
{
    LOG(LOG_FN_ENTRY, "Type::allocateILStorage(%p)\n", r);
    assert((type == TT_Stream || type == TT_Array) && ((Type *) this)->getBase());

    ((Type *) this)->getBase()->allocateILStorage(r);
}

void BaseType::allocateILStorage(bril::store * r) const
{
    LOG(LOG_FN_ENTRY, "BaseType::allocateILStorage(%p)\n");
    Type *real = getRealType();
    BaseTypeSpec t = real && real->isBaseType()? ((BaseType *) real)->typemask : typemask;

    if (t == BT_Double)
        bril::allocate_store(r, 2);
    else if (t == BT_Double2)   
        bril::allocate_store(r, 4);
    else if (t <= BT_Float || t == BT_Fixed || t == BT_ShortFixed || t == BT_Int)
        bril::allocate_store(r, 1);
    else if (t == BT_Float2 || t == BT_Fixed2 || t == BT_ShortFixed2 || t == BT_Int2)   // FIXME - forcing short-fixed to the same size as fixed for now
        bril::allocate_store(r, 2);
    else if (t == BT_Float3 || t == BT_Fixed3 || t == BT_ShortFixed3 || t == BT_Int3)
        bril::allocate_store(r, 3);
    else if (t == BT_Float4 || t == BT_Fixed4 || t == BT_ShortFixed4 || t == BT_Int4)
        bril::allocate_store(r, 4);
    else if (t == BT_Struct)
    {
        StructDef *sd = findStructureDef(real);

        if (!sd)
            assert(0);

        for (u32 i = 0; i < u32(sd->nComponents); ++i)
        {
            bril::store * elt = alloc_struct_elt(r, i);
            sd->components[i]->form->allocateILStorage(elt);
        }
    }
    else
        assert(0);
}

#endif //BROOK_TO_IL_CODE_GEN_PATH
bool BaseType::printStructureStreamHelperType(std::ostream & out, const std::string & name, bool raw) const
{
    LOG(LOG_FN_ENTRY, "BaseType::printStructureStreamHelperType(%p, %s, %d)\n", &out, name.c_str(), raw);
    printQual(out, qualifier);

    if (typemask & BT_Struct)
    {
        if (stDefn != NULL)
        {
            if (!stDefn->printStructureStreamHelper(out, raw))
                return false;
        }
        else
        {
            out << "struct ";

            if (tag)
                out << (raw ? "__castablestruct_" : "__cpustruct_") << *tag << " ";
        }
    }
    else if (typemask & BT_Union)
    {
        return false;
    }
    else if (typemask & BT_Enum)
    {
        return false;
    }
    else if (typemask & BT_UserType)
    {
        if (typeName)
            out << (raw ? "__castablestruct_" : "__cpustruct_") << *typeName << " ";
    }
    else
    {
        bool iscomplex;
        const char *outstr = getBaseTypeName(typemask, BaseTypeRTName, &iscomplex);

        if (iscomplex == false)
        {
            if (typemask & BT_UnSigned)
                out << "unsigned ";
            else if (typemask & BT_Signed)
                out << "signed ";
        }

        if (outstr == NULL)
        {
            outstr = getBaseTypeName(typemask, BaseTypeBRName);
            //out << "ERROR the following data type can't be stream element type: " 
            //    << getBaseTypeName(typemask, BaseTypeBRName) << std::endl;
            assert(outstr);
        }
        out << outstr << " ";
        //out << "int ";        // Default
    }
    out << name;
    return true;
}

static Symbol *findStructureTag(Type const *inType)
{
    LOG(LOG_FN_ENTRY, "findStructureTag(%p)\n", inType);
    BaseType const *base = ((Type *) inType)->getBase();

    while (true)
    {
        BaseTypeSpec mask = base->typemask;

        if (mask & BT_UserType)
        {
            base = base->typeName->entry->uVarDecl->form->getBase();
        }
        else if (mask & BT_Struct)
            return base->tag;
        else
            break;
    }
    return NULL;
}

StructDef *findStructureDef(Type const *inType)
{
    LOG(LOG_FN_ENTRY, "findStructureDef(%p)\n", inType);
    Symbol *tag = findStructureTag(inType);

    if (tag == NULL)
        return NULL;
    return tag->entry->uStructDef->stDefn;
}

bool BaseType::isBuitinStreamElementType() const
{
    LOG(LOG_FN_ENTRY, "BaseType::isBuitinStreamElementType()\n");
    if (typeName)
        return typeName->entry->uVarDecl->form->isBuitinStreamElementType();

    return ((typemask & BT_Float) || (typemask & BT_Float2) ||
            (typemask & BT_Float3) || (typemask & BT_Float4) ||
            (typemask & BT_Int) || (typemask & BT_Int2) ||
            (typemask & BT_Int3) || (typemask & BT_Int4) ||
            (typemask & BT_Char) || (typemask & BT_Char2) ||
            (typemask & BT_Char3) || (typemask & BT_Char4) ||
            (typemask & BT_Short) || (typemask & BT_Short2) ||
            (typemask & BT_Short3) || (typemask & BT_Short4) ||
            (typemask & BT_Double) || (typemask & BT_Double2));

}

bool BaseType::printStructureStreamShape(std::ostream & out)
{
    LOG(LOG_FN_ENTRY, "BaseType::printStructureStreamShape(%p)\n", &out);
    if (typemask & BT_Struct)
    {
        StructDef *s = findStructureDef(this);

        if (s == NULL)
            return false;
        s->printStructureStreamShape(out);
    }
    else
    {
        const char *outstr = getBaseTypeName(typemask, BaseTypeRTEnumName);

        if (outstr == NULL)
        {
            //out << "ERROR the following data type can't be stream element type: " 
            //    << getBaseTypeName(typemask, BaseTypeBRName) << std::endl;
            //assert(0);
            return false;
        }
        out << outstr << ";" << std::endl;
        //out << "int ";        // Default
    }

    return true;
}

void BaseType::printBase(std::ostream & out, int level) const
{
    LOG(LOG_FN_ENTRY, "BaseType::printBase(%p, %d)\n", &out, level);
    printQual(out, qualifier);

    if (typemask & BT_Struct)
    {
        if (stDefn != NULL)
        {
            stDefn->print(out, NULL, level);
        }
        else
        {
            out << "struct ";

            if (tag)
                out << *tag << " ";
        }
    }
    else if (typemask & BT_Union)
    {
        if (stDefn != NULL)
        {
            stDefn->print(out, NULL, level);
        }
        else
        {
            out << "union ";

            if (tag)
                out << *tag << " ";
        }
    }
    else if (typemask & BT_Enum)
    {
        out << "enum ";
        if (enDefn != NULL)
        {
            enDefn->print(out, NULL, level);
        }
        else
        {
            if (tag)
                out << *tag << " ";
        }
    }
    else if (typemask & BT_UserType)
    {
        if (typeName)
            out << *typeName << " ";
    }
    else
    {
        bool iscomplex;
        const char *outstr = getBaseTypeName(typemask, BaseTypeBRName, &iscomplex);

        if (iscomplex == false)
        {
            if (typemask & BT_UnSigned)
                out << "unsigned ";
            else if (typemask & BT_Signed)
                out << "signed ";
        }

        //assert(outstr);
        if (outstr)
            out << outstr << " ";
        //out << "int ";        // Default
    }
}

int BaseType::getNumComponent()
{
    LOG(LOG_FN_ENTRY, "BaseType::getNumComponent()\n");
    BaseTypeInfo *info = getBaseTypeInfo(typemask);

    return info->component;
}


void BaseType::printBefore(std::ostream & out, Symbol * name, int level) const
{
    LOG(LOG_FN_ENTRY, "BaseType::printBefore(%p, %d, %d)\n", &out, name, level);
    if (name)
    {
        out << *name;
    }
}

void BaseType::printAfter(std::ostream &out) const
{
    LOG(LOG_FN_ENTRY, "BaseType::printAfter(%p)\n", &out);
}

void BaseType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BaseType::printForm(%p)\n", &out);
    out << "-Base";
    if (qualifier != TQ_None)
    {
        out << ":";
        printQual(out, qualifier);
    }
}


void BaseType::registerComponents()
{
    LOG(LOG_FN_ENTRY, "BaseType::registerComponents()\n");
    if ((typemask & BT_Struct) | (typemask & BT_Union))
    {
        stDefn->registerComponents();
    }
}


Decl *BaseType::findComponent(Symbol * sym, u32 * offset)
{
    LOG(LOG_FN_ENTRY, "BaseType::findComponent(%p, %p)\n", sym, offset);
    if (getRealType() != this)
        return getRealType()->findComponent(sym, offset);

    if ((typemask & BT_Struct) | (typemask & BT_Union))
        return stDefn->findComponent(sym, offset);

    return NULL;
}


bool BaseType::lookup(Symbol * sym) const
{
    LOG(LOG_FN_ENTRY, "BaseType::lookup(%p)\n", sym);
    if ((typemask & BT_Struct) || (typemask & BT_Union))
    {
        if (stDefn != NULL)
        {
            return stDefn->lookup(sym);
        }
    }
    else if (typemask & BT_UserType)
    {
        if (typeName)
        {
            SymEntry *typeEntry = typeName->entry;

            if (typeEntry)
            {
                return typeEntry->uVarDecl->lookup(sym);
            }
        }
    }

    return false;
}


TypeQual BaseType::getQualifiers(void) const
{
    LOG(LOG_FN_ENTRY, "BaseType::getQualifiers()\n");
    return qualifier;
}


BaseType *BaseType::getBase(void)
{
    LOG(LOG_FN_ENTRY, "BaseType::getBase()\n");
    return this;
}


Type *PtrType::dup0() const
{
    LOG(LOG_FN_ENTRY, "PtrType::dup0()\n");
    PtrType *ret = new PtrType(qualifier);

    ret->subType = subType->dup();
    ret->storage = storage;

    return ret;
}


Type *PtrType::extend(Type * extension)
{
    LOG(LOG_FN_ENTRY, "PtrType::extend(%p)\n", extension);
    if (subType)
        return subType->extend(extension);

    subType = extension;
    return this;
}


void PtrType::printBase(std::ostream & out, int level) const
{
    LOG(LOG_FN_ENTRY, "PtrType::printBase(%p, %d)\n", &out, level);
    if (subType)
        subType->printBase(out, level);
}


void PtrType::printBefore(std::ostream & out, Symbol * name, int level) const
{
    LOG(LOG_FN_ENTRY, "PtrType::printBefore(%p, %p, %d)\n", &out, name, level);
    if (subType)
    {
        subType->printBefore(out, NULL, level);

        bool paren = !(subType->isPointer() || subType->isBaseType());

        if (paren)
            out << "(";

        out << "*";
        printQual(out, qualifier);

    }

    if (name)
    {
        out << *name;
    }
}

void PtrType::printAfter(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "PtrType::printAfter(%p)\n", &out);
    if (subType)
    {
        bool paren = !(subType->isPointer() || subType->isBaseType());

        if (paren)
            out << ")";

        subType->printAfter(out);
    }
}

void PtrType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "PtrType::printForm(%p)\n", &out);
    out << "-Ptr";
    if (qualifier != TQ_None)
    {
        out << ":";
        printQual(out, qualifier);
    }
    if (subType)
        subType->printForm(out);
}


void PtrType::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "PtrType::findExpr(%p)\n", cb);
    if (subType)
        subType->findExpr(cb);
}


bool PtrType::lookup(Symbol * sym) const
{
    LOG(LOG_FN_ENTRY, "PtrType::lookup(%p)\n", sym);
    if (subType)
        return subType->lookup(sym);
    else
        return false;
}


TypeQual PtrType::getQualifiers(void) const
{
    LOG(LOG_FN_ENTRY, "PtrType::getQualifiers()\n");
    return qualifier;
}


BaseType *PtrType::getBase(void)
{
    LOG(LOG_FN_ENTRY, "PtrType::getBase()\n");
    return subType->getBase();
}


ArrayType::ArrayType(TypeType t, Expression * s, ArrayResourceType resType)
    :Type(t), subType(NULL), size(s), resourceType(resType), isSemanticsOk(false)
{
    LOG(LOG_FN_ENTRY, "ArrayType::ArrayType(%08x, %p)\n", t, s);
    assert(t == TT_Stream || t == TT_Array);
    groupSize = 0;
    if (size)
        size->setParent(this);
}


ArrayType::~ArrayType()
{
    LOG(LOG_FN_ENTRY, "ArrayType::~ArrayType()\n");
    // Handled by deleting the global type list
    // delete subType;
    delete size;
}


Type *ArrayType::dup0() const
{
    LOG(LOG_FN_ENTRY, "ArrayType::dup0()\n");
    ArrayType *ret = new ArrayType(type, size->dup(), resourceType);

    ret->isSemanticsOk = isSemanticsOk;
    ret->subType = subType->dup();
    ret->storage = storage;
    ret->groupSize = groupSize;

    return ret;
}


Type *ArrayType::extend(Type * extension)
{
    LOG(LOG_FN_ENTRY, "ArrayType::extend(%p)\n", extension);
    if (subType)
        return subType->extend(extension);
    subType = extension;
    return this;
}


void ArrayType::printBase(std::ostream & out, int level) const
{
    LOG(LOG_FN_ENTRY, "ArrayType::printBase(%p, %d)\n", &out, level);
    if (subType)
        subType->printBase(out, level);
}


void ArrayType::printBefore(std::ostream & out, Symbol * name, int level) const
{
    LOG(LOG_FN_ENTRY, "ArrayType::printBefore(%p, %p, %d)\n", &out, name, level);
    if (subType)
        subType->printBefore(out, name, level);
}

void ArrayType::printAfter(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "ArrayType::printAfter(%p)\n", &out);

    if (type != TT_BrtStream)
    {

        out << (type == TT_Array ? "[" : "<");

        if (size)
            size->print(out);

        out << (type == TT_Array ? "]" : ">");
    }

    if (subType)
        subType->printAfter(out);
}

void ArrayType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "ArrayType::printForm(%p)\n", &out);
    out << (type == TT_Array ? "-Array[" : "-Stream<");
    if (size)
        size->print(out);
    out << (type == TT_Array ? "]" : ">");
    if (subType)
        subType->printForm(out);
}


void ArrayType::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "ArrayType::findExpr(%p)\n", cb);
    if (subType)
        subType->findExpr(cb);

    if (size)
    {
        //size = (cb)(size);
        size->findExpr(cb);
    }
}


bool ArrayType::lookup(Symbol * sym) const
{
    LOG(LOG_FN_ENTRY, "ArrayType::lookup(%p)\n", sym);
    if (subType)
        return subType->lookup(sym);
    else
        return false;
}
//! Returns the array dimension count
//! Returns 1 for 1D array
int ArrayType::getDimensions() 
{
    ArrayType *arrayType = AsArrayType(this);
    int arrayDim = 0;
    while(arrayType->type == TT_Array)
    {
        arrayDim++;
        arrayType = AsArrayType(arrayType->subType);
        assert(arrayType);
    }
    assert(arrayDim);
    return arrayDim;
}

//! Returns size of given dimension
//! dim = 1 means leftmost index size
int ArrayType::getDimensionsSize(int dim)
{
    ArrayType *arrayType = AsArrayType(this);
    int elementCount = 0;
    int arrayDim = 0;
    while(arrayType->type == TT_Array)
    {
        arrayDim++;
        if(arrayType->size->etype == ET_Constant)
        {
            Constant *constant = static_cast<Constant *>(arrayType->size);
            if(constant->ctype == CT_Int)
            {
                IntConstant *intConst = static_cast<IntConstant *>(constant);
                elementCount = intConst->lng;
            }
        }
        if(arrayDim == dim)
            break;

        arrayType = AsArrayType(arrayType->subType);
        assert(arrayType);
    }
	assert(arrayDim);
	assert(elementCount);
    return elementCount;
}

//! Returns total number of elements
//! Returns zero if any dimension value is not set
int ArrayType::getTotalElementCount()
{
    ArrayType *arrayType = AsArrayType(this);
    int elementCount = 0;
    int arrayDim = 0;
    while(arrayType->type == TT_Array)
    {
        arrayDim++;
        if(arrayType->size == NULL)
            return 0;
        if(arrayType->size->etype == ET_Constant)
        {
            Constant *constant = static_cast<Constant *>(arrayType->size);
            if(constant->ctype == CT_Int)
            {
                IntConstant *intConst = static_cast<IntConstant *>(constant);
                if(arrayDim == 1)
                    elementCount = intConst->lng;
                else
                    elementCount = elementCount * intConst->lng;
            }
        }
        arrayType = AsArrayType(arrayType->subType);
        assert(arrayType);
    }
	assert(arrayDim);
	assert(elementCount);
    return elementCount;
}

int ArrayType::getGroupSize()
{
    if(groupSize == 0)
        return 1;
    return groupSize;
}

TypeQual ArrayType::getQualifiers(void) const
{
    LOG(LOG_FN_ENTRY, "ArrayType::getQualifiers()\n");
    return subType->getQualifiers();
}


BaseType *ArrayType::getBase(void)
{
    LOG(LOG_FN_ENTRY, "ArrayType::getBase()\n");
    return subType->getBase();
}


BitFieldType::~BitFieldType()
{
    LOG(LOG_FN_ENTRY, "BitFieldType::~BitFieldType()\n");
    // Handled by deleting the global type list
    // delete subType;
    delete size;
}

Type *BitFieldType::extend(Type * extension)
{
    LOG(LOG_FN_ENTRY, "BitFieldType::extend(%p)\n", extension);
    if (subType)
        return subType->extend(extension);

    subType = extension;
    return this;
}


Type *BitFieldType::dup0() const
{
    LOG(LOG_FN_ENTRY, "BitFieldType::dup0()\n");
    BitFieldType *ret = new BitFieldType(size->dup());

    ret->storage = storage;

    ret->subType = subType->dup();

    return ret;
}


void BitFieldType::printBase(std::ostream & out, int level) const
{
    LOG(LOG_FN_ENTRY, "BitFieldType::printBase(%p, %d)\n", &out, level);
    if (subType)
    {
        subType->printBase(out, level);
    }
}


void BitFieldType::printBefore(std::ostream & out, Symbol * name, int level) const
{
    LOG(LOG_FN_ENTRY, "BitFieldType::printBefore(%p, %p, %d)\n", &out, name, level);
    if (subType)
    {
        subType->printBefore(out, NULL, level);
    }

    if (name)
    {
        out << *name;
    }

    out << ":";

    if (size)
    {
        size->print(out);
    }
}

void BitFieldType::printAfter(std::ostream &out) const
{
    LOG(LOG_FN_ENTRY, "BitFieldType::printAfter(%p)\n", &out);
}

void BitFieldType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "BitFieldType::printForm(%p)\n", &out);
    out << "-BitField";
    if (subType)
        subType->printForm(out);
}


void BitFieldType::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "BitFieldType::findExpr(%p)\n", cb);
    if (size)
    {
        //size = (cb)(size);
        size->findExpr(cb);
    }
}


bool BitFieldType::lookup(Symbol * sym) const
{
    LOG(LOG_FN_ENTRY, "BitFieldType::lookup(%p)\n");
    if (subType)
        return subType->lookup(sym);
    else
        return false;
}


TypeQual BitFieldType::getQualifiers(void) const
{
    LOG(LOG_FN_ENTRY, "BitFieldType::getQualifiers()\n");
    return subType->getQualifiers();
}


BaseType *BitFieldType::getBase(void)
{
    LOG(LOG_FN_ENTRY, "BitFieldType::getBase()\n");
    return subType->getBase();
}


FunctionType::FunctionType(Decl * args_list):Type(TT_Function), KnR_decl(false), nArgs(0),
					     size(0), args(NULL), subType(NULL)
{
    LOG(LOG_FN_ENTRY, "FunctionType::FunctionType(%p)\n", args_list);
    addArgs(args_list);
}

FunctionType::~FunctionType()
{
    LOG(LOG_FN_ENTRY, "FunctionType::~FunctionType()\n");
    for (unsigned int j = 0; j < nArgs; j++)
    {
        delete args[j];
    }

    delete[]args;

    // Handled by deleting the global type list
    // delete subType;
}


Type *FunctionType::dup0() const
{
    LOG(LOG_FN_ENTRY, "FunctionType::dup0()\n");
    FunctionType *ret = new FunctionType();

    ret->storage = storage;
    ret->size = size;
    ret->args = new Decl *[size];
    ret->KnR_decl = KnR_decl;

    for (unsigned int j = 0; j < nArgs; j++)
        ret->addArg(args[j]->dup());

    ret->subType = subType->dup();

    return ret;
}


Type *FunctionType::extend(Type * extension)
{
    LOG(LOG_FN_ENTRY, "FunctionType::extend(%p)\n", extension);
    if (subType)
        return subType->extend(extension);

    subType = extension;
    return this;
}


void FunctionType::printBase(std::ostream & out, int level) const
{
    LOG(LOG_FN_ENTRY, "FunctionType::printBase(%p, %d)\n", &out, level);
    if (subType)
    {
        subType->printBase(out, level);
    }
}


void FunctionType::printBefore(std::ostream & out, Symbol * name, int level) const
{
    LOG(LOG_FN_ENTRY, "FunctionType::printBefore(%p, %p, %d)\n", &out, name, level);
    if (subType)
    {
        subType->printBefore(out, name, level);
    }
    else if (name)
    {
        out << *name;
    }
}

void FunctionType::printAfter(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "FunctionType::printAfter(%p)\n", &out);
    if (KnR_decl)
    {
        out << "(";

        if (nArgs > 0)
        {
            out << *(args[0]->name);
            for (unsigned int j = 1; j < nArgs; j++)
            {
                out << ", ";
                out << *(args[j]->name);
            }
        }

        out << ")\n";

        for (unsigned int j = 0; j < nArgs; j++)
        {
            args[j]->print(out, true);
            out << ";\n";
        }
    }
    else
    {
        out << "(";

        if (nArgs > 0)
        {
            args[0]->print(out, true);
            for (unsigned int j = 1; j < nArgs; j++)
            {
                out << ", ";
                args[j]->print(out, true);
            }
        }

        out << ")";
    }

    if (subType)
    {
        subType->printAfter(out);
    }
}

void FunctionType::printForm(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "FunctionType::printForm(%p)\n", &out);
    out << "-Function";
    if (subType)
        subType->printForm(out);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void FunctionType::insertArg(Decl * arg, u32 idx)
{
    LOG(LOG_FN_ENTRY, "FunctionType::insertArg(%p, %d)\n", arg, idx);
    assert(idx <= nArgs);

    if (size == nArgs)
    {
        if (size == 0)
            size = 4;
        else
            size += size;

        Decl **oldArgs = args;

        args = new Decl *[size];

        for (unsigned int j = 0; j < nArgs; j++)
            args[j] = oldArgs[j];

        delete[]oldArgs;
    }

    for (u32 i = nArgs; i > idx; --i)
        args[i] = args[i - 1];

    args[idx] = arg;
    nArgs++;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void FunctionType::addArg(Decl * arg)
{
    LOG(LOG_FN_ENTRY, "FunctionType::addArg(%p)\n", arg);
    insertArg(arg, nArgs);
}


void FunctionType::addArgs(Decl * args)
{
    LOG(LOG_FN_ENTRY, "FunctionType::addArgs(%p)\n", args);
    Decl *arg = args;

    while (args != NULL)
    {
        args = args->next;
        arg->next = NULL;
        addArg(arg);
        arg = args;
    }
}


Decl *FunctionType::findArg(const char *inname)
{
    LOG(LOG_FN_ENTRY, "FunctionType::findArg(%s)\n", inname);
    std::string name = inname;

    for (unsigned int i = 0; i < nArgs; ++i)
    {
        if (args[i]->name->name == name)
        {
            return args[i];
        }
    }

    return NULL;
}


void FunctionType::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "FunctionType::findExpr(%p)\n", cb);
    if (subType)
        subType->findExpr(cb);

    for (unsigned int j = 0; j < nArgs; j++)
    {
        args[j]->findExpr(cb);
    }
}


bool FunctionType::lookup(Symbol * sym) const
{
    LOG(LOG_FN_ENTRY, "FunctionType::lookup(%p)\n", sym);
    if (subType)
        return subType->lookup(sym);
    else
        return false;
}


TypeQual FunctionType::getQualifiers(void) const
{
    LOG(LOG_FN_ENTRY, "FunctionType::getQualifiers()\n");
    return subType->getQualifiers();
}


BaseType *FunctionType::getBase(void)
{
    LOG(LOG_FN_ENTRY, "FunctionType::getBase()\n");
    return subType->getBase();
}


StructDef::StructDef(bool _is_union /* =false */ )
{
    LOG(LOG_FN_ENTRY, "StructDef::StructDef(%d)\n", _is_union);
    _isUnion = _is_union;
    tag = NULL;
    size = 0;
    nComponents = 0;

    components = NULL;
}

StructDef::~StructDef()
{
    LOG(LOG_FN_ENTRY, "StructDef::~StructDef()\n");
    delete tag;

    for (int j = 0; j < nComponents; j++)
    {
        delete components[j];
    }

    delete[]components;
}


Decl *StructDef::lastDecl()
{
    LOG(LOG_FN_ENTRY, "StructDef::lastDecl()\n");
    return components[nComponents - 1];
}


StructDef *StructDef::dup() const
{
    LOG(LOG_FN_ENTRY, "StructDef::dup()\n");
    StructDef *ret = this ? dup0() : NULL;

    return ret;
}

StructDef *StructDef::dup0() const
{
    LOG(LOG_FN_ENTRY, "StructDef::dup0()\n");
    StructDef *ret = new StructDef();

    ret->size = size;
    ret->_isUnion = _isUnion;
    ret->components = new Decl *[size];

    for (int j = 0; j < nComponents; j++)
        ret->addComponent(components[j]->dup());

    ret->tag = tag->dup();

    return ret;
}


void StructDef::print(std::ostream & out, Symbol * name, int level) const
{
    LOG(LOG_FN_ENTRY, "StructDef::print(%p, %p, %d)\n", &out, name, level);
    if (isUnion())
        out << "union ";
    else
        out << "struct ";

    if (tag)
        out << *tag << " ";

    out << "{\n";

    for (int j = 0; j < nComponents; j++)
    {
        indent(out, level + 1);
        components[j]->print(out, true, level + 1);

        Decl *decl = components[j]->next;

        while (decl != NULL)
        {
            out << ", ";
            decl->print(out, false, level + 1);
            decl = decl->next;
        }

        out << ";\n";
    }

    indent(out, level);
    out << "}";

    if (name)
        out << " " << *name;
}

bool StructDef::printStructureStreamHelper(std::ostream & out, bool raw) const
{
    LOG(LOG_FN_ENTRY, "StructDef::printStructureStreamHelper(%p, %d)\n", &out, raw);
    if (isUnion())
        out << "union ";
    else
        out << "struct ";

    if (tag)
        out << (raw ? "__castablestruct_" : "__cpustruct_") << *tag << " ";

    out << "{\n";

    for (int j = 0; j < nComponents; j++)
    {
        if (!components[j]->printStructureStreamInternals(out, raw))
            return false;

        Decl *decl = components[j]->next;

        while (decl != NULL)
        {
            out << ", ";
            if (!decl->printStructureStreamInternals(out, raw))
                return false;
            decl = decl->next;
        }

        out << ";\n";
    }

    out << "}\n";
    return true;
}

bool StructDef::printStructureStreamShape(std::ostream & out)
{
    LOG(LOG_FN_ENTRY, "StructDef::printStructureStreamShape(%p)\n", &out);

    out << "\t\tcount = " << nComponents << ";" << std::endl;
    out << "\t\tBRformat* formats = new BRformat[count];" << std::endl;

    for (int j = 0; j < nComponents; j++)
    {
        out << "\t\tformats[" << j << "] = ";
        if (!components[j]->printStructureStreamShapeInternals(out))
            return false;

        Decl *decl = components[j]->next;

        while (decl != NULL)
        {
            if (!decl->printStructureStreamShapeInternals(out))
                return false;
            decl = decl->next;
        }
    }
    return true;
}


void StructDef::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "StructDef::findExpr(%p)\n", cb);
    for (int j = 0; j < nComponents; j++)
    {
        components[j]->findExpr(cb);
    }
}


void StructDef::addComponent(Decl * comp)
{
    LOG(LOG_FN_ENTRY, "StructDef::addComponent(%p)\n", comp);
    if (size == nComponents)
    {
        if (size == 0)
            size = 4;
        else
            size += size;

        Decl **oldComps = components;

        components = new Decl *[size];

        for (int j = 0; j < nComponents; j++)
        {
            components[j] = oldComps[j];
        }

        delete[]oldComps;
    }

    components[nComponents] = comp;
    nComponents++;

    do
    {
        // Hook this component's symtable entry back to here.
        if ((comp->name != NULL) && (comp->name->entry != NULL))
        {
            comp->name->entry->uComponent = comp;
            comp->name->entry->u2Container = this;
            // The entry was inserted by gram.y as a ComponentEntry.
            assert(comp->name->entry->IsComponentDecl());
        }
        comp = comp->next;
    }
    while (comp);
}


void StructDef::registerComponents()
{
    LOG(LOG_FN_ENTRY, "StructDef::registerComponents()\n");
    int j;

    for (j = 0; j < nComponents; j++)
    {
        Decl *decl = components[j];

        while (decl != NULL)
        {
            Symbol *ident = decl->name;

            if (ident != NULL)
            {
                ident->entry =
                    gProject->Parse_TOS->transUnit->contxt.syms->Insert(mk_component(ident->name, decl, this));
            }

            // Register any sub-components also.
            decl->form->registerComponents();

            decl = decl->next;
        }
    }
}

Decl *StructDef::findComponent(Symbol * sym, u32 * offset)
{
    LOG(LOG_FN_ENTRY, "StructDef::findComponent(%p, %p)\n", sym, offset);
    for (int j = 0; j < nComponents; j++)
    {
        Decl *decl = components[j];

        if (decl && decl->name->name == sym->name)
        {
            *offset = j;
            return decl;
        }

    }
    return NULL;
}


bool StructDef::lookup(Symbol * sym) const
{
    LOG(LOG_FN_ENTRY, "StructDef::lookup(%p)\n", sym);
    int j;

    for (j = 0; j < nComponents; j++)
    {
        Decl *decl = components[j];

        while (decl != NULL)
        {
            Symbol *ident = decl->name;

            if (ident->name == sym->name)
            {
                sym->entry = ident->entry;

                return true;
            }

            decl = decl->next;
        }
    }

    return false;
}


EnumDef::EnumDef()
{
    LOG(LOG_FN_ENTRY, "EnumDef::EnumDef()\n");
    tag = NULL;
    size = 0;
    nElements = 0;

    names = NULL;
    values = NULL;
}

EnumDef::~EnumDef()
{
    LOG(LOG_FN_ENTRY, "EnumDef::~EnumDef()\n");
    delete tag;

    for (int j = 0; j < nElements; j++)
    {
        delete names[j];
        delete values[j];
    }

    delete[]names;
    delete[]values;
}


void EnumDef::addElement(Symbol * nme, Expression * val /* =NULL */ )
{
    LOG(LOG_FN_ENTRY, "EnumDef::addElement(%p, %p)\n", nme, val);
    if (size == nElements)
    {
        if (size == 0)
            size = 4;
        else
            size += size;

        Symbol **oldNames = names;
        Expression **oldVals = values;

        names = new Symbol *[size];
        values = new Expression *[size];

        for (int j = 0; j < nElements; j++)
        {
            names[j] = oldNames[j];
            values[j] = oldVals[j];
        }

        delete[]oldNames;
        delete[]oldVals;
    }

    names[nElements] = nme;
    values[nElements] = val;
    nElements++;

    if ((nme->entry != NULL) && (nme->entry->type == EnumConstEntry))
    {
        assert(nme->entry->type == EnumConstEntry);
        nme->entry->uEnumValue = val;
        nme->entry->u2EnumDef = this;
    }
    if (val)
        val->setParent(this);
}


void EnumDef::addElement(EnumConstant * ec)
{
    LOG(LOG_FN_ENTRY, "EnumDef::addElement(%p)\n", ec);
    addElement(ec->name, ec->value);

    ec->name = NULL;
    ec->value = NULL;

    delete ec;
}


EnumDef *EnumDef::dup() const
{
    LOG(LOG_FN_ENTRY, "EnumDef::dup()\n");
    EnumDef *ret = this ? dup0() : NULL;

    return ret;
}

EnumDef *EnumDef::dup0() const
{
    LOG(LOG_FN_ENTRY, "EnumDef::dup0()\n");
    EnumDef *ret = new EnumDef();

    ret->size = size;
    ret->names = new Symbol *[size];
    ret->values = new Expression *[size];

    for (int j = 0; j < nElements; j++)
    {
        Expression *val = values[j]->dup();

        ret->addElement(names[j]->dup(), val);
    }

    ret->tag = tag->dup();

    return ret;
}


void EnumDef::print(std::ostream & out, Symbol *dummy, int level) const
{
    LOG(LOG_FN_ENTRY, "EnumDef::print(%p, %p, %d)\n", &out, dummy, level);
    if (tag)
    {
        out << *tag << " ";
    }

    out << "{ ";

    if (nElements > 0)
    {
        out << *names[0];

        if (values[0])
        {
            out << " = ";
            values[0]->print(out);
        }

        for (int j = 1; j < nElements; j++)
        {
            out << ", ";
            out << *names[j];

            if (values[j])
            {
                out << " = ";
                values[j]->print(out);
            }
        }
    }

    out << " }";
}


void EnumDef::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "EnumDef::findExpr(%p)\n", cb);
    for (int j = 0; j < nElements; j++)
    {
        if (values[j] != NULL)
        {
            //values[j] = (cb)(values[j]);
            values[j]->findExpr(cb);
        }
    }
}


GccAttrib::GccAttrib(GccAttribType gccType)
{
    LOG(LOG_FN_ENTRY, "GccAttrib::GccAttrib(%d)\n", gccType);
    type = gccType;
    value = 0;
    mode = NULL;
    next = NULL;
    ns = NULL;
}

GccAttrib::~GccAttrib()
{
    LOG(LOG_FN_ENTRY, "GccAttrib::~GccAttrib()\n");
    delete next;
    delete mode;
    delete ns;
}


void GccAttrib::print(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "GccAttrib::print(%p)\n", &out);
    if(type  == Kernel_GroupSize)
    {
        out << "Attribute((";
        out << "GroupSize(";

        if(ns)
        {
            std::vector<int> values;
            INumList* tempList = ns;
            while(tempList)
            {
                values.push_back(tempList->value);
                tempList = tempList->next;
            }

            for(unsigned int i = 0; i < values.size() - 1; ++i)
            {
                out << values[values.size() - i - 1];
                out << ", ";
            }
            out << values[0];
        }
        out << ")";
        out << "))";

    }
    else
    {
        out << " __attribute__ ((";

        switch (type)
        {
        case GCC_Aligned:
            out << "aligned (" << value << ")";
            break;

        case GCC_Packed:
            out << "packed";
            break;

        case GCC_CDecl:
            out << "__cdecl__";
            break;

        case GCC_Mode:
            out << "__mode__ (" << *mode << ")";
            break;

        case GCC_Format:
            out << "format (" << *mode << "," << strIdx << "," << first << ")";
            break;

        case GCC_Const:
            out << "__const__";
            break;

        case GCC_NoReturn:
            out << "__noreturn__";
            break;

        case GCC_Unsupported:
        default:
            out << "<unsupported gcc attribute>";
            break;
        }

        out << "))";
    }
    if (next)
    {
	next->print(out);
    }
}


GccAttrib *GccAttrib::dup() const
{
    LOG(LOG_FN_ENTRY, "GccAttrib::dup()\n");
    GccAttrib *ret = this ? dup0() : NULL;

    return ret;
}

GccAttrib *GccAttrib::dup0() const
{
    LOG(LOG_FN_ENTRY, "GccAttrib::dup0()\n");
    GccAttrib *ret = new GccAttrib(type);

    ret->value = value;
    ret->mode = mode->dup();
    if (next)
    {
	ret->next = next->dup();
    }

    if(ns)
    {
        ret->ns = ns->dup();
    }

    return ret;
}


Decl::Decl(Symbol * sym /* =NULL */ )
{
    LOG(LOG_FN_ENTRY, "Decl::Decl(%p)\n", sym);
    clear();

    name = sym;
}

Decl::Decl(Type * type)
{
    LOG(LOG_FN_ENTRY, "Decl::Decl(%p)\n", type);
    clear();

    form = type;
    storage = type->storage;
    name = NULL;
}

BaseType & BaseType::operator =(const BaseType & b)
{
    LOG(LOG_FN_ENTRY, "BaseType::operator =(%p)\n", &b);
    this->typemask = b.typemask;
    this->qualifier = b.qualifier;
    this->tag = b.tag ? b.tag->dup() : NULL;
    this->typeName = b.typeName ? b.typeName->dup() : NULL;
    this->stDefn = b.stDefn ? b.stDefn->dup() : NULL;
    this->enDefn = b.enDefn ? b.enDefn->dup() : NULL;
    return *this;
}


Decl::~Decl()
{
    LOG(LOG_FN_ENTRY, "Decl::~Decl()\n");
    // Handled by deleting the global type list
    // delete form;
    delete attrib;
    delete initializer;
}


void Decl::clear()
{
    LOG(LOG_FN_ENTRY, "Decl::clear()\n");
    storage = ST_None;

    name = NULL;
    form = NULL;
    attrib = NULL;
    initializer = NULL;
    next = NULL;
}


Type *Decl::extend(Type * type)
{
    LOG(LOG_FN_ENTRY, "Decl::extend(%p)\n", type);
    if (storage == ST_None)
        storage = type->storage;

    if (form != NULL)
        return form->extend(type);

    form = type;
    return NULL;
}


Decl *Decl::dup() const
{
    LOG(LOG_FN_ENTRY, "Decl::dup()\n");
    Decl *ret = this ? dup0() : NULL;

    return ret;
}

Decl *Decl::dup0() const
{
    LOG(LOG_FN_ENTRY, "Decl::dup0()\n");
    Decl *ret = new Decl();

    ret->storage = storage;
    ret->form = form->dup();    //do copy so that cpu implementation can be above

    ret->name = name->dup();
    ret->attrib = attrib->dup();
    ret->initializer = initializer->dup();
    if (ret->initializer)
        ret->initializer->setParent(ret);
    ret->next = next->dup();

    return ret;
}

void Decl::addInitializer(Expression * init)
{
    LOG(LOG_FN_ENTRY, "Decl::addInitializer(%p)\n", init);
    initializer = init;
    if (init)
        init->setParent(this);
}

void Decl::replaceExpr(Expression * oldExp, Expression * newExp)
{
    LOG(LOG_FN_ENTRY, "Decl::replaceExpr(%p, %p)\n", oldExp, newExp);
    assert(oldExp == initializer);
    initializer = newExp;
    if (newExp)
        newExp->setParent(this);
}


void Decl::copy(const Decl & decl)
{
    LOG(LOG_FN_ENTRY, "Decl::copy(%p)\n", &decl);
    storage = decl.storage;
    name = decl.name;
    form = decl.form;
    attrib = decl.attrib;
    initializer = decl.initializer;
    if (initializer)
        initializer->setParent(this);
}


void Decl::print(std::ostream & out, bool showBase, int level) const
{
    LOG(LOG_FN_ENTRY, "Decl::print(%p, %d, %d)\n", &out, showBase, level);
    assert(this != NULL);

    if (showBase)
    {
        printStorage(out, storage);

        // Hack to fix K&R non-declarations
        if (form == NULL)
        {
            out << "int ";
        }
    }

    if (form)
    {
        form->printType(out, name, showBase, level, false);
    }
    else if (name)
    {
        out << *name;
    }

    if (attrib)
    {
        attrib->print(out);
    }

    if (initializer)
    {
        out << " = ";

        initializer->print(out);
    }

    /*
      if (!form->isBaseType())
      {
      out << "  [FORM: ";
      form->printForm(out);
      out << " ]";
      }
    */
}

void Decl::printExprWithTypeInfo(std::ostream& out)
{
    if(initializer)
    {
        if (form)
        {
			out << "\n\tDeclaration: ";
            form->printType(out, name, false, 0, false);
            out << ", Type : ";
            if(form && form->getBase())
            {
                BaseType* baseType = form->getBase();
                BaseTypeInfo* baseTypeInfo = getBaseTypeInfo(baseType->typemask);
                out << baseTypeInfo->brTypeName << std::endl;
            }
        }
        initializer->printExprWithTypeInfo(out);
    }
}

static std::string PrintCastToBody(StructDef * str)
{
    LOG(LOG_FN_ENTRY, "PrintCastToBody(%p)\n", str);
    if (!str)
    {
        return "    return *this;";
    }
    std::string ret = "    T ret;\n";
    for (int i = 0; i < str->nComponents; ++i)
    {
        ret +=
            "    ret." + str->components[i]->name->name + " = this->" + str->components[i]->name->name +
            ".castToArg(ret." + str->components[i]->name->name + ");\n";
    }
    return ret += "    return ret;";
}

void Decl::printStructureStreamHelpers(std::ostream & out) const
{
    LOG(LOG_FN_ENTRY, "Decl::printStructureStreamHelpers(%p)\n", &out);
    assert(this != NULL);

    if (!isTypedef())
        return;

    for (int i = 0; i < 2; ++i)
    {
        std::ostringstream stringout;
        stringout << "\ntypedef ";
        if (!form->
            printStructureStreamHelperType(stringout,
                                           (i ==
                                            0 ? std::string("__cpustruct_") : std::string("__castablestruct_")) +
                                           name->name, i != 0))
            return;
        stringout << ";\n";
        std::string tmp = stringout.str();
        size_t where = tmp.find("{");
        if (where != std::string::npos)
        {
            tmp =
                tmp.substr(0,
                           where + 1) + "\n  template <typename T> T castToArg(const T& dummy)const{\n" +
                PrintCastToBody(form->getBase()->stDefn) + "\n  }\n" + tmp.substr(where + 1);
        }
        out << tmp;
    }
}

bool Decl::printStructureStreamInternals(std::ostream & out, bool raw) const
{
    LOG(LOG_FN_ENTRY, "Decl::printStructureStreamInternals(%p, %d)\n", &out, raw);
    assert(this != NULL);

    if (true)
    {
        printStorage(out, storage);

        // Hack to fix K&R non-declarations
        if (form == NULL)
        {
            out << "int ";
        }
    }

    if (form)
    {
        if (!form->printStructureStreamHelperType(out, name->name, raw))
            return false;
    }
    else if (name)
    {
        out << *name;
    }

    if (attrib)
    {
        attrib->print(out);
    }

    if (initializer)
    {
        out << " = ";

        initializer->print(out);
    }

    return true;
}

bool Decl::printStructureStreamShape(std::ostream & out)
{
    LOG(LOG_FN_ENTRY, "Decl::printStructureStreamShape(%p)\n", &out);
    assert(this != NULL);

    if (!isTypedef() || form->isBuitinStreamElementType())
        return false;

    std::ostringstream stringout;

    stringout << "\nnamespace brook {\n";
    stringout << "\ttemplate<> inline BRformat* getStreamType(";
    stringout << name->name << "*, unsigned int& count) {\n";
    if (!form->printStructureStreamShape(stringout))
        return false;
    stringout << "\t\treturn formats;\n";
    stringout << "\t}\n}\n";
    out << stringout.str();
    return true;
}

bool Decl::printStructureStreamShapeInternals(std::ostream & out)
{
    LOG(LOG_FN_ENTRY, "Decl::printStructureStreamShapeInternals(%p)\n", &out);
    if (isTypedef())
        return true;
    if ((storage & ST_Static) != 0)
        return true;

    if (form)
    {
        if (!form->printStructureStreamShape(out))
            return false;
    }
    return true;
}

#if BROOK_TO_IL_CODE_GEN_PATH
void Decl::print_il(std::ostream &out, int level) const
{
    LOG(LOG_FN_ENTRY, "Decl::print_il(%p, %d)\n", &out, level);
    // If there's no initializer there's nothin' to do...
    if (initializer)
    {
        assert(name);
        if (initializer->etype == ET_Constant && ((Constant *) initializer)->ctype == CT_Array)
        {
            // ArrayConstants are untyped...
            assert(name->entry && name->entry->IsVarDecl() && name->entry->uVarDecl && name->entry->uVarDecl->form);

            ((ArrayConstant *) initializer)->print_il(out, name->entry->uVarDecl->form->getRealType());
        }
        else
            initializer->print_il(out);

        emitCopy(out, name->get_il_store(), initializer->get_il_store());
    }
}
#endif // BROOK_TO_IL_CODE_GEN_PATH

void Decl::findExpr(fnExprCallback cb)
{
    LOG(LOG_FN_ENTRY, "Decl::findExpr(%p)\n", cb);
    if (form)
        form->findExpr(cb);

    if (initializer != NULL)
    {
        //initializer = (cb)(initializer);
        initializer->findExpr(cb);
    }
}


bool Decl::lookup(Symbol * sym) const
{
    LOG(LOG_FN_ENTRY, "Decl::lookup(%p)\n", sym);
    if (form)
        return form->lookup(sym);
    else
        return false;
}


Decl *ReverseList(Decl * dList)
{
    LOG(LOG_FN_ENTRY, "ReverseList(%p)\n", dList);
    Decl *head = NULL;

    while (dList != NULL)
    {
        Decl *dcl = dList;

        dList = dList->next;

        dcl->next = head;
        head = dcl;
    }

    return head;
}


