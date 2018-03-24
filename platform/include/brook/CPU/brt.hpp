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

#ifndef BRT_HPP
#define BRT_HPP

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#include "janitorial.hpp"
#include "brtvector.hpp"
#include "../VectorDataType.h"

#define NEW_SHAPE_STUFF 1
#define NEW_REDUCTION_STUFF 1

// Lift Brook-specific types by default
float getSentinel();

namespace brt
{

    class StreamInterface;
    class StreamC;
    class Iter;
    class Kernel;

    class stream;
    class iter;
    class kernel;

    static const unsigned int MAXPROGLENGTH = 1024*32;
    static const unsigned int MAXSTREsAMDIMS = 8;

    typedef enum __BRTStreamType
    {
        __BRTNONE=-1,
        __BRTSTREAM=0,//stream of stream is illegal. Used in reduce to stream.
        __BRTFLOAT=1,
        __BRTFLOAT2=2,
        __BRTFLOAT3=3,
        __BRTFLOAT4=4,
        __BRTFIXED=5,
        __BRTFIXED2=6,
        __BRTFIXED3=7,
        __BRTFIXED4=8,
        __BRTSHORTFIXED=9,
        __BRTSHORTFIXED2=10,
        __BRTSHORTFIXED3=11,
        __BRTSHORTFIXED4=12,
        __BRTDOUBLE=13,
        __BRTDOUBLE2=14,
        __BRTINT=15,
        __BRTINT2=16,
        __BRTINT3=17,
        __BRTINT4=18,
        __BRTUINT=19,
        __BRTUINT2=20,
        __BRTUINT3=21,
        __BRTUINT4=22,
        __BRTCHAR=23,
        __BRTCHAR2=24,
        __BRTCHAR3=25,
        __BRTCHAR4=26,
        __BRTUCHAR=27,
        __BRTUCHAR2=28,
        __BRTUCHAR3=29,
        __BRTUCHAR4=30,
        __BRTSHORT=31,
        __BRTSHORT2=32,
        __BRTSHORT3=33,
        __BRTSHORT4=34,
        __BRTUSHORT=35,
        __BRTUSHORT2=36,
        __BRTUSHORT3=37,
        __BRTUSHORT4=38,
    } StreamType;

    unsigned int getElementSize(StreamType);
    template<typename T>
    const ::brt::StreamType* getStreamType(T* unused=0);

