#ifndef _NLMDENOISE_H_
#define _NLMDENOISE_H_

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
  \file NLMDenoise.h
  \brief Contains the class definition for NLMDenoise class
 ****************************************************************************/

#include "SampleBase.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class  NLMDenoise
//!
//! \brief  contains application properties for the sample nlm_denoise
//! 
////////////////////////////////////////////////////////////////////////////////

class NLMDenoise : public SampleBase
{
    public:

        NLMDenoise(char* name);
        void allocateBuffer();
        int run();
        void printTimeInfo();
        ~NLMDenoise();

    private:

        uchar4* _convert8(unsigned char* data, unsigned int &height, unsigned int &width, BRRGBQUAD* colors);
        uchar4* _convert24(unsigned char* data, unsigned int &height, unsigned int &width);
        uchar4* _openImage(std::string filename, unsigned int &width, unsigned int &height);
        void _saveImage(std::string filename, uchar4* data, unsigned int &width, unsigned int &height);
        void _findMinMaxparam(uchar4* matrix, unsigned int height, unsigned int width, float &tmin, float &tmax);
        void _DeNormalize_Output(float4* matrix, uchar4* out, unsigned int height,
                                unsigned int width, float min, float max);

    private:

        //! Input image
        uchar4* _inputImage;

        //! Result Image
        uchar4* _outputImage;

        //! image width
        unsigned int _width;

        //! image height
        unsigned int _height;

        //! input image name
        static const char* _imgStr;

        //! output image name
        static const char* _outImgStr;
};

#endif // _NLMDENOISE_H_
