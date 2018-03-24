/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

// splitcompilercg.h
#ifndef __SPLITCOMPILERCG_H__
#define __SPLITCOMPILERCG_H__

#ifdef _WIN32
#pragma warning(disable:4786)
//debug symbol warning
#endif

#include "splitcompiler.h"
#include "../codegen.h"
#include <string>

class SplitCompilerCg:public SplitCompiler
{
  public:
    SplitCompilerCg(CodeGenTarget inTarget):_target(inTarget)
    {
    }

  protected:
          virtual void printHeaderCode(std::ostream & inStream) const;
    virtual void compileShader(const std::string & inHighLevelCode, std::ostream & outLowLevelCode,
                               const SplitConfiguration & inConfiguration, SplitShaderHeuristics & outHeuristics) const;

  private:
    CodeGenTarget _target;
};

#endif
