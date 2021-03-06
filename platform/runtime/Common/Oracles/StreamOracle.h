#ifndef _STREAMORACLE_H_
#define _STREAMORACLE_H_

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
//! \file StreamOracle.h
//!
//! \brief Contains the declaration of StreamOracle class
//!
////////////////////////////////////////////////////////////////////////////////

#include "StreamImpl.h"
#include "ConstImpl.h"
#include "Utility.h"
#include "Device.h"
#include "ExportDef.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class StreamOracle
//!
//! \brief Class to create StreamImpl depending on its properties.
//!
//! An AT stream is created by the backend if that rank or dimensions
//! is not supported in that backend.
//! This class implements logic for domain implementation.
//! Domain implementation need creation of temporary buffer and 
//! copying data from orginal to temporary and vice-versa.
//! 
//! Construction - Constructed with Runtime construction
//! 
//! Destruction - Destroyed once application exits.
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT StreamOracle
{
    public:

        explicit StreamOracle(BaseDevice* device);
        virtual ~StreamOracle();

        virtual StreamImpl* createStreamImpl(unsigned short rank, 
                                             unsigned int* dimensions,
                                             BRformat* format, unsigned int count,
                                             Device* device = NULL,
                                             unsigned int deviceCount = 0);
        virtual StreamImpl* createStreamImpl(StreamImpl* parent, unsigned int* start,
                                             unsigned int* dimensions);

    protected:

        //! \brief Device for which StreamOracle works
        BaseDevice* _device;
};

#endif //_STREAMORACLE_H_
