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

#include "MersenneTwister.h"
#include "brookgenfiles/mersenne_twister.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

MersenneTwister::MersenneTwister(char* name) : SampleBase(name)
{
    _seedArray = NULL;
    _gpuResult = NULL;
    _width = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
MersenneTwister::allocateBuffer()
{
    _width = info->Width;
    unsigned int dim1 = 0;
    unsigned int dim2 = 0;

    // Kernel operates on vectorized data. So the Width should be multiple of 4.
    if(_width < 4)
    {
        // Set the Width 4, the minimum width.
        _width = 4;
        dim1 = 1;
        dim2 = 1;
    }
    
    // Calculate approximate dim1, dim2 such that dim1 * dim2 * 4 comes around Width.
    else
    {
        unsigned int tempVar1 = (unsigned int)sqrt((float)_width);
        unsigned int tempVar2 = tempVar1 / 4;
        dim1 = tempVar2 * 4;
        dim2 = tempVar2;
    }

    // Reset the Width.
    _width = dim1 * dim2 * 4;
     
    // Print the modified Width.
    if(_width != info->Width)
    {
        std::cout << "The modified width is : " << _width << std::endl;
    }

    // Warn user if she/he has entered the "-y" option that it is ignored.
    // However, this will fail to warn in case the value given equals the default
    // This is because here, the input size is just "number of options" - and thus
    // a one dimensional problem. So, we just need to take either "Width" or "Height"
    // and we chose "Width"
    if (info->Height != DEFAULT_HEIGHT) 
    {
        fprintf(stderr, "Warning: \n\t'Height' is not taken from the commandline.\n");
        fprintf(stderr, "\tIt is not used in this Brook sample.\n");
        fprintf(stderr, "\tPlease use 'Width' if you wish to change the number of seeds.\n\n");
    }
    
    // Save the modified width for later use
    info->Width = _width;
        
    _seedArray = Util::allocateBuffer<unsigned int>(_width, 1);
    _gpuResult = Util::allocateBuffer<float>(_width * 8, 1);
         
    if (!_seedArray || !_gpuResult)
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
MersenneTwister::fillInputBuffer()
{
    for(unsigned int i = 0; i < _width; ++i)
    {
        _seedArray[i] = (unsigned int)rand();
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Initialize the states for CPU implementation
//!         Here one seed generates 624 random numbers.
//!
////////////////////////////////////////////////////////////////////////////////

void 
MersenneTwister::initializeGenerator_cpu(unsigned int seed, unsigned int *M, unsigned int N)
{
    unsigned int i = 0;
    M[0] = seed;
    for(i = 1; i < N; i++)
    {
        int tempVar = M[i-1] ^ (M[i-1] >> 30);
        M[i] = (1812433253 * tempVar + i);
        M[i] &= 0xffffffff;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Generate random numbers for CPU implementation
//!        The state array is populated with the random numbers.
//!
////////////////////////////////////////////////////////////////////////////////

void
MersenneTwister::generateNumber_cpu(unsigned int *M, unsigned int N)
{
    unsigned int i = 0;
    unsigned int y;
    for(; i < N; i++)
    {
        y = (M[i] >> 31) + ((M[(i + 1) % N] >> 1) >> 31);
        if(y % 2 == 0)
        {
            M[i] = M[(i + 397)% N] ^ (y >> 1);
        }
        else
        {
            M[i] = M[(i + 397)% N] ^ (y >> 1) ^ (2567483615u);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  After the random number generation, extract the random numbers
//! 
////////////////////////////////////////////////////////////////////////////////

void
MersenneTwister::extractNumber_cpu(unsigned int *M, unsigned int N)
{
    unsigned int y;
    unsigned int i;

    for(i = 0; i < N; i++)
    {
        y = M[i];
        y = y ^ (y >> 11);
        y = y ^ ((y << 7) & (2636928640u));
        y = y ^ ((y << 15) & (4022730752u));
        M[i] = y ^ (y >> 18);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Box Mullar transformation is applied 
//!        on the generated random numbers
//! 
////////////////////////////////////////////////////////////////////////////////

void
MersenneTwister::boxMullar_cpu(unsigned int *M, float *BM, unsigned int N)
{
    unsigned int i;
    float PI = 3.14159265358979f;

    for(i = 0; i < N; )
    {
        float tempFloat1 = M[i] * 1.0f / 4294967296.0f;
        float tempFloat2 = M[i + 1] * 1.0f / 4294967296.0f;
    
        float r = sqrt((-2.0f) * log(tempFloat1));
        float phi  = 2.0f * PI * tempFloat2;

        BM[i] = r * cos(phi);
        BM[i + 1] = r * sin(phi);
        i += 2;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
MersenneTwister::printTimeInfo()
{
    double time = timer->GetElapsedTime();
    double gpuPerformance = ((double)_width * 8)/ time;

    std::cout << std::setw(16) << "Nums Generated" << std::setw(16) << "Iterations"
              << std::setw(16) << "GPU Total Time" << std::setw(16) << "Num/Sec" 
              << std::endl;

    std::cout << std::setw(16) <<(_width * 8) << std::setw(16) << info->Iterations
                << std::setw(16) << time << std::setw(16) << gpuPerformance
                << std::endl;

    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
MersenneTwister::verifyResults()
{
    std::cout<<"Verification is not applicable for this application!\n";
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compare performance with CPU if requested
//! 
////////////////////////////////////////////////////////////////////////////////

void
MersenneTwister::comparePerformance()
{
    unsigned int i = 0;
    unsigned int j = 0;
    double cpuTime = 0.0;
    double gpuTime = 0.0;
    double cpuPerformance = 0;
    double gpuPerformance = 0;

    float* CPU = Util::allocateBuffer<float>(624 * DEFAULT_WIDTH, 1);
             
    gpuTime = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();
    
    std::cout << "-p Compare performance with CPU.\n";
    
    if(CPU)
    {
        for(i = 0; i < info->Iterations; i++)
        {
            static unsigned int M[624];
            static float BM[624];

            for(j = 0; j < DEFAULT_WIDTH; j++)
            {
                initializeGenerator_cpu(_seedArray[j], M, 624);
                generateNumber_cpu(M, 624);
                extractNumber_cpu(M, 624);
                boxMullar_cpu(M, BM, 624);
                memcpy(&CPU[j * 624], &BM, 624 * sizeof(float));
            }
        }
        
        timer->Stop();
        delete [] CPU;
    }
    else
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
    
    // Print CPU timing and speedup
    cpuTime = timer->GetElapsedTime();
    gpuPerformance = ((double)_width * 8)/ gpuTime;
    cpuPerformance = ((double)DEFAULT_WIDTH * 624)/ cpuTime;
    
    std::cout << std::setw(16) << "Iterations" << std::setw(16) << "CPU Num/Sec"
              << std::setw(16) << "GPU Num/Sec"
              << std::setw(16) << "Speedup" << std::endl;

    std::cout << std::setw(16) << info->Iterations << std::setw(16) << cpuPerformance
              << std::setw(16) << gpuPerformance
              << std::setw(16) << gpuPerformance / cpuPerformance << std::endl;
    
    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
MersenneTwister::run()
{
    unsigned int retVal = 0;
    unsigned int Width = info->Width;
    unsigned int dim1 = 0;
    unsigned int dim2 = 0;
    unsigned int tempVar1 = (unsigned int)sqrt((float)Width);
    unsigned int tempVar2 = tempVar1 / 4;
    dim1 = tempVar2 * 4;
    dim2 = tempVar2;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    {
        bool noError = true;

        // Declare all input parameters as the 2-D streams.
        unsigned int streamDim[] = {dim2, dim1};

        ::brook::Stream<uint4> seed(2, streamDim);
                
        ::brook::Stream< uint4> rand1(2, streamDim);
        ::brook::Stream<uint4> rand2(2, streamDim);
        ::brook::Stream<uint4> rand3(2, streamDim);
        ::brook::Stream<uint4> rand4(2, streamDim);
        ::brook::Stream<uint4> rand5(2, streamDim);
        ::brook::Stream<uint4> rand6(2, streamDim);
        ::brook::Stream<uint4> rand7(2, streamDim);
        ::brook::Stream<uint4> rand8(2, streamDim);
        
        ::brook::Stream<float4> randOut1(2, streamDim);
        ::brook::Stream<float4> randOut2(2, streamDim);
        ::brook::Stream<float4> randOut3(2, streamDim);
        ::brook::Stream<float4> randOut4(2, streamDim);
        ::brook::Stream<float4> randOut5(2, streamDim);
        ::brook::Stream<float4> randOut6(2, streamDim);
        ::brook::Stream<float4> randOut7(2, streamDim);
        ::brook::Stream<float4> randOut8(2, streamDim);
        
        // Record GPU Total time 
        timer->Start();
        for(unsigned int i = 0; i < info->Iterations; ++i)
        {
            // Read seed into seed stream.
            seed.read(_seedArray);

            // Mersenne Twister RNG kernel.
            generateNumber_gpu(seed, rand1, rand2, rand3, rand4, rand5, rand6, rand7, rand8);

            // Invoke Box Mullar Transformation kernel on generated MT random numbers.
            boxMullar_gpu(rand1, rand2, rand3, rand4, rand5, rand6, rand7, rand8,
                randOut1, randOut2, randOut3, randOut4, randOut5, randOut6, randOut7, randOut8);
            
            // Write data back from stream here
            randOut1.write(&_gpuResult[0]);
            randOut2.write(&_gpuResult[dim1 * dim2 * 4 * 1]);
            randOut3.write(&_gpuResult[dim1 * dim2 * 4 * 2]);
            randOut4.write(&_gpuResult[dim1 * dim2 * 4 * 3]);
            randOut5.write(&_gpuResult[dim1 * dim2 * 4 * 4]);
            randOut6.write(&_gpuResult[dim1 * dim2 * 4 * 5]);
            randOut7.write(&_gpuResult[dim1 * dim2 * 4 * 6]);
            randOut8.write(&_gpuResult[dim1 * dim2 * 4 * 7]);

            //Handle errors if occured
            noError &= Util::checkBrookError(randOut1);
            noError &= Util::checkBrookError(randOut2);
            noError &= Util::checkBrookError(randOut3);
            noError &= Util::checkBrookError(randOut4);
            noError &= Util::checkBrookError(randOut5);
            noError &= Util::checkBrookError(randOut6);
            noError &= Util::checkBrookError(randOut7);
            noError &= Util::checkBrookError(randOut8);
            
            if(!noError)
            {
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

MersenneTwister::~MersenneTwister()
{
    delete [] _seedArray;
    delete [] _gpuResult;
}
