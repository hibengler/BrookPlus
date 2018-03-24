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

// brt.cpp

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <float.h>

#ifdef BUILD_DX9
#include "gpu/dx9/dx9runtime.hpp"
#endif

#ifdef BUILD_CAL
#include "cal/calruntime.hpp"
#endif

#ifdef BUILD_OGL
#include "gpu/ogl/oglruntime.hpp"
#endif

#include "cpu.hpp"
#include "Kernel.h"
#include "CPUKernelImpl.h"
#include "Runtime.h"
#include "Formats.h"

#include "CPU/brtscatterintrinsic.hpp"
__StreamScatterAssign STREAM_SCATTER_ASSIGN;
__StreamScatterAdd STREAM_SCATTER_ADD;
__StreamScatterMul STREAM_SCATTER_MUL;
__StreamGatherInc STREAM_GATHER_INC;
__StreamGatherFetch STREAM_GATHER_FETCH;

inline int finite_flt(float x)
{
#ifdef _WIN32
    return _finite(x);
#else
    return finite(x);
#endif
}

inline int isnan_flt(float x)
{
#ifdef _WIN32
    return _isnan(x);
#else
    return isnan(x);
#endif
}

#include "logger.hpp"

static int convertGatherIndexToInt(float inIndex)
{
    LOG(LOG_FN_ENTRY, "convertGatherIndexToInt(%f)\n", inIndex);
    return (int) (inIndex + 0.5f);
}

extern "C" void brookRuntimeCleanupCallback()
{
    LOG(LOG_FN_ENTRY, "brookRuntimeCleanupCallback()\n");
}

namespace brt
{

    void *StreamInterface::getIndexedFieldRenderData(unsigned int i)
    {
        LOG(LOG_FN_ENTRY, "StreamInterface::getIndexedFieldRenderData(%d)\n", i);
        return NULL;
    }
    
    void StreamInterface::synchronizeRenderData()
    {
        LOG(LOG_FN_ENTRY, "StreamInterface::synchronizeRenderData()\n");
    }

    unsigned int getElementSize(StreamType fieldType)
    {
        LOG(LOG_FN_ENTRY, "getElementSize(%d)\n", fieldType);
        switch (fieldType)
        {
        case __BRTDOUBLE:
            return sizeof(double);
        case __BRTDOUBLE2:
            return 2 * sizeof(double);
        case __BRTFLOAT:
            return sizeof(float);
        case __BRTFLOAT2:
            return sizeof(float2);
        case __BRTFLOAT3:
            return sizeof(float3);
        case __BRTFLOAT4:
            return sizeof(float4);
        case __BRTCHAR:
            return sizeof(char);
        case __BRTCHAR2:
            return sizeof(char2);
        case __BRTCHAR3:
            return sizeof(char3);
        case __BRTCHAR4:
            return sizeof(char4);
        case __BRTUCHAR:
            return sizeof(uchar);
        case __BRTUCHAR2:
            return sizeof(uchar2);
        case __BRTUCHAR3:
            return sizeof(uchar3);
        case __BRTUCHAR4:
            return sizeof(uchar4);
        case __BRTSHORT:
            return sizeof(short);
        case __BRTSHORT2:
            return sizeof(short2);
        case __BRTSHORT3:
            return sizeof(short3);
        case __BRTSHORT4:
            return sizeof(short4);
        case __BRTUSHORT:
            return sizeof(ushort);
        case __BRTUSHORT2:
            return sizeof(ushort2);
        case __BRTUSHORT3:
            return sizeof(ushort3);
        case __BRTUSHORT4:
            return sizeof(ushort4);
        case __BRTINT:
            return sizeof(int);
        case __BRTINT2:
            return sizeof(int2);
        case __BRTINT3:
            return sizeof(int3);
        case __BRTINT4:
            return sizeof(int4);
        case __BRTUINT:
            return sizeof(uint);
        case __BRTUINT2:
            return sizeof(uint2);
        case __BRTUINT3:
            return sizeof(uint3);
        case __BRTUINT4:
            return sizeof(uint4);
        default:
            fprintf(stderr, "Internal error: invalid stream element type %d\n", fieldType);
            exit(-1);
            return 0;
        };
    }
    unsigned int StreamInterface::getElementSize() const
    {
        LOG(LOG_FN_ENTRY, "StreamInterface::getElementSize()\n");
        unsigned int result = 0;
        int fieldCount = getFieldCount();
        for (int i = 0; i < fieldCount; i++)
        {
            result +=::brt::getElementSize(getIndexedFieldType(i));
        }
        return result;
    }

