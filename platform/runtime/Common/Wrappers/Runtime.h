#ifndef _RUNTIME_H_
#define _RUNTIME_H_

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
//! \file Runtime.h
//!
//! \brief Contains the declaration of Runtime class
//!
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "ExportDef.h"

//  The warning disables visual studio's
//  annoying habit of warning when using the standard set lib
#ifdef _WIN32
#pragma warning (disable : 4251)
#endif

#include "Formats.h"

class BaseDevice;
class StreamImpl;

typedef void(*MemoryHandler)();

namespace brook
{
    EXPORT void setMemoryHandle(MemoryHandler handle);

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Abstract class represnting Runtime for backend.
//! A singleton that makes sure all the creation routine for stream and 
//! kernel goes to the correct backend.
//! 
//! Construction - Constructed as soon as any new stream is created.
//! 
//! Destruction - Destroyed once application exits.
//! 
//! Interaction - Contains a set of devices representing all the devices available
//! and asscociated Oracle and Mgr classes.
//! 
//! Error Conditions - It can Fail to initialize a particular backend. 
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT Runtime
{
    public:

        virtual unsigned int getDeviceCount() const = 0;
        virtual BaseDevice* getDevice(unsigned int id) const = 0;
        virtual const char* getRuntimeString() const = 0;

        virtual ~Runtime();

    protected:

            Runtime();
            virtual bool initialize() = 0;

    protected:

        //! \brief Number of devices visible
        unsigned int _numDevices;

        //! \brief Contains the list of devices that a developer might want to use
        std::vector<BaseDevice*> _devices;
};

} //end namespace brook

#endif //_RUNTIME_H_
