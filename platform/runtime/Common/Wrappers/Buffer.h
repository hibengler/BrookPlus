#ifndef _BUFFER_H_
#define _BUFFER_H_

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

////////////////////////////////////////////////////////////////////////////////
//!
//! \file Buffer.h
//!
//! \brief Contains the declaration of Buffer class
//!
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "ExportDef.h"

class BaseDevice;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class Buffer
//!
//! \brief Abstarct Base class for all Backend specific Buffers.
//! Provides very low level functionalities required for Buffer creation 
//! and data transfer.
//! 
//! Construction - The object is created based on lazy evaluation.
//! Buffer is created during StreamRead and kernel invocation (only for output) .
//! 
//! Destruction - BufferMgr contains the cache of Buffers and associate a flag 
//! to each buffer mentioning whether it is in use or not. 
//! Unused buffers are deleted if application need a different sized buffer
//! and buffer allocation fails. All the buffers are deleted when application exits.
//!
//! Error conditions - It can generate errors during creation and data transfer.
//! Its BufferMgr's responsibility to handle these error conditions.
//!
////////////////////////////////////////////////////////////////////////////////

class EXPORT Buffer
{
    public:

        Buffer(unsigned short rank, unsigned int* dimensions, BaseDevice* device);
        virtual bool initialize() = 0;
        virtual bool flush() = 0;
        bool operator==(const Buffer& other) const;

        inline const unsigned short getRank() const;
        inline unsigned int* getDimensions() const;
        virtual unsigned int getPitch();
        virtual unsigned short getElementBytes() const = 0;
        virtual const char* setProperty(const char* name, void* value) = 0;
        virtual bool isSync() = 0;

        bool isInitialized();
        void ref();
        virtual void unref();

        virtual ~Buffer() = 0;

    protected:

        virtual bool _isEqual(const Buffer& other) const;

    protected:

        //! \brief number of refernce to this buffer
        unsigned int _refCount;

        //! \brief Rank of Buffer
        unsigned short _rank;

        //! \brief Dimensions of Buffer
        unsigned int* _dimensions;

        //! \brief Device information as each GPU local memory is specific to a device
        BaseDevice* _device;

        //! \brief Is buffer initilized
        bool _initialized;
};

inline const
unsigned short
Buffer::getRank() const
{
    return _rank;
}

inline
unsigned int*
Buffer::getDimensions() const
{
    return _dimensions;
}

#endif //_BUFFER_H_
