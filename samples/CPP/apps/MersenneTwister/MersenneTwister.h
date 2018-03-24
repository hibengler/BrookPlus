#ifndef _MERSENNETWISTER_H_
#define _MERSENNETWISTER_H_

/****************************************************************************

Copyright (c) 2008, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Advanced Micro Devices, Inc nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

****************************************************************************/
/*****************************************************************************
  \file MersenneTwister.h
  \brief Contains the class definition for MersenneTwister class
 ****************************************************************************/

#include "SampleBase.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class  MersenneTwister
//!
//! \brief  contains application properties for the sample mesenne_twister
//! 
////////////////////////////////////////////////////////////////////////////////

class MersenneTwister : public SampleBase
{
    public:

        MersenneTwister(char* name);
        void allocateBuffer();
        void fillInputBuffer();
        int run();
        void printTimeInfo();
        void verifyResults();
        void comparePerformance();
        ~MersenneTwister();

    private:
        void initializeGenerator_cpu(unsigned int seed, unsigned int *M, unsigned int N);
        void generateNumber_cpu(unsigned int *M, unsigned int N);
        void extractNumber_cpu(unsigned int *M, unsigned int N);
        void boxMullar_cpu(unsigned int *M, float *BM, unsigned int N);

    private:

        unsigned int* _seedArray;
        float* _gpuResult;

        unsigned int _width;
};

#endif // _MERSENNETWISTER_H_
