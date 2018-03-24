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
#include "main.h"
#include "cgSemantic.h"
#include "cgBuiltin.h"
#include "cgTool.h"

//fixme:
#include "brtdecl.h"
#include "brtexpress.h"
 
// ----------------------------------------------------------------------------
const char *SemanticChecker::errorMsg[SCETotal] =
{
    //kernel code, Expression
    "",
    "Problem with call expression in kernel",
    "C style cast is ignored in kernel. Use built-in funtions for reinterpreted cast",
    "String constant in kernel not allowed",
    "Problem in index expression",
    "Problem with gather array access. Expect a gather array in place of expr",
    "Operator not supported in kernel",
    "Sizof not allowed in kernel",
    "Problem with Array variable declaration",
    "Problem with variable in kernel",
    "Problem with initializer in kernel",
    "Problem with expression in kernel",

    "Problem in Binary expression",
    "Problem in Relational expression",
    "Problem in Trinary expression",
    "Problem in Assignment expression",
    "Not supported kernel local/parameter data struture type",

    //kernel code, statement
    "control flow statement not supported",
#ifdef __ASMIL__
    "asm not supported outside of kernel",
#endif

    //host code, 
    "Problem with stream declaration",
    "Stream can't have an initializer",

    //both
    "Iterator not supported",
    "Stream element type not supported",
    "Other problems",
};

const char *SemanticChecker::warningMsg[SCWTotal] =
{
    //! we can use these message in any level of warning
    
    // Level 0 warnings
    "", //! select if perticular warning is not in any group
    "language feature not available to dx9 backend",
    "In Initialization",
    "In Binary expression",
    "In Relational expression",
    "In Trinary expression",
    "In Assignment expression",
    "Problem with stream declaration",
    "Non-kernel routine found",

    // Level 1 warnings
    "Unsupported Math operator on CAL backend",

    // Level 2 warnings
    
    // Level 3 warnings
    
};


//to do: add SemanticChecker as a parameter of the syntax semanticCheck routine
//       to avoid this global variable
SemanticChecker semanticChecker;

/////////////////////////////////////////////////////////////////////////////
//!  \brief Returns error message for given errorID
/////////////////////////////////////////////////////////////////////////////
const char* 
SemanticChecker::getErrorMsg(SCErrorID errorID)
{
    return errorMsg[errorID];
}

/////////////////////////////////////////////////////////////////////////////
//!  \brief Returns warning message for given errorID
/////////////////////////////////////////////////////////////////////////////
const char*
SemanticChecker::getWarningMsg(SCWarningID warningID)
{
    return warningMsg[warningID];
}

/////////////////////////////////////////////////////////////////////////////
//!  \brief Returns true if level of warning enabled
/////////////////////////////////////////////////////////////////////////////
bool 
SemanticChecker::isWarningLevelEnabled(SCWarningLevel level)
{

    if(level >= 0 && level <= globals.warningLevel)
        return true;
    return false;
}
void 
SemanticChecker::emitError(SCErrorID errorID)
{
    LOG(LOG_FN_ENTRY, "SemanticChecker::emitError(%d)\n", errorID);
    _outs << "ERROR--" << _errorTotal << ": " << getErrorMsg(errorID);
}

void 
SemanticChecker::emitWarn(SCWarningID warningID, SCWarningLevel level)
{
    LOG(LOG_FN_ENTRY, "SemanticChecker::emitWarn(%d)\n", warningID);

    //! Increment the error count if warnings treated as errors
    if(globals.enableStrongTypeChecking)
    {
        if(level == SCWarningLevelZero)
            _outs << "WARN--" << _warnTotal;
        else
            _outs << "ERROR--" << _errorTotal;
    }
    else
    {
        if(globals.warningsAsErrors == false)
            _outs << "WARN--" << _warnTotal; 
        else
            _outs << "WARNING AS ERROR--" << _warnTotal;
    }

    if(warningID != SCWNull)
        _outs << ": ";
    _outs << getWarningMsg(warningID);
}

void 
SemanticChecker::checkExit(bool exitHere)
{
    LOG(LOG_FN_ENTRY, "SemanticChecker::checkExit(%d)\n", exitHere);
    if (exitHere) 
    {
        cgExit("Fatal error, stopping compilation");
    }
    else if (_errorTotal > _maxErrorsAllowed)
    {
        std::ostringstream tmpmsg;
        tmpmsg << "Error count exceeds " << _maxErrorsAllowed << "; stopping compilation";
        cgExit(tmpmsg.str().c_str());
    }
}

void 
SemanticChecker::recordError(Statement *stmt, SCErrorID errorID, 
                        const char *msg, bool exitHere)
{      
    LOG(LOG_FN_ENTRY, "SemanticChecker::recordError(%p, %d, %s, %d)\n", stmt, errorID, msg, exitHere);
    _errorTotal ++;

    //! Print name of the file and line number in which there is a error
    if (stmt)
        _outs << stmt->location;

    //! Print error message with error count
    emitError(errorID);
    if (msg)
        _outs << ": " << msg;
    _outs << std::endl;

    //! Print the statement in which there is a error
    if (stmt)
    {
        _outs << "\t\t Statement: ";
        stmt->print(_outs, 0);
        _outs << std::endl;
    }

    checkExit(exitHere);
}

void 
SemanticChecker::recordWarn(Statement *stmt, SCWarningID warningID, 
                        const char *msg, SCWarningLevel level)
{      

    //! Check warning level enabled or not
    if(isWarningLevelEnabled(level) == false)
        return;



    //! Increment the error count if warnings treated as errors
    if(globals.enableStrongTypeChecking)
    {
        if(level != 0)
            _errorTotal ++;
        else
            _warnTotal ++;
    }
    else
    {
        _warnTotal ++;
        if(globals.warningsAsErrors == true)
            _errorTotal ++;
    }

    //! Print name of the file and line number in which there is a warning
    if (stmt)
        _outs << stmt->location;

    //! Print error message with warning count
    emitWarn(warningID, level);
    if (msg)
        _outs << ": " << msg;
    _outs << std::endl;

    //! Print the statement in which there is a warning
    if (stmt)
    {
        stmt->print(_outs, 0);
        _outs << std::endl;
    }
}

