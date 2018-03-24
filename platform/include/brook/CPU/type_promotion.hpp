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

template <unsigned int sizeA, unsigned int sizeB> class LUB {
public:
    enum SIZ{size=((sizeA==0&&sizeB==0)?1:
                   (sizeA==1)?sizeB:(sizeB==1)?sizeA:
                   (sizeA>sizeB)?sizeB:sizeA)
    };
};
template<> class LUB<1,1> {public:
    enum SIZ{size=1};
};
template<> class LUB<1,2> {public:
    enum SIZ{size=2};
};
template<> class LUB<2,1> {public:
    enum SIZ{size=2};
};
template<> class LUB<1,3> {public:
    enum SIZ{size=3};
};
template<> class LUB<3,1> {public:
    enum SIZ{size=3};
};
template<> class LUB<1,4> {public:
    enum SIZ{size=4};
};
template<> class LUB<4,1> {public:
    enum SIZ{size=4};
};
template<> class LUB<2,2> {public:
    enum SIZ{size=2};
};
template<> class LUB<2,3> {public:
    enum SIZ{size=2};
};
template<> class LUB<3,2> {public:
    enum SIZ{size=2};
};
template<> class LUB<2,4> {public:
    enum SIZ{size=2};
};
template<> class LUB<4,2> {public:
    enum SIZ{size=2};
};
template<> class LUB<3,3> {public:
    enum SIZ{size=3};
};
template<> class LUB<3,4> {public:
    enum SIZ{size=3};
};
template<> class LUB<4,3> {public:
    enum SIZ{size=3};
};
template<> class LUB<4,4> {public:
    enum SIZ{size=4};
};

template <class VALUE, unsigned int tsize> class vec;

// FIXME - this needs revisiting, several C promotion rules get violated here

template <class A, class B> class LCM {
public:
    typedef vec<typename LCM<typename A::TYPE , typename B::TYPE>::type,
                LUB<A::size,B::size>::size> type;
};
template <> class LCM<float,float> {public:
    typedef float type;
};
template <> class LCM<float,double> {public:
    typedef double type;
};
template <> class LCM<double,float> {public:
    typedef double type;
};
template <> class LCM<double,double> {public:
    typedef double type;
};
template <> class LCM<char,float> {public:
    typedef float type;
};
template <> class LCM<float,char> {public:
    typedef float type;
};
template <> class LCM<char,char> {public:
    typedef char type;
};
template <> class LCM<double,char> {public:
    typedef double type;
};
template <> class LCM<char,double> {public:
    typedef double type;
};
template <> class LCM<unsigned char,float> {public:
    typedef float type;
};
template <> class LCM<float,unsigned char> {public:
    typedef float type;
};
template <> class LCM<unsigned char,unsigned char> {public:
    typedef unsigned char type;
};
template <> class LCM<double,unsigned char> {public:
    typedef double type;
};
template <> class LCM<unsigned char,double> {public:
    typedef double type;
};

template <> class LCM<unsigned short,float> {public:
    typedef float type;
};
template <> class LCM<float,unsigned short> {public:
    typedef float type;
};
template <> class LCM<unsigned short,unsigned short> {public:
    typedef unsigned short type;
};
template <> class LCM<double,unsigned short> {public:
    typedef double type;
};
template <> class LCM<unsigned short,double> {public:
    typedef double type;
};

template <> class LCM<short,float> {public:
    typedef float type;
};
template <> class LCM<float,short> {public:
    typedef float type;
};
template <> class LCM<short,short> {public:
    typedef short type;
};
template <> class LCM<double,short> {public:
    typedef double type;
};
template <> class LCM<short,double> {public:
    typedef double type;
};

template <> class LCM<int,int> {public:
    typedef int type;
};
template <> class LCM<int,float> {public:
    typedef float type;
};
template <> class LCM<float,int> {public:
    typedef float type;
};
template <> class LCM<int,double> {public:
    typedef float type;
};
template <> class LCM<double,int> {public:
    typedef float type;
};

template <> class LCM<int,char> {public:
    typedef int type;
};
template <> class LCM<char,int> {public:
    typedef int type;
};

template <> class LCM<int,short> {public:
    typedef int type;
};
template <> class LCM<short,int> {public:
    typedef int type;
};

