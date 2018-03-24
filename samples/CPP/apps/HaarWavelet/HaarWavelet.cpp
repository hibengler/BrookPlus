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

#include "HaarWavelet.h"
#include "brookgenfiles/haar_wavelet.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

HaarWavelet::HaarWavelet(char* name) : SampleBase(name)
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
HaarWavelet::allocateBuffer()
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
HaarWavelet::fillInputBuffer()
{
    Util::fillBuffer(_input, _width, _height, 1, 1, Util::RANDOM);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
HaarWavelet::printTimeInfo()
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
//! \brief  1D Haar Wavelet Transform implementation on CPU
//!         from http://www.cs.ucf.edu/~mali/haar/ 
//! 
////////////////////////////////////////////////////////////////////////////////

void
HaarWavelet::haarWavelet_cpu(float* input, int n, float sqrt2)
{
    int i = 0;
    int w = n;
    float *temp = new float[n];
    
    for (i = 0; i < n; ++i)
    {
        temp[i] = 0.0f;
    }

    while (w > 1)
    {
        w >>= 1;
        for (i = 0; i < w; ++i)
        {
            temp[i] = (input[i << 1] + input[(i << 1) + 1]) / sqrt2;
            temp[i + w] = (input[i << 1] - input[(i << 1) + 1]) / sqrt2;
        }
        for (i = 0; i < (w * 2); ++i)
        {
            input[i] = temp[i];
        }
    }

    delete [] temp;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
HaarWavelet::verifyResults()
{
    int res = 0;
    
    float* cpu = Util::allocateBuffer<float>(_width, _height);
    
    if (cpu)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Computing Haar Wavelet Transform on CPU ... ";

        memcpy(cpu, _input, sizeof(float)* _width * _height);
        haarWavelet_cpu(cpu, _width * _height, sqrt(2.0f));

        std::cout << "Done\n";

        res += Util::compareBuffers<float>(cpu, _output, _width * _height);
        
        if (res)
        {
            std::cout << _name << ": Failed!\n\n";
        }
        else
        {
            std::cout << _name << ": Passed!\n\n" ;
        }

        delete [] cpu;
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
HaarWavelet::comparePerformance()
{
    unsigned int i;
    double cpu_time = 0.0;
    double gpu_time = 0.0;
    
    float* cpu = Util::allocateBuffer<float>(_width, _height);
    
    gpu_time = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();
    
    if (cpu)
    {
        std::cout << "-p Compare performance with CPU.\n";
        
        // Record CPU Total time
        for(i = 0; i < info->Iterations; i ++)
        {
            memcpy(cpu, _input, sizeof(float)* _width * _height);
            haarWavelet_cpu(cpu, _width * _height, sqrt(2.0f));
        }
        
        timer->Stop();

        // Print CPU timing and speedup
        cpu_time = timer->GetElapsedTime();
        
        std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
                  << std::setw(16) << "GPU Total Time"
                  << std::setw(16) << "Speedup" << std::endl;

        std::cout << std::setw(8) << _width << std::setw(8) << _height
                  << std::setw(16) << info->Iterations << std::setw(16) << cpu_time
                  << std::setw(16) << gpu_time
                  << std::setw(16) << cpu_time / gpu_time << std::endl;
        
        std::cout << std::endl << std::endl;
        
        delete [] cpu;
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
HaarWavelet::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    { 
        unsigned int dim[] = {_width * _height};
        unsigned int i = 0;
        ::brook::Stream<float> stream0(1, dim);
        ::brook::Stream<float> stream1(1, dim);
        ::brook::Stream<float> avgStream(1, dim);
        ::brook::Stream<float> diffStream(1, dim);
        
        timer->Start();
        
        for (i = 0; i < info->Iterations; ++i)
        {
            int inp = 0;
            int w = _width * _height;

            // Write to stream
            stream0.read(_input);
    
            // Run the brook program 
            while (w > 1)
            {
                w >>= 1;
                unsigned int domainStart1[] = {0};
                unsigned int domainEnd1[] = {w};
                unsigned int domainStart2[] = {w};
                unsigned int domainEnd2[] = {w<<1};
                
                if (!inp)
                {
                    copy(stream0, stream1);
                    haar_wavelet(stream0, avgStream.domain(domainStart1, domainEnd1),
                                 diffStream.domain(domainStart1, domainEnd1), 1.0f / sqrt(2.0f));

                    copy(avgStream.domain(domainStart1, domainEnd1) , stream1.domain(domainStart1, domainEnd1));
                    copy(diffStream.domain(domainStart1, domainEnd1), stream1.domain(domainStart2, domainEnd2));
                }
                else
                {
                    copy(stream1, stream0);
                    haar_wavelet(stream1, avgStream.domain(domainStart1, domainEnd1),
                                 diffStream.domain(domainStart1, domainEnd1), 1.0f / sqrt(2.0f));

                    copy(avgStream.domain(domainStart1, domainEnd1) , stream0.domain(domainStart1, domainEnd1));
                    copy(diffStream.domain(domainStart1, domainEnd1), stream0.domain(domainStart2, domainEnd2));
                }

                inp = 1 - inp;
            }

            // Write data back from stream 
            if(!inp)
            {
                stream0.write(_output);
                
                if(stream0.error())
                {
                    std::cout << "Error occured" << std::endl;
                    std::cout << stream0.errorLog() << std::endl;
                    retVal = -1;
                }
            }
            else
            {
                stream1.write(_output);
                
                if(stream1.error())
                {
                    std::cout << "Error occured" << std::endl;
                    std::cout << stream1.errorLog() << std::endl;
                    retVal = -1;
                }
            }
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

HaarWavelet::~HaarWavelet()
{
    if(_input != NULL)
    {
        delete [] _input;
    }
    
    if(_output != NULL)
    {
        delete [] _output;
    }
}
