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
#include <stdarg.h>
#include "logger.hpp"
#include "cgTool.h"
#include "decl.h"
#include "express.h"

// ----------------------------------------------------------------------------

void cgErrorMsg(const char*format, ...)
{
    va_list p;
    va_start(p, format);
    vfprintf(stderr, format, p);
    va_end(p);
}

void cgExit(const char *format, ...)
{
    //cgErrorMsg(format);
    va_list p;
    va_start(p, format);
    vfprintf(stderr, format, p);
    va_end(p);
    exit(1);
}

void cgAssert(bool val, const char *format, ... )
{
    if (val != true)
        cgExit(format);
}

// ----------------------------------------------------------------------------
/**
 * call func for each scalar component in the type
 */
void applyScalarType(Type* intype, std::string fieldPath, fnScalarTypeCallback func, void* data)
{    
    LOG(LOG_FN_ENTRY, " applyScalarType(%p, %s, %p, %p)\n", intype, fieldPath.c_str(), func, data);
    //if (type->isArray() || type->isStream())
    BaseType *type = intype->getBase();

    StructDef *structDef = findStructureDef(type);
    if (structDef == NULL)
    {
        (*func)(type, fieldPath, data);
    }
    else
    {
        for (int i = 0; i < AsInt(structDef->nComponents); i++)
        {
            applyScalarType(structDef->components[i]->form, 
                fieldPath + "." + structDef->components[i]->name->name, 
                func, data);
        }

    }
};


/**
 * return true if it is a recognize kernel scalar data type
 */
bool isKernelScalarType(Type* type)
{
    LOG(LOG_FN_ENTRY, " isKernelScalarType(%p)\n");
    BaseTypeInfo *typeInfo = getBaseTypeInfo(type->getBase()->typemask);

    //temporary use rtTypeIDStr
    //fix me: build a map: kernel scalar type => kernel register type
    return (typeInfo != NULL && typeInfo->rtTypeIDStr != NULL);
}

bool isDoubleType(Type *type)
{
    if (type && type->isBaseType())
    {
        BaseType *baseType = AsBaseType(type);
        BaseTypeSpec typemask = baseType->typemask;

        return (typemask & BT_Double) || (typemask & BT_Double2);
    }

    return false;
}

bool isScatterType(Type *type)
{
    return (type && type->isArray() 
        && (type->getQualifiers() & TQ_Out )!=0);
}

bool isABaseType(Type *type, BaseTypeSpec btmask)
{
    if (type && type->isBaseType())
    {
        BaseType *baseType = AsBaseType(type);
        BaseTypeSpec typemask = baseType->typemask;

        return (typemask & btmask) != 0;
    }

    return false;
}

/**
 *  return the position of the formal arg
 */
int
findArgPos(FunctionType *funcType, Symbol *argSym)
{
    LOG(LOG_FN_ENTRY, "findArgPos(%p, %p)\n", funcType, argSym);
    for (int i = 0; i < AsInt(funcType->nArgs); ++i)
    {
        if (funcType->args[i]->name->name == argSym->name)
        {
            return i;
        }
    }

    return -1;
}

/**
 *  return the position of the first output
 */
int
findFirstOutputPos(FunctionType *funcType)
{
    LOG(LOG_FN_ENTRY, "findFirstOutputPos(%p)\n", funcType);
    for (int i = 0; i < AsInt(funcType->nArgs); ++i)
    {
        if((funcType->args[i]->form->getQualifiers() & TQ_Out) != 0)
        {
            return i;
        }
    }

    return -1;
}

Expression*
getActualArg(FunctionCall *call, int i)
{
    LOG(LOG_FN_ENTRY, "getActualArg(%p, %d)\n", call, i);
    assert(AsInt(call->args.size()) > i);
    return call->args[i];
}

int 
getArrayRank(Type * form)
{
    LOG(LOG_FN_ENTRY, "getArrayRank(%p)\n", form);
    Type *f = form;
    int rank = 0;

    while (f->isArray())
    {
        rank++;
        f = ((ArrayType *) f)->subType;
    }

    return rank;
}
// ----------------------------------------------------------------------------

int 
stringReplace(char *str, char old, char newc)
{
    if (str == NULL)
        return 0;

    int res = 0;
    int cur = 0;

    while (str[cur] != '\0')
    {
        if (str[cur] == old)
        {
            str[cur] = newc;
            res ++;
        }
        cur ++;
    }

    return res;
}

int 
stringReplace(std::string &str, char old, char newc)
{
    int res = 0;
    size_t cur = 0;
    size_t tot = str.size();

    while (cur < tot)
    {
        if (str[cur] == old)
        {
            str[cur] = newc;
            res ++;
        }
        cur ++;
    }

    return res;
}

std::string
stripPath(const std::string& name)
{
    std::string inStr(name);
    size_t pos;
    
    pos = inStr.rfind('/');
    if (pos != std::string::npos)
        inStr = inStr.substr(pos + 1);

    pos = inStr.rfind('\\');
    if (pos != std::string::npos)
        inStr = inStr.substr(pos + 1);

    return inStr;
}

