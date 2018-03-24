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

#include "CALBase.h"
#include "CALBuffer.h"
#include "CALDevice.h"
#include "Runtime.h"
#include "BufferMgr.h"

ThreadLock CALBuffer::_resLock;

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
//! \param rank Rank of the Buffer
//! \param dimensions dimension os the Buffer
//! \param format Used CALformat for the buffer
//! \param bufferPool Location of the Buffer
//! \param device Device associated to CALBuffer
//!
////////////////////////////////////////////////////////////////////////////////

CALBuffer::CALBuffer(unsigned short rank, unsigned int* dimensions, 
                     CALformat format, BufferPool bufferPool, CALuint flag,
                     BaseDevice* device)
                     : Buffer(rank, dimensions, device), _type(NULL),
                        _dataFormat(format), _res(0),
                        _mem(0), _bufferPool(bufferPool), _flag(flag), _pitch(0),
                        _copyEvent(NULL), _inputEvent(NULL), _writeEvent(NULL), _outputEvent(NULL),
                        _writeSync(SYNC_NO_THREAD), _writeThread(NULL), _syncCount(0)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method where actual Buffer allocation happens on GPU 
//!
//! \return flag indicating whether Buffer allocation was successful or not
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::initialize()
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdevice calDevice = device->getDevice();
    CALresult result;

    _resLock.lock();

    if(1 == _rank)
    {
        switch(_bufferPool)
        {
            case BUFFER_LOCAL:
                result = calResAllocLocal1D(&_res, calDevice, _dimensions[0], _dataFormat, _flag);
                break;
            case BUFFER_HOST:
                result = calResAllocRemote1D(&_res, &calDevice, 1, _dimensions[0], 
                                                _dataFormat, _flag);
                break;
        }
    }
    else if(2 == _rank)
    {
        switch(_bufferPool)
        {
            case BUFFER_LOCAL:
                result = calResAllocLocal2D(&_res, calDevice, _dimensions[0], _dimensions[1], _dataFormat, _flag);
                break;
            case BUFFER_HOST:
                result = calResAllocRemote2D(&_res, &calDevice, 1, _dimensions[0], _dimensions[1], 
                                                _dataFormat, _flag);
                break;
        }
    }

    _resLock.unlock();
    CHECK_CAL_RESULT(result, "Failed to allocate resource \n");

    // Get the memory handle
    result = calCtxGetMem(&_mem, device->getContext(), _res);
    CHECK_CAL_RESULT(result, "Failed to get memory handle \n");

    _initialized = true;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method where actual allocation happens on GPU for memory pinned buffer
