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

#ifndef BRTVECTOR_HPP
#define BRTVECTOR_HPP
#if defined (_MSC_VER) && _MSC_VER <=1200 && !defined(VC6VECTOR_HPP)
#include "vc6vector.hpp"
//the above headerfile has the template functions automatically expanded.
//not needed for recent compilers.
#else

#include <iostream>
#include <math.h>

#include "janitorial.hpp"

#include "type_promotion.hpp"

typedef unsigned int uint;

template <class VALUE, unsigned int tsize> class vec;

template <class T, class B> inline T singlequestioncolon (const B& a,
                                                          const T&b,
                                                          const T&c){
    return a.questioncolon(b,c);
};
template <> inline float singlequestioncolon (const char &a,
                                              const float &b,
                                              const float &c) {
    return a?b:c;
}
template <> inline float singlequestioncolon (const float &a,
                                              const float &b,
                                              const float &c) {
    return FPNZ(a)?b:c;
}

template <> inline int singlequestioncolon (const char &a,
                                              const int &b,
                                              const int &c) {
    return a?b:c;
}

template <> inline int singlequestioncolon (const int &a,
                                              const int &b,
                                              const int &c) {
    return FPNZ(a)?b:c;
}

template <> inline uint singlequestioncolon (const char &a,
                                              const uint &b,
                                              const uint &c) {
    return a?b:c;
}

template <> inline uint singlequestioncolon (const uint &a,
                                              const uint &b,
                                              const uint &c) {
    return FPNZ(a)?b:c;
}

template <> inline char singlequestioncolon (const char &a,
                                              const char &b,
                                              const char &c) {
    return a?b:c;
}


template <> inline short singlequestioncolon (const char &a,
                                              const short &b,
                                              const short &c) {
    return a?b:c;
}

template <> inline short singlequestioncolon (const short &a,
                                              const short &b,
                                              const short &c) {
    return FPNZ(a)?b:c;
}

template <> inline ushort singlequestioncolon (const char &a,
                                              const ushort &b,
                                              const ushort &c) {
    return a?b:c;
}

template <> inline ushort singlequestioncolon (const ushort &a,
                                              const ushort &b,
                                              const ushort &c) {
    return FPNZ(a)?b:c;
}

template <> inline uchar singlequestioncolon (const char &a,
                                              const uchar &b,
                                              const uchar &c) {
    return a?b:c;
}

template <> inline uchar singlequestioncolon (const uchar &a,
                                              const uchar &b,
                                              const uchar &c) {
    return FPNZ(a)?b:c;
}

template <> inline double singlequestioncolon (const char &a,
                                               const double &b,
                                               const double &c) {
    return a?b:c;
}
template <> inline double singlequestioncolon (const double &a,
                                              const double &b,
                                              const double &c) {
    return DPNZ(a)?b:c;
}

template <> inline vec<float,1> singlequestioncolon (const vec<float,1> &a,
                                                     const vec<float,1> &b,
                                                     const vec<float,1> &c);
template <> inline vec<float,2> singlequestioncolon (const vec<float,2> & a,
                                                     const vec<float,2> &b,
                                                     const vec<float,2> &c);
template <> inline vec<float,3> singlequestioncolon (const vec<float,3> &a,
                                                     const vec<float,3> &b,
                                                     const vec<float,3> &c);
template <> inline vec<float,4> singlequestioncolon (const vec<float,4> &a,
                                                     const vec<float,4> &b,
                                                     const vec<float,4> &c);

template <> inline vec<double,1> singlequestioncolon (const vec<double,1> &a,
                                                      const vec<double,1> &b,
                                                      const vec<double,1> &c);
template <> inline vec<double,2> singlequestioncolon (const vec<double,2> & a,
                                                      const vec<double,2> &b,
                                                      const vec<double,2> &c);

template <> inline vec<int,1> singlequestioncolon (const vec<int,1> &a,
                                                     const vec<int,1> &b,
                                                     const vec<int,1> &c);
template <> inline vec<int,2> singlequestioncolon (const vec<int,2> & a,
                                                     const vec<int,2> &b,
                                                     const vec<int,2> &c);
template <> inline vec<int,3> singlequestioncolon (const vec<int,3> &a,
                                                     const vec<int,3> &b,
                                                     const vec<int,3> &c);
template <> inline vec<int,4> singlequestioncolon (const vec<int,4> &a,
                                                     const vec<int,4> &b,
                                                     const vec<int,4> &c);

template <> inline vec<uint,1> singlequestioncolon (const vec<uint,1> &a,
                                                     const vec<uint,1> &b,
                                                     const vec<uint,1> &c);
template <> inline vec<uint,2> singlequestioncolon (const vec<uint,2> & a,
                                                     const vec<uint,2> &b,
                                                     const vec<uint,2> &c);
template <> inline vec<uint,3> singlequestioncolon (const vec<uint,3> &a,
                                                     const vec<uint,3> &b,
                                                     const vec<uint,3> &c);
template <> inline vec<uint,4> singlequestioncolon (const vec<uint,4> &a,
                                                     const vec<uint,4> &b,
                                                     const vec<uint,4> &c);

template <> inline vec<char,1> singlequestioncolon (const vec<char,1> &a,
                                                     const vec<char,1> &b,
                                                     const vec<char,1> &c);
template <> inline vec<char,2> singlequestioncolon (const vec<char,2> & a,
                                                     const vec<char,2> &b,
                                                     const vec<char,2> &c);
template <> inline vec<char,3> singlequestioncolon (const vec<char,3> &a,
                                                     const vec<char,3> &b,
                                                     const vec<char,3> &c);
template <> inline vec<char,4> singlequestioncolon (const vec<char,4> &a,
                                                     const vec<char,4> &b,
                                                     const vec<char,4> &c);

template <> inline vec<uchar,1> singlequestioncolon (const vec<uchar,1> &a,
                                                     const vec<uchar,1> &b,
                                                     const vec<uchar,1> &c);
template <> inline vec<uchar,2> singlequestioncolon (const vec<uchar,2> & a,
                                                     const vec<uchar,2> &b,
                                                     const vec<uchar,2> &c);
template <> inline vec<uchar,3> singlequestioncolon (const vec<uchar,3> &a,
                                                     const vec<uchar,3> &b,
                                                     const vec<uchar,3> &c);
template <> inline vec<uchar,4> singlequestioncolon (const vec<uchar,4> &a,
                                                     const vec<uchar,4> &b,
                                                     const vec<uchar,4> &c);

template <> inline vec<short,1> singlequestioncolon (const vec<short,1> &a,
                                                     const vec<short,1> &b,
                                                     const vec<short,1> &c);
template <> inline vec<short,2> singlequestioncolon (const vec<short,2> & a,
                                                     const vec<short,2> &b,
                                                     const vec<short,2> &c);
template <> inline vec<short,3> singlequestioncolon (const vec<short,3> &a,
                                                     const vec<short,3> &b,
                                                     const vec<short,3> &c);
template <> inline vec<short,4> singlequestioncolon (const vec<short,4> &a,
                                                     const vec<short,4> &b,
                                                     const vec<short,4> &c);
template <> inline vec<ushort,1> singlequestioncolon (const vec<ushort,1> &a,
                                                     const vec<ushort,1> &b,
                                                     const vec<ushort,1> &c);
template <> inline vec<ushort,2> singlequestioncolon (const vec<ushort,2> & a,
                                                     const vec<ushort,2> &b,
                                                     const vec<ushort,2> &c);
template <> inline vec<ushort,3> singlequestioncolon (const vec<ushort,3> &a,
                                                     const vec<ushort,3> &b,
                                                     const vec<ushort,3> &c);
template <> inline vec<ushort,4> singlequestioncolon (const vec<ushort,4> &a,
                                                     const vec<ushort,4> &b,
                                                     const vec<ushort,4> &c);


template <typename T> T step_float (T a, T x){
   return (T)((x>=a)?1.0f:0.0f);
}
template <typename T> T max_float(T x, T y) {
   return (T)(x>y?x:y);
}
template<typename T> T min_float(T x, T y) {
   return (T)(x<y?x:y);
}
template<typename T> T ldexp_float(T x, T y) {
   return (T)ldexp(x,(int)y);
}
template <class T> class GetValueOf {public:
    typedef typename T::TYPE type;
};
template <> class GetValueOf <float> {public:
    typedef float type;
};
template <> class GetValueOf <double> {public:
    typedef double type;
};
template <> class GetValueOf <int> {public:
    typedef int type;
};
template <> class GetValueOf <unsigned int> {public:
    typedef unsigned int type;
};
template <> class GetValueOf <char> {public:
    typedef char type;
};
template <> class GetValueOf <unsigned char> {public:
    typedef unsigned char type;
};
template <> class GetValueOf <short> {public:
    typedef short type;
};
template <> class GetValueOf <unsigned short> {public:
    typedef unsigned short type;
};

template <> class GetValueOf <bool> {public:
    typedef bool type;
};
#if defined (_MSC_VER)
template <class T> class Holder {
public:
    static typename GetValueOf<T>::type getAt (const T&t, int i) {
        return t.getAt(i);
    }
};
#define HOLDER(TYP) template <> class Holder<TYP> { \
public: \
    static TYP getAt(TYP t, int i) { \
        return t; \
    } \
}
HOLDER(float);
HOLDER(double);
HOLDER(char);
HOLDER(unsigned char);
HOLDER(short);
HOLDER(unsigned short);
HOLDER(unsigned int);
HOLDER(int);
HOLDER(bool);
template <class T> typename GetValueOf<T>::type GetAt (const T& in,int i) {
    return Holder<T>::getAt(in,i);
}
#else
template <class T> static typename GetValueOf<T>::type GetAt (const T& in,int i) {
    return in.getAt(i);
}
#define SPECIALGETAT(TYP) template <> static TYP GetAt (const TYP& in,int UNUSED(i)) {return in;}

SPECIALGETAT(int)
SPECIALGETAT(unsigned int)
SPECIALGETAT(char)
SPECIALGETAT(unsigned char)
SPECIALGETAT(short)
SPECIALGETAT(unsigned short)
SPECIALGETAT(float)
SPECIALGETAT(double)
SPECIALGETAT(bool)

