/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

// splitconfiguration.h
#ifndef __SPLITCONFIGURATION_H__
#define __SPLITCONFIGURATION_H__

#ifdef _WIN32
#pragma warning(disable:4786)
//debug symbol warning
#endif

class SplitConfiguration
{
  public:
    SplitConfiguration();

    void load(const char *inFileName);
    void save(const char *inFileName);

    bool validateShaders;

    int maximumArithmeticInstructionCount;
    int maximumTextureInstructionCount;
    int maximumSamplerCount;
    int maximumInterpolantCount;
    int maximumConstantCount;
    int maximumTemporaryCount;
    int maximumOutputCount;

    int passCost;
    int textureInstructionCost;
    int arithmeticInstructionCost;
    int samplerCost;
    int interpolantCost;
    int constantCost;
    int temporaryCost;
    int outputCost;
};

#endif
