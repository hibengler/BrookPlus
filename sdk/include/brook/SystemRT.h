#ifndef _SYSTEM_RT_H_
#define _SYSTEM_RT_H_

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
//! \file Runtime.h
//!
//! \brief Contains the declaration of Runtime class
//!
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "ExportDef.h"

#ifdef _WIN32
#include "windows.h"
#else
#include <dlfcn.h>
#endif

#include "ErrorCodes.h"
#include "Formats.h"

class StreamImpl;
class BaseDevice;

typedef void(*MemoryHandler)();

namespace brook
{
    class Runtime;
    class Device;

#ifdef _WIN32
    typedef HMODULE ModuleHandle;
#else
    typedef void* ModuleHandle;
#endif

    EXPORT void setMemoryHandle(MemoryHandler handle);

    class SystemRTData;

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Abstract class represnting Syteam with different available runtimes
    //! A singleton that makes sure all the creation routine for stream and 
    //! kernel goes to the correct backend.
    //! 
    //! Construction - Constructed as soon as any API is called.
    //! 
    //! Destruction - Destroyed once application exits.
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    class EXPORT SystemRT
    {
        public:

            static SystemRT* getInstance();
            Device* getDevices(const char* deviceType, unsigned int& count);
            Device* setDevices(Device* devices, unsigned int count, unsigned int* oldDeviceCount);
            Device* getCurrentDevices(unsigned int& count);
            StreamImpl* createStreamImpl(unsigned short rank, unsigned int* dimensions,
                                         brook::BRformat* format, unsigned int count);

        protected:

                SystemRT();
                ~SystemRT();

        protected:

            //! \brief Static instance of singleton class
            static SystemRT* _system;

            //! \brief Data required for the class
            //! Contains the list of runtimes that a developer might want to use
            SystemRTData* _systemData;

        private:

            Runtime* _loadRuntime(char* libName, ModuleHandle& libHandle);
            Device* _createDevices(const char* deviceType, unsigned int& count);
            Device* _getDevices(const char* deviceType, unsigned int& count);
            int _getRuntimeID(const char* deviceType);
            friend void cleanup();
    };

} //end namespace brook

#endif //_SYSTEM_RT_H_
