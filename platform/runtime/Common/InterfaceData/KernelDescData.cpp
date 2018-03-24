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


#include "KernelDescData.h"
#include <cassert>

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor.
    //! \param name Name of the pass.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    PassData::PassData(const char* name) : _refCount(1), Name(name),
                                           Image(NULL), computeFlag(false)
    {
        ConstArrays = new ArgDesc;
        Constants   = new ArgDesc;
        Inputs      = new ArgDesc;
        Outputs     = new ArgDesc;
    }

    void
    PassData::ref()
    {
        ++_refCount;
    }

    void
    PassData::unref()
    {
        assert(_refCount > 0);

        if(--_refCount == 0)
        {
            delete this;
        }
    }

    PassData::~PassData()
    {
        delete ConstArrays;
        delete Constants;
        delete Inputs;
        delete Outputs;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    TechniqueData::TechniqueData() : _refCount(1), AddressTranslation(false)
    {
        Passes = new std::vector<Pass>;
    }

    void
    TechniqueData::ref()
    {
        ++_refCount;
    }

    void
    TechniqueData::unref()
    {
        assert(_refCount > 0);

        if(--_refCount == 0)
        {
            delete this;
        }
    }

    TechniqueData::~TechniqueData()
    {
        delete Passes;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    KernelDescData::KernelDescData() : _refCount(1)
    {
        Techniques = new std::vector<Technique>;
    }

    void
    KernelDescData::ref()
    {
        ++_refCount;
    }

    void
    KernelDescData::unref()
    {
        assert(_refCount > 0);

        if(--_refCount == 0)
        {
            delete this;
        }
    }

    KernelDescData::~KernelDescData()
    {
        delete Techniques;
    }
}
