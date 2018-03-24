#ifndef _CALEXECMGR_H_
#define _CALEXECMGR_H_

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
//! \file CALExecMgr.h
//!
//! \brief Contains the declaration of CALExecMgr class
//!
///////////////////////////////////////////////////////////////////////////////

#include "cal.h"
#include "ExecMgr.h"
#include "CALConstBuffer.h"
#include "KernelDescData.h"

using namespace brook;

class CALDevice;
class CALProgram;

////////////////////////////////////////////////////////////////////////////////
//!
//! \class CALExecMgr
//!
//! \brief Backend specific implementation of executing a program and 
//! program management
//!
////////////////////////////////////////////////////////////////////////////////

class CALExecMgr : public ExecMgr
{
    public:

        explicit CALExecMgr(CALDevice* device);
        ~CALExecMgr();

        bool executePass(Pass& pass, ArgInfo* arguments, bool forceFlush);

    protected:

        Program* _createProgram(Pass& pass);

    private:

        CALevent* _getExecEvent();

        bool _allocateConstant(CALProgram* program, ConstVec constArguments,
                               CALConstBuffer**& buffer, unsigned int& bufferCount) const;
        bool _bindConstant(unsigned int bufferCount, CALProgram* program, 
                           CALConstBuffer** buffer) const;
        bool _bindInput(ArgDesc* argDescs, CALProgram* program, StreamVec inputArguments) const;
        bool _bindOutput(ArgDesc* argDescs, CALProgram* program, StreamVec outputArguments) const;

        void _setConstEvent(unsigned int bufferCount, CALevent* execEvent,
                            CALConstBuffer** buffers) const;
        void _setInputEvent(ArgDesc* argDescs, CALevent* execEvent, StreamVec inputArguments) const;
        void _setOutputEvent(ArgDesc* argDescs, CALevent* execEvent, StreamVec outputArguments) const;

        void _waitCopyEvent(ArgDesc* argDescs, StreamVec arguments) const;
        void _waitInputEvents(ArgDesc* argDescs, StreamVec arguments) const;
        void _waitOutputEvents(ArgDesc* argDescs, StreamVec arguments) const;

    private:

        //! \brief Array of Execution events.
        //! The same event adddress is used for constant buffers and 
        //! inputs and outputs.
        std::vector<CALevent*> _execEvents;
};

#endif //_CALEXECMGR_H_
