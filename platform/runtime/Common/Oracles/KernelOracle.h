#ifndef _KERNELORACLE_H_
#define _KERNELORACLE_H_

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
//! \file KernelOracle.h
//!
//! \brief Contains KernelOracle class definition.
//!
////////////////////////////////////////////////////////////////////////////////

#include "Runtime.h"
#include "Kernel.h"
#include "Device.h"
#include "ExportDef.h"

using namespace brook;

class KernelImpl;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class KernelOracle
//!
//! \brief Class that takes pointer to decs and choose the correct desc based on
//! runtime backend
//! 
//! Construction - The object is created when runtime is initialized .
//! 
//! Destruction -  Deleted when application exits.
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT KernelOracle
{
    public:
    
        explicit KernelOracle(BaseDevice* device);
        virtual KernelImpl* createKernelImpl(const void* desc[], brook::KernelType kernelType);
        virtual bool allowRWAlias();
        virtual bool disableStreamResize();
        virtual ~KernelOracle();

        protected:

        //! \brief Device for which KernelOracle works
        BaseDevice* _device;
};

#endif //_KERNELORACLE_H_
