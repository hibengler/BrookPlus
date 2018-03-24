////////////////////////////////////////////////////////////////////////////////
//!
//! \file   cpu_copy.cpp
//!
//! \brief  Defines the copy kernel for CPU
//!
////////////////////////////////////////////////////////////////////////////////

#include "cpu_copy.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Copies a part of input buffer to output buffer
//!
//! Given a start position(x0,y0) and an end position(x1,y1), data is copied
//! for these positions from input buffer to output buffer. Rest of the buffer
//! is left unaltered.
//! 
//!
//! \param  i0      Input buffer 0
//! \param  out0    Output buffer
//! \param  width   Buffer width
//! \param  height  Buffer height
//!
////////////////////////////////////////////////////////////////////////////////

void 
copyCPU(float* input, float* o,
        unsigned int startX, unsigned int startY,
        unsigned int endX, unsigned int endY,
        const unsigned int width)
{
    // Iterating through the output dimensions
    for(unsigned int i = startY; i < endY; i++)
    {
        for(unsigned int j = startX; j < endX; j++)
        {
            unsigned int index = i * width + j;
            o[index] = input[index];
        }
    }
}
