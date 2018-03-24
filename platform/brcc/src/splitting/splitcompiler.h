/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

// splitcompiler.h
#ifndef INC_SPLITCOMPILER_H
#define INC_SPLITCOMPILER_H

#ifdef _WIN32
#pragma warning(disable:4786)
//debug symbol warning
#endif

#include "brook/janitorial.hpp"

#include "splitconfiguration.h"

#include <vector>
#include <set>
#include <iostream>
#include <string>
class SplitTree;
class SplitNode;

class SplitShaderHeuristics
{
  public:
    bool valid;                 // whether this shader can be compiled
    int cost;                   // the cost of this shader (if valid)
    bool recompute;             // should we recompute this shader, rather than save it?

    // resource usage...
    int arithmeticInstructionCount;
    int textureInstructionCount;
    int samplerRegisterCount;
    int interpolantRegisterCount;
    int constantRegisterCount;
    int temporaryRegisterCount;
    int outputRegisterCount;
};

class SplitCompiler
{
  public:
    typedef std::vector < SplitNode * >NodeList;
    typedef std::set < SplitNode * >NodeSet;

    void compile(const SplitTree & inTree, const NodeList & inOutputs, std::ostream & inStream,
                 SplitShaderHeuristics & outHeuristics, bool forReal = false) const;

    // TIM: complete hack, even for me
    virtual bool mustScaleAndBiasGatherIndices() const
    {
        return false;
    }
    virtual ~ SplitCompiler()
    {
    }

  protected:
          virtual void printHeaderCode(std::ostream & UNUSED(inStream)) const
    {
    };
    virtual void printFooterCode(std::ostream & UNUSED(inStream)) const
    {
    };
    virtual void compileShader(const std::string & inHighLevelCode, std::ostream & inAssemblerStream,
                               const SplitConfiguration & inConfiguration,
                               SplitShaderHeuristics & outHeuristics) const = 0;

    void printStringConstant(const std::string & inString, const std::string & inPrefix, std::ostream & inStream) const;
};

#endif // INC_SPLITCOMPILER_H
