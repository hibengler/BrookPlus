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

#include <string.h>

#include "Buffer.h"
#include "BaseDevice.h"
#include "BufferMgr.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
//! \param rank Rank of the Buffer
//! \param dimensions dimension os the Buffer
//!
////////////////////////////////////////////////////////////////////////////////

Buffer::Buffer(unsigned short rank, unsigned int* dimensions, BaseDevice* device)
                : _refCount(0), _rank (rank), _device(device), _initialized(false)
{
    _dimensions = new unsigned int[rank];
    memcpy(_dimensions, dimensions, rank * sizeof(unsigned int));
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief overloaded operator == 
//! Check if format, rank and dimensions are same or not
//!
////////////////////////////////////////////////////////////////////////////////

bool
Buffer::operator==(const Buffer& other) const
{
    return _isEqual(other);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Check the eqality of two buffers
//!
////////////////////////////////////////////////////////////////////////////////

bool
Buffer::_isEqual(const Buffer& other) const
{
    // Check equality of rank and dimensions
    bool flag = _rank == other.getRank();

    if(flag)
    {
        unsigned int* dimensions = other.getDimensions();
        for(unsigned int i = 0; i < _rank; ++i)
        {
            flag = flag && (_dimensions[i] == dimensions[i]);
        }
    }

    return flag;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get buffer pitch. Default behavior to return width
//!
////////////////////////////////////////////////////////////////////////////////

unsigned int
Buffer::getPitch()
{
    return _dimensions[0];
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Increase the ref count
//!
////////////////////////////////////////////////////////////////////////////////
void
Buffer::ref()
{
    ++_refCount;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Decrease the ref count
//! Ask BufferMgr to destroy if refCount == 0
//!
////////////////////////////////////////////////////////////////////////////////

void
Buffer::unref()
{
    assert(_refCount > 0);

    if (--_refCount == 0)
    {
        // Ask bufferMgr to mark this buffer as destroyed
        _device->getBufferMgr()->destroyBuffer(this);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Is Buffer allocated properly
//!
////////////////////////////////////////////////////////////////////////////////

bool
Buffer::isInitialized()
{
    return _initialized;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

Buffer::~Buffer()
{
    delete[] _dimensions;
}