#endif
template <class T> class BracketType {public:
  typedef T type;
};
template <> class BracketType <float> {public:
  typedef vec<float,1> type;
};
template <> class BracketType <double> {public:
  typedef vec<double,1> type;
};
template <> class BracketType <int> {public:
  typedef vec<int,1> type;
};
template <> class BracketType <unsigned int> {public:
  typedef vec<uint,1> type;
};
template <> class BracketType <char> {public:
  typedef vec<char,1> type;
};
template <> class BracketType <unsigned char> {public:
  typedef vec<uchar,1> type;
};
template <> class BracketType <short> {public:
  typedef vec<short,1> type;
};
template <> class BracketType <unsigned short> {public:
  typedef vec<ushort,1> type;
};

template <class T> class BracketOp {public:
  template <class U> T& operator ()(const U&u, unsigned int i) {
    return u.unsafeGetAt(i);
  }
  template <class U> T& operator () (U&u, unsigned int i) {
    return u.unsafeGetAt(i);
  }
};
template <> class BracketOp <float> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<float,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <double> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<double,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <int> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<int,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <uint> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<uint,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <char> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<char,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <uchar> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<uchar,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <short> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<short,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <unsigned short> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<ushort,1>(u.unsafeGetAt(i));
  }
};

namespace brt
{
    enum MASKS
    {
        maskX=0,
        maskY=1,
        maskZ=2,
        maskW=3
    };
};

template <class T> class InitializeClass {public:
    template <class V> T operator () (const V&a, const V&b, const V&c,
const V&d) {
        return T(a,b,c,d);
    }
};
#define INITBASECLASS(MYTYPE) template <> class InitializeClass<MYTYPE> { \
 public: \
    template <class V> MYTYPE operator () (const V&a, \
                                          const V& UNUSED(b),  \
                                          const V& UNUSED(c),  \
                                          const V& UNUSED(d)) { \
      return (MYTYPE)a; \
    } \
}
INITBASECLASS(float);
INITBASECLASS(double);
INITBASECLASS(int);
INITBASECLASS(unsigned int);
INITBASECLASS(char);
INITBASECLASS(unsigned char);
INITBASECLASS(short);
INITBASECLASS(unsigned short);


template <> class InitializeClass<bool> {
 public:
    template <class V> bool operator () (const V&a,
                                          const V&b,
                                          const V&c,
                                          const V&d) {
      return (a||b||c||d)?true:false;
    }
};

#ifdef _MSC_VER
#if _MSC_VER <= 1200
#define GCCTYPENAME
#define INTERNALTYPENAME
#else
#define GCCTYPENAME typename
#define INTERNALTYPENAME typename
#endif
#else
#define GCCTYPENAME typename
#define INTERNALTYPENAME typename
#endif

template <class VALUE, unsigned int tsize> class vec{
public:
    typedef VALUE TYPE;
    enum SIZ{size=tsize};
    typedef VALUE array_type[size];
protected:
    VALUE f[size];
public:
    const VALUE &getAt (unsigned int i) const{
       return i<size?f[i]:f[size-1];
    }
    VALUE &getAt (unsigned int i) {
       return i<size?f[i]:f[size-1];
    }
    const VALUE &unsafeGetAt (unsigned int i) const{return f[i];}
    VALUE &unsafeGetAt (unsigned int i) {return f[i];}
    typename BracketType<VALUE>::type operator [] (int i)const {return BracketOp<VALUE>()(*this,i);}
    vec<VALUE,tsize>& gather() {
        return *this;
    }
    const vec<VALUE,tsize>& gather() const{
        return *this;
    }
    template<class BRT_TYPE> BRT_TYPE castTo()const {
        return InitializeClass<BRT_TYPE>()(getAt(0),
                                           getAt(1),
                                           getAt(2),
                                           getAt(3));
    }

    template<class BRT_TYPE> BRT_TYPE castToArg(const BRT_TYPE&)const{
        return InitializeClass<BRT_TYPE>()(getAt(0),
                                           getAt(1),
                                           getAt(2),
                                           getAt(3));
    }
   vec<VALUE,1> any() const{
      return vec<VALUE,1>(getAt(0)!=0.0f||getAt(1)!=0.0f||getAt(2)!=0.0f||getAt(3)!=0.0f);
   }
   vec<VALUE,1> all() const {
      return vec<VALUE,1>(getAt(0)!=0.0f&&getAt(1)!=0.0f&&getAt(2)!=0.0f&&getAt(3)!=0.0f);
   }
   vec<VALUE,1> length() const {
      unsigned int i;
      VALUE tot = unsafeGetAt(0);
      tot*=tot;
      for (i=1;i<tsize;++i) {
         tot+=unsafeGetAt(i)*unsafeGetAt(i);
      }
      return vec<VALUE,1>((VALUE)sqrt(tot));
   }
#define BROOK_UNARY_OP(op) vec<VALUE,tsize> operator op ()const { \
      return vec<VALUE, tsize > (op getAt(0),  \
                                 op getAt(1),  \
                                 op getAt(2),  \
                                 op getAt(3)); \
    }
    BROOK_UNARY_OP(+)
    BROOK_UNARY_OP(-)
    BROOK_UNARY_OP(!)
    BROOK_UNARY_OP(~)
#undef BROOK_UNARY_OP
#define NONCONST_BROOK_UNARY_OP(op) vec<VALUE,tsize> operator op (){ \
      return vec<VALUE, tsize > (op getAt(0),  \
                                 tsize>1?op getAt(1):getAt(1),  \
                                 tsize>2?op getAt(2):getAt(2),  \
                                 tsize>3?op getAt(3):getAt(3)); \
    }
    NONCONST_BROOK_UNARY_OP(--);
    NONCONST_BROOK_UNARY_OP(++);

#define NONCONST_BROOK_POST_UNARY_OP(op) vec<VALUE,tsize> operator op (int i){ \
      return vec<VALUE, tsize > (op getAt(0),  \
                                 tsize>1?op getAt(1):getAt(1),  \
                                 tsize>2?op getAt(2):getAt(2),  \
                                 tsize>3?op getAt(3):getAt(3)); \
    }
    NONCONST_BROOK_POST_UNARY_OP(--);
    NONCONST_BROOK_POST_UNARY_OP(++);

#undef BROOK_UNARY_OP
    vec<VALUE,4> swizzle4(int x,int y,int z,int w)const {
        return vec<VALUE,4>(unsafeGetAt(x),
                            unsafeGetAt(y),
                            unsafeGetAt(z),
                            unsafeGetAt(w));
    }
    vec<VALUE,3> swizzle3(int x,int y,int z)const {
        return vec<VALUE,3>(unsafeGetAt(x),unsafeGetAt(y),unsafeGetAt(z));
    }
    vec<VALUE,2> swizzle2(int x,int y)const {
        return vec<VALUE,2>(unsafeGetAt(x),unsafeGetAt(y));
    }
    vec<VALUE, 1> swizzle1(int x)const {
        return vec<VALUE,1>(unsafeGetAt(x));
    }
    vec() {}
#define GENERAL_TEMPLATIZED_FUNCTIONS
    template <class BRT_TYPE>
      vec (const BRT_TYPE &inx,
           const BRT_TYPE &iny,
           const BRT_TYPE &inz,
           const BRT_TYPE& inw) {
        f[0]=inx;
        if (size>1) f[1]=iny;
        if (size>2) f[2]=inz;
        if (size>3) f[3]=inw;
    }
    template <class BRT_TYPE> vec (const BRT_TYPE& inx,
                                   const BRT_TYPE& iny,
                                   const BRT_TYPE& inz) {
        f[0]=inx;
        if(size>1)f[1]=iny;
        if(size>2)f[2]=inz;
    }
    template <class BRT_TYPE> vec (const BRT_TYPE& inx, const BRT_TYPE& iny) {
        f[0]=inx;
        if (size>1) f[1]=iny;
    }
    template <class BRT_TYPE> vec (const BRT_TYPE& scalar) {
        (*this)=scalar;
    }
    template <class BRT_TYPE> operator BRT_TYPE () const{
      return InitializeClass<BRT_TYPE>()(getAt(0),getAt(1),getAt(2),getAt(3));
    }

#define ASSIGN_OP(op) template <class BRT_TYPE> \
         vec<VALUE,tsize>& operator op (const BRT_TYPE & in) {  \
        f[0] op (VALUE)(GetAt<BRT_TYPE>(in,0));  \
        if (tsize>1) f[1] op (VALUE)(GetAt<BRT_TYPE>(in,1));  \
        if (tsize>2) f[2] op (VALUE)(GetAt<BRT_TYPE>(in,2));  \
        if (tsize>3) f[3] op (VALUE)(GetAt<BRT_TYPE>(in,3));  \
        return *this;  \
    }
    ASSIGN_OP(=);
    ASSIGN_OP(/=);
    ASSIGN_OP(+=);
    ASSIGN_OP(-=);
    ASSIGN_OP(*=);
    ASSIGN_OP(%=);

#undef ASSIGN_OP
#undef GENERAL_TEMPLATIZED_FUNCTIONS
#define VECTOR_TEMPLATIZED_FUNCTIONS
    template <class BRT_TYPE>
      vec<VALUE,4> mask4 (const BRT_TYPE&in,int X, int Y,int Z,int W) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        f[Y]=GetAt<BRT_TYPE>(in,1);
        f[Z]=GetAt<BRT_TYPE>(in,2);
        f[W]=GetAt<BRT_TYPE>(in,3);
        return vec<VALUE,4>(unsafeGetAt(X),
                            unsafeGetAt(Y),
                            unsafeGetAt(Z),
                            unsafeGetAt(W));
    }
    template <class BRT_TYPE>
      vec<VALUE,3> mask3 (const BRT_TYPE&in,int X,int Y,int Z) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        f[Y]=GetAt<BRT_TYPE>(in,1);
        f[Z]=GetAt<BRT_TYPE>(in,2);
        return vec<VALUE,3>(unsafeGetAt(X),unsafeGetAt(Y),unsafeGetAt(Z));
    }
    template <class BRT_TYPE>
      vec<VALUE,2> mask2 (const BRT_TYPE&in,int X,int Y) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        f[Y]=GetAt<BRT_TYPE>(in,1);
        return vec<VALUE,2>(unsafeGetAt(X),unsafeGetAt(Y));
    }
    template <class BRT_TYPE>
      vec<VALUE,1> mask1 (const BRT_TYPE&in,int X) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        return vec<VALUE,1>(unsafeGetAt(X));
    }
    template <class BRT_TYPE>
      vec<typename BRT_TYPE::TYPE,
          LUB<BRT_TYPE::size,tsize>::size> questioncolon(const BRT_TYPE &b,
                                                const BRT_TYPE &c)const {
        return vec<GCCTYPENAME BRT_TYPE::TYPE,
                   LUB<BRT_TYPE::size,tsize>::size>
            (singlequestioncolon(getAt(0),b.getAt(0),c.getAt(0)),
             singlequestioncolon(getAt(1),b.getAt(1),c.getAt(1)),
             singlequestioncolon(getAt(2),b.getAt(2),c.getAt(2)),
             singlequestioncolon(getAt(3),b.getAt(3),c.getAt(3)));
    }

