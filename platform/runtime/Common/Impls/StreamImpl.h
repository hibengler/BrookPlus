#ifndef _STREAMIMPL_H_
#define _STREAMIMPL_H_

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
//! \file StreamImpl.h
//!
//! \brief Contains the declaration of StreamImpl class
//!
////////////////////////////////////////////////////////////////////////////////

#include "Formats.h"
#include "KernelDesc.h"
#include "ErrorCodes.h"
#include "Device.h"
#include "ExportDef.h"
#include <map>
#include <string>

class Buffer;
class BaseDevice;
class brook::Device;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class StreamImpl
//!
//! \brief Actual implementation class of Stream.
//! 
//! Construction - Constructed from Stream class.
//! 
//! Destruction -  Deleted when Stream scope end.
//! 
//! Interaction - This class provide interfce for all the properties of Stream. 
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT StreamImpl
{
    public:

        StreamImpl(unsigned short rank, unsigned int* dimensions,
                   ::brook::BRformat* dataFormats, unsigned int count, BaseDevice* device, 
                   brook::Device* execDevices = NULL, unsigned int deviceCount = 0);
        StreamImpl(StreamImpl* parent, unsigned int* start, unsigned int* dimensions);
        virtual void read(const void* ptr, const char* flags = NULL);
        virtual void write(void* ptr, const char* flags = NULL);
        virtual StreamImpl* createDomainStream(unsigned int* start, unsigned int* end);
        brook::Device* getExecDevices(unsigned int* count);

        virtual StreamImpl* clone();
        StreamImpl* execDomain(int numThreads);

        virtual void setType(const char* type);
        void setProperty(const char* name, void* value);

        virtual bool copy(StreamImpl* src, unsigned int* srcStartPoint, StreamImpl* dst,
                          unsigned int* dstStartPoint, unsigned int* dimension, bool completeCopy = true,
                          unsigned int srcBuf = 0, unsigned int dstBuf = 0);

        virtual void copyDataFrom(StreamImpl* src);
        virtual void copyDataTo(StreamImpl* dst);

        StreamImpl* getParent() const;
        unsigned int* getDomainStartPoint() const;
        unsigned int* getExecDimensions();
        inline void setExecDimensions(unsigned int* execDimension);

        inline const brook::BRformat getDataFormat(unsigned int i);
        Buffer* getBuffer(unsigned int i) const;
        void setBuffer(Buffer* value, unsigned int i);
        inline const unsigned int getBufferCount();

        virtual bool isAddressTranslated() const;
        inline BaseDevice* getDevice() const;
        inline const unsigned short getRank() const;
        inline unsigned int* getDimensions() const;
        virtual unsigned short getBufferRank() const;
        virtual unsigned int* getBufferDimensions() const;
        unsigned int getBufferPitch();

        void setData(void* streamPtr, const void* appPtr, unsigned int bufferNum,
                     unsigned int bufferWidth, unsigned int* startPoint,
                     unsigned int* dimension) const;
        void getData(void* streamPtr, void* appPtr, unsigned int bufferNum,
                     unsigned int bufferWidth, unsigned int* startPoint,
                     unsigned int* dimension) const;

        inline void setInitialized(bool value, unsigned int i);
        inline bool getInitialized(unsigned int i) const;

        bool isSync();
        bool flush();
        brook::BRerror getErrorCode();
        void setErrorCode(brook::BRerror result, bool force = false);
        const char* getErrorString();
        void setErrorString(const char*);

        inline bool getModifyFlag();
        void NotifyModification(StreamImpl* child);
        void ClearChild(StreamImpl* child);
        void copyDataFromTree();
        void copyDataToTree(bool syncParent = true, StreamImpl* childNotToSync = NULL);
        void removeParent();
        void removeChild(StreamImpl* child);

        void ref();
        void unref();

        virtual ~StreamImpl();

    protected:

        StreamImpl();
        unsigned short _getElementBytes() const;

    protected:

        //! \brief number of reference to this stream
        unsigned int _refCount;

        //! \brief Logical rank of the stream
        unsigned short _rank;

        //! \brief Dimensions of the stream
        unsigned int* _dimensions;

        //! \brief Data format of different buffers
        //! associated to this stream
        ::brook::BRformat* _format;

        //! \brief Buffer vector
        //! Represents all the Buffers associated to this stream
        //! More than one buffer will be associated if we use 
        //! stream of a struct
        Buffer** _buffer;

        //! Number of associated buffers
        unsigned int _bufferCount;

        //! \brief Associated device
        BaseDevice* _device;

        //! \brief devices on which we want to execute
        brook::Device* _execDevices;

        //! \brief execution device count
        unsigned int _execDeviceCount;

        //! \brief Parent stream pointer. Needed for domain implementation.
        // It points to the original stream.
        StreamImpl* _parent;

        //! \brief parent domain start point
        unsigned int* _domainStartPoint;

        //! \brief execution end dimensions
        unsigned int* _execDimension;

    private:

        void _setMaxErrors();

    private:

        //! brief stream type - d3d9 d3d10
        const char* _type;

        //! brief A flag to specify whether stream was modified
        //! The flag is set during streamRead or if stream is output of a kernel
        bool _isModified;

        //! brief A flag to specify whether stream was has asked for buffer creation
        bool* _isInitialized;

        //! \brief Enum showing Error code
        brook::BRerror _error;

        //! \brief string containing error stack specific to stream
        std::string* _errorString;

        //! \brief string containing error stack specific to stream
        std::string* _tempErrorString;

        //! \brief Error set on the stream before calling ErrorLog()
        unsigned int _errorCount;

        unsigned int _maxErrors;

        //! \brief all the children of the stream
        // Second data contains information whether this child is being called from the same kernel
        std::map<StreamImpl*, bool>* _children;
};

inline
BaseDevice*
StreamImpl::getDevice() const
{
    return _device;
}

inline const
unsigned short
StreamImpl::getRank() const
{
    return _rank;
}

inline
unsigned int*
StreamImpl::getDimensions() const
{
    return _dimensions;
}

inline
StreamImpl*
StreamImpl::getParent() const
{
    return _parent;
}

inline
unsigned int*
StreamImpl::getDomainStartPoint() const
{
    return _domainStartPoint;
}

inline const
brook::BRformat
StreamImpl::getDataFormat(unsigned int i)
{
    return _format[i];
}

inline const
unsigned int
StreamImpl::getBufferCount()
{
    return _bufferCount;
}

inline
void
StreamImpl::setExecDimensions(unsigned int* execDimension)
{
    _execDimension = execDimension;
}

inline
bool
StreamImpl::getModifyFlag()
{
    return _isModified;
}

inline
void
StreamImpl::setInitialized(bool value, unsigned int i)
{
    _isInitialized[i] = value;
}

inline
bool
StreamImpl::getInitialized(unsigned int i) const
{
    return _isInitialized[i];
}

#endif //_STREAMIMPL_H_
