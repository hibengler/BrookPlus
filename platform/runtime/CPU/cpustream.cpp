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

#ifdef _WIN32
#pragma warning(disable:4786)
//  the above warning disables visual studio's
//  annoying habit of warning when using the standard set lib
#endif

#include "cpu.hpp"
#include <map>
#include <string.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#ifdef __GNUC__
#include <typeinfo>
#endif

using std::map;
using std::string;

namespace brt
{

    CPUStream::CPUStream(unsigned int inFieldCount,
                         const StreamType * inFieldTypes, unsigned int dims, const unsigned int extents[])
    {
        LOG(LOG_FN_ENTRY, "CPUStream::CPUStream(%d, %p, %d, %p)\n", inFieldCount, inFieldTypes, dims, extents);

        unsigned int i;

        elementType.insert(elementType.begin(), inFieldTypes, inFieldTypes + inFieldCount);

#if NEW_SHAPE_STUFF
        assert(dims >= 1 );
        StreamShape t(dims, extents[0],
            dims >= 2 ? extents[1] : 0,
            dims >= 3 ? extents[2] : 0,
            dims >= 4 ? extents[3] : 0);
        logical_shape = t;
        this->domain_min = (unsigned int *)calloc(dims, sizeof(unsigned int));
        this->domain_max = (unsigned int *)malloc(dims * sizeof(unsigned int));
        this->stream_min = (unsigned int *)calloc(dims, sizeof(unsigned int));
        this->stream_max = (unsigned int *)malloc(dims * sizeof(unsigned int));
        for (i = 0; i < dims; ++i)
        {
            this->domain_max[i] = extents[i];
            this->stream_max[i] = extents[i];
        }
        unsigned totalsize = logical_shape.getSize();
#else
        this->extents = (unsigned int *)malloc(dims * sizeof(unsigned int));
        this->domain_min = (unsigned int *)calloc(dims, sizeof(unsigned int));
        this->domain_max = (unsigned int *)malloc(dims * sizeof(unsigned int));

        this->dims = dims;
        totalsize = 1;

        for (i = 0; i < dims; ++i)
        {
            this->extents[i] = extents[i];
            this->domain_max[i] = extents[i];
            totalsize *= extents[i];
        }
#endif
        this->pos = (unsigned int *)malloc(dims * sizeof(unsigned int));
        stride = getElementSize();

        if (stride)
        {
            data = malloc(stride * totalsize);
        }
        else
        {
            std::cerr << "Failure to produce stream: 0 types." << std::endl;
        }

        malloced_size = stride * totalsize;

        isDerived = false;
    }

    CPUStream::CPUStream(const CPUStream & a, const int *dMin,   // FIXME - min/max dangerous identifiers to choose
                         const int *dMax, const int *strMin, const int *strMax)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::CPUStream(%p, %p, %p)\n", &a, dMin, dMax);
        unsigned int i;

        elementType = a.elementType;
        data = a.data;
        stride = a.stride;

        if (strMin == NULL)
            strMin = dMin;
        if (strMax == NULL)
            strMax = dMax;

#if NEW_SHAPE_STUFF
        logical_shape = a.logical_shape;
        unsigned int totalsize = logical_shape.getSize();
        unsigned int dims = logical_shape.getRank();
#else
        extents = a.extents;
        dims = a.dims;
        totalsize = a.totalsize;

#endif

        domain_min = (unsigned int *) malloc(sizeof(int) * dims);
        domain_max = (unsigned int *) malloc(sizeof(int) * dims);
        stream_min = (unsigned int *) malloc(sizeof(int) * dims);
        stream_max = (unsigned int *) malloc(sizeof(int) * dims);

        for (i = 0; i < dims; i++)
        {
            unsigned int d = dims - (i + 1);

            domain_min[i] = a.domain_min[i] + dMin[d];
            domain_max[i] = a.domain_min[i] + dMax[d];
            stream_min[i] = a.domain_min[i] + strMin[i];
            stream_max[i] = a.domain_min[i] + strMax[i];
        }

