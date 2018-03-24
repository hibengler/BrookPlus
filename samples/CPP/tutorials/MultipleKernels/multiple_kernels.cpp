////////////////////////////////////////////////////////////////////////////////
//!
//! \file   multiple_kernels.cpp
//!
//! \brief  Create and execute multiple(two) kernels
//!
//! Two kernels are executed in this tutorial, namely:-
//!     # sum
//!     # saxpy
//!
////////////////////////////////////////////////////////////////////////////////

// Project headers
#include "brookgenfiles/gpu_sum_saxpy.h"
#include "cpu_sum_saxpy.h"

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
//! \brief  Fills the buffer with random values
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
            buffer[index] = (float)(rand() % 9);
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

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Execute multiple CPU kernels
//!
//! \param  a       Scalar value for saxpy
//! \param  buffer0 Buffer 0
//! \param  buffer1 Buffer 1
//! \param  buffer2 Buffer 2
//! \param  width   Buffer width
//! \param  height  Buffer height
//!
////////////////////////////////////////////////////////////////////////////////

float*
executeMultipleKernelsCPU(float a,
                          float* buffer0,
                          float* buffer1,
                          float* buffer2,
                          const unsigned int width, const unsigned int height)
{
    // Creating output buffer for sum kernel
    float* outputSumCPU = new float[width * height];    

    // Executing sum kernel
    sumCPU(buffer0, buffer1, outputSumCPU, width, height);

    // Creating output buffer for saxpy kernel
    float* outputSaxpyCPU = new float[width * height];

    // Executing the saxpy kernel
    saxpyCPU(a, buffer2, outputSumCPU, outputSaxpyCPU, width, height);
    
    delete[] outputSumCPU;

    return outputSaxpyCPU;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Execute multiple GPU kernels
//!
//! \param  a       Scalar value for saxpy
//! \param  buffer0 Buffer 0
//! \param  buffer1 Buffer 1
//! \param  buffer2 Buffer 2
//! \param  width   Buffer width
//! \param  height  Buffer height
//!
////////////////////////////////////////////////////////////////////////////////

float*
executeMultipleKernelsGPU(float a,
                          float* buffer0,
                          float* buffer1,
                          float* buffer2,
                          const unsigned int width, const unsigned int height)
{
    //--------------------------------------------------------------------------
    // Creating and initializing the input streams.
    // Streams 1 and 2 are summed and stored in an output stream.
    // The output stream and stream 3 are passed to saxpy and result stored
    // in another output stream.
    //--------------------------------------------------------------------------

    // Initializing dimensions and rank
    unsigned int streamSize[] = {width, height};
    unsigned int rank = 2;

    // Creating input streams
    brook::Stream<float> input0(rank, streamSize);
    input0.read(buffer0);

    brook::Stream<float> input1(rank, streamSize);
    input1.read(buffer1);

    brook::Stream<float> input2(rank, streamSize);
    input2.read(buffer2);

    //--------------------------------------------------------------------------
    // Calling sum on input0 and input1 and then calling saxpy on output0 and
    // input2 and storing the result in output1.
    //--------------------------------------------------------------------------

    // Create a 2D stream of specified size
    brook::Stream<float> output0(rank, streamSize);
    brook::Stream<float> output1(rank, streamSize);
    
    // Executing kernel and copying back data
    sumGPU(input0, input1, output0);
    saxpyGPU(a, input2, output0, output1);

    // Create an output CPU buffer and copy output stream values
    // to output buffer
    float* outputGPU = new float[width * height];
    output1.write(outputGPU);

    // Check error on stream
    if(output1.error())
    {
        // Print error Log associated to stream
        fprintf(stdout, "%s\n", output1.errorLog());
    }

    return outputGPU;
}

int
main(int argc, char* argv[])
{
    // Specifying the width and height of the 2D buffer
    const unsigned int width = 64;
    const unsigned int height = 64;

    // Specifying the value of 'a' for saxpy
    const float a = 2;

    //--------------------------------------------------------------------------
    // Creating and initializing the input buffers using random values.
    // Buffers 1 and 2 are summed and stored in an output stream.
    // The output stream and buffer 3 are passed to saxpy and result stored
    // in another output stream.
    //--------------------------------------------------------------------------

    float* inputBuffer[3] = {0};

    // Creating buffer 0
    inputBuffer[0] = new float[width * height];
    fillBuffer(inputBuffer[0], width, height);

    // Creating buffer 1
    inputBuffer[1] = new float[width * height];
    fillBuffer(inputBuffer[1], width, height);

    // Creating buffer 2
    inputBuffer[2] = new float[width * height];
    fillBuffer(inputBuffer[2], width, height);

    // Executing both on the CPU and the GPU and comparing results
    float* outputCPU = executeMultipleKernelsCPU(a, inputBuffer[0], inputBuffer[1], inputBuffer[2], width, height);
    fprintf(stdout, "CPU Output Buffer:\n");
    printBuffer(outputCPU, width, 0, 0, 8, 8);

    float* outputGPU = executeMultipleKernelsGPU(a, inputBuffer[0], inputBuffer[1], inputBuffer[2], width, height);
    fprintf(stdout, "GPU Output Buffer:\n");
    printBuffer(outputGPU, width, 0, 0, 8, 8);
    
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
    delete[] inputBuffer[2];
    delete[] inputBuffer[1];
    delete[] inputBuffer[0];
    
    return 0;
}
