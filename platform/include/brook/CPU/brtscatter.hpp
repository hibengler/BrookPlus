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

#ifndef BRTSCATTER_HPP
#define BRTSCATTER_HPP
#include "brtvector.hpp"
#include "brtarray.hpp"
#include "brtscatterintrinsic.hpp"
#define STREAM_SCATTER_FLOAT_MUL STREAM_SCATTER_MUL
#define STREAM_SCATTER_FLOAT_ADD STREAM_SCATTER_ADD
#define STREAM_SCATTER_INTEGER_MUL STREAM_SCATTER_MUL
#define STREAM_SCATTER_INTEGER_ADD STREAM_SCATTER_ADD
template <class T, class Functor> void scatterOpHelper (const T* s, 
                                                        float *index,
                                                        T* out,
                                                        unsigned int size,
                                                        const Functor & op) {
   for (unsigned int i=0;i<size;++i) {
      unsigned int tmp = (unsigned int)index[i];
      if (tmp<size)
        op(out[tmp],s[i]);
   }
}

template <class T, class U, class V, class Functor> 
void scatterOpArrayHelper (const T* s, 
                      V *index,
                      U &out,
                      unsigned int size,
                      const Functor & op) {
   for (unsigned int i=0;i<size;++i) {
      op(out[index[i]],s[i]);
   }
}

template <class Functor, class T> 
void streamScatterOpIndexDet(const T* data,
                             ::brt::StreamInterface * index,
                             ::brt::StreamInterface * out,
                             unsigned int bounds,
                             const Functor &op) {
   unsigned int dim = out->getDimension();
   assert(index->getFieldCount() == 1);
   switch (index->getIndexedFieldType(0)) {
   case __BRTFLOAT4:
      if (dim!=4)dim=1;
      break;
   case __BRTFLOAT3:
      if (dim!=3)dim=1;
      break;
   case __BRTFLOAT2:
      if (dim!=2)dim=1;
      break;
   default:
      dim=1;
   }
   switch (dim) {
   case 4:{
      __BrtArray <T,4,false> o((T*)out->getData(::brt::Stream::WRITE),
                               out->getExtents());
      scatterOpArrayHelper(data,
                           (__BrtFloat4 *)index->getData(::brt::Stream::READ),
                           o,
                           bounds,
                           op);      
      break;
   }
   case 3:{
      __BrtArray<T,3,false> o((T*)out->getData(::brt::Stream::WRITE),
                              out->getExtents());
      scatterOpArrayHelper(data,
                           (__BrtFloat3 *)index->getData(::brt::Stream::READ),
                           o,
                           bounds,
                           op);      
      
      break;
   }
   case 2:{
      __BrtArray<T,2,false> o((T*)out->getData(::brt::Stream::WRITE),
                              out->getExtents());
      scatterOpArrayHelper(data,
                      (__BrtFloat2 *)index->getData(::brt::Stream::READ),
                      o,
                      bounds,
                      op);      
      break;
   }
   default:
      scatterOpHelper(data,
                      (float *)index->getData(::brt::Stream::READ),
                      (T*)out->getData(::brt::Stream::WRITE),
                      bounds,
                      op);
   }
}


template <class Functor> 
void streamScatterOp4 (::brt::StreamInterface *s, 
                       ::brt::StreamInterface *index,
                       ::brt::StreamInterface *array, 
                       const Functor&op) {
   unsigned int bounds = s->getTotalSize();
   const __BrtFloat4* data = 
      (const __BrtFloat4 *) s->getData(::brt::Stream::READ);
   streamScatterOpIndexDet(data,index,array,bounds,op);
   s->releaseData(::brt::Stream::READ); 
   array->releaseData(::brt::Stream::WRITE);      
   index->releaseData(::brt::Stream::READ);
}   
template <class Functor> void streamScatterOp3 (::brt::StreamInterface *s, 
                                                ::brt::StreamInterface *index,
                                                ::brt::StreamInterface *array, 
                                                const Functor&op) {
   unsigned int bounds = s->getTotalSize();
   const __BrtFloat3* data = 
      (const __BrtFloat3 *) s->getData(::brt::Stream::READ);
   streamScatterOpIndexDet(data,index,array,bounds,op);
   s->releaseData(::brt::Stream::READ); 
   array->releaseData(::brt::Stream::WRITE);      
   index->releaseData(::brt::Stream::READ);
}   
template <class Functor> void streamScatterOp2 (::brt::StreamInterface *s, 
                                                ::brt::StreamInterface *index,
                                                ::brt::StreamInterface *array, 
                                                const Functor&op) {
   unsigned int bounds = s->getTotalSize();
   const __BrtFloat2* data = 
      (const __BrtFloat2 *) s->getData(::brt::Stream::READ);
   streamScatterOpIndexDet(data,index,array,bounds,op);
   s->releaseData(::brt::Stream::READ); 
   array->releaseData(::brt::Stream::WRITE);      
   index->releaseData(::brt::Stream::READ);
}   
template <class Functor> void streamScatterOp1 (::brt::StreamInterface *s, 
                                                ::brt::StreamInterface *index,
                                                ::brt::StreamInterface *array, 
                                                const Functor&op) {
   unsigned int bounds = s->getTotalSize();
   const __BrtFloat1* data = 
      (const __BrtFloat1 *) s->getData(::brt::Stream::READ);
   streamScatterOpIndexDet(data,index,array,bounds,op);
   s->releaseData(::brt::Stream::READ); 
   array->releaseData(::brt::Stream::WRITE);      
   index->releaseData(::brt::Stream::READ);
}   
#endif
