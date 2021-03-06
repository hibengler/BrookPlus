/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
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

#include <math.h>
#ifdef _WIN32
#include <float.h>
#endif
#ifndef BRTINTRINSIC_HPP
#define BRTINTRINSIC_HPP
#include "janitorial.hpp"
#define UNINTRINSICMEMBER(FUNC,CALLFUNC,RET) template <typename T> vec<typename T::TYPE, 1> __##FUNC##_cpu_inner(const T &f) {return f.CALLFUNC();}
/*
inline RET __##FUNC##_cpu_inner (const __BrtFloat4 &f) { \
  return f.CALLFUNC(); \
} \
inline RET __##FUNC##_cpu_inner (const __BrtFloat3 &f) { \
  return f.CALLFUNC(); \
} \
inline RET __##FUNC##_cpu_inner (const __BrtFloat2 &f) { \
  return f.CALLFUNC(); \
} \
inline RET __##FUNC##_cpu_inner (const __BrtFloat1 &f) { \
  return f.CALLFUNC(); \
}
*/

#define UNINTRINSIC(FUNC,CALLFUNC) \
template <typename T> T __##FUNC##_cpu_inner (const T&f ) { \
        return T(CALLFUNC(f.getAt(0)), \
                 CALLFUNC(f.getAt(1)), \
                 CALLFUNC(f.getAt(2)), \
                 CALLFUNC(f.getAt(3)));         \
}
/*
inline __BrtFloat4 __##FUNC##_cpu_inner (const __BrtFloat4 &f) { \
  return __BrtFloat4 (CALLFUNC (f.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1)), \
                      CALLFUNC (f.unsafeGetAt(2)), \
                      CALLFUNC (f.unsafeGetAt(3))); \
} \
inline __BrtFloat3 __##FUNC##_cpu_inner (const __BrtFloat3 &f) { \
  return __BrtFloat3 (CALLFUNC (f.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1)), \
                      CALLFUNC (f.unsafeGetAt(2))); \
} \
inline __BrtFloat2 __##FUNC##_cpu_inner (const __BrtFloat2 &f) { \
  return __BrtFloat2 (CALLFUNC (f.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1))); \
} \
inline __BrtFloat1 __##FUNC##_cpu_inner (const __BrtFloat1 &f) { \
  return __BrtFloat1 (CALLFUNC (f.unsafeGetAt(0))); \
}
*/

#define UNINTRINSICINOUT(FUNC,CALLFUNC) \
inline __BrtDouble1 __##FUNC##_cpu_inner (const __BrtDouble1 &f, __BrtDouble1 &out) { \
  return __BrtDouble1 (CALLFUNC (f.unsafeGetAt(0),out.unsafeGetAt(0))); \
} \
inline __BrtDouble2 __##FUNC##_cpu_inner (const __BrtDouble2 &f, __BrtDouble2 &out) { \
    return __BrtDouble2 (CALLFUNC (f.unsafeGetAt(0),out.unsafeGetAt(0)), \
                         CALLFUNC(f.unsafeGetAt(1),out.unsafeGetAt(1))); \
} \
inline __BrtFloat4 __##FUNC##_cpu_inner (const __BrtFloat4 &f, __BrtFloat4 &out) { \
  return __BrtFloat4 (CALLFUNC (f.unsafeGetAt(0),out.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1),out.unsafeGetAt(1)), \
                      CALLFUNC (f.unsafeGetAt(2),out.unsafeGetAt(2)), \
                      CALLFUNC (f.unsafeGetAt(3),out.unsafeGetAt(3))); \
} \
inline __BrtFloat3 __##FUNC##_cpu_inner (const __BrtFloat3 &f, __BrtFloat3 &out) { \
  return __BrtFloat3 (CALLFUNC (f.unsafeGetAt(0),out.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1),out.unsafeGetAt(1)), \
                      CALLFUNC (f.unsafeGetAt(2),out.unsafeGetAt(2))); \
} \
inline __BrtFloat2 __##FUNC##_cpu_inner (const __BrtFloat2 &f, __BrtFloat2 &out) { \
  return __BrtFloat2 (CALLFUNC (f.unsafeGetAt(0),out.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1),out.unsafeGetAt(1))); \
} \
inline __BrtFloat1 __##FUNC##_cpu_inner (const __BrtFloat1 &f, __BrtFloat1 & out) { \
  return __BrtFloat1 (CALLFUNC (f.unsafeGetAt(0),out.unsafeGetAt(0))); \
}

