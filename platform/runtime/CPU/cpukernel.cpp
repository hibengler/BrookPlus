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

#include <iostream>
#include <assert.h>
#include "cpu.hpp"
#include <stdio.h>

#ifdef __GNUC__
#include <typeinfo>
#endif

//#include "CPU/brtarray.hpp"

static brt::CPUKernel * current_cpu_kernel = NULL;

//          The indexof function

// FIXME - double-underscore prefix needs removing

__BrtFloat4 __indexof(const void *ptr)
{
    LOG(LOG_FN_ENTRY, "__indexof(%p)\n", ptr);

    unsigned int i;

    assert(current_cpu_kernel);

    ::brt::CPUKernel * k = current_cpu_kernel;

    for (i = 0; i < k->input_args.size(); i++)
    {
        //k->input_args[i] can be CPUStream* or CPUIter*
        if (typeid(*k->input_args[i]) != typeid(::brt::CPUStream))
            continue;

        ::brt::CPUStream * s = (::brt::CPUStream *) k->input_args[i];
        unsigned char *p = (unsigned char *) ptr;
        unsigned char *p_begin = ((unsigned char *) s->getData(::brt::StreamInterface::READ));
        unsigned char *p_end = p_begin + s->malloced_size;

        if (p >= p_begin && p < p_end)
            return s->computeIndexOf((unsigned int) (p - p_begin) / s->stride);
    }

    for (i = 0; i < k->output_args.size(); i++)
    {
        ::brt::CPUStream * s = (::brt::CPUStream *) k->output_args[i];
        unsigned char *p = (unsigned char *) ptr;
        unsigned char *p_begin = ((unsigned char *) s->getData(::brt::StreamInterface::READ));
        unsigned char *p_end = p_begin + s->malloced_size;

        if (p >= p_begin && p < p_end)
            return s->computeIndexOf((unsigned int) (p - p_begin) / s->stride);
    }

    fprintf(stderr, "Indexof called on bogus address\n");
    assert(0);

    return __BrtFloat4();
}

__BrtInt1 __get_GroupSize()
{
    assert(current_cpu_kernel);

    unsigned int size[4] = { 0, 0, 0, 0 };
    ::brt::CPUKernel * k = current_cpu_kernel;

    for(unsigned int i = 0; i < k->groupDim; ++i)
    {
        size[i] = k->groupSize[0];
    }

    //! Presently we are returning only 1D group size
    return __BrtInt1(size[0]); 
}

__BrtInt4 __get_threadId__InGroup(__BrtInt1 threadId)
{
    assert(current_cpu_kernel);

    unsigned int size[4] = { 0, 0, 0, 0 };
    ::brt::CPUKernel * k = current_cpu_kernel;

    for(unsigned int i = 0; i < k->groupDim; ++i)
    {
        size[i] = k->groupSize[0];
    }

    unsigned int absId = (unsigned int)threadId;

    return __BrtInt4(absId % size[0], size[1], size[2], size[3]); 
}

__BrtInt4 __get_scatter__address(__BrtInt1 threadId)
{
    assert(current_cpu_kernel);

    ::brt::CPUKernel * k = current_cpu_kernel;
    
    unsigned int dim = k->output_args[0]->getDimension();
    unsigned int* domainStartPoint = k->domainStartPoint;
    ::brt::StreamShape log_shape = k->output_args[0]->getLogicalShape();
    const unsigned int* extents = log_shape.getExtents();
    
    assert(dim > 0 && dim <= 4);
    
    unsigned int index[4] = { 0, 0, 0, 0 };
    unsigned int largestaddr = 0;
    
    
    switch(dim)
    {
        case 1:
            if(domainStartPoint != NULL)
            {
                largestaddr = domainStartPoint[0] + threadId.getAt(0);
            }
            else
            {
                largestaddr = threadId.getAt(0);
            }
            index[0] = largestaddr % extents[0];
            break;
        case 2:
            if(domainStartPoint != NULL)
            {
                largestaddr = domainStartPoint[0] + domainStartPoint[1] * extents[1]
                              + threadId.getAt(0);
            }
            else
            {
                largestaddr = threadId.getAt(0);
            }
            index[1] = largestaddr / extents[1];
            index[0] = largestaddr % extents[0];
            break;
        case 3:
            if(domainStartPoint != NULL)
            {
                largestaddr = domainStartPoint[0] + domainStartPoint[1] * extents[2]
                              + domainStartPoint[2] * extents[2] * extents[1]
                              + threadId.getAt(0);
            }
            else
            {
                largestaddr = threadId.getAt(0);
            }
            index[2] = largestaddr / (extents[1] * extents[2]);
            index[1] = (largestaddr - (int)index[2] * (extents[1] * extents[2])) / extents[2];
            index[0] = (largestaddr - (int)index[2] * (extents[1] * extents[2]) - (int)index[1] * extents[2]);
            break;
    }

    return __BrtInt4(index[0], index[1], index[2], index[3]);        
}

