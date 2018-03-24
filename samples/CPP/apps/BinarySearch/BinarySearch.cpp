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

#include "BinarySearch.h"
#include "brookgenfiles/binary_search.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

#define epsilon 0.0f
BinarySearch::BinarySearch(char* name) : SampleBase(name)
{
    _array = NULL;
    _searchValues = NULL;
    _indices[0] = NULL;
    _indices[1] = NULL;
    _width = 0;
    _length = 0;
    _lgWidth = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
BinarySearch::allocateBuffer()
{
    _width = info->Width;
    _length = _width * info->Height;

    _array = Util::allocateBuffer<float>(_length, 1);
    _searchValues = Util::allocateBuffer<float>(_width, 1);
    _indices[0] = Util::allocateBuffer<float>(_width, 1);
    _indices[1] = Util::allocateBuffer<float>(_width, 1);
         
    if (!_array || !_searchValues || !_indices[0] || !_indices[1])
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }

    for(unsigned int i = _length; i > 1; i >>= 1)
    {
        _lgWidth++;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
BinarySearch::fillInputBuffer()
{
    Util::fillBuffer(_array, _length, 1, 1, 2, Util::PREVRAND);
    Util::fillBuffer(_searchValues, _width, 1, 1, 2, Util::PREVRAND);

    // Force some array values to exist in searchValues
    for (unsigned int i = 0; i < _width; ++i) 
    {
        if ((rand() & 0x01) == 0)
        {
            _searchValues[i] = _array[rand() % _length];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  searches for each value of \i search in \i array and stores
//! location of found value in \i results or -1 if not found
//!
//! \param array Array of searchable data
//! \param search array of data to find
//! \param results array to store the results
//! 
////////////////////////////////////////////////////////////////////////////////

void
BinarySearch::_searchCPU(float* array, float* search, float* results, unsigned int numSearches,
                         unsigned int Length)
{
    unsigned int i = 0;
    for (i = 0; i < numSearches; ++i)
    {
        unsigned int y = 0;
        float val = -1.0f;
        for (y = 0; y < Length; ++y)
        {
            if (fabs(array[y] - search[i]) <= epsilon)
            {
                val = (float)y;
            }
        }
        results[i] = val;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
BinarySearch::printTimeInfo()
{
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
BinarySearch::verifyResults()
{
    int res = 0;

    std::cout<<"-e Verify correct output.\n";
    std::cout<<"Performing Binary Searches on CPU ... ";

    for (unsigned int i = 0; i < info->Iterations; ++i)
    {
        _searchCPU(_array, _searchValues, _indices[1], _width, _length);
    }

    std::cout << "Done\n";

    res += Util::compareBuffers<float>(_indices[1], _indices[0], _width);
    
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
BinarySearch::comparePerformance()
{
    unsigned int i;
    double cpu_time = 0.0;
    double gpu_time = 0.0;

    gpu_time = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();

    std::cout << "-p Compare performance with CPU.\n";

    // Record CPU Total time
    for(i = 0; i < info->Iterations; i ++)
    {
        _searchCPU(_array, _searchValues, _indices[1], _width, _length);
    }
    
    timer->Stop();

    // Print CPU timing and speedup
    cpu_time = timer->GetElapsedTime();
    
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
              << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
              << std::setw(16) << "GPU Total Time"
              << std::setw(16) << "Speedup" << std::endl;

    std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
              << std::setw(16) << info->Iterations << std::setw(16) << cpu_time
              << std::setw(16) << gpu_time
              << std::setw(16) << cpu_time / gpu_time << std::endl;
    
    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
BinarySearch::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    {
        unsigned int arrayDim[] = {_length};
        unsigned int searchDim[] = {_width};

        ::brook::Stream<float> searchValueStream(1, searchDim);
        ::brook::Stream<float> indicesStream(1, searchDim);
        ::brook::Stream<float> arrayStream(1, arrayDim);

        // Record GPU Total Time 
        timer->Start();

        for (unsigned int i = 0; i < info->Iterations; ++i)
        {
            // Copy searchable data and search keys to streams
            arrayStream.read(_array);
            searchValueStream.read(_searchValues);

            // Execute parallel binary search
            binary_search(searchValueStream, arrayStream, 
                            indicesStream, (float)(_length), _lgWidth); 

            // Copy results from stream
            indicesStream.write(_indices[0]);
            
            //Handle errors if occured
            if(indicesStream.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << indicesStream.errorLog() << std::endl;
                retVal = -1;
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

BinarySearch::~BinarySearch()
{
    delete[] _array;
    delete[] _searchValues;
    delete[] _indices[0];
    delete[] _indices[1];
}
