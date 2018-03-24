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
#include <iostream>
#include "brookgenfiles/copy.h"


unsigned int count = 0;

void cpuWork(unsigned int itr)
{
    for(unsigned int i = 0; i < itr; ++i)
    {
        ++count;
    }
}

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

int asyncWork()
{
    // Specifying the width and height of the 2D buffer
    const unsigned int width = 1024;
    const unsigned int height = 1024;

    //--------------------------------------------------------------------------
    // Creating and initializing the input buffer
    //--------------------------------------------------------------------------

    // Creating an input buffer
    float* inputBuffer = new float[width * height];

    // Initializing the input buffer such that
    // input(i,j) = i*width + j
    fillBuffer(inputBuffer, width, height);

    // Creating an output buffer
    float* outputBuffer = new float[width * height];
    memset(outputBuffer, 0, width * height * sizeof(float));

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
    while(!inputStream.isSync())
    {
        cpuWork(500);
    }

    std::cout << "\n\nAsynchronous Read: " << "  Count  " << count << "\n\n" << std::endl;


    //--------------------------------------------------------------------------
    // Assigning count to 0 as this count will again increment 
    // at the time of kernel invocation
    //--------------------------------------------------------------------------
    count = 0;

    //--------------------------------------------------------------------------
    // Creating the output stream
    //--------------------------------------------------------------------------    

    brook::Stream<float> outputStream(rank, streamSize);

    //--------------------------------------------------------------------------
    // Executing kernel and copying back data
    //--------------------------------------------------------------------------    

    // Calling the kernel on the input and output streams
    copy(inputStream, outputStream);
    while(!outputStream.isSync())
    {
        cpuWork(500);
    }

    std::cout << "\n\nAsynchronous Kernel: " << "  Count  " << count << "\n\n" << std::endl;

    //--------------------------------------------------------------------------
    // Again assigning count to 0 for stream write
    //--------------------------------------------------------------------------
    count = 0;

    // Copying data from output stream to output buffer
    outputStream.write(outputBuffer, "async");
    while(!outputStream.isSync())
    {
        cpuWork(500);
    }

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    std::cout << "\n\nAsynchronous Write: " << "  Count  " << count << "\n\n" << std::endl;

    //--------------------------------------------------------------------------
    // Checking whether the result is correct or not
    //--------------------------------------------------------------------------
    
    if(!verify(inputBuffer, outputBuffer, width, height))
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
    
    return 0;
}

int syncWork()
{
    // Specifying the width and height of the 2D buffer
    const unsigned int width = 1024;
    const unsigned int height = 1024;

    //--------------------------------------------------------------------------
    // Creating and initializing the input buffer
    //--------------------------------------------------------------------------

    // Creating an input buffer
    float* inputBuffer = new float[width * height];

    // Initializing the input buffer such that
    // input(i,j) = i*width + j
    fillBuffer(inputBuffer, width, height);

    // Creating an output buffer
    float* outputBuffer = new float[width * height];
    memset(outputBuffer, 0, width * height * sizeof(float));

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

    //--------------------------------------------------------------------------
    // Executing kernel and copying back data
    //--------------------------------------------------------------------------    

    // Calling the kernel on the input and output streams
    copy(inputStream, outputStream);

    // Copying data from output stream to output buffer
    outputStream.write(outputBuffer);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    //--------------------------------------------------------------------------
    // Checking whether the result is correct or not
    //--------------------------------------------------------------------------

    if(!verify(inputBuffer, outputBuffer, width, height))
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
    
    return 0;
}



int
main(int argc, char* argv[])
{
    // Normal work will be done
    syncWork();

    //--------------------------------------------------------------------------
    // Some Extra work will done at cpu at the time of stream read stream write 
    // & kernel invocation
    //--------------------------------------------------------------------------
    asyncWork();
}
