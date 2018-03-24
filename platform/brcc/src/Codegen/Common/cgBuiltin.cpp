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

#include "logger.hpp"
#include "cgBuiltin.h"
#include "cgTool.h"

// ----------------------------------------------------------------------------

//! All builtin function supported by brook
//! Add here new builtin function 
static const char*  builtinFunctions[] = {
                            "float sign(float a)",
                            "float2 sign(float2 a)",
                            "float3 sign(float3 a)",
                            "float4 sign(float4 a)",
                            "int sign(int a)",
                            "int2 sign(int2 a)",
                            "int3 sign(int3 a)",
                            "int4 sign(int4 a)",
                            "uint sign(uint a)",
                            "uint2 sign(uint2 a)",
                            "uint3 sign(uint3 a)",
                            "uint4 sign(uint4 a)",
                            "char sign(char a)",
                            "char2 sign(char2 a)",
                            "char3 sign(char3 a)",
                            "char4 sign(char4 a)",
                            "uchar sign(uchar a)",
                            "uchar2 sign(uchar2 a)",
                            "uchar3 sign(uchar3 a)",
                            "uchar4 sign(uchar4 a)",
                            "short sign(short a)",
                            "short2 sign(short2 a)",
                            "short3 sign(short3 a)",
                            "short4 sign(short4 a)",
                            "ushort sign(ushort a)",
                            "ushort2 sign(ushort2 a)",
                            "ushort3 sign(ushort3 a)",
                            "ushort4 sign(ushort4 a)",
                            "double sign(double a)",
                            "double2 sign(double2 a)",
                            "float abs(float a)",
                            "float2 abs(float2 a)",
                            "float3 abs(float3 a)",
                            "float4 abs(float4 a)",
                            "int abs(int a)",
                            "int2 abs(int2 a)",
                            "int3 abs(int3 a)",
                            "int4 abs(int4 a)",
                            "char abs(char a)",
                            "char2 abs(char2 a)",
                            "char3 abs(char3 a)",
                            "char4 abs(char4 a)",
                            "short abs(short a)",
                            "short2 abs(short2 a)",
                            "short3 abs(short3 a)",
                            "short4 abs(short4 a)",
                            "double abs(double a)",
                            "double2 abs(double2 a)",
                            "float exp(float a)",
                            "float2 exp(float2 a)",
                            "float3 exp(float3 a)",
                            "float4 exp(float4 a)",
                            "double exp(double a)",
                            "double2 exp(double2 a)",
                            "float pow(float a, float b)",
                            "float2 pow(float2 a, float2 b)",
                            "float3 pow(float3 a, float3 b)",
                            "float4 pow(float4 a, float4 b)",
                            "double pow(double a, double b)",
                            "double2 pow(double2 a, double2 b)",
                            "float log(float a)",
                            "float2 log(float2 a)",
                            "float3 log(float3 a)",
                            "float4 log(float4 a)",
                            "double log(double a)",
                            "double2 log(double2 a)",
                            "float sqrt(float a)",
                            "float2 sqrt(float2 a)",
                            "float3 sqrt(float3 a)",
                            "float4 sqrt(float4 a)",
                            "double sqrt(double a)",
                            "double2 sqrt(double2 a)",
                            "float rsqrt(float a)",
                            "float2 rsqrt(float2 a)",
                            "float3 rsqrt(float3 a)",
                            "float4 rsqrt(float4 a)",
                            "double rsqrt(double a)",
                            "double2 rsqrt(double2 a)",
                            "float cos(float a)",
                            "float2 cos(float2 a)",
                            "float3 cos(float3 a)",
                            "float4 cos(float4 a)",
                            "double cos(double a)",
                            "double2 cos(double2 a)",
                            "float sin(float a)",
                            "float2 sin(float2 a)",
                            "float3 sin(float3 a)",
                            "float4 sin(float4 a)",
                            "double sin(double a)",
                            "double2 sin(double2 a)",
                            "float acos(float a)",
                            "float2 acos(float2 a)",
                            "float3 acos(float3 a)",
                            "float4 acos(float4 a)",
                            "double acos(double a)",
                            "double2 acos(double2 a)",
                            "float asin(float a)",
                            "float2 asin(float2 a)",
                            "float3 asin(float3 a)",
                            "float4 asin(float4 a)",
                            "double asin(double a)",
                            "double2 asin(double2 a)",
                            "float isfinite(float a)",
                            "float2 isfinite(float2 a)",
                            "float3 isfinite(float3 a)",
                            "float4 isfinite(float4 a)",
                            "double isfinite(double a)",
                            "double2 isfinite(double2 a)",
                            "float isinf(float a)",
                            "float2 isinf(float2 a)",
                            "float3 isinf(float3 a)",
                            "float4 isinf(float4 a)",
                            "double isinf(double a)",
                            "double2 isinf(double2 a)",
                            "float isnan(float a)",
                            "float2 isnan(float2 a)",
                            "float3 isnan(float3 a)",
                            "float4 isnan(float4 a)",
                            "double isnan(double a)",
                            "double2 isnan(double2 a)",
                            "float frac(float a)",
                            "float2 frac(float2 a)",
                            "float3 frac(float3 a)",
                            "float4 frac(float4 a)",
                            "double frac(double a)",
                            "double2 frac(double2 a)",
                            "float floor(float a)",
                            "float2 floor(float2 a)",
                            "float3 floor(float3 a)",
                            "float4 floor(float4 a)",
                            "double floor(double a)",
                            "double2 floor(double2 a)",
                            "float round(float a)",
                            "float2 round(float2 a)",
                            "float3 round(float3 a)",
                            "float4 round(float4 a)",
                            "double round(double a)",
                            "double2 round(double2 a)",
                            "float fmod(float a, float b)",
                            "float2 fmod(float2 a, float2 b)",
                            "float3 fmod(float3 a, float3 b)",
                            "float4 fmod(float4 a, float4 b)",
                            "double fmod(double a, double b)",
                            "double2 fmod(double2 a, double2 b)",
                            "float max(float a, float b)",
                            "float2 max(float2 a, float2 b)",
                            "float3 max(float3 a, float3 b)",
                            "float4 max(float4 a, float4 b)",
                            "int max(int a, int b)",
                            "int2 max(int2 a, int2 b)",
                            "int3 max(int3 a, int3 b)",
                            "int4 max(int4 a, int4 b)",
                            "uint max(uint a, uint b)",
                            "uint2 max(uint2 a, uint2 b)",
                            "uint3 max(uint3 a, uint3 b)",
                            "uint4 max(uint4 a, uint4 b)",
                            "char max(char a, char b)",
                            "char2 max(char2 a, char2 b)",
                            "char3 max(char3 a, char3 b)",
                            "char4 max(char4 a, char4 b)",
                            "uchar max(uchar a, uchar b)",
                            "uchar2 max(uchar2 a, uchar2 b)",
                            "uchar3 max(uchar3 a, uchar3 b)",
                            "uchar4 max(uchar4 a, uchar4 b)",
                            "short max(short a, short b)",
                            "short2 max(short2 a, short2 b)",
                            "short3 max(short3 a, short3 b)",
                            "short4 max(short4 a, short4 b)",
                            "ushort max(ushort a, ushort b)",
                            "ushort2 max(ushort2 a, ushort2 b)",
                            "ushort3 max(ushort3 a, ushort3 b)",
                            "ushort4 max(ushort4 a, ushort4 b)",
                            "double max(double a, double b)",
                            "double2 max(double2 a, double2 b)",
                            "float min(float a, float b)",
                            "float2 min(float2 a, float2 b)",
                            "float3 min(float3 a, float3 b)",
                            "float4 min(float4 a, float4 b)",
                            "int min(int a, int b)",
                            "int2 min(int2 a, int2 b)",
                            "int3 min(int3 a, int3 b)",
                            "int4 min(int4 a, int4 b)",
                            "uint min(uint a, uint b)",
                            "uint2 min(uint2 a, uint2 b)",
                            "uint3 min(uint3 a, uint3 b)",
                            "uint4 min(uint4 a, uint4 b)",
                           
                            "char min(char a, char b)",
                            "char2 min(char2 a, char2 b)",
                            "char3 min(char3 a, char3 b)",
                            "char4 min(char4 a, char4 b)",
                            "uchar min(uchar a, uchar b)",
                            "uchar2 min(uchar2 a, uchar2 b)",
                            "uchar3 min(uchar3 a, uchar3 b)",
                            "uchar4 min(uchar4 a, uchar4 b)",
                            "short min(short a, short b)",
                            "short2 min(short2 a, short2 b)",
                            "short3 min(short3 a, short3 b)",
                            "short4 min(short4 a, short4 b)",
                            "ushort min(ushort a, ushort b)",
                            "ushort2 min(ushort2 a, ushort2 b)",
                            "ushort3 min(ushort3 a, ushort3 b)",
                            "ushort4 min(ushort4 a, ushort4 b)",

                            "double min(double a, double b)",
                            "double2 min(double2 a, double2 b)",
                            "float dot(float a, float b)",
                            "float dot(float2 a, float2 b)",
                            "float dot(float3 a, float3 b)",
                            "float dot(float4 a, float4 b)",
                            "int dot(int a, int b)",
                            "int dot(int2 a, int2 b)",
                            "int dot(int3 a, int3 b)",
                            "int dot(int4 a, int4 b)",
                            "uint dot(uint a, uint b)",
                            "uint dot(uint2 a, uint2 b)",
                            "uint dot(uint3 a, uint3 b)",
                            "uint dot(uint4 a, uint4 b)",
                            "char dot(char a, char b)",
                            "char dot(char2 a, char2 b)",
                            "char dot(char3 a, char3 b)",
                            "char dot(char4 a, char4 b)",
                            "uchar dot(uchar a, uchar b)",
                            "uchar dot(uchar2 a, uchar2 b)",
                            "uchar dot(uchar3 a, uchar3 b)",
                            "uchar dot(uchar4 a, uchar4 b)",
                            "short dot(short a, short b)",
                            "short dot(short2 a, short2 b)",
                            "short dot(short3 a, short3 b)",
                            "short dot(short4 a, short4 b)",
                            "ushort dot(ushort a, ushort b)",
                            "ushort dot(ushort2 a, ushort2 b)",
                            "ushort dot(ushort3 a, ushort3 b)",
                            "ushort dot(ushort4 a, ushort4 b)",
                            "double dot(double a, double b)",
                            "double dot(double2 a, double2 b)",
                            "float3 cross(float3 a, float3 b)",
                            "int3 cross(int3 a, int3 b)",
                            "char3 cross(char3 a, char3 b)",
                            "short3 cross(short3 a, short3 b)",
                            "float clamp(float x, float a, float b)",         
                            "float2 clamp(float2 x, float2 a, float2 b)",
                            "float3 clamp(float3 x, float3 a, float3 b)",
                            "float4 clamp(float4 x, float4 a, float4 b)",
                            "double clamp(double x, double a, double b)",
                            "double2 clamp(double2 x, double2 a, double2 b)",
                            "float lerp(float x, float a, float b)",
                            "float2 lerp(float2 x, float2 a, float2 b)",
                            "float3 lerp(float3 x, float3 a, float3 b)",
                            "float4 lerp(float4 x, float4 a, float4 b)",
                            "double lerp(double x, double a, double b)",
                            "double2 lerp(double2 x, double2 a, double2 b)",
                            "float2 normalize(float2 x)",
                            "float3 normalize(float3 x)",
                            "float4 normalize(float4 x)",
                            "double2 normalize(double2 x)",
                            "int any(int2 a)",
                            "int any(int3 a)",
                            "int any(int4 a)",
                            "uint any(uint2 a)",
                            "uint any(uint3 a)",
                            "uint any(uint4 a)",
                            "char any(char2 a)",
                            "char any(char3 a)",
                            "char any(char4 a)",
                            "uchar any(uchar2 a)",
                            "uchar any(uchar3 a)",
                            "uchar any(uchar4 a)",
                            "short any(short2 a)",
                            "short any(short3 a)",
                            "short any(short4 a)",
                            "ushort any(ushort2 a)",
                            "ushort any(ushort3 a)",
                            "ushort any(ushort4 a)",
                            "float any(float2 a)",
                            "float any(float3 a)",
                            "float any(float4 a)",
                            "double any(double2 a)",
                            "int all(int2 a)",
                            "int all(int3 a)",
                            "int all(int4 a)",
                            "uint all(uint2 a)",
                            "uint all(uint3 a)",
                            "uint all(uint4 a)",
                            "char all(char2 a)",
                            "char all(char3 a)",
                            "char all(char4 a)",
                            "uchar all(uchar2 a)",
                            "uchar all(uchar3 a)",
                            "uchar all(uchar4 a)",
                            "short all(short2 a)",
                            "short all(short3 a)",
                            "short all(short4 a)",
                            "ushort all(ushort2 a)",
                            "ushort all(ushort3 a)",
                            "ushort all(ushort4 a)",
                            "float all(float2 a)",
                            "float all(float3 a)",
                            "float all(float4 a)",
                            "double all(double2 a)",
                            NULL
};

