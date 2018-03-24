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
//!
//! \file Thread.cpp
//!

#include "CPUThread.h"

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#endif

//! pack the function pointer and data inside this struct
typedef struct __argsToThreadFunc
{
    threadFunc func;
    void* data;

} argsToThreadFunc;


#ifdef _WIN32
//! Windows thread callback - invokes the callback set by
//! the application in Thread constructor
unsigned _stdcall win32ThreadFunc(void* args)
{
    argsToThreadFunc* ptr = (argsToThreadFunc*) args;
    CPUThread *obj = (CPUThread *) ptr->data;
    ptr->func(obj->getData());
    delete args;
    return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////
//!
//! Constructor
//!
/////////////////////////////////////////////////////////////////////////

CPUThread::CPUThread() : _tid(0), _data(0)
{
}

/////////////////////////////////////////////////////////////////////////
//!
//! Destructor
//!
/////////////////////////////////////////////////////////////////////////

CPUThread::~CPUThread()
{
#ifdef _WIN32
    if(_tid)
    {
        CloseHandle(_tid);
        _tid = 0;
    }
#endif
}


//////////////////////////////////////////////////////////////
//!
//! Create a new thread and return the status of the operation
//!
/////////////////////////////////////////////////////////////////////////

bool
CPUThread::create(threadFunc func, void *arg)
{
    // Save the data internally
    _data = arg;

#ifdef _WIN32
    // Setup the callback struct for thread function and pass to the
    // begin thread routine
    // xxx The following struct is allocated but never freed!!!!
    argsToThreadFunc *args =  new argsToThreadFunc;
    args->func = func;
    args->data = this;

    _tid = (HANDLE)_beginthreadex(NULL, 0, win32ThreadFunc, args, 0, NULL);
    if(_tid == 0)
    {
        return false;
    }

#else
    //! Now create the thread with pointer to self as the data
    int retVal = pthread_create(&_tid, NULL, func, arg);
    if(retVal != 0)
    {
        return false;
    }

#endif

    return true;
}

/////////////////////////////////////////////////////////////////////////
//!
//! Wait for this thread to join
//!
/////////////////////////////////////////////////////////////////////////

bool
CPUThread::join()
{
    if(_tid)
    {
#ifdef _WIN32
        DWORD rc = WaitForSingleObject(_tid, INFINITE);
        CloseHandle(_tid);

        if(rc == WAIT_FAILED)
        {
            printf("Bad call to function(invalid handle?)\n");
        }
#else
        int rc = pthread_join(_tid, NULL);
#endif

        _tid = 0;

        if(rc != 0)
            return false;
    }

    return true;
}
