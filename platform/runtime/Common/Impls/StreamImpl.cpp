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
#include "Runtime.h"
#include "Kernel.h"
#include "KernelDescData.h"
#include "StreamImpl.h"
#include "StreamOracle.h"
#include "Buffer.h"
#include "BufferMgr.h"
#include "ExecMgr.h"
#include "Utility.h"
#include "VectorDataType.h"
#include "CommonDefs.h"
#include "BaseDevice.h"
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
//! \param rank Rank of the Buffer
//! \param dimensions dimension os the Buffer
//! \param dataFormat Stream data fromat
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl::StreamImpl(unsigned short rank, unsigned int* dimensions, 
                       BRformat* dataFormat, unsigned int count, BaseDevice* device, 
                       Device* execDevices, unsigned int deviceCount)
                       : _refCount(0), _rank(rank), _bufferCount(count),
                       _device(device), _execDevices(execDevices), _execDeviceCount(deviceCount), 
                       _parent(NULL), _domainStartPoint(NULL),
                       _execDimension(NULL), _type(NULL),
                       _isModified(false), _error(BR_NO_ERROR), _errorCount(0)
{
    _dimensions = new unsigned int[rank];
    memcpy(_dimensions, dimensions, rank * sizeof(unsigned int));

    _format = new BRformat[_bufferCount];
    _buffer = new Buffer*[_bufferCount];
    _isInitialized = new bool[_bufferCount];

    memcpy(_format, dataFormat, _bufferCount * sizeof(BRformat));
    memset(_buffer, 0, _bufferCount * sizeof(Buffer*));

    for(unsigned int i = 0; i < _bufferCount; ++i)
    {
        _isInitialized[i] = false;
    }

    _errorString = new std::string("");
    _tempErrorString = new std::string("");
    _children = new std::map<StreamImpl*, bool>;

    _setMaxErrors();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor (overloaded). Used for domain stream
//!
//! \param parent if there is a parent for the stream
//! \param rank Rank of the Buffer
//! \param dimensions dimension os the Buffer
//! \param dataFormat Stream data fromat
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl::StreamImpl(StreamImpl* parent, unsigned int* start,
                       unsigned int* dimensions)
                       : _refCount(0), _rank(parent->getRank()), 
                       _bufferCount(parent->getBufferCount()), 
                       _device(parent->getDevice()), _parent(parent),
                        _execDimension(NULL), _type(NULL),
                        _isModified(false), _error(BR_NO_ERROR), _errorCount(0)
{
    _dimensions = new unsigned int[_rank];
    _domainStartPoint = new unsigned int[_rank];

    memcpy(_domainStartPoint, start, _rank * sizeof(unsigned int));
    memcpy(_dimensions, dimensions, _rank * sizeof(unsigned int));

    _format = new BRformat[_bufferCount];
    _buffer = new Buffer*[_bufferCount];
    _isInitialized = new bool[_bufferCount];

    for(unsigned int i = 0; i < _bufferCount; ++i)
    {
        _format[i] = parent->getDataFormat(i);
        _isInitialized[i] = false;
    }

    memset(_buffer, 0, _bufferCount * sizeof(Buffer*));

    _execDevices = parent->getExecDevices(&_execDeviceCount);
    _errorString = new std::string("");
    _tempErrorString = new std::string("");
    _children = new std::map<StreamImpl*, bool>;

    _setMaxErrors();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Default Constructor
//!
////////////////////////////////////////////////////////////////////////////////
StreamImpl::StreamImpl()
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Clone the stream
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl* 
StreamImpl::clone()
{
    StreamImpl* stream = new StreamImpl();
    memcpy(stream, this, sizeof(StreamImpl));

    for(unsigned int i = 0; i < _bufferCount; ++i)
    {
        stream->setBuffer(_buffer[i], i);
    }

    return stream;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.Read()
//!
//! \param ptr Application pointer to copy
//!
////////////////////////////////////////////////////////////////////////////////

void StreamImpl::read(const void* ptr, const char* flags)
{
    if(_buffer[0] && !_buffer[0]->isInitialized())
    {
        setErrorCode(BR_ERROR_READ);
        setErrorString("Stream Read : Internal buffer is not initialized\n");

        return;
    }

    _device->getBufferMgr()->setBufferData(this, ptr, flags);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.write()
//!
//! \param ptr Application pointer to copy
//!
////////////////////////////////////////////////////////////////////////////////
void
StreamImpl::write(void* ptr, const char* flags)
{
    if(_buffer[0] && !_buffer[0]->isInitialized())
    {
        setErrorCode(BR_ERROR_WRITE);
        setErrorString("Stream Write : Internal buffer is not initialized\n");

        return;
    }

    _device->getBufferMgr()->getBufferData(this, ptr, flags);
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set stream type - d3d9 d3d10
//! Create buffer and ask if this type is supported
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::setType(const char* type)
{
    if(_bufferCount > 1)
    {
        setErrorCode(BR_ERROR_DECLARATION);
        setErrorString("Stream Allocation : Struct streams "
                       "doesn't support this stream type\n");

        return;
    }

    Buffer* buffer = _device->getBufferMgr()->createTypedBuffer(type, this);

    if(buffer)
    {
        if(buffer->initialize())
        {
            _buffer[0] = buffer;
            _buffer[0]->ref();
            _type = type;

            return;
        }
        else
        {
            delete this;
        }
    }

    setErrorCode(BR_ERROR_DECLARATION);
    setErrorString("Stream Allocation : Stream of this type not supported on "
                   "the device\n");
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set stream property
//! required for various interoperability support
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::setProperty(const char* name, void* value)
{
    // Is it an interoperable stream
    if(_type && _buffer[0])
    {
        const char* error = _buffer[0]->setProperty(name, value);

        if(error)
        {
            std::string errorLog("Set Property : ");
            errorLog.append(error);

            setErrorCode(BR_ERROR_INVALID_PARAMATER);
            setErrorString(errorLog.c_str());
        }
        else if(_buffer[0]->isInitialized())
        {
            _isModified = true;
        }
    }
    else
    {
        setErrorCode(BR_ERROR_INVALID_PARAMATER);
        setErrorString("Set Property : This is not an interoperable stream\n");
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief copy data from source stream
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::copyDataFrom(StreamImpl* src)
{
    if(_buffer[0] && !_buffer[0]->isInitialized())
    {
        setErrorCode(BR_ERROR_ASSIGN);
        setErrorString("Stream Assign : Destination Internal buffer is not initialized\n");

        return;
    }

    if(src)
    {
        unsigned int i = 0;
        if(_rank == src->getRank())
        {
            unsigned int* srcDim = src->getDimensions();
            for(i = 0; i < _rank; ++i)
            {
                if(_dimensions[i] != srcDim[i])
                {
                    break;
                }
            }
        }

        if(i < _rank)
        {
            setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
            setErrorString("Stream Assign : Input stream rank and dimension \
                            doesn't match with destination\n");

            return;
        }

        i = 0;
        if(_bufferCount == src->getBufferCount())
        {
            for(i = 0; i < _bufferCount; ++i)
            {
                if(!(src->getBuffer(i)))
                {
                    break;
                }
            }
        }

        if(i < _bufferCount)
        {
            setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
            setErrorString("Stream Assign : Uninitialized input stream\n");

            return;
        }

        src->copyDataTo(this);
    }
    else
    {
        setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
        setErrorString("Stream Assign : Invalid input stream\n");
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief copy data to destination stream
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::copyDataTo(StreamImpl* dst)
{
    if(_buffer[0] && !_buffer[0]->isInitialized())
    {
        setErrorCode(BR_ERROR_ASSIGN);
        setErrorString("Stream ASSIGN : Source Internal buffer is not initialized\n");

        return;
    }

    _device->getBufferMgr()->copyData(dst, this);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.domain()
//!
//! \param start Pointer pointing to start of the dimensions
//! \param end Pointer pointing to end of the dimensions
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl*
StreamImpl::createDomainStream(unsigned int* start, unsigned int* end)
{
    if(_buffer[0] && !_buffer[0]->isInitialized())
    {
        setErrorCode(BR_ERROR_DOMAIN);
        setErrorString("Domain Operator : Internal buffer is not initialized\n");

        return NULL;
    }

    if(!start)
    {
        setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
        setErrorString("Domain Operator : Domain start point can't be NULL\n");

        return NULL;
    }

    if(!end)
    {
        setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
        setErrorString("Domain Operator : Domain end point can't be NULL\n");

        return NULL;
    }

    unsigned int dimensions[MaxRank];
    memset(dimensions, 0, sizeof(unsigned int)* MaxRank);

    /// Calculate the domain width and height.
    for(unsigned int i = 0; i < _rank; i++)
    {
        if(start[i] >= end[i])
        {
            setErrorCode(BR_ERROR_INVALID_PARAMATER);
            setErrorString("Domain Operator : Start point is more than end point, "
                           "failed to create domain stream \n");

            return NULL;
        }

        dimensions[i] = end[i] - start[i];

        if(dimensions[i] > _dimensions[i])
        {
            setErrorCode(BR_ERROR_INVALID_PARAMATER);
            setErrorString("Domain Operator : Domain dimensions are bigger than original stream, "
                           "failed to create domain stream \n");

            return NULL;
        }
    }


    // Create a new stream with domain dimensions
    StreamOracle* oracle = _device->getStreamOracle();
    StreamImpl* stream = oracle->createStreamImpl(this, start, dimensions);

    if(!stream)
    {
        setErrorCode(brook::BR_ERROR_DOMAIN);
        setErrorString("Domain Operator : Failed to create domain stream\n");
    }
    else
    {
        // Add this child into children list
        _children->operator[](stream) = false;
    }

    return stream;
}

Device*
StreamImpl::getExecDevices(unsigned int* count)
{
    *count = _execDeviceCount;
    return _execDevices;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.execDomain()
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl*
StreamImpl::execDomain(int numThreads)
{
    if(_buffer[0] && !_buffer[0]->isInitialized())
    {
        setErrorCode(BR_ERROR_DOMAIN);
        setErrorString("Exec Domain : Internal buffer is not initialized\n");

        return NULL;
    }

    if(numThreads <= 0)
    {
        setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
        setErrorString("Exec Domain : Number of threads must be more than 0\n");

        return NULL;
    }

    if(_rank != 1)
    {
        setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
        setErrorString("Exec Domain : This API is valid only for 1D stream\n");

        return NULL;
    }

    if(numThreads > (int)_dimensions[0])
    {
        setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
        setErrorString("Exec Domain : Number of threads specified more than stream dimension\n");

        return NULL;
    }

    StreamImpl* stream = clone();
    unsigned int* execDim = stream->getExecDimensions();

    execDim[0] = numThreads;

    for(unsigned short i = 1; i < MaxRank; ++i)
    {
        execDim[i] = 1;
    }

    return stream;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Return execution dimensions
//!
////////////////////////////////////////////////////////////////////////////////

unsigned int*
StreamImpl::getExecDimensions()
{
    if(!_execDimension)
    {
        unsigned int bufferRank = getBufferRank();
        _execDimension = new unsigned int[MaxRank];
        memcpy(_execDimension, getBufferDimensions(), bufferRank * sizeof(unsigned int));

        for(unsigned int i = bufferRank; i < MaxRank; ++i)
        {
            _execDimension[i] = 1;
        }
    }

    return _execDimension;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy data from srcBuffer to dstbuffer given the starting point of 
//! source and destinition buffer and dimension part that need to be copied
//! \param src Sorce stream
//! \param srcStartPoint The start point for the source stream 
//! \param dst Destination stream
//! \param dstStartPoint The start point for the destination stream
//! \param dimensions The domain width and height
//!
////////////////////////////////////////////////////////////////////////////////

bool
StreamImpl::copy(StreamImpl* src, unsigned int* srcStartPoint, StreamImpl* dst,
                 unsigned int* dstStartPoint, unsigned int* dimensions,
                 bool completeCopy, unsigned int srcBufferNum, unsigned int dstBufferNum)
{
    unsigned int bufferCount = src->getBufferCount();
    Buffer* srcbuffer = src->getBuffer(srcBufferNum);

    ///If source buffer NULL then skip.
    if(srcbuffer == NULL)
    {
        return false;
    }

    /// Select the copy shader Pass construct.
    BufferMgr* bufferMgr = _device->getBufferMgr();

    Pass& pass = bufferMgr->getCopyShaderDesc()->getData()->Techniques->
                 operator [](0).getData()->Passes->operator [](0);

    float startPoint[MaxRank];
    unsigned int destStart[MaxRank];
    unsigned int outputDomain[MaxRank];

    memset(startPoint, 0, sizeof(float)* MaxRank);
    memset(destStart, 0, sizeof(unsigned int)* MaxRank);

    unsigned int i = 0;
    for(; i < srcbuffer->getRank(); i++)
    {
        startPoint[i] = (float)srcStartPoint[i] - (float)dstStartPoint[i];
        destStart[i] = dstStartPoint[i];
        outputDomain[i] = dimensions[i];
    }
    for(; i < MaxRank; i++)
    {
        outputDomain[i] = 1;
    }

    float4 constParam = float4(startPoint[0], startPoint[1], startPoint[2], startPoint[3]); 
    ConstImpl constant(&constParam, BR_FORMAT_FLOAT_4, _device);

    /// Argument vectors are populated.
    ArgInfo args;
    args.constants.push_back(&constant);
    args.startDomain = destStart;
    args.domainDimension = outputDomain;

    for(unsigned int bufNum = 0; bufNum < bufferCount; ++bufNum)
    {
        Buffer* srcbuf;
        Buffer* dstBuf;

        if(completeCopy)
        {
            srcbuf = src->getBuffer(bufNum);
            dstBuf = dst->getBuffer(bufNum);
        }
        else
        {
            srcbuf = src->getBuffer(srcBufferNum);
            dstBuf = dst->getBuffer(dstBufferNum);
        }

        StreamImpl* tmpSrc = NULL;
        StreamImpl* tmpDst = NULL;

        if(bufferCount == 1)
        {
            tmpSrc = src;
        }
        else
        {
            BRformat srcFormat = src->getDataFormat(bufNum);
            tmpSrc = new StreamImpl(src->getRank(), src->getDimensions(), 
                                    &srcFormat, 1, _device);
            tmpSrc->setBuffer(srcbuf, 0);
        }

        if(dst->getBufferCount() == 1)
        {
            tmpDst = dst;
        }
        else
        {
            BRformat dstFormat = dst->getDataFormat(bufNum);

            tmpDst = new StreamImpl(dst->getRank(), dst->getDimensions(), 
                                    &dstFormat, 1, _device);

            if(dstBuf)
            {
                tmpDst->setBuffer(dstBuf, 0);
            }
        }

        args.inputs.push_back(tmpSrc);
        args.outputs.push_back(tmpDst);

        /// Execute the copy shader.
        bool success = _device->getExecMgr()
                                     ->executePass(pass, &args, false);

        args.inputs.pop_back();
        args.outputs.pop_back();

        if(bufferCount != 1)
        {
            delete tmpSrc;
        }

        if(dst->getBufferCount() != 1)
        {
            if(!dstBuf)
            {
                dst->setBuffer(tmpDst->getBuffer(0), bufNum);
            }

            delete tmpDst;
        }

        if(!success)
        {
            return false;
        }

        if(!completeCopy)
        {
            break;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Returning information wheather AT is required for this stream or not
//!
//! This method is used when we select the proper Technique.
//!
////////////////////////////////////////////////////////////////////////////////

bool
StreamImpl::isAddressTranslated() const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get the physical rank of the stream
//!
////////////////////////////////////////////////////////////////////////////////
unsigned short
StreamImpl::getBufferRank() const
{
    return _rank;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get Buffer Dimensions
//!
////////////////////////////////////////////////////////////////////////////////

unsigned int*
StreamImpl::getBufferDimensions() const
{
    return _dimensions;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get Buffer Pitch
//!
////////////////////////////////////////////////////////////////////////////////

unsigned int
StreamImpl::getBufferPitch()
{
    if(!_buffer[0])
    {
        BufferMgr* mgr = _device->getBufferMgr();
        Buffer* buf = mgr->getBuffer(this, 0);

        if(!buf)
        {
            return _dimensions[0];
        }

        setBuffer(buf, 0);
    }

    return _buffer[0]->getPitch();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get the buffer pointer
//!
//! If it is NULL because of lazy creation then ask BufferMgr to create one.
//!
////////////////////////////////////////////////////////////////////////////////

Buffer*
StreamImpl::getBuffer(unsigned int i) const
{
    return _buffer[i];
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy data from appPtr to streamPtr
//! Used for Stream.read implementation
//!
//! \param bufferWidth pitch of the buffer
//! \param startPoint starting point of the part of the stream that 
//!         is being copied in the buffer
//! \param dimensions dimensions of this part
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::setData(void* streamPtr, const void* appPtr, unsigned int bufferNum,
                    unsigned int bufferWidth, unsigned int* startPoint,
                    unsigned int* dimension) const
{
    Buffer* buffer = _buffer[bufferNum];
    if(buffer == NULL)
    {
        return;
    }

    unsigned int rank = buffer->getRank();
    unsigned int* bufferDimensions = buffer->getDimensions();
    unsigned int height = (rank == 1) ? 1 : dimension[1];
    unsigned int width = dimension[0];

    unsigned short toElementStride = buffer->getElementBytes();
    unsigned short fromElementStride = Utility::getElementBytes(_format[0]);

    for(unsigned int i = 1; i < _bufferCount; ++i)
    {
        fromElementStride += Utility::getElementBytes(_format[i]);
    }

    unsigned int startElementBytes = 0;
    for(unsigned int i = 0; i < bufferNum; ++i)
    {
        startElementBytes += Utility::getElementBytes(_format[i]);
    }

    unsigned int elementBytes = Utility::getElementBytes(_format[bufferNum]);

    unsigned int toRowStride = bufferWidth * toElementStride;
    unsigned int fromRowStride = bufferDimensions[0] * fromElementStride;

    unsigned int totalBytes = fromElementStride;
    for(unsigned int i = 0; i < _rank; ++i)
    {
        totalBytes *= _dimensions[i];
    }

    char* toPtr = (char*) streamPtr;
    char* fromPtr = (char*) appPtr + startPoint[1] * fromRowStride + 
                        startPoint[0] * fromElementStride + startElementBytes;

    char*& cpuPtr = fromPtr;
    unsigned int domainStride = width * fromElementStride;

    // Fast copy case when buffer pitch = stream width & when
    // number of components in an elements are not different for stream and buffer 
    if ((toRowStride == fromRowStride) && (toRowStride == domainStride)
            && (toElementStride == fromElementStride) && (_bufferCount == 1))
    {
        int numBytes = domainStride * height;
        int remainingBytes = totalBytes - (unsigned int)(cpuPtr - (char*)appPtr);

        // For AT buffer elements might be more than stream elements
        if(numBytes > remainingBytes)
        {
            numBytes = remainingBytes;
        }
        memcpy(toPtr, fromPtr, numBytes);
    }
    // pitch != width or tiled copy
    else if((toElementStride == fromElementStride) && (_bufferCount == 1))
    {
        for(unsigned int row = 0; row < height - 1; ++row)
        {
            memcpy(toPtr, fromPtr, domainStride);
            toPtr += toRowStride;
            fromPtr += fromRowStride;
        }

        int numBytes = domainStride;
        int remainingBytes = totalBytes - (unsigned int)(cpuPtr - (char*)appPtr);
        if(numBytes > remainingBytes)
        {
            numBytes = remainingBytes;
        }
        if(numBytes > 0)
        {
            memcpy(toPtr, fromPtr, numBytes);
        }
    }
    else
    {
        // Element by element copy for emulating format 3 datatypes
        for(unsigned int row = 0; row < height - 1; ++row)
        {
            for(unsigned int column = 0; column < width; ++column)
            {
                memcpy(toPtr, fromPtr, elementBytes);
                toPtr += toElementStride;
                fromPtr += fromElementStride;
            }
            fromPtr += fromRowStride - width * fromElementStride;
            toPtr += toRowStride - width * toElementStride;
        }

        if(totalBytes > (unsigned int)(cpuPtr - (char*)appPtr))
        {
            int numElement = width;
            int remainingElement = (totalBytes - (unsigned int)(cpuPtr - (char*)appPtr)
                                    + startElementBytes) / fromElementStride;
            if(numElement > remainingElement)
            {
                numElement = remainingElement;
            }

            for(int column = 0; column < numElement; ++column)
            {
                memcpy(toPtr, fromPtr, elementBytes);
                toPtr += toElementStride;
                fromPtr += fromElementStride;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy data from appPtr to streamPtr
//! Used for Stream.read implementation
//!
//! \param bufferWidth pitch of the buffer
//! \param startPoint starting point of the part of the stream that 
//!         is being copied from the buffer
//! \param dimensions dimensions of this part
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::getData(void* streamPtr, void* appPtr, unsigned int bufferNum,
                    unsigned int bufferWidth, unsigned int* startPoint,
                    unsigned int* dimension) const
{
    Buffer* buffer = _buffer[bufferNum];
    if(buffer == NULL)
    {
        return;
    }

    unsigned int rank = buffer->getRank();
    unsigned int* bufferDimensions = buffer->getDimensions();
    unsigned int height = (rank == 1) ? 1 : dimension[1];
    unsigned int width = dimension[0];

    unsigned short fromElementStride = buffer->getElementBytes();
    unsigned short toElementStride = Utility::getElementBytes(_format[0]);

    for(unsigned int i = 1; i < _bufferCount; ++i)
    {
        toElementStride += Utility::getElementBytes(_format[i]);
    }

    unsigned int startElementBytes = 0;
    for(unsigned int i = 0; i < bufferNum; ++i)
    {
        startElementBytes += Utility::getElementBytes(_format[i]);
    }

    unsigned int elementBytes = Utility::getElementBytes(_format[bufferNum]);

    unsigned int fromRowStride = bufferWidth * fromElementStride;
    unsigned int toRowStride = bufferDimensions[0] * toElementStride;

    unsigned int totalBytes = toElementStride;
    for(unsigned int i = 0; i < _rank; ++i)
    {
        totalBytes *= _dimensions[i];
    }

    char* fromPtr = (char*) streamPtr;
    char* toPtr = (char*) appPtr + startPoint[1] * toRowStride + 
                        startPoint[0] * toElementStride + startElementBytes;
    char*& cpuPtr = toPtr;

    unsigned int domainStride = width * fromElementStride;

    // Fast copy case when buffer pitch = stream width & when
    // number of components in an elements are not different for stream and buffer 
    if ((toRowStride == fromRowStride) && (toRowStride == domainStride)
            && (toElementStride == fromElementStride) && (_bufferCount == 1))
    {
        int numBytes = domainStride * height;
        int remainingBytes = totalBytes - (unsigned int)(cpuPtr - (char*)appPtr);

        // For AT buffer elements might be more than stream elements
        if(numBytes > remainingBytes)
        {
            numBytes = remainingBytes;
        }
        memcpy(toPtr, fromPtr, numBytes);
    }
    // pitch != width or tiled copy
    else if((toElementStride == fromElementStride) && (_bufferCount == 1))
    {
        for(unsigned int row = 0; row < height - 1; ++row)
        {
            memcpy(toPtr, fromPtr, domainStride);
            toPtr += toRowStride;
            fromPtr += fromRowStride;
        }

        int numBytes = domainStride;
        int remainingBytes = totalBytes - (unsigned int)(cpuPtr - (char*)appPtr);
        if(numBytes > remainingBytes)
        {
            numBytes = remainingBytes;
        }
        if(numBytes > 0)
        {
            memcpy(toPtr, fromPtr, numBytes);
        }
    }
    else
    {
        // Element by element copy for emulating format 3 datatypes
        for(unsigned int row = 0; row < height - 1; ++row)
        {
            for(unsigned int column = 0; column < width; ++column)
            {
                memcpy(toPtr, fromPtr, elementBytes);
                toPtr += toElementStride;
                fromPtr += fromElementStride;
            }
            fromPtr += fromRowStride - width * fromElementStride;
            toPtr += toRowStride - width * toElementStride;
        }

        if(totalBytes > (unsigned int)(cpuPtr - (char*)appPtr))
        {
            int numElement = width;
            int remainingElement = (totalBytes - (unsigned int)(cpuPtr - (char*)appPtr)
                                    + startElementBytes) / toElementStride;
            if(numElement > remainingElement)
            {
                numElement = remainingElement;
            }

            for(int column = 0; column < numElement; ++column)
            {
                memcpy(toPtr, fromPtr, elementBytes);
                toPtr += toElementStride;
                fromPtr += fromElementStride;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief set the buffer value
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::setBuffer(Buffer* value, unsigned int i)
{
    _buffer[i] = value;

    if(_buffer[i])
    {
        _buffer[i]->ref();
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Are aynchronous operations done
//!
////////////////////////////////////////////////////////////////////////////////

bool
StreamImpl::isSync()
{
    for(unsigned int i = 0; i < _bufferCount; ++i)
    {
        Buffer* buffer = getBuffer(i);
        if(buffer)
        {
            if(!buffer->isSync())
            {
                return false;
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Flush the command queue
//!
////////////////////////////////////////////////////////////////////////////////

bool
StreamImpl::flush()
{
    for(unsigned int i = 0; i < _bufferCount; ++i)
    {
        Buffer* buffer = getBuffer(i);
        if(!buffer)
        {
            BufferMgr* mgr = _device->getBufferMgr();
            Buffer* buf = mgr->getBuffer(this, i);

            if(!buf)
            {
                return false;
            }

            setBuffer(buf, i);
        }
        else
        {
            return buffer->flush();
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Mark this child that it is going to be used in the same kernel
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::NotifyModification(StreamImpl* child)
{
    _children->operator[](child) = true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Once the kernel execution is over, clear child
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::ClearChild(StreamImpl* child)
{
    _children->operator[](child) = false;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy data from parent in case stream is not modified yet
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::copyDataFromTree()
{
    if(!_isModified && _parent)
    {
        _parent->copyDataFromTree();

        unsigned int* dstStartPoint = new unsigned int[_rank];
        memset(dstStartPoint, 0, _rank * sizeof(unsigned int));

        // Invoke copy to run the copy shader.
        bool success = _parent->copy(_parent, _domainStartPoint, this, 
                                     dstStartPoint, _dimensions);

        delete[] dstStartPoint;
        if(!success)
        {
            setErrorCode(BR_ERROR_DOMAIN, true);
            setErrorString("Domain Operator : Failed to copy data from parent stream to domain stream");
        }

        _isModified = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Propogate the changes done in this stream to all the associated streams
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::copyDataToTree(bool syncParent, StreamImpl* childNotToSync)
{
    if(_parent && syncParent)
    {
        unsigned int* srcStartPoint = new unsigned int[_rank];

        memset(srcStartPoint, 0, sizeof(unsigned int)* _rank);
        bool success = _parent->copy(this, srcStartPoint, _parent,
                                     _domainStartPoint, _dimensions);

        delete[] srcStartPoint;

        if(!success)
        {
            _parent->setErrorCode(BR_ERROR_DOMAIN);
            _parent->setErrorString("Domain Operator : Failed to copy data from " 
                                   "domain stream to parent stream");
        }
        else
        {
            // Now ask parent to sync its tree, but not the same child again
            _parent->copyDataToTree(true, this);
        }
    }

    std::map<StreamImpl*, bool>::iterator itr;
    for(itr = _children->begin(); itr != _children->end(); ++itr)
    {
        if(itr->first != childNotToSync && itr->second != true)
        {
            unsigned int* dstStartPoint = new unsigned int[_rank];
            memset(dstStartPoint, 0, _rank * sizeof(unsigned int));

            // Invoke ::copy to run the copy shader.
            bool success = copy(this, itr->first->getDomainStartPoint(), itr->first, 
                                dstStartPoint, itr->first->getDimensions());

            delete[] dstStartPoint;

            if(!success)
            {
                itr->first->setErrorCode(BR_ERROR_DOMAIN, true);
                itr->first->setErrorString("Domain Operator : Failed to copy data from parent "
                                       "stream to domain stream");
            }
            else
            {
                // Ask child to sync its tree, but not the parent
                itr->first->copyDataToTree(false, NULL);
            }
        }
    }

    _isModified = true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Remove the associated parent
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::removeParent()
{
    _parent = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Remove the given child from children list
//!
////////////////////////////////////////////////////////////////////////////////
void
StreamImpl::removeChild(StreamImpl* child)
{
    _children->erase(child);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Increase the ref count
//!
////////////////////////////////////////////////////////////////////////////////
void
StreamImpl::ref()
{
    ++_refCount;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Decrease the ref count
//! destroy if refCount == 0
//!
////////////////////////////////////////////////////////////////////////////////
void
StreamImpl::unref()
{
    assert(_refCount > 0);

    if (--_refCount == 0)
    {
        if(_parent)
        {
            _parent->removeChild(this);
        }

        std::map<StreamImpl*, bool>::iterator itr;
        for(itr = _children->begin(); itr != _children->end(); ++itr)
        {
            itr->first->removeParent();
        }

        delete this;
    }
}

void
StreamImpl::_setMaxErrors()
{
    char* maxErrorCount;
    if(maxErrorCount = getenv("BRT_ERROR_LOG_SIZE"))
    {
        _maxErrors = atoi(maxErrorCount);
    }
    else
    {
        _maxErrors = 5;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get the error code of this stream
//! Flush all the asynchronous events of this stream and clear the previous error
//!
////////////////////////////////////////////////////////////////////////////////

brook::BRerror
StreamImpl::getErrorCode()
{
    // No need to flush previous commands if 
    // stream is already having an error
    if(!_error)
    {
        flush();
    }

    // Retun error and set the error to null
    brook::BRerror error = _error;
    _error = brook::BR_NO_ERROR;

    return error;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set error on this stream
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImpl::setErrorCode(brook::BRerror error, bool force)
{
    // Set only if it is first error
    // or it was forced to set this error
    if(!_error || force)
    {
        _error = error;

        if(force)
        {
            _errorString->clear();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Return the error log and clear error log
//!
////////////////////////////////////////////////////////////////////////////////

const char*
StreamImpl::getErrorString()
{
    // Assign the temporay string, so that user need not
    // changing actual error string doesn't effect error log
    _tempErrorString->clear();
    _tempErrorString->assign(*_errorString);

    const char* value = _tempErrorString->c_str();

    // Clear error log
    _errorString->clear();
    _errorCount = 0;

    return value;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Append the log. It shows only first five error
//!
////////////////////////////////////////////////////////////////////////////////

void 
StreamImpl::setErrorString(const char* error)
{
    if(_errorCount++ < _maxErrors)
    {
        _errorString->append(error);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl::~StreamImpl()
{
    delete[] _dimensions;
    delete[] _format;

    if(_domainStartPoint)
    {
        delete[] _domainStartPoint;
    }
    if(_execDimension)
    {
        delete[] _execDimension;
    }

    for(unsigned int i =0; i < _bufferCount; ++i)
    {
        if(_buffer[i] != NULL)
        {
            _buffer[i]->unref();
        }
    }

    delete[] _isInitialized;
    delete[] _buffer;
    delete _errorString;
    delete _tempErrorString;
    delete _children;
}
