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


#include "KernelDesc.h"
#include "KernelDescData.h"

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor.
    //! \param name Name of the pass.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass::Pass(const char* name)
    {
        _data = new PassData(name);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Overloded copyConstructor
    //! \param pass the Pass object reference
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass::Pass(const Pass& p)
    {
        _data = p.getData();
        _data->ref();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Operator overloaded to provide support for the copy constructor
    //! \param pass the Pass object reference
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass&
    Pass::operator=(const Pass& p)
    {
        if(this != &p && _data != p.getData())
        {
            _data->unref();
            _data = p.getData();
            _data->ref();
        }

        return *this;
    }

    PassData*
    Pass::getData() const
    {
        return _data;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Set the location of image
    //! \param name Name of the pass.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass& Pass::image(const char* image)
    {
        if(image)
        {
            _data->Image = image;
        }

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Set constant arrays metadata using argument index of the constant
    //!         and its size
    //! \param argumentIndex 
    //! \param size
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass& Pass::constArray(int argumentIndex, int size)
    {
        _data->ConstArrays->push_back(ArgumentDesc(argumentIndex, size));

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Set constant metadata using argument index of the constant
    //!         and its usage if it is an extra constant required for different
    //!         virtualization.
    //! \param argumentIndex 
    //! \param usage 
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass& Pass::constant(int argumentIndex, ConstUsage usage)
    {
        _data->Constants->push_back(ArgumentDesc(argumentIndex, usage));

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Input metadata information showing its argument index.
    //!         usage is 0 for now.
    //! \param argumentIndex 
    //! \param usage 
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass& Pass::input(int argumentIndex, int usage, AccessType type)
    {
        _data->Inputs->push_back(ArgumentDesc(argumentIndex, usage, type));

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Output metadata information showing its argument index.
    //!         usage is 0 for now.
    //! \param argumentIndex 
    //! \param usage 
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Pass& Pass::output(int argumentIndex, int usage, AccessType type)
    {
        _data->Outputs->push_back(ArgumentDesc(argumentIndex, usage, type));

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief meta data information whether a shader is of compute shader type
    //!
    ////////////////////////////////////////////////////////////////////////////////
    
    Pass& Pass::computeShader()
    {
        _data->computeFlag = true;

        return *this;
    }

    Pass::~Pass()
    {
        _data->unref();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Technique::Technique()
    {
        _data = new TechniqueData();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Overloded copy constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Technique::Technique(const Technique& p)
    {
        _data = p.getData();
        _data->ref();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Operator= overloaded
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Technique&
    Technique::operator=(const Technique& p)
    {
        if(this != &p && _data != p.getData())
        {
            _data->unref();
            _data = p.getData();
            _data->ref();
        }

        return *this;
    }

    TechniqueData*
    Technique::getData() const
    {
        return _data;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Add a pass in technique
    //! \param Pass 
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Technique& Technique::pass(Pass& p)
    {
        _data->Passes->push_back(p);

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief set AddressTranslation property
    //!
    ////////////////////////////////////////////////////////////////////////////////
    
    Technique& Technique::addressTranslation()
    {
        _data->AddressTranslation = true;

        return *this;
    }

    Technique::~Technique()
    {
        _data->unref();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    KernelDesc::KernelDesc()
    {
        _data = new KernelDescData();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Operator overloaded to provide support for the copy constructor
    //! \param kernelDesc the KernelDesc object reference
    //!
    ////////////////////////////////////////////////////////////////////////////////

    KernelDesc::KernelDesc(const KernelDesc& desc)
    {
        _data = desc.getData();
        _data->ref();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Operator overloaded to provide support for the copy constructor
    //! \param technique the Technique object reference
    //!
    ////////////////////////////////////////////////////////////////////////////////

    KernelDesc&
    KernelDesc::operator=(const KernelDesc& desc)
    {
        if(this != &desc && _data != desc.getData())
        {
            _data->unref();
            _data = desc.getData();
            _data->ref();
        }

        return *this;
    }

    KernelDescData*
    KernelDesc::getData() const
    {
        return _data;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Add a technique in KernelDesc
    //! \param Technique 
    //!
    ////////////////////////////////////////////////////////////////////////////////
    
    KernelDesc& KernelDesc::technique(Technique& t)
    {
        _data->Techniques->push_back(t);

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Destructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    KernelDesc::~KernelDesc()
    {
        _data->unref();
    }
}
