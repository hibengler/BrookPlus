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

#include "BaseDevice.h"
#include "KernelOracle.h"
#include "StreamOracle.h"
#include "BufferMgr.h"
#include "ExecMgr.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//! Initializes a particular device.
//!
////////////////////////////////////////////////////////////////////////////////

BaseDevice::BaseDevice(unsigned short id) : _id(id), _bufferMgr(NULL), _execMgr(NULL),
                                            _streamOracle(NULL), _kernelOracle(NULL),
                                            _bindings("")
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief initializes the Device.
//!
//! \return flag saying initialization was proper or not
//!
////////////////////////////////////////////////////////////////////////////////

bool
BaseDevice::initialize()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create StreamImpl given required parameters
//!
////////////////////////////////////////////////////////////////////////////////

StreamImpl*
BaseDevice::createStreamImpl(unsigned short rank, unsigned int* dimensions,
                             brook::BRformat* format, unsigned int count,
                             Device* device, unsigned int deviceCount)
{
    return _streamOracle->createStreamImpl(rank, dimensions, format, count,
                                           device, deviceCount);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//! cleanup the device.
//!
////////////////////////////////////////////////////////////////////////////////

BaseDevice::~BaseDevice()
{
    delete _bufferMgr;
    delete _execMgr;
    delete _kernelOracle;
    delete _streamOracle;
}
