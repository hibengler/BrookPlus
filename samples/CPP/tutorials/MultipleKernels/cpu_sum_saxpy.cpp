////////////////////////////////////////////////////////////////////////////////
//!
//! \file   cpu_sum_saxpy.cpp
//!
//! \brief  Defines the sum and saxpy kernels for CPU
//!
////////////////////////////////////////////////////////////////////////////////

#include "cpu_sum_saxpy.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Sums two buffers
//!
//! \param  i0      Input buffer 0
//! \param  i1      Input buffer 1
//! \param  out0    Output buffer
//!
////////////////////////////////////////////////////////////////////////////////

void 
sumCPU(const float* i0, const float* i1, float* out0, 
    const unsigned int width, const unsigned int height)
{
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            unsigned int index = i * width + j;
            out0[index] = i0[index] + i1[index];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Performs saxpy i.e. a*x + y on two streams
//!
//! \param  a       Scalar value
//! \param  x       Input buffer 0
//! \param  y       Input buffer 1
//! \param  out0    Output buffer
//!
////////////////////////////////////////////////////////////////////////////////

void 
saxpyCPU(const float a, const float* x, const float* y, float* out0,
      const unsigned int width, const unsigned int height)
{
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            unsigned int index = i * width + j;
            out0[index] = a * x[index] + y[index];
        }
    }
}
