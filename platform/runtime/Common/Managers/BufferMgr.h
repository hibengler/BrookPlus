#ifndef _BUFFERMGR_H_
#define _BUFFERMGR_H_

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
//! \file BufferMgr.h
//!
//! \brief Contains the declaration of BufferMgr class
//!
////////////////////////////////////////////////////////////////////////////////

#include<vector>
#include <map>
#include "KernelDesc.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)

//The warning disables visual studio's
//annoying habit of warning when using the standard set lib
#pragma warning (disable : 4251)

#else
#define EXPORT
#endif

class BaseDevice;
class Buffer;
class StreamImpl;

typedef std::map<Buffer*, bool> BufferMap;
typedef BufferMap::iterator BufferMapIter;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class BufferMgr
//!
//! \brief Abstaract Base class for all Backend specific BufferMgr.
//! It provides different base level and Backend specific optimizations.
//! 
//! Construction - The object is created when runtime is initialized .
//! 
//! Destruction -  Deleted when application exits.
//! 
//! Interaction - This class provide interface for all the high level
//! functionalities of Buffer. Contains factory method for Buffer. 
//! 
//! Error conditions - It tries to handle different errors came from Buffer class and 
//! if BufferMgr is not able to handle that, it reports it to User.
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT BufferMgr
{
    public:

        explicit BufferMgr(BaseDevice* device);
        virtual Buffer* getBuffer(StreamImpl* stream, unsigned int i);
        virtual Buffer* createTypedBuffer(const char* type, StreamImpl* stream);

        virtual void setBufferData(StreamImpl* stream, const void* ptr, const char* flags);
        virtual void getBufferData(StreamImpl* stream, void* ptr, const char* flags);
        virtual void copyData(StreamImpl* dst, StreamImpl* src);
        virtual void copyBufferData(Buffer* dst, Buffer* src);
        virtual const brook::KernelDesc* getCopyShaderDesc();
        virtual const brook::KernelDesc* getResizeShaderDesc();

        void destroyBuffer(Buffer* buffer);
        virtual ~BufferMgr();

        enum
        {
            TileSize = 256
        };

    protected:

        virtual Buffer* _createBuffer(StreamImpl* stream, unsigned int i);

    protected:

        //! \brief A cache that keeps Buffer and their status.
        //! It is possible that a Stream exists but Buffer doesn't (Lazy creation of Buffer).
        //! It is also possible a Stream is deleted but a Buffer exists. 
        //! (Memory Management - avoiding allocation/deallocation of resources)
        BufferMap _bufferMap;

        //! \brief Device for which this BufferMgr works
        BaseDevice* _device;

};

#endif //_BUFFERMGR_H_
