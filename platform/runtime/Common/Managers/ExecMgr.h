#ifndef _EXECMGR_H_
#define _EXECMGR_H_

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
//! \file ExecMgr.h
//!
//! \brief Contains the declaration of ExecMgr class
//!
////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <vector>
#include <string>

#include "KernelDesc.h"
#include "ConstImpl.h"
#include "StreamImpl.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)

//The warning disables visual studio's
//annoying habit of warning when using the standard set lib
#pragma warning (disable : 4251)

#else
#define EXPORT
#endif

using namespace brook;

class brook::Device;
class Program;
class Buffer;

typedef std::vector<ConstImpl*> ConstVec;
typedef std::vector<StreamImpl*> StreamVec;

////////////////////////////////////////////////////////////////////////////////
//!
//! \struct ArgInfo
//!
//! \brief Structure containing all the input and output parameters of a Kernel.
//!
////////////////////////////////////////////////////////////////////////////////

struct ArgInfo
{
    ConstVec constants;
    StreamVec inputs;
    StreamVec outputs;

    unsigned int* startDomain;
    unsigned int* domainDimension;
    unsigned int* groupSize;
};

#ifdef _USE_CACHE_

////////////////////////////////////////////////////////////////////////////////
//!
//! \structure StreamArguments
//!
//! \brief Structure containing input and output streams
//!
////////////////////////////////////////////////////////////////////////////////

struct StreamArguments
{
    //! \brief vector containing all the inputs
    std::vector<StreamImpl*> inputs;

    //! \brief vector containing all the outputs
    std::vector<StreamImpl*> outputs;
};

typedef std::map<Program*, StreamArguments*> ExecutionMap;

#endif //_USE_CACHE_

typedef std::map<std::string, Program*> ProgramMap;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class ExecMgr
//!
//! \brief An abstract class for executing a program and program management.
//! It provide different base level and Backend specific optimizations.
//! 
//! Construction - The object is created when runtime is initialized .
//! 
//! Destruction -  Deleted when application exits.
//! 
//! Interaction -  Interacts with Program to get various information about program.
//! Interacts with Buffer to bind it with program.
//! Interacts with BufferMgr to create a buffer if an output buffer is not created yet.
//! 
//! Error conditions - Reports errors related to symbol-memory binding and 
//! program execution to User.
//! 
////////////////////////////////////////////////////////////////////////////////

class EXPORT ExecMgr
{
    public:

        explicit ExecMgr(BaseDevice* device);
        virtual bool executePass(Pass& pass, ArgInfo* arguments,
                                 bool forceFlush);
        virtual ~ExecMgr();

    protected:

        virtual Program* _createProgram(Pass& pass);
        Program* _getProgram(Pass& pass);

    protected:

        //! \brief Device for which this program management is done
        BaseDevice* _device;

        //! \brief A cache of programs.
        //! A map of Pass name and Program
        ProgramMap _programs;

#ifdef _USE_CACHE_
        //! \brief A cache of Program* and all the user arguments
        ExecutionMap _ioCache;
#endif //_USE_CACHE_
};

#endif //_EXECMGR_H_