CgBuiltinImp::CgBuiltinImp() 
: argType(NULL), retType(NULL), next(NULL), argTotal(0) 
{
    LOG(LOG_FN_ENTRY, "CgBuiltinImp::CgBuiltinImp()\n");
}


CgBuiltinImp::~CgBuiltinImp() 
{
    LOG(LOG_FN_ENTRY, "CgBuiltinImp::~CgBuiltinImp()\n");
//! Donot delete any Type objects created dynamically
//! because added to typelist in project object
//! project object will delete all type objects created in the project
#if 0
    delete retType;

    for (int i = 0; i < argTotal; i++) 
        delete argType[i];
#endif
    delete argType;

    delete next;

}

void CgBuiltinImp::addArgs(int argCount, BaseType** args)
{
    if(args == NULL)
    {
        assert(args);
        LOG(LOG_ERROR, "Internal error: %s, %d: NULL pointer for arguments", __FILE__, __LINE__);
        return;
    }
    argTotal = argCount;
    argType = args;
}

void CgBuiltinImp::setReturnType(BaseType* returnType)
{
    if(returnType == NULL)
    {
        assert(returnType);
        LOG(LOG_ERROR, "Internal error: %s, %d: NULL pointer for return type", __FILE__, __LINE__);
        return;
    }
    retType = returnType;
}

