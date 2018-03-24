#ifndef _STREAM_H_
#define _STREAM_H_

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
//! \file Stream.h
//!
//! \brief Contains the definition of Stream class
//!
////////////////////////////////////////////////////////////////////////////////

#include "VectorDataType.h"
#include "ErrorCodes.h"

#define USE_OLD_API

class StreamImpl;

namespace brook
{
    class Device;
    class StreamData;

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class Stream
    //!
    //! \brief Templated class represnting a SIMD data on backend
    //! Template value can be float, float2, float3, float4, 
    //! and similarly for other data types.
    //! 
    //! Construction - Constructed when stream is declared in the application.
    //! 
    //! Destruction - Destroyed when stream goes out of scope.
    //! 
    //! Interaction - interact with StreamOracle to get StreamImpl object. 
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    class Stream
    {
        public:

            Stream(unsigned short rank, unsigned int* dimensions);
            Stream(unsigned short rank, unsigned int* dimensions, const char* type);
            explicit Stream(StreamImpl* streamImpl);
            Stream(const Stream<T>& other);
            Stream<T>& operator=(const Stream<T>& other);
            void read(const void* ptr, const char* flags = NULL);
            void write(void* ptr, const char* flags = NULL) const;
            Stream<T> domain(unsigned int* start, unsigned int* end) const;
            void assign(Stream<T> source);
            void setProperty(const char* name, void* value);
            Device* getDevices(unsigned int* count);
            bool isSync();
            void finish();
            BRerror error();
            const char* errorLog();
             operator StreamImpl*() const;
            ~Stream();

#ifdef USE_OLD_API

            Stream<T> domain(int start, int end) const;
            Stream<T> domain(int2 start, int2 end) const;
            Stream<T> domain(int3 start, int3 end) const;
            Stream<T> domain(int4 start, int4 end) const;
            Stream<T> execDomain(int numThreads) const;

#endif

        private:

            //! \brief implements the Stream functionalities
            StreamData* _stream;

    };

} //end brook namespace

#ifdef USE_OLD_API

template<class T>
void streamRead(brook::Stream<T> stream, void* ptr);

template<class T>
void streamWrite(brook::Stream<T> stream, void* ptr);

#endif

#include "StreamDef.h"

#endif //_STREAM_H_
