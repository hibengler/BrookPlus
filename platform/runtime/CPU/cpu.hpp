/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2008, Advanced Micro Devices, Inc.
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

#ifndef INC_BROOK_CPU_HPP
#define INC_BROOK_CPU_HPP

#include <vector>

#include "Runtime.h"

#include "CPU/brt.hpp"
#include "logger.hpp"

#include "CPU/janitorial.hpp"

#include "CPU/brtarray.hpp"

#ifdef _WIN32
#define THREADRETURNTYPE unsigned long
#else
#define THREADRETURNTYPE void *
#endif

namespace brt
{
    class CPUKernel: public KernelC
    {
          //OpenMP declaration
          int TotalItems() const;
      public:
        CPUKernel(const void *source[]);
        virtual void PushConstant(void* val);
        virtual void PushIter(class Iter * i);
        virtual void PushStream(StreamC* s);
        virtual void PushGatherStream(StreamC* s);
        virtual void PushScatterStream(StreamC* s);
        virtual void PushReduce(void *val, StreamType type);
        virtual void PushReduceStream(StreamC* s, StreamType type);
        virtual void PushOutput(StreamC* s);
        virtual void Map(unsigned int* startExecDomain, unsigned int* domainDimension);
        virtual void Reduce(unsigned int* startExecDomain, unsigned int* domainDimension);
        virtual void SetGroupSize(unsigned int* size, unsigned int dim = 1);

        virtual void PushStreamInterface(StreamInterface * s);
        //Inteface modified for supporting OpenMP
        virtual void* getVectorElement(unsigned int index);
        virtual void * FetchElem(StreamInterface *s, int idx);
        virtual bool Continue();
        virtual bool isValidAddress(int idx);
        virtual void Release();

        // public for indexof
             std::vector < StreamInterface * >input_args;
             std::vector < StreamInterface * >output_args;

      public:

             virtual ~ CPUKernel();

            //Interface modified for supporting OpenMP
                typedef void callable(::brt::CPUKernel *__k,
                                     int __brt_idxstart,
                                     int __brt_idxend,
                                     bool __brt_isreduce);

        callable *func;

                 std::vector < void *>args;
             std::vector < StreamInterface * >freeme;
             std::vector < __BrtArray < unsigned char >*>freeme_array;

        unsigned int dims;
        unsigned int *curpos;
        unsigned int *extents;
        unsigned int *minpos;
        unsigned int *maxpos;
        unsigned int *domainStartPoint;
        unsigned int *domainEndPoint;

        //! \brief group size for compute shader
        unsigned int* groupSize;

        //! \brief group dimension for compute shader
        unsigned int groupDim;
        
        bool is_reduce;
        bool reduce_is_scalar;
        void *reduce_arg;
        void *reduce_value;

        void Cleanup();
    };

    class  kernel
    {
    public:

        kernel(const void* code[]);

        ~kernel();
        operator CPUKernel*() const;
        CPUKernel* operator->() const;

    private:
        kernel( const kernel& ); // no copy constructor
        CPUKernel* _kernel;
    };

    class CPUStream:public StreamC
    {
      public:

        CPUStream(unsigned int inFieldCount,
                  const StreamType * inFieldTypes, unsigned int dims, const unsigned int extents[]);

        // Domain Constructor
            CPUStream(const CPUStream & a, const int *min, const int *max, const int *strMin = NULL, const int *strMax = NULL);

        // Default Constructor
            CPUStream()
        {
            LOG(LOG_FN_ENTRY, "CPUStream::CPUStream()\n");
        }

        /* Stream Operators */
        virtual void Read(const void *inData);
        virtual void Write(void *outData);
        virtual CPUStream *Domain(int min, int max);
        virtual CPUStream *Domain(const int2 & min, const int2 & max);
        virtual CPUStream *Domain(const int3 & min, const int3 & max);
        virtual CPUStream *Domain(const int4 & min, const int4 & max);
        virtual CPUStream *ExecDomain(int num_threads);

