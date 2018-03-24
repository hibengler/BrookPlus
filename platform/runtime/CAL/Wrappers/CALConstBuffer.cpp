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


#include "CALConstBuffer.h"
#include "Utility.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
//! \param dimensions dimension of the Buffer
//! \param device Device associated to CALBuffer
//! \param format 
//!
////////////////////////////////////////////////////////////////////////////////

CALConstBuffer::CALConstBuffer(unsigned int* dimensions, BaseDevice* device, CALformat format)
                              :CALBuffer(1, dimensions, format, BUFFER_HOST, 0, device),
                              _nBytes(0)
{
    unsigned int bufferBytes = getElementBytes();
    unsigned int totalBytes = dimensions[0] * bufferBytes;
    _data = new unsigned char[totalBytes];
    memset((void*)_data, 0, totalBytes);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method that tells const buffer that it is going to be reused
//!
////////////////////////////////////////////////////////////////////////////////

void
CALConstBuffer::reuse()
{
    // Set the previous data to 0
    memset((void*)_data, 0, _nBytes);
    _nBytes = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Sets the cpu pointer data with constant values for 8/16 bit integers
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALConstBuffer::_convertHook(void* data, BRformat format, unsigned int count)
{
    uint4* tempData = NULL;
    unsigned short bytes = 0;
    unsigned short bufferBytes = getElementBytes();
    
    switch(format)
    {
        case BR_FORMAT_UCHAR_1:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);
            
            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((uchar*)data)[i];
                tempData[i].y = 0;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_UCHAR_2:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);
            
            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((uchar2*)data)[i].x;
                tempData[i].y = (unsigned int)((uchar2*)data)[i].y;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_UCHAR_3:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);
            
            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((uchar3*)data)[i].x;
                tempData[i].y = (unsigned int)((uchar3*)data)[i].y;
                tempData[i].z = (unsigned int)((uchar3*)data)[i].z;
                tempData[i].w = (unsigned int)0;
            }
            break;
        case BR_FORMAT_UCHAR_4:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i< count; i++)
            {
                tempData[i].x = (unsigned int)((uchar4*)data)[i].x;
                tempData[i].y = (unsigned int)((uchar4*)data)[i].y;
                tempData[i].z = (unsigned int)((uchar4*)data)[i].z;
                tempData[i].w = (unsigned int)((uchar4*)data)[i].w;
            }
            break;
        case BR_FORMAT_CHAR_1:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((char*)data)[i];
                tempData[i].y = 0;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_CHAR_2:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);
            
            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((char2*)data)[i].x;
                tempData[i].y = (unsigned int)((char2*)data)[i].y;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_CHAR_3:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((char3*)data)[i].x;
                tempData[i].y = (unsigned int)((char3*)data)[i].y;
                tempData[i].z = (unsigned int)((char3*)data)[i].z;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_CHAR_4:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((char4*)data)[i].x;
                tempData[i].y = (unsigned int)((char4*)data)[i].y;
                tempData[i].z = (unsigned int)((char4*)data)[i].z;
                tempData[i].w = (unsigned int)((char4*)data)[i].w;
            }
            break;
        case BR_FORMAT_USHORT_1:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((ushort*)data)[i];
                tempData[i].y = 0;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_USHORT_2:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((ushort2*)data)[i].x;
                tempData[i].y = (unsigned int)((ushort2*)data)[i].y;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_USHORT_3:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((ushort3*)data)[i].x;
                tempData[i].y = (unsigned int)((ushort3*)data)[i].y;
                tempData[i].z = (unsigned int)((ushort3*)data)[i].z;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_USHORT_4:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((ushort4*)data)[i].x;
                tempData[i].y = (unsigned int)((ushort4*)data)[i].y;
                tempData[i].z = (unsigned int)((ushort4*)data)[i].z;
                tempData[i].w = (unsigned int)((ushort4*)data)[i].w;
            }
            break;
        case BR_FORMAT_SHORT_1:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((short*)data)[i];
                tempData[i].y = 0;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_SHORT_2:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((short2*)data)[i].x;
                tempData[i].y = (unsigned int)((short2*)data)[i].y;
                tempData[i].z = 0;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_SHORT_3:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((short3*)data)[i].x;
                tempData[i].y = (unsigned int)((short3*)data)[i].y;
                tempData[i].z = (unsigned int)((short3*)data)[i].z;
                tempData[i].w = 0;
            }
            break;
        case BR_FORMAT_SHORT_4:
            tempData = new uint4[count];
            bytes = sizeof(unsigned int);

            for(unsigned int i = 0; i < count; i++)
            {
                tempData[i].x = (unsigned int)((short4*)data)[i].x;
                tempData[i].y = (unsigned int)((short4*)data)[i].y;
                tempData[i].z = (unsigned int)((short4*)data)[i].z;
                tempData[i].w = (unsigned int)((short4*)data)[i].w;
            }
            break;
        default:
            break;
    }

    if(tempData)
    {
        for(unsigned int i = 0; i < count; ++i)
        {
            memcpy(_data + _nBytes, (char*)tempData + i * bytes * 4, bytes * 4);
            _nBytes += bufferBytes;
        }

        delete [] tempData;
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Sets the cpu pointer data with constant values
//!
////////////////////////////////////////////////////////////////////////////////
void
CALConstBuffer::pushConstant(void* data, BRformat format, unsigned int count)
{
    if(_convertHook(data, format, count))
    {
        return;
    }

    unsigned short bytes = Utility::getElementBytes(format);
    unsigned short bufferBytes = getElementBytes();
    
    // initialize data and set number of bytes initialized
    if(bytes == bufferBytes)
    {
        memcpy(_data + _nBytes, data, bytes * count);
        _nBytes += bufferBytes * count;
    }
    else
    {
        for(unsigned int i = 0; i < count; ++i)
        {
            memcpy(_data + _nBytes, (char*)data + i * bytes, bytes);
            _nBytes += bufferBytes;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Transfer data from cpu pointer _data to GPU buffer
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALConstBuffer::setDataToBuffer()
{
    CALuint pitch;
    void* data = getBufferPointerCPU(pitch);
    if(!data)
    {
        return false;
    }

    memcpy(data, _data, _nBytes);
    freeBufferPointerCPU();

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALConstBuffer::~CALConstBuffer()
{
    delete[] _data;
}

