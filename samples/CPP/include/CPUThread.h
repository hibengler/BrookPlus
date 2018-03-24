#ifndef _CPU_THREAD_H_
#define _CPU_THREAD_H_

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
//! \file Thread.h
//!

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#include "windows.h"
#else
#include "pthread.h"
#define EXPORT
#endif

//! Entry point for the thread
//! prototype of the entry point in windows
typedef void* (*threadFunc)(void*);

//////////////////////////////////////////////////////////////
//!
//! \class Thread
//! \brief Provides a wrapper for creating a _CPU_ thread.
//!
//! This class provides a simple wrapper to a CPU thread/
//!
class CPUThread
{
    public:
       //! Thread constructor and destructor. Note that the thread is
       //! NOT created in the constructor. The thread creation takes
       //! place in the create method
       CPUThread();

       ~CPUThread();
       
       //! Wrapper for pthread_create. Pass the thread's entry
       //! point and data to be passed to the routine
       bool create(threadFunc func, void* arg);

       //! Wrapper for pthread_join. The calling thread
       //! will wait until _this_ thread exits
       bool join();

       //! Get the thread data passed by the application
       void* getData() { return _data; }

    private:

       /////////////////////////////////////////////////////////////
       //!
       //! Private data members and methods
       //!

    #ifdef _WIN32
       //!  store the handle
       HANDLE _tid;
    #else
       pthread_t _tid;
    #endif

       void* _data;

};

#endif // _CPU_THREAD_H_