void 
SemanticChecker::recordError(Expression *expr, SCErrorID errorID, 
                        const char *msg, bool exitHere)
{
    LOG(LOG_FN_ENTRY, "SemanticChecker::recordError(%p, %d, %s, %d)\n", expr, errorID, msg, exitHere);
    _errorTotal ++;

    //! Print name of the file and line number in which there is a error
    if (expr)
    {
        _outs << expr->getRootExp()->location;
    }

    //! Print error message with error count
    emitError(errorID);
    if (msg)
        _outs << ": " << msg;
    _outs << std::endl;

    //! Print the statement in which there is a error
    if (expr)
    {
        _outs << "\n\tStatement: ";
        expr->print(_outs);
        _outs << " in ";
        expr->getRootExp()->print(_outs);
        _outs << std::endl;
        expr->printSubExprInfo(_outs);
        _outs << "\n";
    }

    checkExit(exitHere);
}

void 
SemanticChecker::recordWarn(Expression *expr, SCWarningID warningID, 
                        const char *msg, SCWarningLevel level)
{
    LOG(LOG_FN_ENTRY, "SemanticChecker::recordWarn(%p, %d, %s)\n", expr, warningID, msg);
    
    //! Check warning level enabled or not
    if(isWarningLevelEnabled(level) == false)
        return;

    //! Increment the error count if warnings treated as errors
    if(globals.enableStrongTypeChecking)
    {
        if(level != 0)
            _errorTotal ++;
        else
            _warnTotal ++;
    }
    else
    {
        _warnTotal ++;
        if(globals.warningsAsErrors == true)
            _errorTotal ++;
    }

    //! Print name of the file and line number in which there is a warning
    if (expr)
    {
        _outs << expr->getRootExp()->location;
    }

    //! Print error message with warning count
    emitWarn(warningID, level);
    if (msg)
        _outs << ": " << msg;
    _outs << std::endl;

    //! Print the statement in which there is a warning
    if (expr)
    {
        _outs << "\n\tStatement: ";
        expr->print(_outs);
        _outs << " in ";
        expr->getRootExp()->print(_outs);
        _outs << std::endl;
        expr->printSubExprInfo(_outs);
        _outs << "\n";
    }

}

void 
SemanticChecker::recordError(Decl *dcl, Statement *parent, 
                             SCErrorID errorID, 
                             const char *msg, bool exitHere)
{      
    LOG(LOG_FN_ENTRY, "SemanticChecker::recordError(%p, %p, %d, %s, %d)\n", dcl, parent, errorID, msg, exitHere);
    _errorTotal ++;

    //! Print name of the file and line number in which there is a error
    if (dcl && parent)
    {
        _outs << parent->location;  
    }

    //! Print error message with error count
    emitError(errorID);
    if (msg)
        _outs << ": " << msg;
    _outs << std::endl;

    //! Print the statement in which there is a error
    if (dcl)
    {
        _outs << "\n\tStatement: ";
        //dcl->form != NULL to work around the odd that form==NULL but dcl->print shown "int"
        dcl->print(_outs, dcl->form != NULL, 0);
        dcl->printExprWithTypeInfo(_outs);
        _outs << std::endl;
    }

    checkExit(exitHere);
}

void 
SemanticChecker::recordWarn(Decl *dcl, Statement *parent, 
                            SCWarningID warningID, 
                            const char *msg, SCWarningLevel level)
{      
    LOG(LOG_FN_ENTRY, "SemanticChecker::recordWarn(%p, %p, %d, %s)\n", dcl, parent, warningID, msg);
    
    //! Check warning level enabled or not
    if(isWarningLevelEnabled(level) == false)
        return;
    
    //! Increment the error count if warnings treated as errors
    if(globals.enableStrongTypeChecking)
    {
        if(level != 0)
            _errorTotal ++;
        else
            _warnTotal ++;
    }
    else
    {
        _warnTotal ++;
        if(globals.warningsAsErrors == true)
            _errorTotal ++;
    }

    //! Print name of the file and line number in which there is a warning
    if (dcl && parent)
    {
        _outs << parent->location;  
    }

    //! Print error message with warning count
    emitWarn(warningID, level);
    if (msg)
        _outs << ": " << msg;
    _outs << std::endl;

    //! Print the statement in which there is a warning
    if (dcl)
    {
        _outs << "\n\tStatement: ";
        //dcl->form != NULL to work around the odd that form==NULL but dcl->print shown "int"
        dcl->print(_outs, dcl->form != NULL, 0);
        dcl->printExprWithTypeInfo(_outs);
        _outs << std::endl;
    }

    if (warningID == SCWDx9)
        _passTargetDx9 = false;
}


void SemanticChecker::recordNote(SCNoteID noteId, const char *msg)
{
    switch(noteId)
    {
        case SCNoteOnStrongTypeChecking:
            if(globals.strongTypeCheckNote)
            {
                _outs << "\n Note: To disable strong type checking, use \"-a\".\n"
                      << "       However, it is recommended that you modify your code to match the types explicitly\n" 
                      << "       instead of relying on implicit type casting.\n\n";
                globals.strongTypeCheckNote = false;
            }
            break;
        case SCNoteOnDisableCachedGatherArray:
            _outs << "\n Note: Use \"-c\" option to suppress this error\n"
                  << "       However, it is recommended to resolve the error which helps the brcc to generate \n" 
                  << "       efficient code. For more details, please see usage of cached gather array. \n\n";            
            break;
        case SCBackEndCodeGenFailed:
            _outs << "\nError--" << ":" << msg << std::endl;
            break;
        case SCRelationalConditionalExpr:
            _outs << "\n Note: Use built-in functions any() or all() when you are using relational operators on vector data types\n\n";  
            break;
        default:
            LOG(LOG_ERROR, " Unknown NoteId", __FILE__, __LINE__);

    }
}
void
SemanticChecker::fini()
{
    if (_checkTargetDx9 && _passTargetDx9==false)
    {
        _outs << "WARN: DX9 backend is off. The program uses language features not supported by dx9."
            << std::endl;

    }

    if (_errorTotal >= 1)
    {
        _passTargetDx9 = false;
        _passTargetCal = false;
    }
}

