#ifndef _KERNELIMPL_H_
#define _KERNELIMPL_H_

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
//! \file KernelImpl.h
//!
//! \brief Contains KernelImpl class declaration.
//!
////////////////////////////////////////////////////////////////////////////////


#include <map>
#include "Kernel.h"
#include "KernelDesc.h"
#include "KernelDescData.h"
#include "ExecMgr.h"
#include "ConstImpl.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

using namespace brook;

class ConstImpl;
class StreamImpl;
class BaseDevice;

typedef std::map<unsigned int, ConstImpl*> ConstMap;
typedef std::map<unsigned int, StreamImpl*> StreamMap;

class ParameterInfo
{
    public:
        //! \brief map containing all the constants and their index.
        ConstMap constArguments;

        //! \brief map containing all the input streams and their index.
        StreamMap inputArguments;

        //! \brief map containing all the output streams and their index.
        StreamMap outputArguments;
};

////////////////////////////////////////////////////////////////////////////////
//!
//! \class KernelImpl
//!
//! \brief  Actual implementation class of Kernel.
//! This class provides logic for selection of technique, 
//! execution of technique and binding of extra constants.
//! 
//! Construction - Constructed by Kernel class.
//! 
//! Destruction -  Deleted when Kernel scope end.
//! 
//! Interaction - Interact with StreamImpl to get extra constants.
//! 
//! Correctness concern - We have tried implementing Scatter 
//! and see no limitation of scatter size in CAL. If we see this problem we can go 
//! further to derive a new calss CALKernelImpl for this workaround.
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT KernelImpl
{
    public:

        KernelImpl(const KernelDesc* desc, BaseDevice* device);
        virtual void run(brook::ArgumentInfo* arguments);
        virtual void PushStream(StreamImpl* s);
        virtual void PushConstant(ConstImpl* c);
        virtual void PushConstantBuffer(ConstImpl* c, unsigned int rank, unsigned int* dimension);
        virtual void PushGatherStream(StreamImpl* s);
        virtual void PushScatterStream(StreamImpl* s);
        virtual void PushReduceStream(StreamImpl* s);
        virtual void PushReduce(StreamImpl* s);
        virtual void PushOutput(StreamImpl* s);
        virtual ~KernelImpl();

    protected:

        virtual void _executeTechnique(Technique& technique, ArgumentInfo* arguments);

        virtual void _executePass(Pass& pass, ArgumentInfo* arguments);

        bool _executeDomainPass(Pass& pass, ArgInfo* args, ArgumentInfo* arguments, 
                                StreamImpl* firstOutput);
        void _createExtraConstant(ArgDesc& constDesc, ConstVec* vec);
        void _destroyExtraConstants(ConstVec consts, unsigned int start = 0);

        void _copyMapToVector(ArgDesc& inDesc, StreamMap* inMap, 
                              StreamVec* outVec);

        void _copyMapToVector(ArgDesc& inDesc, ConstMap* inMap, 
                              ConstVec* outVec);

        StreamImpl* _resizeStream(StreamImpl* input, const ArgumentDesc* desc,
                                  unsigned int* dimensions);

    protected:

        //! \brief KernelDesc object depending on the selected backend
        const KernelDesc* _desc;

        ParameterInfo _parameters;

        //! \brief total parameters
        unsigned int _paramCount;

        //! \brief Associated device
        BaseDevice* _device;

};

#endif //_KERNELIMPL_H_
