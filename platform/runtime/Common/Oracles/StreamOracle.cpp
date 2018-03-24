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
#include "StreamOracle.h"
#include "BufferMgr.h"
#include "Runtime.h"


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create StreamImpl.
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl*
StreamOracle::createStreamImpl(unsigned short rank, unsigned int* dimensions, 
                               BRformat* format, unsigned int count,
                               Device* devices, unsigned int deviceCount)
{
    return new StreamImpl(rank, dimensions, format, count, _device, devices, deviceCount);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create StreamImpl, overloaded for domain stream.
//! Constains logic for data transfer from parent to domain stream
//! Contains logic for creating AT or Simple Stream.
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl*
StreamOracle::createStreamImpl(StreamImpl* parent, unsigned int* start,
                               unsigned int* dimensions)
{
    return new StreamImpl(parent, start, dimensions);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

StreamOracle::StreamOracle(BaseDevice* device) : _device(device)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

StreamOracle::~StreamOracle()
{
}