namespace brt
{

    CPUKernel::CPUKernel(const void *source[])
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::CPUKernel(%p)\n", source);
        const char **src = (const char **) (source);

        func = 0;

        for (unsigned int i = 0;; i += 2)
        {
            if (src[i] == NULL)
            {
                if (!func)
                {
                    func = NULL;
                    std::cerr << "CPUKernel failure - ";
                    std::cerr << "no CPU program string found.";
                    std::cerr << std::endl;
                }
                break;
            }
            if (strcmp(src[i], "cpu") == 0)
            {
                func = (callable *) source[i + 1];
                assert(func);
            }
        }

        curpos = NULL;
        extents = NULL;
        minpos = NULL;
        maxpos = NULL;
        domainStartPoint = NULL;
        domainEndPoint = NULL;
        dims = 0;

        Cleanup();
    }

    void CPUKernel::PushStreamInterface(StreamInterface * s)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::PushStreamInterface(%p)\n", s);
        if (!s->isCPU())
        {
            s = new CPUStreamShadow(s,::brt::StreamC::READ);
            freeme.push_back(s);
        }

        args.push_back(s);
        input_args.push_back(s);
    }


    void CPUKernel::PushIter(Iter * i)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::PushIter(%p)\n", i);
        PushStreamInterface(i);
    }

    void CPUKernel::PushStream(StreamC * s)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::PushStream(%p)\n", s);
        PushStreamInterface(s);
    }

    void CPUKernel::PushConstant( void* val)
    {
        args.push_back(val);
    }    

    void CPUKernel::PushGatherStream(StreamC * s)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::PushGatherStream(%p)\n");

        if (!s->isCPU())
        {
            s = new CPUStreamShadow(s,::brt::StreamC::READ);
            freeme.push_back(s);
        }

        // Template only determins return type
        // This can be recasted to any other type
        __BrtArray < unsigned char >*array = new __BrtArray < unsigned char >(s);

        args.push_back(array);
        freeme_array.push_back(array);
    }

    void CPUKernel::PushScatterStream(StreamC * s)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::PushScatterStream(%p)\n");
        if (!s->isCPU())
        {
            s = new CPUStreamShadow(s,::brt::StreamC::WRITE);
            freeme.push_back(s);
        }

        // Template only determins return type
        // This can be recasted to any other type

        __BrtArray < unsigned char >*array = new __BrtArray < unsigned char >
            (s, s->getStreamMin(), s->getStreamMax());

        args.push_back(array);
        output_args.push_back(s);
        freeme_array.push_back(array);
    }
   void CPUKernel::PushReduceStream(StreamC * s, StreamType type)
   {
       LOG(LOG_FN_ENTRY, "CPUKernel::PushReduce(%p, %d)\n", s->getData(0), type);
       
       if (!s->isCPU())
       {
           s = new CPUStreamShadow(s,::brt::StreamC::WRITE);
           freeme.push_back(s);
       }
       args.push_back(s);
       output_args.push_back(s);
       reduce_is_scalar = false;
       reduce_arg = (void *) s;
   }
   void CPUKernel::PushReduce(void *data, StreamType type)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::PushReduce(%p, %d)\n", data, type);
        if (type == __BRTSTREAM)
        {
            StreamInterface *s = (StreamInterface *) (*(const ::brt::stream *) data);

            if (!s->isCPU())
            {
                s = new CPUStreamShadow(s, ::brt::StreamC::WRITE);
                freeme.push_back(s);
            }
            args.push_back(s);
            output_args.push_back(s);
            reduce_is_scalar = false;
            reduce_arg = (void *) s;

        }
        else
        {
            args.push_back(data);
            reduce_is_scalar = true;
            reduce_arg = data;
        }

    }

    void CPUKernel::PushOutput(StreamC * s)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::PushOutput(%p)\n", s);
        if (!s->isCPU())
        {
            s = new CPUStreamShadow(s,::brt::StreamC::WRITE);
            freeme.push_back(s);
        }

        args.push_back(s);
        output_args.push_back(s);
    }

    void CPUKernel::Cleanup()
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::Cleanup()\n");
        args.clear();
        output_args.clear();
        input_args.clear();

        if (curpos)
            free(curpos);
        if (extents)
            free(extents);
        if (minpos)
            free(minpos);
        if (maxpos)
            free(maxpos);

        curpos = NULL;
        extents = NULL;
        minpos = NULL;
        maxpos = NULL;
        reduce_arg = NULL;
        reduce_value = NULL;

        dims = 0;
        reduce_is_scalar = false;
        is_reduce = false;

        for (unsigned int i = 0; i < freeme.size(); i++)
            freeme[i]->releaseReference();

        for (unsigned int i = 0; i < freeme_array.size(); i++)
            delete freeme_array[i];

        freeme.clear();
        freeme_array.clear();
    }

    bool CPUKernel::isValidAddress(int idx)
    {
        if(domainStartPoint == NULL || domainEndPoint == NULL)
        {
            return true;
        }

        unsigned int temp[4];

        const unsigned int *dmin;
        const unsigned int *dmax;
        
        dmin = output_args[0]->getDomainMin();
        dmax = output_args[0]->getDomainMax();

#if NEW_SHAPE_STUFF
        StreamShape out_log_shape = output_args[0]->getLogicalShape();
        dims = out_log_shape.getRank();
#else
        dims = output_args[0]->getDimension();
#endif

        /* Create the extents vector */
        for (unsigned int i = 0; i < dims; i++)
        {
            int delta = dmax[i] - dmin[i];
            extents[i] = delta;
        }

        // Save the CPU kernel
        current_cpu_kernel = this;
        
        switch(dims)
        {
            case 1:
                temp[0] = idx % extents[0];
                break;
            case 2:
                temp[1] = idx / extents[1];
                temp[0] = idx % extents[1];
                break;
            case 3:
                temp[2] = idx / (extents[2] * extents[1]);
                temp[1] = (idx - temp[2] * (extents[2] * extents[1]))/ extents[2];
                temp[0] = idx - temp[2] *(extents[2] * extents[1]) - temp[1] * extents[2];                
                break;
        }

        for(unsigned int k = 0; k < dims; k++)
        {
            if(temp[k] < domainStartPoint[k] || temp[k] >= domainEndPoint[k])
            {
                return false;
            }
        }

        return true;
    }

    void CPUKernel::Map(unsigned int* startExecDomain, unsigned int* domainDimension)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::Map()\n");
        unsigned int i;

        /* First check to see if all of the outputs
         ** are of the same size and get the dim */
        assert(output_args.size() > 0);
        
        const unsigned int *dmin;
        const unsigned int *dmax;
        
        // Keeping the domain start point for later use
        domainStartPoint = startExecDomain;

        dmin = output_args[0]->getDomainMin();
        dmax = output_args[0]->getDomainMax();

