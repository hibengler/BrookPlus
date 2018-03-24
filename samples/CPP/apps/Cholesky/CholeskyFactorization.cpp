/****************************************************************************

Copyright (c) 2008, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Advanced Micro Devices, Inc nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

****************************************************************************/

#include "CholeskyFactorization.h"
#include "brookgenfiles/cholesky_factorization.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

CholeskyFactorization::CholeskyFactorization(char* name) : SampleBase(name)
{
    _input_matrix = NULL;
    _L = NULL;
    _width = 0;
    _height = 0;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Adjusts the matrix dimension to suit the implementation.
//!                               Since only square matrices of dimension that are
//!                               multiples of 4 are allowed, we first select the larger
//!                               dimension input by the user and set the size to the next
//!                               higher multiple of 4.
//!
//! \param w             The width of the input matrix as input by user
//! \param h             The height of the input matrix as input by user
//! 
////////////////////////////////////////////////////////////////////////////////


unsigned int 
CholeskyFactorization::_adjustSampleSize(unsigned int w, unsigned int h)
{
    unsigned int final_dimension = w;

    // Make square by selecting the larger dimension
    if(w != h)
    {
        final_dimension = (w > h) ? w : h;
        std::cout << "Only square matrices allowed. Setting dimension = "
                  << final_dimension << "x" << final_dimension << std::endl;
    }

    // Make multiple of 4
    if(final_dimension % 4 != 0)
    {
        final_dimension = 4 * (final_dimension / 4 + 1);
        std::cout << "Dimension must be multiple of 4. Setting dimension = "
                  << final_dimension << "x" << final_dimension << std::endl;
    }

    std::cout << "Adjusted Matrix Dimensions: " << final_dimension << "x"
                << final_dimension << std::endl;

    return final_dimension;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
CholeskyFactorization::allocateBuffer()
{
    _width = info->Width;
    _height = info->Height;

    _width = _adjustSampleSize(_width, _height);
    info->Width = _width;
    info->Height = _height = _width;

    _input_matrix = Util::allocateBuffer<float>(_width, _width);
    _L = Util::allocateBuffer<float4>(_width / 4, _width);
        
    if (!_input_matrix || !_L)
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief                Generates a symmetric, positive-definite,
//!                       square matrix of desired dimension and
//!                       arranges the entries in a float4 array
//!                       for efficient computation on the GPU.
//!
//! \param w:             The width of the desired input matrix
//! \param h:             The height of the desired input matrix
//! \param I:             float array to hold generated input matrix
//! \param M:             float4 array for efficient computation
//! 
////////////////////////////////////////////////////////////////////////////////

int 
CholeskyFactorization::_generateInputMatrix(unsigned int w, unsigned int h, float* I, float4* M)
{
    unsigned int  index = 0, srcindex = 0, dstindex = 0;
    unsigned int  k = 0, j = 0, i = 0;
    float  value = 0.000000;

    // Check for null pointers
    if(! I || ! M)
    {
        printf("Null pointer!\n");
        return 0;
    }

    // Fill up the input matrix
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            if (j <= i)
            {
                value = 0.000000;
                for (k = 0; k <= j; k++)
                {
                    value += (k + 1) * (k + 1);
                }
            }

            index = i * w + j;
            I[index] = value;
        }
    }

    // Arrange it row wise in a float4 array
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j+=4)
        {
            // source
            srcindex = i * w + j;

            //destination
            dstindex = i * w/4 + j/4;   // row-wise
            M[dstindex].x = I[srcindex];
            M[dstindex].y = I[srcindex+1];
            M[dstindex].z = I[srcindex+2];
            M[dstindex].w = I[srcindex+3];
        }
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
CholeskyFactorization::fillInputBuffer()
{
    _generateInputMatrix(_width, _height, _input_matrix, _L);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Computes Cholesky Factorization of the input
//!                               matrix L on the CPU
//!
//! \param w The width of the input matrix
//! \param h The height of the input matrix
//! \param L The input matrix
//! 
////////////////////////////////////////////////////////////////////////////////

void 
CholeskyFactorization::_choleskyCPU(unsigned int width, unsigned int height, float *L)
{
    float dot_product,sqrt_Akk;
    unsigned int k = 0, i = 0, j = 0, p = 0;

    for(k = 0; k < width - 1; k ++) {

        // Square root
        sqrt_Akk = sqrt(L[k*width+k]);
        L[k*width+k] /= sqrt_Akk;

        // Normalization
        for(i = k+1; i < width; i ++)   {
            L[i*width+k] /= sqrt_Akk;
        }

        // Submatrix update
        for(j = k+1; j < width; j ++)   {
            float temp = L[j*width + k+1];
            dot_product = 0.0;

            for(p = 0; p <= k; p ++)
            {
                dot_product += (L[j*width + p] * L[(k+1)*width + p]);
            }

            temp -= dot_product;
            L[j*width + k + 1] = temp;
        }
    }

    L[(height-1)*width+height-1] /= sqrt(L[(height-1)*width+height-1]);

    // Triangularize
    for(i = 0; i < height; i ++)
    {
        for(j = 0; j < width; j++)
        {
            if( j > i )
            {
                L[i*width +j] = 0.0;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
CholeskyFactorization::printTimeInfo()
{
    double time = timer->GetElapsedTime();

    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::endl;

    std::cout << std::setw(8) << _width << std::setw(8) << _height
                  << std::setw(16) << info->Iterations << std::setw(16) << time
                  << std::endl;

    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
CholeskyFactorization::verifyResults()
{
    int res = 0;

    std::cout<<"-e Verify correct output.\n";
    std::cout<<"Performing Cholesky Factorization on CPU ... ";

    _choleskyCPU(_width, _height, _input_matrix);
    std::cout << "Done\n";

    res += Util::compareBuffers<float>(_input_matrix, (float*)_L, _height * _width);
    
    if (res)
    {
        std::cout << _name << ": Failed!\n\n";
    }
    else
    {
        std::cout << _name << ": Passed!\n\n" ;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compare performance with CPU if requested
//! 
////////////////////////////////////////////////////////////////////////////////
void
CholeskyFactorization::comparePerformance()
{
    unsigned int i;
    double cpuTime = 0.0;
    double gpuTime = 0.0;
    
    gpuTime = timer->GetElapsedTime();
    timer->Reset();
    timer->Start();
    
    std::cout << "-p Compare performance with CPU.\n";
    
    // Record CPU Total time
    for(i = 0; i < info->Iterations; i ++)
    {
        _choleskyCPU(_width, _height, _input_matrix);
    }
    
    timer->Stop();
    cpuTime = timer->GetElapsedTime();

    // Print CPU timing and speedup
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
              << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
              << std::setw(16) << "GPU Total Time"
              << std::setw(16) << "Speedup" << std::endl;

    std::cout << std::setw(8) << _width << std::setw(8) << _height
              << std::setw(16) << info->Iterations << std::setw(16) << cpuTime
              << std::setw(16) << gpuTime
              << std::setw(16) << cpuTime / gpuTime << std::endl;
    
    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for the sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
CholeskyFactorization::run()
{
    unsigned int retVal = 0;

    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    {
        unsigned int wx = _width / 4;
        unsigned int dim1[] = {_width, _height};
        unsigned int dim2[] = {wx, _height};
        unsigned int dim3[] = {1, 1};
        ::brook::Stream<float> input(2, dim1);
        
        ::brook::Stream<float4> o1(2, dim3);
        ::brook::Stream<float4> o2(2, dim3);
        ::brook::Stream<float4> o3(2, dim3);
        ::brook::Stream<float4> o4(2, dim3);
        
        ::brook::Stream<float4> Astream(2, dim2);
        ::brook::Stream<float4> Lstream(2, dim2);
        ::brook::Stream<float4> output(2, dim2);
        
        // Record GPU Total Time
        timer->Start();

        for(unsigned int itr = 0; itr < info->Iterations; itr++)
        {
            Astream.read(_L);
            Lstream.read(_L);

            for (unsigned int i = 0; i < wx; i++)
            {
                //Compute Cholesky factorization of 4x4 block L11 = cholesky(A11)
                unsigned int dimS1[] = {i, i * 4};
                unsigned int dimE1[] = {i + 1, (i + 1)* 4};
                unsigned int dimS2[] = {i, i * 4 + 0};
                unsigned int dimE2[] = {i + 1, i * 4 + 1};
                unsigned int dimS3[] = {i, i * 4 + 1};
                unsigned int dimE3[] = {i + 1, i * 4 + 2};
                unsigned int dimS4[] = {i, i * 4 + 2};
                unsigned int dimE4[] = {i + 1, i * 4 + 3};
                unsigned int dimS5[] = {i, i * 4 + 3};
                unsigned int dimE5[] = {i + 1, i * 4 + 4};
                
                unsigned int dimS6[] = {i, (i + 1)* 4 - 1};
                unsigned int dimE6[] = {i + 1, _height};
                unsigned int dimS7[] = {i, i * 4};
                unsigned int dimE7[] = {i + 1, (i + 1)* 4};
                unsigned int dimS8[] = {i, (i + 1)* 4 - 1};
                unsigned int dimE8[] = {i + 1, _height};

                unsigned int dimS9[] = {i + 1, (i + 1)* 4};
                unsigned int dimE9[] = {wx, _height};
                unsigned int dimS10[] = {i, (i + 1)* 4 - 1};
                unsigned int dimE10[] = {i + 1, _height};
                unsigned int dimS11[] = {i + 1, (i + 1)* 4};
                unsigned int dimE11[] = {wx, _height};

                unsigned int dimS12[] = {i + 1, (i + 1)* 4};
                unsigned int dimE12[] = {wx, _height};
                unsigned int dimS13[] = {i + 1, (i + 1)* 4};
                unsigned int dimE13[] = {wx, _height};


                cholesky4x4_gather(Astream.domain(dimS1, dimE1), o1, o2, o3, o4);
                stream_copy(o1, Lstream.domain(dimS2, dimE2));
                stream_copy(o2, Lstream.domain(dimS3, dimE3));
                stream_copy(o3, Lstream.domain(dimS4,dimE4));
                stream_copy(o4, Lstream.domain(dimS5,dimE5));


                //Block Normalize L21 = A21 * L11_inv_T
                normalize_gather( Astream.domain(dimS6, dimE6),
                                  Lstream.domain(dimS7, dimE7),
                                  Lstream.domain(dimS8, dimE8));


                //Update sub-matrix A22 = A22 - L21*L21_T
                matrix_update(Astream.domain(dimS9, dimE9),
                              Lstream.domain(dimS10, dimE10),
                              Lstream.domain(dimS11, dimE11));

                //Copy output stream to input stream for next pass
                stream_copy( Lstream.domain(dimS12, dimE12),
                             Astream.domain(dimS13, dimE13) );
            }

            //Lower triangularize to produce the final result
            lower_triangularize(Lstream, output);

            output.write(_L);

            //Handle errors if occured
            if(output.error())
            {
                std::cout << "Error occured" << std::endl;
                std::cout << output.errorLog() << std::endl;
                retVal = -1;
            }
        }

        timer->Stop();
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

CholeskyFactorization ::~CholeskyFactorization()
{
    delete [] _input_matrix;
    delete [] _L;
}
