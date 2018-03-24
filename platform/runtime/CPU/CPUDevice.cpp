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

#include "CPUDevice.h"
#include "CPUStreamImpl.h"
#include "CPUKernelOracle.h"
#include "CPUBufferMgr.h"
#include "ExecMgr.h"
#include "StreamOracle.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//! User has to initialize the device before using this device
//!
////////////////////////////////////////////////////////////////////////////////

CPUDevice::CPUDevice(unsigned short id) : BaseDevice(id)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief initializes the CPUDevice.
//!
//! \return flag saying initialization was proper or not
//!
////////////////////////////////////////////////////////////////////////////////

bool
CPUDevice::initialize()
{
    _bufferMgr = new CPUBufferMgr(this);
    _execMgr = new ExecMgr(this);
    _streamOracle = new StreamOracle(this);
    _kernelOracle = new CPUKernelOracle(this);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Device type - "cpu"
//!
//! \return flag saying initialization was proper or not
//!
////////////////////////////////////////////////////////////////////////////////

const char*
CPUDevice::getType() const
{
    return "cpu";
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Supported interoperability
//!
////////////////////////////////////////////////////////////////////////////////

const char*
CPUDevice::getSupportedInteroperability() const
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create StreamImpl given required parameters
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl* 
CPUDevice::createStreamImpl(unsigned short rank, unsigned int* dimensions,
                            brook::BRformat* format, unsigned int count,
                            brook::Device* device, unsigned int deviceCount)
{
    return new CPUStreamImpl(rank, dimensions, format, count, this, device, deviceCount);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//! cleanup the device.
//!
////////////////////////////////////////////////////////////////////////////////

CPUDevice::~CPUDevice()
{
}

