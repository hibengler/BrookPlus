#ifndef _STREAMIMPLAT_H_
#define _STREAMIMPLAT_H_

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
//! \file StreamImplAT.h
//!
//! \brief Contains the declaration of StreamImplAT class
//!
////////////////////////////////////////////////////////////////////////////////

#include "StreamImpl.h"
#include "ExportDef.h"

using namespace brook;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class StreamImplAT
//!
//! \brief Override StreamImpl methods for AT specific implementation
//!
////////////////////////////////////////////////////////////////////////////////

class EXPORT StreamImplAT : public StreamImpl
{
    public:

        StreamImplAT(unsigned short logRank, unsigned short bufferRank,
                     unsigned int* logDimensions, unsigned int* bufferDimensions,
                     BRformat* dataFormat, unsigned int count, BaseDevice* device, 
                     brook::Device* execDevices = NULL, unsigned int deviceCount = 0);

        StreamImplAT(StreamImpl* parent, unsigned int* start, unsigned short bufferRank,
                           unsigned int* logDimensions, unsigned int* bufferDimensions);

        StreamImpl* clone();
        void setType(const char* type);

        bool copy(StreamImpl* src, unsigned int* srcStartPoint, StreamImpl* dst,
                  unsigned int* dstStartPoint, unsigned int* dimension,
                  bool completeCopy = true, unsigned int srcBuf = 0, unsigned int dstBuf = 0);

        bool isAddressTranslated() const;
        unsigned short getBufferRank() const;
        unsigned int* getBufferDimensions() const;

        ~StreamImplAT();

    private:

        StreamImplAT();

    private:
        
        //! \brief Rank of the GPU buffer
        unsigned short _bufferRank;

        //! \brief Dimensions of the GPU buffer.
        unsigned int* _bufferDimensions;
};

#endif //_STREAMIMPLAT_H_
