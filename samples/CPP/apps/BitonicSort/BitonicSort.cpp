/****************************************************************************

Copyright (c) 2008, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Advanced Micro Devices, Inc nor the names of its contributors
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

#include "BitonicSort.h"
#include "brookgenfiles/bitonic_sort.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

BitonicSort::BitonicSort(char* name) : SampleBase(name)
{
    _lgArraySize = 0;
    _stage = 0;

    _input = NULL;
    _output = NULL;
    _array[0] = NULL;
    _array[1] = NULL;
    _array[2] = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
BitonicSort::allocateBuffer()
{
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    unsigned int Length = Width * Height;

    _input = Util::allocateBuffer<float>(Width, Height);
    _output = Util::allocateBuffer<float>(Width, Height);
        
    if (!_input || !_output)
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }

    for(unsigned int i = 0; i < 3; i++)
    {
        _array[i] = Util::allocateBuffer<float>(Length, 1);
        
        if(!_array[i])
        {
            fprintf(stderr, "Error: Memory Allocation.\n");
            exit(-1);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
BitonicSort::fillInputBuffer()
{
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    unsigned int Length = Width * Height;
    
    Util::fillBuffer(_input, Width, Height, 1, 100, Util::RANDOM);
    Util::fillBuffer(_array[0], Width, Height, 1, 100, Util::RANDOM);

    for (_stage = Length >> 1; _stage; _lgArraySize++)
    {
        _stage >>= 1;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  runs qsort on the data
//!         qsort implementation taken from a public domain implementation at:
//!         http://alienryderflex.com/quicksort
//!         This public-domain C implementation by Darel Rex Finley.
//!
//! \param input    The randomize input matrix to be sorted
//! \param height   The height of the matrix
//! \param width    The width of the matrix
//! 
////////////////////////////////////////////////////////////////////////////////

void 
BitonicSort::sort_cpu(float* input, int height, int width)
{
    float  piv = 0;
    int beg[1000] = { 0 };
    int end[1000] = { 0 };
    int i = 0;
    int L = 0;
    int R = 0;
    int swap = 0;

    beg[0] = 0;
    end[0] = width * height;
    while (i >= 0) {
        L = beg[i];
        R = end[i] - 1;
        if (L < R)
        {
            piv = input[L];
            while (L < R)
            {
                while (input[R] >= piv && L < R)
                {
                    R--;
                }
                if (L < R)
                {
                    input[L++] = input[R];
                }
                while (input[L] <= piv && L < R)
                {
                    L++;
                }
                if (L < R)
                {
                    input[R--] = input[L];
                }
            }
            input[L] = piv;
            beg[i + 1] = L + 1;
            end[i + 1] = end[i];
            end[i++] = L;
            if ((end[i] - beg[i]) > (end[i - 1] - beg[i - 1]))
            {
                swap = beg[i];
                beg[i] = beg[i - 1];
                beg[i - 1] = swap;
                swap = end[i];
                end[i] = end[i - 1];
                end[i - 1] = swap;
            }
        }
        else
        {
            i--;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
BitonicSort::printTimeInfo()
{
    int Width = info->Width;
    int Height = info->Height;

    double time = timer->GetElapsedTime();
	
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::endl;

    std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
                  << std::setw(16) << info->Iterations << std::setw(16) << time
                  << std::endl;

    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
BitonicSort::verifyResults()
{
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    unsigned int Length = Width * Height;
    int res = 0;

    std::cout<<"-e Verify correct output.\n";
    std::cout<<"Performing Bitonic Sort on CPU ... ";

    sort_cpu(_array[0], 1, Length);
    //sort_cpu(_input, Height, Width);
    
    std::cout << "Done\n";

    res += Util::compareBuffers<float>(_array[0], _array[1], Height * Width);
    //res += Util::compareBuffers<float>(_output, _input, Height * Width);
    if (res)
    {
        std::cout << _name << ": Failed!\n\n";
    }
    else
    {
        std::cout << _name << ": Passed!\n\n" ;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compare performance with CPU if requested
//! 
////////////////////////////////////////////////////////////////////////////////
void
BitonicSort::comparePerformance()
{
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    unsigned int Length = Width * Height;
    unsigned int i;
	double cpuTime = 0.0;
	double gpuTime = 0.0;
    
    gpuTime = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();
    
    std::cout << "-p Compare performance with CPU.\n";
	
    // Record CPU Total time
    for(i = 0; i < info->Iterations; i ++)
    {
	    sort_cpu(_array[0], 1, Length);
        //sort_cpu(_input, Height, Width);
    }
	
    timer->Stop();
    cpuTime = timer->GetElapsedTime();

	// Print CPU timing and speedup
	std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
              << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
              << std::setw(16) << "GPU Total Time"
              << std::setw(16) << "Speedup" << std::endl;

    std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
              << std::setw(16) << info->Iterations << std::setw(16) << cpuTime
              << std::setw(16) << gpuTime
              << std::setw(16) << cpuTime / gpuTime << std::endl;
	
    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
BitonicSort::run()
{
    unsigned int retVal = 0;
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    unsigned int Length = Width * Height;
    
    /////////////////////////////////////////////////////////////////////////
	// Brook code block
	/////////////////////////////////////////////////////////////////////////
    {
        unsigned int dim2D[] = {Width, Height};
        unsigned int dim1D[] = {Length};
        float2 maxvalue   = float2((float)Height, (float)Width);
        unsigned int flip = 0;

        ::brook::Stream<float> sorted1Strm(1, dim1D);
        ::brook::Stream<float> sorted2Strm(1, dim1D);
        ::brook::Stream<float> input2D(2, dim2D);
        ::brook::Stream<float> output2D(2, dim2D);

		// Record GPU Total time
        timer->Start();

        for (unsigned int i = 0; i < info->Iterations; ++i)
        {
			// Write to stream 
            sorted1Strm.read(_array[0]);
            //input2D.read(_input]);

            for(_stage = 1; _stage <= _lgArraySize; _stage++)
            {
                unsigned int step = 0;

                // Width of each sorted segment to be sorted in parallel (2, 4, 8, ...)
                float segWidth = (float)pow(2.0f, (int)_stage);

                for (step = 1; step <= _stage; ++step)
                {
                    // offset = (stageWidth/2, stageWidth/4, ... , 2, 1)
                    float offset = (float)pow(2.0f, (int)(_stage - step));

                    if (!flip)
                    {
                        bitonic(sorted1Strm, sorted2Strm, segWidth, offset, offset * 2.0f);
                        //bitonic_2D(input2D, output2D, segWidth, offset, 2 * offset, maxvalue);
                    }
                    else
                    {
                        bitonic(sorted2Strm, sorted1Strm, segWidth, offset, offset * 2.0f);
                        //bitonic_2D(output2D, input2D, segWidth, offset, 2 * offset, maxvalue);
                    }

                    flip ^= 0x01; // XOR flip w/ 0b1 which flips the flip variable between 0 and 1
                }
            }

            // Write data back from streams
            if(flip)
            {
                sorted2Strm.write(_array[1]);
                //output2D.write(_output);

                //Handle errors if any
                if(sorted2Strm.error())
                {
                    std::cout << "Error occured" << std::endl;
                    std::cout << sorted2Strm.errorLog() << std::endl;
                    retVal = -1;
                }
            }
            else
            {
                sorted1Strm.write(_array[1]);
                //input2D.write(_output);

                //Handle errors if any
                if(sorted1Strm.error())
                {
                    std::cout << "Error occured" << std::endl;
                    std::cout << sorted1Strm.errorLog() << std::endl;
                    retVal = -1;
                }
            }
		}

        timer->Stop();
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

BitonicSort ::~BitonicSort()
{
    if(_input != NULL)
    {
        delete [] _input;
    }

    if(_output != NULL)
    {
        delete [] _output;
    }

    for(unsigned int i = 0; i < 3; i++)
    {
        if(_array[i] != NULL)
        {
            delete [] _array[i];
        }
    }
}