    void StreamInterface::readItem(void *output, unsigned int *index)
    {
        LOG(LOG_FN_ENTRY, "StreamInterface::readItem(%p, %p)\n");
        void *data = (char *) getData(READ);
        void *ptr = fetchItemPtr(data, index);
        unsigned int size = getElementSize();

        memcpy(output, ptr, size);
        releaseData(READ);
    }

    __BrtFloat4 StreamInterface::computeIndexOf(unsigned int linear_index)
    {
        LOG(LOG_FN_ENTRY, "StreamInterface::computeIndexOf(%d)\n", linear_index);
        const unsigned int *domain_min = getDomainMin();
#if NEW_SHAPE_STUFF
        StreamShape log_shape = getLogicalShape();
        const unsigned int *extents = log_shape.getExtents();
        unsigned int dim = log_shape.getRank();
#else
        const unsigned int *extents = getExtents();
        unsigned int dim = getDimension();
#endif
        int i;

        assert(dim > 0 && dim <= 4);

        unsigned int index[4] = { 0, 0, 0, 0 };
        unsigned int largestaddr = 1;

        for (i = ((int) dim) - 1; i >= 0; --i)
        {
            largestaddr *= extents[i];
        }

        for (i = ((int) dim) - 1; i >= 0; --i)
        {
            largestaddr /= extents[dim - i - 1];
            index[i] = linear_index / largestaddr;
            linear_index -= largestaddr * index[i];
            index[i] -= domain_min[dim - i - 1];
        }

        return __BrtFloat4((float) index[0], (float) index[1], (float) index[2], (float) index[3]);
    }

    void *StreamInterface::fetchItemPtr(void *data, unsigned int *index)
    {
        LOG(LOG_FN_ENTRY, "StreamInterface::fetchItemPtr(%p, %p)\n", data, index);
        const unsigned int *domain_min = getDomainMin();
#if NEW_SHAPE_STUFF
        StreamShape log_shape = getLogicalShape();
        const unsigned int *extents = log_shape.getExtents();
        unsigned int dim = log_shape.getRank();
#else
        const unsigned int *extents = getExtents();
        unsigned int dim = getDimension();
#endif
        unsigned int size = getElementSize();

        assert(dim > 0);

        unsigned int linearindex = 0;
        unsigned int offset = 1;

        for (int i = ((int)dim) - 1; i >= 0; --i)
        {
            linearindex += (index[i] + domain_min[i]) * offset;
            offset *= extents[i];
        }

        return ((unsigned char *) data) + linearindex * size;
    }

    stream::stream()            // FIXME - default param instead?
        :     _stream(0)
    {
        LOG(LOG_FN_ENTRY, "stream::stream()\n");
    }

    stream::stream(const stream & inStream):_stream(inStream._stream)
    {
        LOG(LOG_FN_ENTRY, "stream::stream(%p)\n", &inStream);
        if (_stream)
            _stream->acquireReference();
    }

    stream & stream::operator=(const stream & inStream)
    {
        LOG(LOG_FN_ENTRY, "stream::operator=(%p)\n", &inStream);
        StreamC *s = inStream._stream;

        if (s)
            s->acquireReference();
        if (_stream)
            _stream->releaseReference();
        _stream = s;
        return *this;
    }

    stream::~stream()
    {
        LOG(LOG_FN_ENTRY, "stream::~stream()\n");
        if (_stream)
            _stream->releaseReference();
    }


    stream::stream(const StreamType * inElementTypes, ...):_stream(0)
    {
        LOG(LOG_FN_ENTRY, "stream::stream(%p, ...)\n", inElementTypes);
        std::vector < StreamType > elementTypes;
        std::vector < unsigned int >extents;

        const StreamType *e = inElementTypes;

        while (*e != __BRTNONE)
        {
            elementTypes.push_back(*e);
            e++;
        }

        va_list args;

        va_start(args, inElementTypes);
        for (;;)
        {
            int extent = va_arg(args, int);

            if (extent == -1)
                break;
            extents.push_back(extent);
        }
        va_end(args);

        _stream = NULL;
    }


    stream::stream(const unsigned int *extents, unsigned int dims, const StreamType * type, bool read_only):_stream(0)
    {
        LOG(LOG_FN_ENTRY, "stream::stream(%p, %d, %p, %d)\n", extents, dims, type, read_only);
        std::vector < StreamType > elementTypes;
        const StreamType *e = type;

        while (*e != __BRTNONE)
        {
            elementTypes.push_back(*e);
            e++;
        }
        _stream = NULL;
    }


