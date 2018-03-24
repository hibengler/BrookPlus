/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

// splitbuilder.h
#ifndef INC_SPLITBUILDER_H
#define INC_SPLITBUILDER_H
#ifdef _WIN32
#pragma warning(disable:4786)
//debug symbol warning
#endif
#include <string>
#include <map>
#include "../stemnt.h"

#include "splittypes.h"

class SplitTree;
class SplitCompiler;

class SplitTreeBuilder
{
  public:
    SplitTreeBuilder(SplitTree & ioTree);

    SplitNode *addArgument(Decl * inDeclaration, int inArgumentIndex);
    SplitNode *addArgument(Decl * inDeclaration, int inArgumentIndex, SplitNode * inValue);
    void addVariable(const std::string & inName, Type * inForm);

    SplitNode *addConstant(Constant * inConstant);
    SplitNode *addConstant(int inValue);
    SplitNode *addMember(SplitNode * inValue, const std::string & inName);
    SplitNode *addUnaryOp(const std::string & inOperation, SplitNode * inOperand);
    SplitNode *addBinaryOp(const std::string & inOperation, SplitNode * inLeft, SplitNode * inRight);
    SplitNode *addGather(SplitNode * inStream, const std::vector < SplitNode * >inIndices);
    SplitNode *addCast(BaseType * inType, SplitNode * inValue);
    SplitNode *addConstructor(BaseType * inType, const std::vector < SplitNode * >&inArguments);
    SplitNode *addConstructor(SplitBasicType inType, SplitNode * inX = 0, SplitNode * inY = 0, SplitNode * inZ =
                              0, SplitNode * inW = 0);
    SplitNode *addIndexof(const std::string & inName);
    SplitNode *addConditional(SplitNode * inCondition, SplitNode * inConsequent, SplitNode * inAlternate);
    SplitNode *findVariable(const std::string & inName);
    SplitNode *assign(const std::string & inName, SplitNode * inValue);

    SplitNode *addFunctionCall(Expression * inFunction, const std::vector < SplitNode * >&inArguments);

    SplitNode *getResultValue()
    {
        return _resultValue;
    }

    void setResultValue(SplitNode * inValue)
    {
        _resultValue = inValue;
    }

    SplitNode *getOutputInterpolant();

  private:
    typedef std::map < std::string, SplitNode * >NodeMap;
    NodeMap nodeMap;

    SplitTree & tree;
    const SplitCompiler & compiler;
    SplitNode *_resultValue;
};

#endif // INC_SPLITBUILDER_H
