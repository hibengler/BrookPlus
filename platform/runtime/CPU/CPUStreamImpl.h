#ifndef _CPUSTREAMIMPL_H_
#define _CPUSTREAMIMPL_H_

/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2008, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Stanford University nor the names of any contributors
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

#include "StreamImpl.h"
#include "cpu.hpp"

class CPUStream;

namespace brt
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class CPUStreamImpl
    //!
    //! \brief Actual implementation class of CPU Stream.
    //! 
    //! Construction - Constructed from Stream class.
    //! 
    //! Destruction -  Deleted when Stream scope end.
    //! 
    //! Interaction - This class provide interfce for all the properties of CPU Stream. 
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    class CPUStreamImpl:public StreamImpl
    {
        public:
            CPUStreamImpl(unsigned short rank, unsigned int* dimensions,
                   ::brook::BRformat* dataFormats, unsigned int count, BaseDevice* device,
                   brook::Device* execDevices = NULL, unsigned int deviceCount = 0);

            /* Stream Operators */
            void read(const void *ptr, const char* flags = NULL);
            void write(void* ptr, const char* flags = NULL);

            void copyDataFrom(StreamImpl* src);
            void copyDataTo(StreamImpl* dst);
            StreamImpl* createDomainStream(unsigned int* start, unsigned int* end);
            StreamImpl* execDomain(int numThreads);
            virtual ~CPUStreamImpl();

    public:
        CPUStream* _cpuStream;
    };

} // namspace brook

#endif // _CPUSTREAMIMPL_H_
