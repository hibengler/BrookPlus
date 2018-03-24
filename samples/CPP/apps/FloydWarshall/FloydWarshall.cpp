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

#include "FloydWarshall.h"
#include "brookgenfiles/floyd_warshall.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

FloydWarshall::FloydWarshall(char* name)
              :SampleBase(name)
{
    for(unsigned int i = 0; i < 2; i++)
    {
        _input[i] = NULL;
        _output[i] = NULL;
        _cpu[i] = NULL;
    }

    _width = 0;
    _height = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
FloydWarshall::allocateBuffer()
{
    _width = info->Width;
    _height = info->Height;
    unsigned int length = _width * _height;
    
    for(unsigned int i = 0; i < 2; i++)
    {
        _input[i] = Util::allocateBuffer<float>(length, 1);
        _output[i] = Util::allocateBuffer<float>(length, 1);
        _cpu[i] = Util::allocateBuffer<float>(length, 1);
            
        if (!_input[i] || !_output[i] || !_cpu[i])
        {
            fprintf(stderr, "Error: Memory Allocation.\n");
            exit(-1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
FloydWarshall::fillInputBuffer()
{
    Util::fillBuffer(_input[0], _width, _height, 1, 100, Util::RANDOM);
    Util::fillBuffer(_input[1], _width, _height, 1, _width, Util::POSY);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Auxiliary function for the CPU implementation of FloydWarshall
//!
////////////////////////////////////////////////////////////////////////////////

void 
FloydWarshall::floyd_warshall_cpu_inner(float* idist, float* ipos, float* odist,
                                       float* opos, int step, unsigned int Height, unsigned int Width)
{
    unsigned int y;
    for (y = 0; y < Height; ++y)
    {
        unsigned int x;
        for (x = 0; x < Width; ++x)
        {
            float dirDist;
            float indirDist;
            int idx1 = step * Width + x;
            int idx2 = y * Width + step;
            int index = y * Width + x;
            dirDist = idist[index];
            indirDist = idist[idx1] + idist[idx2];

            odist[index]= (dirDist < indirDist) ? dirDist: indirDist;
            opos[index]  = (dirDist < indirDist) ? ipos[index]: ipos[idx1];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  The CPU implementation of FloydWarshall algorithm
//! 
////////////////////////////////////////////////////////////////////////////////

void 
FloydWarshall::floyd_warshall_cpu(float* idist, float* ipos, float* odist, float* opos,
                                 unsigned int Length, unsigned int Height, unsigned int Width)
{
    unsigned int inp = 0;
    unsigned int i;
    float *tmp[4] = {NULL};
    for (i = 0; i < 4; ++i)
    {
        if(!(tmp[i] = Util::allocateBuffer<float>(Length, 1)))
        {
            fprintf(stderr, "Error: Memory Allocation.\n");
            goto tmp_cleanup;
        }
    }

    memcpy(tmp[0], idist, sizeof *idist * Length);
    memcpy(tmp[1], ipos, sizeof *ipos * Length);
    for (i = 0; i < Height; ++i)
    {
        if (!inp)
        {
            floyd_warshall_cpu_inner(tmp[0], tmp[1], tmp[2], tmp[3], i, Height, Width);
        }
        else
        {
            floyd_warshall_cpu_inner(tmp[2], tmp[3], tmp[0], tmp[1], i, Height, Width);
        }
        inp = 1 - inp;
    }
    if (!inp)
    {
        memcpy(odist, tmp[0], sizeof *odist * Length);
        memcpy(opos, tmp[1], sizeof *opos * Length);
    }
    else
    {
        memcpy(odist, tmp[2], sizeof *odist * Length);
        memcpy(opos, tmp[3], sizeof *opos * Length);
    }

tmp_cleanup:
    for (i = 0; i < 4; ++i)
    {
        if (tmp[i])
        {
            delete [] tmp[i];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
FloydWarshall::printTimeInfo()
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
FloydWarshall::verifyResults()
{
    unsigned int length = _width * _height;
    int res = 0;

    std::cout<<"-e Verify correct output.\n";
    std::cout<<"Performing Floyd Warshall on CPU ... ";

    floyd_warshall_cpu(_input[0], _input[1], _cpu[0], _cpu[1], length, _height, _width);
    
    std::cout << "Done\n";

    res += Util::compareBuffers<float>(_output[0], _cpu[0], length);
    res += Util::compareBuffers<float>(_output[1], _cpu[1], length);
    
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
FloydWarshall::comparePerformance()
{
    unsigned int length = _width * _height;
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
        floyd_warshall_cpu(_input[0], _input[1], _cpu[0], _cpu[1], length, _height, _width);
    }
    
    timer->Stop();

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
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
FloydWarshall::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    
    unsigned int dim[] = {_width, _height};
    
    ::brook::Stream<float> distStream0(2, dim);
    ::brook::Stream<float> distStream1(2, dim);
    ::brook::Stream<float> nextPosStream0(2, dim);
    ::brook::Stream<float> nextPosStream1(2, dim);

    // Record GPU Total time
    timer->Start();

    for (unsigned int j = 0; j < info->Iterations; ++j)
    {
        // Write to stream 
        distStream0.read(_input[0]);
        nextPosStream0.read(_input[1]);

        // This is a multi-pass algorithm. We call the kernel as many times as there are number of nodes in the  graph.
        // Run the brook program here
        int inp = 0;
        for (unsigned i = 0; i < _height; ++i)
        {
            if (!inp)
            {
                floyd_warshall(distStream0, nextPosStream0, distStream1, nextPosStream1, i);
            }
            else
            {
                floyd_warshall(distStream1, nextPosStream1, distStream0, nextPosStream0, i);
            }
            inp = 1 - inp;
        }

        // Write data back from stream 
        if (!inp)
        {
            distStream0.write(_output[0]);
            //Handle errors if any
            if(distStream0.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << distStream0.errorLog() << std::endl;
                retVal = -1;
            }
            
            nextPosStream0.write(_output[1]);
            
            //Handle errors if any
            if(nextPosStream0.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << nextPosStream0.errorLog() << std::endl;
                retVal = -1;
            }
        }
        else
        {
            distStream1.write(_output[0]);

            //Handle errors if any
            if(distStream1.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << distStream1.errorLog() << std::endl;
                retVal = -1;
            }

            nextPosStream1.write(_output[1]);

            //Handle errors if any
            if(nextPosStream1.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << nextPosStream1.errorLog() << std::endl;
                retVal = -1;
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

FloydWarshall ::~FloydWarshall()
{
    for(unsigned int i = 0; i < 2; i++)
    {
        delete _input[i];
        delete _output[i];
        delete _cpu[i];
    }
}
