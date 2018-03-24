#ifndef _STREAMDEF_H_
#define _STREAMDEF_H_

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
//! \file StreamDef.h
//!
//! \brief Contains the definition of Stream class members.
//!
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "SystemRT.h"
#include "StreamData.h"

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor of Stream
    //!
    //! \param rank Rank of Stream
    //! \param dimensions A pointer to an array holding the upper bound 
    //! for each of the stream's dimensons
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>::Stream(unsigned short rank, unsigned int* dimensions)
    {
        T* temp = NULL;
        unsigned int count = 0;
        BRformat* format = getStreamType(temp, count);

        // Ask runtime to create streamImpl
        _stream = new StreamData(SystemRT::getInstance()->
                                 createStreamImpl(rank, dimensions, format, count));

        delete[] format;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor of Stream
    //!
    //! \param rank Rank of Stream
    //! \param dimensions A pointer to an array holding the upper bound 
    //! for each of the stream's dimensons
    // \param type - Stream type - d3d9, d3d10
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>::Stream(unsigned short rank, unsigned int* dimensions, const char* type)
    {
        T* temp = NULL;
        unsigned int count = 0;
        BRformat* format = getStreamType(temp, count);

        // Ask runtime to create streamImpl
        _stream = new StreamData(SystemRT::getInstance()->
                                 createStreamImpl(rank, dimensions, format, count));
        _stream->setType("d3d9");

        delete[] format;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor of Stream
    //!
    //! \param streamImpl object of StreamImpl
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template <class T>
    Stream<T>::Stream(StreamImpl* streamImpl)
    {
        _stream = new StreamData(streamImpl);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Copy Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template <class T>
    Stream<T>::Stream(const Stream<T>& other)
    {
        _stream = new StreamData(other);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Assignement operator
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template <class T>
    Stream<T>&
    Stream<T>::operator=(const Stream<T>& other)
    {
        if(this != &other)
        {
            delete _stream;
            _stream = new StreamData(other);
        }

        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Transfer data from application to backend specific device
    //!
    //! \param ptr Application pointer
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void
    Stream<T>::read(const void* ptr, const char* flags)
    {
        _stream->read(ptr, flags);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Transfer data from backend specific device to application
    //!
    //! \param ptr Application pointer
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void
    Stream<T>::write(void* ptr, const char* flags) const
    {
        _stream->write(ptr, flags);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Copy data from source stream to this stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void
    Stream<T>::assign(Stream<T> source)
    {
        _stream->copyDataFrom(source);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set property to stream given by name
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void
    Stream<T>::setProperty(const char* name, void* value)
    {
        _stream->setProperty(name, value);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Return device associated to stream for execution
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Device*
    Stream<T>::getDevices(unsigned int* count)
    {
        return _stream->getExecDevices(count);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Query if asynchronous operations are done on stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    bool
    Stream<T>::isSync()
    {
        return _stream->isSync();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Wait for all the asynchronous operations to finish
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void
    Stream<T>::finish()
    {
        _stream->flush();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set the domain on Stream
    //!
    //! All the operators or kernel on the Stream is executed on the 
    //! domain part of the stream
    //!
    //! \param start Pointer pointing to start of the dimensions
    //! \param end Pointer pointing to end of the dimensions
    //!
    //! \return Return a new stream with dimensions = end - start
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>
    Stream<T>::domain(unsigned int* start, unsigned int* end) const
    {
        // Create a new stream of domain size
        Stream<T> stream(_stream->createDomainStream(start, end));
        return stream;
    }

#ifdef USE_OLD_API

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set the domain on Stream
    //!
    //! This interface will be deprecated in the next release
    //! All the operators or kernel on the Stream is executed on the 
    //! domain part of the stream
    //!
    //! \param start point pointing to start of the dimensions
    //! \param end point pointing to end of the dimensions
    //!
    //! \return Return a new stream with dimensions = end - start
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>
    Stream<T>::domain(int start, int end) const
    {
        unsigned int begin[] = {start};
        unsigned int stop[] = {end};

        // Create a new stream of domain size
        return domain(begin, stop);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set the domain on Stream
    //!
    //! This interface will be deprecated in the next release
    //! All the operators or kernel on the Stream is executed on the 
    //! domain part of the stream
    //!
    //! \param start point pointing to start of the dimensions
    //! \param end point pointing to end of the dimensions
    //!
    //! \return Return a new stream with dimensions = end - start
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>
    Stream<T>::domain(int2 start, int2 end) const
    {
        unsigned int begin[] = {start.x, start.y};
        unsigned int stop[] = {end.x, end.y};

        // Create a new stream of domain size
        return domain(begin, stop);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set the domain on Stream
    //!
    //! This interface will be deprecated in the next release
    //! All the operators or kernel on the Stream is executed on the 
    //! domain part of the stream
    //!
    //! \param start point pointing to start of the dimensions
    //! \param end point pointing to end of the dimensions
    //!
    //! \return Return a new stream with dimensions = end - start
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>
    Stream<T>::domain(int3 start, int3 end) const
    {
        unsigned int begin[] = {start.x, start.y, start.z};
        unsigned int stop[] = {end.x, end.y, end.z};

        // Create a new stream of domain size
        return domain(begin, stop);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set the domain on Stream
    //!
    //! This interface will be deprecated in the next release
    //! All the operators or kernel on the Stream is executed on the 
    //! domain part of the stream
    //!
    //! \param start point pointing to start of the dimensions
    //! \param end point pointing to end of the dimensions
    //!
    //! \return Return a new stream with dimensions = end - start
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>
    Stream<T>::domain(int4 start, int4 end) const
    {
        unsigned int begin[] = {start.x, start.y, start.z, start.w};
        unsigned int stop[] = {end.x, end.y, end.z, end.w};

        // Create a new stream of domain size
        return domain(begin, stop);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set the execution domain of Stream
    //!
    //! This interface will be deprecated in the next release
    //! Shows number of threads to be executed for a scatter output
    //!
    //! \param numThreads number of threads 
    //!
    //! \return Return a new stream with execdomain set on it
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>
    Stream<T>::execDomain(int numThreads) const
    {
        // Create a new stream of domain size
        Stream<T> stream(_stream->execDomain(numThreads));
        return stream;
    }

#endif

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Return the error code associated to the stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    brook::BRerror
    Stream<T>::error()
    {
        return _stream->getErrorCode();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Return the error log associated to the stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    const char*
    Stream<T>::errorLog()
    {
        return _stream->getErrorString();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Conversion operator to StreamImpl* object
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>::operator StreamImpl*() const
    {
        return _stream->getImplObject();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Destructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    Stream<T>::~Stream()
    {
        delete _stream;
    }

} //end brook namespace

#ifdef USE_OLD_API

template<class T>
void streamRead(brook::Stream<T> stream, void* ptr)
{
    stream.read(ptr);
}

template<class T>
void streamWrite(brook::Stream<T> stream, void* ptr)
{
    stream.write(ptr);
}

#endif

#endif //_STREAMDEF_H_