#if NEW_SHAPE_STUFF
        StreamShape out_log_shape = output_args[0]->getLogicalShape();
        dims = out_log_shape.getRank();
#else
        dims = output_args[0]->getDimension();
#endif

        for (i = 1; i < output_args.size(); i++)
        {
#if NEW_SHAPE_STUFF
            unsigned int out_rank = output_args[i]->getLogicalShape().getRank();
#else
            unsigned int out_rank = output_args[i]->getDimension();
#endif
            assert(out_rank == dims);
            /*for (j = 0; j < dims; j++)
                assert(output_args[i]->getDomainMax()[j] - output_args[i]->getDomainMin()[j] == dmax[j] - dmin[j]);*/
        }

        /* Initialize the position vector */
        curpos = (unsigned int *) malloc(dims * sizeof(unsigned int));
        extents = (unsigned int *) malloc(dims * sizeof(unsigned int));
        minpos = (unsigned int *) malloc(dims * sizeof(unsigned int));
        maxpos = (unsigned int *) malloc(dims * sizeof(unsigned int));

        for (i = 0; i < dims; i++)
        {
            curpos[i] = 0;
            minpos[i] = 0;
        }

        /* Create the extents vector */
        for (i = 0; i < dims; i++)
        {
            int delta = dmax[i] - dmin[i];

            assert(delta >= 0);

            if (delta == 0)
            {
                // If delta is 0 than the
                // output stream has a dimension of 0 size.
                Cleanup();
                return;
            }
            else
                extents[i] = delta;
        }

        for (i = 0; i < dims; i++)
            maxpos[i] = extents[i];

        // Save the CPU kernel
        current_cpu_kernel = this;

        if(startExecDomain != NULL || domainDimension != NULL)
        {
            unsigned int kernelStart = startExecDomain[0];
            unsigned int kernelEnd = 0;
            unsigned int temp[4];

            for(unsigned k = 0; k < dims; k++)
            {
                temp[k] = startExecDomain[k] + domainDimension[k];
            }
            kernelEnd = temp[0];

            switch(dims)
            {
                case 2:
                    kernelStart += extents[1] * startExecDomain[1];
                    kernelEnd += extents[1] * temp[1];
                    break;
                case 3:
                    kernelStart += extents[1] * startExecDomain[1] + extents[0] * extents[1] * startExecDomain[2];
                    kernelStart += extents[1] * temp[1] + extents[0] * extents[1] * temp[2];
                    break;
            }

            domainStartPoint = startExecDomain;
            domainEndPoint = temp;

            func(this, kernelStart, kernelEnd, false);

        }
        else
        {
            // OpenMP optimization - Call the kernel
            func(this, 0, TotalItems(), false);
        }

        // Free the resources
        Cleanup();
    }

    void* CPUKernel::getVectorElement(unsigned int index)
    {
        return args[index];
    }

    void CPUKernel::Reduce(unsigned int* startExecDomain, unsigned int* domainDimension)
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::Reduce()\n");
        unsigned int i;
        StreamInterface *rstream;
        StreamInterface *instream;

        assert(output_args.size() == 1 || (output_args.size() == 0 && reduce_is_scalar));

        assert(input_args.size() == 1);

        // These are the reduction arguments
        instream = input_args[0];
        
        // Set the is_reduce flag
        is_reduce = true;

        // Create the counter arrays
