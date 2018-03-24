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
//! \brief  convert the unsigned byte information to 24 bit representation
//! 
////////////////////////////////////////////////////////////////////////////////

uchar4*
NLMDenoise::_convert8(unsigned char* data, unsigned int &height, unsigned int &width, BRRGBQUAD* colors)
{
    uchar4* image;
    unsigned int offset, diff;
    diff = width * height * RGB_BYTE_SIZE;
    
    if (!(image = Util::allocateBuffer<uchar4>(width, height)))
    {
        std::cout << "Unable to allocate image matrix data." << std::endl;
        return NULL;
    }

    if (height > 0)
    {
        unsigned int j = 0;
        offset = padWidth - byteWidth;
        for (unsigned int i = 0; i < datasize * RGB_BYTE_SIZE; i += 3)
        {
            if ((i + 1) % padWidth == 0)
            {
                i += offset;
            }
            (*(image + i)).x = colors[*(data + j)].rgbRed;
            (*(image + i)).y = colors[*(data + j)].rgbGreen;
            (*(image + i)).z = colors[*(data + j)].rgbBlue;
            j++;
        }
    }
    else
    {
        unsigned int j = datasize - 1;
        offset = padWidth - byteWidth;
        for (unsigned int i = 0; i < datasize * RGB_BYTE_SIZE; i += 3)
        {
            if ((i + 1) % padWidth == 0)
            {
                i += offset;
            }
            (*(image + i)).x = colors[*(data + j)].rgbRed;
            (*(image + i)).y = colors[*(data + j)].rgbGreen;
            (*(image + i)).z = colors[*(data + j)].rgbBlue;
            j--;
        }
    }
    
    return image;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  convert the unsigned byte information to 24 bit representation
//! 
////////////////////////////////////////////////////////////////////////////////

uchar4*
NLMDenoise::_convert24(unsigned char* data, unsigned int &height, unsigned int &width)
{
    uchar4* image;
    unsigned int offset, diff;
    diff = width * height * RGB_BYTE_SIZE;
    if (!(image = Util::allocateBuffer<uchar4>(width, height)))
    {
        std::cout << "Unable to allocate image matrix data." << std::endl;
        return NULL;
    }
    if (height > 0)
    {
        int j = 0;
        offset = padWidth - byteWidth;
        for (unsigned int i = 0; i < datasize; i += 3)
        {
            if (((i + 1) % padWidth) == 0)
            {
                i += offset;
            }
            (*(image + j)).x = (*(data + i + 2));
            (*(image + j)).y = (*(data + i + 1));
            (*(image + j)).z = (*(data + i + 0));
            ++j;
        }
    }
    else
    {
        int j;
        offset = padWidth - byteWidth;
        j = datasize / 3;
        for (unsigned int i = 0; i < datasize; i += 3)
        {
            if (((i + 1) % padWidth) == 0)
            {
                i += offset;
            }
            (*(image + j)).x = (*(data + i + 2));
            (*(image + j)).y = (*(data + i + 1));
            (*(image + j)).z = (*(data + i + 0));
            --j;
        }
    }
    
    return image;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Read the input file and extract data into unsigned byte buffer
//! 
////////////////////////////////////////////////////////////////////////////////

uchar4*
NLMDenoise::_openImage(std::string filename, unsigned int &width, unsigned int &height)
{
    uchar4* image   = NULL;
    FILE *fid       = NULL;
    unsigned char *tempData  = NULL;
    BRRGBQUAD* colors = NULL;
    unsigned int numColors = 0;
    BRBITMAPFILEHEADER bmfh = {0, 0, 0, 0, 0};
    BRBITMAPINFOHEADER bmih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (!(fid = fopen(filename.c_str(), "rb")))
    {
        std::cout << "Error opening file: " << filename.c_str() << std::endl;
        return NULL;
    }

    fread(&bmfh.bfType, 2, 1, fid);
    fread(&bmfh.bfSize, 4, 1, fid);
    fread(&bmfh.bfReserved1, 2, 1, fid);
    fread(&bmfh.bfReserved2, 2, 1, fid);
    fread(&bmfh.bfOffBits, 4, 1, fid);
    if (bmfh.bfType != BITMAP_MAGIC_NUMBER)
    {
        std::cout << "File is not in DIB format\n" << std::endl;
        goto exit;
    }
    fread(&bmih, sizeof(bmih), 1, fid);
    height = bmih.biHeight;
    width = bmih.biWidth;
    numColors = 1 << bmih.biBitCount;
    datasize = width * height * (bmih.biBitCount >> 3);
    if (bmih.biBitCount == 8)
    {
        colors = (BRRGBQUAD*)malloc(sizeof*colors * numColors);
        if (!(colors))
        {
            std::cout << "Error allocating space for color data" << std::endl;
            goto exit;
        }
        fread(colors, sizeof*colors, numColors, fid);
    }

    if(!(tempData = (unsigned char*)malloc(sizeof *tempData * datasize)))
    {
        printf("Error allocating temp data space\n");
        goto exit;
    }

    fread(tempData, sizeof *tempData, datasize, fid);

    byteWidth = padWidth = (width * bmih.biBitCount) >> 3;

    while((padWidth % 4) != 0) padWidth++;
    switch(bmih.biBitCount)
    {
        case 8:
            image = _convert8(tempData, height, width, colors);
            break;
        case 16:
            std::cout << "Does not support 16 bit images yet." << std::endl;
            break;
        case 24:
            image = _convert24(tempData, height, width);
            break;
        case 32:
            std::cout << "Does not support 32 bit images yet." << std::endl;
            break;
    }

    exit:
        if (tempData)
        {
            free(tempData);
        }
        if (colors)
        {
            free(colors);
        }
        if(fid)
        {
            fclose(fid);
        }
        
        return image;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief save the unsigned byte data into file 
//! 
////////////////////////////////////////////////////////////////////////////////

void 
NLMDenoise::_saveImage(std::string filename, uchar4* data, unsigned int &width, unsigned int &height)
{
    BRBITMAPFILEHEADER bmfh = {0, 0, 0, 0, 0};
    BRBITMAPINFOHEADER bmih = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int  offset, diff;
    FILE *fid = NULL;
    bmih.biWidth = width;
    bmih.biHeight = height;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biSize = 40;
    bmih.biXPelsPerMeter = 2835;
    bmih.biYPelsPerMeter = 2835;
    bmfh.bfType = BITMAP_MAGIC_NUMBER;
    bmfh.bfOffBits = Offset;
    bmfh.bfSize = (width * height * 3) + 40 + 14;
    if (!(fid = fopen(filename.c_str(), "wb")))
    {
        std::cout << "Error creating file: %s" << filename.c_str() << std::endl;
        return;
    }
    
    fwrite(&bmfh.bfType, 2, 1, fid);
    fwrite(&bmfh.bfSize, 4, 1, fid);
    fwrite(&bmfh.bfReserved1, 2, 1, fid);
    fwrite(&bmfh.bfReserved2, 2, 1, fid);
    fwrite(&bmfh.bfOffBits, 4, 1, fid);
    fwrite(&bmih, sizeof(bmih), 1, fid);
    diff = width * height * RGB_BYTE_SIZE;
    byteWidth = padWidth = (width * bmih.biBitCount) >> 3;
    datasize = (byteWidth * height);
    
    while((padWidth % 4) != 0) padWidth++;
    
    if (height > 0)
    {
        unsigned int j = 0;
        offset = padWidth - byteWidth;
        for (unsigned int i = 0; i < datasize; i += 3)
        {
            if (((i + 1) % padWidth) == 0)
            {
                i += offset;
            }
            fprintf(fid,"%c", data[j].z);
            fprintf(fid,"%c", data[j].y);
            fprintf(fid,"%c", data[j].x);
            ++j;
        }
    }
    else
    {
        unsigned int j = diff / 3;
        offset = padWidth - byteWidth;
        for (unsigned int i = 0; i < datasize; i += 3)
        {
            if (((i + 1) % padWidth) == 0)
            {
                i += offset;
            }
            fprintf(fid,"%c", data[j].z);
            fprintf(fid,"%c", data[j].y);
            fprintf(fid,"%c", data[j].x);
            --j;
        }
    }

    fclose(fid);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Find max, min value for GPU normalization of the image buffer data 
//! 
////////////////////////////////////////////////////////////////////////////////

void 
NLMDenoise::_findMinMaxparam(uchar4* matrix, unsigned int height, unsigned int width, float &tmin, float &tmax)
{
    tmin = (float)INT_MAX;
    tmax = (float)INT_MIN;
    unsigned int size = height * width;
    uchar4* ptr = matrix;
    do {
        tmax = (ptr->x > tmax) ? ptr->x : tmax;
        tmin = (ptr->x < tmin) ? ptr->x : tmin;
        tmax = (ptr->y > tmax) ? ptr->y : tmax;
        tmin = (ptr->y < tmin) ? ptr->y : tmin;
        tmax = (ptr->z > tmax) ? ptr->z : tmax;
        tmin = (ptr->z < tmin) ? ptr->z : tmin;
        tmax = (ptr->w > tmax) ? ptr->w : tmax;
        tmin = (ptr->w < tmin) ? ptr->w : tmin;
    }while(++ptr < (matrix + size));
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  denormalized the float data to unsigned byte
//! 
////////////////////////////////////////////////////////////////////////////////

void 
NLMDenoise::_DeNormalize_Output(float4* matrix, uchar4* out, unsigned int height, unsigned int width, float min, float max)
{
    float tmin = (float)INT_MAX;
    float tmax = (float)INT_MIN;
    unsigned int size = height * width;
    float4* ptr = matrix;
    do {
        tmax = (ptr->x > tmax) ? ptr->x : tmax;
        tmin = (ptr->x < tmin) ? ptr->x : tmin;
        tmax = (ptr->y > tmax) ? ptr->y : tmax;
        tmin = (ptr->y < tmin) ? ptr->y : tmin;
        tmax = (ptr->z > tmax) ? ptr->z : tmax;
        tmin = (ptr->z < tmin) ? ptr->z : tmin;
        tmax = (ptr->w > tmax) ? ptr->w : tmax;
        tmin = (ptr->w < tmin) ? ptr->w : tmin;
    }while(++ptr < (matrix + size));
    ptr = matrix;
    do {
        out->x = (char)(int)((((ptr->x - tmin) * (max - min)) / (tmax - tmin)) + min);
        out->y = (char)(int)((((ptr->y - tmin) * (max - min)) / (tmax - tmin)) + min);
        out->z = (char)(int)((((ptr->z - tmin) * (max - min)) / (tmax - tmin)) + min);
        out->w = (char)(int)((((ptr->w - tmin) * (max - min)) / (tmax - tmin)) + min);
        ++out;
    }while(++ptr < (matrix + size));
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
NLMDenoise::allocateBuffer()
{
    _inputImage = _openImage(_imgStr, _width, _height);
    if (!_inputImage)
    {
        fprintf(stderr, "Error: Image loading.\n");
        exit(-1);
    }

    _outputImage = Util::allocateBuffer<uchar4>(_width, _height);
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

    Stream<uchar4> image_stream(2, dimension);
    Stream<float4> normalizedInput_stream(2, dimension);
    Stream<float4> output_stream(2, dimension);
    Stream<float4> weight_stream(2, dimension);

    // Record GPU Total time
    timer->Start();

    for (unsigned int itr = 0; itr < info->Iterations; ++itr)
    {
        float tmin = 0.0f;
        float tmax = 0.0f;
        float4* tempOut = NULL;

        tempOut = new float4[_width * _height];
        memset(tempOut, 0, sizeof(float4)* _width * _height);
        _findMinMaxparam(_inputImage, _height, _width, tmin, tmax);
        
        // Copying Input
        image_stream.read(_inputImage);
        
        //Normalized input image
        Normalize_Input(image_stream, tmax, tmin, normalizedInput_stream);
        
        // Get the mask weights first
        NLM_Denoise_Pass1(weight_stream, normalizedInput_stream, 1.0f / (1.45f * 1.45f), 1.0f/49.0f);
        
        // Apply Convolution
        NLM_Denoise_Pass2(output_stream, weight_stream, normalizedInput_stream, 1.0f/49.0f);
        
        // Copying data back to HOST memory
        output_stream.write(tempOut);
        
        _DeNormalize_Output(tempOut, _outputImage, _height, _width, 0.0f, 255.0f);
        
        // Error checking on the output stream
        if(output_stream.error())
        {
            std::cout << "Error occured" << std::endl;
            std::cout << output_stream.errorLog() << std::endl;
            retVal = -1;
        }

        delete [] tempOut;
    }

    timer->Stop();

    std::cout << "Saving image: " << _outImgStr << "\n";
    _saveImage(_outImgStr, _outputImage, _width, _height);

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
