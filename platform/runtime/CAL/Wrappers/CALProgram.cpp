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

#include "calcl.h"
#include "CALProgram.h"
#include "CALDevice.h"
#include "KernelDesc.h"
#include "KernelDescData.h"
#include "CALBase.h"

#include <iostream>
#include <sstream>
#include <string>

#ifdef _USE_HLSL_
#include "calutAMDhlslCompiler.h"
#endif

ThreadLock CALProgram::_compileLock;

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
//! \param pass Associated pass
//! \param device Associated CALdevice
//!
////////////////////////////////////////////////////////////////////////////////

CALProgram::CALProgram(Pass* pass, BaseDevice* device)
            : Program(pass, device), _func(0), _module(0)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implemet all the logic to get symbol handle and module
//!
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALProgram::initialize()
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdeviceinfo info = device->getInfo();
    CALcontext ctx = device->getContext();

    CALresult result;

    // Compiling program
    CALobject obj;

    _compileLock.lock();

#ifdef _USE_HLSL_
    result = calutAMDhlslCompileProgram(&obj, CAL_PROGRAM_TYPE_PS, _pass.getData()->Image, info.target);
#else
    result = calclCompile(&obj, CAL_LANGUAGE_IL, _pass->getData()->Image, info.target);
#endif


    CAL_RESULT_ERROR(result, "Failed to compile program\n");

    // Linking program
    CALimage image;
    result = calclLink(&image, &obj, 1);
    CAL_RESULT_ERROR(result, "Failed to create image\n");

#ifndef _USE_HLSL_
    calclFreeObject(obj);
#endif

    // Load program
    result = calModuleLoad(&_module, ctx, image);

    calclFreeImage(image);
    _compileLock.unlock();

    CAL_RESULT_ERROR(result, "Failed to load module\n");

    // Get function handle
    result = calModuleGetEntry(&_func, ctx, _module, "main");
    CAL_RESULT_ERROR(result, "Failed to get function handle\n");

    unsigned int i = 0;
    for(i = 0; i < _pass->getData()->ConstArrays->size(); ++i)
    {
        CALname name;
        std::ostringstream tmpStr;
        tmpStr << "cb" << i;
        result = calModuleGetName(&name, ctx, _module, tmpStr.str().c_str());
        CAL_RESULT_ERROR(result, "Failed to get name handle for constant array\n");

        _constNames.push_back(name);
    }

    // Get constant name handle
    if(_pass->getData()->Constants->size() > 0)
    {
        CALname name;
        std::ostringstream tmpStr;
        tmpStr << "cb" << i;
        result = calModuleGetName(&name, ctx, _module, tmpStr.str().c_str());
        CAL_RESULT_ERROR(result, "Failed to get name handle for constants\n");

        _constNames.push_back(name);
    }

    // Get all the input name handles
    for(unsigned int i = 0; i < _pass->getData()->Inputs->size(); ++i)
    {
        CALname name;
        std::ostringstream tmpStr;
        tmpStr << "i" << i;
        result = calModuleGetName(&name, ctx, _module, tmpStr.str().c_str());
        CAL_RESULT_ERROR(result, "Failed to get name handle for input stream\n");

        _inputNames.push_back(name);
    }

    // Get all the output name handles
    for(unsigned int i = 0; i < _pass->getData()->Outputs->size(); ++i)
    {
        ArgumentDesc output = _pass->getData()->Outputs->operator [](i);
        CALname name;
        std::ostringstream tmpStr;

        // Use o# for color buffer and g[] for scatter
        if((output.Type == ACCESS_STREAM) && !(_pass->getData()->computeFlag))
        {
            tmpStr << "o" << i;
        }
        else
        {
            tmpStr << "g[]";
        }

        result = calModuleGetName(&name, ctx, _module, tmpStr.str().c_str());
        CAL_RESULT_ERROR(result, "Failed to get name handle for output stream\n");

        _outputNames.push_back(name);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get the constant name handle
//!
//! \param i constant indxex
//!
////////////////////////////////////////////////////////////////////////////////

CALname
CALProgram::getConstName(unsigned short i) const
{
    return _constNames[i];
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get the input name handle
//!
//! \param i input indxex
//!
////////////////////////////////////////////////////////////////////////////////

CALname
CALProgram::getInputName(unsigned short i) const
{
    return _inputNames[i];
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get the output name handle
//!
//! \param i output indxex
//!
////////////////////////////////////////////////////////////////////////////////

CALname
CALProgram::getOutputName(unsigned short i) const
{
    return _outputNames[i];
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALProgram::~CALProgram()
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALcontext ctx = device->getContext();

    // Destroy the module
    if(_module)
    {
        calModuleUnload(ctx, _module);
    }
}