bool
SemanticChecker::checkKernelStruct(Decl *dcl, Statement *parent, StructDef* strDef)
{
    LOG(LOG_FN_ENTRY, "isKernelAllowStruct(%p)\n", strDef);
    if (strDef->isUnion())
    {
        return false;
    }

    for (int i = 0; i < strDef->nComponents; i++)
    {
        Decl* eleDecl = strDef->components[i];
        //if (elementDecl->storage & ST_Static)
        //    continue;
        Type* eleType = eleDecl->form;

        //pointer, bitfield, etc ..
        //struct enum are consider base type
        if (eleType->isBaseType() == false)
        {
            return false; 
        }
 
        BaseType* eleBase = eleType->getBase();
        StructDef* eleStr = findStructureDef(eleBase);
        if (eleStr == NULL)
        {
            if (_checkTargetDx9 && isDoubleType(eleBase))
            {
                recordWarn(dcl, parent, SCWDx9);
            } 

            if (isKernelScalarType(eleBase) == false)
            {
                return false;
            }
        }
        else 
        {
            return checkKernelStruct(dcl, parent, strDef);
        }
    }

    return true;
}

void 
SemanticChecker::checkBuiltin(FunctionCall *callExp, Variable *calleeVar)
{
    LOG(LOG_FN_ENTRY, " checkBuiltin(%p, %p)\n", callExp, calleeVar);

    //todo check parameter
    CgBuiltin *builtin = findBuiltin(calleeVar->name->name);

    if (builtin == NULL)
    {
        recordError(callExp, SCEKExprCall, "callee unknown");
        return;
    }
    
    //! Check semantics of function call with the overloaded functions available 
    //! function table
    bool matched = false;
    int acutalArgCount = AsInt(callExp->args.size());
    for(int i = 0; i < builtin->getImpCount(); ++i)
    {

        CgBuiltinImp *imp = builtin->getImplementation(i);
        if(imp->getArgTot() == acutalArgCount)
        {
            int matchedArgCount = 0;
            for(int j = 0; j < imp->getArgTot(); ++j)
            {
                if(callExp->args[j]->getType() == NULL || callExp->args[j]->getType()->getBase()== NULL)
                {
                    assert(0);
                    LOG(LOG_ERROR, " %s :line %d -> Type information is not set by parser", __FILE__, __LINE__);
                    continue;
                }
                BaseType *type = callExp->args[j]->getType()->getBase();
                if(imp->getArg(j)->typemask == type->typemask)
                    matchedArgCount++;
            }
            if(matchedArgCount == imp->getArgTot())
            {
                matched = true;
                //! Set return type of function call for further use for type checking
                assert(!callExp->function->type);
                BaseTypeSpec actualMask = imp->getReturnType()->typemask;
                callExp->function->type = new BaseType(actualMask);
                break;
            }

        }
    }
 
    if(matched == false)
    {
        recordError(callExp, SCEKNull, "No matched builtin function found");
    }

    // get function name
    std::ostringstream tempStrStream;
    callExp->function->print(tempStrStream);
    std::string tempStr = tempStrStream.str();

    //! Check the Math fucntion parameters. warn if any parameter data type is double
    //! we are using float operation because there are no IL instructions for double math operations
    if((globals.userTargets & TARGET_CAL) && isSupportedDoubleMathOps(tempStr))
    {
        for (int i = 0; i < int (callExp->args.size()); ++i)
        {
            //! Type information is not set always during parse
            //! so skep if type is NULL
            if(callExp->args[i]->getType() == NULL || callExp->args[i]->getType()->getBase()== NULL)
            {
                LOG(LOG_ERROR, " %s :line %d -> Type information is not set by parser", __FILE__, __LINE__);
                continue;
            }
            
            BaseType *type = callExp->args[i]->getType()->getBase();
            BaseTypeInfo *typeInfo = getBaseTypeInfo(type->typemask);

            std::string doubleDataType = "double";
            std::string double2DataType = "double2";
            if(doubleDataType.compare(typeInfo->brTypeName) == 0 
                || double2DataType.compare(typeInfo->brTypeName) == 0)
            {
                std::ostringstream tmpmsg;
                tmpmsg << "all Parameters of " << tempStr << " are converted to float";
                recordWarn(callExp, SemanticChecker::SCWDoubleToFloat, tmpmsg.str().c_str());
                break;
            }
        }
    }
}

void
SemanticChecker::checkStreamDimension(Decl *dcl, Statement *parent, Expression *exp)
{
    LOG(LOG_FN_ENTRY, "checkStreamDimension(%p, %p, %p)\n", dcl, parent, exp);
    Type *expType = exp->getType();
    if (expType == NULL)
    {
        //warning
        std::ostringstream tmpmsg;
        tmpmsg << "can't determine on the type of ";
        exp->print(tmpmsg);
        tmpmsg << ", expected int";

        recordWarn(dcl, parent, SCWStream, 
            tmpmsg.str().c_str());
    }
    else
    {
        if (expType->isBaseType() == false ||
            ((AsBaseType(expType))->typemask & BT_Int) == 0)
        {
            recordError(dcl, parent, SCEHStreamOther, "expect int for stream dimension parameter");
        }
    }
}

//! Checks stream declarations kernel parameters
void SemanticChecker::checkStreamDeclSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal)
{
	//! To do
}