    stream::stream(const::brt::iter & i):_stream(0)
    {
        LOG(LOG_FN_ENTRY, "stream::stream(%p)\n", &i);
        ::brt::Iter * iterator = i;

        StreamType elementType = iterator->getIndexedFieldType(0);
#if NEW_SHAPE_STUFF
        StreamShape iter_log_shape = iterator->getLogicalShape();
        unsigned int dimensionCount = iter_log_shape.getRank();
        const unsigned int *extents = iter_log_shape.getExtents();
#else
        unsigned int dimensionCount = iterator->getDimension();
        const unsigned int *extents = (unsigned int *) (iterator->getExtents());
#endif

        _stream = NULL;
        //_stream->Read(iterator->getData(::brt::Stream::READ));
        iterator->releaseData(::brt::StreamC::READ);
    }


    iter::iter(StreamType type,...):_iter(0)
    {
        LOG(LOG_FN_ENTRY, "iter::iter(%d, ...)\n", type);
        std::vector < unsigned int >extents;
        std::vector < float >ranges;
        va_list args;

        va_start(args, type);
        for (;;)
        {
            int extent = va_arg(args, int);

            if (extent == -1)
                break;
            extents.push_back(extent);
        }
        for (int i = 0; i < type; ++i)
        {
            float f = (float) va_arg(args, double);

            //     fprintf(stderr, "float %f\n",f);
            ranges.push_back(f);
            f = (float) va_arg(args, double);

            //     fprintf(stderr, "float %f\n",f);
            ranges.push_back(f);
        }
        va_end(args);

        _iter = NULL;
    }


    float getSentinel()
    {
        LOG(LOG_FN_ENTRY, "getSentinel()\n");
        return (float) -1844632.18612444856320;
    }

    class StreamSentinels
    {
    public:
        std::vector < stream * >sentinels;
        ~StreamSentinels()
            {
                LOG(LOG_FN_ENTRY, "~StreamSentinels()\n");
                while (!sentinels.empty())
                {
                    if (sentinels.back())
                        delete sentinels.back();
                    sentinels.pop_back();
                }
            }
    };

    stream *sentinelStream(int dim)
    {
        LOG(LOG_FN_ENTRY, "sentinelStream(%d)\n", dim);
        static StreamSentinels s;
        float onehalf = 0.5f;
        float inf = 1.0f / (float) floor(onehalf);

        if (dim < (int) s.sentinels.size())
            if (s.sentinels[dim] != 0)
                return s.sentinels[dim];

        while ((int) s.sentinels.size() <= dim)
            s.sentinels.push_back(0);

        std::vector < unsigned int >extents;

        for (int i = 0; i < dim; ++i)
        {
            extents.push_back(1);
        }

        s.sentinels[dim] = new ::brt::stream(&extents[0], dim, ::brt::getStreamType((float *) 0));
        //streamRead(*s.sentinels[dim], &inf);

        return s.sentinels[dim];
    }

}

