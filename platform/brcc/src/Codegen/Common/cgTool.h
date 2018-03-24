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

#ifndef CG_TOOL_H
#define CG_TOOL_H
#include <stdio.h>

#include "decl.h"
#include "express.h"

class FunctionCall;
/// print message and exit
#define CGEXIT cgExit
#define CGASSERT cgAssert
extern void cgAssert(bool, const char*, ...);
extern void cgExit(const char*, ...);
extern void cgErrorMsg(const char*, ...);


//IR utility
#define AsArrayType(e) static_cast<ArrayType*>(e)
#define AsBaseType(t) static_cast<BaseType*>(t)
#define AsBinaryExpr(e) static_cast<BinaryExpr*>(e)
#define AsBrtIndexofExpr(e) static_cast<BrtIndexofExpr*>(e)
#define AsBrtStreamType(e) static_cast<BrtInitializedStreamType*>(e)
#define AsDeclaration(e) static_cast<DeclStemnt*>(e)
#define AsFunctionCall(e) static_cast<FunctionCall*>(e)
#define AsFunctionDef(val) static_cast<FunctionDef*>(val)
#define AsFunctionType(t) static_cast<FunctionType*>(t)
#define AsInt(val) static_cast<int>(val)
#define AsIndexExpr(e) static_cast<IndexExpr*>(e)
#define AsVariable(e) static_cast<Variable*>(e)

#define isUnsignedType(t) ((t & BT_UnSigned) != 0)
#define stripUnsignedMask(t)  (t & (~BT_UnSigned))

#define GetFunctionDefName(fDef) (fDef->decl->name->name)
#define IsOutputArg(q) ((q & TQ_Out)!=0 || (q & TQ_Reduce) != 0)
#define IsReductionOut(q) ((q & TQ_Reduce) != 0)
#define IsScatterOutType(t) (t && t->isArray() && \
    (t->getQualifiers() & TQ_Out) != 0)

extern bool isDoubleType(Type *);
extern bool isScatterType(Type *);
extern bool isABaseType(Type *, BaseTypeSpec);

typedef void (*fnScalarTypeCallback) (BaseType *type, std::string , void*);
extern void applyScalarType(Type*, std::string, fnScalarTypeCallback, void*);
extern bool isKernelScalarType(Type*);

extern int findArgPos(FunctionType *funcType, Symbol *argSym);
extern int findFirstOutputPos(FunctionType *funcType);
extern Expression* getActualArg(FunctionCall *call, int i);
extern int getArrayRank(Type * form);

//string utility
extern int stringReplace(char *str, char oldc, char newc);
extern int stringReplace(std::string &str, char oldc, char newc);
extern std::string stripPath(const std::string &);

#endif //CG_TOOL_H
