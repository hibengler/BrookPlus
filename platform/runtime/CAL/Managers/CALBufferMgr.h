#ifndef _CALBUFFERMGR_H_
#define _CALBUFFERMGR_H_

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
//! \file CALBufferMgr.h
//!
//! \brief Contains the declaration of CALBufferMgr class
//!
////////////////////////////////////////////////////////////////////////////////

#include "cal.h"
#include "cal_ext.h"
#include "BufferMgr.h"
#include "StreamImpl.h"
#include "CPUThread.h"

#include <vector>

class CALBuffer;
class CALConstBuffer;
class CALDevice;

using namespace brook;

// I don't want to overload program with too many threads
#define MaxThreads 4

struct WriteData
{
    CALBuffer* srcBuffer;
    CALBuffer* dstBuffer;
    CALevent* copyEvent;
    StreamImpl* stream;
    void* ptr;
    CALcontext context;
};

class ThreadWriteData
{
    public:
        ThreadWriteData()
        {
            thread = new CPUThread;
            data = new WriteData;
        }

        ~ThreadWriteData()
        {
            delete thread;
            delete data;
        }

        CPUThread* thread;
        WriteData* data;
};

////////////////////////////////////////////////////////////////////////////////
//!
//! \class CALBufferMgr
//!
//! \brief Backend specific implementation of memory management.
//! 
//! Contains various optimization and logic for CAL parity for data transfer 
//! and host buffer allocation/de-allocation.
//! 
//! Contains a cache of host resources. Try to use these host resources to-
//! 1. Get asynchronous behavior in streamRead. 
//! 2. Avoid allocation/de-allocation and
//! 3. Avoid an error if host resource memory is full.
//! 
/////////////////////////////////////////////////////////////////////////////////

class CALBufferMgr : public BufferMgr
{
    public:

        explicit CALBufferMgr(CALDevice* device);
        Buffer* createTypedBuffer(const char* type, StreamImpl* stream);
        CALConstBuffer* createConstBuffer(unsigned int numConstants, 
                                          BRformat format = BR_FORMAT_UINT_4);
        void clearUsedConstCache();

        void setBufferData(StreamImpl* stream, const void* ptr, const char* flags = NULL);
        void getBufferData(StreamImpl* stream, void* ptr, const char* flags);
        void copyBufferData(Buffer* dst, Buffer* src);
        CALformat getCALFormat(BRformat format) const;

        static void* writeBufferData(void* args);
        const brook::KernelDesc* getCopyShaderDesc();
        const brook::KernelDesc* getResizeShaderDesc();

        ~CALBufferMgr();

    protected:

        Buffer* _createBuffer(StreamImpl* stream, unsigned int i);

    private:
        void* _getPinnedFunctionPointer(unsigned int rank);

        CALBuffer* _createHostBuffer(unsigned short rank, unsigned int* dimensions, 
                                     const CALformat & format);

        CALBuffer* _createPCIeHostBuffer(unsigned short rank, unsigned int* dimensions, 
                                           const CALformat & format);

        CALBuffer* _createPinnedBuffer(const void* cpuPtr, unsigned short rank, unsigned int* dimensions,
                                       const CALformat &format, void* funcPtr);

        CALevent* _getCopyEvent();
        ThreadWriteData* _getThreadWriteData();

  private:

        //! \brief Array of copy events.
        //! The same event adddress is used for temporary host resouce and 
        //! local resource during data transfer.
        ThreadWriteData* _writeData[MaxThreads];

        //! \brief Array of copy events.
        //! The same event adddress is used for temporary host resouce and 
        //! local resource during data transfer.
        std::vector<CALevent*> _copyEvents;

        //! \brief Cache for host memory data management.
        std::vector<CALBuffer*> _hostBufferCache;

        //! \brief Cache for PCIe host memory data management.
        std::vector<CALBuffer*> _hostPCIeBufferCache;

        //! \brief Cache for host memory data management.
        std::vector<CALBuffer*> _pinnedBufferCache;

        //! \brief Cache containg constant buffers
        std::vector<CALConstBuffer*> _constBufferCache;

        //! \brief constant buffers being used in the the same kernel
        std::vector<CALConstBuffer*> _usedConstBuffers;

        //! \brief function pointer for memory pinned 2D resources
        PFNCALRESCREATE2D _calResCreate2D;

        //! \brief function pointer for memory pinned 1D resources
        PFNCALRESCREATE1D _calResCreate1D;

        //! \brief Kernel descriptor to generate resized stream.
        static const brook::KernelDesc* _resize_shader_desc;

        //! \brief Kernel descriptor for copy shaders.
        static const brook::KernelDesc* _copy_shader_desc;

        //! \brief Enable DMA
        bool _enableDMA;
};

#endif //_CALBUFFERMGR_H_
