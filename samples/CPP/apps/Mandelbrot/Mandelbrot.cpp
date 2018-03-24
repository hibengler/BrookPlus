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

#include "Mandelbrot.h"
#include "brookgenfiles/mandelbrot_gpu.h"

#define SCALE 3.0f
#define MAXITERATIONS 1000.0f

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

Mandelbrot::Mandelbrot(char* name) : SampleBase(name)
{
    _mandelbrotArray = NULL;
    _width = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
Mandelbrot::allocateBuffer()
{
    _width = info->Width;

    _mandelbrotArray = Util::allocateBuffer<float>(_width, _width);

    if (!_mandelbrotArray)
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Generates an image using mandelbrot fractal generator on the CPU
//!
//! \param scale            Represents the distance from which the fractal
//!                         is being seen if this is greater more area and
//!                         less detail is seen
//! \param maxIterations    More iterations gives more accurate mandelbrot image
//! \param mandelbrotCPU    output image
//! \param size             size of image
//! 
////////////////////////////////////////////////////////////////////////////////

void 
Mandelbrot::mandelbrot_cpu(float scale, float maxIterations, float* mandelbrotCPU, int size)
{
    int i, j;
    float iteration;
    float x,y,x0,y0,x2,y2;
    float scaleSquare;

    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
        {
            x0 = ((2*j) - size) * scale/2.0f;
            y0 = ((2*i) - size) * scale/2.0f;

            x  = x0;
            y  = y0;

            x2 = x*x;
            y2 = y*y;

            scaleSquare = scale * scale * size * size;
            for(iteration = 0.0f; (x2+y2 < scaleSquare) && (iteration < maxIterations); iteration += 1.0f)
            {
                y = 2.0f*(x*y) + y0;
                x = (x2 - y2)  + x0;

                x2  = x*x;
                y2  = y*y;
            }
            mandelbrotCPU[i*size + j] = iteration/maxIterations;
        }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
Mandelbrot::printTimeInfo()
{
    double time = timer->GetElapsedTime();

    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::endl;

    std::cout << std::setw(8) << _width << std::setw(8) << _width
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
Mandelbrot::verifyResults()
{
    int res = 0;
    float* CPU = Util::allocateBuffer<float>(_width, _width);

    if(CPU)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Performing Mandelbrot on CPU ... ";

        mandelbrot_cpu(SCALE, MAXITERATIONS, CPU, _width);
        
        std::cout << "Done\n";

        res += Util::compareBuffers<float>(CPU, _mandelbrotArray, _width * _width);
        
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
Mandelbrot::comparePerformance()
{
    unsigned int i;
    double cpuTime = 0.0;
    double gpuTime = 0.0;
    
    float* CPU = Util::allocateBuffer<float>(_width, _width);

    gpuTime = timer->GetElapsedTime();
    
    if(CPU)
    {
        timer->Reset();
        timer->Start();
        
        std::cout << "-p Compare performance with CPU.\n";
        
        // Record CPU Total time
        for(i = 0; i < info->Iterations; i ++)
        {
            mandelbrot_cpu(SCALE, MAXITERATIONS, CPU, _width);
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

    std::cout << std::setw(8) << _width << std::setw(8) << _width
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
Mandelbrot::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    {
        unsigned int dim[] = {_width, _width};
        
        ::brook::Stream<float> mandelbrotStream(2, dim);

        // Record GPU Total time
        timer->Start();
        for (unsigned int i = 0; i < info->Iterations; ++i)
        {
            // Running Mandelbrot
            mandelbrot(SCALE, MAXITERATIONS, (float)_width, mandelbrotStream);

            mandelbrotStream.write(_mandelbrotArray);

            //Handle errors if any
            if(mandelbrotStream.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << mandelbrotStream.errorLog() << std::endl;
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

Mandelbrot ::~Mandelbrot()
{
    delete [] _mandelbrotArray;
}
