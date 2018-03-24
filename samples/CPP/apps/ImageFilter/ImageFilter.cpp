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

#include "ImageFilter.h"
#include "brookgenfiles/image_filter.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

ImageFilter::ImageFilter(char* name) : SampleBase(name)
{
    _input = NULL;
    _output = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
ImageFilter::allocateBuffer()
{
    _height = info->Height;
    _width =info->Width;

    _input = Util::allocateBuffer<float>(_width + 2, _height + 2);
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
ImageFilter::fillInputBuffer()
{
    Util::fillBuffer(_input, _width + 2, _height + 2, 1, 1, Util::RANDOM);
    Util::fillBuffer((float*)_mask, 3, 3, 1, 1, Util::RANDOM);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  C = A (+) B
//!         Convolution of given image
//!
//! \param input    Input matrix
//! \param mask     mask used to convolve (3 x 3)
//! \param output   Output matrix
//! \param Width    Columns in output
//! \param Height   Rows in output
//! 
////////////////////////////////////////////////////////////////////////////////

void 
ImageFilter::_imageFilterCPU(float* input, float mask[3][3], float* output, 
                              unsigned int Width, unsigned int Height)
{
    unsigned int i, j;
    unsigned int insize = Width + 2;
    for(i = 0; i < Height; i++){
        for(j = 0; j < Width; j++){
            int ii, jj;
            float tmp = 0.0f;
            int a;
            ii = i + 1;
            jj = j + 1;
            for (a = -1; a <= 1; ++a)
            {
                int b;
                for (b = -1; b <= 1; ++b)
                {
                    tmp += input[(ii + a) * insize + (jj + b)] * mask[a + 1][b + 1];
                }
            }
            output[i * Width + j] = tmp;
        }
    } 
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
ImageFilter::printTimeInfo()
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
ImageFilter::verifyResults()
{
    int res = 0;
    float *CPU = Util::allocateBuffer<float>(_width, _height);

    if (CPU)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Applying Image Filter on CPU ... ";

        _imageFilterCPU(_input, _mask, CPU, _width, _height);

        std::cout << "Done\n";

        res += Util::compareBuffers<float>(CPU, _output, _width * _height);
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
ImageFilter::comparePerformance()
{
    unsigned int i;
    double cpu_time = 0.0;
    double gpu_time = timer->GetElapsedTime();
    float *CPU = Util::allocateBuffer<float>(_width, _height);

    if (CPU)
    {
        std::cout << "-p Compare performance with CPU.\n";

        // Record CPU Total time

        timer->Reset();
        timer->Start();

        for(i = 0; i < info->Iterations; i ++)
        {
            _imageFilterCPU(_input, _mask, _output, _width, _height);
        }

        timer->Stop();

        // Print CPU timing and speedup
        cpu_time = timer->GetElapsedTime();

        std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
                  << std::setw(16) << "GPU Total Time" << std::setw(16) << "Speedup" 
                  << std::endl;

        std::cout << std::setw(8) << _width << std::setw(8) << _height
                  << std::setw(16) << info->Iterations << std::setw(16) << cpu_time
                  << std::setw(16) << gpu_time << std::setw(16) << cpu_time / gpu_time 
                  << std::endl;

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
ImageFilter::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////

    // Stream dimensions
    unsigned int dimImg[] = {_width + 2, _height + 2};
    unsigned int dimOImg[] = {_width, _height};

    Stream<float> img(2, dimImg);
    Stream<float> oimg(2, dimOImg);

    // Record GPU Total time
    timer->Start();

    for (unsigned int itr = 0; itr < info->Iterations; ++itr)
    {
        // Copying Input
        img.read(_input);

        // Running Image filter
        image_filter(img, _mask, oimg);

        // Copying data back to HOST memory
        oimg.write(_output);

        // Error checking on the output stream
        if(oimg.error())
        {
            std::cout << "Error occured" << std::endl;
            std::cout << oimg.errorLog() << std::endl;
            retVal = -1;
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

ImageFilter ::~ImageFilter()
{
    delete [] _input;
    delete [] _output;
}
