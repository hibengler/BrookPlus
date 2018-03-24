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

#include "OutofCoreMatmult.h"
#include "brookgenfiles/out_of_core_matmult.h"

// SPLIT_FACTOR is the max dimension supported for the particular system
// configuration. Can be increased or decreased according to system capabilities.
// Maximum SPLIT_FACTOR dimension leads maximum performance gain.
#define SPLIT_FACTOR 4096 * 4096

// The input matrices could be splitted in max of 128 parts (always power of 2).
// Can decrease or increase according to system capabilities.
#define MAX_SPLIT 32

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

OutofCoreMatmult::OutofCoreMatmult(char* name)
                 :SampleBase(name)
{
    _inputA = NULL;
    _inputB = NULL;
    _output = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
OutofCoreMatmult::allocateBuffer()
{
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;

    _inputA = Util::allocateBuffer<float>(Width, Height);
    _inputB = Util::allocateBuffer<float>(Width, Height);
    _output = Util::allocateBuffer<float>(Height, Height);

    if (!_inputA || !_inputB || !_output)
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
OutofCoreMatmult::fillInputBuffer()
{
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    
    Util::fillBuffer(_inputA, Width, Height, 1, 1, Util::RANDOM);
    Util::fillBuffer(_inputB, Width, Height, 1, 1, Util::RANDOM);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  C = A * B Simple Matrix multiplication on the CPU
//! \param    a    Input matrix (m x k)
//! \param    b    Input matrix (k x n)
//! \param    c    Output matrix (m x n)
//! \param    m    Rows in a
//! \param    k    Rows in b
//! \param    n    Columns in b
//!
////////////////////////////////////////////////////////////////////////////////
void 
OutofCoreMatmult::_matmultCPU(float* a, float* b, float* c, int m, int k, int n)
{
    int y;
    for (y = 0; y < m; y++)
    {
        int x;
        for (x = 0; x < n; x++)
        {
            float temp = 0;
            int z;
            for (z = 0; z < k; z++)
            {
                temp += a[y * k + z] * b[z * n + x];
            }

            c[y * n + x] = temp;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
OutofCoreMatmult::printTimeInfo()
{
    int Width = info->Width;
    int Height = info->Height;

    double time = timer->GetElapsedTime();
    double gflop = (double)(2.0 * (double)Width * Height * Height * info->Iterations)/((double)1024 * 1024 * 1024 * time);
    
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::setw(16) << "Gflops" << std::endl;

    std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
                  << std::setw(16) << info->Iterations << std::setw(16) << time
                  << std::setw(16) << gflop << std::endl;

    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
OutofCoreMatmult::verifyResults()
{
    int Width = info->Width;
    int Height = info->Height;
    int res = 0;
    float *CPU = Util::allocateBuffer<float>(Height, Height);
    
    if (CPU)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Performing Matrix Multiplication on CPU ... ";

        _matmultCPU((float*)_inputA, (float*)_inputB, (float*)CPU, Height, Width, Height);

        std::cout << "Done\n";

        res += Util::compareBuffers<float>(CPU, _output, Height * Height);
        if (res)
        {
            std::cout << _name << ": Failed!\n\n";
        }
        else
        {
            std::cout << _name << ": Passed!\n\n" ;
        }

        delete [] CPU;
    }
    else
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compare performance with CPU if requested
//! 
////////////////////////////////////////////////////////////////////////////////
void
OutofCoreMatmult::comparePerformance()
{
    int Width = info->Width;
    int Height = info->Height;
    unsigned int i;
    double cpu_time = 0.0;
    double gpu_time = 0.0;
    double gflop = (double)(2.0 * (double)Width * Height * Height * info->Iterations)/(double)(1024 * 1024 * 1024);
    float *CPU = Util::allocateBuffer<float>(Height, Height);

    gpu_time = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();

    if (CPU)
    {
        std::cout << "-p Compare performance with CPU.\n";
        
        // Record CPU Total time
        
        for(i = 0; i < info->Iterations; i ++)
        {
            _matmultCPU((float*)_inputA, (float*)_inputB, (float*)CPU, Height, Width, Height);
        }
        
        timer->Stop();

        // Print CPU timing and speedup
        cpu_time = timer->GetElapsedTime();
        
        std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
                  << std::setw(16) << "GPU Total Time" << std::setw(16) << "Gflops"
                  << std::setw(16) << "Speedup" << std::endl;

        std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
                  << std::setw(16) << info->Iterations << std::setw(16) << cpu_time
                  << std::setw(16) << gpu_time << std::setw(16) << gflop / gpu_time
                  << std::setw(16) << cpu_time / gpu_time << std::endl;
        
        std::cout << std::endl << std::endl;
        
        delete [] CPU;
    }
    else
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Merges sub matrices into final matrix 
//
// \!param matrix       final matrix
// \!param tempOutput   sub matrix 
// \!param height       height of sub matrix 
// \!param width        width of sub matrix
// \!param loopCount    number of sub matrix for any of the input matrix
// \!param heightOffset the offset of final matrix height, specify where to start copying
// \!param widthOffset  the offset of final matrix width, specify where to start copying
////////////////////////////////////////////////////////////////////////////////////////

void 
OutofCoreMatmult::_copyOutput(float* matrix, float* tempOutput, unsigned int height,
                   unsigned int width, unsigned int loopCount, unsigned int *heightOffset,
                   unsigned int *widthOffset){
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    unsigned int m = 0;
    unsigned int N = width * loopCount;
    
    j = *heightOffset * N + *widthOffset;
    while(k < (height * width))
    {
        memcpy(&matrix[j],&tempOutput[k], width * sizeof(float));
        k += width;
        j += N;
    }
    *widthOffset += width;
    if(*widthOffset == N)
    {
        *heightOffset += height;
        *widthOffset = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief              Function to split Matrics B into 4 parts. 
//
// \!param matrix       input matrix B.
// \!param matrixParts  bParts of input matrix B.
// \!param index        index of sub matrices of input matrix B.
// \!param width        width of a sub matrix of input matrix B.
// \!param height       height of a sub matrix of input matrix B.
// \!param parts        number of parts of bParts[], typically it is 4.
////////////////////////////////////////////////////////////////////////////////////////////////

void
OutofCoreMatmult::_divideMatrixB(float* matrix, float* matrixParts[][4], unsigned int* splitOffsetB,
                                unsigned int index, unsigned int width, unsigned int height, unsigned int parts)
{
    long unsigned int i = 0;
    long unsigned int j = 0;
    long unsigned int k = 0;
    for(k = 0; k < height; k++)
    {
        i = k % parts;
        j = k * info->Height + *splitOffsetB;
        memcpy(&matrixParts[index][i][(k / parts) * width], &matrix[j], width * sizeof(float));
    }
    *splitOffsetB += width;
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for simple matmult sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
OutofCoreMatmult::run()
{
    unsigned int retVal = 0;
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    unsigned int reductionFactor = 1;
    unsigned int splitOffsetB = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    long long unsigned int Length = Width * Height;

    float* bParts[MAX_SPLIT][4];
    
    // Initialize the 2-D buffer
    for(i = 0; i < MAX_SPLIT; i++)
    {
        for(j = 0; j < 4; j++)
        {
            bParts[i][j] = NULL;
        }
    }

    // Calculate number of splits required for input matrices,
    // allocate memory for temporary buffers according to reductionFactor
    if(Length > SPLIT_FACTOR)
    {
        unsigned int constFactor = 0;
        unsigned int loopCount1 = 0;
        unsigned int loopCount2 = 0;
        long long unsigned int temp = Height;
        
        long unsigned int dimM = 0;
        long unsigned int dimN = 0;
        
        while((temp * Width)> SPLIT_FACTOR)
        {
            temp = temp / 2;
            constFactor++;
        }

        reductionFactor = (unsigned int)(pow((double)2,(double)constFactor));
        unsigned int widthDimB  = Height / reductionFactor;
        
        // Allocate Memory for _bParts[]
        for(i = 0; i < reductionFactor; i++)
        {
            for(j = 0; j < 4; j++)
            {
                bParts[i][j] = Util::allocateBuffer<float>(widthDimB, Width / 4);
            }
        }
        
        for(unsigned int i = 0; i < reductionFactor; i++)
        {
            _divideMatrixB(_inputB, bParts, &splitOffsetB, i, widthDimB, Width, 4);
        }
    }
    else
    {
        // Allocate memory for _bParts[]
        for(i = 0; i < reductionFactor; i++)
        {
            for(j = 0; j < 4; j++)
            {
                bParts[i][j] = Util::allocateBuffer<float>(Height, Width / 4);
            }
        }

        // Divide _inputB matrix into 4 equal parts to _bParts[]
        _divideMatrixB(_inputB, bParts, &splitOffsetB, 0, Height, Width, 4);
    }

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////

    // Calculate dimensions for each split of matrices
    unsigned int dimM = Height / reductionFactor;
    unsigned int dimN = Height / reductionFactor;
    unsigned int dimK = Width;

    // Stream dimensions
    unsigned int dimA1 = dimM /8;
    unsigned int dimA2 = dimK /4;
    unsigned int dimB1 = dimK /4;
    unsigned int dimB2 = dimN /4;
    unsigned int dimC1 = dimA1;
    unsigned int dimC2 = dimB2;

    unsigned int widthOffset = 0;
    unsigned int heightOffset = 0;
    unsigned int heightSplitOffset = 0; 

    unsigned int dimAPart[] = {dimA2, dimA1};
    unsigned int dimBPart[] = {dimB2, dimB1};
    unsigned int dimCPart[] = {dimC2, dimC1};
    float *tempOutput = NULL;

    ::brook::Stream<float4>* A[8];
    ::brook::Stream<float4>* B[4];
    ::brook::Stream<float4>* C[8];

    for(unsigned int x = 0; x < 8; x++)
    {
        A[x] = new ::brook::Stream<float4>(2, dimAPart);
        C[x] = new ::brook::Stream<float4>(2, dimCPart);
        
        if(x < 4)
        {
            B[x] = new ::brook::Stream<float4>(2, dimBPart);
        }
    }

    timer->Start();

    if(reductionFactor > 1)
    {
        i = 0;
        tempOutput = Util::allocateBuffer<float>(dimM, dimN);

        while(i < reductionFactor)
        {
            //Read 8 equal parts into A1, A2, A3, A4, A5, A6, A7, A8 respectively
            for(unsigned int x = 0; x < 8; x++)
            {
                A[x]->read(&_inputA[heightSplitOffset + (dimM / 8) * x * Width]);
            }

            // calculate height offset for next sub matrix of _inputA 
            heightSplitOffset += dimM * dimK;
            j = 0;

            while(j < reductionFactor)
            {
                // Read 4 equal parts into B1, B2, B3, B4 respectively
                for(unsigned int y = 0; y < 4; y++)
                {
                    B[y]->read(bParts[j][y]);
                }

                // Call the 8 MRT kernel for GPU computation
                optimized_matmult(dimA2, *A[0], *A[1], *A[2], *A[3], *A[4], *A[5], *A[6], *A[7],
                                               *B[0], *B[1], *B[2], *B[3],
                                               *C[0], *C[1], *C[2], *C[3], *C[4], *C[5], *C[6], *C[7]);

                // Write the results into a sub resultant matrix
                for(unsigned int z = 0; z < 8; z++)
                {
                    C[z]->write(&tempOutput[z *(dimM / 8)* dimN]);
                    
                    if(C[z]->error())
                    {
                        std::cout << "Error occured" << std::endl;
                        std::cout << C[z]->errorLog() << std::endl;
                        retVal = -1;
                    }
                }

                // Copy results to final matrix
                _copyOutput(_output, tempOutput, dimM, dimN,
                           reductionFactor, &widthOffset, &heightOffset);
                j++;
            }
            i++;
        }
        
        delete [] tempOutput;
    }
    else
    {
        //Read 8 equal parts into A1, A2, A3, A4, A5, A6, A7, A8 respectively
        for(unsigned int x = 0; x < 8; x++)
        {
            A[x]->read(&_inputA[heightSplitOffset + (Height / 8) * x * Width]);
        }

        // Read 4 equal parts into B1, B2, B3, B4 respectively
        for(unsigned int y = 0; y < 4; y++)
        {
            B[y]->read(bParts[0][y]);
        }

        // Call the 8 MRT kernel for GPU computation
        optimized_matmult(dimA2, *A[0], *A[1], *A[2], *A[3], *A[4], *A[5], *A[6], *A[7],
                                 *B[0], *B[1], *B[2], *B[3],
                                 *C[0], *C[1], *C[2], *C[3], *C[4], *C[5], *C[6], *C[7]);

        // Write the results into a final output matrix
        for(unsigned int z = 0; z < 8; z++)
        {
            C[z]->write(&_output[z * (Height /8) * Height]);
            
            if(C[z]->error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << C[z]->errorLog() << std::endl;
                retVal = -1;
            }
        }
    }

    timer->Stop();
    
    for(unsigned int x = 0; x < 8; x++)
    {
        delete A[x];
        delete C[x];
        
        if(x < 4)
        {
            delete B[x];
        }
    }

    // Delete memory for bParts
    for(i = 0; i < reductionFactor; i++)
    {
        for(j = 0; j < 4; j++)
        {
            delete [] bParts[i][j];
        }
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

OutofCoreMatmult ::~OutofCoreMatmult()
{
    delete [] _inputA;
    delete [] _inputB;
    delete [] _output;
}
