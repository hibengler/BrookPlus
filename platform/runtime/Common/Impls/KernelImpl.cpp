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
#include "Runtime.h"
#include "KernelOracle.h"
#include "StreamOracle.h"
#include "ConstImpl.h"
#include "KernelImpl.h"
#include "StreamImpl.h"
#include "BufferMgr.h"
#include "BaseDevice.h"

#include<iostream>

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

KernelImpl::KernelImpl(const KernelDesc* desc, BaseDevice* device)
            :_desc(desc), _paramCount(0), _device(device)
{

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implemetation of Kernel.run() method
//!
//! select the best technique depending on the stream properties and execute it
//!
//! \param arguments Arguments information.
//!
////////////////////////////////////////////////////////////////////////////////

void 
KernelImpl::run(brook::ArgumentInfo* arguments)
{
    StreamMap output = _parameters.outputArguments;
    StreamMap input = _parameters.inputArguments;

    // Check to make sure that the user hasn't bound a stream for both regular input 
    //and output at the same time
    StreamMap::iterator iterOutput;
    StreamMap::iterator iterInput;

    // Is any output stream invalid
    for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
    {
        if (!iterOutput->second)
        {
            //Set Error on all the output streams
            StreamMap::iterator iterOut;
            for (iterOut = output.begin(); iterOut != output.end(); ++iterOut)
            {
                if(iterOut->second)
                {
                    iterOut->second->setErrorCode(BR_ERROR_INVALID_PARAMATER);
                    iterOut->second->setErrorString("Kernel Execution : Invalid output stream.\n");
                }
            }

            return;
        }
        else
        {
            StreamImpl* parent = iterOutput->second->getParent();
            if(parent)
            {
                parent->NotifyModification(iterOutput->second);
            }
        }
    }

    // Check that no input stream is NULL
    for (iterInput = input.begin(); iterInput != input.end(); ++iterInput)
    {
        if(!iterInput->second)
        {
            for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
            {
                iterOutput->second->setErrorCode(BR_ERROR_INVALID_PARAMATER);
                iterOutput->second->setErrorString("Kernel Execution : Invalid Input stream.\n");
            }

            return;
        }
    }

    // Check that all outpput stream use the same device as curent device
    for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
    {
        if (iterOutput->second->getDevice() != _device)
        {
            //Set Error on all the output streams
            StreamMap::iterator iterOut;
            for (iterOut = output.begin(); iterOut != output.end(); ++iterOut)
            {
                iterOut->second->setErrorCode(BR_ERROR_KERNEL);
                iterOut->second->setErrorString("Kernel Execution : Output stream device is different \
                                                    from current device.\n");
            }

            return;
        }
    }

    // Check that all input stream use the same device as curent device
    for (iterInput = input.begin(); iterInput != input.end(); ++iterInput)
    {
        if(iterInput->second->getDevice() != _device)
        {
            for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
            {
                iterOutput->second->setErrorCode(BR_ERROR_KERNEL);
                iterOutput->second->setErrorString("Kernel Execution : Input stream device is different \
                                                    from current device.\n");
            }

            return;
        }
    }

    BufferMgr* bufMgr = _device->getBufferMgr();

    // I don't want to set the same warning again
    bool warningSet = false;

    // Check that all input streams are initialized and allocated
    for (iterInput = input.begin(); iterInput != input.end(); ++iterInput)
    {
        StreamImpl* stream = iterInput->second;
        for(unsigned int i = 0; i < iterInput->second->getBufferCount(); ++i)
        {
            if(!stream->getBuffer(i))
            {
                Buffer* buffer = bufMgr->getBuffer(stream, i);
                if(buffer)
                {
                    stream->setBuffer(buffer, i);
                }
                else
                {
                    for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
                    {
                        iterOutput->second->setErrorCode(BR_ERROR_KERNEL);
                        iterOutput->second->setErrorString("Kernel Execution : Input streams Allocation failed.\n");
                    }

                    return;
                }
            }
        }

        // If it's an un-initialized domain stream, copy data from parent
        stream->copyDataFromTree();

        if(!stream->getModifyFlag() && !warningSet)
        {
            for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
            {
                iterOutput->second->setErrorCode(BR_ERROR_WARNING);
                iterOutput->second->setErrorString("Kernel Execution : Uninitialized Input streams. "
                                                   "The results might be undefined.\n");
            }
            warningSet = true;
        }
    }

    KernelOracle* oracle = _device->getKernelOracle();
    if(!oracle->allowRWAlias())
    {
        for (iterInput = input.begin(); iterInput != input.end(); ++iterInput)
        {
            for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
            {
                if (iterOutput->second == iterInput->second)
                {
                    //Set Error on all the output streams
                    StreamMap::iterator iterOut;
                    for (iterOut = output.begin(); iterOut != output.end(); ++iterOut)
                    {
                        iterOut->second->setErrorCode(BR_ERROR_INVALID_PARAMATER);
                        iterOut->second->setErrorString("Kernel Execution : Input stream is same "
                                                        "as output stream.\n"
                                                        "Binding kernels read-write is prohibited.\n");
                    }

                    std::cout << "Kernel Execution Error: Input stream is same as output stream.\n"
                                 "Binding kernels read-write is prohibited.\n"
                                 "Environment variable BRT_PERMIT_READ_WRITE_ALIASING can be used to allow input-output aliasing.\n"
                                 "But the results can be unpredictable.\n";
                    return;
                }
            }
        }
    }

    StreamImpl* outputStream = output.begin()->second;

    //See the stream properties and see if need AT Technique
    bool requiresAddressTranslation = outputStream->isAddressTranslated();

    size_t streamArgumentCount = input.size();

    for (iterInput = input.begin(); iterInput != input.end(); ++iterInput)
    {
        StreamImpl* stream = iterInput->second;
        if (stream->isAddressTranslated())
        {
            requiresAddressTranslation = true;
        }
    }

    // Find an appropriate technique
    bool foundTechnique = false;
    unsigned int techIndex = 0;

    std::vector<Technique>* techniques = _desc->getData()->Techniques;

    while(techIndex < techniques->size() && !foundTechnique)
    {
        bool techniqueTrans = techniques->operator [](techIndex).getData()->AddressTranslation;
        
        if(requiresAddressTranslation == false)
        {
            std::vector<Pass>* passes = techniques->operator [](techIndex).getData()->Passes;
            
            for(unsigned int i = 0; i < passes->size(); ++i)
            {
                if(passes->operator [](i).getData()->computeFlag)
                {
                    requiresAddressTranslation = true;
                    break;
                }
            }
        }
        
        if (requiresAddressTranslation != techniqueTrans)
        {
            ++techIndex;
            continue;
        }

        foundTechnique = true;
    }

    if (!foundTechnique)
    {
        //Set Error on all the output streams
        StreamMap::iterator iterOut;
        for (iterOut = output.begin(); iterOut != output.end(); ++iterOut)
        {
            iterOut->second->setErrorCode(BR_ERROR_KERNEL);
            iterOut->second->setErrorString("Kernel Execution : No appropriate map technique found\n");
        }

        return;
    }

    
    //Execute the selected technique
    _executeTechnique(techniques->operator [](techIndex), arguments);

    // Copy data to the tree these output streams belong
    for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
    {
        iterOutput->second->copyDataToTree();
    }

    for (iterOutput = output.begin(); iterOutput != output.end(); ++iterOutput)
    {
        StreamImpl* parent = iterOutput->second->getParent();
        if(parent)
        {
            parent->ClearChild(iterOutput->second);
        }
    }
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
KernelImpl::_executeTechnique(Technique& technique, ArgumentInfo* arguments)
{
    std::vector<Pass>* passes = technique.getData()->Passes;
    for(unsigned int i = 0; i < passes->size(); ++i)
    {
        _executePass(passes->operator [](i), arguments);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Execute the given pass.
//!
//! Contain logic to bind extra constant parameters
//!
////////////////////////////////////////////////////////////////////////////////

void
KernelImpl::_executePass(Pass& pass, ArgumentInfo* arguments)
{
    ArgInfo args;

    _copyMapToVector(*(pass.getData()->Inputs), &_parameters.inputArguments, &(args.inputs));
    _copyMapToVector(*(pass.getData()->Outputs), &_parameters.outputArguments, &(args.outputs));
    _copyMapToVector(*(pass.getData()->ConstArrays), &_parameters.constArguments, &(args.constants));

    StreamVec outputs = args.outputs;

    BufferMgr* bufMgr = _device->getBufferMgr();
    KernelOracle* oracle = _device->getKernelOracle();

    // Check that we are able to allocate all output streams
    for (unsigned int iter = 0; iter < outputs.size(); ++iter)
    {
        StreamImpl* stream = outputs[iter];
        for(unsigned int i = 0; i < stream->getBufferCount(); ++i)
        {
            if(!stream->getBuffer(i))
            {
                Buffer* buffer = bufMgr->getBuffer(stream, i);
                if(buffer)
                {
                    stream->setBuffer(buffer, i);
                }
                else
                {
                    for (unsigned int j = 1; j < outputs.size(); ++j)
                    {
                        outputs[j]->setErrorCode(BR_ERROR_KERNEL);
                        outputs[j]->setErrorString("Kernel Execution : Output stream is not allocated\n");
                    }

                    return;
                }
            }
        }
    }

    // Find out first output stream dimension and rank
    unsigned short outputRank = 0;
    unsigned int* outDim = NULL;
    ArgDesc* argOutputs = pass.getData()->Outputs;

    for (unsigned int iter = 0; iter < outputs.size(); ++iter)
    {
        if(argOutputs->operator [](iter).Type == ACCESS_STREAM)
        {
            outputRank = outputs[iter]->getRank();
            outDim = outputs[iter]->getDimensions();

            break;
        }
    }

    if(outDim)
    {
        for (unsigned int iter = 1; iter < outputs.size(); ++iter)
        {
            // This check is required only for regular output stream
            if(argOutputs->operator [](iter).Type == ACCESS_STREAM)
            {
                unsigned short rank = outputs[iter]->getRank();
                if(rank != outputRank)
                {
                    outputs[iter]->setErrorCode(BR_ERROR_WARNING);
                    outputs[iter]->setErrorString("Kernel Execution : output stream rank doesn't match "
                                                    " with first output rank. The results might be undefined\n");
                }
                else
                {
                    unsigned int* dim = outputs[iter]->getDimensions();
                    unsigned short i = 0;
                    for(; i < outputRank; ++i)
                    {
                        if(outDim[i] != dim[i])
                        {
                            break;
                        }
                    }
                    if(i < outputRank)
                    {
                        outputs[iter]->setErrorCode(BR_ERROR_WARNING);
                        outputs[iter]->setErrorString("Kernel Execution : output stream dimension doesn't match "
                                                      " with first output dimension. The results might be undefined\n");
                    }
                }
            }
        }
    }

    // Does all the input streams have same rank as output.
    if(outDim)
    {
        StreamVec inputs = args.inputs;
        ArgDesc* argInputs = pass.getData()->Inputs;
        for(unsigned int i = 0; i < inputs.size(); ++i)
        {
            unsigned short rank = inputs[i]->getRank();
            if(rank != outputRank && argInputs->operator [](i).Type == ACCESS_STREAM)
            {
                 //Set Error on all the output streams
                for (unsigned int iterOut = 0; iterOut < outputs.size(); ++iterOut)
                {
                    outputs[iterOut]->setErrorCode(BR_ERROR_INVALID_PARAMATER);
                    outputs[iterOut]->setErrorString("Kernel Execution : input stream rank doesn't match "
                                                     "with first output rank.\n");
                }

                return;
            }
        }
    }

    // Resize input stream if rank doesn't match
    std::vector<StreamImpl*> resizedStreams;
    if(outDim)
    {
        bool misMatch = false;
        StreamVec& inputs = args.inputs;
        ArgDesc* argInputs = pass.getData()->Inputs;
        for(unsigned int i = 0; i < inputs.size(); ++i)
        {
            if(argInputs->operator [](i).Type == ACCESS_STREAM)
            {
                unsigned int* dim = inputs[i]->getDimensions();
                unsigned int j = 0;
                for(j = 0; j < outputRank; ++j)
                {
                    if(outDim[j] != dim[j])
                    {
                        break;
                    }
                }
                if(j != outputRank) // Size mismatch
                {
                    misMatch = true;

                    // Ask Oracle if user has disabled stream resize
                    if(oracle->disableStreamResize())
                    {
                        break;
                    }
                    else
                    {
                        StreamImpl* stream = _resizeStream(inputs[i], &(argInputs->operator [](i)), outDim);
                        if(stream)
                        {
                            resizedStreams.push_back(stream);
                            inputs[i] = stream;
                        }
                        else // Set error if not able to create temporary resized stream
                        {
                            // Destroy all the resized streams
                            for(unsigned int i = 0; i < resizedStreams.size(); ++i)
                            {
                                delete resizedStreams[i];
                            }

                            //Set Error on all the output streams
                            for (unsigned int iterOut = 0; iterOut < outputs.size(); ++iterOut)
                            {
                                outputs[iterOut]->setErrorCode(BR_ERROR_INVALID_PARAMATER);
                                outputs[iterOut]->setErrorString("Kernel Execution : Failed to create"
                                                                 "temporary resized input stream.\n");
                            }

                            return;
                        }
                    }
                }
            }
        }

        if(misMatch)
        {
            if(oracle->disableStreamResize())
            {
                //Set warning on all the output streams that it can produce undefined results
                for (unsigned int iterOut = 0; iterOut < outputs.size(); ++iterOut)
                {
                    outputs[iterOut]->setErrorCode(BR_ERROR_INVALID_PARAMATER);
                    outputs[iterOut]->setErrorString("Kernel Execution : Input stream dimension "
                                                     "doesn't match with output dimension. "
                                                     "Results might be undefined\n");
                }
            }
            else
            {
                //Set warning on all the output streams that this feature will be deprecated in next release
                for (unsigned int iterOut = 0; iterOut < outputs.size(); ++iterOut)
                {
                    outputs[iterOut]->setErrorCode(BR_ERROR_INVALID_PARAMATER);
                    outputs[iterOut]->setErrorString("Kernel Execution : Input stream dimension "
                                                     "doesn't match with output dimension. "
                                                     "An implicit resize is done to match output dimension. "
                                                     "In the next release this feature will be deprecated\n");
                }
            }
        }
    }

    unsigned int* domainOffset = arguments->startExecDomain;
    unsigned int* domainDimensions = arguments->domainDimension;
    
    args.groupSize = arguments->groupSize;

    // XXX - ExecDomain not supported for 3D streams
    if((domainOffset || domainDimensions) && (outputRank > 2))
    {
        //Set error on all the output streams that domain of execution can't be set for 3D stream
        for (unsigned int iterOut = 0; iterOut < outputs.size(); ++iterOut)
        {
            outputs[iterOut]->setErrorCode(BR_ERROR_NOT_SUPPORTED);
            outputs[iterOut]->setErrorString("Kernel Execution : Setting Execution domain not supported for 3D streams\n");
        }
    }
    else
    {
        _createExtraConstant(*(pass.getData()->Constants), &(args.constants));
        _executeDomainPass(pass, &args, arguments, args.outputs[0]);
        _destroyExtraConstants(args.constants, (unsigned int)pass.getData()->ConstArrays->size());
    }

    // Destroy all the resized streams
    for(unsigned int i = 0; i < resizedStreams.size(); ++i)
    {
        delete resizedStreams[i];
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set the execution domain and ask ExecMgr to run the given pass
//!
////////////////////////////////////////////////////////////////////////////////

bool 
KernelImpl::_executeDomainPass(Pass& pass, ArgInfo* args, ArgumentInfo* arguments,
                               StreamImpl* outputStream)
{
    unsigned int streamRank = outputStream->getRank();
    unsigned int bufferRank = outputStream->getBufferRank();
    unsigned int* dimensions = outputStream->getBufferDimensions();
    unsigned int* execDim = outputStream->getExecDimensions();
    unsigned int startDomain[MaxRank];
    unsigned int domain[MaxRank];

    unsigned int* domainOffset = arguments->startExecDomain;
    unsigned int* domainDimensions = arguments->domainDimension;

    // Get domain in which to run this program
    // By default, Domain is same as output dimensions
    if(domainOffset == NULL)
    {
        startDomain[0] = 0;
        startDomain[1] = 0;
    }
    else
    {
        startDomain[0] = domainOffset[0];
        startDomain[1] = domainOffset[1];
    }

    if(domainDimensions == NULL)
    {
        domain[0] = execDim[0];
        domain[1] = execDim[1];
    }
    else
    {
        domain[0] = domainDimensions[0];
        domain[1] = domainDimensions[1];
    }

    if(streamRank == 1 && bufferRank == 2)
    {
        // first identify the case where domain of execution is not set
        if(startDomain[0] == 0 && domainDimensions == NULL)
        {
            // is domain of execution set using execDomain API
            if(domain[0] == dimensions[0] && domain[1] == dimensions[1])
            {
                args->startDomain = startDomain;
                args->domainDimension = domain;

                return _device->getExecMgr()
                                         ->executePass(pass, args, false);
            }
        }

        unsigned int* streamDim = outputStream->getDimensions();

        unsigned int startX = startDomain[0] % dimensions[0];
        unsigned int startY = startDomain[0] / dimensions[0];

        unsigned int endPoint = startDomain[0] + domain[0] - 1;
        unsigned int endX = endPoint % dimensions[0];
        unsigned int endY = endPoint / dimensions[0];

        // If execution domain creates a rectangle, we need not to go for multi-passing
        if(startY == endY || (startX == 0 && endX == dimensions[0]- 1))
        {
            startDomain[0] = startX;
            startDomain[1] = startY;

            domain[0] = endX - startX + 1;
            domain[1] = endY - startY + 1;

            args->startDomain = startDomain;
            args->domainDimension = domain;

            return _device->getExecMgr()
                                     ->executePass(pass, args, false);
        }

        bool success = true;
        unsigned int currentHeight = startY;

        // Pass for the first line
        if(startX)
        {
            startDomain[0] = startX;
            startDomain[1] = startY;

            domain[0] = dimensions[0] - startX;
            domain[1] = 1;

            args->startDomain = startDomain;
            args->domainDimension = domain;

            success = _device->getExecMgr()
                                     ->executePass(pass, args, false);

            currentHeight += 1;
        }

        // Run for middle rectangle
        if(success && currentHeight < endY)
        {
            startDomain[0] = 0;
            startDomain[1] = currentHeight;

            domain[0] = dimensions[0];

            if(endX == dimensions[0]- 1)
            {
                domain[1] = endY - currentHeight + 1;
            }
            else
            {
                domain[1] = endY - currentHeight;
            }

            args->startDomain = startDomain;
            args->domainDimension = domain;

            success = _device->getExecMgr()
                                     ->executePass(pass, args, false);

            currentHeight += domain[1];
        }

        // Run for last line
        if(success && currentHeight == endY)
        {
            startDomain[0] = 0;
            startDomain[1] = currentHeight;

            domain[0] = endX + 1;
            domain[1] = 1;

            args->startDomain = startDomain;
            args->domainDimension = domain;

            success = _device->getExecMgr()
                                     ->executePass(pass, args, false);
        }

        return success;
    }

    // All other cases except 1D AT stream goes to one pass
    args->startDomain = startDomain;
    args->domainDimension = domain;

    return _device->getExecMgr()
                             ->executePass(pass, args, false);

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief create the constants needed for AT implementation and 
//! put them in ArgumentInfo
//!
////////////////////////////////////////////////////////////////////////////////

void
KernelImpl::_createExtraConstant(ArgDesc& constDesc, ConstVec* vec)
{
    ConstMap consts = _parameters.constArguments;
    StreamMap inputs = _parameters.inputArguments;
    StreamMap outputs = _parameters.outputArguments;

    for(unsigned int i = 0; i < constDesc.size(); ++i)
    {
        ArgumentDesc desc = constDesc[i];
        if(desc.Usage != CONST_USER_PARAM)
        {
            StreamImpl* stream = NULL;
            if(inputs.find(desc.Index) != inputs.end())
            {
                stream = inputs[desc.Index];
            }
            else if(outputs.find(desc.Index) != outputs.end())
            {
                stream = outputs[desc.Index];
            }

            if(stream != NULL)
            {
                switch(desc.Usage)
                {
                    case CONST_STREAMSHAPE:
                    {
                        unsigned int* dimension = stream->getDimensions();
                        unsigned int rank = stream->getRank();

                        unsigned int dim[MaxRank];
                        memcpy(dim, dimension, sizeof(unsigned int) * rank);
                        for(unsigned int i= rank; i < MaxRank; ++i)
                        {
                            dim[i] = 1;
                        }

                        vec->push_back(new ConstImpl(dim, BR_FORMAT_INT_4, _device));
                        break;
                    }
                    case CONST_BUFFERSHAPE:
                    {
                        unsigned int* dimension = stream->getBufferDimensions();
                        unsigned int rank = stream->getBufferRank();

                        unsigned int dim[MaxRank];
                        memcpy(dim, dimension, sizeof(unsigned int) * rank);
                        for(unsigned int i= rank; i < MaxRank; ++i)
                        {
                            dim[i] = 1;
                        }

                        vec->push_back(new ConstImpl(dim, BR_FORMAT_INT_4, _device));
                        break;
                    }
                    case CONST_BUFFERPITCH:
                    {
                        unsigned int pitch = stream->getBufferPitch();
                        vec->push_back(new ConstImpl(&pitch, BR_FORMAT_INT_1, _device));
                        break;
                    }
                }
            }
        }
        else
        {
            ConstImpl* cnst = NULL;
            if(consts.find(desc.Index) != consts.end())
            {
                cnst = consts[desc.Index];
                vec->push_back(new ConstImpl(cnst->getData(),
                                             cnst->getFormat(), _device));
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief delete the extra constants
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::_destroyExtraConstants(ConstVec consts, unsigned int start)
{
    for(unsigned int i = start; i < consts.size(); ++i)
    {
        delete consts[i];
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy data from stream map to a vector
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::_copyMapToVector(ArgDesc& inDesc, StreamMap* inMap, 
                                  StreamVec* outVec)
{
    for(unsigned int i = 0; i < inDesc.size(); ++i)
    {
        ArgumentDesc desc = inDesc[i];
        StreamMap::iterator itr = inMap->find(desc.Index);
        outVec->push_back(itr->second);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy data from const map to a vector
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::_copyMapToVector(ArgDesc& inDesc, ConstMap* inMap, 
                                  ConstVec* outVec)
{
    for(unsigned int i = 0; i < inDesc.size(); ++i)
    {
        ArgumentDesc desc = inDesc[i];
        ConstMap::iterator itr = inMap->find(desc.Index);
        outVec->push_back(itr->second);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Resize Input stream to the given dimensions
//!
///////////////////////////////////////////////////////////////////////////////

StreamImpl*
KernelImpl::_resizeStream(StreamImpl* input, const ArgumentDesc* desc, unsigned int* dimensions)
{
    StreamOracle* oracle = _device->getStreamOracle();
    BufferMgr* bufferMgr = _device->getBufferMgr();

    unsigned int bufferCount = input->getBufferCount();

    BRformat* formats = new BRformat[bufferCount];
    unsigned int i = 0;
    for(i = 0; i < bufferCount; ++i)
    {
        formats[i] = input->getDataFormat(i);
    }

    // Create a output stream
    StreamImpl* output = oracle->createStreamImpl(input->getRank(), dimensions, formats, bufferCount);
    Pass& pass = bufferMgr->getResizeShaderDesc()->getData()->Techniques->operator [](0).getData()->Passes->operator [](0);

    unsigned int rank = input->getRank();
    unsigned int inBufferRank = input->getBufferRank();
    unsigned int outBufferRank = output->getBufferRank();
    unsigned int* inStreamDim = input->getDimensions();
    unsigned int* inBufferDim = input->getBufferDimensions();
    unsigned int* outBufferDim = output->getBufferDimensions();

    // All the constants needed to resize stream
    float resizeFactor[MaxRank];
    int inStreamShape[MaxRank];
    int inBufferShape[MaxRank];
    int outStreamShape[MaxRank];
    int outBufferShape[MaxRank];

    unsigned int domainStart[MaxRank];
    memset(domainStart, 0, sizeof(unsigned int) * MaxRank);

    // set all the constant values
    for(i = 0; i < rank; ++i)
    {
        resizeFactor[i] = (float)dimensions[i] / (float)inStreamDim[i];
        inStreamShape[i] = inStreamDim[i];
        outStreamShape[i] = dimensions[i];
    }
    for(; i < MaxRank; ++i)
    {
        resizeFactor[i] = 1.0f;
        inStreamShape[i] = 1;
        outStreamShape[i] = 1;
    }

    for(i = 0; i < inBufferRank; ++i)
    {
        inBufferShape[i] = inBufferDim[i];
    }
    for(; i < MaxRank; ++i)
    {
        inBufferShape[i] = 1;
    }

    for(i = 0; i < outBufferRank; ++i)
    {
        outBufferShape[i] = outBufferDim[i];
    }
    for(; i < MaxRank; ++i)
    {
        outBufferShape[i] = 1;
    }

    // Create a non-AT stream, The stream is created with buffer rank and buffer dimensions
    // If it a struct stream, we need to copy only the part given by desc->Usage
    StreamImpl* tmpOutput = new StreamImpl(outBufferRank, outBufferDim, &formats[desc->Usage], 1, _device);

    Buffer* tmpOutBuffer = bufferMgr->getBuffer(tmpOutput, 0);
    if(!tmpOutBuffer)
    {
        delete formats;
        delete tmpOutput;
        return NULL;
    }
    else
    {
        tmpOutput->setBuffer(tmpOutBuffer, 0);
    }

    StreamImpl* tmpInput = new StreamImpl(inBufferRank, inBufferDim, &formats[desc->Usage], 1, _device);
    tmpInput->setBuffer(input->getBuffer(desc->Usage), 0);

    ConstImpl constResize(resizeFactor, BR_FORMAT_FLOAT_4, _device);
    ConstImpl constInStream(inStreamShape, BR_FORMAT_INT_4, _device);
    ConstImpl constInBuffer(inBufferShape, BR_FORMAT_INT_4, _device);
    ConstImpl constOutStream(outStreamShape, BR_FORMAT_INT_4, _device);
    ConstImpl constOutBuffer(outBufferShape, BR_FORMAT_INT_4, _device);

    // Set all the arguments of resize pass
    ArgInfo args;
    args.constants.push_back(&constResize);
    args.constants.push_back(&constInStream);
    args.constants.push_back(&constInBuffer);
    args.constants.push_back(&constOutStream);
    args.constants.push_back(&constOutBuffer);
    args.inputs.push_back(tmpInput);
    args.outputs.push_back(tmpOutput);

    // Domain of execution is same as output buffer
    args.startDomain = domainStart;
    args.domainDimension = (unsigned int*)outBufferShape;

    bool success = _device->getExecMgr()->executePass(pass, &args, false);
    if(!success)
    {
        delete formats;
        delete tmpOutput;
        delete tmpInput;
        return NULL;
    }
    else
    {
        // Set the output buffer
        // Copy only the desc->Usage number buffer
        output->setBuffer(tmpOutput->getBuffer(0), desc->Usage);
    }

    // Cleanup
    delete formats;
    delete tmpInput;
    delete tmpOutput;

    return output;

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes stream for CPU. Since CPU backend distinguishes
//! between stream, gather stream, reduce output, scatter stream, we need diff.
//! methods respectively.
//!
//! \param s stream
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushStream(StreamImpl* s)
{
    _parameters.inputArguments[_paramCount++] = s;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes constant for CPU backend
//!
//! \param c constant
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushConstant(ConstImpl* c)
{
    _parameters.constArguments[_paramCount++] = c;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes constant buffer for CPU backend
//!
//! \param c constant
//! \rank rank of the constant buffer
//! \dimension dimensions for the constant buffer
//
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushConstantBuffer(ConstImpl* c, unsigned int rank, unsigned int* dimension)
{
    _parameters.constArguments[_paramCount++] = c;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes stream for CPU. Since CPU backend distinguishes
//! between stream, gather stream, reduce output, scatter stream, we need diff.
//! methods respectively.
//!
//! \param s stream
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushGatherStream(StreamImpl* s)
{
    _parameters.inputArguments[_paramCount++] = s;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes stream for CPU. Since CPU backend distinguishes
//! between stream, gather stream, reduce output, scatter stream, we need diff.
//! methods respectively.
//!
//! \param s stream
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushScatterStream(StreamImpl* s)
{
    _parameters.outputArguments[_paramCount++] = s;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes stream for CPU. Since CPU backend distinguishes
//! between stream, gather stream, reduce output, scatter stream, we need diff.
//! methods respectively.
//!
//! \param s stream
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushReduceStream(StreamImpl* s)
{
    _parameters.outputArguments[_paramCount++] = s;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes temporary stream for CPU backend if
//! the reduction output is constant
//!
//! \param c constant
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushReduce(StreamImpl* s)
{
    _parameters.outputArguments[_paramCount++] = s;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Pushes stream for CPU. Since CPU backend distinguishes
//! between stream, gather stream, reduce output, scatter stream, we need diff.
//! methods respectively.
//!
//! \param s stream
//!
///////////////////////////////////////////////////////////////////////////////

void
KernelImpl::PushOutput(StreamImpl* s)
{
    _parameters.outputArguments[_paramCount++] = s;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

KernelImpl::~KernelImpl()
{
}

