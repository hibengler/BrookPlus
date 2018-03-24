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

#include "CALBase.h"
#include "CALRuntime.h"
#include "CALDevice.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

CALRuntime::CALRuntime() 
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Function called from Runtime. It is used to construct the backend
//! specific runtime
//!
////////////////////////////////////////////////////////////////////////////////

CALRuntime*
CALRuntime::create()
{
    CALRuntime* runtime = new CALRuntime();
    if(!runtime->initialize())
    {
        delete runtime;
    
        return NULL;
    }

    return runtime;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief initializes the Runtime.
//!
//! \return flag saying initialization was proper or not
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALRuntime::initialize()
{
    CALresult result;

    // Initialize CAL
    result = calInit();
    CHECK_CAL_RESULT(result, "Failed to initialize CAL \n");

    // Get device count and initialize them
    calDeviceGetCount(&_numDevices);
    CHECK_CONDITION(_numDevices > 0, "No CAL devices available \n");

    unsigned int activeDevices = _numDevices;
    for(unsigned int i = 0; i < _numDevices; ++i)
    {
        CALDevice* device = new CALDevice(i);

        // If we are not able to initialize one device out of available two 
        // devices. No need to say 
        if(!device->initialize())
        {
            --activeDevices;
            delete device;
        }
        else
        {
            _devices.push_back(device);
        }
    }

    // If we don't have any active device - report error
    CHECK_CONDITION(_numDevices = activeDevices, "Not able to initialize any CAL device\n");

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALRuntime::~CALRuntime()
{
    calShutdown();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief return Total number of devices available for this backend
//!
////////////////////////////////////////////////////////////////////////////////

unsigned int
CALRuntime::getDeviceCount() const
{
    return _numDevices;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief return Device given by id
//!
////////////////////////////////////////////////////////////////////////////////

BaseDevice*
CALRuntime::getDevice(unsigned int id) const
{
    assert(id < _numDevices);
    return _devices[id];
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief return the runtime needed. Needed by Kernel Oracle to decide the 
//! correct kernelDesc
//!
////////////////////////////////////////////////////////////////////////////////
const char* 
CALRuntime::getRuntimeString() const
{
    return "cal";
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief C method to create CAL RUNTIME
//!
////////////////////////////////////////////////////////////////////////////////

CALRuntime*
createRuntime()
{
    return CALRuntime::create();
}