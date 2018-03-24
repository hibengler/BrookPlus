////////////////////////////////////////////////////////////////////////////////
//!
//! \file   constants.cpp
//!
//! \brief  Demonstrates the use of constants
//!
////////////////////////////////////////////////////////////////////////////////

// brcc generated header
#include "brookgenfiles/add_constants.h"

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

    const float constant0 = 1.0f;
    const float constant1 = 2.0f;
    const float result = constant0 + constant1;

    // Printing input buffer
    fprintf(stdout, "Constant values:\n");
    fprintf(stdout, "\tconstant0 = %f\n", constant0);
    fprintf(stdout, "\tconstant1 = %f\n", constant1);

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

    // Calling addConstant kernel on the output stream
    addConstant(constant0, constant1, outputStream);
    
    // Creating an output buffer for GPU result & cpuOutputBuffer for CPU result
    float* outputBuffer = new float[width * height];
    float* cpuOutputBuffer = new float[width * height];
    memset(outputBuffer, 0, width * height * sizeof(float));
    memset(cpuOutputBuffer, 0, width * height * sizeof(float));


    // Copying data from input stream to output buffer
    outputStream.write(outputBuffer);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    // Printing GPU output Buffer
    fprintf(stdout, "Output buffer value = constant0 + constant1\n");
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

    // Printing CPU output Buffer
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