//! checks the Array declaration semantics (gather/scatter or local array declarations)
void SemanticChecker::checkArrayDeclSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal)
{
    if(isKernel)
    {
        Type* dclType = dcl->form;
        if(dclType->isArray())
        {
            ArrayType* arrayType = AsArrayType(dclType);

            if(isFormal)
            {
                if(IsScatterOutType(dclType))
                {
                    //! scatter array declaration semantic checks
                    int arrayDim = 0;
                    int dim = arrayType->getDimensions();
                    ArrayType* tmpArrayType = arrayType;
                    bool isArraySizesSpecified = false;
                    bool isSemanticsOk = true;
                    while(tmpArrayType->type == TT_Array)
                    {
                        arrayDim++;
                        if(tmpArrayType->size)
                        {
                            if(tmpArrayType->size->etype == ET_Constant)
                            {
                                isArraySizesSpecified = true;
                                Constant *constant = static_cast<Constant *>(tmpArrayType->size);
                                if(constant->ctype == CT_Int)
                                {
                                    IntConstant *intConst = static_cast<IntConstant *>(constant);
                                    if(intConst->lng <= 0)
                                    {
                                        recordError(dcl, parent, SCEKArrayDecl, 
                                                    "Scatter Array size must be non-zero positive int value if specified.");
                                        isSemanticsOk = false;
                                        break;
                                    }
                                }
                                else
                                {
                                    recordError(dcl, parent, SCEKArrayDecl, 
                                                 "Scatter Array size must be constant int type if specified");
                                    isSemanticsOk = false;
                                    break;
                                }
                            }
                            else
                            {
                                if(globals.disableConstBuffer == false)
                                {
                                    recordError(dcl, parent, SCEKArrayDecl, 
                                                "Scatter Array size expression must be simple constant int if specified");
                                }
                                else
                                {
                                    recordWarn(dcl, parent, SCWNull, "Scatter Array size expression must be simple constant int if specified");
                                }
                                isSemanticsOk = false;
                                break;
                            }
                        }
                        if(arrayDim == dim)
                            break;

                        tmpArrayType = AsArrayType(tmpArrayType->subType);
                        if(tmpArrayType == NULL)
                        {
                            isSemanticsOk = false;
                            std::ostringstream tmpStr;
                            dcl->print(tmpStr, true);
                            LOG(LOG_ERROR, "Internal error :%s : %d -- Type information missing for Scatter array declaration %s", tmpStr.str().c_str());
                            break;
                        }
                    }

                    if(globals.disableConstBuffer == false)
                    {
                        if(isArraySizesSpecified)
                            recordError(dcl, parent, SCEKArrayDecl, 
                                        "Specifying scatter array sizes have no meaning presently. Donot specify sizes");
                    }
                    else
                    {
                        if(isArraySizesSpecified)
                            recordWarn(dcl, parent, SCWNull, 
                                        "Specifying scatter array sizes have no meaning presently. Donot specify sizes");
                    }
                    arrayType->isSemanticsOk = isSemanticsOk;
                }
                else
                {
                    //! Gather Array declaration semantic checks
                    //! Sets the resouceType of Gather array 
                    //! resourceType can be TextureType or CosntBufferType

                    int arrayDim = 0;
                    int dim = arrayType->getDimensions();
                    bool arrayUsedAsConstBuffer = true;
                    ArrayType* tmpArrayType = arrayType;
                    int elementCount = 0;
                    int specifiedArraySizesCount = 0;
                    bool isSemanticsOk = true;
                    while(tmpArrayType->type == TT_Array)
                    {
                        arrayDim++;
                        if(tmpArrayType->size)
                        {
                            if(tmpArrayType->size->etype == ET_Constant)
                            {
                                specifiedArraySizesCount++;
                                Constant *constant = static_cast<Constant *>(tmpArrayType->size);
                                if(constant->ctype == CT_Int)
                                {
                                    IntConstant *intConst = static_cast<IntConstant *>(constant);
                                    if(intConst->lng <= 0)
                                    {
                                        recordError(dcl, parent, SCEKArrayDecl, 
                                                    "Gather Array size must be non-zero positive int value if specified");
                                        isSemanticsOk = false;
                                        break;
                                    }
                                    if(arrayDim == 1)
                                        elementCount = intConst->lng;
                                    else
                                        elementCount = elementCount * intConst->lng;
                                }
                                else
                                {
                                    recordError(dcl, parent, SCEKArrayDecl, 
                                                 "Gather Array size must be constant int type if specified");
                                    isSemanticsOk = false;
                                    break;
                                }
                            }
                            else
                            {
                                if(globals.disableConstBuffer == false)
                                {
                                    recordError(dcl, parent, SCEKArrayDecl, 
                                                "Gather Array size expression must be simple constant int if specified");
                                    recordNote(SCNoteOnDisableCachedGatherArray);
                                }
                                else
                                {
                                    recordWarn(dcl, parent, SCWNull, "Gather Array size expression must be simple constant int if specified");
                                }
                                isSemanticsOk = false;
                                break;
                            }
                        }
                        else
                        {
                            arrayUsedAsConstBuffer = false;
                        }
                        if(arrayDim == dim)
                            break;

                        tmpArrayType = AsArrayType(tmpArrayType->subType);
                        if(tmpArrayType == NULL)
                        {
                            isSemanticsOk = false;
                            std::ostringstream tmpStr;
                            dcl->print(tmpStr, true);
                            LOG(LOG_ERROR, "Internal error :%s : %d -- Type information missing for gather array declaration %s", tmpStr.str().c_str());
                            break;
                        }
                    }

                    if(arrayUsedAsConstBuffer && elementCount <= 4096)
                    {
                        arrayType->resourceType = ConstBufferType;
                    }

                    if(globals.disableConstBuffer == false)
                    {
                        if(specifiedArraySizesCount > 0 && specifiedArraySizesCount < arrayDim)
                            recordError(dcl, parent, SCEKArrayDecl, 
                                            "Incomplete array size specification: Specify all array sizes for cached gather array (e.g. a[5][5]) or no array sizes for Streams (e.g. a[][])");
                    }
                    else
                    {
                        if(specifiedArraySizesCount > 0 && specifiedArraySizesCount < arrayDim)
                            recordWarn(dcl, parent, SCWNull, 
                                            "Incomplete array size specification: Specify all array sizes for cached gather array (e.g. a[5][5]) or no array sizes for Streams (e.g. a[][])");

                    }

                    arrayType->isSemanticsOk = isSemanticsOk;
                }
            }
            else
            {
                //! Local Array declaration semantic checks
                if(arrayType->getQualifiers() != TQ_Shared)
                {
                    recordError(dcl, parent, SCEKArrayDecl, "Local Array not supported yet");
                    return;
                }
                int arrayDim = 0;
                int dim = arrayType->getDimensions();
                if(arrayType->getQualifiers() == TQ_Shared && dim != 1)
                {
                    recordError(dcl, parent, SCEKArrayDecl, "Shared Local Array must be 1D");
                    return;
                }
                ArrayType* tmpArrayType = arrayType;
                bool isSemanticsOk = true;
                while(tmpArrayType->type == TT_Array)
                {
                    arrayDim++;
                    if(tmpArrayType->size)
                    {
                        if(tmpArrayType->size->etype == ET_Constant)
                        {
                            Constant *constant = static_cast<Constant *>(tmpArrayType->size);
                            if(constant->ctype == CT_Int)
                            {
                                IntConstant *intConst = static_cast<IntConstant *>(constant);
                                if(intConst->lng <= 0)
                                {
                                    recordError(dcl, parent, SCEKArrayDecl, 
                                                "Local Array size must be non-zero positive int value.");
                                    isSemanticsOk = false;
                                    break;
                                }
                            }
                            else
                            {
                                recordError(dcl, parent, SCEKArrayDecl, 
                                             "Local Array size must be constant int type.");
                                isSemanticsOk = false;
                                break;
                            }
                        }
                        else
                        {
                            recordError(dcl, parent, SCEKArrayDecl, 
                                         "Local Array size expression must be simple constant int.");
                            isSemanticsOk = false;
                            break;
                        }
                    }
                    else
                    {
                        recordError(dcl, parent, SCEKArrayDecl, 
                                    "Local Array size expression must be simple constant int.");
                        isSemanticsOk = false;
                        break;
                    }
                    if(arrayDim == dim)
                        break;

                    tmpArrayType = AsArrayType(tmpArrayType->subType);
                    if(tmpArrayType == NULL)
                    {
                        isSemanticsOk = false;
                        std::ostringstream tmpStr;
                        dcl->print(tmpStr, true);
                        LOG(LOG_ERROR, "Internal error :%s : %d -- Type information missing for gather array declaration %s", tmpStr.str().c_str());
                        break;
                    }
                }

                arrayType->isSemanticsOk = isSemanticsOk;
            }

        }
    }
    
}

