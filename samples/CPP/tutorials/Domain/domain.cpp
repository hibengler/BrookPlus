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
#include "brookgenfiles/copy.h"
#include "cpu_copy.h"

////////////////////////////////////////////////////////////////////////////////
//! 
//! \brief  Tolerance between two values for them to be equal
//!
////////////////////////////////////////////////////////////////////////////////

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
            fprintf(stdout, 
                    "%f\t", 
                    buffer[index]);
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

    //--------------------------------------------------------------------------
    // Creating and initializing the input buffer
    //--------------------------------------------------------------------------

    // Creating an input buffer
    float* inputBuffer = new float[width * height];

    // Initializing the input buffer such that
    // input(i,j) = i*width + j
    fillBuffer(inputBuffer, width, height);

    // Printing input buffer
    fprintf(stdout, "Input buffer:\n");
    printBuffer(inputBuffer, width, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // Creating the input stream and copying data from input buffer
    //--------------------------------------------------------------------------

    // Specifying the size of the 2D stream
    unsigned int streamSize[] = {width, height};

    // Specifying the rank of the stream
    unsigned int rank = 2;

    // Create a 2D stream of specified size i.e. 64x64 floating-point values    
    brook::Stream<float> inputStream(rank, streamSize);

    // Copying data from input buffer to input stream
    inputStream.read(inputBuffer);

    //--------------------------------------------------------------------------
    // Creating the output stream
    //--------------------------------------------------------------------------    

    brook::Stream<float> outputStream(rank, streamSize);

    // Creating an output buffer
    float* outputGPU = new float[width * height];
    memset(outputGPU, 0, width * height * sizeof(float));

    // Setting stream values to 0
    outputStream.read(outputGPU);

    //--------------------------------------------------------------------------
    // Executing kernel and copying back data
    //--------------------------------------------------------------------------    

    // Specifying the domain for the output stream
    // For a 2D stream, the start and end coordinates are 2D like (x,y).
    // start = (x0,y0)
    // end = (x1,y1)
    unsigned int start[] = {2,2};
    unsigned int end[] = {width - 1, height - 1};

    // Calling the kernel on the input and output streams
    // Passing the values of start is done to use the values in offsetting the
    // positions from where the values are read in the input stream.
    copy(inputStream.domain(start, end), outputStream.domain(start, end));

    // Copying data from output stream to output buffer
    outputStream.write(outputGPU);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    fprintf(stdout, "Output GPU buffer:\n");
    printBuffer(outputGPU, width, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // CPU implementation
    //--------------------------------------------------------------------------
    float* outputCPU = new float[width * height];
    memset(outputCPU, 0, width * height * sizeof(float));
    copyCPU(inputBuffer, outputCPU, start[0], start[1], end[0], end[1], width);
    fprintf(stdout, "Output CPU buffer:\n");
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
    
    delete[] inputBuffer;
    delete[] outputCPU;
    delete[] outputGPU;
    
    return 0;
}
