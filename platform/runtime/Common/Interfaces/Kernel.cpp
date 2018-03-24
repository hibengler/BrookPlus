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

#include "Kernel.h"
#include "KernelImpl.h"
#include "KernelOracle.h"
#include "ConstImpl.h"
#include "SystemRT.h"
#include "Runtime.h"
#include "BaseDevice.h"
#include <iostream>
#include <cassert>

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Kernel::Kernel(const void* desc[], KernelType kernelType)
    {
        // XXX - requires same device as of Streams to be set before Kernel Call
        unsigned int deviceCount = 0;

        // We are using only one device in this release
        BaseDevice *device = SystemRT::getInstance()->getCurrentDevices(deviceCount)[0];
        KernelOracle *kernelOracle = device->getKernelOracle();

        // Ask kernel oracle to create the kernelImpl
        _kernelImpl = kernelOracle->createKernelImpl(desc, kernelType);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Ask KernelImpl to run it
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    Kernel::run(ArgumentInfo* argumentInfo) const 
    {
        if(_kernelImpl)
        {
            _kernelImpl->run(argumentInfo);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes stream for CPU. Since CPU backend distinguishes
    //! between stream, gather stream, reduce output, scatter stream, we need diff.
    //! methods respectively.
    //!
    //! \param s stream
    //!
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushStream(StreamImpl* s)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushStream(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes constant for CPU backend
    //!
    //! \param c constant
    //!
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushConstant(ConstImpl* c)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushConstant(c);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes constant buffer for CPU backend
    //!
    //! \param c constant
    //! \rank rank of the constant buffer
    //! \dimension dimensions for the constant buffer
    //
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushConstantBuffer(ConstImpl* c, unsigned int rank, unsigned int* dimension)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushConstantBuffer(c, rank, dimension);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes stream for CPU. Since CPU backend distinguishes
    //! between stream, gather stream, reduce output, scatter stream, we need diff.
    //! methods respectively.
    //!
    //! \param s stream
    //!
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushGatherStream(StreamImpl* s)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushGatherStream(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes stream for CPU. Since CPU backend distinguishes
    //! between stream, gather stream, reduce output, scatter stream, we need diff.
    //! methods respectively.
    //!
    //! \param s stream
    //!
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushScatterStream(StreamImpl* s)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushScatterStream(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes stream for CPU. Since CPU backend distinguishes
    //! between stream, gather stream, reduce output, scatter stream, we need diff.
    //! methods respectively.
    //!
    //! \param s stream
    //!
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushReduceStream(StreamImpl* s)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushReduceStream(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes temporary stream for CPU backend when
    //! reduction output is constant
    //!
    //! \param c constant
    //!
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushReduce(StreamImpl* s)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushReduce(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Pushes stream for CPU. Since CPU backend distinguishes
    //! between stream, gather stream, reduce output, scatter stream, we need diff.
    //! methods respectively.
    //!
    //! \param s stream
    //!
    ///////////////////////////////////////////////////////////////////////////////

    void
    Kernel::PushOutput(StreamImpl* s)
    {
        if(_kernelImpl)
        {
            _kernelImpl->PushOutput(s);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Destructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Kernel::~Kernel()
    {
        delete _kernelImpl;
    }

} //end brook namespace
