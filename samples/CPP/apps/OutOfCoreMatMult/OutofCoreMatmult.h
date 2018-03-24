#ifndef _OUTOFCOREMATMULT_H_
#define _OUTOFCOREMATMULT_H_

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
  \file OutofCoreMatmult.h
  \brief Contains the class definition for OutofCoreMatmult class
 ****************************************************************************/

#include "SampleBase.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class  OutofCoreMatmult
//!
//! \brief  contains application properties for the sample out_of_core_matmult
//! 
////////////////////////////////////////////////////////////////////////////////

class OutofCoreMatmult : public SampleBase
{
    public:

        OutofCoreMatmult(char* name);
        void allocateBuffer();
        void fillInputBuffer();
        int run();
        void printTimeInfo();
        void verifyResults();
        void comparePerformance();
        ~OutofCoreMatmult();

    private:

        void _divideMatrixB(float* matrix, float* matrixParts[][4], unsigned int* splitOffsetB,
                           unsigned int index, unsigned int width, unsigned int height, unsigned int parts);
        void _copyOutput(float* matrix, float* tempOutput, unsigned int height,
                   unsigned int width, unsigned int loopCount, unsigned int *heightOffset,
                   unsigned int *widthOffset);
        void _matmultCPU(float* a, float* b, float* c, int m, int k, int n);

    private:

        float* _inputA;
        float* _inputB;
        float* _output;
};

#endif // _OUTOFCOREMATMULT_H_
