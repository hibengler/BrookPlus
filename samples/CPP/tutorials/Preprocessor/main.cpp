// Project headers
#include "brookgenfiles/preprocessor.h"
#include "brook/brook.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Simple multiplication function which 
//! \ helps to calculate expected output 
//
////////////////////////////////////////////////////////////////////////////////

int cpu_multiply(int a, int b)
{
	return a * b;
}

int main()
{
        // Temporary variable pass as a parameter to cpu_multiply 
    int temp = 0;

        // Declaration of input & output buffer
    int* i0 = NULL;
    int* o0 = NULL;

        // Declaration of expected output buffer
    int* expectedo0 = NULL;
    unsigned int streamSize[]={10, 10};

        // Stream allocation
    brook::Stream<int> streami0(2, streamSize);
    brook::Stream<int> streamo0(2, streamSize);

        // Some more temporary variables used in for loop & for verifying result
    int a = 2;
    int b = 5;
    int i = 0, j = 0;
    int mismatched = 0;

        // Memory allocation to input  output & expected buffer
    i0 = (int*)malloc(sizeof(int) * 10 * 10);
    o0 = (int*)malloc(sizeof(int) * 10 * 10);
    expectedo0 = (int*)malloc(sizeof(int) * 10 * 10);

        // Value assign to input buffer & expected output buffer
    for(i = 0; i < 10; ++i)
    {
        for(j = 0; j < 10; ++j)
        {
            i0[i * 10 + j]  = i + j;

        // Some preprocessor are also used
#ifdef ONE
    #ifdef TWO
            temp = a + a;
            expectedo0[i * 10 + j] = i0[i * 10 + j] + cpu_multiply(temp, b) ;
    #else
            temp = b + b;
            expectedo0[i * 10 + j] = i0[i * 10 + j] + cpu_multiply(temp, b) ;
    #endif
#else
    #undef TWO
    #ifdef TWO
            temp = a + a;
            expectedo0[i * 10 + j] = i0[i * 10 + j] + cpu_multiply(a, temp) ;
    #else
            temp = b + b;
            expectedo0[i * 10 + j] = i0[i * 10 + j] + cpu_multiply(a, temp) ;
    #endif
#endif
        }
    }

    // Copying data from input buffer to input stream
    streamRead(streami0, i0);

    // calling kernel
    sample1(a, b, streami0, streamo0);

    // Copying data from output stream to output buffer
    streamWrite(streamo0, o0);


    // Verifying results Comparing output buffer with expected output
    for(i = 0; i < 10; ++i)
    {
        for(j = 0; j < 10; ++j)
        {
            if(o0[i * 10 + j]  != expectedo0[i * 10 + j])
            {
                
                mismatched = 1;
                j = 10, i = 10;
            }
        }
    }

    // Printing of results
    if(mismatched)
    {
        printf("Failed!!\n");
    }
    else
    {
        printf("Passed!!\n");
    }

    // Cleaning up
    free(i0);
    free(o0);
    free(expectedo0);

    return 0;
}
