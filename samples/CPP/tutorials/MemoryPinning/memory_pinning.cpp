
// Project headers
#include "brookgenfiles/copy.h"
#include "brook/brook.h"
#include<malloc.h>
#include<math.h>

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fills the buffer with some values
//!
//! \param  in  Buffer to be filled
//! \param  dimensions   No.of dimensions
//!
////////////////////////////////////////////////////////////////////////////////
void 
fillBuffer(float *in, float val, unsigned int dimension)
{
    unsigned int i = 0; 
    for(i = 0; i < dimension; i++)
    {
        in[i] = val;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compares two buffers for equality
//!
//! \param  output    Output buffer
//! \param  Expected     Expected Result
//! \param  dimensions   No.of dimensions
//!
////////////////////////////////////////////////////////////////////////////////
int
verify(float* output, float* expected, unsigned int dimension)
{
    unsigned int i = 0;
    for(i = 0; i < dimension; i++)
    {
        if(fabs(output[i] - expected[i]) < 1e-4f)
        {
            continue;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

int
main(void)
{
  float*  input0 = NULL;
  float*  input1 = NULL;
  float*  out0 = NULL;
  float*  out1 = NULL;
  
  unsigned int i = 0, width = 1024, height = 1024;

////////////////////////////////////////////////////////////////////////////////
//! Aligned Memory Allocated
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
    input0 = (float*)_aligned_malloc(width * height * sizeof(float), 256);
    out0 = (float*)_aligned_malloc(width * height * sizeof(float), 256);
#else
    input0 = (float*)memalign(256, width * height * sizeof(float));
    out0 = (float*)memalign(256, width * height * sizeof(float));
#endif

      // Initializing the aligned input buffer
    fillBuffer(input0, 60, width * height);

  {
    unsigned int dim[] = { width, height };

////////////////////////////////////////////////////////////////////////////////
//! Creating the input Streams
////////////////////////////////////////////////////////////////////////////////
    ::brook::Stream< float > s1(2, dim);

////////////////////////////////////////////////////////////////////////////////
//! Creating the output Streams
////////////////////////////////////////////////////////////////////////////////
    ::brook::Stream< float > s2(2, dim);


    // Copying data from aligned input buffer to input stream
    s1.read(input0, "nocopy");

////////////////////////////////////////////////////////////////////////////////
//! Calling a kernel which will just copy the data of stream s1 to s2
////////////////////////////////////////////////////////////////////////////////
copy(s1, s2);

    // Copying data from input stream to aligned output buffer
    s2.write(out0, "nocopy");

    if (s1.error())
    {
      printf("\nError occured %s\n", s1.errorLog());
    }

    // Verifying input & output Buffer
    if(verify(out0, input0, width * height))
    {
        printf("\nMemory pinned read/write passed!\n"); 
    }
    else
    {
        printf("\nMemory pinned read/write failed!\n");
    }

  }

    //--------------------------------------------------------------------------
    // Cleaning up
    //--------------------------------------------------------------------------

#ifdef _WIN32
    _aligned_free(input0);
    _aligned_free(out0);
#else
    free(input0);
    free(out0);
#endif

  free(input1);
  free(out1);
  return 0;
}

