#ifndef _KERNEL_H_
#define _KERNEL_H_

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
//! \file Kernel.h
//!
//! \brief Contains kernel class definition.
//!
////////////////////////////////////////////////////////////////////////////////


#include <map>
#include "ExportDef.h"

class StreamImpl;
class KernelImpl;
class ConstImpl;

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \eum KernelType
    //!
    //! \brief Enum showing Kernel type. Simple or Reduce.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    enum KernelType 
    {
      KERNEL_MAP,
      KERNEL_REDUCE,
      KERNEL_FORCE_DWORD    = 0x7fffffff /* force 32-bit size enum */
    };
     
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \struct ArgumentInfo
    //!
    //! \brief Structure containing all the input and output parameters of a Kernel.
    //!
    ////////////////////////////////////////////////////////////////////////////////
    
    struct ArgumentInfo
    {
        //! \brief Start of domain of execution
        unsigned int* startExecDomain;

        //! \brief domain dimensions
        unsigned int* domainDimension;

        //! \brief group size for compute shader
        unsigned int* groupSize;
    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class Kernel
    //!
    //! \brief Public interface of Brook Kernel.
    //!
    //! Construction - Constructed when kernel is invoked in the application.
    //!
    //! Destruction - Destroyed when kernel execution is complete.
    //!
    //! Interaction - interact with KernelOracle to get KernelImpl object. 
    //!
    ////////////////////////////////////////////////////////////////////////////////

    class EXPORT Kernel
    {
        public:

            Kernel(const void* desc[], KernelType kernelType);
            void run(ArgumentInfo* argumentInfo) const;
            void PushStream(StreamImpl* s);
            void PushConstant(ConstImpl* c);
            void PushConstantBuffer(ConstImpl* c, unsigned int rank, unsigned int* dimensions);
            void PushGatherStream(StreamImpl* s);
            void PushScatterStream(StreamImpl* s);
            void PushReduceStream(StreamImpl* s);
            void PushReduce(StreamImpl* s);
            void PushOutput(StreamImpl* s);
            ~Kernel();

        private:
            
            //! \brief Forward all the calls to KernelImpl object
            KernelImpl* _kernelImpl;
    };

} //end brook namespace

#endif //_KERNEL_H_
