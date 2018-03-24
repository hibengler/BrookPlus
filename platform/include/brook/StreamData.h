#ifndef _STREAM_DATA_H_
#define _STREAM_DATA_H_

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
//! \file StreamData.h
//!
////////////////////////////////////////////////////////////////////////////////

#include "ErrorCodes.h"
#include "ExportDef.h"

class StreamImpl;

namespace brook
{
    class Device;

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class StreamData
    //!
    //! \brief Forward all the calls to StreamImpl
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    class EXPORT StreamData
    {
        public:

            explicit StreamData(StreamImpl* streamImpl);

            void read(const void* ptr, const char* flags);
            void write(void* ptr, const char* flags) const;

            void setType(const char* type);
            void setProperty(const char* name, void* value);

            StreamImpl* createDomainStream(unsigned int* start, unsigned int* end) const;
            StreamImpl* execDomain(int numThreads);

            void copyDataFrom(StreamImpl* src);
            Device* getExecDevices(unsigned int* count);
            StreamImpl* getImplObject() const;

            bool isSync();
            void flush();

            brook::BRerror getErrorCode();
            const char* getErrorString();
            ~StreamData();

        private:

            //! \brief implements the Stream functionalities
            StreamImpl* _stream;

    };

} //end brook namespace

#endif //_STREAM_DATA_H_