        pos = (unsigned int *) malloc(sizeof(unsigned int) * dims);
        malloced_size = stride * totalsize;

        isDerived = true;
    }

    CPUStreamShadow::CPUStreamShadow(StreamInterface * s, unsigned int flags)
    {
        LOG(LOG_FN_ENTRY, "CPUStreamShadow::CPUStreamShadow(%p, %08x)\n", s, flags);
        unsigned int i;

#if NEW_SHAPE_STUFF
        StreamShape log_shape = s->getLogicalShape();
        unsigned int dims = log_shape.getRank();
        unsigned int totalsize = log_shape.getSize();
        logical_shape = log_shape;
#else
        dims = s->getDimension();
        totalsize = s->getTotalSize();
        const unsigned int *e = s->getExtents();
        extents = (unsigned int *) malloc(dims * sizeof(unsigned int));
        for (i = 0; i < dims; i++)
        {
            extents[i] = e[i];
        }
#endif
        const unsigned int *dmin = s->getDomainMin();
        const unsigned int *dmax = s->getDomainMax();

        for (i = 0; i < s->getFieldCount(); i++)
        {
            elementType.push_back(s->getIndexedFieldType(i));
        }

        data = s->getData(flags);
        stride = s->getElementSize();

        domain_min = (unsigned int *) malloc(dims * sizeof(unsigned int));
        domain_max = (unsigned int *) malloc(dims * sizeof(unsigned int));
        pos = (unsigned int *) malloc(dims * sizeof(unsigned int));

        for (i = 0; i < dims; i++)
        {
            domain_min[i] = dmin[i];
            domain_max[i] = dmax[i];
        }

        shadow = s;
        this->flags = flags;

        isDerived = true;
    }

    CPUStreamShadow::~CPUStreamShadow()
    {
        LOG(LOG_FN_ENTRY, "PUStreamShadow::~CPUStreamShadow()\n");

#if 0
        assert(extents);
        free(extents);
        extents = NULL;

        assert(pos);
        free(pos);
        pos = NULL;

        assert(domain_min);
        free(domain_min);
        domain_min = NULL;

        assert(domain_max);
        free(domain_max);
        domain_max = NULL;
#endif

        shadow->releaseData(flags);
    }

    void CPUStream::Read(const void *inData)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::Read(%p)\n");
        int i;
        unsigned int *index;
        unsigned int rowlen;
        unsigned char *src = (unsigned char *) inData;

#if NEW_SHAPE_STUFF
        unsigned int dims = logical_shape.getRank();
#endif

        index = (unsigned int *)calloc(dims, sizeof(unsigned int));

        rowlen = domain_max[dims - 1] - domain_min[dims - 1];
        rowlen *= stride;

        while (1)
        {
            void *ptr = fetchItemPtr(data, index);

            memcpy(ptr, src, rowlen);
            src += rowlen;

            if (dims < 2)
            {
                free(index);
                return;
            }

            for (i = ((int) dims) - 2; i >= 0; i--)
            {
                index[i]++;
                if (index[i] >= domain_max[i] - domain_min[i])
                {
                    if (i == 0)
                    {
                        free(index);
                        return;
                    }
                    index[i] = 0;
                }
                else
                    break;
            }
        }
    }

    void CPUStream::Write(void *outData)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::Write(%p)\n", outData);
        int i;
        unsigned int *index;
        unsigned int rowlen;
        unsigned char *dst = (unsigned char *) outData;

#if NEW_SHAPE_STUFF
        unsigned int dims = logical_shape.getRank();