#if defined (_MSC_VER) && (_MSC_VER <= 1200)
#define TEMPL_TYPESIZE sizeof(BRT_TYPE)/sizeof(BRT_TYPE::TYPE)
#else
#define TEMPL_TYPESIZE BRT_TYPE::size
#endif

#define BININTRINSIC(FUNC,CALLFUNC) \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, \
       LUB<TEMPL_TYPESIZE,4>::size> __##FUNC##_cpu_inner (const __BrtFloat4 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, \
       LUB<TEMPL_TYPESIZE,3>::size> __##FUNC##_cpu_inner (const __BrtFloat3 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, \
       LUB<TEMPL_TYPESIZE,2>::size> __##FUNC##_cpu_inner (const __BrtFloat2 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, \
       LUB<TEMPL_TYPESIZE,1>::size> __##FUNC##_cpu_inner (const __BrtFloat1 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,double>::type, \
       LUB<TEMPL_TYPESIZE,2>::size> __##FUNC##_cpu_inner (const __BrtDouble2 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,double>::type, \
       LUB<TEMPL_TYPESIZE,1>::size> __##FUNC##_cpu_inner (const __BrtDouble1 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,int>::type, \
       LUB<TEMPL_TYPESIZE,4>::size> __##FUNC##_cpu_inner (const __BrtInt4 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,int>::type, \
       LUB<TEMPL_TYPESIZE,3>::size> __##FUNC##_cpu_inner (const __BrtInt3 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,int>::type, \
       LUB<TEMPL_TYPESIZE,2>::size> __##FUNC##_cpu_inner (const __BrtInt2 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,int>::type, \
       LUB<TEMPL_TYPESIZE,1>::size> __##FUNC##_cpu_inner (const __BrtInt1 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,unsigned int>::type, \
       LUB<TEMPL_TYPESIZE,4>::size> __##FUNC##_cpu_inner (const __BrtUInt4 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,unsigned int>::type, \
       LUB<TEMPL_TYPESIZE,3>::size> __##FUNC##_cpu_inner (const __BrtUInt3 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,unsigned int>::type, \
       LUB<TEMPL_TYPESIZE,2>::size> __##FUNC##_cpu_inner (const __BrtUInt2 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
} \
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,unsigned int>::type, \
       LUB<TEMPL_TYPESIZE,1>::size> __##FUNC##_cpu_inner (const __BrtUInt1 &f, const BRT_TYPE &g) { \
  return f.CALLFUNC(g); \
}

#define TRIINTRINSIC(FUNC,CALLFUNC) \
template <class ThirdEye> __BrtFloat4 __##FUNC##_cpu_inner (const __BrtFloat4 &f, \
             const __BrtFloat4 &g, \
             const ThirdEye &h) { \
  return __BrtFloat4 (CALLFUNC (f.unsafeGetAt(0), \
                                       g.unsafeGetAt(0), \
                                       h.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1), \
                                       g.unsafeGetAt(1), \
                                       h.getAt(1)), \
                      CALLFUNC (f.unsafeGetAt(2), \
                                       g.unsafeGetAt(2), \
                                       h.getAt(2)), \
                      CALLFUNC (f.unsafeGetAt(3), \
                                       g.unsafeGetAt(3), \
                                       h.getAt(3))); \
} \
template <class ThirdEye> __BrtFloat3 __##FUNC##_cpu_inner (const __BrtFloat3 &f, \
                         const __BrtFloat3 &g, \
                         const ThirdEye &h) { \
  return __BrtFloat3 (CALLFUNC (f.unsafeGetAt(0), \
                                       g.unsafeGetAt(0), \
                                       h.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1), \
                                       g.unsafeGetAt(1), \
                                       h.getAt(1)), \
                      CALLFUNC (f.unsafeGetAt(2), \
                                       g.unsafeGetAt(2), \
                                       h.getAt(2))); \
} \
template <class ThirdEye> __BrtFloat2 __##FUNC##_cpu_inner (const __BrtFloat2 &f, \
                                                            const __BrtFloat2 &g, \
                                                            const ThirdEye &h) { \
  return __BrtFloat2 (CALLFUNC (f.unsafeGetAt(0), \
                                       g.unsafeGetAt(0), \
                                       h.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1), \
                                       g.unsafeGetAt(1), \
                                       h.getAt(1))); \
} \
template <class ThirdEye> __BrtFloat1 __##FUNC##_cpu_inner (const __BrtFloat1 &f, \
                                                            const __BrtFloat1 &g, \
                                                            const ThirdEye &h) { \
  return __BrtFloat1 (CALLFUNC (f.unsafeGetAt(0), \
                                g.unsafeGetAt(0),    \
                                h.unsafeGetAt(0)));        \
} \
template <class ThirdEye> __BrtDouble2 __##FUNC##_cpu_inner (const __BrtDouble2 &f, \
                                                            const __BrtDouble2 &g, \
                                                            const ThirdEye &h) { \
  return __BrtDouble2 (CALLFUNC (f.unsafeGetAt(0), \
                                       g.unsafeGetAt(0), \
                                       h.unsafeGetAt(0)), \
                      CALLFUNC (f.unsafeGetAt(1), \
                                       g.unsafeGetAt(1), \
                                       h.getAt(1))); \
} \
template <class ThirdEye> __BrtDouble1 __##FUNC##_cpu_inner (const __BrtDouble1 &f, \
                                                            const __BrtDouble1 &g, \
                                                            const ThirdEye &h) { \
  return __BrtDouble1 (CALLFUNC (f.unsafeGetAt(0), \
                                g.unsafeGetAt(0),    \
                                h.unsafeGetAt(0)));        \
}

