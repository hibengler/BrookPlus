#ifndef _CPUKERNELIMPL_H_
#define _CPUKERNELIMPL_H_

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
//! \file CPUKernelImpl.h
//!
//! \brief Contains CPUKernelImpl class declaration.
//!
////////////////////////////////////////////////////////////////////////////////


#include <map>
#include "cpu.hpp"
#include "Kernel.h"
#include "KernelDesc.h"
#include "ExecMgr.h"
#include "KernelImpl.h"

using namespace brook;

class ConstImpl;
class StreamImpl;
class Device;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class KernelImpl
//!
//! \brief  Wrapper implementation class of CPUKernel.
//! 
////////////////////////////////////////////////////////////////////////////////

class CPUKernelImpl:public KernelImpl
{
    public:

        CPUKernelImpl(const void* src[], brook::KernelType kernelType,
                      KernelDesc* desc, BaseDevice* device);
        void PushStream(StreamImpl* s);
        void PushConstant(ConstImpl* c);
        void PushConstantBuffer(ConstImpl* c, unsigned int rank, unsigned int* dimension);
        void PushGatherStream(StreamImpl* s);
        void PushScatterStream(StreamImpl* s);
        void PushReduceStream(StreamImpl* s);
        void PushReduce(StreamImpl* s);
        void PushOutput(StreamImpl* s);
        void run(brook::ArgumentInfo* arguments);
        virtual ~CPUKernelImpl();
    
    public:
        
        CPUKernel* _cpuKernel;
        StreamImpl* _cpuStreamImpl;
        brook::KernelType _kernelType;
};

#endif //_CPUKERNELIMPL_H_
