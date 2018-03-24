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

#include "CommonDefs.h"
#include "CALKernelImpl.h"
#include "CALBufferMgr.h"
#include "StreamImpl.h"
#include "CALBuffer.h"
#include "Utility.h"
#include "CALDevice.h"
#include "Runtime.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

CALKernelImpl::CALKernelImpl(const KernelDesc* desc, BaseDevice* device)
                : KernelImpl(desc, device)
{

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Execute the given pass.Used only for scatter
//!
////////////////////////////////////////////////////////////////////////////////

void
CALKernelImpl::_executePass(Pass& pass, ArgumentInfo* arguments)
{
    // check do we have any scatter stream
    StreamImpl* scatterStream = NULL;
    ArgDesc* argOutputs = pass.getData()->Outputs;
    unsigned int scatterIndex = argOutputs->operator [](0).Index;

    if(pass.getData()->computeFlag)
    {
        scatterStream = _parameters.outputArguments[scatterIndex];
    }
    else
    {
        for(unsigned int iter = 0; iter < argOutputs->size(); ++iter)
        {
            if(argOutputs->operator [](iter).Type == ACCESS_RANDOM)
            {
                scatterIndex = argOutputs->operator [](iter).Index;
                scatterStream = _parameters.outputArguments[scatterIndex];
                break;
            }
        }
    }

    //If pass doesn't contain any scatter and shader type is not compute shader,
    //we need not to go through this CAL hack.
    if(!scatterStream)
    {
        KernelImpl::_executePass(pass, arguments);
        return;
    }

    StreamMap output = _parameters.outputArguments;
    // Check if scatter is supported on the underlying hardware
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdeviceattribs attrib = device->getAttribs();

    if(!attrib.memExport)
    {
        //Set Error on all the output streams
        StreamMap::iterator iterOut;
        for(iterOut = output.begin(); iterOut != output.end(); ++iterOut)
        {
            iterOut->second->setErrorCode(BR_ERROR_NOT_SUPPORTED);
            iterOut->second->setErrorString("Kernel Execution : Scatter is not supported on underlying \
                                             hardware\n");
        }

        return;
    }

    if(pass.getData()->computeFlag && !attrib.computeShader)
    {
        //Set Error on all the output streams
        StreamMap::iterator iterOut;
        for(iterOut = output.begin(); iterOut != output.end(); ++iterOut)
        {
            iterOut->second->setErrorCode(BR_ERROR_NOT_SUPPORTED);
            iterOut->second->setErrorString("Kernel Execution : \
                                            Compute Shader is not supported on underlying hardware\n");
        }

        return;
    }

#ifdef ENABLE_COMPUTE_SHADER

    KernelImpl::_executePass(pass, arguments);
    return;

#endif

    unsigned int rank = 0;
    rank = scatterStream->getBufferRank();

    // We need not to create 1D stream with GLOBAL BUFFER flag.
    // Virtualization of global buffer < 128 bit data type needs a temporary buffer of 128-bit.
    // If we are already allocating 128-bit type no need to create temporary
    BRformat brFormat = scatterStream->getDataFormat(0);
    BRformat supFormat = _getSupportedFormat(brFormat);
    if(rank < 2 && supFormat == brFormat)
    {
        KernelImpl::_executePass(pass, arguments);
        return;
    }

    // Create a new stream. Create a non-AT stream irrespective of the type of 
    // original stream.
    //It will make copy faster from temporary to original stream

    unsigned int* dimensions = scatterStream->getBufferDimensions();
    StreamImpl* tempStream = new StreamImpl(rank, dimensions, &brFormat, 1, _device);

    // Create a new temporary buffer with linear memory
    CALBufferMgr* bufferMgr = static_cast<CALBufferMgr*>(_device->getBufferMgr());
    CALformat format = bufferMgr->getCALFormat(supFormat);
    CALBuffer* tempBuffer = new CALBuffer(rank, dimensions, format, BUFFER_LOCAL,
                                          CAL_RESALLOC_GLOBAL_BUFFER, _device);

    if(!tempBuffer->initialize())
    {
        delete tempStream;
        delete tempBuffer;

        StreamMap::iterator iterOut;
        for(iterOut = output.begin(); iterOut != output.end(); ++iterOut)
        {
            iterOut->second->setErrorCode(BR_ERROR_KERNEL);
            iterOut->second->setErrorString("Kernel Execution : Failed to create temporary linear stream\n");
        }

        return;
    }
    tempStream->setBuffer(tempBuffer, 0);

    // If any changes has been made on the stream, copy it to temprary stream
    if(scatterStream->getModifyFlag())
    {
        // Copy data to temporary stream
        unsigned int startPoint[MaxRank];
        memset(startPoint, 0, MaxRank * sizeof(unsigned int));
        bool success = tempStream->copy(scatterStream, startPoint, tempStream, startPoint, dimensions);
        if(!success)
        {
            scatterStream->setErrorCode(BR_ERROR_KERNEL);
            scatterStream->setErrorString("Kernel Execution : Failed to copy original data to temporary stream\n");
        }
    }

    // Set scatter stream with new temporary buffer
    Buffer* orgBuffer = scatterStream->getBuffer(0);
    scatterStream->setBuffer(tempBuffer, 0);

    KernelImpl::_executePass(pass, arguments);

    // Revert to original buffer
    scatterStream->setBuffer(orgBuffer, 0);

    // Copy back the data
    unsigned int startPoint[MaxRank];
    memset(startPoint, 0, MaxRank * sizeof(unsigned int));
    bool success = tempStream->copy(tempStream, startPoint, scatterStream, startPoint, dimensions);
    if(!success)
    {
        scatterStream->setErrorCode(BR_ERROR_KERNEL);
        scatterStream->setErrorString("Kernel Execution : Failed to copy data from temporary stream\n");
    }

    tempStream->setBuffer(NULL, 0);
    delete tempBuffer;
    delete tempStream;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Scatter implementation needs 128-bit formats.
//! See if the given format is supported or not
//!
////////////////////////////////////////////////////////////////////////////////

BRformat
CALKernelImpl::_getSupportedFormat(BRformat format)
{
    BRformat supFormat = format;
    switch(format)
    {
        case BR_FORMAT_UCHAR_1:
        case BR_FORMAT_UCHAR_2:
        case BR_FORMAT_UCHAR_3:
        case BR_FORMAT_UCHAR_4:
            supFormat = BR_FORMAT_UINT_4;
            break;

        case BR_FORMAT_CHAR_1:
        case BR_FORMAT_CHAR_2:
        case BR_FORMAT_CHAR_3:
        case BR_FORMAT_CHAR_4:
            supFormat = BR_FORMAT_INT_4;
            break;

        case BR_FORMAT_USHORT_1:
        case BR_FORMAT_USHORT_2:
        case BR_FORMAT_USHORT_3:
        case BR_FORMAT_USHORT_4:
            supFormat = BR_FORMAT_UINT_4;
            break;

        case BR_FORMAT_SHORT_1:
        case BR_FORMAT_SHORT_2:
        case BR_FORMAT_SHORT_3:
        case BR_FORMAT_SHORT_4:
            supFormat = BR_FORMAT_INT_4;
            break;

        case BR_FORMAT_FLOAT_1:
        case BR_FORMAT_FLOAT_2:
            supFormat = BR_FORMAT_FLOAT_4;
            break;

        case BR_FORMAT_INT_1:
        case BR_FORMAT_INT_2:
            supFormat = BR_FORMAT_INT_4;
            break;

        case BR_FORMAT_UINT_1:
        case BR_FORMAT_UINT_2:
            supFormat = BR_FORMAT_UINT_4;
            break;

        case BR_FORMAT_DOUBLE_1:
            supFormat = BR_FORMAT_DOUBLE_2;
            break;
    }

    return supFormat;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALKernelImpl::~CALKernelImpl()
{
}

