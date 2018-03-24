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
#include "CALStreamOracle.h"
#include "CALDevice.h"
#include "StreamImplAT.h"
#include "BufferMgr.h"
#include "Runtime.h"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

CALStreamOracle::CALStreamOracle(BaseDevice* device) : StreamOracle(device)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create StreamImpl.
//! Contains logic for creating AT or Simple Stream.
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl*
CALStreamOracle::createStreamImpl(unsigned short rank, unsigned int* dimensions, 
                               BRformat* format, unsigned int count,
                               Device* devices, unsigned int deviceCount)
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdeviceinfo info = device->getInfo();
    unsigned int maxWidth = info.maxResource1DWidth;

    if(rank == 2 || (rank ==1 && dimensions[0] <= maxWidth))
    {
        return StreamOracle::createStreamImpl(rank, dimensions, format, count,
                                              devices, deviceCount);
    }
    else // AT case
    {
        // Get total number of elements
        unsigned int numElements = 1;
        for(unsigned int i = 0; i < rank; ++i)
        {
            numElements *= dimensions[i];
        }

        //Some optmizations point - 
        //1. Pitch is aligned with width. We can use the fastest memcopy path.
        //2. Number of extra elements are minimum. (streamElement < bufferElement)
        float width = sqrt((float)numElements);
        unsigned int expectedWidth = (unsigned int)(floor(width));
        expectedWidth =  Utility::floorPow(expectedWidth);

        // Do we really need to create 2D buffer
        // Possible for a small 3D stream
        if(expectedWidth < 64)
        {
            expectedWidth = numElements;
        }

        unsigned int expectedHeight = (unsigned int)(ceilf(numElements / 
                                                   (float)expectedWidth));

        if(expectedHeight > info.maxResource2DHeight)
        {
            expectedWidth = info.maxResource2DWidth;

            expectedHeight = (unsigned int)(ceilf(numElements / 
                             (float)expectedWidth));
        }

        unsigned int bufferRank = 2;
        unsigned int bufferDimension[2];
        bufferDimension[0] = expectedWidth;
        bufferDimension[1] = expectedHeight;

        return new StreamImplAT(rank, bufferRank, dimensions, bufferDimension, 
                                format, count, _device, devices, deviceCount);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create StreamImpl, overloaded for domain stream.
//! Contains logic for creating AT or Simple Stream.
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl*
CALStreamOracle::createStreamImpl(StreamImpl* parent, unsigned int* start,
                               unsigned int* dimensions)
{
    unsigned int rank = parent->getRank();
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdeviceinfo info = device->getInfo();
    unsigned int maxWidth = info.maxResource1DWidth;
    StreamImpl* stream = NULL;

    if(rank == 2 || (rank ==1 && dimensions[0] <= maxWidth))
    {
        stream = StreamOracle::createStreamImpl(parent, start, dimensions);
    }
    else
    {
        unsigned int numElements = 1;
        for(unsigned int i = 0; i < rank; ++i)
        {
            numElements *= dimensions[i];
        }

        float width = sqrt((float)numElements);
        unsigned int expectedWidth = (unsigned int)(floor(width));
        expectedWidth =  Utility::floorPow(expectedWidth);

        if(expectedWidth < 64)
        {
            expectedWidth = numElements;
        }

        unsigned int expectedHeight = (unsigned int)(ceilf(numElements /
                                                (float)expectedWidth));

        if(expectedHeight > info.maxResource2DHeight)
        {
            expectedWidth = info.maxResource2DWidth;

            expectedHeight = (unsigned int)(ceilf(numElements / 
                             (float)expectedWidth));
        }

        unsigned int bufferRank = 2;
        unsigned int bufferDimension[2];
        bufferDimension[0] = expectedWidth;
        bufferDimension[1] = expectedHeight;

        stream = new StreamImplAT(parent, start, bufferRank,
                                  dimensions, bufferDimension);
    }

    return stream;
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALStreamOracle::~CALStreamOracle()
{
}

