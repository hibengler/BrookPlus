#ifndef _BASE_DEVICE_H_
#define _BASE_DEVICE_H_

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
//! \file BaseDevice.h
//!
//! \brief Contains the declaration of BaseDevice class
//!
////////////////////////////////////////////////////////////////////////////////

#include<string>
#include <cstdio>

#include "Formats.h"
#include "Device.h"
#include "ExportDef.h"

class StreamImpl;
class ExecMgr;
class BufferMgr;
class KernelOracle;
class StreamOracle;


////////////////////////////////////////////////////////////////////////////////
//!
//! \class BaseDevice
//!
//! \brief Base class for all backend specific devices.
//! Provides an abstraction to cpu thread.
//! 
//! Construction - Constructed when runtime is initialized.
//! 
//! Destruction -  Deleted when application exits.
//!
//! Every class contains a BaseDevice member.
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT BaseDevice
{
    public:

        explicit BaseDevice(unsigned short id);
        virtual bool initialize() = 0;
        virtual const char* getType() const = 0;
        virtual const char* getSupportedInteroperability() const = 0;
        virtual StreamImpl* createStreamImpl(unsigned short rank, unsigned int* dimensions,
                                             brook::BRformat* format, unsigned int count,
                                             brook::Device* device, unsigned int deviceCount);
        virtual ~BaseDevice();

        
        inline KernelOracle* getKernelOracle() const;
        inline StreamOracle* getStreamOracle() const;
        inline BufferMgr* getBufferMgr() const;
        inline ExecMgr* getExecMgr() const;

    protected:

        //! \brief User ID of the device
        unsigned short _id;

        //! \brief one BufferMgr per device
        BufferMgr* _bufferMgr;

        //! \brief one ExecMgr per device
        ExecMgr* _execMgr;

        //! \brief Acces to StreamOracle is through Runtime
        //! There is one StreamOracle per device
        StreamOracle* _streamOracle;

        //! \brief Acces to KernelOracle is through Runtime
        //! There is one KernelOracle per device
        KernelOracle* _kernelOracle;

        //! \brief Supported bindings
        std::string _bindings;
};

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get backend specifc KernelOracle
//!
//! \return KernelOracle handle
//!
////////////////////////////////////////////////////////////////////////////////

inline
KernelOracle*
BaseDevice::getKernelOracle() const
{
    return _kernelOracle;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get backend specifc StreamOracle
//!
//! \return StreamOracle handle
//!
////////////////////////////////////////////////////////////////////////////////

inline
StreamOracle*
BaseDevice::getStreamOracle() const
{
    return _streamOracle;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get backend specifc BufferMgr
//!
//! \return Device specifc BufferMgr handle
//!
////////////////////////////////////////////////////////////////////////////////

inline
BufferMgr* 
BaseDevice::getBufferMgr() const
{
    return _bufferMgr;
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get backend specifc ExecMgr
//!
//! \return Device specifc ExecMgr handle
//!
////////////////////////////////////////////////////////////////////////////////

inline
ExecMgr*
BaseDevice::getExecMgr() const
{
    return _execMgr;
}


#endif //_BASE_DEVICE_H_
