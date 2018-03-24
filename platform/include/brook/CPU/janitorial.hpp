/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Stanford University nor the names of any contributors 
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

// The janitorial supplies closet

#ifndef INC_JANITORIAL_HPP
#define INC_JANITORIAL_HPP

#include <float.h>
#include <math.h>

#define HIDE_KNOWN_UNUSED_PARAMS	1
#define PERMIT_WIN32_NO_UNDERSCORES	1

// Unused function parameters

#if HIDE_KNOWN_UNUSED_PARAMS
  #ifdef _WIN32
    #define UNUSED(a)	a // FIXME - need to find a Windows equivalent
  #else
    #define UNUSED(a)	a##_marked_as_unused __attribute__ ((unused))
  #endif
#endif

// Floating point comparison for equality

#define FPEQ(a, b)	(fabsf((a)-(b))<FLT_EPSILON)
#define FPNEQ(a, b)	(fabsf((a)-(b))>=FLT_EPSILON)
#define FPZ(a)		FPEQ(a, 0.0f)
#define FPNZ(a)		FPNEQ(a, 0.0f)
#define DPEQ(a, b)	(fabs((a)-(b))<DBL_EPSILON)
#define DPNEQ(a, b)	(fabs((a)-(b))>=DBL_EPSILON)
#define DPZ(a)		DPEQ(a, 0.0)
#define DPNZ(a)		DPNEQ(a, 0.0)

// Use of "WIN32" macro instead of "_WIN32" (already present from cmd line in some build configs)

#if PERMIT_WIN32_NO_UNDERSCORES
  #ifdef _WIN32
    #ifndef WIN32	
      #define WIN32
    #endif
  #endif
#endif

#endif // INC_JANITORIAL_HPP
