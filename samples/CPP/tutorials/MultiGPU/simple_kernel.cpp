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
#include "brook/Device.h"
#include "CPUThread.h"
#include "Timer.h"
#include<iostream>

////////////////////////////////////////////////////////////////////////////////
//! 
//! \brief  Tolerance between two values for them to be equal
//!
////////////////////////////////////////////////////////////////////////////////

const double DoubleTolerance = 0.000001;

// Specifying the width and height of the 2D buffer
unsigned int width = 64;
unsigned int height = 64;

// number of devices found
unsigned int count = 0;

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
fillBuffer(float* buffer)
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
       const float* src1)
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
            if((diff > DoubleTolerance))
            {
                isEqual = false;
            }
        }
    }

    return isEqual;
}

struct ThreadData
{
    brook::Device* device;
    float* inData;
    float* outData;
};

void* run(void* data)
{
    ThreadData* threadData = (ThreadData*)data;

    brook::useDevices(threadData->device, 1, NULL);

    //--------------------------------------------------------------------------
    // Creating the input stream and copying data from input buffer
    //--------------------------------------------------------------------------

    // Specifying the size of the 2D stream
    unsigned int streamSize[] = {width, height/count};

    // Specifying the rank of the stream
    unsigned int rank = 2;

    // Create a 2D stream of specified size i.e. 64x64 floating-point values    
    brook::Stream<float> inputStream(rank, streamSize);

    // Copying data from input buffer to input stream
    inputStream.read(threadData->inData);

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
    outputStream.write(threadData->outData);

    // Check error on stream
    if(outputStream.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", outputStream.errorLog());
    }

    return NULL;
}

int
main(int argc, char* argv[])
{   
    brook::Device* device = brook::getDevices("cal", &count);

    // Printing number of cal devices found
	
	printf("Number of cal Devices found = %d\n", count);
	
	//--------------------------------------------------------------------------
    // Creating and initializing the input buffer
    //--------------------------------------------------------------------------

    float* inData = new float[width * height];

    // Initializing the input buffer such that
    // input(i,j) = i*width + j
    fillBuffer(inData);

    // Printing input buffer
    fprintf(stdout, "Input buffer:\n");
    printBuffer(inData, width, 0, 0, 8, 8);

    // Creating an output buffer
    float* outData = new float[width * height];
    memset(outData, 0, width * height * sizeof(float));

    ThreadData* data = new ThreadData[count];
    CPUThread* threads = new CPUThread[count];

    //-------------------------------------------------------------------------
    // Dividing data as per number of GPU
    //-------------------------------------------------------------------------

    for(unsigned int i = 0; i < count; ++i)
    {
        data[i].inData = inData + i * width * height/count;
        data[i].outData = outData + i * width * height/count;

        data[i].device = device + i;
    }

    //-------------------------------------------------------------------------
    // Creating new Threads for each GPU
    //-------------------------------------------------------------------------
    for(unsigned int i = 0; i < count; ++i)
    {
        threads[i].create(run, (void*)(data + i));
    }

    //-------------------------------------------------------------------------
    // Waiting for each Thread to finish
    //-------------------------------------------------------------------------

    for(unsigned int i = 0; i < count; ++i)
    {
        threads[i].join();
    }

    delete[] data;
    delete[] threads;

    // Printing output buffer
    fprintf(stdout, "Output buffer:\n");
    printBuffer(outData, width, 0, 0, 8, 8);

    //--------------------------------------------------------------------------
    // Checking whether the result is correct or not
    //--------------------------------------------------------------------------

    if(!verify(inData, outData))
    {   
        fprintf(stdout, "Failed\n");
    }
    else
    {        
        fprintf(stdout, "Passed\n");
    }

    //--------------------------------------------------------------------------
    // Cleaning up
    //--------------------------------------------------------------------------
    
    delete[] inData;
    delete[] outData;

    return 0;
}

