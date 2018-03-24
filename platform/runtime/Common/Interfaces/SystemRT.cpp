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

#include<iostream>
#include<new>

#include "SystemRT.h"
#include "Device.h"
#include "BaseDevice.h"
#include "cpu.hpp"
#include "CPUThread.h"
#include "ThreadData.h"


namespace brook
{
    using namespace brook;

    struct RuntimeData
    {
        RuntimeData() : type(NULL), runtime(NULL), libHandle(NULL), 
                        devices(NULL), count(0)
        {
        }

        const char* type;
        Runtime* runtime;
        ModuleHandle libHandle;
        Device* devices;
        unsigned int count;
    };

    class SystemRTData
    {
        public:

            //! \brief Contains the list of runtimes that a developer might want to use
            std::vector<RuntimeData*> Runtimes;

            ~SystemRTData()
            {
                for(unsigned int i = 0; i < Runtimes.size(); ++i)
                {
                    delete[] Runtimes[i]->devices;
                    delete Runtimes[i]->runtime;

                    destroyThreadData();

                    // Free the library handle
                    if(Runtimes[i]->libHandle)
                    {
#ifdef _WIN32
                        FreeLibrary(Runtimes[i]->libHandle);
#else
                        dlclose(Runtimes[i]->libHandle);
#endif
                    }
                }
            }
    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Default handler that is used in case user has not provided any hadler
    //! Gives a warning and throws bad_alloc exception. So the user gets opportunity 
    //! to handle this exception in case he has not provided a callback.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void defaultHandler()
    {
        std::cout << "Failed to allocate memory.\n";
        throw std::bad_alloc();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Function to set memory handle
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    setMemoryHandle(MemoryHandler handler)
    {
        std::set_new_handler(handler);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Function regestered in atexit call
    //!
    //! A friend function that calls runtime destructor.
    //! This function is called as soon as application exits.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    void
    cleanup()
    {
        SystemRT*& system = SystemRT::_system;
        if (system != NULL)
        {
            delete system;
            system = NULL;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Static instance of singleton class
    //!
    ////////////////////////////////////////////////////////////////////////////////

    SystemRT* SystemRT::_system = NULL;

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Thread lock used for singleton class implementation
    //!
    ////////////////////////////////////////////////////////////////////////////////

    static ThreadLock singletonLock;

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    SystemRT::SystemRT()
    {
        _systemData = new SystemRTData();
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Load library and get createRuntime function pointer
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Runtime*
    SystemRT::_loadRuntime(char *libName, ModuleHandle& libHandle)
    {
        Runtime* runtime = NULL;

        typedef Runtime* (*funct)();
        Runtime* (*funcPtr)();

#ifdef _WIN32

        libHandle = LoadLibrary(LPTSTR(libName));

        if(libHandle)
        {
            funcPtr = (funct)GetProcAddress(libHandle, "createRuntime");
            
            if(funcPtr)
            {
                runtime = funcPtr();

                // If failed to initialize free library handle
                if(!runtime)
                {
                    FreeLibrary(libHandle);
                }
            }
            else
            {
                std::cout << "Method Not Found!" << std::endl;
                FreeLibrary(libHandle);
            }
        }
        else
        {
            std::cout<< libName << " DLL Not Found!\n" << std::endl;
        }
#else

        char linuxLib[32];
        strcpy(linuxLib, "lib");
        strcat(linuxLib, libName);
        strcat(linuxLib, ".so");

        libHandle = dlopen(linuxLib, RTLD_NOW);

        if(libHandle)
        {
            funcPtr = (funct)dlsym(libHandle, "createRuntime");
            
            if(funcPtr)
            {
                runtime = funcPtr();

                // If failed to initialize free library handle
                if(!runtime)
                {
                    dlclose(libHandle);
                }
            }
            else
            {
                std::cout << "Method Not Found!" << std::endl;
                dlclose(libHandle);
            }
        }
        else
        {
            std::cout << "Shared Object NOT Found!\n" << dlerror() << std::endl;
        }
#endif
        return runtime;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Static method to get Runtime Instance.
    //!
    //! Calls Create method on backend runtime if instance is NULL.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    SystemRT* 
    SystemRT::getInstance()
    {
        if(!_system)
        {
            singletonLock.lock();
            if(!_system)
            {
                MemoryHandler oldHandle = std::set_new_handler(defaultHandler);
                if(oldHandle)
                {
                    std::set_new_handler(oldHandle);
                }

                createThreadData();

                _system = new SystemRT();

                // Register the callback for application exit
                // to make sure we cleanup everything
                atexit(cleanup);
            }
            singletonLock.unlock();
        }

        return _system;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Search the devicetype in the available runtimes and return its 
    //! position. Return -1 if not available
    //!
    ////////////////////////////////////////////////////////////////////////////////

    int
    SystemRT::_getRuntimeID(const char* deviceType)
    {
        int i = 0;
        for(; i < (int)_systemData->Runtimes.size(); ++i)
        {
            if(!strcmp(_systemData->Runtimes[i]->type, deviceType))
            {
                return i;
            }
        }

        return -1;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Create new runtime if not already created
    //! Return devices queried from the runtime
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Device* 
    SystemRT::_createDevices(const char* deviceType, unsigned int& count)
    {
        RuntimeData* data = NULL;
        Runtime* runtime = NULL;
        ModuleHandle libHandle = NULL;

        if(strcmp(deviceType, "all"))
        {
            if(!strcmp(deviceType, "cal"))
            {
#ifdef _DEBUG
                runtime = _loadRuntime("brook_cal_d", libHandle);
#else
                runtime = _loadRuntime("brook_cal", libHandle);
#endif
            }
            else if(!strcmp(deviceType, "cpu"))
            {
                runtime = CPURuntime::create();
            }
            else
            {
                count = 0;
                return NULL;
            }

            data = new RuntimeData();
            data->type = deviceType;
            data->runtime = runtime;
            data->libHandle = libHandle;
            if(runtime)
            {
                unsigned int count = runtime->getDeviceCount();
                Device* devices = new Device[count];

                for(unsigned int i = 0; i < count; ++i)
                {
                    devices[i].setDevice(runtime->getDevice(i));
                }

                data->count = count;
                data->devices = devices;
            }
        }
        else
        {
            unsigned int calDeviceCount = 0;
            Device* calDevices = NULL;
            calDevices = _getDevices("cal", calDeviceCount);

            unsigned int cpuDeviceCount = 0;
            Device* cpuDevices = NULL;
            cpuDevices = _getDevices("cpu", cpuDeviceCount);

            data = new RuntimeData();
            data->type = deviceType;

            if(calDeviceCount + cpuDeviceCount)
            {
                Device* devices = new Device[calDeviceCount + cpuDeviceCount];
                for(unsigned int i = 0; i < calDeviceCount; ++i)
                {
                    devices[i].setDevice(calDevices[i]);
                }

                for(unsigned int i = calDeviceCount; i < calDeviceCount + cpuDeviceCount; ++i)
                {
                    devices[i].setDevice(cpuDevices[i - calDeviceCount]);
                }

                data->count = calDeviceCount + cpuDeviceCount;
                data->devices = devices;
            }
        }

        _systemData->Runtimes.push_back(data);
        count = data->count;
        return data->devices;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Create new runtime if not already created
    //! Return devices queried from the runtime
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Device* 
    SystemRT::getDevices(const char* deviceType, unsigned int& count)
    {
        int id = _getRuntimeID(deviceType);
        Device* devices = NULL;
        count = 0;

        if(id != -1)
        {
            count = _systemData->Runtimes[id]->count;
            devices = _systemData->Runtimes[id]->devices;
        }
        else
        {
            // This is executed only if application is asking for a particular
            // backend first time

            // double check locking comes to rescue for
            // avoiding locks each time application asks for getDevices
            singletonLock.lock();

            int id = _getRuntimeID(deviceType);
            if(id != -1)
            {
                count = _systemData->Runtimes[id]->count;
                devices = _systemData->Runtimes[id]->devices;
            }
            else
            {
                devices = _createDevices(deviceType, count);
            }

            singletonLock.unlock();
        }

        return devices;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Internal method to create new runtime if not already created
    //! Return devices queried from the runtime
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Device* 
    SystemRT::_getDevices(const char* deviceType, unsigned int& count)
    {
        int id = _getRuntimeID(deviceType);
        if(id != -1)
        {
            count = _systemData->Runtimes[id]->count;
            return _systemData->Runtimes[id]->devices;
        }
        else
        {
            // The call is from _createDevice for CAL/CPU backned and
            // we have already locked this thread

            return _createDevices(deviceType, count);
        }

        count = 0;
        return NULL;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Set current device state
    //! Return old devices
    //!
    ////////////////////////////////////////////////////////////////////////////////
    Device*
    SystemRT::setDevices(Device* devices, unsigned int count, unsigned int* oldDeviceCount)
    {
        ThreadData* tls = getThreadData();

        return tls->setCurrentDevices(devices, count, oldDeviceCount);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Get current device state
    //!
    ////////////////////////////////////////////////////////////////////////////////

    Device*
    SystemRT::getCurrentDevices(unsigned int& deviceCount)
    {
        ThreadData* tls = getThreadData();
        Device* currentDevices = tls->getCurrentDevices(deviceCount);
        if(currentDevices)
        {
            return currentDevices;
        }
        else
        {
            Device* devices = NULL;
            char* defaultRuntime = getenv("BRT_RUNTIME");
            if((defaultRuntime != NULL) && (strcmp(defaultRuntime, "cpu") == 0))
            {
                devices = getDevices("cpu", deviceCount);
            }
            else
            {
                devices = getDevices("cal", deviceCount);

                // Fallback to CPU if CAL not initialized
                if(!devices)
                {
                    std::cout << "Failed to initialize CAL. Falling back to CPU\n";
                    devices = getDevices("cpu", deviceCount);
                }
            }

            if(!devices)
            {
                std::cout << "Failed to initialize the runtime. Application exiting";
                exit(1);
            }

            char *adapterEnv;
            int which_device = 0;
            if ((adapterEnv = getenv("BRT_ADAPTER")))
                which_device = atoi(adapterEnv);

            if(which_device >= (int)deviceCount)
            {
                which_device = 0;
            }

            tls->setCurrentDevices(&devices[which_device], 1, NULL);

            deviceCount = 1;
            return &devices[which_device];
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Destructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    SystemRT::~SystemRT()
    {
        delete _systemData;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Method to create StreamImpl given required parameters
    //!
    ////////////////////////////////////////////////////////////////////////////////

    StreamImpl* 
    SystemRT::createStreamImpl(unsigned short rank, 
                              unsigned int* dimensions, BRformat* format, unsigned int count)
    {
        unsigned int deviceCount = 0;
        Device* device = getCurrentDevices(deviceCount);

        // This release is going to use only one device
        BaseDevice* baseDevice = device[0];
        return baseDevice->createStreamImpl(rank, dimensions, format, count, device, deviceCount);
    }
}
