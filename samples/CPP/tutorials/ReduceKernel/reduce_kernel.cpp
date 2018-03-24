////////////////////////////////////////////////////////////////////////////////
//!
//! \file   reduce_kernel.cpp
//!
//! \brief  Create and execute a reduce kernel
//!
//! The kernel sums up an input stream and writes the value into an output
//! of size 1.
//!
////////////////////////////////////////////////////////////////////////////////

// Project headers
#include "brookgenfiles/reduce.h"
#include "math.h"

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
            buffer[index] = (float)index;
        }
    }
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

    // Calculating the expected value i.e. sum of natural numbers from 1 to n
    unsigned int n = width * height - 1;
    float sum = (float)(n * (n + 1) / 2);
    fprintf(stdout, "Expected value = %f\n", sum);

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

    unsigned int rankReduce = 2;
    unsigned int streamSizeReduce[] = {1, 1};
    brook::Stream<float> outputStream(rankReduce, streamSizeReduce);

    //--------------------------------------------------------------------------
    // Executing kernel and copying back data
    //--------------------------------------------------------------------------    

    // Calling the kernel on the input and output streams
    reduceGPU(inputStream, outputStream);

    // Creating an output buffer
    float outputBuffer;

    //// Copying data from output stream to output buffer
    outputStream.write(&outputBuffer);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    fprintf(stdout, "Output value = %f\n", outputBuffer);

    //--------------------------------------------------------------------------
    // Verifying results
    //--------------------------------------------------------------------------
    if(fabs(sum - outputBuffer) < 0.000001f)
        printf("Passed\n");
    else
        printf("Failed\n");

    //--------------------------------------------------------------------------
    // Cleaning up
    //--------------------------------------------------------------------------

    delete[] inputBuffer;

    return 0;
}