template <class T> T __normalize_cpu_inner (const T &x) {
   if (T::size==1) return x/x;
   if (T::size==2)
      return x/__sqrt_cpu_inner(x[0]*x[0]
                                + x[1]*x[1]);
   if (T::size==3)
      return x/__sqrt_cpu_inner(x[0]*x[0]
                                + x[1]*x[1]
                                + x[2]*x[2]);
   if (T::size==4)
      return x/__sqrt_cpu_inner(x[0]*x[0]
                                + x[1]*x[1]
                                + x[2]*x[2]
                                + x[3]*x[3]);
   typename T::TYPE size =x.unsafeGetAt(0)*x.unsafeGetAt(0);
   for (unsigned int i=1;i<T::size;++i) {
      size+=x.unsafeGetAt(i)*x.unsafeGetAt(i);
   }
   return x/__sqrt_cpu_inner(T(size));
}
template <typename T>  T degrees_float (T x) {
   return x*180.0f/3.1415926536f;
}
template <typename T>  float radians_float (T x) {
   return x*3.1415926536f/180.0f;
}
template <typename T>  T saturate_float (T x) {
   return x>1.0f?1.0f:x<0.0f?0.0f:x;
}

template <typename T>  T clamp_float(T x, T l, T u) {
   return x>u?u:x<l?l:x;
}

template <typename T>  T exp2_float (T x) {
   return (T)pow(2.0f,x);
}
static const float _const_log2 = (float) log(2.0f);
template <typename T>  T log2_float (T x) {
   return (T)log (x)/_const_log2;
}
template <typename T>  T round_float (T x) {
   T f = x-(T)floor(x);
   T g = (T)ceil(x)-x;
   return FPEQ(f, g) ?(x<0.0f?(T)floor(x):(T)ceil(x)):f<g?(T)floor(x):(T)ceil(x);
}
template <typename T>  T lerp_float (T a, T b, T s) {
   return a + s*(b - a);
}
template <typename T>  T rsqrt_float (T x) {
    return (T)(1.0f/sqrt(x));
}
template <typename T>  T frac_float (T x) {
   T y = x-(T)floor(x);
   return x<0.0f?1.0f-y:y;
}
template <typename T>  T frc_float (T x) {
   return frac_float(x);
}
template <typename T>  T frexp_float (T x, T & oout) {
   int exp;
   x = (T)frexp(x,&exp);
   oout=(T)exp;
   return x;
}
template <typename T>  T modf_float (T x, T & oout) {
   double exp;
   x = (T)modf(x,&exp);
   oout=(T)exp;
   return x;
}
inline float finite_float (double x) {
#ifdef _WIN32
   return (float) _finite(x);
#else
   return (float) finite(x);
#endif
}
inline  float isnan_float (double x) {
#ifdef _WIN32
   return (float) _isnan(x);
#else
   return (float) isnan(x);
#endif
}
inline float isinf_float (double x) {
#ifdef _WIN32
   return (!finite_float(x))&&(!isnan_float(x));
#else
   return isinf(x);
#endif
}

