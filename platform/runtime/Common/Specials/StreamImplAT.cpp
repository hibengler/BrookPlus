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


#include "StreamImplAT.h"
#include "BaseDevice.h"
#include "ConstImpl.h"
#include "BufferMgr.h"
#include "KernelImpl.h"
#include "VectorDataType.h"
#include "CommonDefs.h"
#include "Utility.h"
#include "Buffer.h"
#include <string>

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//! It needs information of both logical and physical representation
//!
////////////////////////////////////////////////////////////////////////////////

StreamImplAT::StreamImplAT(unsigned short logRank, unsigned short bufferRank, 
                           unsigned int*logDimensions,unsigned int*bufferDimensions,
                           BRformat* dataFormat, unsigned int count, BaseDevice* device,
                           Device* execDevices, unsigned int deviceCount)
                        : StreamImpl(logRank, logDimensions, dataFormat, count, device, 
                                     execDevices, deviceCount),
                          _bufferRank(bufferRank)
{
    _bufferDimensions = new unsigned int[bufferRank];

    memcpy(_bufferDimensions, bufferDimensions, bufferRank * sizeof(unsigned int));
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor (overloaded). Used for domain stream
//!
////////////////////////////////////////////////////////////////////////////////

StreamImplAT::StreamImplAT(StreamImpl* parent, unsigned int* start, unsigned short bufferRank,
                           unsigned int* logDimensions, unsigned int* bufferDimensions) 
                           : StreamImpl(parent, start, logDimensions), _bufferRank(bufferRank)
{
    _bufferDimensions = new unsigned int[bufferRank];

    memcpy(_bufferDimensions, bufferDimensions, bufferRank * sizeof(unsigned int));
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Default Constructor
//!
////////////////////////////////////////////////////////////////////////////////
StreamImplAT::StreamImplAT()
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Clone the stream
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl* 
StreamImplAT::clone()
{
    StreamImpl* stream = new StreamImplAT();
    memcpy(stream, this, sizeof(StreamImplAT));

    for(unsigned int i = 0; i < _bufferCount; ++i)
    {
        stream->setBuffer(_buffer[i], i);
    }

    return stream;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set stream type - d3d9 d3d10
//! Address translated stream doesn't work with any interoperable type
//!
////////////////////////////////////////////////////////////////////////////////

void
StreamImplAT::setType(const char* type)
{
    setErrorCode(BR_ERROR_DECLARATION);
    setErrorString("Stream Allocation : Rank and dimension of stream "
                   "doesn't support this stream type\n");
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy data from dstbuffer to srcBuffer given the starting point of 
//! source and destinition buffer and dimension part that need to be copied
//! \param src Sorce stream
//! \param srcStartPoint The start point for the source stream 
//! \param dst Destination stream
//! \param dstStartPoint The start point for the destination stream
//! \param dimensions The domain width and height
//!
////////////////////////////////////////////////////////////////////////////////

bool
StreamImplAT::copy(StreamImpl* src, unsigned int* srcStartPoint, StreamImpl* dst,
                  unsigned int* dstStartPoint, unsigned int* dimensions,
                  bool completeCopy, unsigned int srcBufferNum, unsigned int dstBufferNum)
{
    unsigned int bufferCount = src->getBufferCount();

    ///If source buffer NULL then skip.
    if(src->getBuffer(srcBufferNum) == NULL)
    {
        return false;
    }

    // Select the copy shader Pass construct.
    BufferMgr* bufferMgr = _device->getBufferMgr();

    // Select the techniques[1] for domainAT copy shader.
    Pass& pass = bufferMgr->getCopyShaderDesc()->getData()->
                Techniques->operator [](1).getData()->Passes->operator [](0);

    unsigned int oneDDimension = 1;
    unsigned int srcStart[MaxRank];
    unsigned int dstStart[MaxRank];
    unsigned int dstEnd[MaxRank];
    unsigned int domainStartPoint[MaxRank];
    unsigned int outputDomain[MaxRank];
    unsigned int copyOfDimension[MaxRank];
    unsigned int srcStreamDimension[MaxRank];
    unsigned int dstStreamDimension[MaxRank];
    unsigned int srcBufferDimension[MaxRank];
    unsigned int dstBufferDimension[MaxRank];

    // Constant parameters for the copy shader.
    int4 gatherStreamShape_const;
    int4 gatherBufferShape_const;
    int4 srcStartPoint_const;
    int4 dstStartPoint_const;
    int4 dstEndPoint_const;
    int4 outStreamShape_const;
    int4 outBufferShape_const;

    // Initialize the memory pointers.
    memset(srcStart, 0, sizeof(unsigned int)* MaxRank);
    memset(dstStart, 0, sizeof(unsigned int)* MaxRank);
    memset(dstEnd, 0, sizeof(unsigned int)* MaxRank);
    memset(domainStartPoint, 0, sizeof(unsigned int)* MaxRank);
    memset(outputDomain, 0, sizeof(unsigned int)* MaxRank);
    memset(copyOfDimension, 0, sizeof(unsigned int)* MaxRank);

    // Calculate the shader specific constants.
    unsigned int i = 0;
    for(i = 0; i < src->getRank(); i++)
    {
        srcStreamDimension[i] = src->getDimensions()[i];
        dstStreamDimension[i] = dst->getDimensions()[i];
        srcStart[i] = srcStartPoint[i];
        dstStart[i] = dstStartPoint[i];
        dstEnd[i] = dstStartPoint[i] + dimensions[i] - 1;
        copyOfDimension[i] = dimensions[i];
        oneDDimension = oneDDimension * dimensions[i]; 
    }
    for(; i < MaxRank; ++i)
    {
        srcStreamDimension[i] = 1;
        dstStreamDimension[i] = 1;
    }

    for(i = 0; i < src->getBufferRank(); ++i)
    {
        srcBufferDimension[i] = src->getBufferDimensions()[i];
    }
    for(; i < MaxRank; ++i)
    {
        srcBufferDimension[i] = 1;
    }

    for(i = 0; i < dst->getBufferRank(); ++i)
    {
        dstBufferDimension[i] = dst->getBufferDimensions()[i];
    }
    for(; i < MaxRank; ++i)
    {
        dstBufferDimension[i] = 1;
    }

    // The output domain specific constants are calculated.
    unsigned int oneDStartPoint = dstStart[2] * dstStreamDimension[1] * dstStreamDimension[0] +
                                  dstStart[1] * dstStreamDimension[0] + dstStart[0];
    unsigned int oneDEndPoint = dstEnd[2] * dstStreamDimension[1] * dstStreamDimension[0] +
                                dstEnd[1] * dstStreamDimension[0] + dstEnd[0];
    unsigned int buffStartHeight = oneDStartPoint / dstBufferDimension[0];
    unsigned int buffStartWidth = oneDStartPoint - buffStartHeight * dstBufferDimension[0];
    unsigned int buffEndHeight = oneDEndPoint / dstBufferDimension[0];
    unsigned int buffEndWidth = oneDEndPoint - buffEndHeight * dstBufferDimension[0];

    domainStartPoint[1] = buffStartHeight;
    outputDomain[1] = buffEndHeight - buffStartHeight + 1;

    // If both the points are in the same row, we can create rectangular domain
    if(outputDomain[1] == 1)
    {
        domainStartPoint[0] = buffStartWidth;
        outputDomain[0] = buffEndWidth - buffStartWidth + 1;
    }
    else
    {
        // otherwise start from the 0th point of the starting row 
        // to the end point of the last row
        domainStartPoint[0] = 0;
        outputDomain[0] = dstBufferDimension[0];
    }

    // Convert the shader specific constants to shader accepted format.
    gatherStreamShape_const = int4(srcStreamDimension[0], srcStreamDimension[1], \
                                   srcStreamDimension[2], srcStreamDimension[3]);
    
    gatherBufferShape_const = int4(srcBufferDimension[0], srcBufferDimension[1], \
                                   srcBufferDimension[2], srcBufferDimension[3]);
    
    outStreamShape_const = int4(dstStreamDimension[0], dstStreamDimension[1], \
                                   dstStreamDimension[2], dstStreamDimension[3]);

    outBufferShape_const = int4(dstBufferDimension[0], dstBufferDimension[1], \
                                   dstBufferDimension[2], dstBufferDimension[3]);

    srcStartPoint_const = int4(srcStart[0], srcStart[1], srcStart[2], srcStart[3]);
    dstStartPoint_const = int4(dstStart[0], dstStart[1], dstStart[2], dstStart[3]);
    dstEndPoint_const = int4(dstEnd[0], dstEnd[1], dstEnd[2], dstEnd[3]);

    // Create constats.
    ConstImpl constant1(&gatherStreamShape_const, BR_FORMAT_INT_4, _device);
    ConstImpl constant2(&gatherBufferShape_const, BR_FORMAT_INT_4, _device);
    ConstImpl constant3(&srcStartPoint_const, BR_FORMAT_INT_4, _device);
    ConstImpl constant4(&dstStartPoint_const, BR_FORMAT_INT_4, _device);
    ConstImpl constant5(&dstEndPoint_const, BR_FORMAT_INT_4, _device);
    ConstImpl constant6(&outStreamShape_const, BR_FORMAT_INT_4, _device);
    ConstImpl constant7(&outBufferShape_const, BR_FORMAT_INT_4, _device);

    // Argument vectors are populated.
    ArgInfo args;
    args.constants.push_back(&constant1);
    args.constants.push_back(&constant2);
    args.constants.push_back(&constant3);
    args.constants.push_back(&constant4);
    args.constants.push_back(&constant5);
    args.constants.push_back(&constant6);
    args.constants.push_back(&constant7);

    args.startDomain = domainStartPoint;
    args.domainDimension = outputDomain;

   for(unsigned int bufNum = 0; bufNum < bufferCount; ++bufNum)
    {
        Buffer* srcBuf = src->getBuffer(bufNum);
        Buffer* dstBuf = dst->getBuffer(bufNum);

        StreamImpl* tmpSrc = NULL;
        StreamImpl* tmpDst = NULL;

        if(bufferCount == 1)
        {
            tmpSrc = src;
            tmpDst = dst;
        }
        else
        {
            BRformat srcFormat = src->getDataFormat(bufNum);
            tmpSrc = new StreamImpl(src->getRank(), src->getDimensions(), 
                                    &srcFormat, 1, _device);
            tmpSrc->setBuffer(srcBuf, 0);

            tmpDst = new StreamImpl(dst->getRank(), dst->getDimensions(), 
                                    &srcFormat, 1, _device);

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
            if(!dstBuf)
            {
                dst->setBuffer(tmpDst->getBuffer(0), bufNum);
            }

            delete tmpSrc;
            delete tmpDst;
        }

        if(!success)
        {
            return false;
        }
    }

    return true;
}

/////////////dstStreamDimension///////////////////////////////////////////////////////////////////
//!          srcBufferDimension
//! \brief RedstBufferDimensionturning information wheather AT is required for this stream or not
//!
//! This method is used when we select the proper Technique.
//!
////////////////////////////////////////////////////////////////////////////////

bool
StreamImplAT::isAddressTranslated() const
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get the physical rank of the stream
//!
////////////////////////////////////////////////////////////////////////////////

unsigned short
StreamImplAT::getBufferRank() const
{
    return _bufferRank;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get Buffer Dimensions
//!
////////////////////////////////////////////////////////////////////////////////

unsigned int*
StreamImplAT::getBufferDimensions() const
{
    return _bufferDimensions;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

StreamImplAT::~StreamImplAT()
{
    delete[] _bufferDimensions;
}