//! Checks kernel parameters semantics which includes storage classes and qualifiers , local variable storage
//! classes and qualifiers and also stream semantics in non-kernel code
void 
SemanticChecker::checkDeclarationSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal)
{
    if (isKernel)
    {
        Type* dclType = dcl->form;

        if (IsScatterOutType(dclType))
        {
			//! Do semantics for scatter array declaration
			checkArrayDeclSemantics(dcl, parent, isKernel, isFormal);
            if (_checkTargetDx9)
                recordWarn(dcl, parent, SCWDx9);

            bool typeok = false;
            Type *baseType = *(static_cast<ArrayType*>(dclType))->getSubType();
            if (baseType)
            {
                if (baseType->isBaseType() || baseType->isArray())
                {
                    typeok = true;
                }
            }
            if (typeok != true)
            {
                recordError(dcl, parent, SCEKDeclDataType, 
                    "Scatter stream not supported for struct.");
            }
        }  else if (isFormal && dclType && 
            (dclType->getQualifiers() & TQ_Out) != 0 &&
            (dclType->getQualifiers() & TQ_Reduce) == 0 &&
            dclType->isStream() == false)
        {
            recordError(dcl, parent, SCEKDeclDataType, 
                "non-reduce output parameter must be stream type.");     
        }

        bool isStream = false;
        if (dclType && dclType->isStream())
        {
            isStream = true;
            checkStreamDeclSemantics(dcl, parent, isKernel, isFormal);
            dclType = dclType->getBase();
        }

        if (isStream && isFormal == false)
            recordError(dcl, parent, SCEKDeclDataType);


		//! check semantics of gather array declaration
        if(dclType && dclType->isArray())
            checkArrayDeclSemantics(dcl, parent, isKernel, isFormal);

        if (_checkTargetDx9 && dclType && isDoubleType(dclType))
        {
            recordWarn(dcl, parent, SCWDx9);
        }

        StructDef* strDef = findStructureDef(dclType);
        if ((strDef && checkKernelStruct(dcl, parent, strDef) == false)
            || (strDef == NULL && isKernelScalarType(dclType) == false))
        {
            if (isStream)
                recordError(dcl, parent, SCEBStreamEleType);
            else
                recordError(dcl, parent, SCEKDeclDataType);
        }

        //! static or extern not allowed inside kernel
        if(dclType->storage == ST_Static || dclType->storage == ST_Extern)
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "static or extern not allowed inside kernel");

        //! volatile and restrict not allowed inside kernel
        if(dclType->getQualifiers() == TQ_Restrict || dclType->getQualifiers() == TQ_Volatile)
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "volatile or Restrict qualifiers not allowed inside kernel");

        //! out qualifier allowed only for kernel parameter
        if(dclType->getQualifiers() == TQ_Out && isFormal == false)
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "out qualifier allowed only for kernel parameters");

        
        //! reduce qualifier can be used as kernel qualifier or kernel parameter qualifier
        if(dclType->getQualifiers() == TQ_Reduce && isFormal == false)
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "reduce qualifier allowed only for kernel parameters");

        //! Vout deprecated
        if(dclType->getQualifiers() == TQ_Vout)
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "vout deprecated");

        //! pointers not allowed inside kernel
        if(dclType->isPointer())
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "pointers not allowed inside kernel");
        
        //! const qualifier not allowed for kernel parameters presently
        if(isFormal && dclType->getQualifiers() == TQ_Const)
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "Nothing special to use const qualifier for kernel parameters. It is a reserved usage");

        //! shared qualifier allowed only for Local Array declared inside kernel
        if(dclType && dclType->isArray() && dclType->getQualifiers() == TQ_Shared && isFormal)
            recordError(dcl, parent, SCEKExprVariable, "Shared qualifier vaild only for Local Array declared inside kernel");
                
        //! shared qualifier allowed only for Arrays
        if(dclType && dclType->isArray() == false && dclType->getQualifiers() == TQ_Shared)
            recordError(dcl, parent, SCEKExprVariable, "Shared qualifier vaild only for Local Array declared inside kernel");
        
    }
    else
    {
        //form->type may becomes NULL ...
        if (!dcl->isStream())
            return;

        if ((dcl->form->getQualifiers() & TQ_Iter) != 0)
        {
            recordError(dcl, parent, SCEBStreamIter);
        }
        else if (dcl->initializer != NULL)
        {
            recordError(dcl, parent, SCEHStreamInit);
        }
        else
        {
            CGASSERT (dcl->form->type == TT_Stream, "internal error");
            ArrayType *brtType = AsArrayType(dcl->form);
 
            if (brtType->subType->isBaseType()==false || brtType->size == NULL)
            {
                recordError(dcl, parent, SCEHStreamOther);
            }

            Expression *exp = brtType->size;
            int numdim = 1;

            while (exp && exp->etype == ET_BinaryExpr && (AsBinaryExpr(exp))->op() == BO_Comma)
            {
                if ((AsBinaryExpr(exp))->rightExpr())
                    checkStreamDimension(dcl, parent, (AsBinaryExpr(exp))->rightExpr());

                exp = (AsBinaryExpr(exp))->leftExpr();
                numdim ++;
            }

            if (exp)
                checkStreamDimension(dcl, parent, exp);

            if (numdim < 1 || numdim >4)
            {
                std::ostringstream tmpmsg;
                tmpmsg << "dimension == " << numdim << " not supported";
                recordError(dcl, parent, SCEHStreamOther, tmpmsg.str().c_str());
            }
        }
    }
}