inline float imp_abs (float x) {
    return fabsf(x);
}
inline double imp_abs (double x) {
    return fabs(x);
}
inline int imp_abs (int x) {
    return abs(x);
}
inline float imp_sign (float x)
{
   int ix = * (int *) (&x);
   return ((ix & (~(1<<sizeof(float)))) == 0) ? 0.0f:x<0.0f?-1.0f:1.0f;
}
inline double imp_sign (double x)
{
   long long ix = * (long long *) (&x);
   return ((ix & (~(1<<sizeof(double)))) == 0) ? 0.0:x<0.0?-1.0:1.0;
}
inline int imp_sign (int x)
{
   return x==0?0:x<0?-1:1;
}

inline unsigned int imp_sign (unsigned int x)
{
   return x==0?0:1;
}


  BININTRINSIC(atan2,atan2)
  BININTRINSIC(fmod,fmod)
  BININTRINSIC(pow,pow)
  UNINTRINSICMEMBER(any,any,__BrtFloat1);
  UNINTRINSICMEMBER(all,all,__BrtFloat1);
  UNINTRINSICMEMBER(length,length,__BrtFloat1);
  UNINTRINSICMEMBER(len,length,__BrtFloat1);
  UNINTRINSICINOUT(frexp,frexp_float);
  UNINTRINSIC(degrees,degrees_float)
  UNINTRINSIC(radians,radians_float)
  UNINTRINSIC(saturate,saturate_float)
  UNINTRINSIC(abs,imp_abs)
  TRIINTRINSIC(clamp,clamp_float)
  UNINTRINSIC(isfinite,finite_float)
  UNINTRINSIC(isnan,isnan_float)
  BININTRINSIC(max,max_float)
  BININTRINSIC(min,min_float)
  UNINTRINSIC(sign,imp_sign)
  UNINTRINSIC(acos,acos)
  UNINTRINSIC(asin,asin)
  UNINTRINSIC(atan,atan)
  UNINTRINSIC(ceil,ceil)
  UNINTRINSIC(cos,cos)
  UNINTRINSIC(cosh,cosh)
  UNINTRINSIC(exp,exp)
  UNINTRINSIC(exp2,exp2_float)
  UNINTRINSIC(floor,floor)
  UNINTRINSIC(frac,frac_float)
  UNINTRINSIC(frc,frc_float)
  UNINTRINSIC(isinf,isinf_float)
  BININTRINSIC(ldexp,ldexp_float)
  BININTRINSIC(distance,distance)
  UNINTRINSIC(log,log)
  UNINTRINSIC(log2,log2_float)
  UNINTRINSIC(log10,log10)
  UNINTRINSICINOUT(modf,modf_float)
  UNINTRINSIC(round,round_float)
  UNINTRINSIC(rsqrt,rsqrt_float)
  UNINTRINSIC(sin,sin)
  //UNINTRINSIC(sincos)
  UNINTRINSIC(sinh,sinh)
  UNINTRINSIC(sqrt,sqrt)
  BININTRINSIC(step,step_float)
  UNINTRINSIC(tan,tan)
  UNINTRINSIC(tanh,tanh)
     //  TRIINTRINSIC(smoothstep)
  TRIINTRINSIC(lerp,lerp_float)

#undef UNINTRINSIC
#undef BININTRINSIC
#undef TRIINTRINSIC
inline __BrtFloat3 __cross_cpu_inner (const __BrtFloat3 &u, const __BrtFloat3 v) {
     return __BrtFloat3( u.unsafeGetAt(1)*v.unsafeGetAt(2)
                         -u.unsafeGetAt(2)*v.unsafeGetAt(1),
                         u.unsafeGetAt(2)*v.unsafeGetAt(0)
                         -u.unsafeGetAt(0)*v.unsafeGetAt(2),

                         u.unsafeGetAt(0)*v.unsafeGetAt(1)
                         -u.unsafeGetAt(1)*v.unsafeGetAt(0));
}

