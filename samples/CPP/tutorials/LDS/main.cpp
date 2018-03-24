// Project headers
#include "brookgenfiles/lds.h"
#include "brook/brook.h"
#include "math.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \Some preprocessor which will used in whole programme
//
////////////////////////////////////////////////////////////////////////////////


#define WIDTH 64
#define HEIGHT 64
#define COMPONENTS 4
#define ScalarType float
#define VectorType float4
#define GROUP_SIZE 64

////////////////////////////////////////////////////////////////////////////////
//!
//! \Function for calculating expected output
//
////////////////////////////////////////////////////////////////////////////////


void sum_2d_cpu(VectorType cc, ScalarType *a, ScalarType *b , ScalarType *c)
{
    int i = 0, j = 0, k = 0, m = 0;
    ScalarType constValue[4] = {0};
    
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \Assigning 0 to constant array
    //
    ////////////////////////////////////////////////////////////////////////////////

    constValue[0] = cc.x;
    constValue[1] = cc.y;
    constValue[2] = cc.z;
    constValue[3] = cc.w;

    for(i = 0; i < HEIGHT; ++i)
    {
        for(j = 0; j < WIDTH / GROUP_SIZE ; ++j)
        {
            for(k = 0; k < GROUP_SIZE; ++k)
            {
                unsigned int index = i * WIDTH + j * GROUP_SIZE + k;
                unsigned int reverseIndex = i * WIDTH + j * GROUP_SIZE + GROUP_SIZE - 1 - k;
                for(m = 0; m < COMPONENTS; ++m)
                {
                    ////////////////////////////////////////////////////////////////////////////////
                    //!
                    //! \Giving last thread value to first
                    //! \Second last value to second..........so on
                    //
                    ////////////////////////////////////////////////////////////////////////////////

                    c[index * COMPONENTS + m] = constValue[m] + a[reverseIndex * COMPONENTS + m] + b[reverseIndex * COMPONENTS + m];
                }
            }
        }
    }
}
int main()
{
    
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \Assigning preprocessors value to local variables
    //
    ////////////////////////////////////////////////////////////////////////////////

    unsigned int width = WIDTH;
    unsigned int height = HEIGHT;
    
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \Declaring & intializing some more temporary variable
    //
    ////////////////////////////////////////////////////////////////////////////////

    unsigned int i = 0, j = 0, k = 0, mismatched = 0;
    unsigned int streamSize[] = {WIDTH, HEIGHT};
    
    // Stream allocation
    brook::Stream<VectorType> streami0(2, streamSize);
    brook::Stream<VectorType> streami1(2, streamSize);
    brook::Stream<VectorType> streamo0(2, streamSize);
    
    // Declaration of input & output buffer
    ScalarType *i0 = NULL;
    ScalarType *i1 = NULL;
    ScalarType *o0 = NULL;
    ScalarType *expectedo0 = NULL;
    VectorType cc = VectorType(0, 0, 0, 0);

    // Memory allocation to input  output & expected buffer
    i0 = (ScalarType*)malloc(height * width * sizeof(ScalarType) * COMPONENTS);
    i1 = (ScalarType*)malloc(height * width * sizeof(ScalarType) * COMPONENTS);
    o0 = (ScalarType*)malloc(height * width * sizeof(ScalarType) * COMPONENTS);
    expectedo0 = (ScalarType*)malloc(height * width * sizeof(ScalarType) * COMPONENTS);

    // Value assign to input buffer 
    for(i = 0; i < height; ++i)
    {
        for(j = 0; j < width; ++j)
        {
            unsigned int index = i * width + j;
            for(k = 0; k < COMPONENTS; ++k)
            {
                i0[index * COMPONENTS + k] = (ScalarType)i;
                i1[index * COMPONENTS + k] = (ScalarType)j;
            }
        }
    }

    // Copying data from input buffer to input stream
    streamRead(streami0, i0);
    streamRead(streami1, i1);
    
    // calling kernel
    sum_2d(cc, streami0, streami1, streamo0);
    if(streamo0.error())
    {
        printf("Error : %s", streamo0.errorLog());
    }
    streamWrite(streamo0, o0);

    // Function calculating expected output 
    sum_2d_cpu(cc, i0, i1, expectedo0);
    
    // Verifying results Comparing output buffer with expected output
    for(i = 0; i < height; ++i)
    {
        for(j = 0; j < width; ++j)
        {
            unsigned int index = i * width + j;
            for(k = 0; k < COMPONENTS; ++k)
            {
                if(fabs(o0[index * COMPONENTS + k] - expectedo0[index * COMPONENTS + k]) > 0.000001f)
                {
                    mismatched = 1;
                    break;
                } 
            }
            if(mismatched)
            {
                printf("Failed!!\n");
                j = width;
                i = height;
            }

        }
    }


    if(mismatched == 0)
    {
        printf("Passed!!\n");
    }

    // Cleaning up
    free(i0);
    free(i1);
    free(o0);
    free(expectedo0);
}
