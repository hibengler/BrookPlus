////////////////////////////////////////////////////////////////////////////////
//!
//! \file   scatter_stream_kernel.cpp
//!
//! \brief  Create and execute a transpose kernel which uses a scatter stream
//!
////////////////////////////////////////////////////////////////////////////////

// brcc generated header
#include "brookgenfiles/scatter_transpose.h"

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
                    "%3.0f  \t",
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
fillBuffer(float* buffer, 
           const unsigned int width, const unsigned int height)
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
    
    // Output stream for scatter is always 1D and float4
    unsigned int streamSizeScatter[] = {height, width};
    brook::Stream<float> outputStream(2, streamSizeScatter);

    //--------------------------------------------------------------------------
    // Executing kernel and copying back data
    //--------------------------------------------------------------------------    

    // Calling the kernel on the input and output streams
    scatterTransposeGPU(inputStream, outputStream);

    // Creating an output buffer
    float* outputBuffer = new float[width * height];
    float* cpuOutputBuffer = new float[width * height];
    memset(cpuOutputBuffer, 0, width * height * sizeof(float));

    // Copying data from output stream to output buffer
    outputStream.write(outputBuffer);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    fprintf(stdout, "Transpose:GPU Result\n");
    printBuffer(outputBuffer, height, 0, 0, 8, 8);

    // creating CPU Result
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            cpuOutputBuffer[j*height + i] = inputBuffer[i*width + j];
        }
    }

    // Printing CPU Result
    fprintf(stdout, "Transpose:CPU Result\n");
    printBuffer(cpuOutputBuffer, height, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // Checking whether the result is correct or not
    //--------------------------------------------------------------------------

    if(!verify(cpuOutputBuffer, outputBuffer, width, height))
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
    delete[] outputBuffer;
    delete[] cpuOutputBuffer;
    return 0;
}