inline __BrtInt3 __cross_cpu_inner (const __BrtInt3 &u, const __BrtInt3 v) {
     return __BrtInt3( u.unsafeGetAt(1)*v.unsafeGetAt(2)
                         -u.unsafeGetAt(2)*v.unsafeGetAt(1),
                         u.unsafeGetAt(2)*v.unsafeGetAt(0)
                         -u.unsafeGetAt(0)*v.unsafeGetAt(2),

                         u.unsafeGetAt(0)*v.unsafeGetAt(1)
                         -u.unsafeGetAt(1)*v.unsafeGetAt(0));
}

inline __BrtUInt3 __cross_cpu_inner (const __BrtUInt3 &u, const __BrtUInt3 v) {
     return __BrtUInt3( u.unsafeGetAt(1)*v.unsafeGetAt(2)
                         -u.unsafeGetAt(2)*v.unsafeGetAt(1),
                         u.unsafeGetAt(2)*v.unsafeGetAt(0)
                         -u.unsafeGetAt(0)*v.unsafeGetAt(2),

                         u.unsafeGetAt(0)*v.unsafeGetAt(1)
                         -u.unsafeGetAt(1)*v.unsafeGetAt(0));
}

inline __BrtChar3 __cross_cpu_inner (const __BrtChar3 &u, const __BrtChar3 v) {
     return __BrtChar3( u.unsafeGetAt(1)*v.unsafeGetAt(2)
                         -u.unsafeGetAt(2)*v.unsafeGetAt(1),
                         u.unsafeGetAt(2)*v.unsafeGetAt(0)
                         -u.unsafeGetAt(0)*v.unsafeGetAt(2),

                         u.unsafeGetAt(0)*v.unsafeGetAt(1)
                         -u.unsafeGetAt(1)*v.unsafeGetAt(0));
}

inline __BrtUChar3 __cross_cpu_inner (const __BrtUChar3 &u, const __BrtUChar3 v) {
     return __BrtUChar3( u.unsafeGetAt(1)*v.unsafeGetAt(2)
                         -u.unsafeGetAt(2)*v.unsafeGetAt(1),
                         u.unsafeGetAt(2)*v.unsafeGetAt(0)
                         -u.unsafeGetAt(0)*v.unsafeGetAt(2),

                         u.unsafeGetAt(0)*v.unsafeGetAt(1)
                         -u.unsafeGetAt(1)*v.unsafeGetAt(0));
}


inline __BrtShort3 __cross_cpu_inner (const __BrtShort3 &u, const __BrtShort3 v) {
     return __BrtShort3( u.unsafeGetAt(1)*v.unsafeGetAt(2)
                         -u.unsafeGetAt(2)*v.unsafeGetAt(1),
                         u.unsafeGetAt(2)*v.unsafeGetAt(0)
                         -u.unsafeGetAt(0)*v.unsafeGetAt(2),

                         u.unsafeGetAt(0)*v.unsafeGetAt(1)
                         -u.unsafeGetAt(1)*v.unsafeGetAt(0));
}

inline __BrtUShort3 __cross_cpu_inner (const __BrtUShort3 &u, const __BrtUShort3 v) {
     return __BrtUShort3( u.unsafeGetAt(1)*v.unsafeGetAt(2)
                         -u.unsafeGetAt(2)*v.unsafeGetAt(1),
                         u.unsafeGetAt(2)*v.unsafeGetAt(0)
                         -u.unsafeGetAt(0)*v.unsafeGetAt(2),

                         u.unsafeGetAt(0)*v.unsafeGetAt(1)
                         -u.unsafeGetAt(1)*v.unsafeGetAt(0));
}

template <class T, class BRT_TYPE>
vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,GCCTYPENAME T::TYPE>::type, 1>
       __dot_cpu_inner (const T &f, const BRT_TYPE &g) { \
  return f.dot(g); \
}
/*
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, 1>
       __dot_cpu_inner (const __BrtFloat4 &f, const BRT_TYPE &g) { \
  return f.dot(g); \
}
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, 1>
       __dot_cpu_inner (const __BrtFloat3 &f, const BRT_TYPE &g) { \
  return f.dot(g); \
}
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, 1>
       __dot_cpu_inner (const __BrtFloat2 &f, const BRT_TYPE &g) { \
  return f.dot(g); \
}
template <class BRT_TYPE> vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,float>::type, 1>
       __dot_cpu_inner (const __BrtFloat1 &f, const BRT_TYPE &g) { \
  return f.dot(g); \
}
*/
#endif
