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
#include "ConstImpl.h"
#include "Runtime.h"
#include "CALExecMgr.h"
#include "CALDevice.h"
#include "CALProgram.h"
#include "KernelDesc.h"
#include "CALBufferMgr.h"


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

CALExecMgr::CALExecMgr(CALDevice* device): ExecMgr(device)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to execute a given Pass and with given arguments
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALExecMgr::executePass(Pass& pass, ArgInfo* arguments, bool forceFlush)
{
    CALDevice* device = static_cast<CALDevice*>(_device);

    StreamVec inputArguments = arguments->inputs;
    StreamVec outputArguments = arguments->outputs;

    // Create the program with the given pass
    Program* program = _getProgram(pass);

    if(program == NULL)
    {
        for(unsigned int i = 0; i < outputArguments.size(); ++i)
        {
            StreamImpl* stream = outputArguments[i];
            stream->setErrorCode(BR_ERROR_KERNEL);
            stream->setErrorString("Kernel Execution : Failed to create Program");
        }

        return false;
    }

    CALProgram* calPrgm = static_cast<CALProgram*>(program);

    ArgDesc* outputDesc = NULL;
    outputDesc = pass.getData()->Outputs;

    // Wait for pending events.
    _waitCopyEvent(pass.getData()->Inputs, inputArguments);
    _waitOutputEvents(pass.getData()->Inputs, inputArguments);

    _waitCopyEvent(outputDesc, outputArguments);
    _waitInputEvents(outputDesc, outputArguments);
    _waitOutputEvents(outputDesc, outputArguments);

    ConstVec constArguments = arguments->constants;
    CALConstBuffer** constBuffer = NULL;
    unsigned int constBufferCount = 0;

    // Create a constant buffer and bind constsnt
    if(!_allocateConstant(calPrgm, constArguments, constBuffer, constBufferCount))
    {
        for(unsigned int i = 0; i < outputArguments.size(); ++i)
        {
            StreamImpl* stream = outputArguments[i];
            stream->setErrorCode(BR_ERROR_KERNEL);
            stream->setErrorString("Kernel Execution : Failed to create Constants");
        }

        return false;
    }

    // Bind input and output memory - symbol handles
    if(!_bindConstant(constBufferCount, calPrgm, constBuffer))
    {
        for(unsigned int i = 0; i < outputArguments.size(); ++i)
        {
            StreamImpl* stream = outputArguments[i];
            stream->setErrorCode(BR_ERROR_KERNEL);
            stream->setErrorString("Kernel Execution : Failed to bind constant buffer\n");
        }

        return false;
    }

    // Bind input and output memory - symbol handles
    if(!_bindInput(pass.getData()->Inputs, calPrgm, inputArguments))
    {
        for(unsigned int i = 0; i < outputArguments.size(); ++i)
        {
            StreamImpl* stream = outputArguments[i];
            stream->setErrorCode(BR_ERROR_KERNEL);
            stream->setErrorString("Kernel Execution : Error with input streams\n");
        }

        return false;
    }

    if(!_bindOutput(outputDesc, calPrgm, outputArguments))
    {
        for(unsigned int i = 0; i < outputArguments.size(); ++i)
        {
            StreamImpl* stream = outputArguments[i];
            stream->setErrorCode(BR_ERROR_KERNEL);
            stream->setErrorString("Kernel Execution : Error with output streams\n");
        }

        return false;
    }

    // Get Exectuion event
    CALevent* execEvent = _getExecEvent();
    CALcontext ctx = device->getContext();
    CALfunc func = calPrgm->getFunction();

    // Set the execution domain
    unsigned int* startExecDomain = arguments->startDomain;
    unsigned int* domainDimensions = arguments->domainDimension;

    CALresult result;

    // Run the kernel on GPU
    if(pass.getData()->computeFlag)
    {
        CALdomain3D rect;
        CALprogramGrid pg;
        unsigned int* groupSize = arguments->groupSize;

        if(groupSize == NULL)
        {
            StreamImpl* stream = outputArguments[0];
            stream->setErrorCode(BR_ERROR_KERNEL);
            stream->setErrorString("Kernel Execution : group size null for compute shader\n");
            
            return false;
        }

        rect.width = domainDimensions[0];
        rect.height = domainDimensions[1];
        rect.depth = 1;
        
        pg.func             = func;
        pg.flags            = 0;
        pg.gridBlock.width  = groupSize[0]; //needs to be same value as what is in the kernal for thread group size.
        pg.gridBlock.height = 1;
        pg.gridBlock.depth  = 1;
        pg.gridSize.width   = (rect.width * rect.height + pg.gridBlock.width - 1) / pg.gridBlock.width;
        pg.gridSize.height  = 1;
        pg.gridSize.depth   = 1;

        result = calCtxRunProgramGrid(execEvent, ctx, &pg);
    }
    else
    {
        CALdomain rect = {startExecDomain[0], startExecDomain[1], 
                        domainDimensions[0], domainDimensions[1]};
        result = calCtxRunProgram(execEvent, ctx, func, &rect);
    }
    
    if(result != CAL_RESULT_OK)
    {
        for(unsigned int i = 0; i < outputArguments.size(); ++i)
        {
            StreamImpl* stream = outputArguments[i];
            stream->setErrorCode(BR_ERROR_KERNEL);
            stream->setErrorString("Kernel Execution : Failed to run kernel\n");
        }

        return false;
    }

    calCtxIsEventDone(ctx, *execEvent);

    if(forceFlush)
    {
        while(calCtxIsEventDone(ctx, *execEvent));
    }

    // Set all the events
    _setConstEvent(constBufferCount, execEvent, constBuffer);
    _setInputEvent(pass.getData()->Inputs, execEvent, inputArguments);
    _setOutputEvent(outputDesc, execEvent, outputArguments);

    delete[] constBuffer;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create CALProgram.
//!
////////////////////////////////////////////////////////////////////////////////

Program* 
CALExecMgr::_createProgram(Pass& pass)
{
    CALProgram* program = new CALProgram(&pass, _device);

    return program;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Allocate constant buffers
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALExecMgr::_allocateConstant(CALProgram* program, ConstVec constArguments,
                                  CALConstBuffer**& buffer, unsigned int& bufferCount) const
{
    const Pass* pass = program->getPass();
    ArgDesc* constArrays = pass->getData()->ConstArrays;
    ArgDesc* constants = pass->getData()->Constants;

    unsigned int constArrayCount = (unsigned int)constArrays->size();
    unsigned int constCount = (unsigned int)constants->size();

    // Total number of buffers is constant arrays
    bufferCount = constArrayCount;

    // All other constants are combined into single buffer
    if(constCount > 0)
    {
        bufferCount += 1;
    }

    if(bufferCount == 0)
    {
        return true;
    }

    CALBufferMgr* bufMgr = 
        static_cast<CALBufferMgr*>(_device->getBufferMgr());
    bufMgr->clearUsedConstCache();

    buffer = new CALConstBuffer*[bufferCount];
    unsigned int i = 0;
    for(i = 0; i < constArrayCount; ++i)
    {
        // Ask BufferMgr to create constantbuffer of given size

        // WAR - CAL requires 128 bit allocation for constant buffers
        // irrespective of format of constant array
        // We have used 128-bit format for constant buffer allocation and the data is copied 
        // from a float constant array to GPU buffer sucth that - 
        // *(gpuBuffer + 4 * i) = *(constArray + i)
        buffer[i] = bufMgr->createConstBuffer(constArguments[i]->getElementCount());

        if(buffer[i] == NULL)
        {
            return false;
        }

        // Push data from ConstImpl to Const Buffer
        buffer[i]->pushConstant(constArguments[i]->getData(), constArguments[i]->getFormat(),
                                constArguments[i]->getElementCount());

        // Transfer data to Constant buffer
        if(!buffer[i]->setDataToBuffer())
        {
            return false;
        }
    }

    if(constCount > 0)
    {
        // Ask BufferMgr to create constantbuffer of given size
        buffer[i] = bufMgr->createConstBuffer(constCount);

        if(buffer[i] == NULL)
        {
            return false;
        }

        // Push all the data from ConstImpl to Const Buffer
        for(unsigned int j = 0; j < constCount; ++j)
        {
            buffer[i]->pushConstant(constArguments[j + i]->getData(),
                                    constArguments[j + i]->getFormat());
        }

        // Transfer data to Constant buffer
        if(!buffer[i]->setDataToBuffer())
        {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief bind cosntants memory handle and  synbol handle
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALExecMgr::_bindConstant(unsigned int bufferCount, CALProgram* program, 
                          CALConstBuffer** buffer) const
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALcontext ctx = device->getContext();

    for(unsigned int i = 0; i < bufferCount; ++i)
    {
        CALname name = program->getConstName(i);
        CALmem mem = buffer[i]->getMemHandle();

        CALresult result = calCtxSetMem(ctx, name, mem);
        CAL_RESULT_ERROR(result, "Failed to bind constant resource\n");
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief bind input memory handle and  synbol handle
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALExecMgr::_bindInput(ArgDesc* argDescs, CALProgram* program,
                       StreamVec inputArguments) const
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALcontext ctx = device->getContext();

#ifdef _USE_CACHE_
    // Get the previous input arguments from the iocache
    ExecutionMap::const_iterator execItr = _ioCache.find(program);

    StreamArguments* arg = execItr->second;
    std::vector<StreamImpl*>& inputs = arg->inputs;
#endif //_USE_CACHE_

    StreamVec::iterator itr = inputArguments.begin();
    ArgDesc::iterator descItr = argDescs->begin();

    int i = 0;

    for (; itr != inputArguments.end(); ++itr, ++i, ++descItr)
    {
        ArgumentDesc desc = *descItr;
        StreamImpl* stream = *itr;
        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(desc.Usage));
        if(buffer != NULL)
        {
#ifdef _UAE_CACHE_
            //Check if it is getting called first time
            if(inputs.size() == inputArguments.size())
            {
                //Don't bind if was already bound from the same buffer
                if(inputs[i] == stream)
                {
                    continue;
                }
                else
                {
                    // If it is being called with another parameter 
                    // put the new parameter in the cache
                    inputs[i] = stream;
                }
            }
#endif // _UAE_CACHE_

            // Bind symbol nad memory handle
            CALname name = program->getInputName(i);
            CALmem mem = buffer->getMemHandle();

            CALresult result = calCtxSetMem(ctx, name, mem);
            CAL_RESULT_ERROR(result, "Failed to bind input resource\n");

#ifdef _USE_CACHE_
            // If it is being called first time
            // initialize cache
            if(inputs.size() != inputArguments.size())
            {
                inputs.push_back(stream);
            }
#endif //_USE_CACHE_
        }
        else
        {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief bind output memory handle and  synbol handle
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALExecMgr::_bindOutput(ArgDesc* argDescs, CALProgram* program,
                        StreamVec outputArguments) const
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALcontext ctx = device->getContext();
    BufferMgr* bufMgr = _device->getBufferMgr();

#ifdef _USE_CACHE_
    // Get the previous input arguments from the iocache
    ExecutionMap::const_iterator execItr = _ioCache.find(program);

    StreamArguments* arg = execItr->second;
    std::vector<StreamImpl*>& outputs = arg->outputs;
#endif // _USE_CACHE_

    StreamVec::iterator itr = outputArguments.begin();
    ArgDesc::iterator descItr = argDescs->begin();
    int i = 0;

    for (; itr != outputArguments.end(); ++itr, ++i, ++descItr)
    {
        ArgumentDesc desc = *descItr;
        StreamImpl* stream = *itr;
        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(desc.Usage));
        if(buffer == NULL)
        {
            buffer = static_cast<CALBuffer*>(bufMgr->getBuffer(stream, desc.Usage));

            if(buffer == NULL)
            {
                return false;
            }

            stream->setBuffer(buffer, desc.Usage);
        }

#ifdef _USE_CACHE_
        //Check if it is getting called first time
        if(outputs.size() == outputArguments.size())
        {
            //Don't bind if was already bound from the same buffer
            if(outputs[i] == stream)
            {
                continue;
            }
            else
            {
                // If it is being called with another parameter 
                // put the new parameter in the cache
                outputs[i] = stream;
            }
        }
#endif // _USE_CACHE_

        // Bind symbol nad memory handle
        CALname name = program->getOutputName(i);
        CALmem mem = buffer->getMemHandle();

        CALresult result = calCtxSetMem(ctx, name, mem);
        CAL_RESULT_ERROR(result, "Failed to bind output resource\n");

#ifdef _USE_CACHE_
        // If it is being called first time
        // initialize cache
        if(outputs.size() != outputArguments.size())
        {
            outputs.push_back(stream);
        }
#endif // _USE_CACHE_
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set input events on all the constant buffers
//!
////////////////////////////////////////////////////////////////////////////////

void
CALExecMgr::_setConstEvent(unsigned int bufferCount, CALevent* execEvent,
                           CALConstBuffer** buffers) const
{
    for (unsigned int i = 0; i < bufferCount; ++i)
    {
        buffers[i]->setInputEvent(execEvent);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set input events on all the input arguments
//!
////////////////////////////////////////////////////////////////////////////////

void
CALExecMgr::_setInputEvent(ArgDesc* argDescs, CALevent* execEvent,
                           StreamVec inputArguments) const
{
    StreamVec::iterator itr = inputArguments.begin();
    ArgDesc::iterator descItr = argDescs->begin();

    for (; itr != inputArguments.end(); ++itr, ++descItr)
    {
        ArgumentDesc desc = *descItr;
        StreamImpl* stream = *itr;
        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(desc.Usage));
        buffer->setInputEvent(execEvent);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set output events on all the output arguments
//!
////////////////////////////////////////////////////////////////////////////////

void
CALExecMgr::_setOutputEvent(ArgDesc* argDescs, CALevent* execEvent,
                            StreamVec outputArguments) const
{
    StreamVec::iterator itr = outputArguments.begin();
    ArgDesc::iterator descItr = argDescs->begin();

    for (; itr != outputArguments.end(); ++itr, ++descItr)
    {
        ArgumentDesc desc = *descItr;
        StreamImpl* stream = *itr;
        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(desc.Usage));
        buffer->setOutputEvent(execEvent);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief wait for copy events
//!
////////////////////////////////////////////////////////////////////////////////

void
CALExecMgr::_waitCopyEvent(ArgDesc* argDescs, StreamVec arguments) const
{
    StreamVec::iterator itr = arguments.begin();
    ArgDesc::iterator descItr = argDescs->begin();

    for (; itr != arguments.end(); ++itr, ++descItr)
    {
        ArgumentDesc desc = *descItr;
        StreamImpl* stream = *itr;

        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(desc.Usage));
        if(buffer)
        {
            buffer->waitCopyEvent();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief wait for input events
//!
////////////////////////////////////////////////////////////////////////////////

void
CALExecMgr::_waitInputEvents(ArgDesc* argDescs, StreamVec arguments) const
{
    StreamVec::iterator itr = arguments.begin();
    ArgDesc::iterator descItr = argDescs->begin();

    for (; itr != arguments.end(); ++itr, ++descItr)
    {
        ArgumentDesc desc = *descItr;
        StreamImpl* stream = *itr;

        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(desc.Usage));
        if(buffer)
        {
            buffer->waitInputEvent();
            buffer->waitWriteThread();
            buffer->waitWriteEvent();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief wait for output events
//!
////////////////////////////////////////////////////////////////////////////////

void
CALExecMgr::_waitOutputEvents(ArgDesc* argDescs, StreamVec arguments) const
{
    StreamVec::iterator itr = arguments.begin();
    ArgDesc::iterator descItr = argDescs->begin();

    for (; itr != arguments.end(); ++itr, ++descItr)
    {
        ArgumentDesc desc = *descItr;
        StreamImpl* stream = *itr;

        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(desc.Usage));
        if(buffer)
        {
            buffer->waitOutputEvent();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALExecMgr::~CALExecMgr()
{
    for(unsigned int i = 0; i < _execEvents.size(); ++i)
    {
        delete _execEvents[i];
    }

    _execEvents.clear();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Return an event that is not being used
//!
////////////////////////////////////////////////////////////////////////////////

CALevent* 
CALExecMgr::_getExecEvent()
{
    // Look for a free event in the cache
    unsigned int i;
    for(i = 0; i < _execEvents.size(); ++i)
    {
        if(*_execEvents[i] == 0)
        {
            return _execEvents[i];
        }
    }

    // Create if not found
    CALevent* event = new CALevent();
    *event = 0;
    _execEvents.push_back(event);

    return event;
}

