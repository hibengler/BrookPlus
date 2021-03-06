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

#include <string>
#include "CommonDefs.h"
#include "CALKernelOracle.h"
#include "CALKernelImpl.h"
#include "ReduceImpl.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

CALKernelOracle::CALKernelOracle(BaseDevice* device) : KernelOracle(device)
{
    _rwAlias = (getenv("BRT_PERMIT_READ_WRITE_ALIASING") != NULL);
    _disableResize = (getenv("BRT_DISABLE_STREAM_RESIZE") != NULL);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Depending on Backend genarates backend specific KernelImpl
//! \param desc The kernel descriptor
//! \param kernelType The Type of the kernel. 
//! \return Object of type KernelImpl*
//!
////////////////////////////////////////////////////////////////////////////////

KernelImpl* CALKernelOracle::createKernelImpl(const void* desc[], brook::KernelType kernelType)
{
    unsigned int i = 0;
    while (desc[i] != NULL)
    {
        const char *nameString = (const char *) desc[i];
        const ::brook::KernelDesc* descriptor = (::brook::KernelDesc*) desc[i + 1];

        // Look for backend specific string in the given descriptors
        if (descriptor != NULL && nameString != NULL && strcmp("cal", nameString) == 0)
        {
            switch(kernelType)
            {
                case brook::KERNEL_MAP:
                    return new CALKernelImpl(descriptor, _device);

                case brook::KERNEL_REDUCE:
                    return new ReduceImpl(descriptor, _device);
            }
        }

        i += 2;
    }

    SET_ERROR("Could not create Kernel. Please check whether brcc has generated "
                 "the code for the Backend specified by BR_RUNTIME.\n");

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to determine whether allow reading/writing feom/to the same stream
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALKernelOracle::allowRWAlias()
{
    return _rwAlias;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Should we disable stream resize in case stream dimensions doesn't 
//! match with output stream
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALKernelOracle::disableStreamResize()
{
    return _disableResize;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALKernelOracle::~CALKernelOracle()
{
}

