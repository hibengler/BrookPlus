////////////////////////////////////////////////////////////////////////////////
//!
//! \file   constants.cpp
//!
//! \brief  Demonstrates the use of constants
//!
////////////////////////////////////////////////////////////////////////////////

// Brook generated headers
#include "brookgenfiles/copy_constantArray.h"

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
            fprintf(stdout, "%3.2f\t", buffer[index]);
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
    // Initializing constant value
    //--------------------------------------------------------------------------

    const float constant[3][2] = {1.0f, 2.0f,
                                  3.0f, 4.0f,
                                  5.0f, 6.0f};
    float result = 0.0f;

    // Printing constant array
    fprintf(stdout, "Constant Array:\n");
    for(unsigned int i = 0; i < 3; ++i)
    {
        for(unsigned int j = 0; j < 2; ++j)
        {
            fprintf(stdout, "%3.2f  ", constant[i][j]);
            result += constant[i][j];
        }
        fprintf(stdout, "\n");
    }

    //--------------------------------------------------------------------------
    // Creating the output stream, calling kernel on output stream and
    // copying data to an output buffer
    //--------------------------------------------------------------------------

    // Specifying the size of the 2D stream
    unsigned int streamSize[] = {width, height};

    // Specifying the rank of the stream
    unsigned int rank = 2;

    // Create a 2D stream of specified size i.e. 64x64 floating-point values    
    brook::Stream<float> outputStream(rank, streamSize);

    // Calling copyconstant kernel on the output stream
    copyConstantArray(constant, outputStream);
    
    // Creating an output buffer for GPU result & cpuOutputBuffer for CPU result
    float* outputBuffer = new float[width * height];
    float* cpuOutputBuffer = new float[width * height];
    memset(outputBuffer, 0, width * height * sizeof(float));
    memset(cpuOutputBuffer, 0, width * height * sizeof(float));

    // Copying data from stream to output buffer
    outputStream.write(outputBuffer);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    fprintf(stdout, "Output buffer value = %3.2f\n", result);
    printBuffer(outputBuffer, width, 0, 0, 8, 8);

    // Intialization of CPU output Buffer
    unsigned int index = 0;
    for(unsigned int i = 0; i < 64; ++i)
    {
        for(unsigned int j = 0; j < 64; ++j)
        {
            index = i*64 + j;
            cpuOutputBuffer[index] = result;
        }
    }

    printBuffer(cpuOutputBuffer, width, 0, 0, 8, 8);

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
    delete[] outputBuffer;
    delete[] cpuOutputBuffer;
    return 0;
}
