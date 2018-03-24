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

#include "StreamData.h"
#include "StreamImpl.h"

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor of Stream
    //!
    //! \param streamImpl object of StreamImpl
    //!
    ////////////////////////////////////////////////////////////////////////////////

    StreamData::StreamData(StreamImpl* streamImpl)
                            :_stream(streamImpl)
    {
        if(_stream)
        {
            _stream->ref();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Transfer data from application to backend specific device
    //!
    //! \param ptr Application pointer
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    StreamData::read(const void* ptr, const char* flags)
    {
        if(_stream)
        {
            if(!ptr)
            {
                _stream->setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
                _stream->setErrorString("Stream Read : Stream can't read from a NULL pointer\n");

                return;
            }

            _stream->read(ptr, flags);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Transfer data from backend specific device to application
    //!
    //! \param ptr Application pointer
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    StreamData::write(void* ptr, const char* flags) const
    {
        if(_stream)
        {
            if(!ptr)
            {
                _stream->setErrorCode(brook::BR_ERROR_INVALID_PARAMATER);
                _stream->setErrorString("Stream Write : Stream can't write to a NULL pointer\n");

                return;
            }

            _stream->write(ptr, flags);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Copy data from source stream to this stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    StreamData::copyDataFrom(StreamImpl* src)
    {
        if(_stream)
        {
            _stream->copyDataFrom(src);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set stream type - d3d9 d3d10
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    StreamData::setType(const char* type)
    {
        if(_stream)
        {
            _stream->setType(type);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set property to stream given by name
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    StreamData::setProperty(const char* name, void* value)
    {
        if(_stream)
        {
            _stream->setProperty(name, value);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Return device associated to stream for execution
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Device*
    StreamData::getExecDevices(unsigned int* count)
    {
        Device* devices = NULL;
        if(_stream && count)
        {
            devices = _stream->getExecDevices(count);
        }

        return devices;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Query if asynchronous operations are done on stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    bool
    StreamData::isSync()
    {
        if(_stream)
        {
            return _stream->isSync();
        }

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Wait for all the asynchronous operations to finish
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    StreamData::flush()
    {
        if(_stream)
        {
            _stream->flush();
        }
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

    StreamImpl*
    StreamData::createDomainStream(unsigned int* start, unsigned int* end) const
    {
        // Create a new stream of domain size
        return (_stream ? (_stream->createDomainStream(start, end)) : NULL);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set the execution domain of Stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    StreamImpl*
    StreamData::execDomain(int numThreads)
    {
        // Create a new stream of domain size
        return (_stream ? (_stream->execDomain(numThreads)) : NULL);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Return the error code associated to the stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    brook::BRerror
    StreamData::getErrorCode()
    {
        return (_stream ? (_stream->getErrorCode()) : brook::BR_ERROR_INVALID_PARAMATER);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Return the error log associated to the stream
    //!
    ////////////////////////////////////////////////////////////////////////////////

    const char*
    StreamData::getErrorString()
    {
        return (_stream ? _stream->getErrorString() : "Invalid stream\n");
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief get StreamImpl* object
    //!
    ////////////////////////////////////////////////////////////////////////////////

    StreamImpl*
    StreamData::getImplObject() const
    {
        return _stream;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Destructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    StreamData::~StreamData()
    {
        if(_stream)
        {
            _stream->unref();
        }
    }

} //end brook namespace
