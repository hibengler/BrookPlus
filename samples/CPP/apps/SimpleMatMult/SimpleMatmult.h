#ifndef _SIMPLEMATMULT_H_
#define _SIMPLEMATMULT_H_

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
  \file simpleMatmult.h
  \brief Contains the class definition for simpleMatmult class
 ****************************************************************************/

#include "SampleBase.h"
#include "brookgenfiles/simple_matmult.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class  simpleMatmult
//!
//! \brief  contains application properties for the sample simpleMatmult
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
class SimpleMatmult : public SampleBase
{
    public:

        SimpleMatmult(char* name);
        void allocateBuffer();
        void fillInputBuffer();
        int run();
        void printTimeInfo();
        void verifyResults();
        void comparePerformance();
        ~SimpleMatmult();

    private:
        void _matmultCPU(T* a, T* b, T* c, int m, int k, int n);
        void _callBrookKernel(int width, ::brook::Stream<T> A,
                              ::brook::Stream<T> B, ::brook::Stream<T> C);

        T* _inputA;
        T* _inputB;
        T* _output;
};

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
SimpleMatmult<T>::SimpleMatmult(char* name) : SampleBase(name)
{
    _inputA = NULL;
    _inputB = NULL;
    _output = NULL;
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

template <class T>
void 
SimpleMatmult<T>::_matmultCPU(T* a, T* b, T* c, int m, int k, int n)
{
    int y;
    for (y = 0; y < m; y++)
    {
        int x;
        for (x = 0; x < n; x++)
        {
            T temp = (T)0;
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
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
void
SimpleMatmult<T>::allocateBuffer()
{
    int Width = info->Width;
    int Height = info->Height;
    
    _inputA = Util::allocateBuffer<T>(Width, Height);
    _inputB = Util::allocateBuffer<T>(Width, Height);
    _output = Util::allocateBuffer<T>(Height, Height);
    
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

template <class T>
void
SimpleMatmult<T>::fillInputBuffer()
{
    int Width = info->Width;
    int Height = info->Height;

    Util::fillBuffer<T>(_inputA, Width, Height, 1, 1, Util::RANDOM);
    Util::fillBuffer<T>(_inputB, Width, Height, 1, 1, Util::RANDOM);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
void
SimpleMatmult<T>::printTimeInfo()
{
    int Width = info->Width;
    int Height = info->Height;

    double time = timer->GetElapsedTime();
    double gflop = (double)(2.0 * Width * Height * Height * info->Iterations)/((double)1024 * 1024 * 1024 * time);
    
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::setw(16) << "Gflops" << std::endl;

    std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
                  << std::setw(16) << info->Iterations << std::setw(16) << time
                  << std::setw(16) << gflop / time << std::endl;

    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
void
SimpleMatmult<T>::verifyResults()
{
    int Width = info->Width;
    int Height = info->Height;
    int res = 0;
    T *CPU = Util::allocateBuffer<T>(Height, Height);
    
    if (CPU)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Performing Matrix Multiplication on CPU ... ";

        _matmultCPU(_inputA, _inputB, CPU, Height, Width, Height);
        
        std::cout << "Done\n";

        res += Util::compareBuffers<T>(CPU, _output, Height * Height);
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

template <class T>
void
SimpleMatmult<T>::comparePerformance()
{
    int Width = info->Width;
    int Height = info->Height;
    unsigned int i;
    double cpu_time = 0.0;
    double gpu_time = 0.0;
    double gflop = (double)(2.0 * Width * Height * Height * info->Iterations)/(double)(1024 * 1024 * 1024);
    T *CPU = Util::allocateBuffer<T>(Height, Height);
    
    gpu_time = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();
    
    if (CPU)
    {
        std::cout << "-p Compare performance with CPU.\n";
        
        // Record CPU Total time
        
        for(i = 0; i < info->Iterations; i ++)
        {
            _matmultCPU(_inputA, _inputB, CPU, Height, Width, Height);
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

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  invokes the corresponding brook kernel depending on datatype
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
void
SimpleMatmult<T>::_callBrookKernel(int width, ::brook::Stream<T> A, ::brook::Stream<T> B,
                                   ::brook::Stream<T> C)
{
}

template<>
void
SimpleMatmult<float>::_callBrookKernel(int width, ::brook::Stream<float> A, ::brook::Stream<float> B,
                                   ::brook::Stream<float> C)
{
    simple_matmult(width, A, B, C);
}

template<>
void
SimpleMatmult<double>::_callBrookKernel(int width, ::brook::Stream<double> A, ::brook::Stream<double> B,
                                   ::brook::Stream<double> C)
{
    double_precision_simple_matmult(width, A, B, C);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for simple matmult sample
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
int
SimpleMatmult<T>::run()
{
    int Width = info->Width;
    int Height = info->Height;
    int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    {
        unsigned int inputDim1[] = {Width, Height};
        unsigned int inputDim2[] = {Height, Width};
        unsigned int outputDim[] = {Height, Height};

        ::brook::Stream<T> A(2, inputDim1);
        ::brook::Stream<T> B(2, inputDim2);
        ::brook::Stream<T> C(2, outputDim);

        // Record GPU Total time
        timer->Start();
        
        for (unsigned int i = 0; i < info->Iterations; ++i)
        {
            // Write to stream 
            A.read(_inputA);
            B.read(_inputB);

            // Run the brook program 
            _callBrookKernel((int)Width, A, B, C);
            
            // Write data back from stream 
            C.write(_output);

            // Handle errors if any
            if(C.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << C.errorLog() << std::endl;
                retVal = -1;
            }
        }

        timer->Stop();
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

template <class T>
SimpleMatmult<T>::~SimpleMatmult()
{
    if(_inputA != NULL)
    {
        delete [] _inputA;
    }
    
    if(_inputB != NULL)
    {
        delete [] _inputB;
    }

    if(_output != NULL)
    {
        delete [] _output;
    }
}

#endif // _SIMPLEMATMULT_H_
