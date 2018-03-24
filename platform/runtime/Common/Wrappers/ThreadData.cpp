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


#include "ThreadData.h"

using namespace brook;

#ifdef _WIN32

#include "windows.h"

static DWORD tlsKey;

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, 
                      DWORD dwReason, LPVOID /*pReserved*/)
{
    ThreadData* pData;

    switch (dwReason) {
        case DLL_THREAD_DETACH:

            // release memory for this thread
            pData = (ThreadData*) ::TlsGetValue(tlsKey);
            delete pData;

            break;
    }

    return TRUE;
}

#else

 #include <pthread.h>

pthread_key_t tlsKey = 0;

void destructor(void *value)
{
    ThreadData* pData = (ThreadData*)value;
    delete pData;
    pthread_setspecific(tlsKey, NULL);
}

#endif

namespace brook
{
    bool createThreadData()
    {
#ifdef _WIN32

        // allocate a TLS index
        tlsKey = ::TlsAlloc();
        if (tlsKey == TLS_OUT_OF_INDEXES)
        {
            return false;
        }

#else

        int result = pthread_key_create(&tlsKey, destructor);
        if(!result)
        {
            return false;
        }

#endif

        return true;
    }

    ThreadData* getThreadData()
    {
        ThreadData* pData = NULL;  // The stored memory pointer 

#ifdef _WIN32

        pData = (ThreadData*)TlsGetValue(tlsKey); 
        if (pData == NULL)
        {
            pData = new ThreadData();
            ::TlsSetValue(tlsKey, (LPVOID)pData);
        }

#else

        pData = (ThreadData*)pthread_getspecific(tlsKey);
        if (pData == NULL)
        {
            pData = new ThreadData();
            pthread_setspecific(tlsKey, (void*)pData);
        }

#endif

        return pData;
    }

    void destroyThreadData()
    {
        ThreadData* pData = NULL;

#ifdef _WIN32

        // release memory for this thread
        pData = (ThreadData*)::TlsGetValue(tlsKey);
        delete pData;

        // release the TLS index
        ::TlsFree(tlsKey);

#else

        pData = (ThreadData*)pthread_getspecific(tlsKey);
        delete pData;
        pthread_setspecific(tlsKey, NULL);

        pthread_key_delete(tlsKey);

#endif
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Constructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    ThreadData::ThreadData() : _currentDevices(NULL)
    {
    }

    Device*
    ThreadData::getCurrentDevices(unsigned int& count)
    {
        count = _deviceCount;
        return _currentDevices;
    }

    Device*
    ThreadData::setCurrentDevices(brook::Device* devices, unsigned int count,
                                  unsigned int* oldDeviceCount)
    {
        Device* oldDevices = NULL;
        if(oldDeviceCount)
        {
            if(_currentDevices)
            {
                oldDevices = _currentDevices;
                *oldDeviceCount = _deviceCount;
            }
            else
            {
                *oldDeviceCount = 0;
            }
        }

        _deviceCount = count;
        _currentDevices = devices;

        return oldDevices;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Destructor
    //!
    ////////////////////////////////////////////////////////////////////////////////

    ThreadData::~ThreadData()
    {
    }
}
