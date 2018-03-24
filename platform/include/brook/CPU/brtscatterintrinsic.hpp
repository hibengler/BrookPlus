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
#ifndef GATHERINTRINSIC_HPP
#define GATHERINTRINSIC_HPP
class __StreamScatterAssign {public:
   template <class T> void operator () (T& out, const T& in)const{ 
      out=in;
   }
};
class __StreamScatterAdd {public:
   template <class T> void operator () (T& out, const T& in)const{ 
      out+=in;
   }
};
class __StreamScatterMul {public:
   template <class T> void operator () (T& out, const T& in)const{ 
      out*=in;
   }
};
class __StreamGatherInc {public:
   template <class T> void operator () (T&out, const T&in) const {
      out+=1;
   }
};
class __StreamGatherFetch {public:
   template <class T> void operator () (T&out, const T&in) const {}
};
extern  __StreamScatterAssign STREAM_SCATTER_ASSIGN;
extern  __StreamScatterAdd STREAM_SCATTER_ADD;
extern  __StreamScatterMul STREAM_SCATTER_MUL;
extern __StreamGatherInc STREAM_GATHER_INC;
extern __StreamGatherFetch STREAM_GATHER_FETCH;

#endif
