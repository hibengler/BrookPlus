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

#include "HelloBrook.h"
#include "brookgenfiles/hello_brook.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

HelloBrook::HelloBrook(char* name) : SampleBase(name)
{
    _input = NULL;
    _output = NULL;
    _result = 0.0f;
    _length = 10;
    _count = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
HelloBrook::allocateBuffer()
{
    _input = Util::allocateBuffer<float>(_length, 1);
    _output = Util::allocateBuffer<float>(_length, 1);
         
    if (!_input || !_output)
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
HelloBrook::fillInputBuffer()
{
    unsigned int i = 0;
    for (i = 0; i < _length; ++i)
    {
        _input[i] = (float)(i);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//  \Equal CPU Code
//
////////////////////////////////////////////////////////////////////////////////

void 
HelloBrook::hellobrook_cpu(float* input)
{
    for (int i = 0; i < 10; ++i)
    {
        if(input[i] > 3.33333)
        {
            _count =_count + 1;
        }
    }
}

void 
HelloBrook::VerifyResults()
{
    hellobrook_cpu(_input);
    
    if(fabs(_result - _count) > 0.0f)
    {
        printf("Failed\n");
    }
    else
    {
        printf("Passed\n");
    }
}
////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
HelloBrook::run()
{
    unsigned int retVal = 0;
    
    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    { 
        unsigned int dim[] = {_length};
        ::brook::Stream<float> inputStream(1, dim);
        ::brook::Stream<float> outputStream(1, dim);
        unsigned int redDim[] = {1};
        ::brook::Stream<float> res(1, redDim);

        inputStream.read(_input);
        hello_brook_check(inputStream, outputStream, (float)_length / 3.0f);
        outputStream.write(_output);
        
        // Handle errors if any
        if(outputStream.error())
        {
            std::cout << "Error occured" << std::endl;
            std::cout << outputStream.errorLog() << std::endl;
            retVal = -1;
        }
                       
        hello_brook_sum(outputStream, res);
        streamWrite(res, &_result);
              
        // Handle errors if any
        if(res.error())
        {
            std::cout << "Error occured" << std::endl;
            std::cout << res.errorLog() << std::endl;
            retVal = -1;
        }
    }

    /////////////////////////////////////////////////////////////////////////
    // Print results
    /////////////////////////////////////////////////////////////////////////
        
    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

HelloBrook::~HelloBrook()
{
    delete [] _input;
    delete [] _output;
}