#if defined (_MSC_VER) && (_MSC_VER <= 1200)
#define TEMPL_TYPESIZE sizeof(BRT_TYPE)/sizeof(BRT_TYPE::TYPE)
#else
#define TEMPL_TYPESIZE BRT_TYPE::size
#endif
#define BROOK_BINARY_OP(op,opgets,TYPESPECIFIER) template <class BRT_TYPE>          \
    vec<GCCTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type, \
       LUB<TEMPL_TYPESIZE,tsize>::size> operator op (const BRT_TYPE &b)const{ \
      return vec<INTERNALTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE, \
                                                VALUE>::type, \
                 LUB<TEMPL_TYPESIZE,tsize>::size>(*this) opgets b; \
    }
    BROOK_BINARY_OP(*,*=,LCM);
    BROOK_BINARY_OP(/,/=,LCM);
    BROOK_BINARY_OP(+,+=,LCM);
    BROOK_BINARY_OP(-,-=,LCM);
    BROOK_BINARY_OP(%,%=,LCM);
#undef BROOK_BINARY_OP
#define BROOK_BINARY_OP(op,TYPESPECIFIER) template <class BRT_TYPE>          \
    vec<GCCTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type, \
       LUB<TEMPL_TYPESIZE,tsize>::size> operator op (const BRT_TYPE &b)const{ \
      return vec<INTERNALTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE, \
                                           VALUE>::type, \
                 LUB<TEMPL_TYPESIZE,tsize>::size> \
                (getAt(0) op GetAt<BRT_TYPE>(b,0), \
                 getAt(1) op GetAt<BRT_TYPE>(b,1), \
                 getAt(2) op GetAt<BRT_TYPE>(b,2), \
                 getAt(3) op GetAt<BRT_TYPE>(b,3)); \
    }
    BROOK_BINARY_OP(||,LCM);
    BROOK_BINARY_OP(&&,LCM);
    BROOK_BINARY_OP(>>,LCM);
    BROOK_BINARY_OP(<<,LCM);
    BROOK_BINARY_OP(&,LCM);
    BROOK_BINARY_OP(|,LCM);
    BROOK_BINARY_OP(^,LCM);

  // The following two templates explicitly added because VS2008 doesn't infer
  // types the way the other compilers we use do
  //
  // FIXME - work out exactly what's needed to get these cases automatically generated
  //

