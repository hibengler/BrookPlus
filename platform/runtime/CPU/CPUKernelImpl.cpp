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

#include <map>
#include "cpu.hpp"
#include "Kernel.h"
#include "KernelDesc.h"
#include "KernelImpl.h"
#include "CPUKernelImpl.h"

using namespace brook;

#include "Runtime.h"
#include "KernelOracle.h"
#include "ConstImpl.h"
#include "KernelImpl.h"
#include "CPUStreamImpl.h"
#include "BaseDevice.h"

using namespace brook;

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

CPUKernelImpl::CPUKernelImpl(const void* src[], brook::KernelType kernelType,
                             KernelDesc* desc, BaseDevice* device)
                             :KernelImpl(desc, device)

{
    _cpuKernel = new CPUKernel(src);
    _kernelType = kernelType;
    _cpuStreamImpl = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implemetation of Kernel.run() method for CPU backend
//!
//! select the kernel execution method depending upon kernel type
//!
//! \param arguments Arguments information, it is not used.
//!
////////////////////////////////////////////////////////////////////////////////

void 
CPUKernelImpl::run(brook::ArgumentInfo* arguments)
{
    unsigned int* startExecDomain = arguments->startExecDomain;
    unsigned int* domainDimension = arguments->domainDimension;

    //! Set groupSize and group dimension 
    _cpuKernel->SetGroupSize(arguments->groupSize);
    
    switch(_kernelType)
    {
        case brook::KERNEL_MAP:
            _cpuKernel->Map(startExecDomain, domainDimension);
            break;
        case brook::KERNEL_REDUCE:
            _cpuKernel->Reduce(startExecDomain, domainDimension);
            break;
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
CPUKernelImpl::PushStream(StreamImpl* s)
{
    CPUStreamImpl* cpuStreamImpl = static_cast<CPUStreamImpl*>(s);
    _cpuKernel->PushStream(cpuStreamImpl->_cpuStream);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes constant for CPU back end
//!
//! \param c constant
//!
///////////////////////////////////////////////////////////////////////////////

void
CPUKernelImpl::PushConstant(ConstImpl* c)
{
    _cpuKernel->PushConstant((void*)c->getData());
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
CPUKernelImpl::PushConstantBuffer(ConstImpl* c, unsigned int rank, unsigned int* dimension)
{
    ::brook::BRformat dataFormat = c->getFormat();
    _cpuStreamImpl = new CPUStreamImpl(rank, dimension, &dataFormat, 1, 0);
    _cpuStreamImpl->read(c->getData());
    PushGatherStream(_cpuStreamImpl);
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
CPUKernelImpl::PushGatherStream(StreamImpl* s)
{
    CPUStreamImpl* cpuStreamImpl = static_cast<CPUStreamImpl*>(s);
    _cpuKernel->PushGatherStream(cpuStreamImpl->_cpuStream);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes temporary stream for CPU backend if
//! the reduction output is constant
//!
//! \param c constant
//!
///////////////////////////////////////////////////////////////////////////////

void
CPUKernelImpl::PushReduce(StreamImpl* s)
{
    CPUStreamImpl* cpuStreamImpl = static_cast<CPUStreamImpl*>(s);
    std::vector < StreamType >::iterator iterElementType = cpuStreamImpl->_cpuStream->elementType.begin();
    _cpuKernel->PushReduce(cpuStreamImpl->_cpuStream->data, *iterElementType);
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
CPUKernelImpl::PushScatterStream(StreamImpl* s)
{
    CPUStreamImpl* cpuStreamImpl = static_cast<CPUStreamImpl*>(s);
    _cpuKernel->PushScatterStream(cpuStreamImpl->_cpuStream);
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
CPUKernelImpl::PushReduceStream(StreamImpl* s)
{
    CPUStreamImpl* cpuStreamImpl = static_cast<CPUStreamImpl*>(s);
    std::vector < StreamType >::iterator iterElementType = cpuStreamImpl->_cpuStream->elementType.begin();
    _cpuKernel->PushReduceStream(cpuStreamImpl->_cpuStream, *iterElementType);
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
CPUKernelImpl::PushOutput(StreamImpl* s)
{
    CPUStreamImpl* cpuStreamImpl = static_cast<CPUStreamImpl*>(s);
    _cpuKernel->PushOutput(cpuStreamImpl->_cpuStream);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CPUKernelImpl::~CPUKernelImpl()
{
    if(_cpuStreamImpl != NULL)
    {
        delete _cpuStreamImpl;
    }

    delete _cpuKernel;
}