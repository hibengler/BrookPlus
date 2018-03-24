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

#include "CommonDefs.h"
#include "ErrorCodes.h"
#include "BufferMgr.h"
#include "Buffer.h"
#include "StreamImpl.h"
#include "Device.h"
#include "Utility.h"

using namespace brook;

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

BufferMgr::BufferMgr(BaseDevice* device) : _device(device)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create backend specific buffers using CALBuffer.
//!
//! Contains logic to use local buffer cache to reuse the unused buffer
//!
//! \param stream The associated stream
//! \return Pointer to the newly created buffer
//!
////////////////////////////////////////////////////////////////////////////////

Buffer*
BufferMgr::getBuffer(StreamImpl* stream, unsigned int i)
{
    // create a buffer with information of datatype and size
    // but don't allocate it on device

    if(stream->getInitialized(i))
    {
        return NULL;
    }
    else
    {
        stream->setInitialized(true, i);
    }

    Buffer* buffer = _createBuffer(stream, i);

    if(!buffer)
    {
        stream->setErrorCode(BR_ERROR_DECLARATION);
        stream->setErrorString("Stream Allocation : Failed to create buffer\n");

        return NULL;
    }

    std::vector<Buffer*> unusedBuffers;

    // Look into cache if a free resource exist of the same size
    for(BufferMapIter itr = _bufferMap.begin(); itr != _bufferMap.end(); ++itr)
    {
        Buffer* cachedBuffer = itr->first;

        // Is the buffer unused
        if(itr->second == false)
        {
            // If size and format matches return the unused buffer
            if(*cachedBuffer == *buffer)
            {
                delete buffer;
                itr->second = true;
                return cachedBuffer;
            }

            // If size is not matching keep all the unused buffers
            unusedBuffers.push_back(cachedBuffer);
        }
    }

    // Ceate if no free buffer in cache and push it in the cache
    if(!buffer->initialize())
    {
        if(unusedBuffers.size())
        {
            // If not able to create buffer try deleting unused buffers
            // and try again
            for(unsigned int i = 0; i < unusedBuffers.size(); ++i)
            {
                Buffer* unusedBuffer = unusedBuffers[i];
                _bufferMap.erase(unusedBuffer);

                delete unusedBuffer;
            }

            // Try allocating again and report error if coudn't allocate
            if(!buffer->initialize())
            {
                stream->setErrorCode(BR_ERROR_DECLARATION);
                stream->setErrorString("Stream Allocation : Failed to create Buffer\n");

                delete buffer;
                return NULL;
            }
        }
        else
        {
            stream->setErrorCode(BR_ERROR_DECLARATION);
            stream->setErrorString("Stream Allocation : Failed to create Buffer\n");

            delete buffer;
            return NULL;
        }
    }

    _bufferMap[buffer] = true;

    return buffer;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Do this backend support interoperability with type
//! Base class returns NULL
//!
////////////////////////////////////////////////////////////////////////////////

Buffer*
BufferMgr::createTypedBuffer(const char* type, StreamImpl* stream)
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.read()
//!
//! \param stream StreamImpl Class
//! \param ptr Application pointer to copy
////////////////////////////////////////////////////////////////////////////////

void
BufferMgr::setBufferData(StreamImpl* stream, const void* ptr, const char* flags)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Returns the backend specific copyShader kernel descriptor.
//!
////////////////////////////////////////////////////////////////////////////////

const brook::KernelDesc* 
BufferMgr::getCopyShaderDesc()
{
    return NULL;
};

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief 
//! return backend specific resize kernel, used in input stream resize 
//! to match their size with output streams
//!
////////////////////////////////////////////////////////////////////////////////

const brook::KernelDesc* 
BufferMgr::getResizeShaderDesc()
{
    return NULL;
};

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.write()
//!
//! \param stream StreamImpl Class
//! \param ptr Application pointer to copy
////////////////////////////////////////////////////////////////////////////////

void
BufferMgr::getBufferData(StreamImpl* stream, void* ptr, const char* flags)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy content of src stream to dst stream
//!
////////////////////////////////////////////////////////////////////////////////

void
BufferMgr::copyData(StreamImpl* dst, StreamImpl* src)
{
    if(!dst->flush())
    {
        dst->setErrorCode(BR_ERROR_ASSIGN);
        dst->setErrorString("Assign : Could not flush previous events\n");

        return;
    }

    if(dst->getDevice() != src->getDevice())
    {
        unsigned int size = 1;
        for(unsigned int i = 0; i < src->getBufferCount(); ++i)
        {
            size *= Utility::getElementBytes(src->getDataFormat(i));
        }

        unsigned int* dimensions = src->getDimensions();
        for(unsigned int i = 0; i < src->getRank(); ++i)
        {
            size *= dimensions[i];
        }

        // Currently, this is the best way to do it given the performance of
        // data copying from one device to another device on the supported backend
        // If a particular backend has better performance, override this method
        char* data = new char[size];
        src->write(data);
        dst->read(data);
        delete[] data;
    }
    else
    {
        // If the device is same we have better methods to copy data
        for(unsigned int i = 0; i < dst->getBufferCount(); ++i)
        {
            // Copy data from other streams aliased with dst stream
            dst->copyDataFromTree();

            Buffer* dstBuffer = dst->getBuffer(i);
            Buffer* srcBuffer = src->getBuffer(i);
            copyBufferData(dstBuffer, srcBuffer);

            // Copy data from to streams aliased with src stream
            src->copyDataToTree();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy content of src Buffer to dst Buffer
//!
////////////////////////////////////////////////////////////////////////////////

void
BufferMgr::copyBufferData(Buffer* dst, Buffer* src)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief The default behavior for the virtual method.
//!
////////////////////////////////////////////////////////////////////////////////

Buffer*
BufferMgr::_createBuffer(StreamImpl *stream, unsigned int i)
{
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief destroy the buffer
//!
//! Turn on the flag in Buffer Cache saying buffer 
//! is now available to use by other stream
//!
//! \param stream StreamImpl Class
//! \param ptr Application pointer to copy
//!
////////////////////////////////////////////////////////////////////////////////

void
BufferMgr::destroyBuffer(Buffer* buffer)
{
    if(buffer)
    {
        if(_bufferMap.find(buffer) != _bufferMap.end())
        {
            // Mark it as destroyed
            _bufferMap[buffer] = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
//! Delete all the buffers available in the cache
//!
////////////////////////////////////////////////////////////////////////////////

BufferMgr::~BufferMgr()
{
    // Deallocate all the buffers available in the cache
    BufferMapIter itr = _bufferMap.begin();
    for(; itr != _bufferMap.end(); ++itr)
    {
        delete itr->first;
    }
}