template <> class LCM<int,unsigned short> {public:
    typedef int type;
};
template <> class LCM<unsigned short,int> {public:
    typedef int type;
};

template <> class LCM<int,unsigned char> {public:
    typedef int type;
};
template <> class LCM<unsigned char,int> {public:
    typedef int type;
};

template <> class LCM<unsigned int, unsigned int> {public:
    typedef unsigned int type;
};
template <> class LCM<unsigned int,float> {public:
    typedef float type;
};
template <> class LCM<float,unsigned int> {public:
    typedef float type;
};
template <> class LCM<unsigned int,double> {public:
    typedef float type;
};
template <> class LCM<double,unsigned int> {public:
    typedef float type;
};
template <> class LCM<unsigned int,int> {public:
    typedef unsigned int type;
};
template <> class LCM<int,unsigned int> {public:
    typedef unsigned int type;
};

template <> class LCM<unsigned char,char> {public:
    typedef unsigned char type;
};
template <> class LCM<char,unsigned char> {public:
    typedef unsigned char type;
};

template <> class LCM<unsigned short,char> {public:
    typedef unsigned short type;
};
template <> class LCM<char,unsigned short> {public:
    typedef unsigned short type;
};

template <> class LCM<unsigned short,short> {public:
    typedef unsigned short type;
};
template <> class LCM<short,unsigned short> {public:
    typedef unsigned short type;
};

template <> class LCM<short,char> {public:
    typedef short type;
};
template <> class LCM<char,short> {public:
    typedef short type;
};

template <> class LCM<unsigned char,short> {public:
    typedef short type;
};
template <> class LCM<short,unsigned char> {public:
    typedef short type;
};

template <> class LCM<unsigned int,char> {public:
    typedef unsigned int type;
};
template <> class LCM<char,unsigned int> {public:
    typedef unsigned int type;
};
template <> class LCM<unsigned int,unsigned char> {public:
    typedef unsigned int type;
};
template <> class LCM<unsigned char,unsigned int> {public:
    typedef unsigned int type;
};
template <> class LCM<unsigned int,unsigned short> {public:
    typedef unsigned int type;
};
template <> class LCM<unsigned short,unsigned int> {public:
    typedef unsigned int type;
};
template <> class LCM<unsigned int,short> {public:
    typedef unsigned int type;
};
template <> class LCM<short,unsigned int> {public:
    typedef unsigned int type;
};

template <> class LCM<vec<float,1>,float> {public:
    typedef float type;
};
template <> class LCM<float,vec<float,1> > {public:
    typedef float type;
};
template <> class LCM<vec<float,1>,double> {public:
    typedef double type;
};
template <> class LCM<float,vec<double,1> > {public:
    typedef double type;
};
template <> class LCM<double,vec<float,1> > {public:
    typedef double type;
};
template <> class LCM<vec<double,1>,float > {public:
    typedef double type;
};
template <> class LCM<vec<double,1>,double> {public:
    typedef double type;
};
template <> class LCM<double,vec<double,1> > {public:
    typedef double type;
};

template <> class LCM<vec<float,2>,float> {public:
    typedef float type;
};
template <> class LCM<float,vec<float,2> > {public:
    typedef float type;
};
template <> class LCM<vec<float,2>,double> {public:
    typedef double type;
};
template <> class LCM<float,vec<double,2> > {public:
    typedef double type;
};
template <> class LCM<double,vec<float,2> > {public:
    typedef double type;
};
template <> class LCM<vec<double,2>,float > {public:
    typedef double type;
};
template <> class LCM<vec<double,2>,double> {public:
    typedef double type;
};
template <> class LCM<double,vec<double,2> > {public:
    typedef double type;
};

template <> class LCM<vec<float,3>,float> {public:
    typedef float type;
};
template <> class LCM<float,vec<float,3> > {public:
    typedef float type;
};
template <> class LCM<vec<float,3>,double> {public:
    typedef double type;
};
template <> class LCM<float,vec<double,3> > {public:
    typedef double type;
};
template <> class LCM<double,vec<float,3> > {public:
    typedef double type;
};
template <> class LCM<vec<double,3>,float > {public:
    typedef double type;
};
template <> class LCM<vec<double,3>,double> {public:
    typedef double type;
};
template <> class LCM<double,vec<double,3> > {public:
    typedef double type;
};

