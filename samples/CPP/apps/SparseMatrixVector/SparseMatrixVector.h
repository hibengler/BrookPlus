#ifndef _PREFIXSUM_H_
#define _PREFIXSUM_H_

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
  \file SparseMatrixVector.h
  \brief Contains the class definition for SparseMatrixVector class
 ****************************************************************************/

#include "SampleBase.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class  SparseMatrixVector
//!
//! \brief  Contains application properties for the sample sparse_matrix_vector
//! 
////////////////////////////////////////////////////////////////////////////////

class SparseMatrixVector : public SampleBase
{
    public:

        SparseMatrixVector(char* name);
        void prefix_sum_cpu(float* input, float* output, int len);
        void allocateBuffer();
        void fillInputBuffer();
        int run();
        void printTimeInfo();
        void verifyResults();
        void comparePerformance();
        ~SparseMatrixVector();

    private:

        void _reshuffleData(float* nz, int* cols, int* rowStart, float* Anz,
                            float* Acols, unsigned int size, unsigned int nzWidth);
        void _createCSRFromSparseMatrix(float* matrixA, float* csrNz, int* csrCols,
                                        int* csrRowStart, unsigned int size, unsigned int length);

    private:

        // stores compressed sparse row representation of the matrix on disk
        int* _csrCols;      // corresponding column
        float* _csrNz;         // nonzero values

        // index into csrNz of first nz element in each row
        // note: number of nz in row i = csrRowStart[i+1] - csrRowStart[i]
        int* _csrRowStart; 

        // storage for ITPACK representation of the matrix
        float* _Anz; // nonzeros of A
        float* _cIdx; // column indices

        float* _x; // X data
        float* _y; // Y data
        float* _matrixA;     // Y = AX.

        unsigned int _size;
        unsigned int _length;
        unsigned int _nzWidth;
};

#endif // _PREFIXSUM_H_