#ifdef _MSC_VER
#if _MSC_VER >= 1500
  template <class BRT_TYPE> vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, 1>
     operator || (const BRT_TYPE &b) const
     {
       return vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, LUB<BRT_TYPE::size,tsize>::size>
               (getAt(0) ||
                GetAt<BRT_TYPE>(b,0), getAt(1) ||
                GetAt<BRT_TYPE>(b,1), getAt(2) ||
                GetAt<BRT_TYPE>(b,2), getAt(3) ||
                GetAt<BRT_TYPE>(b,3));
     };

  template <class BRT_TYPE> vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, 1>
      operator && (const BRT_TYPE &b) const
      {
        return vec<typename LCM<typename BRT_TYPE::TYPE, VALUE>::type, LUB<BRT_TYPE::size,tsize>::size>
          (getAt(0) &&
           GetAt<BRT_TYPE>(b,0), getAt(1) &&
           GetAt<BRT_TYPE>(b,1), getAt(2) &&
           GetAt<BRT_TYPE>(b,2), getAt(3) &&
           GetAt<BRT_TYPE>(b,3));
      };

    template <class BRT_TYPE> vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, 1>
      questioncolon(const BRT_TYPE &b, const BRT_TYPE &c)const {
      return vec<typename LCM<typename BRT_TYPE::TYPE, VALUE>::type, LUB<BRT_TYPE::size,tsize>::size>
            (singlequestioncolon(getAt(0),b.getAt(0),c.getAt(0)),
             singlequestioncolon(getAt(1),b.getAt(1),c.getAt(1)),
             singlequestioncolon(getAt(2),b.getAt(2),c.getAt(2)),
             singlequestioncolon(getAt(3),b.getAt(3),c.getAt(3)));
    }

  // I don't understand why VS 2008 won't pick up the generic template for (*)
  // in the case of __BrtDouble1, but this fixes it for the moment.
  // FIXME: Need to resolve if this is an issue with the code base or VS 2008.
  
  vec<typename LCM<char,char>::type, 1>
      operator * (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) *= b;
  }

  vec<typename LCM<char,char>::type, 1>
      operator / (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) /= b;
  }

  vec<typename LCM<char,char>::type, 1>
      operator + (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) += b;
  }

  vec<typename LCM<char,char>::type, 1>
      operator - (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) -= b;
  }

  vec<typename LCM<char,char>::type, 2>
      operator * (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 2>
      operator / (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 2>
      operator + (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 2>
      operator - (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 3>
      operator * (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<char,char>::type, 3>
      operator / (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<char,char>::type, 3>
      operator + (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<char,char>::type, 3>
      operator - (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<char,char>::type, 4>
      operator * (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<char,char>::type, 4>
      operator / (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<char,char>::type, 4>
      operator + (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<char,char>::type, 4>
      operator - (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

    vec<typename LCM<short,short>::type, 1>
      operator * (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) *= b;
  }

  vec<typename LCM<short,short>::type, 1>
      operator / (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) /= b;
  }

  vec<typename LCM<short,short>::type, 1>
      operator + (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) += b;
  }

  vec<typename LCM<short,short>::type, 1>
      operator - (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) -= b;
  }

  vec<typename LCM<short,short>::type, 2>
      operator * (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 2>
      operator / (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 2>
      operator + (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 2>
      operator - (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 3>
      operator * (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<short,short>::type, 3>
      operator / (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<short,short>::type, 3>
      operator + (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<short,short>::type, 3>
      operator - (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<short,short>::type, 4>
      operator * (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<short,short>::type, 4>
      operator / (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<short,short>::type, 4>
      operator + (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<short,short>::type, 4>
      operator - (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

    vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator * (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) *= b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator / (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) /= b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator + (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) += b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator - (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) -= b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator * (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator / (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator + (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator - (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator * (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator / (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator + (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator - (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator * (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator / (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator + (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator - (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

    vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator * (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) *= b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator / (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) /= b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator + (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) += b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator - (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) -= b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator * (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator / (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator + (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator - (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator * (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator / (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator + (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator - (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator * (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator / (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator + (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator - (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

  vec<typename LCM<double,double>::type, 1>
      operator * (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) *= b;
  }

  vec<typename LCM<double,double>::type, 1>
      operator / (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) /= b;
  }

  vec<typename LCM<double,double>::type, 1>
      operator + (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) += b;
  }

  vec<typename LCM<double,double>::type, 1>
      operator - (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) -= b;
  }

  vec<typename LCM<double,double>::type, 2>
      operator * (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<double,double>::type, 2>
      operator / (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<double,double>::type, 2>
      operator + (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<double,double>::type, 2>
      operator - (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename COMMON_CHAR<double,double>::type, 1>
      operator == (const vec<double,1> &b) const
  {
      return vec<typename COMMON_CHAR<double,double>::type,1>(getAt(0) == b.getAt(0));
  }

#endif
#endif

    BROOK_BINARY_OP(<,COMMON_CHAR)
    BROOK_BINARY_OP(>,COMMON_CHAR)
    BROOK_BINARY_OP(<=,COMMON_CHAR)
    BROOK_BINARY_OP(>=,COMMON_CHAR)
    BROOK_BINARY_OP(!=,COMMON_CHAR)
    BROOK_BINARY_OP(==,COMMON_CHAR)
#undef BROOK_BINARY_OP
template <class BRT_TYPE>
    vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type,1>
    dot (const BRT_TYPE &b) const{
      return vec<INTERNALTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,
                                           VALUE>::type, 1>
                ((LUB<TEMPL_TYPESIZE,
                     tsize>::size)==4?(getAt(0) * GetAt<BRT_TYPE>(b,0) +
                                     getAt(1) * GetAt<BRT_TYPE>(b,1) +
                                     getAt(2) * GetAt<BRT_TYPE>(b,2) +
                                     getAt(3) * GetAt<BRT_TYPE>(b,3)):
                 (LUB<TEMPL_TYPESIZE,
                     tsize>::size)==3?(getAt(0) * GetAt<BRT_TYPE>(b,0) +
                                      getAt(1) * GetAt<BRT_TYPE>(b,1) +
                                      getAt(2) * GetAt<BRT_TYPE>(b,2)):
                 (LUB<TEMPL_TYPESIZE,
                     tsize>::size)==2?(getAt(0) * GetAt<BRT_TYPE>(b,0) +
                                      getAt(1) * GetAt<BRT_TYPE>(b,1)):
                 getAt(0) * GetAt<BRT_TYPE>(b,0));

    }

template <class BRT_TYPE>
    vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type,1>
    distance (const BRT_TYPE &b) const{
      return (b-*this).length();
    }



#define BROOK_BINARY_OP(op, subop,TYPESPECIFIER) template <class BRT_TYPE>          \
    vec<GCCTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type, \
       LUB<TEMPL_TYPESIZE,tsize>::size> op (const BRT_TYPE &b)const{ \
      return vec<INTERNALTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE, \
                                           VALUE>::type, \
                 LUB<TEMPL_TYPESIZE,tsize>::size> \
        (::subop(getAt(0) , GetAt<BRT_TYPE>(b,0)),  \
         ::subop(getAt(1) , GetAt<BRT_TYPE>(b,1)),  \
         ::subop(getAt(2) , GetAt<BRT_TYPE>(b,2)),   \
         ::subop(getAt(3) , GetAt<BRT_TYPE>(b,3)));  \
    }
    BROOK_BINARY_OP(atan2,atan2,LCM)
    BROOK_BINARY_OP(fmod,fmod,LCM)
    BROOK_BINARY_OP(pow,pow,LCM);
    BROOK_BINARY_OP(step_float,step_float,LCM);
    BROOK_BINARY_OP(ldexp_float,ldexp_float,LCM);
    BROOK_BINARY_OP(min_float,min_float,LCM);
    BROOK_BINARY_OP(max_float,max_float,LCM);
#undef TEMPL_TYPESIZE
#undef BROOK_BINARY_OP
#undef VECTOR_TEMPLATIZED_FUNCTIONS
};


template <> inline vec<float,1> singlequestioncolon (const vec<float,1> &a,
                                                     const vec<float,1> &b,
                                                     const vec<float,1> &c) {
    return FPNZ(a.unsafeGetAt(0))?b:c;
}

template <> inline vec<float,2> singlequestioncolon (const vec<float,2> & a,
                                                     const vec<float,2> &b,
                                                     const vec<float,2> &c) {
    return vec<float,2> (FPNZ(a.unsafeGetAt(0))?b.unsafeGetAt(0):c.unsafeGetAt(0),
                         FPNZ(a.unsafeGetAt(1))?b.unsafeGetAt(1):c.unsafeGetAt(1));
}
template <> inline vec<float,3> singlequestioncolon (const vec<float,3> &a,
                                              const vec<float,3> &b,
                                              const vec<float,3> &c) {
    return vec<float,3> (FPNZ(a.unsafeGetAt(0))?b.unsafeGetAt(0):c.unsafeGetAt(0),
                         FPNZ(a.unsafeGetAt(1))?b.unsafeGetAt(1):c.unsafeGetAt(1),
                         FPNZ(a.unsafeGetAt(2))?b.unsafeGetAt(2):c.unsafeGetAt(2));
}
template <> inline vec<float,4> singlequestioncolon (const vec<float,4> &a,
                                              const vec<float,4> &b,
                                              const vec<float,4> &c) {
    return vec<float,4> (FPNZ(a.unsafeGetAt(0))?b.unsafeGetAt(0):c.unsafeGetAt(0),
                         FPNZ(a.unsafeGetAt(1))?b.unsafeGetAt(1):c.unsafeGetAt(1),
                         FPNZ(a.unsafeGetAt(2))?b.unsafeGetAt(2):c.unsafeGetAt(2),
                         FPNZ(a.unsafeGetAt(3))?b.unsafeGetAt(3):c.unsafeGetAt(3));
}

template <> inline vec<int,1> singlequestioncolon (const vec<int,1> &a,
                                                   const vec<int,1> &b,
                                                   const vec<int,1> &c) {
    return a.unsafeGetAt(0)?b:c;
}

template <> inline vec<int,2> singlequestioncolon (const vec<int,2> & a,
                                                   const vec<int,2> &b,
                                                   const vec<int,2> &c) {
    return vec<int,2> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                       a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1));
}
template <> inline vec<int,3> singlequestioncolon (const vec<int,3> &a,
                                                   const vec<int,3> &b,
                                                   const vec<int,3> &c) {
    return vec<int,3> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                       a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                       a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2));
}
template <> inline vec<int,4> singlequestioncolon (const vec<int,4> &a,
                                              const vec<int,4> &b,
                                              const vec<int,4> &c) {
    return vec<int,4> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                       a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                       a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2),
                       a.unsafeGetAt(3)?b.unsafeGetAt(3):c.unsafeGetAt(3));
}

template <> inline vec<uint,1> singlequestioncolon (const vec<uint,1> &a,
                                                    const vec<uint,1> &b,
                                                    const vec<uint,1> &c) {
    return a.unsafeGetAt(0)?b:c;
}

template <> inline vec<uint,2> singlequestioncolon (const vec<uint,2> & a,
                                                    const vec<uint,2> &b,
                                                    const vec<uint,2> &c) {
    return vec<uint,2> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                        a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1));
}
template <> inline vec<uint,3> singlequestioncolon (const vec<uint,3> &a,
                                                    const vec<uint,3> &b,
                                                    const vec<uint,3> &c) {
    return vec<uint,3> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                        a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                        a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2));
}
template <> inline vec<uint,4> singlequestioncolon (const vec<uint,4> &a,
                                                    const vec<uint,4> &b,
                                                    const vec<uint,4> &c) {
    return vec<uint,4> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                        a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                        a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2),
                        a.unsafeGetAt(3)?b.unsafeGetAt(3):c.unsafeGetAt(3));
}


template <class T>
  std::ostream& operator^ (std::ostream& os, const T & a){
    if (T::size==1) {
        os << a.getAt(0);
    }else {
        os << "{";
        for (unsigned int i=0;i<T::size;++i) {
            os << a.getAt(i)<<(i!=T::size-1?", ":"");
        }
        os << "}";
    }
    return os;
}

#define VECX_CLASS(NAME,TYPE,X) \
inline std::ostream& operator << (std::ostream&a,const vec<TYPE,X> & b) { \
    return a^b; \
}   \
typedef vec<TYPE,X> NAME

VECX_CLASS(__BrtFloat1,float,1);
VECX_CLASS(__BrtFloat2,float,2);
VECX_CLASS(__BrtFloat3,float,3);
VECX_CLASS(__BrtFloat4,float,4);
VECX_CLASS(__BrtChar1,char,1);
VECX_CLASS(__BrtChar2,char,2);
VECX_CLASS(__BrtChar3,char,3);
VECX_CLASS(__BrtChar4,char,4);
VECX_CLASS(__BrtUChar1,unsigned char,1);
VECX_CLASS(__BrtUChar2,unsigned char,2);
VECX_CLASS(__BrtUChar3,unsigned char,3);
VECX_CLASS(__BrtUChar4,unsigned char,4);
VECX_CLASS(__BrtShort1,short,1);
VECX_CLASS(__BrtShort2,short,2);
VECX_CLASS(__BrtShort3,short,3);
VECX_CLASS(__BrtShort4,short,4);
VECX_CLASS(__BrtUShort1,unsigned short,1);
VECX_CLASS(__BrtUShort2,unsigned short,2);
VECX_CLASS(__BrtUShort3,unsigned short,3);
VECX_CLASS(__BrtUShort4,unsigned short,4);
VECX_CLASS(__BrtDouble1,double,1);
VECX_CLASS(__BrtDouble2,double,2);
VECX_CLASS(__BrtInt1,int,1);
VECX_CLASS(__BrtInt2,int,2);
VECX_CLASS(__BrtInt3,int,3);
VECX_CLASS(__BrtInt4,int,4);
VECX_CLASS(__BrtUInt1,unsigned int,1);
VECX_CLASS(__BrtUInt2,unsigned int,2);
VECX_CLASS(__BrtUInt3,unsigned int,3);
VECX_CLASS(__BrtUInt4,unsigned int,4);
#undef VECX_CLASS
#define MATRIXXY_CLASS(TYPE,X,Y) \
inline std::ostream& operator << (std::ostream&a, \
                                  const vec<TYPE##X,Y> & b) { \
    return a^b; \
}   \
typedef vec<TYPE##X,Y> TYPE##X##x##Y

MATRIXXY_CLASS(__BrtFloat,4,4);
MATRIXXY_CLASS(__BrtFloat,4,3);
MATRIXXY_CLASS(__BrtFloat,4,2);
MATRIXXY_CLASS(__BrtFloat,4,1);
MATRIXXY_CLASS(__BrtFloat,3,4);
MATRIXXY_CLASS(__BrtFloat,3,3);
MATRIXXY_CLASS(__BrtFloat,3,2);
MATRIXXY_CLASS(__BrtFloat,3,1);
MATRIXXY_CLASS(__BrtFloat,2,4);
MATRIXXY_CLASS(__BrtFloat,2,3);
MATRIXXY_CLASS(__BrtFloat,2,2);
MATRIXXY_CLASS(__BrtFloat,2,1);
MATRIXXY_CLASS(__BrtFloat,1,4);
MATRIXXY_CLASS(__BrtFloat,1,3);
MATRIXXY_CLASS(__BrtFloat,1,2);
MATRIXXY_CLASS(__BrtFloat,1,1);


MATRIXXY_CLASS(__BrtChar,4,4);
MATRIXXY_CLASS(__BrtChar,4,3);
MATRIXXY_CLASS(__BrtChar,4,2);
MATRIXXY_CLASS(__BrtChar,4,1);
MATRIXXY_CLASS(__BrtChar,3,4);
MATRIXXY_CLASS(__BrtChar,3,3);
MATRIXXY_CLASS(__BrtChar,3,2);
MATRIXXY_CLASS(__BrtChar,3,1);
MATRIXXY_CLASS(__BrtChar,2,4);
MATRIXXY_CLASS(__BrtChar,2,3);
MATRIXXY_CLASS(__BrtChar,2,2);
MATRIXXY_CLASS(__BrtChar,2,1);
MATRIXXY_CLASS(__BrtChar,1,4);
MATRIXXY_CLASS(__BrtChar,1,3);
MATRIXXY_CLASS(__BrtChar,1,2);
MATRIXXY_CLASS(__BrtChar,1,1);

#undef MATRIXXY_CLASS



#endif
#endif


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

#ifndef BRTVECTOR_HPP
#define BRTVECTOR_HPP
#if defined (_MSC_VER) && _MSC_VER <=1200 && !defined(VC6VECTOR_HPP)
#include "vc6vector.hpp"
//the above headerfile has the template functions automatically expanded.
//not needed for recent compilers.
#else

#include <iostream>
#include <math.h>

#include "janitorial.hpp"

#include "type_promotion.hpp"

typedef unsigned int uint;

template <class VALUE, unsigned int tsize> class vec;

template <class T, class B> inline T singlequestioncolon (const B& a,
                                                          const T&b,
                                                          const T&c){
    return a.questioncolon(b,c);
};
template <> inline float singlequestioncolon (const char &a,
                                              const float &b,
                                              const float &c) {
    return a?b:c;
}
template <> inline float singlequestioncolon (const float &a,
                                              const float &b,
                                              const float &c) {
    return FPNZ(a)?b:c;
}

template <> inline int singlequestioncolon (const char &a,
                                              const int &b,
                                              const int &c) {
    return a?b:c;
}

template <> inline int singlequestioncolon (const int &a,
                                              const int &b,
                                              const int &c) {
    return FPNZ(a)?b:c;
}

template <> inline uint singlequestioncolon (const char &a,
                                              const uint &b,
                                              const uint &c) {
    return a?b:c;
}

template <> inline uint singlequestioncolon (const uint &a,
                                              const uint &b,
                                              const uint &c) {
    return FPNZ(a)?b:c;
}

template <> inline char singlequestioncolon (const char &a,
                                              const char &b,
                                              const char &c) {
    return a?b:c;
}


template <> inline short singlequestioncolon (const char &a,
                                              const short &b,
                                              const short &c) {
    return a?b:c;
}

template <> inline short singlequestioncolon (const short &a,
                                              const short &b,
                                              const short &c) {
    return FPNZ(a)?b:c;
}

template <> inline ushort singlequestioncolon (const char &a,
                                              const ushort &b,
                                              const ushort &c) {
    return a?b:c;
}

template <> inline ushort singlequestioncolon (const ushort &a,
                                              const ushort &b,
                                              const ushort &c) {
    return FPNZ(a)?b:c;
}

template <> inline uchar singlequestioncolon (const char &a,
                                              const uchar &b,
                                              const uchar &c) {
    return a?b:c;
}

template <> inline uchar singlequestioncolon (const uchar &a,
                                              const uchar &b,
                                              const uchar &c) {
    return FPNZ(a)?b:c;
}

template <> inline double singlequestioncolon (const char &a,
                                               const double &b,
                                               const double &c) {
    return a?b:c;
}
template <> inline double singlequestioncolon (const double &a,
                                              const double &b,
                                              const double &c) {
    return DPNZ(a)?b:c;
}

template <> inline vec<float,1> singlequestioncolon (const vec<float,1> &a,
                                                     const vec<float,1> &b,
                                                     const vec<float,1> &c);
template <> inline vec<float,2> singlequestioncolon (const vec<float,2> & a,
                                                     const vec<float,2> &b,
                                                     const vec<float,2> &c);
template <> inline vec<float,3> singlequestioncolon (const vec<float,3> &a,
                                                     const vec<float,3> &b,
                                                     const vec<float,3> &c);
template <> inline vec<float,4> singlequestioncolon (const vec<float,4> &a,
                                                     const vec<float,4> &b,
                                                     const vec<float,4> &c);

template <> inline vec<double,1> singlequestioncolon (const vec<double,1> &a,
                                                      const vec<double,1> &b,
                                                      const vec<double,1> &c);
template <> inline vec<double,2> singlequestioncolon (const vec<double,2> & a,
                                                      const vec<double,2> &b,
                                                      const vec<double,2> &c);

template <> inline vec<int,1> singlequestioncolon (const vec<int,1> &a,
                                                     const vec<int,1> &b,
                                                     const vec<int,1> &c);
template <> inline vec<int,2> singlequestioncolon (const vec<int,2> & a,
                                                     const vec<int,2> &b,
                                                     const vec<int,2> &c);
template <> inline vec<int,3> singlequestioncolon (const vec<int,3> &a,
                                                     const vec<int,3> &b,
                                                     const vec<int,3> &c);
template <> inline vec<int,4> singlequestioncolon (const vec<int,4> &a,
                                                     const vec<int,4> &b,
                                                     const vec<int,4> &c);

template <> inline vec<uint,1> singlequestioncolon (const vec<uint,1> &a,
                                                     const vec<uint,1> &b,
                                                     const vec<uint,1> &c);
template <> inline vec<uint,2> singlequestioncolon (const vec<uint,2> & a,
                                                     const vec<uint,2> &b,
                                                     const vec<uint,2> &c);
template <> inline vec<uint,3> singlequestioncolon (const vec<uint,3> &a,
                                                     const vec<uint,3> &b,
                                                     const vec<uint,3> &c);
template <> inline vec<uint,4> singlequestioncolon (const vec<uint,4> &a,
                                                     const vec<uint,4> &b,
                                                     const vec<uint,4> &c);

template <> inline vec<char,1> singlequestioncolon (const vec<char,1> &a,
                                                     const vec<char,1> &b,
                                                     const vec<char,1> &c);
template <> inline vec<char,2> singlequestioncolon (const vec<char,2> & a,
                                                     const vec<char,2> &b,
                                                     const vec<char,2> &c);
template <> inline vec<char,3> singlequestioncolon (const vec<char,3> &a,
                                                     const vec<char,3> &b,
                                                     const vec<char,3> &c);
template <> inline vec<char,4> singlequestioncolon (const vec<char,4> &a,
                                                     const vec<char,4> &b,
                                                     const vec<char,4> &c);

template <> inline vec<uchar,1> singlequestioncolon (const vec<uchar,1> &a,
                                                     const vec<uchar,1> &b,
                                                     const vec<uchar,1> &c);
template <> inline vec<uchar,2> singlequestioncolon (const vec<uchar,2> & a,
                                                     const vec<uchar,2> &b,
                                                     const vec<uchar,2> &c);
template <> inline vec<uchar,3> singlequestioncolon (const vec<uchar,3> &a,
                                                     const vec<uchar,3> &b,
                                                     const vec<uchar,3> &c);
template <> inline vec<uchar,4> singlequestioncolon (const vec<uchar,4> &a,
                                                     const vec<uchar,4> &b,
                                                     const vec<uchar,4> &c);

template <> inline vec<short,1> singlequestioncolon (const vec<short,1> &a,
                                                     const vec<short,1> &b,
                                                     const vec<short,1> &c);
template <> inline vec<short,2> singlequestioncolon (const vec<short,2> & a,
                                                     const vec<short,2> &b,
                                                     const vec<short,2> &c);
template <> inline vec<short,3> singlequestioncolon (const vec<short,3> &a,
                                                     const vec<short,3> &b,
                                                     const vec<short,3> &c);
template <> inline vec<short,4> singlequestioncolon (const vec<short,4> &a,
                                                     const vec<short,4> &b,
                                                     const vec<short,4> &c);
template <> inline vec<ushort,1> singlequestioncolon (const vec<ushort,1> &a,
                                                     const vec<ushort,1> &b,
                                                     const vec<ushort,1> &c);
template <> inline vec<ushort,2> singlequestioncolon (const vec<ushort,2> & a,
                                                     const vec<ushort,2> &b,
                                                     const vec<ushort,2> &c);
template <> inline vec<ushort,3> singlequestioncolon (const vec<ushort,3> &a,
                                                     const vec<ushort,3> &b,
                                                     const vec<ushort,3> &c);
template <> inline vec<ushort,4> singlequestioncolon (const vec<ushort,4> &a,
                                                     const vec<ushort,4> &b,
                                                     const vec<ushort,4> &c);


template <typename T> T step_float (T a, T x){
   return (T)((x>=a)?1.0f:0.0f);
}
template <typename T> T max_float(T x, T y) {
   return (T)(x>y?x:y);
}
template<typename T> T min_float(T x, T y) {
   return (T)(x<y?x:y);
}
template<typename T> T ldexp_float(T x, T y) {
   return (T)ldexp(x,(int)y);
}
template <class T> class GetValueOf {public:
    typedef typename T::TYPE type;
};
template <> class GetValueOf <float> {public:
    typedef float type;
};
template <> class GetValueOf <double> {public:
    typedef double type;
};
template <> class GetValueOf <int> {public:
    typedef int type;
};
template <> class GetValueOf <unsigned int> {public:
    typedef unsigned int type;
};
template <> class GetValueOf <char> {public:
    typedef char type;
};
template <> class GetValueOf <unsigned char> {public:
    typedef unsigned char type;
};
template <> class GetValueOf <short> {public:
    typedef short type;
};
template <> class GetValueOf <unsigned short> {public:
    typedef unsigned short type;
};

template <> class GetValueOf <bool> {public:
    typedef bool type;
};
#if defined (_MSC_VER)
template <class T> class Holder {
public:
    static typename GetValueOf<T>::type getAt (const T&t, int i) {
        return t.getAt(i);
    }
};
#define HOLDER(TYP) template <> class Holder<TYP> { \
public: \
    static TYP getAt(TYP t, int i) { \
        return t; \
    } \
}
HOLDER(float);
HOLDER(double);
HOLDER(char);
HOLDER(unsigned char);
HOLDER(short);
HOLDER(unsigned short);
HOLDER(unsigned int);
HOLDER(int);
HOLDER(bool);
template <class T> typename GetValueOf<T>::type GetAt (const T& in,int i) {
    return Holder<T>::getAt(in,i);
}
#else
template <class T> static typename GetValueOf<T>::type GetAt (const T& in,int i) {
    return in.getAt(i);
}
#define SPECIALGETAT(TYP) template <> static TYP GetAt (const TYP& in,int UNUSED(i)) {return in;}

SPECIALGETAT(int)
SPECIALGETAT(unsigned int)
SPECIALGETAT(char)
SPECIALGETAT(unsigned char)
SPECIALGETAT(short)
SPECIALGETAT(unsigned short)
SPECIALGETAT(float)
SPECIALGETAT(double)
SPECIALGETAT(bool)

#endif
template <class T> class BracketType {public:
  typedef T type;
};
template <> class BracketType <float> {public:
  typedef vec<float,1> type;
};
template <> class BracketType <double> {public:
  typedef vec<double,1> type;
};
template <> class BracketType <int> {public:
  typedef vec<int,1> type;
};
template <> class BracketType <unsigned int> {public:
  typedef vec<uint,1> type;
};
template <> class BracketType <char> {public:
  typedef vec<char,1> type;
};
template <> class BracketType <unsigned char> {public:
  typedef vec<uchar,1> type;
};
template <> class BracketType <short> {public:
  typedef vec<short,1> type;
};
template <> class BracketType <unsigned short> {public:
  typedef vec<ushort,1> type;
};

template <class T> class BracketOp {public:
  template <class U> T& operator ()(const U&u, unsigned int i) {
    return u.unsafeGetAt(i);
  }
  template <class U> T& operator () (U&u, unsigned int i) {
    return u.unsafeGetAt(i);
  }
};
template <> class BracketOp <float> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<float,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <double> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<double,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <int> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<int,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <uint> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<uint,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <char> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<char,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <uchar> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<uchar,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <short> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<short,1>(u.unsafeGetAt(i));
  }
};
template <> class BracketOp <unsigned short> {public:
  template <class U> U operator ()(const U&u, unsigned int i) {
     return vec<ushort,1>(u.unsafeGetAt(i));
  }
};

namespace brt
{
    enum MASKS
    {
        maskX=0,
        maskY=1,
        maskZ=2,
        maskW=3
    };
};

template <class T> class InitializeClass {public:
    template <class V> T operator () (const V&a, const V&b, const V&c,
const V&d) {
        return T(a,b,c,d);
    }
};
#define INITBASECLASS(MYTYPE) template <> class InitializeClass<MYTYPE> { \
 public: \
    template <class V> MYTYPE operator () (const V&a, \
                                          const V& UNUSED(b),  \
                                          const V& UNUSED(c),  \
                                          const V& UNUSED(d)) { \
      return (MYTYPE)a; \
    } \
}
INITBASECLASS(float);
INITBASECLASS(double);
INITBASECLASS(int);
INITBASECLASS(unsigned int);
INITBASECLASS(char);
INITBASECLASS(unsigned char);
INITBASECLASS(short);
INITBASECLASS(unsigned short);


template <> class InitializeClass<bool> {
 public:
    template <class V> bool operator () (const V&a,
                                          const V&b,
                                          const V&c,
                                          const V&d) {
      return (a||b||c||d)?true:false;
    }
};

#ifdef _MSC_VER
#if _MSC_VER <= 1200
#define GCCTYPENAME
#define INTERNALTYPENAME
#else
#define GCCTYPENAME typename
#define INTERNALTYPENAME typename
#endif
#else
#define GCCTYPENAME typename
#define INTERNALTYPENAME typename
#endif

template <class VALUE, unsigned int tsize> class vec{
public:
    typedef VALUE TYPE;
    enum SIZ{size=tsize};
    typedef VALUE array_type[size];
protected:
    VALUE f[size];
public:
    const VALUE &getAt (unsigned int i) const{
       return i<size?f[i]:f[size-1];
    }
    VALUE &getAt (unsigned int i) {
       return i<size?f[i]:f[size-1];
    }
    const VALUE &unsafeGetAt (unsigned int i) const{return f[i];}
    VALUE &unsafeGetAt (unsigned int i) {return f[i];}
    typename BracketType<VALUE>::type operator [] (int i)const {return BracketOp<VALUE>()(*this,i);}
    vec<VALUE,tsize>& gather() {
        return *this;
    }
    const vec<VALUE,tsize>& gather() const{
        return *this;
    }
    template<class BRT_TYPE> BRT_TYPE castTo()const {
        return InitializeClass<BRT_TYPE>()(getAt(0),
                                           getAt(1),
                                           getAt(2),
                                           getAt(3));
    }

    template<class BRT_TYPE> BRT_TYPE castToArg(const BRT_TYPE&)const{
        return InitializeClass<BRT_TYPE>()(getAt(0),
                                           getAt(1),
                                           getAt(2),
                                           getAt(3));
    }
   vec<VALUE,1> any() const{
      return vec<VALUE,1>(getAt(0)!=0.0f||getAt(1)!=0.0f||getAt(2)!=0.0f||getAt(3)!=0.0f);
   }
   vec<VALUE,1> all() const {
      return vec<VALUE,1>(getAt(0)!=0.0f&&getAt(1)!=0.0f&&getAt(2)!=0.0f&&getAt(3)!=0.0f);
   }
   vec<VALUE,1> length() const {
      unsigned int i;
      VALUE tot = unsafeGetAt(0);
      tot*=tot;
      for (i=1;i<tsize;++i) {
         tot+=unsafeGetAt(i)*unsafeGetAt(i);
      }
      return vec<VALUE,1>((VALUE)sqrt(tot));
   }
#define BROOK_UNARY_OP(op) vec<VALUE,tsize> operator op ()const { \
      return vec<VALUE, tsize > (op getAt(0),  \
                                 op getAt(1),  \
                                 op getAt(2),  \
                                 op getAt(3)); \
    }
    BROOK_UNARY_OP(+)
    BROOK_UNARY_OP(-)
    BROOK_UNARY_OP(!)
#undef BROOK_UNARY_OP
#define NONCONST_BROOK_UNARY_OP(op) vec<VALUE,tsize> operator op (){ \
      return vec<VALUE, tsize > (op getAt(0),  \
                                 tsize>1?op getAt(1):getAt(1),  \
                                 tsize>2?op getAt(2):getAt(2),  \
                                 tsize>3?op getAt(3):getAt(3)); \
    }
    NONCONST_BROOK_UNARY_OP(--);
    NONCONST_BROOK_UNARY_OP(++);
#undef BROOK_UNARY_OP
    vec<VALUE,4> swizzle4(int x,int y,int z,int w)const {
        return vec<VALUE,4>(unsafeGetAt(x),
                            unsafeGetAt(y),
                            unsafeGetAt(z),
                            unsafeGetAt(w));
    }
    vec<VALUE,3> swizzle3(int x,int y,int z)const {
        return vec<VALUE,3>(unsafeGetAt(x),unsafeGetAt(y),unsafeGetAt(z));
    }
    vec<VALUE,2> swizzle2(int x,int y)const {
        return vec<VALUE,2>(unsafeGetAt(x),unsafeGetAt(y));
    }
    vec<VALUE, 1> swizzle1(int x)const {
        return vec<VALUE,1>(unsafeGetAt(x));
    }
    vec() {}
#define GENERAL_TEMPLATIZED_FUNCTIONS
    template <class BRT_TYPE>
      vec (const BRT_TYPE &inx,
           const BRT_TYPE &iny,
           const BRT_TYPE &inz,
           const BRT_TYPE& inw) {
        f[0]=inx;
        if (size>1) f[1]=iny;
        if (size>2) f[2]=inz;
        if (size>3) f[3]=inw;
    }
    template <class BRT_TYPE> vec (const BRT_TYPE& inx,
                                   const BRT_TYPE& iny,
                                   const BRT_TYPE& inz) {
        f[0]=inx;
        if(size>1)f[1]=iny;
        if(size>2)f[2]=inz;
    }
    template <class BRT_TYPE> vec (const BRT_TYPE& inx, const BRT_TYPE& iny) {
        f[0]=inx;
        if (size>1) f[1]=iny;
    }
    template <class BRT_TYPE> vec (const BRT_TYPE& scalar) {
        (*this)=scalar;
    }
    template <class BRT_TYPE> operator BRT_TYPE () const{
      return InitializeClass<BRT_TYPE>()(getAt(0),getAt(1),getAt(2),getAt(3));
    }

#define ASSIGN_OP(op) template <class BRT_TYPE> \
         vec<VALUE,tsize>& operator op (const BRT_TYPE & in) {  \
        f[0] op (VALUE)(GetAt<BRT_TYPE>(in,0));  \
        if (tsize>1) f[1] op (VALUE)(GetAt<BRT_TYPE>(in,1));  \
        if (tsize>2) f[2] op (VALUE)(GetAt<BRT_TYPE>(in,2));  \
        if (tsize>3) f[3] op (VALUE)(GetAt<BRT_TYPE>(in,3));  \
        return *this;  \
    }
    ASSIGN_OP(=);
    ASSIGN_OP(/=);
    ASSIGN_OP(+=);
    ASSIGN_OP(-=);
    ASSIGN_OP(*=);
    ASSIGN_OP(%=);

#undef ASSIGN_OP
#undef GENERAL_TEMPLATIZED_FUNCTIONS
#define VECTOR_TEMPLATIZED_FUNCTIONS
    template <class BRT_TYPE>
      vec<VALUE,4> mask4 (const BRT_TYPE&in,int X, int Y,int Z,int W) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        f[Y]=GetAt<BRT_TYPE>(in,1);
        f[Z]=GetAt<BRT_TYPE>(in,2);
        f[W]=GetAt<BRT_TYPE>(in,3);
        return vec<VALUE,4>(unsafeGetAt(X),
                            unsafeGetAt(Y),
                            unsafeGetAt(Z),
                            unsafeGetAt(W));
    }
    template <class BRT_TYPE>
      vec<VALUE,3> mask3 (const BRT_TYPE&in,int X,int Y,int Z) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        f[Y]=GetAt<BRT_TYPE>(in,1);
        f[Z]=GetAt<BRT_TYPE>(in,2);
        return vec<VALUE,3>(unsafeGetAt(X),unsafeGetAt(Y),unsafeGetAt(Z));
    }
    template <class BRT_TYPE>
      vec<VALUE,2> mask2 (const BRT_TYPE&in,int X,int Y) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        f[Y]=GetAt<BRT_TYPE>(in,1);
        return vec<VALUE,2>(unsafeGetAt(X),unsafeGetAt(Y));
    }
    template <class BRT_TYPE>
      vec<VALUE,1> mask1 (const BRT_TYPE&in,int X) {
        f[X]=GetAt<BRT_TYPE>(in,0);
        return vec<VALUE,1>(unsafeGetAt(X));
    }
    template <class BRT_TYPE>
      vec<typename BRT_TYPE::TYPE,
          LUB<BRT_TYPE::size,tsize>::size> questioncolon(const BRT_TYPE &b,
                                                const BRT_TYPE &c)const {
        return vec<GCCTYPENAME BRT_TYPE::TYPE,
                   LUB<BRT_TYPE::size,tsize>::size>
            (singlequestioncolon(getAt(0),b.getAt(0),c.getAt(0)),
             singlequestioncolon(getAt(1),b.getAt(1),c.getAt(1)),
             singlequestioncolon(getAt(2),b.getAt(2),c.getAt(2)),
             singlequestioncolon(getAt(3),b.getAt(3),c.getAt(3)));
    }

#if defined (_MSC_VER) && (_MSC_VER <= 1200)
#define TEMPL_TYPESIZE sizeof(BRT_TYPE)/sizeof(BRT_TYPE::TYPE)
#else
#define TEMPL_TYPESIZE BRT_TYPE::size
#endif
#define BROOK_BINARY_OP(op,opgets,TYPESPECIFIER) template <class BRT_TYPE>          \
    vec<GCCTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type, \
       LUB<TEMPL_TYPESIZE,tsize>::size> operator op (const BRT_TYPE &b)const{ \
      return vec<INTERNALTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE, \
                                                VALUE>::type, \
                 LUB<TEMPL_TYPESIZE,tsize>::size>(*this) opgets b; \
    }
    BROOK_BINARY_OP(*,*=,LCM);
    BROOK_BINARY_OP(/,/=,LCM);
    BROOK_BINARY_OP(+,+=,LCM);
    BROOK_BINARY_OP(-,-=,LCM);
    BROOK_BINARY_OP(%,%=,LCM);
#undef BROOK_BINARY_OP
#define BROOK_BINARY_OP(op,TYPESPECIFIER) template <class BRT_TYPE>          \
    vec<GCCTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type, \
       LUB<TEMPL_TYPESIZE,tsize>::size> operator op (const BRT_TYPE &b)const{ \
      return vec<INTERNALTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE, \
                                           VALUE>::type, \
                 LUB<TEMPL_TYPESIZE,tsize>::size> \
                (getAt(0) op GetAt<BRT_TYPE>(b,0), \
                 getAt(1) op GetAt<BRT_TYPE>(b,1), \
                 getAt(2) op GetAt<BRT_TYPE>(b,2), \
                 getAt(3) op GetAt<BRT_TYPE>(b,3)); \
    }
    BROOK_BINARY_OP(||,LCM);
    BROOK_BINARY_OP(&&,LCM);
    BROOK_BINARY_OP(>>,LCM);
    BROOK_BINARY_OP(<<,LCM);
    BROOK_BINARY_OP(&,LCM);
    BROOK_BINARY_OP(|,LCM);
    BROOK_BINARY_OP(^,LCM);

  // The following two templates explicitly added because VS2008 doesn't infer
  // types the way the other compilers we use do
  //
  // FIXME - work out exactly what's needed to get these cases automatically generated
  //

#ifdef _MSC_VER
#if _MSC_VER >= 1500
  template <class BRT_TYPE> vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, 1>
     operator || (const BRT_TYPE &b) const
     {
       return vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, LUB<BRT_TYPE::size,tsize>::size>
               (getAt(0) ||
                GetAt<BRT_TYPE>(b,0), getAt(1) ||
                GetAt<BRT_TYPE>(b,1), getAt(2) ||
                GetAt<BRT_TYPE>(b,2), getAt(3) ||
                GetAt<BRT_TYPE>(b,3));
     };

  template <class BRT_TYPE> vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, 1>
      operator && (const BRT_TYPE &b) const
      {
        return vec<typename LCM<typename BRT_TYPE::TYPE, VALUE>::type, LUB<BRT_TYPE::size,tsize>::size>
          (getAt(0) &&
           GetAt<BRT_TYPE>(b,0), getAt(1) &&
           GetAt<BRT_TYPE>(b,1), getAt(2) &&
           GetAt<BRT_TYPE>(b,2), getAt(3) &&
           GetAt<BRT_TYPE>(b,3));
      };

    template <class BRT_TYPE> vec<typename LCM<typename BRT_TYPE::TYPE,VALUE>::type, 1>
      questioncolon(const BRT_TYPE &b, const BRT_TYPE &c)const {
      return vec<typename LCM<typename BRT_TYPE::TYPE, VALUE>::type, LUB<BRT_TYPE::size,tsize>::size>
            (singlequestioncolon(getAt(0),b.getAt(0),c.getAt(0)),
             singlequestioncolon(getAt(1),b.getAt(1),c.getAt(1)),
             singlequestioncolon(getAt(2),b.getAt(2),c.getAt(2)),
             singlequestioncolon(getAt(3),b.getAt(3),c.getAt(3)));
    }

  // I don't understand why VS 2008 won't pick up the generic template for (*)
  // in the case of __BrtDouble1, but this fixes it for the moment.
  // FIXME: Need to resolve if this is an issue with the code base or VS 2008.
  
  vec<typename LCM<char,char>::type, 1>
      operator * (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) *= b;
  }

  vec<typename LCM<char,char>::type, 1>
      operator / (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) /= b;
  }

  vec<typename LCM<char,char>::type, 1>
      operator + (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) += b;
  }

  vec<typename LCM<char,char>::type, 1>
      operator - (const vec<char,1> &b) const
  {
      return vec<typename LCM<char,char>::type,1> (*this) -= b;
  }

  vec<typename LCM<char,char>::type, 2>
      operator * (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 2>
      operator / (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 2>
      operator + (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 2>
      operator - (const vec<char,2> &b) const
  {
      return vec<typename LCM<char,char>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<char,char>::type, 3>
      operator * (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<char,char>::type, 3>
      operator / (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<char,char>::type, 3>
      operator + (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<char,char>::type, 3>
      operator - (const vec<char,3> &b) const
  {
      return vec<typename LCM<char,char>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<char,char>::type, 4>
      operator * (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<char,char>::type, 4>
      operator / (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<char,char>::type, 4>
      operator + (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<char,char>::type, 4>
      operator - (const vec<char,4> &b) const
  {
      return vec<typename LCM<char,char>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

    vec<typename LCM<short,short>::type, 1>
      operator * (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) *= b;
  }

  vec<typename LCM<short,short>::type, 1>
      operator / (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) /= b;
  }

  vec<typename LCM<short,short>::type, 1>
      operator + (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) += b;
  }

  vec<typename LCM<short,short>::type, 1>
      operator - (const vec<short,1> &b) const
  {
      return vec<typename LCM<short,short>::type,1> (*this) -= b;
  }

  vec<typename LCM<short,short>::type, 2>
      operator * (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 2>
      operator / (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 2>
      operator + (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 2>
      operator - (const vec<short,2> &b) const
  {
      return vec<typename LCM<short,short>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<short,short>::type, 3>
      operator * (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<short,short>::type, 3>
      operator / (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<short,short>::type, 3>
      operator + (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<short,short>::type, 3>
      operator - (const vec<short,3> &b) const
  {
      return vec<typename LCM<short,short>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<short,short>::type, 4>
      operator * (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<short,short>::type, 4>
      operator / (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<short,short>::type, 4>
      operator + (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<short,short>::type, 4>
      operator - (const vec<short,4> &b) const
  {
      return vec<typename LCM<short,short>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

    vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator * (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) *= b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator / (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) /= b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator + (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) += b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 1>
      operator - (const vec<unsigned char,1> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,1> (*this) -= b;
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator * (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator / (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator + (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 2>
      operator - (const vec<unsigned char,2> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator * (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator / (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator + (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 3>
      operator - (const vec<unsigned char,3> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator * (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator / (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator + (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<unsigned char,unsigned char>::type, 4>
      operator - (const vec<unsigned char,4> &b) const
  {
      return vec<typename LCM<unsigned char,unsigned char>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

    vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator * (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) *= b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator / (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) /= b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator + (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) += b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 1>
      operator - (const vec<unsigned short,1> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,1> (*this) -= b;
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator * (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator / (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator + (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 2>
      operator - (const vec<unsigned short,2> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator * (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator / (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator + (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 3>
      operator - (const vec<unsigned short,3> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,3>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2));
  }
  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator * (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1), \
                                                      getAt(2) * b.getAt(2), \
                                                      getAt(3) * b.getAt(3));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator / (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1), \
                                                      getAt(2) / b.getAt(2), \
                                                      getAt(3) / b.getAt(3));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator + (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1), \
                                                      getAt(2) + b.getAt(2), \
                                                      getAt(3) + b.getAt(3));
  }

  vec<typename LCM<unsigned short,unsigned short>::type, 4>
      operator - (const vec<unsigned short,4> &b) const
  {
      return vec<typename LCM<unsigned short,unsigned short>::type,4>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1), \
                                                      getAt(2) - b.getAt(2), \
                                                      getAt(3) - b.getAt(3));
  }

  vec<typename LCM<double,double>::type, 1>
      operator * (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) *= b;
  }

  vec<typename LCM<double,double>::type, 1>
      operator / (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) /= b;
  }

  vec<typename LCM<double,double>::type, 1>
      operator + (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) += b;
  }

  vec<typename LCM<double,double>::type, 1>
      operator - (const vec<double,1> &b) const
  {
      return vec<typename LCM<double,double>::type,1> (*this) -= b;
  }

  vec<typename LCM<double,double>::type, 2>
      operator * (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) * b.getAt(0), \
                                                      getAt(1) * b.getAt(1));
  }

  vec<typename LCM<double,double>::type, 2>
      operator / (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) / b.getAt(0), \
                                                      getAt(1) / b.getAt(1));
  }

  vec<typename LCM<double,double>::type, 2>
      operator + (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) + b.getAt(0), \
                                                      getAt(1) + b.getAt(1));
  }

  vec<typename LCM<double,double>::type, 2>
      operator - (const vec<double,2> &b) const
  {
      return vec<typename LCM<double,double>::type,2>(getAt(0) - b.getAt(0), \
                                                      getAt(1) - b.getAt(1));
  }

#endif
#endif

    BROOK_BINARY_OP(<,COMMON_CHAR)
    BROOK_BINARY_OP(>,COMMON_CHAR)
    BROOK_BINARY_OP(<=,COMMON_CHAR)
    BROOK_BINARY_OP(>=,COMMON_CHAR)
    BROOK_BINARY_OP(!=,COMMON_CHAR)
    BROOK_BINARY_OP(==,COMMON_CHAR)
#undef BROOK_BINARY_OP
template <class BRT_TYPE>
    vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type,1>
    dot (const BRT_TYPE &b) const{
      return vec<INTERNALTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,
                                           VALUE>::type, 1>
                ((LUB<TEMPL_TYPESIZE,
                     tsize>::size)==4?(getAt(0) * GetAt<BRT_TYPE>(b,0) +
                                     getAt(1) * GetAt<BRT_TYPE>(b,1) +
                                     getAt(2) * GetAt<BRT_TYPE>(b,2) +
                                     getAt(3) * GetAt<BRT_TYPE>(b,3)):
                 (LUB<TEMPL_TYPESIZE,
                     tsize>::size)==3?(getAt(0) * GetAt<BRT_TYPE>(b,0) +
                                      getAt(1) * GetAt<BRT_TYPE>(b,1) +
                                      getAt(2) * GetAt<BRT_TYPE>(b,2)):
                 (LUB<TEMPL_TYPESIZE,
                     tsize>::size)==2?(getAt(0) * GetAt<BRT_TYPE>(b,0) +
                                      getAt(1) * GetAt<BRT_TYPE>(b,1)):
                 getAt(0) * GetAt<BRT_TYPE>(b,0));

    }

template <class BRT_TYPE>
    vec<GCCTYPENAME LCM<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type,1>
    distance (const BRT_TYPE &b) const{
      return (b-*this).length();
    }



#define BROOK_BINARY_OP(op, subop,TYPESPECIFIER) template <class BRT_TYPE>          \
    vec<GCCTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE,VALUE>::type, \
       LUB<TEMPL_TYPESIZE,tsize>::size> op (const BRT_TYPE &b)const{ \
      return vec<INTERNALTYPENAME TYPESPECIFIER<GCCTYPENAME BRT_TYPE::TYPE, \
                                           VALUE>::type, \
                 LUB<TEMPL_TYPESIZE,tsize>::size> \
        (::subop(getAt(0) , GetAt<BRT_TYPE>(b,0)),  \
         ::subop(getAt(1) , GetAt<BRT_TYPE>(b,1)),  \
         ::subop(getAt(2) , GetAt<BRT_TYPE>(b,2)),   \
         ::subop(getAt(3) , GetAt<BRT_TYPE>(b,3)));  \
    }
    BROOK_BINARY_OP(atan2,atan2,LCM)
    BROOK_BINARY_OP(fmod,fmod,LCM)
    BROOK_BINARY_OP(pow,pow,LCM);
    BROOK_BINARY_OP(step_float,step_float,LCM);
    BROOK_BINARY_OP(ldexp_float,ldexp_float,LCM);
    BROOK_BINARY_OP(min_float,min_float,LCM);
    BROOK_BINARY_OP(max_float,max_float,LCM);
#undef TEMPL_TYPESIZE
#undef BROOK_BINARY_OP
#undef VECTOR_TEMPLATIZED_FUNCTIONS
};


template <> inline vec<float,1> singlequestioncolon (const vec<float,1> &a,
                                                     const vec<float,1> &b,
                                                     const vec<float,1> &c) {
    return FPNZ(a.unsafeGetAt(0))?b:c;
}

template <> inline vec<float,2> singlequestioncolon (const vec<float,2> & a,
                                                     const vec<float,2> &b,
                                                     const vec<float,2> &c) {
    return vec<float,2> (FPNZ(a.unsafeGetAt(0))?b.unsafeGetAt(0):c.unsafeGetAt(0),
                         FPNZ(a.unsafeGetAt(1))?b.unsafeGetAt(1):c.unsafeGetAt(1));
}
template <> inline vec<float,3> singlequestioncolon (const vec<float,3> &a,
                                              const vec<float,3> &b,
                                              const vec<float,3> &c) {
    return vec<float,3> (FPNZ(a.unsafeGetAt(0))?b.unsafeGetAt(0):c.unsafeGetAt(0),
                         FPNZ(a.unsafeGetAt(1))?b.unsafeGetAt(1):c.unsafeGetAt(1),
                         FPNZ(a.unsafeGetAt(2))?b.unsafeGetAt(2):c.unsafeGetAt(2));
}
template <> inline vec<float,4> singlequestioncolon (const vec<float,4> &a,
                                              const vec<float,4> &b,
                                              const vec<float,4> &c) {
    return vec<float,4> (FPNZ(a.unsafeGetAt(0))?b.unsafeGetAt(0):c.unsafeGetAt(0),
                         FPNZ(a.unsafeGetAt(1))?b.unsafeGetAt(1):c.unsafeGetAt(1),
                         FPNZ(a.unsafeGetAt(2))?b.unsafeGetAt(2):c.unsafeGetAt(2),
                         FPNZ(a.unsafeGetAt(3))?b.unsafeGetAt(3):c.unsafeGetAt(3));
}

template <> inline vec<int,1> singlequestioncolon (const vec<int,1> &a,
                                                   const vec<int,1> &b,
                                                   const vec<int,1> &c) {
    return a.unsafeGetAt(0)?b:c;
}

template <> inline vec<int,2> singlequestioncolon (const vec<int,2> & a,
                                                   const vec<int,2> &b,
                                                   const vec<int,2> &c) {
    return vec<int,2> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                       a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1));
}
template <> inline vec<int,3> singlequestioncolon (const vec<int,3> &a,
                                                   const vec<int,3> &b,
                                                   const vec<int,3> &c) {
    return vec<int,3> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                       a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                       a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2));
}
template <> inline vec<int,4> singlequestioncolon (const vec<int,4> &a,
                                              const vec<int,4> &b,
                                              const vec<int,4> &c) {
    return vec<int,4> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                       a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                       a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2),
                       a.unsafeGetAt(3)?b.unsafeGetAt(3):c.unsafeGetAt(3));
}

template <> inline vec<uint,1> singlequestioncolon (const vec<uint,1> &a,
                                                    const vec<uint,1> &b,
                                                    const vec<uint,1> &c) {
    return a.unsafeGetAt(0)?b:c;
}

template <> inline vec<uint,2> singlequestioncolon (const vec<uint,2> & a,
                                                    const vec<uint,2> &b,
                                                    const vec<uint,2> &c) {
    return vec<uint,2> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                        a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1));
}
template <> inline vec<uint,3> singlequestioncolon (const vec<uint,3> &a,
                                                    const vec<uint,3> &b,
                                                    const vec<uint,3> &c) {
    return vec<uint,3> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                        a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                        a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2));
}
template <> inline vec<uint,4> singlequestioncolon (const vec<uint,4> &a,
                                                    const vec<uint,4> &b,
                                                    const vec<uint,4> &c) {
    return vec<uint,4> (a.unsafeGetAt(0)?b.unsafeGetAt(0):c.unsafeGetAt(0),
                        a.unsafeGetAt(1)?b.unsafeGetAt(1):c.unsafeGetAt(1),
                        a.unsafeGetAt(2)?b.unsafeGetAt(2):c.unsafeGetAt(2),
                        a.unsafeGetAt(3)?b.unsafeGetAt(3):c.unsafeGetAt(3));
}


template <class T>
  std::ostream& operator^ (std::ostream& os, const T & a){
    if (T::size==1) {
        os << a.getAt(0);
    }else {
        os << "{";
        for (unsigned int i=0;i<T::size;++i) {
            os << a.getAt(i)<<(i!=T::size-1?", ":"");
        }
        os << "}";
    }
    return os;
}

#define VECX_CLASS(NAME,TYPE,X) \
inline std::ostream& operator << (std::ostream&a,const vec<TYPE,X> & b) { \
    return a^b; \
}   \
typedef vec<TYPE,X> NAME

VECX_CLASS(__BrtFloat1,float,1);
VECX_CLASS(__BrtFloat2,float,2);
VECX_CLASS(__BrtFloat3,float,3);
VECX_CLASS(__BrtFloat4,float,4);
VECX_CLASS(__BrtChar1,char,1);
VECX_CLASS(__BrtChar2,char,2);
VECX_CLASS(__BrtChar3,char,3);
VECX_CLASS(__BrtChar4,char,4);
VECX_CLASS(__BrtUChar1,unsigned char,1);
VECX_CLASS(__BrtUChar2,unsigned char,2);
VECX_CLASS(__BrtUChar3,unsigned char,3);
VECX_CLASS(__BrtUChar4,unsigned char,4);
VECX_CLASS(__BrtShort1,short,1);
VECX_CLASS(__BrtShort2,short,2);
VECX_CLASS(__BrtShort3,short,3);
VECX_CLASS(__BrtShort4,short,4);
VECX_CLASS(__BrtUShort1,unsigned short,1);
VECX_CLASS(__BrtUShort2,unsigned short,2);
VECX_CLASS(__BrtUShort3,unsigned short,3);
VECX_CLASS(__BrtUShort4,unsigned short,4);
VECX_CLASS(__BrtDouble1,double,1);
VECX_CLASS(__BrtDouble2,double,2);
VECX_CLASS(__BrtInt1,int,1);
VECX_CLASS(__BrtInt2,int,2);
VECX_CLASS(__BrtInt3,int,3);
VECX_CLASS(__BrtInt4,int,4);
VECX_CLASS(__BrtUInt1,unsigned int,1);
VECX_CLASS(__BrtUInt2,unsigned int,2);
VECX_CLASS(__BrtUInt3,unsigned int,3);
VECX_CLASS(__BrtUInt4,unsigned int,4);
#undef VECX_CLASS
#define MATRIXXY_CLASS(TYPE,X,Y) \
inline std::ostream& operator << (std::ostream&a, \
                                  const vec<TYPE##X,Y> & b) { \
    return a^b; \
}   \
typedef vec<TYPE##X,Y> TYPE##X##x##Y

MATRIXXY_CLASS(__BrtFloat,4,4);
MATRIXXY_CLASS(__BrtFloat,4,3);
MATRIXXY_CLASS(__BrtFloat,4,2);
MATRIXXY_CLASS(__BrtFloat,4,1);
MATRIXXY_CLASS(__BrtFloat,3,4);
MATRIXXY_CLASS(__BrtFloat,3,3);
MATRIXXY_CLASS(__BrtFloat,3,2);
MATRIXXY_CLASS(__BrtFloat,3,1);
MATRIXXY_CLASS(__BrtFloat,2,4);
MATRIXXY_CLASS(__BrtFloat,2,3);
MATRIXXY_CLASS(__BrtFloat,2,2);
MATRIXXY_CLASS(__BrtFloat,2,1);
MATRIXXY_CLASS(__BrtFloat,1,4);
MATRIXXY_CLASS(__BrtFloat,1,3);
MATRIXXY_CLASS(__BrtFloat,1,2);
MATRIXXY_CLASS(__BrtFloat,1,1);


MATRIXXY_CLASS(__BrtChar,4,4);
MATRIXXY_CLASS(__BrtChar,4,3);
MATRIXXY_CLASS(__BrtChar,4,2);
MATRIXXY_CLASS(__BrtChar,4,1);
MATRIXXY_CLASS(__BrtChar,3,4);
MATRIXXY_CLASS(__BrtChar,3,3);
MATRIXXY_CLASS(__BrtChar,3,2);
MATRIXXY_CLASS(__BrtChar,3,1);
MATRIXXY_CLASS(__BrtChar,2,4);
MATRIXXY_CLASS(__BrtChar,2,3);
MATRIXXY_CLASS(__BrtChar,2,2);
MATRIXXY_CLASS(__BrtChar,2,1);
MATRIXXY_CLASS(__BrtChar,1,4);
MATRIXXY_CLASS(__BrtChar,1,3);
MATRIXXY_CLASS(__BrtChar,1,2);
MATRIXXY_CLASS(__BrtChar,1,1);

#undef MATRIXXY_CLASS



#endif
#endif

