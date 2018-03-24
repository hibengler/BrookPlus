#ifndef _PROGRAM_H_
#define _PROGRAM_H_

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
//! \file Program.h
//!
//! \brief Contains the declaration of Program class
//!
////////////////////////////////////////////////////////////////////////////////

#include "KernelDesc.h"
#include "ExportDef.h"

using namespace brook;

class BaseDevice;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class Program
//!
//! \brief Abstract Base class for all Backend specific Programs. 
//! Provide functionality for gpu kernel compilation, getting different symbol handles.
//! 
//! Construction - ExecMgr contains a cache for all the programs invoked.
//! If the program doesn't exist in the cache, it is created.
//! 
//! Destruction -  All of the programs are destroyed in ExecMgr destruction 
//! when application exits.
//! 
//! Interaction - It interacts only with ExecMgr to provide interface for kernel loading
//! and getting symbol handles for ExecMgr.
//! 
//! Error Conditions - Failing in compilation and symbol binding is reported to Developer.
////////////////////////////////////////////////////////////////////////////////

class EXPORT Program
{
    public:
        Program(Pass* pass, BaseDevice* device);
        virtual bool initialize() = 0;
        inline const Pass* getPass() const;

        virtual ~Program();

    protected:
        //! \brief Contains the information of Pass that this program corresponds to
        Pass* _pass;

        //! \brief Constains Device information for Loading pass on a specific device
        BaseDevice* _device;
};

inline const
Pass*
Program::getPass() const
{
    return _pass;
}

#endif //_PROGRAM_H_
