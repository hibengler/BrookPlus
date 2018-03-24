////////////////////////////////////////////////////////////////////////////////
//!
//! \file   cpu_sum_saxpy.h
//!
//! \brief  Declaration of functions sum and saxpy
//!
////////////////////////////////////////////////////////////////////////////////

void sumCPU(const float* i0, const float* i1, float* out0, 
         const unsigned int width, const unsigned int height);

void saxpyCPU(const float a, const float* x, const float* y, float* out0,
           const unsigned int width, const unsigned int height);