void streamPrint(::brt::StreamInterface * s, bool flatten)
{
    LOG(LOG_FN_ENTRY, "streamPrint(%p, %d)\n", s, flatten);
    flatten = false;
#if NEW_SHAPE_STUFF
    ::brt::StreamShape s_log_shape = s->getLogicalShape();
    unsigned int dims = s_log_shape.getRank();
    const unsigned int *extent = s_log_shape.getExtents();
    unsigned int tot = s_log_shape.getSize();
#else
    unsigned int dims = s->getDimension();
    const unsigned int *extent = s->getExtents();
    unsigned int tot = s->getTotalSize();
#endif
    unsigned int numfloats = 0;
    unsigned int numfields = s->getFieldCount();

    for (unsigned int fields = 0; fields < numfields; ++fields)
    {
        int count;
        switch (s->getIndexedFieldType(fields))
        {
        case __BRTDOUBLE:
            count = 1;
            break;
        case __BRTDOUBLE2:
            count = 2;
            break;
        case __BRTFIXED:
            count = 1;
            break;
        case __BRTFIXED2:
            count = 2;
            break;
        case __BRTFIXED3:
            count = 3;
            break;
        case __BRTFIXED4:
            count = 4;
            break;
        case __BRTSHORTFIXED:
            count = 1;
            break;
        case __BRTSHORTFIXED2:
            count = 2;
            break;
        case __BRTSHORTFIXED3:
            count = 3;
            break;
        case __BRTSHORTFIXED4:
            count = 4;
            break;
        case __BRTFLOAT:
            count = 1;
            break;
        case __BRTFLOAT2:
            count = 2;
            break;
        case __BRTFLOAT3:
            count = 3;
            break;
        case __BRTFLOAT4:
            count = 4;
            break;
        case __BRTINT:
            count = 1;
            break;
        case __BRTINT2:
            count = 2;
            break;
        case __BRTINT3:
            count = 3;
            break;
        case __BRTINT4:
            count = 4;
            break;
        case __BRTUINT:
            count = 1;
            break;
        case __BRTUINT2:
            count = 2;
            break;
        case __BRTUINT3:
            count = 3;
            break;
        case __BRTUINT4:
            count = 4;
            break;
        default:
            count = 0;
            break;
        };
        numfloats += count;
    }
    float *data = (float *) s->getData(::brt::StreamInterface::READ);

    for (unsigned int i = 0; i < tot; ++i)
    {
        if (numfloats != 1)
            printf("{");
        for (unsigned int j = 0; j < numfloats; ++j)
        {
            float x = data[i * numfloats + j];

            if (j != 0)
            {
                printf(",");
                printf(" ");
            }
            if (finite_flt(x))
            {

                if (FPEQ(x, 36893206672442393000.00f))
                    printf("inf");
                else if (FPEQ(x, -18446321861244485632.0f))
                {
                    printf("inf");
                }
                else if (FPZ(x))
                    printf("0.00");
                else
                    printf("%3.2f", x);
            }
            else if (isnan_flt(x))
                printf("NaN");
            else
                printf("inf");
        }

        if (numfloats != 1)
            printf("}");
        else
            printf(" ");
        if (!flatten)
            if ((i + 1) % extent[dims - 1] == 0)
                printf("\n");
    }
    s->releaseData(::brt::StreamInterface::READ);
}

void readItem(::brt::StreamInterface * s, void *p, ...)
{
    LOG(LOG_FN_ENTRY, "readItem(%p, %p, ...)\n", s, p);
#if NEW_SHAPE_STUFF
    unsigned int dims = s->getLogicalShape().getRank();
#else
    unsigned int dims = s->getDimension();
#endif
    std::vector < unsigned int >index;
    va_list args;

    va_start(args, p);
    for (unsigned int i = 0; i < dims; ++i)
    {
        index.push_back(va_arg(args, int));
    }
    va_end(args);
    s->readItem(p, &index[0]);
}