    template<>
    inline const ::brt::StreamType* getStreamType(float*)
    {
        static const ::brt::StreamType result[] = {__BRTFLOAT, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(float2*)
    {
        static const ::brt::StreamType result[] = {__BRTFLOAT2, __BRTNONE};
        return result;
    }

    template<>
    inline const StreamType* getStreamType(float3*)
    {
        static const ::brt::StreamType result[] = {__BRTFLOAT3, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(float4*)
    {
        static const ::brt::StreamType result[] = {__BRTFLOAT4, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(double*)
    {
        static const ::brt::StreamType result[] = {__BRTDOUBLE,__BRTNONE};
        return result;
    }
    template<>
    inline const ::brt::StreamType* getStreamType(double2*)
    {
        static const ::brt::StreamType result[] = {__BRTDOUBLE2,__BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(int*)
    {
        static const ::brt::StreamType result[] = {__BRTINT, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(int2*)
    {
        static const ::brt::StreamType result[] = {__BRTINT2, __BRTNONE};
        return result;
    }

    template<>
    inline const StreamType* getStreamType(int3*)
    {
        static const ::brt::StreamType result[] = {__BRTINT3, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(int4*)
    {
        static const ::brt::StreamType result[] = {__BRTINT4, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(char*)
    {
        static const ::brt::StreamType result[] = {__BRTCHAR, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(char2*)
    {
        static const ::brt::StreamType result[] = {__BRTCHAR2, __BRTNONE};
        return result;
    }

    template<>
    inline const StreamType* getStreamType(char3*)
    {
        static const ::brt::StreamType result[] = {__BRTCHAR3, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(char4*)
    {
        static const ::brt::StreamType result[] = {__BRTCHAR4, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(uchar*)
    {
        static const ::brt::StreamType result[] = {__BRTUCHAR, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(uchar2*)
    {
        static const ::brt::StreamType result[] = {__BRTUCHAR2, __BRTNONE};
        return result;
    }

    template<>
    inline const StreamType* getStreamType(uchar3*)
    {
        static const ::brt::StreamType result[] = {__BRTUCHAR3, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(uchar4*)
    {
        static const ::brt::StreamType result[] = {__BRTUCHAR4, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(ushort*)
    {
        static const ::brt::StreamType result[] = {__BRTUSHORT, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(ushort2*)
    {
        static const ::brt::StreamType result[] = {__BRTUSHORT2, __BRTNONE};
        return result;
    }

    template<>
    inline const StreamType* getStreamType(ushort3*)
    {
        static const ::brt::StreamType result[] = {__BRTUSHORT3, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(ushort4*)
    {
        static const ::brt::StreamType result[] = {__BRTUSHORT4, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(short*)
    {
        static const ::brt::StreamType result[] = {__BRTSHORT, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(short2*)
    {
        static const ::brt::StreamType result[] = {__BRTSHORT2, __BRTNONE};
        return result;
    }

    template<>
    inline const StreamType* getStreamType(short3*)
    {
        static const ::brt::StreamType result[] = {__BRTSHORT3, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(short4*)
    {
        static const ::brt::StreamType result[] = {__BRTSHORT4, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(uint*)
    {
        static const ::brt::StreamType result[] = {__BRTUINT, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(uint2*)
    {
        static const ::brt::StreamType result[] = {__BRTUINT2, __BRTNONE};
        return result;
    }

    template<>
    inline const StreamType* getStreamType(uint3*)
    {
        static const ::brt::StreamType result[] = {__BRTUINT3, __BRTNONE};
        return result;
    }

    template<>
    inline const ::brt::StreamType* getStreamType(uint4*)
    {
        static const ::brt::StreamType result[] = {__BRTUINT4, __BRTNONE};
        return result;
    }
    /****************************************************/
    /*****           Runtime routines            ********/

    class Runtime;

    /* start up the brook runtime with a given implemention */
    EXPORT void initialize( const char* inRuntimeName,
                            void* inContextValue = 0 );

    /* shut down the brook runtime, (shouldn't often be needed) */
    EXPORT void finalize();

    /* tell brook to complete any queued operations on the gpu */
    EXPORT void finish();

    /* inform brook that the client intends to use the gpu (e.g. to render) */
    EXPORT void unbind();

    /* inform brook that the client is done with the gpu, and wants to use brook again */
    EXPORT void bind();

    EXPORT Runtime* createRuntime( bool useAddressTranslation );

#if NEW_SHAPE_STUFF

    //! The "shape" of a stream. Currently, all streams have Cartesian
    //! (array-like) shapes, but this may change in the future when we move to
    //! NDP. All streams now have a "logical" and a "physical" shape, so it
    //! makes sense to factor out some of this stuff and make the distinction
    //! between the two more obvious.

    //! We use a constant max # of extents rather than dynamically allocating
    //! them to reduce allocation/deallocation overhead. If we choose to allow
    //! more extents in the future, we can revisit this decision.

    //! \todo We should also use this mechanism for domains as well.

    class StreamShape
    {
    public:
        StreamShape(unsigned int rank = 0,      //< The rank (dimensionality) of the stream
                    unsigned int e0 = 0,        //< Extent 0 (x for 1d, y for 2d, z for 3d, w for 4d)
                    unsigned int e1 = 0,        //< Extent 1 (x for 2d, y for 3d, z for 4d)
                    unsigned int e2 = 0,        //< Extent 2 (x for 3d, y for 4d)
                    unsigned int e3 = 0)        //< Extent 3 (x for 4d)
            {
                this->rank = rank;
                extents[0] = e0;
                extents[1] = 1;
                extents[2] = 1;
                extents[3] = 1;
                if(rank > 1)
                    extents[1] = e1;
                if(rank > 2)
                    extents[2] = e2;
                if(rank > 3)
                    extents[3] = e3;
                reversed_extents = NULL;
            }
        StreamShape(const StreamShape& s)
            {
                rank = s.rank;
                extents[0] = s.extents[0];
                extents[1] = s.extents[1];
                extents[2] = s.extents[2];
                extents[3] = s.extents[3];
                reversed_extents = NULL;
            }
        StreamShape& operator =(const StreamShape& s)
            {
                free(reversed_extents);
                reversed_extents = NULL;
                rank = s.rank;
                extents[0] = s.extents[0];
                extents[1] = s.extents[1];
                extents[2] = s.extents[2];
                extents[3] = s.extents[3];
                return *this;
            }
        bool operator ==(const StreamShape &s)
            {
                return (rank == s.rank
                        && extents[0] == s.extents[0]
                        && extents[1] == s.extents[1]
                        && extents[2] == s.extents[2]
                        && extents[3] == s.extents[3]);
            }
        bool operator !=(const StreamShape &s)
            {
                return !(rank == s.rank
                         && extents[0] == s.extents[0]
                         && extents[1] == s.extents[1]
                         && extents[2] == s.extents[2]
                         && extents[3] == s.extents[3]);
            }
        ~StreamShape()
            {
                free(reversed_extents);
            }
        unsigned int getRank(void)
            {
                return rank;
            }
        unsigned int *getExtents(void)
            {
                assert(rank > 0); //"StreamShape not initialised",
                return extents;
            }
        unsigned int *getReversedExtents(void)
            {
                assert(rank > 0); //"StreamShape not initialised"
                if (!reversed_extents)
                {
                    reversed_extents = (unsigned int *)malloc(rank * sizeof(unsigned int));
                    if (reversed_extents)
                    {
                        for (unsigned int i = 0; i < rank; i++)
                        {
                            reversed_extents[i] = extents[rank-1-i];
                        }
                    }
                }
                return reversed_extents;
            }
        unsigned int getSize(void)      //< \todo Should we cache this as well?
            {
                assert(rank > 0); //"StreamShape not initialised"
                unsigned int r = 1;
                for (unsigned int i = 0; i < rank; i++)
                {
                    r *= extents[i];
                }
                return r;
            }
        enum {MaxExtents = 4};
    private:
        unsigned int rank;
        unsigned int extents[MaxExtents];
        unsigned int *reversed_extents;         //< The extents, but in
        //reversed order (some code
        //needs this). We do this
        //on-demand and cache to
        //avoid unnecessary work.
    };

#endif


    /************************************************/
    /**********       Stream classes      ***********/

    class EXPORT StreamInterface
    {
    private:
        size_t referenceCount;

    protected:
        StreamInterface()
            : referenceCount(1) {}
        virtual ~StreamInterface(){}

    public:
        void acquireReference() { referenceCount++; }
        void releaseReference()
        {
            if( --referenceCount == 0 ) delete this;
        }

        enum USAGEFLAGS {NONE=0x0,READ=0x1,WRITE=0x2,READWRITE=0x3};

        virtual void *getData (unsigned int flags)=0;
        virtual void releaseData(unsigned int flags)=0;
        virtual void readItem(void * p,unsigned int * index);
        virtual void *fetchItemPtr (void *data, unsigned int * index);
        
#if NEW_SHAPE_STUFF
        void *getPhysicalData(void);
#endif

#if NEW_SHAPE_STUFF
        virtual StreamShape getLogicalShape(void) const=0;
        virtual StreamShape getPhysicalShape(void) const=0;
        virtual StreamShape logicalToPhysical(StreamShape logshape, bool force_at = false) const=0;             // FIXME force_at is a hack for the CAL runtime

        virtual const unsigned int *getExtents() const=0;
        virtual unsigned int getDimension() const {return 0;}
        virtual unsigned int getTotalSize() const
        {
            unsigned int ret=1;
            unsigned int dim=getDimension();
            const unsigned int * extents = getExtents();
            for (unsigned int i=0;i<dim;++i)
            {
                ret*=extents[i];
            }
            return ret;
        }
#else
        virtual const unsigned int *getExtents() const=0;
        virtual unsigned int getDimension() const {return 0;}
        virtual unsigned int getTotalSize() const
        {
            unsigned int ret=1;
            unsigned int dim=getDimension();
            const unsigned int * extents = getExtents();
            for (unsigned int i=0;i<dim;++i)
            {
                ret*=extents[i];
            }
            return ret;
        }
#endif
        virtual const unsigned int * getDomainMin() const {assert(0); return 0;}
        virtual const unsigned int * getDomainMax() const {assert(0); return 0;}

        virtual const unsigned int * getStreamMin() const {assert(0); return 0;}
        virtual const unsigned int * getStreamMax() const {assert(0); return 0;}

        unsigned int getElementSize() const;
        virtual unsigned int getFieldCount() const = 0;
        virtual StreamType getIndexedFieldType(unsigned int i) const=0;


        // functions for getting at low-level representation,
        // so that an application can render and simulate
        virtual void * getIndexedFieldRenderData(unsigned int i);
        virtual void   synchronizeRenderData();

        /* Used CPU Runtime Only */
        virtual void * fetchElem(const unsigned int pos[],
                                 const unsigned int bounds[],
                                 unsigned int UNUSED(dim))
        {
            unsigned int UNUSED(a) = pos[0]; // dummy to avoid unused param warning - macro doesn't work for arrays
            unsigned int UNUSED(b) = bounds[0];
            assert (0); return (void *) 0;
        }

        __BrtFloat4 computeIndexOf(unsigned int linear_index);

        virtual bool isCPU() const  { return false; }

        // debug

        virtual void dump(FILE *f) = 0;

    };

    class StreamC : public StreamInterface
    {
    public:
        StreamC () {}
        
        //virtual unsigned int getStride() const {return sizeof(float)*getStreamType();}
        //virtual __BRTStreamType getStreamType ()const{return type;}

    protected:
        virtual ~StreamC() {}
    };

    class Iter : public StreamInterface
    {
    public:
        Iter (::brt::StreamType type) {this->type=type;}
        virtual unsigned int getFieldCount() const { return 1; }
        virtual ::brt::StreamType getIndexedFieldType(unsigned int i) const
            {
                assert(i == 0);
                return type;
            }
        virtual void dump(FILE *f)
            {
                // no-op - dump not valid for iterators
            }

    protected:
        ::brt::StreamType type;
        virtual ~Iter() {}
    };

    /*** Main Kernel Class ***/

    class KernelC
    {
    public:
        KernelC() {}
        virtual void Release() {delete this;}

        // CPU version only
        virtual bool isValidAddress(int idx) = 0;
        virtual bool   Continue() {assert (0); return false;}
        // Interface changed for supporting OpenMP
        virtual void * FetchElem(StreamInterface *s, int idx = 0)
            {
                assert (0); return (void *) 0;
            }
        
        virtual void* getVectorElement(unsigned int index)
        {
            assert(0);
            
            return (void*)0;
        }
    protected:
        virtual ~KernelC() {}
    };

    /*************************************************************/
    /****     Stub classes for initialization               ******/

    class EXPORT stream
    {
    public:
        stream();
        stream( const ::brt::stream& );
        stream& operator=( const ::brt::stream& );

        // easy-to-use constructors for C++ interface
        template<typename T>
            static ::brt::stream create( int inExtent0 )
        {
            return stream( getStreamType((T*)0), inExtent0, -1 );
        }

        template<typename T>
            static ::brt::stream create( int y, int x )
        {
            return stream( ::brt::getStreamType((T*)0), y, x, -1 );
        }

        // for domain
        stream(::brt::StreamC *s) {_stream = s;}

        // standard constructors for BRCC-generated code
        stream(const ::brt::StreamType*,...);
        stream(const unsigned int extents[],
               unsigned int dims,
               const ::brt::StreamType *type,
               bool read_only=false);
        stream( const ::brt::iter& );

        ~stream();

        operator ::brt::StreamC*() const
        {
            return _stream;
        }

        operator ::brt::StreamInterface*() const
        {
            return _stream;
        }

        ::brt::StreamC* operator->() const
        {
            return _stream;
        }

    private:

        ::brt::StreamC* _stream;
    };

    class EXPORT iter
    {
    public:
        iter(::brt::StreamType, ...);
        ~iter()
        {
            if(_iter) _iter->releaseReference();
        }

        operator ::brt::Iter*() const
        {
            return _iter;
        }

        operator ::brt::StreamInterface*() const
        {
            return _iter;
        }

        ::brt::Iter* operator->() const
        {
            return _iter;
        }

    private:
        iter( const ::brt::iter& ); // no copy constructor
        ::brt::Iter* _iter;
    };

    /* wrapper class for write masking with z buffer */
    class  EXPORT IWriteMask;
    class  EXPORT write_mask
    {
    public:
        static write_mask create( int inY, int inX );

        write_mask();
        write_mask( const write_mask& inMask );

        write_mask& operator=( const write_mask& inMask );

        // bind/unbind this mask as the current mask of the context
        void bind();
        void unbind();

        // enable/disable use of this mask to limit computation
        void enableTest();
        void disableTest();

        // enable/disable writing to this mask
        void enableSet();
        void disableSet();

        // clear the mask to enable all writes
        void clear();

    private:
        explicit write_mask( IWriteMask* inMask );

        IWriteMask* _mask;
    };

    /* wrapper class for occlusion queries */

    class  EXPORT IWriteQuery;
    class  EXPORT write_query
    {
    public:
        static write_query create();

        write_query();
        write_query( const write_query& inQuery );

        write_query& operator=( const write_query& inQuery );

        void begin();
        void end();

        bool poll();
        void wait();

        size_t count();

        operator IWriteQuery*();

    private:
        explicit write_query( IWriteQuery* inQuery );

        IWriteQuery* _query;
    };


    /* For vout */
    inline static void maxDimension(unsigned int * out,
                                    const unsigned int * in,
                                    int dims)
    {
        for (int i=0;i<dims;++i)
        {
            if (in[i]>(unsigned int)out[i])out[i]=in[i];
        }
    }
    float getSentinel();
    ::brt::stream* sentinelStream(int dim);


    void readItem(::brt::StreamInterface *s, void * p, ... );
}

/***********************************************************/
/*******      S T R E A M      O P E R A T O R S       *****/

EXPORT void streamPrint(::brt::StreamInterface*s, bool flatten=false);

inline static float4 streamSize(::brt::stream &s)
{
    unsigned int i;
#if NEW_SHAPE_STUFF
    ::brt::StreamShape log_shape = s->getLogicalShape();
    const unsigned int *extents = log_shape.getExtents();
    unsigned int dim = log_shape.getRank();
#else
    const unsigned int * extents = s->getExtents();
    unsigned int dim             = s->getDimension();
#endif

    float4 ret(0.0f,0.0f,0.0f,0.0f);

    switch (dim)
    {
    case 3:
        ret.z=(float)extents[dim-3];
    case 2:
        ret.y=(float)extents[dim-2];
    case 1:
        ret.x=(float)extents[dim-1];
        break;
    case 4:
    default:
        for (i=0;i<dim-3;++i) ret.w+=(float)extents[i];
        ret.z=(float)extents[dim-3];
        ret.y=(float)extents[dim-2];
        ret.x=(float)extents[dim-1];
        break;
    }
    return ret;
}

typedef ::brt::iter __BRTIter;

#define streamGatherOp(a,b,c,d)                                         \
    __streamGatherOrScatterOp(a,b,c,                                    \
                              (void (*)(void *, void *))__##d##_cpu_inner,true)

#define streamScatterOp(a,b,c,d)                                        \
    __streamGatherOrScatterOp(a,b,c,                                    \
                              (void (*)(void *, void *))__##d##_cpu_inner,false)

EXPORT void __streamGatherOrScatterOp (::brt::StreamInterface *dst,
                                       ::brt::StreamInterface *index,
                                       ::brt::StreamInterface *src,
                                       void (*func) (void *, void *),
                                       bool GatherNotScatter);
template <class T> class Addressable: public T
{
public:
    mutable void *address;
    template <typename U>Addressable(U* Address)
        : T(Address->castToArg(T()))
        {
            this->address=Address;
        }
    //  Addressable(const T &t, void * Address):T(t){this->address=Address;}
    Addressable(const T&t):T(t){address=NULL;}
    Addressable(const Addressable<T>& b):T(b){
        this->address=b.address;
    }
    Addressable<T>&operator = (const T&b)
        {
            *static_cast<T*>(this)=static_cast<const T&>(b);
            return *this;
        }
    Addressable<T>& operator = (const Addressable<T>& b)
        {
            *static_cast<T*>(this)=static_cast<const T&>(b);
            if (address==NULL) this->address=b.address;
            return *this;
        }
};
template <class T> const void * getStreamAddress(const T* a)
{
    return static_cast<const Addressable<T>* > (a)->address;
}
#if 0
#define indexof(a) __indexof(a.address)
#else
#define indexof(a) __indexof(getStreamAddress(&a))
#endif
EXPORT  __BrtFloat4 __indexof (const void *);
EXPORT  __BrtInt4 __get_scatter__address (__BrtInt1 threadId);
EXPORT  __BrtInt1 __get_GroupSize();
EXPORT  __BrtInt4 __get_threadId__InGroup(__BrtInt1 threadId);
// TIM: adding conditional magick for raytracer
void streamEnableWriteMask();
void streamDisableWriteMask();
void streamSetWriteMask( ::brt::stream& );
void streamBeginWriteQuery();
int streamEndWriteQuery();

#endif

