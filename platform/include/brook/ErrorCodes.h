#ifndef _ERRORCODES_H_
#define _ERRORCODES_H_

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

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \enum Represnt all the error codes visible to brook developer.
    //!
    ////////////////////////////////////////////////////////////////////////////////
    enum BRerror
    {
        BR_NO_ERROR = 0,                        // No error
        BR_ERROR_DECLARATION,                   // Error with stream declaration
        BR_ERROR_READ,                          // Error in read method 
        BR_ERROR_WRITE,                         // Error in Write method
        BR_ERROR_KERNEL,                        // Error in Kernel invocation
        BR_ERROR_DOMAIN,                        // Error in domain operator
        BR_ERROR_ASSIGN,                        // Error in assign operator
        BR_ERROR_INVALID_PARAMATER,             // An invalid parameter passed
        BR_ERROR_NOT_SUPPORTED,                 // Feature not supported in brook+ or in the underlying hardware
        BR_ERROR_WARNING,                       // Flag for warning
        BR_ERROR_FORCE_DWORD    = 0x7fffffff,   // force 32-bit size enum
    };
}

#endif //_ERRORCODES_H_
