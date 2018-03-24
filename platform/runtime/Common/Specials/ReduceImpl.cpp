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
#include "Utility.h"
#include "ReduceImpl.h"
#include "StreamImpl.h"
#include "Device.h"
#include "StreamOracle.h"
#include "BufferMgr.h"
#include "BaseDevice.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

ReduceImpl::ReduceImpl(const KernelDesc* desc, BaseDevice* device)
            :KernelImpl(desc, device)
{
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Execute the given technique.
//!
//! The only method that needs to be overriden by CAL and Reduce
//! Also contains logic for multi-passing
//!
////////////////////////////////////////////////////////////////////////////////

void
ReduceImpl::_executeTechnique(Technique& technique, ArgumentInfo* arguments)
{
    StreamMap output = _parameters.outputArguments;
    StreamMap input = _parameters.inputArguments;

    StreamImpl* outputStream = output.begin()->second;
    StreamImpl* inputStream = input.begin()->second;

    BRformat format = outputStream->getDataFormat(0);
    unsigned int outRank = outputStream->getRank();
    unsigned int inRank = inputStream->getRank();

    if(inRank < outRank)
    {
        outputStream->setErrorCode(BR_ERROR_INVALID_PARAMATER);
        outputStream->setErrorString("Kernel Execution : Reduction operation don't allow "
                                     "output rank > input rank");

        return;
    }

    unsigned int* domainOffset = arguments->startExecDomain;
    unsigned int* domainDimensions = arguments->domainDimension;

    // XXX - ExecDomain not supported for 3D streams
    if((domainOffset || domainDimensions) && (inRank > 2))
    {
        outputStream->setErrorCode(BR_ERROR_NOT_SUPPORTED);
        outputStream->setErrorString("Kernel Execution : Setting Execution domain not supported for 3D streams\n");

        return;
    }

    unsigned int* inputDimensions = inputStream->getDimensions();
    unsigned int* outputDimensions = outputStream->getDimensions();
 
    unsigned int outputDim[MaxRank];
    unsigned int reductionFactor[MaxRank];
    unsigned int passReduction[MaxRank];

    StreamOracle* oracle = _device->getStreamOracle();

    unsigned int outputElements = 1;
    for(unsigned int i = 0; i < outRank; ++i)
    {
        outputElements *= outputDimensions[i];
        outputDim[i] = outputDimensions[i];
    }

    for(unsigned int i = outRank; i < MaxRank; ++i)
    {
        outputDim[i] = 1;
    }

    unsigned int inputElements = 1;
    for(unsigned int i = 0; i < inRank; ++i)
    {
        inputElements *= inputDimensions[i];
    }

    for(unsigned int i = 0; i < inRank; ++i)
    {
        unsigned int isIntFactor = inputDimensions[i] % outputDim[i];

        if(isIntFactor != 0)
        {
            outputStream->setErrorCode(BR_ERROR_INVALID_PARAMATER);
            outputStream->setErrorString("Kernel Execution : Input dimensions should be "
                                         "integral multiple of output dimensions.\n" 
                                         "The results might be undefined.");

            break;
        }
    }

    for(unsigned int i = 0; i < MaxRank; ++i)
    {
        reductionFactor[i] = passReduction[i] = 1;
    }

    StreamImpl* tempInput = inputStream;
    StreamImpl* tempOutput = NULL;

    unsigned int tempInElements = inputElements;
    unsigned int tempOutElements = 1;

    unsigned int tempOutDim[MaxRank];

    // Run multiple passes with different reduction factors
    while(tempInElements > outputElements)
    {
        unsigned int* tempInDim = tempInput->getDimensions();
        for(unsigned short i = 0; i < inRank; ++i)
        {
            reductionFactor[i] = tempInDim[i] / outputDim[i];
        }

        // This loop is to avoid buffer creation failures for 
        // temporary stream.
        unsigned int failedFactor[] = {1, 1, 1, 1};
        while(true)
        {
            tempOutElements = 1;
            for(unsigned short i = 0; i < inRank; ++i)
            {
                // Reduction should work for all the cases
                //Try to use the lowest prime number as reduction factor
                passReduction[i] = Utility::lowestPrimeDivisor(reductionFactor[i] / failedFactor[i])
                                   * failedFactor[i];
                tempOutDim[i] = tempInDim[i] / passReduction[i];
                tempOutElements *= tempOutDim[i];
            }

            // Create a temporary stream
            if(tempOutElements == outputElements)
            {
                tempOutput = outputStream;
                if(!tempOutput->getBuffer(0))
                {
                    BufferMgr* bufMgr = _device->getBufferMgr();
                    Buffer* buffer = bufMgr->getBuffer(tempOutput, 0);
                    if(buffer)
                    {
                        tempOutput->setBuffer(buffer, 0);
                    }
                    else
                    {
                        outputStream->setErrorCode(BR_ERROR_KERNEL);
                        outputStream->setErrorString("Kernel Execution : Output stream is not allocated\n");

                        // delete temporary streams
                        if(tempInput != inputStream)
                        {
                            delete tempInput;
                        }

                        return;
                    }
                }

                break;
            }
            else
            {
                tempOutput = oracle->createStreamImpl(inRank, tempOutDim, &format, 1);
                if(!tempOutput)
                {
                    outputStream->setErrorCode(BR_ERROR_KERNEL);
                    outputStream->setErrorString("Kernel Execution : Unable to create temporary stream "
                                                 "required for reduction\n.");

                    return;
                }

                BufferMgr* bufMgr = _device->getBufferMgr();
                Buffer* buffer = bufMgr->getBuffer(tempOutput, 0);
                if(buffer)
                {
                    tempOutput->setBuffer(buffer, 0);
                    break;
                }
                else
                {
                    // If temporary stream could not be allocated, try again with increaing
                    // reduction factor for this pass
                    delete tempOutput;
                    memcpy(failedFactor, passReduction, inRank * sizeof(unsigned int));
                }
            }
        }

        Pass& pass = technique.getData()->Passes->operator [](0);

        ArgInfo args;
        args.inputs.push_back(tempInput);
        args.outputs.push_back(tempOutput);

        // Set a new constant in the IL shader showing reduction factor to be used
        ArgDesc* argConsts = pass.getData()->Constants;
        for(unsigned int i = 0; i < pass.getData()->Constants->size(); ++i)
        {
            if(argConsts->operator [](i).Usage == CONST_REDUCTIONFACTOR)
            {
                args.constants.push_back(new ConstImpl(passReduction, BR_FORMAT_INT_4, _device));
            }
        }

        _parameters.inputArguments[0] = tempInput;
        _parameters.outputArguments[1] = tempOutput;

        _createExtraConstant(*(pass.getData()->Constants), &(args.constants));
        
        // Calculate output domain for this pass
        unsigned int startDomain[MaxRank];
        unsigned int domain[MaxRank];

        if(domainOffset)
        {
            for(unsigned int i = 0; i < MaxRank; ++i)
            {
                startDomain[i] = domainOffset[i] * reductionFactor[i] / passReduction[i];
            }

            arguments->startExecDomain = startDomain;
        }

        if(domainDimensions)
        {
            for(unsigned int i = 0; i < MaxRank; ++i)
            {
                domain[i] = domainDimensions[i] * reductionFactor[i] / passReduction[i];
            }

            arguments->domainDimension = domain;
        }

        bool success = _executeDomainPass(pass, &args, arguments, args.outputs[0]);

        _destroyExtraConstants(args.constants);

        // delete temporary streams
        if(tempInput != inputStream)
        {
            delete tempInput;
        }

        if(!success)
        {
            outputStream->setErrorCode(BR_ERROR_KERNEL);
            outputStream->setErrorString("Kernel Execution : Failed to execute a reduction pass\n.");
            return;
        }

        tempInput = tempOutput;
        tempInElements = tempOutElements;
    }
}
