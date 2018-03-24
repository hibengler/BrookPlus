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

#include "BinomialOptionPricing.h"
#include "brookgenfiles/binomial_option.h"

/**
 * \NUMSTEP 12
 * \brief Macro specifying defaule time steps, this needs to be changed accordingly.
 */
#define NUMSTEP 12

/**
 * \RISKFREE 0.02f
 * \brief risk free interest rate.
 */
#define RISKFREE 0.02f

/**
 * \VOLATILITY 0.30f
 * \brief Volatility factor for Binomial Option Pricing.
 */
#define VOLATILITY 0.30f


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

BinomialOptionPricing::BinomialOptionPricing(char* name)
          :SampleBase(name)
{
    _numSteps = 0;
    _numSamples = 0;

    _stockPrice = NULL;
    _optionStrike = NULL;
    _optionYears = NULL;
    _resultCPU = NULL;
    _resultGPU = NULL;
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief           generates random numbers.
//! \param randMax   maximum element for the given range.
//! \param randMin   minimum element for the given range.
//!
////////////////////////////////////////////////////////////////////////////////

float
BinomialOptionPricing::_random(const float randMax, const float randMin)
{
    float result;
    result =(float)rand()/(float)RAND_MAX;

    return ((1.0f - result) * randMin + result *randMax);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief           returns the maximum number between two given numbers.
//! \param param1    the first number.
//! \param param2    the second number.
//!
////////////////////////////////////////////////////////////////////////////////

float
BinomialOptionPricing::_max(const float param1, const float param2)
{
    if(param1 >= param2)
    {

        return param1;
    }
    else
    {

        return param2;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
BinomialOptionPricing::allocateBuffer()
{
    _numSamples = info->Width;
    _numSteps = info->Height;

    if(info->Height == DEFAULT_HEIGHT)
    {
        _numSteps = NUMSTEP;
        info->Height = _numSteps;
        std::cout << "Height is take as number of time steps for binomial tree." << std::endl
                  <<  "It should be 4, 8 or 12. It is adjusted to 12" << std::endl;
    }
    
    std::cout << std::endl;

    // The width is adjusted accordingly to support implementation.
    unsigned int temp = (unsigned int)sqrt((float)info->Width);
    temp = temp * temp * 4;
    _numSamples = temp;
    info->Width = _numSamples;

    std::cout << "Adjusting sample size to avoid address translation\n";
    std::cout << "Adjusted Sample Size = " << _numSamples << std::endl;

    _stockPrice = Util::allocateBuffer<float>(_numSamples, 1);
    _optionStrike = Util::allocateBuffer<float>(_numSamples, 1);
    _optionYears = Util::allocateBuffer<float>(_numSamples, 1);
    _resultCPU = Util::allocateBuffer<float>(_numSamples, 1);
    _resultGPU = Util::allocateBuffer<float>(_numSamples, 1);

    if(!_stockPrice || !_optionStrike || !_optionYears || !_resultCPU || !_resultGPU)
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
BinomialOptionPricing::fillInputBuffer()
{
    for(unsigned int i = 0; i < _numSamples; i++)
    {
        _stockPrice[i] = _random(30.0f, 5.0f);
        _optionStrike[i] = _random(100.0f, 1.0f);
        _optionYears[i] = _random(10.0f, 0.25f);
        _resultCPU[i] = -1.0f;
        _resultGPU[i] = -1.0f;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Reference CPU program to perform binomial option pricing
//! 
////////////////////////////////////////////////////////////////////////////////

void 
BinomialOptionPricing::_binomialOptionCPU()
{
    static float stepsArray[NUMSTEP + 1];
    unsigned int i = 0;
    
    //Iterate for all samples...
    for(i = 0; i < _numSamples; i++)
    {
        const float s = _stockPrice[i];
        const float x = _optionStrike[i];
        const float t = _optionYears[i];

        const float dt = t * (1.0f / _numSteps);
        const float vsdt = VOLATILITY * sqrtf(dt);
        const float rdt = RISKFREE * dt;

        const float r = expf(rdt);
        const float rInv = 1.0f / r;

        const float u = expf(vsdt);
        const float d = 1.0f / u;
        const float pu = (r - d)/(u - d);
        const float pd = 1.0f - pu;
        const float puByr = pu * rInv;
        const float pdByr = pd * rInv;

        // Compute values at expiration date:
        // Call option value at period end is v(t) = s(t) - x
        // If s(t) is greater than x, or zero otherwise...
        // The computation is similar for put options...
        for(unsigned int j = 0; j <= _numSteps; j++)
        {
            float price = s * expf(vsdt * (2.0f * j - _numSteps));
            stepsArray[j] = _max(price - x, 0.0f);
        }
        
        //walk backwards up on the binomial tree of depth _numSteps...
        //Reduce the price step by step...
        for(unsigned int j = _numSteps; j > 0; j--)
        {
            for(unsigned int k = 0; k <= j - 1; k++)
            {
                stepsArray[k] = puByr * stepsArray[k+1] + pdByr * stepsArray[k];
            }
        }
        
        //Copy the root to result...
        _resultCPU[i] = stepsArray[0];
    }
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
BinomialOptionPricing::printTimeInfo()
{
    double time = timer->GetElapsedTime();
    
    std::cout << std::setw(16) << "Number Of Samples" << std::setw(16) << "TimeSteps"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::endl;

    std::cout << std::setw(16) << _numSamples << std::setw(16) << _numSteps
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
BinomialOptionPricing::verifyResults()
{
    int res = 0;
    
    std::cout<<"-e Verify correct output.\n";
    std::cout<<"Performing Prefix Sum on CPU ... ";

    _binomialOptionCPU();

    std::cout << "Done\n";

    res += Util::compareBuffers<float>(_resultCPU, _resultGPU, _numSamples);
    
    if (res)
    {
        std::cout << _name << ": Failed!\n\n";
    }
    else
    {
        std::cout << _name << ": Passed!\n\n" ;
    }

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compare performance with CPU if requested
//! 
////////////////////////////////////////////////////////////////////////////////

void
BinomialOptionPricing::comparePerformance()
{
    unsigned int i;
    double cpuTime = 0.0;
    double gpuTime = 0.0;
    
    gpuTime = timer->GetElapsedTime();
    
    timer->Reset();
    timer->Start();
    
    std::cout << "-p Compare performance with CPU.\n";
    
    // Record CPU Total time
    for(i = 0; i < info->Iterations; i ++)
    {
        _binomialOptionCPU();
    }
    
    timer->Stop();
    cpuTime = timer->GetElapsedTime();

    // Print CPU timing and speedup
    std::cout << std::setw(16) << "Number of Samples" << std::setw(16) << "TimeStep"
              << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
              << std::setw(16) << "GPU Total Time"
              << std::setw(16) << "Speedup" << std::endl;

    std::cout << std::setw(16) << _numSamples << std::setw(16) << _numSteps
              << std::setw(16) << info->Iterations << std::setw(16) << cpuTime
              << std::setw(16) << gpuTime
              << std::setw(16) << cpuTime / gpuTime << std::endl;
    
    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
BinomialOptionPricing::run()
{
    int retVal = 0;
    unsigned int temp1 = _numSamples / 4;
    unsigned int temp2 = (unsigned int)sqrt((float)temp1);
    unsigned int dim = temp2;
    unsigned int streamDim[] = {dim, dim};
    
    timer->Start();

    switch(_numSteps)
    {
        case 12:
        {
            ::brook::Stream<float4> s4(2, streamDim);
            ::brook::Stream<float4> x4(2, streamDim);
            ::brook::Stream<float4> t4(2, streamDim);
            ::brook::Stream<float4> result(2, streamDim);
            ::brook::Stream<float4>* A[6];
            
            for(unsigned int i = 0; i < 6; i++)
            {
                A[i] = new ::brook::Stream<float4>(2, streamDim);
            }
            
            s4.read(_stockPrice);
            x4.read(_optionStrike);
            t4.read(_optionYears);
            
            for( unsigned int k = 0; k < info->Iterations; k++)
            {
                mainGPU12((float)_numSteps, RISKFREE, VOLATILITY, s4, x4, t4,
                          result, *A[0], *A[1], *A[2], *A[3], *A[4], *A[5]);
            }

            result.write(_resultGPU);

            if(result.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << result.errorLog() << std::endl;
                retVal = -1;
            }

            for(unsigned int i = 0; i < 6; i++)
            {
                delete A[i];
            }
            
            break;
        }
        case 8:
        {
            ::brook::Stream<float4> s4(2, streamDim);
            ::brook::Stream<float4> x4(2, streamDim);
            ::brook::Stream<float4> t4(2, streamDim);
            ::brook::Stream<float4> result(2, streamDim);
            ::brook::Stream<float4>* A[6];
            
            for(unsigned int i = 0; i < 6; i++)
            {
                A[i] = new ::brook::Stream<float4>(2, streamDim);
            }
            
            s4.read(_stockPrice);
            x4.read(_optionStrike);
            t4.read(_optionYears);
            
            for(unsigned int k = 0; k < info->Iterations; k++)
            {
                mainGPU8((float)_numSteps, RISKFREE, VOLATILITY, s4, x4, t4,
                         result, *A[0], *A[1], *A[2], *A[3], *A[4], *A[5]);
            }

            result.write(_resultGPU);

            if(result.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << result.errorLog() << std::endl;
                retVal = -1;
            }

            for(unsigned int i = 0; i < 6; i++)
            {
                delete A[i];
            }
            
            break;
        }
        case 4:
        {
            ::brook::Stream<float4> s4(2, streamDim);
            ::brook::Stream<float4> x4(2, streamDim);
            ::brook::Stream<float4> t4(2, streamDim);
            ::brook::Stream<float4> result(2, streamDim);
            ::brook::Stream<float4>* A[4];
            
            for(unsigned int i = 0; i < 4; i++)
            {
                A[i] = new ::brook::Stream<float4>(2, streamDim);
            }
            
            s4.read(_stockPrice);
            x4.read(_optionStrike);
            t4.read(_optionYears);
            
            for(unsigned int k = 0; k < info->Iterations; k++)
            {
                mainGPU4((float)_numSteps, RISKFREE, VOLATILITY, s4, x4, t4,
                        result, *A[0], *A[1], *A[2], *A[3]);
            }

            result.write(_resultGPU);

            if(result.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << result.errorLog() << std::endl;
                retVal = -1;
            }

            for(unsigned int i = 0; i < 4; i++)
            {
                delete A[i];
            }
            
            break;
        }
        default:
        {
            std::cout<<"\n\nWhoops! Time steps only can on be 4, 8 or 12...\n";
        }
    }

    timer->Stop();

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

BinomialOptionPricing ::~BinomialOptionPricing()
{
    delete [] _stockPrice;
    delete [] _optionStrike;
    delete [] _optionYears;
    delete [] _resultGPU;
    delete [] _resultCPU;
}
