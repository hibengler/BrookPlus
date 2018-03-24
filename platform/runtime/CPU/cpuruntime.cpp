/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Stanford University nor the names of any contributors 
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

#include "cpu.hpp"
#include "CPUStreamImpl.h"
#include "CPUDevice.h"

namespace brt
{

    CPURuntime::CPURuntime()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Function called from Runtime. It is used to construct the backend
    //! specific runtime
    //!
    ////////////////////////////////////////////////////////////////////////////////

    CPURuntime*
    CPURuntime::create()
    {
        CPURuntime* runtime = new CPURuntime();
        if(!runtime->initialize())
        {
            delete runtime;
        
            return NULL;
        }

        return runtime;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief return Total number of devices available for this backend
    //!
    ////////////////////////////////////////////////////////////////////////////////

    unsigned int
    CPURuntime::getDeviceCount() const
    {
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief return Device given by id
    //!
    ////////////////////////////////////////////////////////////////////////////////

    BaseDevice*
    CPURuntime::getDevice(unsigned int id) const
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
    CPURuntime::getRuntimeString() const
    {
        return "cpu";
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief initializes the Runtime.
    //!
    //! \return flag saying initialization was proper or not
    //!
    ////////////////////////////////////////////////////////////////////////////////

    bool
    CPURuntime::initialize()
    {
        _numDevices = 1;
        BaseDevice* device = new CPUDevice(0);

        if(!device->initialize())
        {
            --_numDevices;
            delete device;
        }
        else
        {
            _devices.push_back(device);
        }

        return true;
    }

    // FIXME - the following section is a work-in-progress bringing the writemask stuff through to the CPU runtime

    IWriteQuery *CPURuntime::createWriteQuery()
    {
        LOG(LOG_FN_ENTRY, "CPURuntime::createWriteQuery()\n");
        return NULL;//new CPUWriteQuery();
    }

    IWriteMask *CPURuntime::createWriteMask(int inY, int inX)
    {
        LOG(LOG_FN_ENTRY, "CPURuntime::createWriteMask(%d, %d)\n", inY, inX);
        return NULL;//new CPUWriteMask(inY, inX);
    }

    // ---- writemask stuff ends here

    CPURuntime::~CPURuntime()
    {

    }
}
