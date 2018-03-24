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

#ifndef INC_CG_FUNCTION_H
#define INC_CG_FUNCTION_H

#include <string.h>

#include "cgTool.h"
#include "project.h"
#include "stemnt.h"

#include "cgResource.h"
#include "cgKernel.h"

// ----------------------------------------------------------------------------
class CgFunction;
class CgEdge
{
    friend class CgFunction;
public:
    CgEdge(CgFunction *);
protected:
    CgFunction *funcNode;
    CgEdge *nextEdge;
    //other edge info
};

// ----------------------------------------------------------------------------
class CgProgram;

class CgFunction
{
    friend class CgProgram;
public:
    CgFunction(FunctionDef *fDef, CgProgram*);
    ~CgFunction();

    virtual void createResource(void);
    virtual void codegen(std::ostream &outs);
    virtual void allocateResource(CgResourceManager&, int factor, bool outaddr, bool inaddr = false);
    virtual void genPassCallmain(std::ostream &outs, char* linePrefix);
    virtual void genPassLocal(std::ostream &outs, bool, bool, int, char* linePrefix, OutputStreamType outType);
    virtual void genAsSupport(std::ostream &shader, CgKernel*);

    virtual char* getTargetString();

    void setTarget(bool v) {target = v;}

    //query
    bool isReduction() {return isReduce;}
    bool isEntry() {return funcDef->returnsVoid();}
    std::string& getFuncName();
    ShaderType getShaderType() const {return funcDef->getShaderType();}
    GccAttrib* getGroupSize() const { return funcDef->getGroupSize();}
    int getLocalDataShareSize() const {return funcDef->getLocalDataShareSize();}
    int getSharedArrayTypeComponents() const {return funcDef->getSharedArrayTypeComponents();}
    bool isThereInstanceInGroup() const { return hasInstanceInGroup;}

    //codegen
    void cpuCodegen(std::ostream &outs, std::ostream &kernelDeclHeader);

    void irEdit_3();
    void irEdit_4();
    void irEdit_5(bool);

    void genPassIndexofAssign(std::ostream &outs, bool, bool, int, char* linePrefix, int, int);
    void genPassGlobal(std::ostream &outs, char* linePrefix);
    void genPassConstAndOutput(std::ostream &outs, char*, CgPass &pass, CgKernel*, OutputStreamType outType);
    void genPassInputs(std::ostream &outs, char*, CgPass &pass, CgKernel *cgkernel);
    void genPassStreamfetch(std::ostream &outs, bool, bool, int, char* linePrefix);
    void genPassStreamstore(std::ostream &outs, char* linePrefix, int, int, OutputStreamType outType);
    void genPassSubroutine(std::ostream &shader, CgKernel*);
    void genPassTypedef(std::ostream &shader);

    //semantic check
    int semanticCheck(void);
    int collectIndexof(void);

    int getNumScatterArray() {return numScatterArray;}
    //call graph
    int addCallee(Variable*);
    void addEdge();
    int addInEdge(CgFunction*);
    int addOutEdge(CgFunction*);

    //query
    bool IsReduce() {return isReduce;}
    CgProgram * getCgProgram() {return cgProgram;}
    CgResourceParam *getReduceoutStream(void);
    CgResourceParam *getReduceinStream(void);

    CgResourceParam *findResource(std::string &);
    SymEntry *findFormal(std::string &) const;

    int getResourceTotal() const {return AsInt(paramResource.size());}
    CgResourceParam *getResourceParam(int i) {return paramResource[i];}

    bool isCurrentMain() const {return isCurMain;}
    void setCurrentMain(bool v) {isCurMain = v;}

private:

    void genCpuKernel(std::ostream &outs);
    void genKernelStub(std::ostream &outs, std::ostream &kernelDeclHeader);

    int addEdge(CgEdge**, CgFunction *);

    int setIndexof(Symbol *formalArg);
    int setInstance(Variable* formalArg);
    void setInstanceInGroup(bool isThere) { hasInstanceInGroup = isThere;}
    int propagateIndexof(FunctionCall *fcall);

protected:

    CgFunction *next;
    FunctionDef *funcDef;
    FunctionDef *keepFuncDef; //the store the irEdit_3 result
    CgProgram* cgProgram;
    int numArg;

    CgResourceParamTbl paramResource;
    CgResourceTbl otherResource;
    //CgResourceManager resourceManager;

    CgEdge *inEdge;
    CgEdge *outEdge;

    bool target;

    bool isReduce;   //reduction? map?
    bool isRecurs;
    bool isMain;
    bool isSub;
    bool isCurMain;

    int numScatterArray;

    //! set if kernel using instanceInGroup() intrinsic
    bool hasInstanceInGroup;

private:
    static CgFunction *curCgFunc;
    static std::ostream *curShader;
    static Expression *impAddEdge(Expression *exp);
    static Expression *impCollectIndexof(Expression *expr);
    static void impgenPassTypedef(Statement *stmt);
};

#endif // INC_CG_FUNCTION_H
