#ifndef _CONSTIMPL_H_
#define _CONSTIMPL_H_

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
//! \file ConstImpl.h
//!
//! \brief Contains ConstImpl class declaration.
//!
////////////////////////////////////////////////////////////////////////////////

#include "Formats.h"

class BaseDevice;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class ConstImpl
//!
//! \brief Actual implementation class of Constant
//! 
//! Construction - Constructed from Constant class.
//! 
//! Destruction -  Deleted by the owning Constant
//! 
////////////////////////////////////////////////////////////////////////////////

#include "ExportDef.h"

class EXPORT ConstImpl
{
    public:

        ConstImpl(void* data, ::brook::BRformat format,
                  BaseDevice* device, unsigned int numElements = 1);
        inline void* getData() const;
        inline const ::brook::BRformat getFormat() const;
        inline const unsigned int getElementCount() const;
        ~ConstImpl();

    private:

        //! \brief Contains the data pointer
        unsigned char* _data;

        //! \brief contains the data format
        ::brook::BRformat _dataFormat;

        //! \brief number of elements
        unsigned int _numElements;

        //! \brief Associated device
        BaseDevice* _device;
};

inline
void*
ConstImpl::getData() const
{
    return _data;
}

inline const
::brook::BRformat
ConstImpl::getFormat() const
{
    return _dataFormat;
}

inline const
unsigned int 
ConstImpl::getElementCount() const
{
    return _numElements;
}

#endif //_CONSTIMPL_H_
