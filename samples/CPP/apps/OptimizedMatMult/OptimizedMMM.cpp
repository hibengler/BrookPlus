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

#include "OptimizedMMM.h"
#include "brookgenfiles/optimized_matmult.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

OptimizedMMM::OptimizedMMM(char* name) : SampleBase(name)
{
    _inputA = NULL;
    _inputB = NULL;
    _output = NULL;
    _m = 0;
    _k = 0;
    _n = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
OptimizedMMM::allocateBuffer()
{
    // Shift right then left to make sure we are a multiple of 8
    _m = (info->Height >> 3) << 3;

    // Assume output matrix is a sqaure matrix
    _n = _m;

    // Shift right then left to make sure we are a multiple of 4
    _k = (info->Width >> 2) << 2;

    if(!_m || !_k)
    {
        fprintf(stderr, "Error: Minimum size for Width/Height is 8.\n");
        exit(-1);
    }

    _inputA = Util::allocateBuffer<float>(_k, _m);
    _inputB = Util::allocateBuffer<float>(_n, _k);
    _output = Util::allocateBuffer<float>(_n, _m);

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
OptimizedMMM::fillInputBuffer()
{
    Util::fillBuffer(_inputA, _k, _m, 1, 1, Util::RANDOM);
    Util::fillBuffer(_inputB, _n, _k, 1, 1, Util::RANDOM);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  C = A * B
//!         Simple Matrix multiplication on the CPU
//!
//! \param a    Input matrix (m x k)
//! \param b    Input matrix (k x n)
//! \param c    Output matrix (m x n)
//! \param m    Rows in a
//! \param k    Rows in b
//! \param n    Columns in b
//! 
////////////////////////////////////////////////////////////////////////////////

void 
OptimizedMMM::_matmultCPU(float* a, float* b, float* c, int m, int k, int n)
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
OptimizedMMM::printTimeInfo()
{
    double time = timer->GetElapsedTime();
    double gflop = (double)(2.0 * _m * _k * _n * info->Iterations)/((double)1024 * 1024 * 1024 * time);
    
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::setw(16) << "Gflops" << std::endl;

    std::cout << std::setw(8) << _k << std::setw(8) << _m
                  << std::setw(16) << info->Iterations << std::setw(16) << time
                  << std::setw(16) << gflop / time << std::endl;

    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
OptimizedMMM::verifyResults()
{
    int res = 0;
    float *CPU = Util::allocateBuffer<float>(_n, _m);

    if (CPU)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Performing Matrix Multiplication on CPU ... ";

        _matmultCPU(_inputA, _inputB, CPU, _m, _k, _n);

        std::cout << "Done\n";

        res += Util::compareBuffers<float>(CPU, _output, _m * _n);
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
OptimizedMMM::comparePerformance()
{
    unsigned int i;
    double cpu_time = 0.0;
    double gpu_time = timer->GetElapsedTime();
    double gflop = (double)(2.0 * _m * _k * _n * info->Iterations)/(double)(1024 * 1024 * 1024);
    float *CPU = Util::allocateBuffer<float>(_n, _m);

    if (CPU)
    {
        std::cout << "-p Compare performance with CPU.\n";

        // Record CPU Total time

        timer->Reset();
        timer->Start();

        for(i = 0; i < info->Iterations; i ++)
        {
            _matmultCPU((float*)_inputA, (float*)_inputB, (float*)CPU, _m, _k, _n);
        }

        timer->Stop();

        // Print CPU timing and speedup
        cpu_time = timer->GetElapsedTime();

        std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
                  << std::setw(16) << "GPU Total Time" << std::setw(16) << "Gflops"
                  << std::setw(16) << "Speedup" << std::endl;

        std::cout << std::setw(8) << _k << std::setw(8) << _m
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

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for simple matmult sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
OptimizedMMM::run()
{
    unsigned int retVal = 0;

    // Stream dimensions divided bt number of tiles
    int m8 = _m >> 3;
    int k4 = _k >> 2;
    int n4 = _n >> 2;

    // Allocate tiled buffers
    float* Aparts[8] = {NULL};
    float* Bparts[4] = {NULL};
    float* Cparts[8] = {NULL};

    for (unsigned int i = 0; i < 8; i++)
    {
        Aparts[i] = Util::allocateBuffer<float>(_k, m8);
        Cparts[i] = Util::allocateBuffer<float>(_n, m8);
        if (!Aparts[i] || !Cparts[i])
        {
            fprintf(stderr, "Error: Memory Allocation.\n");
            exit(-1);
        }
    }

    for (unsigned int i = 0; i < 4; i++)
    {
        if (!(Bparts[i] = Util::allocateBuffer<float>(_n, k4)))
        {
            fprintf(stderr, "Error: Memory Allocation.\n");
            exit(-1);
        }
    }

    // Divide the input buffers into smaller buffers

    _divide(_inputA, Aparts, 8, _k, _m);
    _divide(_inputB, Bparts, 4, _n, _k);


    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////

    // Stream dimensions
    unsigned int dimA[] = {k4, m8};
    unsigned int dimB[] = {n4, k4};
    unsigned int dimC[] = {n4, m8};

    Stream<float4>* streamA[8];
    Stream<float4>* streamB[4];
    Stream<float4>* streamC[8];

    // declare input streams
    for(unsigned int i = 0; i < 8; ++i)
    {
        streamA[i] = new Stream<float4>(2, dimA);
    }

    for(unsigned int i = 0; i < 4; ++i)
    {
        streamB[i] = new Stream<float4>(2, dimB);
    }

    // declare output streams
    for(unsigned int i = 0; i < 8; ++i)
    {
        streamC[i] = new Stream<float4>(2, dimC);
    }

    // Record GPU Total time
    timer->Start();

    for (unsigned int itr = 0; itr < info->Iterations; ++itr)
    {
        // Copying A
        for(unsigned int i = 0; i < 8; ++i)
        {
            streamA[i]->read(Aparts[i]);
        }

        // Copying B
        for(unsigned int i = 0; i < 4; ++i)
        {
            streamB[i]->read(Bparts[i]);
        }

        // Running matmatmult
        optimized_matmult(k4, *streamA[0], *streamA[1], *streamA[2], *streamA[3],
                            *streamA[4], *streamA[5], *streamA[6], *streamA[7],
                            *streamB[0], *streamB[1], *streamB[2], *streamB[3],
                            *streamC[0], *streamC[1], *streamC[2], *streamC[3], 
                            *streamC[4], *streamC[5], *streamC[6], *streamC[7]);

        // Copying data back to HOST memory
        for(unsigned int i = 0; i < 8; ++i)
        {
            streamC[i]->write(Cparts[i]);

            // Error checking on the output stream
            if(streamC[i]->error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << streamC[i]->errorLog() << std::endl;
                retVal = -1;
            }
        }
    }

    timer->Stop();

    // Combine the output parts
    _combine(_output, Cparts, 8, _n, _m);

    // delete streams
    for(unsigned int i = 0; i < 8; ++i)
    {
        delete Aparts[i];
        delete Cparts[i];
        delete streamA[i];
        delete streamC[i];
    }


    for(unsigned int i = 0; i < 4; ++i)
    {
        delete Bparts[i];
        delete streamB[i];
    }

    /////////////////////////////////////////////////////////////////////////
    // Operations to combine different parts
    /////////////////////////////////////////////////////////////////////////

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

OptimizedMMM ::~OptimizedMMM()
{
    delete [] _inputA;
    delete [] _inputB;
    delete [] _output;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Performs division of a matrix
//!
//! Given a matrix 'whole', it is divided into 'n' parts. The division is as
//! follows:-
//!     Row 0 goes to part 0, Row 1 goes to part 1, ... Row n-1 goes to part n-1
//!     and then Row n goes to part 0, Row n+1 goes to part 1 and so on.
//!
//! To achieve the above, given a row 'i' of 'whole', we calculate which part
//! the row should be copied to. That is done using the modulo operations i.e.
//!     partIndex = i % n
//! The row in part numbered 'partIndex' where the ith row of 'whole' is copied
//! is calculated using the following formula:-
//!     position = i / n;
//! 
////////////////////////////////////////////////////////////////////////////////


void
OptimizedMMM::_divide(float* whole, float* part[], unsigned int numParts, 
                      unsigned int width, unsigned int height)
{
    // Calculate part dimensions
    unsigned int partWidth  = width;
    unsigned int partHeight = height / numParts;

    // Copy data from whole to part
    for(unsigned int i = 0; i < height; ++i)
    {
        // Find which part the row 'i' corresponds to
        unsigned int partIndex = i % numParts;
        float* partBuffer = part[partIndex];

        // Find memory address of the part where the 'ith' row goes
        unsigned int rowNumber = i / numParts; 
        unsigned int position = rowNumber * partWidth;
        float* dstMem = &partBuffer[position];

        float* srcMem = &whole[i * width];

        // Copy data from whole to part.
        // Row number 'i' in 'width' is copied to
        // Row number 'position' in 'partBuffer'.
        // Number of elements copied are 'width'.
        memcpy(dstMem, srcMem, width * sizeof(float));
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Performs combination of a matrix
//!
//! Given 'n' matrices, they're combined into a single matrix 'whole'. The 
//! division is as follows:-
//!     Row 0 goes to part 0, Row 1 goes to part 1, ... Row n-1 goes to part n-1
//!     and then Row n goes to part 0, Row n+1 goes to part 1 and so on.
//!
//! To achieve the above, given a row 'i' of 'whole', we calculate which part
//! the row should be copied to. That is done using the modulo operations i.e.
//!     partIndex = i % n
//! The row in part numbered 'partIndex' where the ith row of 'whole' is copied
//! is calculated using the following formula:-
//!     position = i / n;
//! 
////////////////////////////////////////////////////////////////////////////////


void
OptimizedMMM::_combine(float* whole, float* part[], unsigned int numParts,
                       unsigned int width, unsigned int height)
{
    // Calculate part dimensions
    unsigned int partWidth  = width;
    unsigned int partHeight = height / numParts;

    // Copy data from whole to part
    for(unsigned int i = 0; i < height; ++i)
    {
        // Find which part the row 'i' corresponds to
        unsigned int partIndex = i % numParts;
        float* partBuffer = part[partIndex];

        // Find memory address of the part where the 'ith' row goes
        unsigned int rowNumber = i / numParts; 
        unsigned int position = rowNumber * partWidth;
        float* srcMem = &partBuffer[position];

        float* dstMem = &whole[i * width];

        // Copy data from whole to part.
        // Row number 'i' in 'width' is copied to
        // Row number 'position' in 'partBuffer'.
        // Number of elements copied are 'width'.
        memcpy(dstMem, srcMem, partWidth * sizeof(float));
    }
}
