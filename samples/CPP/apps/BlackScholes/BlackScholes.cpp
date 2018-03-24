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

#include "BlackScholes.h"
#include "brookgenfiles/black_scholes.h"

#define S_LOWER_LIMIT 10.0f
#define S_UPPER_LIMIT 100.0f
#define K_LOWER_LIMIT 10.0f
#define K_UPPER_LIMIT 100.0f
#define T_LOWER_LIMIT 1.0f
#define T_UPPER_LIMIT 10.0f
#define r_LOWER_LIMIT 0.01f
#define r_UPPER_LIMIT 0.05f
#define sigma_LOWER_LIMIT 0.01f
#define sigma_UPPER_LIMIT 0.10f

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

BlackScholes::BlackScholes(char* name) : SampleBase(name)
{
    unsigned int i = 0;
    
    for(i = 0; i < 5; i++)
    {
        _inputArray[i] = NULL;
    }
    _callArray = NULL;
    _putArray = NULL;
    _width = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Abromowitz Stegun approxmimation for PHI on the CPU
//!        (Cumulative Normal Distribution Function)
//!
////////////////////////////////////////////////////////////////////////////////

float
BlackScholes::_PHI_cpu(float X)
{
    float y, absX, t;

    // the coeffs
    const float c1 =  0.319381530f;
    const float c2 = -0.356563782f;
    const float c3 =  1.781477937f;
    const float c4 = -1.821255978f;
    const float c5 =  1.330274429f;

    const float one_by_sqrt_2_pi = 0.398942280f;

    absX = fabs(X);
    t = 1.0f/(1.0f + 0.2316419f * absX);

    y = 1.0f - one_by_sqrt_2_pi * exp(-X*X/2.0f) *
        t * (c1 +
                t * (c2 +
                    t * (c3 +
                        t * (c4 + t * c5))));

    return (X < 0) ? (1.0f - y) : y;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief CPU version of black scholes
//!
////////////////////////////////////////////////////////////////////////////////

void
BlackScholes::_black_scholes_cpu(float* S, float* K, float* T, float* r, float* sigma, float* call,
                                 float* put, unsigned int Width)
{
    unsigned int y;
    for (y = 0; y < Width; ++y)
    {
        float d1, d2;
        float sigma_sqrt_T;
        float K_exp_minus_r_T;

        sigma_sqrt_T = sigma[y] * sqrt(T[y]);

        d1 = (log(S[y]/K[y]) + (r[y] + sigma[y]*sigma[y]/2.0f)*T[y]) / sigma_sqrt_T;
        d2 = d1 - sigma_sqrt_T;

        K_exp_minus_r_T = K[y] * exp(-r[y] * T[y]);
        call[y] = S[y] * _PHI_cpu(d1) - K_exp_minus_r_T * _PHI_cpu(d2);
        put[y]  = K_exp_minus_r_T * _PHI_cpu(-d2) - S[y] * _PHI_cpu(-d1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Randomization function to create initial data set
//!
////////////////////////////////////////////////////////////////////////////////

float
BlackScholes::_frand(float lowerLimit, float upperLimit)
{
    unsigned int intRand;
    float t;

    assert(upperLimit > lowerLimit);

    intRand = rand();
    t = (float)intRand/(float)RAND_MAX;

    return lowerLimit * t + upperLimit * (1.0f - t);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
BlackScholes::allocateBuffer()
{
    unsigned int i = 0;
    unsigned int dim1;
    unsigned int dim2;

    _width = info->Width;

    // The implementation modification involves the use of 2-D stream to avoid address
    // translation and float4 data type. The minimum width is taken 4, for implementation
    // specific reasons.
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
        unsigned int tempVar1 = (unsigned int)sqrt((double)_width);
        unsigned int tempVar2 = tempVar1 / 4;
        dim1 = tempVar2 * 4;
        dim2 = tempVar2;
    }

    // Reset the Width.
    _width = dim1 * dim2 * 4;
    
    // Print the Width if modified.
    if(_width != info->Width)
    {
        std::cout << "\nThe modified width is : " << _width << std::endl;
    }
    
    info->Width = _width;

    // Warn user if s/he has entered the "-y" option that it is ignored.
    // However, this will fail to warn in case the value given equals the default
    // This is because here, the input size is just "number of options" - and thus
    // a one dimensional problem. So, we just need to take either "Width" or "Height"
    // and we chose "Width"
    if (info->Height != DEFAULT_HEIGHT)
    {
        fprintf(stderr, "Warning: \n\t'Height' is not taken from the commandline.\n");
        fprintf(stderr, "\tIt is not used in this Brook sample.\n");
        fprintf(stderr, "\tPlease use 'Width' if you wish to change the number of options.\n\n");
    }
    
    for(i = 0; i < 5; i++)
    {
        _inputArray[i] = Util::allocateBuffer<float>(_width, 1);
        if (!_inputArray[i])
        {
            fprintf(stderr, "Error: Memory Allocation.\n");
            exit(-1);
        }
    }

    _callArray = Util::allocateBuffer<float>(_width, 1);
    _putArray = Util::allocateBuffer<float>(_width, 1);
         
    if (!_callArray || !_putArray)
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
BlackScholes::fillInputBuffer()
{
    int i = 0;

    for (i = 0; i < _width; ++i)
    {
        // S
        _inputArray[0][i] = _frand(S_LOWER_LIMIT, S_UPPER_LIMIT);
        // K
        _inputArray[1][i] = _frand(K_LOWER_LIMIT, K_UPPER_LIMIT);
        // T
        _inputArray[2][i] = _frand(T_LOWER_LIMIT, T_UPPER_LIMIT);
        // R
        _inputArray[3][i] = _frand(r_LOWER_LIMIT, r_UPPER_LIMIT);
        // Sigma
        _inputArray[4][i] = _frand(sigma_LOWER_LIMIT, sigma_UPPER_LIMIT);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
BlackScholes::printTimeInfo()
{
    double time = timer->GetElapsedTime();

    std::cout << std::setw(8) << "Width" << std::setw(8) 
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::endl;

    std::cout << std::setw(8) << _width << std::setw(8)
                  << std::setw(16) << info->Iterations << std::setw(16) << time
                  << std::endl;

    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
BlackScholes::verifyResults()
{
    int res = 0;

    float* call = Util::allocateBuffer<float>(_width, 1);
    float* put = Util::allocateBuffer<float>(_width, 1);

    if (call && put)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Performing Black Scholes Pricing on CPU ... ";

        _black_scholes_cpu(_inputArray[0], _inputArray[1], _inputArray[2],
                          _inputArray[3], _inputArray[4], call, put, _width);

        std::cout << "Done\n";

        res += Util::compareBuffers<float>(call, _callArray, _width);
        res += Util::compareBuffers<float>(put, _putArray, _width);

        if (res)
        {
            std::cout << _name << ": Failed!\n\n";
        }
        else
        {
            std::cout << _name << ": Passed!\n\n" ;
        }

        delete [] call;
        delete [] put;
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
BlackScholes::comparePerformance()
{
    unsigned int i;
    double cpu_time = 0.0;
    double gpu_time = 0.0;
    
    float* call = Util::allocateBuffer<float>(_width, 1);
    float* put = Util::allocateBuffer<float>(_width, 1);
    
    gpu_time = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();
    
    if (call && put)
    {
        std::cout << "-p Compare performance with CPU.\n";
        
        // Record CPU Total time
        for(i = 0; i < info->Iterations; i ++)
        {
            _black_scholes_cpu(_inputArray[0], _inputArray[1], _inputArray[2],
                          _inputArray[3], _inputArray[4], call, put, _width);
        }
        
        timer->Stop();

        // Print CPU timing and speedup
        cpu_time = timer->GetElapsedTime();
        
        std::cout << std::setw(8) << "Width"
                  << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
                  << std::setw(16) << "GPU Total Time"
                  << std::setw(16) << "Speedup" << std::endl;

        std::cout << std::setw(8) << _width
                  << std::setw(16) << info->Iterations << std::setw(16) << cpu_time
                  << std::setw(16) << gpu_time
                  << std::setw(16) << cpu_time / gpu_time << std::endl;

        std::cout << std::endl << std::endl;
        
        delete [] call;
        delete [] put;
    }
    else
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
BlackScholes::run()
{
    int dim1 = 0;
    int dim2 = 0;
    int retVal = 0;

    dim1 = (unsigned int)sqrt((double)_width);
    dim1 = dim1 / 4;
    dim2 = dim1;
    dim1 = dim1 * 4;
    
    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    {
        // Declare all input parameters as the 2-D streams.
        unsigned int dim[] = { dim2, dim1 };
        
        ::brook::Stream<float4> S_stream(2, dim);
        ::brook::Stream<float4> K_stream(2, dim);
        ::brook::Stream<float4> T_stream(2, dim);
        ::brook::Stream<float4> r_stream(2, dim);
        ::brook::Stream<float4> sigma_stream(2, dim);

        ::brook::Stream<float4> call_stream(2, dim);
        ::brook::Stream<float4> put_stream(2, dim);
        
        // Record GPU Total Time
        timer->Start();

        for(unsigned int i = 0; i < info->Iterations; ++i)
        {
            // Read all streams.
            S_stream.read(_inputArray[0]);
            K_stream.read(_inputArray[1]);
            T_stream.read(_inputArray[2]);
            r_stream.read(_inputArray[3]);
            sigma_stream.read(_inputArray[4]);

            // Run the brook kernel for Black Scholes.
            black_scholes(  S_stream, sigma_stream, K_stream, T_stream, r_stream, call_stream,
                                            put_stream);

            // Write data back from stream
            call_stream.write(_callArray);
            put_stream.write(_putArray);

            // Handle errors if any
            if(call_stream.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << call_stream.errorLog() << std::endl;
                retVal = -1;
            }

            if(put_stream.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << put_stream.errorLog() << std::endl;
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

BlackScholes::~BlackScholes()
{
    for(unsigned int i = 0; i < 5; i++)
    {
        delete [] _inputArray[i];
    }

    delete [] _callArray;
    delete [] _putArray;
}
