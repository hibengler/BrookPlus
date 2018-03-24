////////////////////////////////////////////////////////////////////////////////
//!
//! \file   loop.cpp
//!
//! \brief  Create and execute a loop in a kernel
//!
//! The kernel sums up 3x3 matrices of the input stream
//!
////////////////////////////////////////////////////////////////////////////////

// Project headers
#include "brookgenfiles/loop.h"

////////////////////////////////////////////////////////////////////////////////
//! 
//! \brief  Tolerance between two values for them to be equal
//!
////////////////////////////////////////////////////////////////////////////////

const double DoubleTolerance = 0.000001;

////////////////////////////////////////////////////////////////////////////////
//! 
//! \brief CPU implementation of Kernel
//!
////////////////////////////////////////////////////////////////////////////////

void loopCPU(float* buffer, int width, int height)
{
    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            float sum = 0.0f;

            int end = i * width + j;
            for(int start = 1; start < end; ++start)
            {
                sum += start;
            }

            buffer[end] = sum;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Prints the subset of the buffer
//!
//! \param  buffer  Buffer to be printed
//! \param  w       Width of values to be printed
//! \param  h       Height of values to be printed
//! \param  x       Starting X
//! \param  y       Starting Y
//! \param  pitch   Pitch of the buffer
//!
////////////////////////////////////////////////////////////////////////////////

void
printBuffer(float* buffer, 
            const unsigned int pitch,
            const unsigned int x, const unsigned int y,
            const unsigned int w, const unsigned int h)
{
    // Iterating through the buffer and setting values
    for(unsigned int i = y; i < h; i++)
    {
        for(unsigned int j = x; j < w; j++)
        {
            unsigned int index = i * pitch + j;
            fprintf(stdout, "%3.2f\t", buffer[index]);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fills the buffer with some values
//!
//! \param  buffer  Buffer to be filled
//! \param  width   Buffer width
//! \param  height  Buffer height
//!
////////////////////////////////////////////////////////////////////////////////

void
fillBuffer(float* buffer, const unsigned int width, const unsigned int height)
{
    // Iterating through the buffer and setting values
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            unsigned int index = i * width + j;
            buffer[index] = (float)(index);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compares two buffers for equality
//!
//! \param  src0    Source buffer 0
//! \param  src1    Source buffer 1
//! \param  width   Buffer width
//! \param  height  Buffer height
//!
////////////////////////////////////////////////////////////////////////////////

bool
verify(const float* src0,
       const float* src1,
       const unsigned int width, const unsigned int height)
{
    // Flag for tracking whether buffers are equal or not
    bool isEqual = true;

    // Iterating through the buffers and checking whether the values
    // are equal or not
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            unsigned int index = i * width + j;
            double diff = src0[index] - src1[index];
            if(isEqual && (diff > DoubleTolerance))
            {
                isEqual = false;
            }
        }
    }

    return isEqual;
}

int
main(int argc, char* argv[])
{
    // Specifying the width and height of the 2D buffer
    const unsigned int width = 64;
    const unsigned int height = 64;

    // Initializing dimensions and rank
    unsigned int streamSize[] = {width, height};
    unsigned int rank = 2;

    //--------------------------------------------------------------------------
    // Creating the output stream
    //--------------------------------------------------------------------------

    brook::Stream<float> outputStream(rank, streamSize);

    //--------------------------------------------------------------------------
    // Executing kernel and copying back data
    //--------------------------------------------------------------------------    

    // Calling the kernel on the output stream
    loopGPU(width, outputStream);

    // Creating an output buffer
    float* outputGPU = new float[width * height];

    // Copying data from output stream to output buffer
    outputStream.write(outputGPU);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    fprintf(stdout, "GPU Output buffer:\n");
    printBuffer(outputGPU, width, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // Calculating CPU result
    //--------------------------------------------------------------------------

    // Allocating CPU output buffer
    float* outputCPU = new float[width * height];

    // Calculating result
    loopCPU(outputCPU, width, height);
    fprintf(stdout, "CPU Output buffer:\n");
    printBuffer(outputCPU, width, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // Checking whether the result is correct or not
    //--------------------------------------------------------------------------
    
    fprintf(stdout, "Result:\n");
    if(!verify(outputCPU, outputGPU, width, height))
    {   
        fprintf(stdout, "Failed.\n");
    }
    else
    {        
        fprintf(stdout, "Passed.\n");
    }

    //--------------------------------------------------------------------------
    // Cleaning up
    //--------------------------------------------------------------------------
    
    delete[] outputCPU;
    delete[] outputGPU;
    
    return 0;
}
