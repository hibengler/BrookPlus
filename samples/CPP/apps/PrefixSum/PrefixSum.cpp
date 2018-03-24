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

#include "PrefixSum.h"
#include "brookgenfiles/prefix_sum.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

PrefixSum::PrefixSum(char* name) : SampleBase(name)
{
    _input = NULL;
    _output = NULL;

    _width = 0;
    _height = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
PrefixSum::allocateBuffer()
{
    _width = info->Width;
    _height = info->Height;

    _input = Util::allocateBuffer<float>(_width, _height);
    _output = Util::allocateBuffer<float>(_width, _height);
        
    if (!_input || !_output)
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
PrefixSum::fillInputBuffer()
{
    Util::fillBuffer(_input, _width, _height, 1, 1, Util::RANDOM);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Reference CPU program to perform prefix scan
//!
//! \param input Input array
//! \param output    Output containing prefix sums
//! \param len        Length of the input array
//! 
////////////////////////////////////////////////////////////////////////////////

void 
PrefixSum::prefix_sum_cpu(float* input, float* output, int len)
{
    float temp = 0.0f;
    int y;
    for (y = 0; y < len; ++y)
    {
        temp += input[y];
        output[y] = temp;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
PrefixSum::printTimeInfo()
{
    double time = timer->GetElapsedTime();
    
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::endl;

    std::cout << std::setw(8) << _width << std::setw(8) << _height
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
PrefixSum::verifyResults()
{
    int res = 0;
    float* CPU = Util::allocateBuffer<float>(_width, _height);

    if (CPU)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Performing Prefix Sum on CPU ... ";

        prefix_sum_cpu((float*)_input, (float*)CPU, _height * _width);
        
        std::cout << "Done\n";

        res += Util::compareBuffers<float>(CPU, _output, _height * _width);
        
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
PrefixSum::comparePerformance()
{
    unsigned int i;
    double cpuTime = 0.0;
    double gpuTime = 0.0;
    float* CPU = Util::allocateBuffer<float>(_width, _height);

    gpuTime = timer->GetElapsedTime();
    
    if(CPU)
    {
        timer->Reset();
        timer->Start();
        
        std::cout << "-p Compare performance with CPU.\n";
        
        // Record CPU Total time
        for(i = 0; i < info->Iterations; i ++)
        {
            prefix_sum_cpu((float*)_input, (float*)CPU, _height * _width);
        }
        
        timer->Stop();
        delete [] CPU;
    }
    else
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }

    cpuTime = timer->GetElapsedTime();

    // Print CPU timing and speedup
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
              << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
              << std::setw(16) << "GPU Total Time"
              << std::setw(16) << "Speedup" << std::endl;

    std::cout << std::setw(8) << _width << std::setw(8) << _height
              << std::setw(16) << info->Iterations << std::setw(16) << cpuTime
              << std::setw(16) << gpuTime
              << std::setw(16) << cpuTime / gpuTime << std::endl;
    
    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for simple matmult sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
PrefixSum::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    {
        unsigned int dim[] = {_width, _height};
        float2 maxvalue   = float2(0.0f, 0.0f);
        int sign = -1;
        unsigned int logN = 0, j = 0;

        ::brook::Stream<float> si2(2, dim);
        ::brook::Stream<float> istream(2, dim);
        ::brook::Stream<float> so2(2, dim);

        // Record GPU Total time
        timer->Start();
        for (unsigned int i = 0; i < info->Iterations; ++i)
        {
            // Write to stream 
            istream.read(_input);

            j = 0;
            logN = (unsigned int)ceil(log((float)_height * _width)/log(2.0f));
            maxvalue.x = (float)_width;
            maxvalue.y = (float)_height;
            
            // Run the brook program 
            scan(istream, so2, (float)sign, maxvalue);

            for (j = 1; j < logN; ++j)
            {
                if (j & 0x1)
                {
                    scan(so2, si2, (float)sign * (1 << j), maxvalue);
                }
                else
                {
                    scan(si2, so2, (float)sign * (1 << j), maxvalue);
                }
            }
            if (!(logN % 2) && (_height * _width !=1))
            {
                // Swap si2 and so2
                ::brook::Stream<float> temp = si2;
                si2 = so2;
                so2 = temp;
            }

            // Write data back from stream 
            so2.write(_output);

            //Handle errors if any
            if(so2.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << so2.errorLog() << std::endl;
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

PrefixSum ::~PrefixSum()
{
    delete [] _input;
    delete [] _output;
}
