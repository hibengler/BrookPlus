////////////////////////////////////////////////////////////////////////////////
//!
//! \file   cpu_domain.h
//!
//! \brief  Declaration of function copyCPU
//!
////////////////////////////////////////////////////////////////////////////////

void 
copyCPU(float* i, float* o,
        unsigned int startX, unsigned int startY,
        unsigned int endX, unsigned int endY,
        const unsigned int width);

