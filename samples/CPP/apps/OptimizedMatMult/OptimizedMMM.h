#ifndef _OPTIMIZEDMMM_H_
#define _OPTIMIZEDMMM_H_

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
  \file OptimizedMMM.h
  \brief Contains the class definition for OptimizedMMM class
 ****************************************************************************/

#include "SampleBase.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class  OptimizedMMM
//!
//! \brief  contains application properties for the sample optimized_matmult
//! 
////////////////////////////////////////////////////////////////////////////////

class OptimizedMMM : public SampleBase
{
    public:

        OptimizedMMM(char* name);
        void prefix_sum_cpu(float* input, float* output, int len);
        void allocateBuffer();
        void fillInputBuffer();
        int run();
        void printTimeInfo();
        void verifyResults();
        void comparePerformance();
        ~OptimizedMMM();

    private:

        void _matmultCPU(float* a, float* b, float* c, int m, int k, int n);

        // Divides a matrix into parts
        void _divide(float* whole, float* part[], unsigned int numParts,
                        unsigned int width, unsigned int height);

        // Combines a set of parts into a single matrix
        void _combine(float* whole, float* part[], unsigned int numParts,
                        unsigned int width, unsigned int height);

    private:

        //! First matrix
        float* _inputA;

        //! Second matrix
        float* _inputB;

        //! Result matrix
        float* _output;

        //! Height of the first matrix
        int _m;

        //! Width of the first matrix/Height of the second matrix
        int _k;

        //! Width of the second matrix
        int _n;
};

#endif // _OPTIMIZEDMMM_H_
