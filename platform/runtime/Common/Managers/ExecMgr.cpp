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
#include "ExecMgr.h"
#include "Program.h"
#include "KernelDesc.h"
#include "KernelDescData.h"
#include "Device.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
////////////////////////////////////////////////////////////////////////////////

ExecMgr::ExecMgr(BaseDevice* device) : _device(device)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to execute a given Pass and with given arguments
//!
////////////////////////////////////////////////////////////////////////////////

bool
ExecMgr::executePass(Pass& pass, ArgInfo* arguments, bool forceFlush)
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Providing default implementation for the virtual method.
//!
////////////////////////////////////////////////////////////////////////////////

Program*
ExecMgr::_createProgram(brook::Pass &pass)
{
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Consult the cache if program already exists, otherwise 
//! create the program
//!
////////////////////////////////////////////////////////////////////////////////

Program* ExecMgr::_getProgram(Pass& pass)
{
    // Look into cache if this program alraedy exists
    std::string name = pass.getData()->Name;
    if(_programs.find(name) != _programs.end())
    {
        return _programs[name];
    }

    // Create a new program and push into cache if not found
    Program* program =  _createProgram(pass);

    if(!program->initialize())
    {
        // Return an error if compilation failed
        delete program;

        SET_ERROR("Failed to create Program");

        return NULL;
    }

    // Put it in cache
    _programs[name] = program;

#ifdef _USE_CACHE_
    if(_ioCache.find(program) == _ioCache.end())
    {
        // If the program is being called first time
        // create an associated structure to contain input and output streams
        _ioCache[program] = new StreamArguments;
    }
#endif

    return program;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
//! Delete all the programs in the cache
//!
////////////////////////////////////////////////////////////////////////////////

ExecMgr::~ExecMgr()
{
#ifdef _USE_CACHE_
    ExecutionMap::iterator itr;

    // delete all the programs in cache
    for(itr = _ioCache.begin(); itr != _ioCache.end(); ++itr)
    {
        delete itr->first;
        delete itr->second;
    }
#else
    ProgramMap::iterator itr;

    // delete all the programs in cache
    for(itr = _programs.begin(); itr != _programs.end(); ++itr)
    {
        delete itr->second;
    }
#endif
}