//!
//! \return flag indicating whether Buffer allocation was successful or not
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::initializePinnedBuffer(const void* cpuPtr, void* funcPtr)
{
    if(funcPtr == NULL)
    {
        return false;
    }

    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdevice calDevice = device->getDevice();
    CALresult result;

    PFNCALRESCREATE2D calResCreate2D;
    PFNCALRESCREATE1D calResCreate1D;
    
    if(calExtSupported((CALextid)CAL_EXT_RES_CREATE) == CAL_RESULT_OK)
    {
        _resLock.lock();
        switch(_rank)
        {
            case 1:
            {
                unsigned int bytes = _dimensions[0] * getElementBytes();
                
                calResCreate1D = (PFNCALRESCREATE1D)funcPtr;
                result = calResCreate1D(&(_res), calDevice,(CALvoid*)cpuPtr, _dimensions[0], _dataFormat, bytes, 0);
                CHECK_CAL_RESULT(result, "Failed to allocate 1D pinned resource \n");
                break;
            }
            case 2:
            {
                unsigned int bytes = _dimensions[0] * _dimensions[1] * getElementBytes();
                
                calResCreate2D = (PFNCALRESCREATE2D)funcPtr;
                result = calResCreate2D(&(_res), calDevice,(CALvoid*)cpuPtr, _dimensions[0], _dimensions[1],
                                        _dataFormat, bytes, 0);
                CHECK_CAL_RESULT(result, "Failed to allocate 2D pinned resource \n");
                break;
            }
        }
        CHECK_CAL_RESULT(result, "Failed to allocate pinned resource \n");

        // Get the memory handle
        result = calCtxGetMem(&_mem, device->getContext(), _res);
        CHECK_CAL_RESULT(result, "Failed to get memory handle \n");

        _initialized = true;

        _resLock.unlock();

        return true;
    }

    return false;
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set property on buffer required for interoperable buffers
//!
////////////////////////////////////////////////////////////////////////////////

const char*
CALBuffer::setProperty(const char* name, void* value)
{
    return "This is not an interoperable buffer";
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Are asynchronous operations done on stream
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::isSync()
{
    if(!isCopyEventSync() || !isInputEventSync() || !isOutputEventSync() || 
        !isWriteThreadSync() || !isWriteEventSync())
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Flush all the events associated to the buffer
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::flush()
{
    waitCopyEvent();
    waitInputEvent();
    waitOutputEvent();
    waitWriteThread();
    waitWriteEvent();

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Check the equality of two buffers
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::_isEqual(const Buffer& other) const
{
    // Is base class properties same?
    bool flag = Buffer::_isEqual(other);

    // Check for data format equality if base class properties are same
    if(flag)
    {
        const CALBuffer& buffer = static_cast<const CALBuffer&>(other);

        return (_dataFormat == buffer.getFormat());
    }

    return flag;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Maps Buffer to a CPU addressible pointer
//!
//! \param pitch Pitch of the mapped memory
//! \return Pointer to the mapped memory
//!
////////////////////////////////////////////////////////////////////////////////

void*
CALBuffer::getBufferPointerCPU(CALuint& pitch)
{
    void* bufferPtr;
    CALresult result = calResMap(&bufferPtr, &pitch, _res, 0);

    if(result != CAL_RESULT_OK)
    {
        SET_ERROR("Failed to map resource \n");
        return NULL;
    }

    return bufferPtr;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Frees mapped memory
//!
//! \param buffer CALBuffer to be unmapped
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::freeBufferPointerCPU()
{
    CALresult result = calResUnmap(_res);
    CAL_RESULT_LOG(result, "Failed to unmap resource \n");
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get Pitch. CAL buffers allocated width is usually power of 2.
//!
////////////////////////////////////////////////////////////////////////////////

unsigned int
CALBuffer::getPitch()
{
    if(!_pitch)
    {
        // The only way to get pitch is by mapping resouce to cpu pointer
        // This is costly, so we have cached the result in _pitch
        getBufferPointerCPU(_pitch);
        freeBufferPointerCPU();
    }

    return _pitch;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Function to copy data from srcBuffer to this Buffer.
//!
//! It is implemented using DMA
//!
//! \param srcBuffer Data to copy from
//! \param event event associated
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::copyAsync(CALBuffer* srcBuffer, CALevent* event) const
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALcontext ctx = device->getContext();
    CALresult result = calMemCopy(event, ctx, 
                                    srcBuffer->getMemHandle(), 
                                    this->getMemHandle(), 0);

    CAL_RESULT_ERROR(result, "Failed to copy data \n");

    calCtxIsEventDone(ctx, *event);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Decrease the ref count
//! Ask BufferMgr to destroy if refCount == 0 only if Buffer is local
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::unref()
{
    assert(_refCount > 0);

    if (--_refCount == 0)
    {
        waitInputEvent();
        waitOutputEvent();
        waitWriteEvent();
        waitWriteThread();

        // That bufferMap is only for local resource
        if(_bufferPool == BUFFER_LOCAL)
        {
            _device->getBufferMgr()->destroyBuffer(this);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALBuffer::~CALBuffer()
{
    CALDevice* device = static_cast<CALDevice*>(_device);

    // Destroy resource and release mem handle
    if(_mem)
    {
        calCtxReleaseMem(device->getContext(), _mem);
    }

    if(_res)
    {
        calResFree(_res);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Is copy event done
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::isCopyEventSync()
{
    if(_copyEvent)
    {
        if(*_copyEvent != 0)
        {
            CALDevice* device = static_cast<CALDevice*>(_device);
            if(calCtxIsEventDone(device->getContext(), *_copyEvent))
            {
                return false;
            }
            *_copyEvent = 0;
            _copyEvent = NULL;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Is input event done
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::isInputEventSync()
{
    if(_inputEvent)
    {
        if(*_inputEvent != 0)
        {
            CALDevice* device = static_cast<CALDevice*>(_device);
            if(calCtxIsEventDone(device->getContext(), *_inputEvent))
            {
                return false;
            }
            *_inputEvent = 0;
            _inputEvent = NULL;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Is write event done
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::isWriteEventSync()
{
    if(_writeEvent)
    {
        if(*_writeEvent != 0)
        {
            CALDevice* device = static_cast<CALDevice*>(_device);
            if(calCtxIsEventDone(device->getContext(), *_writeEvent))
            {
                return false;
            }
            *_writeEvent = 0;
            _writeEvent = NULL;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Is output event done
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::isOutputEventSync()
{
    if(_outputEvent)
    {
        if(*_outputEvent != 0)
        {
            CALDevice* device = static_cast<CALDevice*>(_device);
            if(calCtxIsEventDone(device->getContext(), *_outputEvent))
            {
                return false;
            }
            *_outputEvent = 0;
            _outputEvent = NULL;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Is write thread done
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALBuffer::isWriteThreadSync()
{

#ifdef _WIN32
        // XXX- ghost slowdowns - wait for 1 ms to make sur we switch to this thread
        if(_writeThread && _writeThread->getID())
        {
            WaitForSingleObject((HANDLE)_writeThread->getID(), 1);
        }
#endif

    if(_writeSync == SYNC_THREAD_STARTED)
    {
        return false;
    }
    else if(_writeSync == SYNC_THREAD_END)
    {
        if(_writeThread->getID())
        {
            _writeThread->join();
            _writeThread = NULL;
        }

        _syncCount = 0;
        _writeSync = SYNC_NO_THREAD;
    }
    else if(_writeSync == SYNC_NO_THREAD)
    {
        // This is the case when thread start-up is taking too mach time
        if(_writeThread)
        {
            // Wait for 20 such calls, and then join the thread
            if(++_syncCount > 20)
            {
                if(_writeThread->getID())
                {
                    _writeThread->join();
                    _writeThread = NULL;
                }

                _syncCount = 0;
                _writeSync = SYNC_NO_THREAD;
            }
            else
            {
                return false;
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Wait for the write event to finish
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::waitWriteEvent()
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    if(_writeEvent)
    {
        if(*_writeEvent != 0)
        {
            while(calCtxIsEventDone(device->getContext(), *_writeEvent))
                ;
            *_writeEvent = 0;
            _writeEvent = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Wait for the copy event to finish
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::waitCopyEvent()
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    if(_copyEvent)
    {
        if(*_copyEvent != 0)
        {
            while(calCtxIsEventDone(device->getContext(), *_copyEvent))
                ;
            *_copyEvent = 0;
            _copyEvent = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Wait for the input event to finish
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::waitInputEvent()
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    if(_inputEvent)
    {
        if(*_inputEvent != 0)
        {
            while(calCtxIsEventDone(device->getContext(), *_inputEvent))
                ;
            *_inputEvent = 0;
            _inputEvent = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Wait for the output event to finish
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::waitOutputEvent()
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    if(_outputEvent)
    {
        if(*_outputEvent != 0)
        {
            while(calCtxIsEventDone(device->getContext(), *_outputEvent))
                ;
            *_outputEvent = 0;
            _outputEvent = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Wait for the write thread to finish
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::waitWriteThread()
{
    if(_writeThread)
    {
        if(_writeThread->getID())
        {
            _writeThread->join();
            _writeThread = NULL;
        }

        _syncCount = 0;
        _writeSync = SYNC_NO_THREAD;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief set copy event value
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::setCopyEvent(CALevent* value)
{
    _copyEvent = value;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief set input event value
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::setInputEvent(CALevent* value)
{
    _inputEvent = value;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief set write event value
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::setWriteEvent(CALevent* value)
{
    _writeEvent = value;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief set output event value
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::setOutputEvent(CALevent* value)
{
    _outputEvent = value;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief set write thread
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::setWriteThread(CPUThread* value)
{
    _writeThread = value;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief set condition vaiable to query if thread work is done
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBuffer::setWriteSyncStatus(SyncStatus value)
{
    _writeSync = value;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Returns bytes in one element
//!
////////////////////////////////////////////////////////////////////////////////

unsigned short 
CALBuffer::getElementBytes()const
{
    unsigned short numComponents = 0;
    unsigned short bytes = 0;

    switch(_dataFormat)
    {
        case CAL_FORMAT_UNSIGNED_INT8_1:
        case CAL_FORMAT_SIGNED_INT8_1:
            numComponents = 1;
            bytes = sizeof(char);
            break;

        case CAL_FORMAT_UNSIGNED_INT8_2:
        case CAL_FORMAT_SIGNED_INT8_2:
            numComponents = 2;
            bytes = sizeof(char);
            break;

        case CAL_FORMAT_UNSIGNED_INT8_4:
        case CAL_FORMAT_SIGNED_INT8_4:
            numComponents = 4;
            bytes = sizeof(char);
            break;
        
        case CAL_FORMAT_UNSIGNED_INT16_1:
        case CAL_FORMAT_SIGNED_INT16_1:
            numComponents = 1;
            bytes = sizeof(short int);
            break;

        case CAL_FORMAT_UNSIGNED_INT16_2:
        case CAL_FORMAT_SIGNED_INT16_2:
            numComponents = 2;
            bytes = sizeof(short int);
            break;

        case CAL_FORMAT_UNSIGNED_INT16_4:
        case CAL_FORMAT_SIGNED_INT16_4:
            numComponents = 4;
            bytes = sizeof(short int);
            break;

        case CAL_FORMAT_FLOAT_1:
            numComponents = 1;
            bytes = sizeof(float);
            break;

        case CAL_FORMAT_UINT_1:
        case CAL_FORMAT_INT_1:
            numComponents = 1;
            bytes = sizeof(int);
            break;

        case CAL_FORMAT_DOUBLE_1:
            numComponents = 1;
            bytes = sizeof(double);
            break;

        case CAL_FORMAT_FLOAT_2:
            numComponents = 2;
            bytes = sizeof(float);
            break;

        case CAL_FORMAT_UINT_2:
        case CAL_FORMAT_INT_2:
            numComponents = 2;
            bytes = sizeof(int);
            break;

        case CAL_FORMAT_DOUBLE_2:
            numComponents = 2;
            bytes = sizeof(double);
            break;

        case CAL_FORMAT_FLOAT_4:
            numComponents = 4;
            bytes = sizeof(float);
            break;

        case CAL_FORMAT_UINT_4:
        case CAL_FORMAT_INT_4:
            numComponents = 4;
            bytes = sizeof(int);
            break;

        default:
            numComponents = 0;
            bytes = 0;
    }

    return numComponents * bytes;
}