#if NEW_SHAPE_STUFF
        StreamShape in_log_shape = instream->getLogicalShape();
        dims = in_log_shape.getRank();
#else
        dims = instream->getDimension();
#endif
        curpos = (unsigned int *) malloc(dims * sizeof(unsigned int));
        extents = (unsigned int *) malloc(dims * sizeof(unsigned int));
        minpos = (unsigned int *) malloc(dims * sizeof(unsigned int));
        maxpos = (unsigned int *) malloc(dims * sizeof(unsigned int));

        /* Create the extents vector */
        const unsigned int *dmin = instream->getDomainMin();
        const unsigned int *dmax = instream->getDomainMax();

        for (i = 0; i < dims; i++)
        {
            assert(dmax[i] >= dmax[i]);
            // XXX: I have no idea what will happen if delta == 0
            unsigned int delta = dmax[i] - dmin[i];

            extents[i] = delta;
        }

        // Save the CPU kernel
        current_cpu_kernel = this;

        if (reduce_is_scalar)
        {
            reduce_value = (void *) reduce_arg;

            /* Initialize counter */
            for (i = 0; i < dims; i++)
            {
                curpos[i] = 0;
                minpos[i] = 0;
                maxpos[i] = extents[i];
                LOG(LOG_COMMENTARY, "\tcurpos[%d] = %d, minpos[%d] = %d, maxpos[%d] = %d\n",
                    i, curpos[i], i, minpos[i], i, maxpos[i]);
            }

            // Fetch the initial value
            void *initial = instream->fetchElem(curpos, extents, dims);

            // Set the initial value
            memcpy(reduce_value, initial, instream->getElementSize());

            // OpenMP - Perform the reduce
            if (Continue())
                                func(this, 1, TotalItems(), true);

            Cleanup();
            return;
        }

        assert(!reduce_is_scalar);

        /*  Reduce to Stream
         **  This is a bit more complicated.
         **  Here we call the reduction kernel on
         **  sub-blocks of the reduction regions.
         */

        /* Create the reduction variable vectors */
        rstream = output_args[0];

        // Hopefully we are reduce streams of the same type
        assert(rstream->getElementSize() == instream->getElementSize());

#if NEW_SHAPE_STUFF
        unsigned int reduce_dims = rstream->getLogicalShape().getRank();
#else
        unsigned int reduce_dims = rstream->getDimension();
