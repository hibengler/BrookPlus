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

#include "NLMDenoise.h"
#include "brookgenfiles/NLM_Denoise.h"


//! input image name
const char* NLMDenoise::_imgStr = "image_noise.bmp";

//! output image name
const char* NLMDenoise::_outImgStr = "image_denoise.bmp";

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

NLMDenoise::NLMDenoise(char* name) : SampleBase(name)
{
    _inputImage = NULL;
    _outputImage = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
NLMDenoise::allocateBuffer()
{
    _inputImage = Util::openImage(_imgStr, _width, _height);
    if (!_inputImage)
    {
        fprintf(stderr, "Error: Image loading.\n");
        exit(-1);
    }

    _outputImage = Util::allocateBuffer<float4>(_width, _height);
    if (!_outputImage)
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
NLMDenoise::printTimeInfo()
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
//! \brief  backend implementation for simple matmult sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
NLMDenoise::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////

    // Stream dimensions
    unsigned int dimension[] = {_width, _height};

    Stream<float4> image_stream(2, dimension);
    Stream<float4> output_stream(2, dimension);
    Stream<float4> weight_stream(2, dimension);

    // Record GPU Total time
    timer->Start();

    for (unsigned int itr = 0; itr < info->Iterations; ++itr)
    {
        // Copying Input
        image_stream.read(_inputImage);

        // Get the mask weights first
        NLM_Denoise_Pass1(weight_stream, image_stream, 1.0f / (1.45f * 1.45f), 1.0f/49.0f);

        // Apply Convolution
        NLM_Denoise_Pass2(output_stream, weight_stream, image_stream, 1.0f/49.0f);

        // Copying data back to HOST memory
        output_stream.write(_outputImage);

        // Error checking on the output stream
        if(output_stream.error())
        {
            std::cout << "Error occured" << std::endl;
            std::cout << output_stream.errorLog() << std::endl;
            retVal = -1;
        }
    }

    timer->Stop();

    std::cout << "Saving image: " << _outImgStr << "\n";
    Util::saveImage(_outImgStr, _outputImage, _width, _height);

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

NLMDenoise ::~NLMDenoise()
{
    delete [] _inputImage;
    delete [] _outputImage;
}