        /* Internal runtime functions */
        virtual void *getData(unsigned int flags)
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getData(%08x)\n", flags);
            return data;
        }
        virtual void releaseData(unsigned int flags)
        {
            LOG(LOG_FN_ENTRY, "CPUStream::releaseData(%08x)\n", flags);
        }

#if NEW_SHAPE_STUFF
        virtual StreamShape getLogicalShape(void) const
            {
                return logical_shape;
            }
        virtual StreamShape getPhysicalShape(void) const
            {
                fputs("Debug-mode CPU streams don't have a distinct physical shape. Aborting", stderr);
                exit(-1);
            }
      virtual StreamShape logicalToPhysical(StreamShape logshape, bool force_at = false) const  // force_at is a hack for the CAL runtime
            {
                fputs("Debug-mode CPU streams don't have a distinct physical shape. Aborting", stderr);
                exit(-1);
            }

        virtual const unsigned int *getExtents() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getExtents()\n");
            StreamShape temp = getLogicalShape();
            return temp.getExtents();
        }
        virtual unsigned int getDimension() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getDimension()\n");
                StreamShape temp = getLogicalShape();
                return temp.getRank();
        }
        virtual unsigned int getTotalSize() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getTotalSize()\n");
            StreamShape temp = getLogicalShape();
            return temp.getSize();
        }
#else
        virtual const unsigned int *getExtents() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getExtents()\n");
            return extents;
        }
        virtual unsigned int getDimension() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getDimension()\n");
            return dims;
        }
        virtual unsigned int getTotalSize() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getTotalSize()\n");
            return totalsize;
        }
#endif
        virtual const unsigned int *getDomainMin() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getDomainMin()\n");
            return domain_min;
        }
        virtual const unsigned int *getDomainMax() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getDomainMax()\n");
            return domain_max;
        }
       virtual const unsigned int * getStreamMin() const
       {
           return stream_min;
       }

        virtual const unsigned int * getStreamMax() const
        {
            return stream_max;
        }

        virtual unsigned int getFieldCount() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getFieldCount()\n");
            return (unsigned int) elementType.size();
        }

        virtual StreamType getIndexedFieldType(unsigned int i) const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getIndexedFieldType(%d)\n", i);
            return elementType[i];
        }

        virtual void *fetchElem(const unsigned int pos[], const unsigned int bounds[], unsigned int dim);
        virtual bool isCPU() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::isCPU()\n");
            return true;
        }

        virtual ~ CPUStream();

        // public for indexof
        unsigned int malloced_size;
        unsigned int stride;

        virtual void dump(FILE *f);

      public:

        std::vector < StreamType > elementType;
        void *data;

#if NEW_SHAPE_STUFF
        StreamShape logical_shape;
#else
        unsigned int dims;
        unsigned int totalsize;
        unsigned int *extents;
#endif
        unsigned int *domain_min;
        unsigned int *domain_max;
        unsigned int *stream_min;  //boundary for stream storage
        unsigned int *stream_max;  //different from domain_min/max
                                   //when execDomain is used
        unsigned int *pos;

        bool isDerived;
    };