#endif

        index = (unsigned int *)calloc(dims, sizeof(unsigned int));

        rowlen = domain_max[dims - 1] - domain_min[dims - 1];
        rowlen *= stride;

        while (1)
        {
            void *ptr = fetchItemPtr(data, index);

            memcpy(dst, ptr, rowlen);
            dst += rowlen;

            if (dims < 2)
            {
                free(index);
                return;
            }

            for (i = ((int) dims) - 2; i >= 0; i--)
            {
                index[i]++;
                if (index[i] >= domain_max[i] - domain_min[i])
                {
                    if (i == 0)
                    {
                        free(index);
                        return;
                    }
                    index[i] = 0;
                }
                else
                    break;
            }
        }
    }

    CPUStream* CPUStream::Domain(int min, int max)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::Domain(%d, %d)\n", min, max);
#if NEW_SHAPE_STUFF
        assert(logical_shape.getRank() == 1);
#else
        assert(getDimension() == 1);
#endif
        return new CPUStream(*this, &min, &max);
    }

    CPUStream* CPUStream::Domain(const int2 & min, const int2 & max)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::Domain([%d, %d], [%d, %d])\n", min.x, min.y, max.x, max.y);
#if NEW_SHAPE_STUFF
        assert(logical_shape.getRank() == 2);
#else
        assert(getDimension() == 2);
#endif
        return new CPUStream(*this, (const int *) &min, (const int *) &max);
    }

    CPUStream* CPUStream::Domain(const int3 & min, const int3 & max)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::Domain([%d, %d, %d], [%d, %d, %d])\n", min.x, min.y, min.z, max.x, max.y, max.z);
#if NEW_SHAPE_STUFF
        assert(logical_shape.getRank() == 3);
#else
        assert(getDimension() == 3);
#endif
        return new CPUStream(*this, (const int *) &min, (const int *) &max);
    }

    CPUStream* CPUStream::Domain(const int4 & min, const int4 & max)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::Domain([%d, %d, %d, %d], [%d, %d, %d, %d])\n",
            min.x, min.y, min.z, min.w, max.x, max.y, max.z, max.w);
#if NEW_SHAPE_STUFF
        assert(logical_shape.getRank() == 4);
#else
        assert(getDimension() == 4);
#endif
        return new CPUStream(*this, (const int *) &min, (const int *) &max);
    }

    CPUStream* CPUStream::ExecDomain(int num_threads)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::ExecDomain(%d)\n", num_threads);
#if NEW_SHAPE_STUFF
        assert(logical_shape.getRank() == 1);
#else
        assert(getDimension() == 1);
#endif
        int dMin = 0;
        int dMax = num_threads;
        int sMin = stream_min[0];
        int sMax = stream_max[0];
        return new CPUStream(*this, &dMin, &dMax, &sMin, &sMax);
    }

    void *CPUStream::fetchElem(const unsigned int curpos[], const unsigned int bounds[], unsigned int kdim)
    {
        LOG(LOG_FN_ENTRY, "CPUStream::fetchElem(%p, %p, %d)\n", curpos, bounds, kdim);
        unsigned int i;

#if NEW_SHAPE_STUFF
        unsigned int rank = logical_shape.getRank();
#else
        unsigned int rank = getDimension();
#endif
        for (i = 0; i < rank; i++)
            if (i < kdim)
                pos[i] = (unsigned int) ((float) curpos[i] * ((float) (domain_max[i] - domain_min[i]) / bounds[i]));
            else
                pos[i] = 0;

        /* Fetch element from ptr */
        return fetchItemPtr(data, pos);
    }

    CPUStream::~CPUStream()
    {
        LOG(LOG_FN_ENTRY, "CPUStream::~CPUStream()\n");

        assert(domain_min);
        free(domain_min);
        domain_min = NULL;

        assert(domain_max);
        free(domain_max);
        domain_max = NULL;

        assert(pos);
        free(pos);
        pos = NULL;

        if (!isDerived)
        {
#if (!NEW_SHAPE_STUFF)
            assert(extents);
            free(extents);
            extents = NULL;
#endif
            assert(data);
            free(data);
            data = NULL;
        }
    }

    void CPUStream::dump(FILE *f)    // FIXME - not implemented
    {
        LOG(LOG_FN_ENTRY, "CPUStream::dump()\n");
        fputs("dump() not implemented for CPU streams\n", stderr);
    }
}
