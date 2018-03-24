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

#ifndef INC_CG_SEMANTIC_H
#define INC_CG_SEMANTIC_H

#include "stemnt.h"

// ----------------------------------------------------------------------------
class SemanticChecker
{
public:

    //! Error IDs
    typedef enum
    {
        //Kernel code, expression
        SCEKNull = 0,
        SCEKExprCall,
        SCEKExprCast,
        SCEKExprStringConst,
        SCEKIndexExpr,
        SCEKExprGather,
        SCEKExprOp,
        SCEKExprSizeof,
        SCEKArrayDecl,
        SCEKExprVariable,
        SCEKInitializerExpr,
        SCEKExprOther,

        SCEBinaryExpr,
        SCERelExpr,
        SCETrinaryExpr,
        //Assignment Expression
        SCEAssignExpr,
        SCEKDeclDataType,

        //kernel code, statement
        SCEKStmtControlFlow,
#ifdef __ASMIL__
	SCEKStmtASM,
#endif /* __ASMIL__ */

        //host code
        SCEHStreamOther,
        SCEHStreamInit,

        //both
        SCEBStreamIter,
        SCEBStreamEleType,
        SCEBOther,

        SCETotal
    } SCErrorID;

    //! Warning IDs
    typedef enum
    {
        SCWNull,
        SCWDx9,
        SCWInitializerExpr,
        SCWBinaryExpr,
        SCWRelExpr,
        SCWTrinaryExpr,
        SCWAssignExpr,
        SCWStream,
        SCWNonKernel, 
        SCWDoubleToFloat,

        SCWTotal
    } SCWarningID;

    //! Warning levels
    typedef enum
    {
        SCWarningLevelZero = 0,
        SCWarningLevelOne,
        SCWarningLevelTwo,
        SCWarningLevelThree,
        SCWarningLevelTotal
    }SCWarningLevel;

    //! Note IDs
    typedef enum
    {
        SCNoteOnStrongTypeChecking = 0,
        SCNoteOnDisableCachedGatherArray,
        SCBackEndCodeGenFailed,
        SCRelationalConditionalExpr,
        SCNoteTotal

    }SCNoteID;

public:
    SemanticChecker() :
        _checkTargetCal(false), _checkTargetDx9(false),
        _passTargetCal(false), _passTargetDx9(false), _outs(std::cerr)
    {
        LOG(LOG_FN_ENTRY, "SemanticChecker::SemanticChecker()\n");
        _errorTotal = 0;
        _warnTotal = 0;
        _maxErrorsAllowed = 10;
    }

    void init(void) {_errorTotal = 0; }
    void fini(void);

    void setErrorMaximum(int i) {_maxErrorsAllowed = i;}
    int getErrorTotal(void) {return _errorTotal;}

    void setCheckTargetCal() {_checkTargetCal = true; _passTargetCal = true;}
    void setCheckTargetDx9() {_checkTargetDx9 = true; _passTargetDx9 = true;}
    bool isTargetCalPass() {return _passTargetCal;}
    bool isTargetDx9Pass() {return _passTargetDx9;}

    void recordError(Statement *stmt, SCErrorID errorID, const char *msg = NULL, bool exithere = false);
    void recordError(Expression *expr, SCErrorID errorID, const char *msg = NULL, bool exithere = false);
    void recordError(Decl *decl, Statement*, SCErrorID errorID, const char *msg = NULL, bool exithere = false);

    void recordWarn(Statement *stmt, SCWarningID warningID, const char *msg = NULL, SCWarningLevel level = SCWarningLevelZero);
    void recordWarn(Expression *expr, SCWarningID warningID, const char *msg = NULL, SCWarningLevel level = SCWarningLevelZero);
    void recordWarn(Decl *decl, Statement*, SCWarningID warningID, const char *msg = NULL, SCWarningLevel level = SCWarningLevelZero);
    void recordNote(SCNoteID noteId, const char *msg = NULL);

public:
    void checkDecl(Decl *dcl, Statement *parent,  bool isKernel, bool isFormal = false);
    void checkStreamDimension(Decl *dcl, Statement *parent, Expression *exp);
    void checkBuiltin(FunctionCall *callExp, Variable *calleeVar);
    bool checkKernelStruct(Decl *dcl, Statement *parent, StructDef* strDef);
    void checkDeclarationSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal);
    void checkStreamDeclSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal);
    void checkArrayDeclSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal);
    void checkInitializerSemantics(Decl *dcl, Statement *parent, bool isKernel, bool isFormal);
    void arrayInitiailizerSemantics(Decl *dcl, Statement *parent, ArrayType *arrayDclType, 
                                    Expression *initializerType, int dim);
    void checkConditionalSemantics(Expression * condExpr, bool isKernel);

private:

    void checkExit(bool exithere);
    void emitError(SCErrorID errorID);
    void emitWarn(SCWarningID warningID, SCWarningLevel level);
    const char* getErrorMsg(SCErrorID errorID);
    const char* getWarningMsg(SCWarningID warningID);
    bool isWarningLevelEnabled(SCWarningLevel level);

private:
    int _errorTotal;
    int _warnTotal;
    int _maxErrorsAllowed;

    bool _checkTargetCal;
    bool _checkTargetDx9;
    bool _passTargetCal;
    bool _passTargetDx9;

    std::ostream &_outs;

    static const char *errorMsg[SCETotal];
    static const char *warningMsg[SCWTotal];
};

extern SemanticChecker semanticChecker;

#define RecordSemanticError semanticChecker.recordError
#define RecordSemanticWarn semanticChecker.recordWarn
#define RecordSemanticNote semanticChecker.recordNote

// ----------------------------------------------------------------------------

#endif // INC_CG_SEMANTIC_H