BaseType* CgBuiltinImp::getArg(int nthArg)
{
    if(nthArg < 0 || nthArg > argTotal)
    {
        assert(0);
        LOG(LOG_ERROR, "Internal error: %s, %d: requested arg %d but actual args are only %d", __FILE__, __LINE__, nthArg, argTotal);
        return NULL;
    }
    return argType[nthArg];
}
BaseType* CgBuiltinImp::getReturnType()
{
    if(retType == NULL)
    {
        assert(0);
        LOG(LOG_ERROR, "Internal error: %s, %d: return type is not set", __FILE__, __LINE__);
        return NULL;
    }
    return retType;
}

// ----------------------------------------------------------------------------

int CgBuiltin::addImplementation(int argTotal, BaseType *returnType, BaseType **args)
{
    //! create cgBuiltinImpl for given parameters
    CgBuiltinImp *impl = new CgBuiltinImp();
    impl->setReturnType(returnType);
    impl->addArgs(argTotal, args);

    //! Increment list count
    impTotal++;

    //! Add impl at head of list
    if(impList)
        impl->setNext(impList);
    impList = impl;

    return impTotal;
}

CgBuiltinImp* CgBuiltin::getImplementation(int nth)
{
    if(nth < 0 && nth >= impTotal)
    {
        assert(0);
        LOG(LOG_ERROR, "Internal error: %s, %d: requested nth(%d) but actual implements are only %d", __FILE__, __LINE__, nth, impTotal);
        return NULL;
    }
    CgBuiltinImp *tempImp = impList;
    for(int i = 0; i < nth; ++i)
    {
        tempImp = tempImp->getNext(); 
    }
    return tempImp;
}
// ----------------------------------------------------------------------------