//! Checks array initializer semantics for given dimensions dim
void 
SemanticChecker::arrayInitiailizerSemantics(Decl *dcl, Statement *parent, ArrayType *arrayDclType, Expression *initializerType, int dim)
{
    BaseTypeInfo *arrayTypeInfo = getBaseTypeInfo(arrayDclType->getBase()->typemask);
    BTDetails arrayBTDetails = getTypeDetails(arrayDclType->getBase()->typemask);
    int elementCount = arrayDclType->getDimensionsSize(dim) * arrayTypeInfo->component;

    //! divide element count by for double2 because components for double2 is 4 we know actually it is 2
    if(BT_Double2 == arrayDclType->getBase()->typemask)
        elementCount = elementCount / 2;

    ArrayConstant *arrayConst = static_cast<ArrayConstant*>(initializerType);
    assert(!arrayConst->items.empty());
    
    if(!arrayConst->items.empty())
    {
        int arrayCount = arrayConst->items.size();
        if(elementCount != arrayCount)
        {
            if(elementCount > arrayCount)
                recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, "Uninitialized components", SemanticChecker::SCWarningLevelOne);
            else
                recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, "Too many initializers", SemanticChecker::SCWarningLevelOne);
        }
        
        //! Get category type name for constructor type
        std::string arrayTypeName = std::string(arrayTypeInfo->brTypeName);
        if(arrayTypeInfo->component != 1)
            arrayTypeName = arrayTypeName.substr(0, arrayTypeName.length() - 1);

        for(int i = 0; i < arrayCount; ++i)
        {

            Constant *constant = static_cast<Constant*>(arrayConst->items[i]);
            if(constant->ctype == CT_Array)
            {
                arrayInitiailizerSemantics(dcl, parent, arrayDclType, arrayConst->items[i], dim++);
            }
            else
            {
                //! Check declaration type and initializers type
                Type *constType = arrayConst->items[i]->getType();
                assert(constType->getBase());
                BaseTypeInfo *constTypeInfo = getBaseTypeInfo(constType->getBase()->typemask);
                BTDetails constDetails = getTypeDetails(constType->getBase()->typemask);

                if(arrayBTDetails.category != constDetails.category)
                {
                    if(globals.enableStrongTypeChecking)
                    {
                        if(!checkForTypeAndComponents(arrayBTDetails, constDetails))
                        {
                            std::ostringstream tmpStr;
                            tmpStr << "Mismatched type for element " << i << ": " 
                                << "actual type is " << constTypeInfo->brTypeName << " but expected type is " << arrayTypeName.c_str();
                            recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                            recordNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                        }
                    }
                    else
                    {
                        if(checkForConversionLose(arrayBTDetails, constDetails))
                        {
                            std::ostringstream tmpStr;
                            tmpStr << "possible lose of data for element " << i << ": " 
                                << "conversion from " << constTypeInfo->brTypeName << " to " << arrayTypeName.c_str();
                            recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        }
                    }
                }
            }
        }

    }
    
}