#if 0
    class CPUIter:public Iter
    {
      protected:
        CPUStream stream;
        virtual ~ CPUIter()
        {
            LOG(LOG_FN_ENTRY, "CPUIter::~CPUIter()\n");
        }
        void allocateStream(unsigned int dims, const unsigned int extents[], const float ranges[]);

      public:
        CPUIter(StreamType type, unsigned int dims,
                const unsigned int extents[], const float ranges[]):Iter(type), stream(1, &type, dims, extents)
        {
            LOG(LOG_FN_ENTRY, "CPUIter::CPUIter(%d, %d, %p, %p)\n", type, dims, extents, ranges);
            allocateStream(dims, extents, ranges);      //now we always have this
        }

        virtual void *getData(unsigned int flags)
        {
            LOG(LOG_FN_ENTRY, "CPUIter::getData(%08x)\n", flags);
            return stream.getData(flags);
        }
        virtual void releaseData(unsigned int flags)
        {
            LOG(LOG_FN_ENTRY, "CPUIter::releaseData(%08x)\n", flags);
            stream.releaseData(flags);
        }
#if NEW_SHAPE_STUFF
        virtual StreamShape getLogicalShape(void) const
            {
                return stream.getLogicalShape();
            }
        virtual StreamShape getPhysicalShape(void) const
            {
                fputs("Debug-mode CPU streams don't have a distinct physical shape. Aborting", stderr);
                exit(-1);
            }
      virtual StreamShape logicalToPhysical(StreamShape logshape, bool force_at = false) const // force_at is a hack for the CAL runtime
            {
                fputs("Debug-mode CPU streams don't have a distinct physical shape. Aborting", stderr);
                exit(-1);
            }

        virtual const unsigned int *getExtents() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getExtents()\n");
            StreamShape temp = getLogicalShape();
            return temp.getExtents();
        }
        virtual unsigned int getDimension() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getDimension()\n");
                StreamShape temp = getLogicalShape();
                return temp.getRank();
        }
        virtual unsigned int getTotalSize() const
        {
            LOG(LOG_FN_ENTRY, "CPUStream::getTotalSize()\n");
            StreamShape temp = getLogicalShape();
            return temp.getSize();
        }
#else
        virtual const unsigned int *getExtents() const
        {
            LOG(LOG_FN_ENTRY, "CPUIter::getExtents()\n");
            return stream.getExtents();
        }
        virtual unsigned int getDimension() const
        {
            LOG(LOG_FN_ENTRY, "CPUIter::getDimension()\n");
            return stream.getDimension();
        }
        virtual unsigned int getTotalSize() const
        {
            LOG(LOG_FN_ENTRY, "CPUIter::getTotalSize()\n");
            return stream.getTotalSize();
        }
#endif
        virtual const unsigned int *getDomainMin() const
        {
            LOG(LOG_FN_ENTRY, "CPUIter::getDomainMin()\n");
            return stream.getDomainMin();
        }
        virtual const unsigned int *getDomainMax() const
        {
            LOG(LOG_FN_ENTRY, "CPUIter::getDomainMax()\n");
            return stream.getDomainMax();
        }

        virtual void *fetchElem(const unsigned int pos[], const unsigned int bounds[], unsigned int dim)
        {
            LOG(LOG_FN_ENTRY, "CPUIter::fetchElem(%p, %p, %d)\n", pos, bounds, dim);
            return stream.fetchElem(pos, bounds, dim);
        }

        // Totally bogus... I know.  But it is to force the
        // iter stream to create a CPUStreamShadow.
        virtual bool isCPU() const
        {
            LOG(LOG_FN_ENTRY, "CPUIter::isCPU()\n");
            return false;
        }
    };
#endif

    class CPURuntime:public ::brook::Runtime
    {
      public:
        static CPURuntime* create();
        unsigned int getDeviceCount() const;
        BaseDevice* getDevice(unsigned int id) const;
        const char* getRuntimeString() const;
#if 0
        virtual Kernel *CreateKernel(const void *[]);
        virtual Iter *CreateIter(StreamType type,
                                 unsigned int dims, const unsigned int extents[], const float ranges[]);
#endif
        
        // new entry points needed by C++ brook API
        void finish()
        {
            LOG(LOG_FN_ENTRY, "CPURuntime::finish()\n");
        }
        void unbind()
        {
            LOG(LOG_FN_ENTRY, "CPURuntime::unbind()\n");
        }
        void bind()
        {
            LOG(LOG_FN_ENTRY, "CPURuntime::bind()\n");
        }

        virtual IWriteQuery *createWriteQuery();
        virtual IWriteMask *createWriteMask(int inY, int inX);
        ~CPURuntime();

    private:
        CPURuntime();
        bool initialize();
    };

    class CPUStreamShadow:public CPUStream
    {
      public:

        CPUStreamShadow(StreamInterface * s, unsigned int flags);
            virtual ~ CPUStreamShadow();

      private:
            StreamInterface * shadow;
        unsigned int flags;
    };

} // namespace brt

#endif