void CgBuiltinMap::addBuiltin(CgBuiltin *val)
{
    LOG(LOG_FN_ENTRY, "CgBuiltinMap::addBuiltin(%p)\n", val);
    CgBuiltinIter iter = find(val->getName());
    if (iter == end())
        (*this)[val->getName()] = val;
    else
        CGEXIT("duplication definition of builtin");
}

CgBuiltin * CgBuiltinMap::getBuiltin(const std::string &name)
{
    LOG(LOG_FN_ENTRY, "CgBuiltinMap::getBuiltin(%s)\n", name.c_str());
    CgBuiltinIter iter = find(name);
    if (iter != end())
        return (*iter).second;

    return NULL;
}

// ----------------------------------------------------------------------------

CgBuiltinMap hlslBuiltin;

/** Create the table for all hlsl builtin functions
 */
void builtinInit()
{
    LOG(LOG_FN_ENTRY, "builtinInit()\n");

    for(int i = 0; builtinFunctions[i] != NULL; ++i)
    {
        std::string returnTypeName = "";
        BaseTypeSpec typeMask = BT_Int;
        std::string funcName = "";
        BaseType *returnType = NULL;
        BaseType **args = NULL;
        int argTotal = 0;

        std::string funcProtoType = std::string(builtinFunctions[i]);
//#define PRINT_BUILTIN_FUNCTION_DETAILS
#ifdef PRINT_BUILTIN_FUNCTION_DETAILS
        std::cout << std::endl << funcProtoType.c_str() << std::endl;
#endif
        size_t pos = 0;
        
        //! get the number of arguments
        for(int i = 0; funcProtoType[i] != '\0'; ++i)
        {
            if(funcProtoType[i] == ',')
                argTotal++;
        }
        if(argTotal)
            argTotal++;
        else
            argTotal = 1;

#ifdef PRINT_BUILTIN_FUNCTION_DETAILS
        std::cout << "arg = " << argTotal;
#endif

        //! get the return type 
        pos = funcProtoType.find(' ');
        returnTypeName = funcProtoType.substr(0, pos);
        typeMask = getBaseTypeSpec(returnTypeName);
        returnType = new BaseType(typeMask);

#ifdef PRINT_BUILTIN_FUNCTION_DETAILS
        std::cout << " " << returnTypeName.c_str();
#endif

        //! get function name
        funcProtoType = funcProtoType.substr(pos + 1);
        pos = funcProtoType.find('(');
        funcName = funcProtoType.substr(0, pos);

#ifdef PRINT_BUILTIN_FUNCTION_DETAILS
        std::cout << " " << funcName.c_str() << " ";
#endif

        //! get argument types
        size_t startPos = pos + 1;
        size_t endPos = startPos;
        args = new BaseType*[argTotal];
        for(int j = 0; j < argTotal; ++j)
        {
            std::string tempArgName = "";
            endPos = funcProtoType.find(' ', startPos);
            tempArgName = funcProtoType.substr(startPos, endPos - startPos);
            args[j] = new BaseType(getBaseTypeSpec(tempArgName));
#ifdef PRINT_BUILTIN_FUNCTION_DETAILS
        std::cout << tempArgName.c_str() << "(" << j << ")" << " ";
#endif            
            if(j != argTotal - 1)
                startPos = funcProtoType.find(',', startPos) + 2;
        }

        //! check function is already available or not in the table
        CgBuiltin *curBuiltin = NULL;
        if(!hlslBuiltin.getBuiltin(funcName))
        {
            CgBuiltin *curBuiltin = new CgBuiltin(funcName);
            curBuiltin->addImplementation(argTotal, returnType, args);
            hlslBuiltin.addBuiltin(curBuiltin);
        }
        else
        {
            curBuiltin = hlslBuiltin.getBuiltin(funcName);
            curBuiltin->addImplementation(argTotal, returnType, args);
        }
    }
}

void builtinFinit()
{
    LOG(LOG_FN_ENTRY, "builtinFinit()\n");
    for (CgBuiltinIter iter = hlslBuiltin.begin(); iter != hlslBuiltin.end(); iter++)
        delete (*iter).second;

    hlslBuiltin.clear();
}