//! Checks the semantics of all types of initializer 
void 
SemanticChecker::checkInitializerSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal)
{
    //! check semantics of initalizers inside kernel 
    if(isKernel && isFormal == false)
    {
        Type* dclType = dcl->form;

        assert(dclType->getBase());
        
        if(dclType->type == TT_Base) //! Check semantic if declaration type is base type
        {
            BaseTypeInfo *dclTypeInfo = getBaseTypeInfo(dclType->getBase()->typemask);
            BTDetails dclBTDetails = getTypeDetails(dclType->getBase()->typemask);

            if(dcl->initializer->etype == ET_Constant) //! Check semantic if initializer expression type is constant type
            {
                Constant *constant = static_cast<Constant*>(dcl->initializer);
                Type *initType = constant->getType();
                if(constant->ctype == CT_Array)
                {
                    //! Get category type name for constructor type
                    std::string dclTypeName = std::string(dclTypeInfo->brTypeName);
                    dclTypeName = dclTypeName.substr(0, dclTypeName.length() - 1);

                    ArrayConstant *arrayConst = static_cast<ArrayConstant*>(dcl->initializer);
                    
                    if(!arrayConst->items.empty())
                    {
                        int arrayCount = arrayConst->items.size();

                        if(dclBTDetails.components != arrayCount)
                        {
                            if(dclBTDetails.components > arrayCount)
                                recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, "Uninitialized components", SemanticChecker::SCWarningLevelOne);
                            else
                                recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, "Too many initializers", SemanticChecker::SCWarningLevelOne);
                        }
                        
                        for(int i = 0; i < arrayCount; ++i)
                        {

                            //! Check constructor type and constructor initializers type
                            Type *constType = arrayConst->items[i]->getType();
                            assert(constType->getBase());
                            BaseTypeInfo *constTypeInfo = getBaseTypeInfo(constType->getBase()->typemask);
                            BTDetails constDetails = getTypeDetails(constType->getBase()->typemask);

                            if(dclBTDetails.category != constDetails.category)
                            {
                                if(globals.enableStrongTypeChecking)
                                {
                                    if(!checkForTypeAndComponents(dclBTDetails, constDetails))
                                    {
                                        std::ostringstream tmpStr;
                                        tmpStr << "Mismatched type for element " << i << ": " 
                                            << "actual type is " << constTypeInfo->brTypeName << " but expected type is " << dclTypeName.c_str();
                                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                                        recordNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                                    }
                                }
                                else
                                {
                                    if(checkForConversionLose(dclBTDetails, constDetails))
                                    {
                                        std::ostringstream tmpStr;
                                        tmpStr << "possible lose of data for element " << i << ". " 
                                            << "conversion from " << constTypeInfo->brTypeName << " to " << dclTypeName.c_str();
                                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                                    }
                                }
                            }
                        }
                    }
                }
                else if(constant->ctype == CT_Enum || constant->ctype == CT_String)
                {
                    recordError(dcl, parent, SemanticChecker::SCEKInitializerExpr, "Unsupported constant type inside kernel");
                }
                else
                {
                    if(isScalarType(dclBTDetails) == false)
                    {
                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, "Initializer is not vector type", SemanticChecker::SCWarningLevelOne);
                    }
                    else
                    {
                        assert(initType->getBase());
                        BaseTypeInfo *constTypeInfo = getBaseTypeInfo(initType->getBase()->typemask);
                        BTDetails constBTDetails = getTypeDetails(initType->getBase()->typemask);

                        if(constBTDetails.category != dclBTDetails.category)
                        {
                            if(globals.enableStrongTypeChecking)
                            {
                                if(!checkForTypeAndComponents(dclBTDetails, constBTDetails))
                                {
                                    std::ostringstream tmpStr;
                                    tmpStr << "Mismatched type: " 
                                        << "actual type is " << constTypeInfo->brTypeName << " but expected type is " << dclTypeInfo->brTypeName;
                                    recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                                    recordNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                                }
                            }
                            else
                            {
                                if(checkForConversionLose(dclBTDetails, constBTDetails))
                                {
                                    std::ostringstream tmpStr;
                                    tmpStr << " possible lose of data : "
                                           << "conversion from " << constTypeInfo->brTypeName << " to " << dclTypeInfo->brTypeName;
                                    recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                                }
                            }
                        }
                    }
                }
            }
            else if(dcl->initializer->etype == ET_ConstructorExpr)//! Check semantic if initializer expression type is constructor expression type
            {
                Type *vecType = dcl->initializer->getType();
                assert(vecType->getBase());
                BaseTypeInfo *constructTyeInfo = getBaseTypeInfo(vecType->getBase()->typemask);
                BTDetails constructBTDetails = getTypeDetails(vecType->getBase()->typemask);

                //! Check constructor type and declaration type
                BinaryOperandsDetails details = getBinaryOperandsDetails(dclType->getBase()->typemask, vecType->getBase()->typemask);
                std::string warnMsg = "conversion from ";
                warnMsg.append(constructTyeInfo->brTypeName);
                warnMsg.append(" to ");
                warnMsg.append(dclTypeInfo->brTypeName);
                if(globals.enableStrongTypeChecking)
                {
                    if(!checkForTypeAndComponents(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        std::ostringstream tmpStr;
                        tmpStr << "Mismatched types : " << warnMsg.c_str();
                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        recordNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                }
                else
                {
                    if(getWarnMsgForBinarayOperands(details, warnMsg))
                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, warnMsg.c_str(), SemanticChecker::SCWarningLevelOne);
                }

            }
            else
            {
                //! Handle all other expressions
                Type *exprType = dcl->initializer->getType();
                if(exprType->isStructure())
                {
                    std::ostringstream tmpStr;
                    parent->print(tmpStr, 0);
                    LOG(LOG_ERROR, "Structurewise semantics not handled still: %s\n", tmpStr.str().c_str());
                    return;
                }
                assert(exprType->getBase());
                BaseTypeInfo *exprTyeInfo = getBaseTypeInfo(exprType->getBase()->typemask);

                //! Check expression type and declaration type
                BinaryOperandsDetails details = getBinaryOperandsDetails(dclType->getBase()->typemask, exprType->getBase()->typemask);
                
                std::string warnMsg = "conversion from ";
                warnMsg.append(exprTyeInfo->brTypeName);
                warnMsg.append(" to ");
                warnMsg.append(dclTypeInfo->brTypeName);
                if(globals.enableStrongTypeChecking)
                {
                    if(!checkForTypeAndComponents(details.leftOperandDetails, details.rightOperandDetails))
                    {
                        std::ostringstream tmpStr;
                        tmpStr << "Mismatched types : " << warnMsg.c_str();
                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                        recordNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                    }
                }
                else
                {
                    if(getWarnMsgForBinarayOperands(details, warnMsg))
                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, warnMsg.c_str(), SemanticChecker::SCWarningLevelOne);
                }
            }
        }
        else if(dclType->type == TT_Array) //! Check semantic if declaration type is Array type
        {

			//! Check semantics of local array declaration
            checkArrayDeclSemantics(dcl, parent, isKernel, isFormal);
            ArrayType *arrayType = static_cast<ArrayType *>(dclType);
            
            //! Skip array initializer semantic checks if array declaration semantics are wrong
            if(arrayType->isSemanticsOk == false)
                return;
            BaseTypeInfo *arrayTypeInfo = getBaseTypeInfo(arrayType->getBase()->typemask);
            BTDetails arrayBTDetails = getTypeDetails(arrayType->getBase()->typemask);
            
            //! array dimensions
            int dimCount = arrayType->getDimensions();
            if(arrayType->getQualifiers() == TQ_Shared && dcl->initializer != NULL)
            {
                recordError(dcl, parent, SCEKArrayDecl, "Shared Local Array should not be initialized");
                return;
            }
            if(dcl->initializer->etype == ET_Constant)
            {
                Constant *constant = static_cast<Constant*>(dcl->initializer);
                Type *initType = constant->getType();
                if(constant->ctype == CT_Array)
                {
                    int leftDim = 1;
                    //! initializer dimensions
                    int initializerDim = 0;
                    ArrayConstant *arrayConst = static_cast<ArrayConstant*>(dcl->initializer);
                    if(!arrayConst->items.empty())
                    {
                        initializerDim++;
                        Constant *tmpConst = static_cast<Constant*>(arrayConst->items[0]);
                        while(tmpConst->ctype == CT_Array)
                        {
                            initializerDim++;
                            arrayConst = static_cast<ArrayConstant*>(arrayConst->items[0]);
                            assert(!arrayConst->items.empty());
                            tmpConst = static_cast<Constant*>(arrayConst->items[0]);;
                        }
                    }

                    if(initializerDim != dimCount)
                    {
                        recordError(dcl, parent, SemanticChecker::SCEKInitializerExpr, "Mismatched array dimensions and initializer dimensions");
                    }
                    else
                    {
                        //! Semantics on array initializers
                        arrayInitiailizerSemantics(dcl, parent, arrayType, dcl->initializer, leftDim);
                    }

                }
                else if(constant->ctype == CT_Enum || constant->ctype == CT_String)
                {
                    recordError(dcl, parent, SemanticChecker::SCEKInitializerExpr, "Unsupported constant type inside kernel");
                }
                else
                {
                    if(isScalarType(arrayBTDetails) == false && arrayType->getTotalElementCount() != 1)
                    {
                        recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, "Initializer is not vector type", SemanticChecker::SCWarningLevelOne);
                    }
                    else
                    {
                        assert(initType->getBase());
                        BaseTypeInfo *constTypeInfo = getBaseTypeInfo(initType->getBase()->typemask);
                        BTDetails constBTDetails = getTypeDetails(initType->getBase()->typemask);

                        if(constBTDetails.category != arrayBTDetails.category)
                        {
                            if(globals.enableStrongTypeChecking)
                            {
                                if(!checkForTypeAndComponents(arrayBTDetails, constBTDetails))
                                {
                                    std::ostringstream tmpStr;
                                    tmpStr << "Mismatched type: " 
                                        << "actual type is " << constTypeInfo->brTypeName << " but expected type is " << arrayTypeInfo->brTypeName;
                                    recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                                    recordNote(SemanticChecker::SCNoteOnStrongTypeChecking);
                                }
                            }
                            else
                            {
                                if(checkForConversionLose(arrayBTDetails, constBTDetails))
                                {
                                    std::ostringstream tmpStr;
                                    tmpStr << " possible lose of data : "
                                           << " conversion from " << constTypeInfo->brTypeName << " to " << arrayTypeInfo->brTypeName;
                                    recordWarn(dcl, parent, SemanticChecker::SCWInitializerExpr, tmpStr.str().c_str(), SemanticChecker::SCWarningLevelOne);
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(dclType->type == TT_Function) //! Check semantic if declaration type is function type
        {
            dcl->initializer->semanticCheck(isKernel);
        }
        else
        {
            recordError(dcl, parent, SemanticChecker::SCEKExprVariable, "Unsupported variable type");
        }
    }

}
void 
SemanticChecker::checkDecl(Decl *dcl, Statement *parent,  bool isKernel, bool isFormal)
{
    LOG(LOG_FN_ENTRY, "semanticCheckDecl(%p, %p, %d, %d)\n", dcl, parent, isKernel, isFormal);

    //! Check declaration type 
    if (dcl->form == NULL)
    {
        recordError(dcl, parent, SCEBOther, "variable has NULL data type");
        return;
    }

    //! Check semantics all declarations both inside kernel or outside kernel  
    //! including function or kernel parameters
    checkDeclarationSemantics(dcl, parent,isKernel, isFormal);


    //! Check semantics of initializers of all declarations 
    if (dcl->initializer)
    {
        dcl->initializer->semanticCheck(isKernel);
        checkInitializerSemantics(dcl, parent,isKernel, isFormal);
    }
    else 
    {
        //! Only for variable declarations inside kernel
        if(isKernel == true && isFormal == false && dcl->form->type != TT_Function)
        {
            //! Uninitialized const variable
            if(dcl->form->getQualifiers() == TQ_Const)
                recordError(dcl, parent, SCEKExprVariable, "uninitialized const");
            //! Handle other uninitialized variables
            //recordWarn(dcl, parent, SCWNull, "Uninitialized variable");
        }
 
    }
}


//! Checks condtional expression semantics
void
SemanticChecker::checkConditionalSemantics(Expression *condExpr, bool isKernel)
{
    if(isKernel)
    {
        if(condExpr->etype == ET_VoidExpr || condExpr->etype == ET_BrtStreamInitializer)
        {
            LOG(LOG_ERROR, "Unsupported conditional expression");
            assert(0);
            return;
        }
        else
        {
            //! Resultant type of conditional exprssion
            Type *resultantType = condExpr->getType();

            if(resultantType == NULL || resultantType->getBase() == NULL)
            {
                std::ostringstream tmpStr;
                condExpr->print(tmpStr);
                LOG(LOG_ERROR, "Internal error :%s : %d -- Type information missing for conditional expression %s", tmpStr.str().c_str());
                assert(0);
            }
            else
            {
                BaseTypeInfo *resultantTypeInfo = getBaseTypeInfo(resultantType->getBase()->typemask);
                BTDetails details = getTypeDetails(resultantType->getBase()->typemask);
                if(isArithmeticType(details))
                {
                    //! Check for relational builtIn functions like any and all
                    if(condExpr->etype == ET_FunctionCall)
                    {
                        FunctionCall * callExpr = AsFunctionCall(condExpr);
                        std::ostringstream tempStrStream;
                        callExpr->function->print(tempStrStream);
                        std::string tempStr = tempStrStream.str();
                        if(isRelationalBuiltIn(tempStr))
                            return;
                    }
                    
                    if(!isScalarType(details))
                    {
                        //recordWarn(condExpr, SemanticChecker::SCWNull, "conditional expression must have scalar type. On short vectors, assumes x components as condition");
                        recordError(condExpr, SemanticChecker::SCEKNull, "conditional expression must be a scalar data type.");
                        recordNote(SemanticChecker::SCRelationalConditionalExpr);
                    }
                }
                else
                {
                    RecordSemanticError(condExpr, SemanticChecker::SCEKNull, "conditional resultant type is not a Arithmetic type");
                }
            }
        }
    }
}