void __streamGatherOrScatterOp(::brt::StreamInterface * dst,  // FIXME - double-underscore prefix reserved
                               ::brt::StreamInterface * index,::brt::StreamInterface * src,
                               void (*func) (void *, void *), bool GatherNotScatter)
{
    LOG(LOG_FN_ENTRY, " __streamGatherOrScatterOp(%p, %p, %p, %p)\n", dst, index, src, func);
    unsigned i;
    unsigned int *curpos;

    ::brt::StreamType index_type;

    // Fetch the relevent stream properties
    // Get the Dimensions
#if NEW_SHAPE_STUFF
    ::brt::StreamShape index_log_shape = index->getLogicalShape();
    ::brt::StreamShape src_log_shape = src->getLogicalShape();
    ::brt::StreamShape dst_log_shape = dst->getLogicalShape();
    unsigned int index_dims = index_log_shape.getRank();
    unsigned int src_dims = src_log_shape.getRank();
    unsigned int dst_dims = dst_log_shape.getRank();
#else
    unsigned int index_dims = index->getDimension();
    unsigned int src_dims = src->getDimension();
    unsigned int dst_dims = dst->getDimension();
#endif

    // Get Domain properties.
    const unsigned int *index_dmin = index->getDomainMin();
    const unsigned int *index_dmax = index->getDomainMax();
    const unsigned int *dst_dmin = dst->getDomainMin();
    const unsigned int *dst_dmax = dst->getDomainMax();

    // Get element sizes
    unsigned int src_elemsize = src->getElementSize();
    unsigned int dst_elemsize = dst->getElementSize();

    // Get actual data
    float *index_data = (float *) index->getData(::brt::StreamC::READ);
    unsigned char *src_data;
    unsigned char *dst_data;

    if (GatherNotScatter)
    {
        src_data = (unsigned char *) src->getData(::brt::StreamC::READWRITE);
        dst_data = (unsigned char *) dst->getData(::brt::StreamC::WRITE);
    }
    else
    {
        src_data = (unsigned char *) src->getData(::brt::StreamC::READ);
        dst_data = (unsigned char *) dst->getData(::brt::StreamC::READWRITE);
    }

    // Where we'll do the op
    void *buf1;
    void *buf2;

    // Do some assertion tests here to make sure
    // that the gatherop is legal

    // Really, we should check to make sure that the
    // dst and src streams should be the same time...
    // But for now, we are simply going to make sure
    // that the stream elements are of the same size.
    assert(src_elemsize == dst_elemsize);

    // The index type must be a float1-4 stream
    assert(index->getFieldCount() == 1);
    index_type = index->getIndexedFieldType(0);

    // The index type must match the number of dimensions
    // in the source, i.e. float4 index for a 4D stream
    assert(index_type == (int) src_dims);

    // The index dimensionality must match the
    // output dimensionality.
    assert(index_dims == dst_dims);

    // The number of elements of the index must match the
    // the number of elements of the destination
    for (i = 0; i < index_dims; i++)
        assert(index_dmax[i] - index_dmin[i] == dst_dmax[i] - dst_dmin[i]);

    // Create the counter
    curpos = (unsigned int *) malloc(index_dims * sizeof(unsigned int));

    // Create the buffer for gather op
    buf1 = malloc(src_elemsize);
    buf2 = malloc(src_elemsize);

    // Initialize it to zero
    for (i = 0; i < index_dims; i++)
        curpos[i] = 0;

    // Perform the gatherop
    bool finished = false;

    do
    {
        unsigned int index_intptr[4];

        // get the index value
        float *index_ptr = (float *) index->fetchItemPtr(index_data,
                                                         curpos);

        // create an int vector from the float values
        // Note that we have to flip the vector since I
        // do everything as z,y,x and the user specifies as
        // x,y,z
        for (i = 0; i < (unsigned int) index_type; i++)
            index_intptr[i] = convertGatherIndexToInt(index_ptr[index_type - 1 - i]);

        // get the src value
        void *src_ptr = src->fetchItemPtr(src_data,
                                          index_intptr);

        // get the dst value
        void *dst_ptr = dst->fetchItemPtr(dst_data,
                                          curpos);

        if (GatherNotScatter)
        {
            // Gather Path
            // copy the data over
            memcpy(dst_ptr, src_ptr, src_elemsize);

            // perform the op part of the gather op on src
            // XXX: Bug we need to know what which is the output
            // and which is the input.
            // For now we will just take whichever output changes

            memcpy(buf1, src_ptr, src_elemsize);
            memcpy(buf2, src_ptr, src_elemsize);
            func(buf1, buf2);

            if (memcmp(buf1, src_ptr, src_elemsize))
            {
                assert(memcmp(buf2, src_ptr, src_elemsize) == 0);
                memcpy(src_ptr, buf1, src_elemsize);
            }
            else
                memcpy(src_ptr, buf2, src_elemsize);

        }
        else
        {
            // Scatter Path
            // XXX: Bug we need to know what which is the output
            // and which is the input.
            // For now we assume that the reduce argument is second
            func(src_ptr, dst_ptr);
        }

        // Increment the curpos
        i = index_dims - 1;
        curpos[i]++;
        while (curpos[i] == (index_dmax[i] - index_dmin[i]))
        {
            if (i == 0)
            {
                finished = true;
                break;
            }
            curpos[i] = 0;
            i--;
            curpos[i]++;
        }
    }
    while (!finished);

    index->releaseData(::brt::StreamC::READ);
    if (GatherNotScatter)
    {
        src->releaseData(::brt::StreamC::READWRITE);
        dst->releaseData(::brt::StreamC::WRITE);
    }
    else
    {
        src->releaseData(::brt::StreamC::READ);
        dst->releaseData(::brt::StreamC::READWRITE);
    }

    free(curpos);
    free(buf1);
    free(buf2);
}

// TIM: adding conditional magic for raytracer
void streamEnableWriteMask()
{
    LOG(LOG_FN_ENTRY, "streamEnableWriteMask()\n");
    using namespace brt;
    
}

void streamDisableWriteMask()
{
    LOG(LOG_FN_ENTRY, "streamDisableWriteMask()\n");
    using namespace brt;
    
}

void streamSetWriteMask(::brt::stream & inStream)
{
    LOG(LOG_FN_ENTRY, "streamSetWriteMask(%p)\n", &inStream);
    using namespace brt;
    
}

void streamBeginWriteQuery()
{
    LOG(LOG_FN_ENTRY, "streamBeginWriteQuery()\n");
    using namespace brt;
    
}

int streamEndWriteQuery()
{
    LOG(LOG_FN_ENTRY, "streamEndWriteQuery()\n");
    using namespace brt;

    return 0;
}