template <> class LCM<char,vec<float,3> > {public:
    typedef float type;
};
template <> class LCM<vec<char,3>,float> {public:
    typedef float type;
};
template <> class LCM<float,vec<char,3> > {public:
    typedef float type;
};
template <> class LCM<vec<float,3>,char> {public:
    typedef float type;
};
template <> class LCM<char,vec<char,3> > {public:
    typedef char type;
};
template <> class LCM<vec<char,3>,char> {public:
    typedef char type;
};
template <> class LCM<double,vec<char,3> > {public:
    typedef double type;
};
template <> class LCM<vec<double,3>,char> {public:
    typedef double type;
};
template <> class LCM<vec<char,3>,double> {public:
    typedef double type;
};
template <> class LCM<char,vec<double,3> > {public:
    typedef double type;
};


template <> class LCM<vec<float,4>,float> {public:
    typedef float type;
};
template <> class LCM<float,vec<float,4> > {public:
    typedef float type;
};
template <> class LCM<vec<float,4>,double> {public:
    typedef double type;
};
template <> class LCM<float,vec<double,4> > {public:
    typedef double type;
};
template <> class LCM<double,vec<float,4> > {public:
    typedef double type;
};
template <> class LCM<vec<double,4>,float > {public:
    typedef double type;
};
template <> class LCM<vec<double,4>,double> {public:
    typedef double type;
};
template <> class LCM<double,vec<double,4> > {public:
    typedef double type;
};

template <class A, class B> class COMMON_CHAR {
public:
   typedef vec<typename COMMON_CHAR<typename A::TYPE ,
                                    typename B::TYPE>::type,
               LUB<A::size,B::size>::size> type;
};
template <> class COMMON_CHAR<float,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<int,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<int,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<int,double> {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<char,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned int,char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<char,unsigned int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<char,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<char,char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<char,double> {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,unsigned char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned char,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned int,unsigned char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned char,unsigned int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned char,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,unsigned char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned char,unsigned char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,unsigned char> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned char,double> {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,unsigned short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned short,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned int,unsigned short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned short,unsigned int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned short,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,unsigned short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned short,unsigned short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,unsigned short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned short,double> {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<short,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned int,short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<short,unsigned int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<short,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<short,short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,short> {public:
    typedef char type;
};
template <> class COMMON_CHAR<short,double> {public:
    typedef char type;
};

template <> class COMMON_CHAR<unsigned int,unsigned int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,unsigned int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned int,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,unsigned int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<unsigned int,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,1>,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<float,1> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,1>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<double,1> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<float,1> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,1>,float > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,1>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<double,1> > {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,vec<int,1> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,1>,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<int,1> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,1>,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<int,1> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,1>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<char,vec<int,1> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,1>,char> {public:
    typedef char type;
};

template <> class COMMON_CHAR<vec<float,2>,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<float,2> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,2>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<double,2> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<float,2> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,2>,float > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,2>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<double,2> > {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,vec<int,2> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,2>,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<int,2> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,2>,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<int,vec<float,2> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,2>,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<int,2> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,2>,int> {public:
    typedef char type;
};

template <> class COMMON_CHAR<vec<float,3>,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<float,3> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,3>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<double,3> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<float,3> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,3>,float > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,3>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<double,3> > {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,vec<int,3> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,3>,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<int,3> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,3>,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<int,vec<float,3> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,3>,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<int,3> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,3>,int> {public:
    typedef char type;
};

template <> class COMMON_CHAR<vec<float,4>,float> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<float,4> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,4>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<double,4> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<float,4> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,4>,float > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,4>,double> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<double,4> > {public:
    typedef char type;
};

template <> class COMMON_CHAR<int,vec<int,4> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<int,4>,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<float,vec<int,4> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<float,4>,int> {public:
    typedef char type;
};
template <> class COMMON_CHAR<double,vec<int,4> > {public:
    typedef char type;
};
template <> class COMMON_CHAR<vec<double,4>,int> {public:
    typedef char type;
};
