////////////////////////////////////////////////////////////////////////////////
//!
//! \file   simple_kernel.cpp
//!
//! \brief  Create and execute a simple kernel
//!
//! The kernel copies data from one 2D stream into another
//!
////////////////////////////////////////////////////////////////////////////////

// Project headers
#include "brookgenfiles/writePos.h"

const double DoubleTolerance = 0.000001;

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
printBuffer(int2* buffer, 
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
            fprintf(stdout, 
                    "(%d,%d)\t",
                    buffer[index].x,
                    buffer[index].y);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n");
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
verify(const int2* src0,
       const int2* src1,
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
            double diff = src0[index].y - src1[index].y;
            double diff1 = src0[index].x - src1[index].x;
            if(isEqual && (diff > DoubleTolerance) && (diff1 > DoubleTolerance))
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

    //--------------------------------------------------------------------------
    // Creating the output stream
    //--------------------------------------------------------------------------    

    // Specifying the size of the 2D stream
    unsigned int streamSize[] = {width, height};

    // Specifying the rank of the stream
    unsigned int rank = 2;

    brook::Stream<int2> outputStream(rank, streamSize);

    // Creating an output buffer
    int2* outputGPU = new int2[width * height];
    int2* outputCPU = new int2[width * height];
    memset(outputGPU, 0, width * height * sizeof(int2));
    memset(outputCPU, 0, width * height * sizeof(int2));

    // Setting stream values to 0
    outputStream.read(outputGPU);

    // Output stream befor calling kernel
    outputStream.write(outputGPU);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    fprintf(stdout, "Output GPU buffer before calling kernel:\n");
    printBuffer(outputGPU, width, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // Executing kernel and copying back data
    //--------------------------------------------------------------------------    

    // Specifying the domain of execution for output stream

    // Offset from the top-left corner
    writePos.domainOffset(uint4(1, 1, 0, 0));

    // Size of domain of execution
    writePos.domainSize(uint4(5, 5, 1, 1));

    // Declaring & Intializing two variables which will use in calculating CPU result
    unsigned int domainStart = 1;
    unsigned int domainEnd = 5;

    // Calling the kernel on the output stream
    writePos(outputStream);

    // Copying data from output stream to output buffer
    outputStream.write(outputGPU);
    fprintf(stdout, "Output GPU buffer after calling kernel:\n");
    printBuffer(outputGPU, width, 0, 0, 8, 8);

    // Calculating CPU result
    unsigned int index = 0;
    for(unsigned int i = 0; i < 64; ++i)
    {
        for(unsigned int j = 0; j < 64; ++j)
        {
            index = i * 64 + j;
            if(domainEnd + 1 > i && i > domainStart - 1 && domainEnd + 1 > j && j > domainStart - 1)
             {
                 outputCPU[index].y = i;
                 outputCPU[index].x = j;
             }
        }
    }

    // Printing CPU result
    fprintf(stdout, "Output CPU buffer :\n");
    printBuffer(outputCPU, width, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // Checking whether the result is correct or not
    //--------------------------------------------------------------------------

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

    delete[] outputGPU;
    delete[] outputCPU;
    return 0;
}