#endif
        unsigned int *reduce_curpos = (unsigned int *) malloc(reduce_dims * sizeof(unsigned int));
        unsigned int *reduce_extents = (unsigned int *) malloc(reduce_dims * sizeof(unsigned int));

        // Need to implement!!!
        if (dims != reduce_dims)
        {
            fprintf(stderr, "Reductions of streams with different " "dimensionality not implemented yet, sorry.\n");
            assert(dims == reduce_dims);
            exit(1);
        }

        /* Initialize the reduction counters */
        dmin = rstream->getDomainMin();
        dmax = rstream->getDomainMax();
        
        for (i = 0; i < reduce_dims; i++)
        {
            reduce_curpos[i] = 0;
            assert(dmax[i] > dmin[i]);
            reduce_extents[i] = dmax[i] - dmin[i];
        }

        do
        {
            bool exit_cond = false;

            // This is the value we are going
            // to reduce into
            reduce_value = rstream->fetchElem(reduce_curpos, reduce_extents, reduce_dims);

            // Compute the curpos which corresponds
            // to this reduction element
            for (i = 0; i < dims; i++)
            {
                if (extents[i] % reduce_extents[i] != 0)
                {
                    fprintf(stderr, "CPU: Error reduction stream " "not a multiple size of input stream.\n");
                    assert(extents[i] % reduce_extents[i] == 0);
                    exit(1);
                }

                int factor = extents[i] / reduce_extents[i];

                curpos[i] = factor * reduce_curpos[i];
                minpos[i] = curpos[i];
                maxpos[i] = factor * (reduce_curpos[i] + 1);
            }

            // Fetch the initial value
            void *initial = instream->fetchElem(curpos, extents, dims);

            // Set the initial value
            memcpy(reduce_value, initial, rstream->getElementSize());

            // OpenMP - Perform the reduce on the sub-block
            if (Continue())
            {
                func(this, 1, TotalItems(), true);
            }

            // Increment the reduction_curpos
            for (i = 0; i < reduce_dims; i++)
            {
                reduce_curpos[i]++;
                if (reduce_curpos[i] == reduce_extents[i])
                {
                    if (i == reduce_dims - 1)
                    {
                        exit_cond = true;
                        break;
                    }
                    reduce_curpos[i] = 0;
                }
                else
                    break;
            }

            // Exit condition
            if (exit_cond)
                break;

        }
        while (1);

        // Free the resources
        Cleanup();

        free(reduce_curpos);
        free(reduce_extents);
    }

    
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Sets the kernel groupsize and group dimension 
//!
//! \param size kernel group size 
//! \param dim  kernel group dimension (default value is 1) 
//!
////////////////////////////////////////////////////////////////////////////////
    void CPUKernel::SetGroupSize(unsigned int* size, unsigned int dim)
    {
        groupSize = size;
        groupDim = dim;
    }

    bool CPUKernel::Continue()  // FIXME - ugly, needs cleanup
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::Continue()\n");
        int i;

        // Increment the curpos
        for (i = dims - 1; i >= 0; i--)
        {
            curpos[i]++;
            if (curpos[i] == maxpos[i])
            {
                if (i == 0)
                    return false;
                curpos[i] = minpos[i];
            }
            else
                break;
            if (i == 0)
                break;
        }
        return true;
    }

        // OpenMP
    int CPUKernel::TotalItems() const {
        int totalwork=1;

                for (int i=dims-1; i>=0; i--) {
                  totalwork*=maxpos[i]-minpos[i];
                }
                //fprintf(stderr, "Thread %u totalwork=%u\n", (unsigned int) pthread_self(), totalwork);
                return totalwork;
        }

        void * CPUKernel::FetchElem(StreamInterface *s, int idx)
        {
                // Save the CPU kernel
                current_cpu_kernel = this;

                if (is_reduce &&
                        s == (StreamInterface *) reduce_arg)
                  return reduce_value;

                //if(!(idx % 100000))
                //  fprintf(stderr, "Thread %u fetching element %u\n", (unsigned int) pthread_self(), idx);
                unsigned int *curpos = (unsigned int *) alloca(dims * sizeof (unsigned int));
                for(int u=0; u<(int) dims; u++) {
                  unsigned int part=1;
                  for(int i=dims-1; i>u; i--) {
                        part*=maxpos[i]-minpos[i];
                  }
                  curpos[u]=minpos[u]+idx/part;
                  idx%=part;
                }
                // Let the stream do the fetch
                void *ret=s->fetchElem(curpos, extents, dims);
                //fprintf(stderr, "Item is at %p\n", ret);
                return ret;
  }



    void CPUKernel::Release()
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::Release()\n");
        delete this;
    }

    CPUKernel::~CPUKernel()
    {
        LOG(LOG_FN_ENTRY, "CPUKernel::~CPUKernel()\n");
        // Do nothing
    }

    kernel::~kernel()
    {
        if( _kernel != 0 )
            _kernel->Release();
    }

    kernel::operator CPUKernel*() const
    {
        return _kernel;
    }

    CPUKernel* kernel::operator->() const
    {
        return _kernel;
    }

}
