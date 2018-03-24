/****************************************************************************

Copyright (c) 2007, Advanced Micro Devices, Inc.
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
/*****************************************************************************
  \file MatMult.h
  \brief Contains header declaration and definition for class matrixMult
    
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include "Timer.h"
using namespace std;

#define MAX_SPLIT 128
/**
 * \MAX_SPLIT 128
 * \brief Determines the total number of sub matrice of C. Can be changed according to input size.
 */

#define SPLIT_FACTOR 4096 * 4096
/**
 * \SPLIT_FACTOR 4096 * 4096
 * \brief Determining condition for calculating sub matrices of input A and B.
 */

#define SPLIT_FACTOR_DOUBLE 2048 * 2048
/**
 * \SPLIT_FACTOR_DOUBLE 4096 * 4096
 * \brief Determining condition for calculating sub matrices of input A and B.
 * \ It has been kept half of SPLIT FACTOR because sizeof(double) = 8 byte.
 */

// Defaults used for the struct below. They are defined in Main.cpp.
extern unsigned int DEFAULT_WIDTH;
extern unsigned int DEFAULT_HEIGHT;
extern int          DEFAULT_VERIFY;
extern int          DEFAULT_QUIET;
extern int          DEFAULT_TIMING;
extern int	    DEFAULT_USEINTEGER;
extern int	    DEFAULT_USEFLOAT;
extern int          DEFAULT_USEDOUBLE;
extern int          DEFAULT_PERFORALL;

typedef struct infoRec
{
    /**
     * Flag to specifiy whether to use verbose output.
     * Activated with the -v command line option.
     */
    int Verbose;
    /**
     * Flag to specify whether to do CPU verification.
     * Activiated with -e command line option.
     */
    int Verify;
    /**
     * Value of the seed to use in the randomization.
     * Defaults to 0, modified with the -s command line option.
     */
    unsigned int Seed;
    /**
      Flag to specify whether to be quiet or not.
      Activated with the -q command line option.
      */
    int Quiet;
    /**
      Flag to specify whether to print timing results or not.
      Activated with the -t command line option.
      */
    int Timing;
    /**
     * Modified via the -y command line parameter.
     * Specifies the height of the input matrix
     */
    unsigned int Height;
    /**
     * Modified via the -x command line parameter.
     * Specifies the width of the input matrix
     */
    unsigned int Width;

    /**
     * Number of iterations to run the kernels
     * Specified with the -i command line option
     */
    unsigned int Iterations;

    /**
     * Flag to use integer data type
     * Specified with the -int command line option
     */
    int UseInteger;

    /**
     * Flag to use float data type
     * Specified with the -float command line option
     */
    int UseFloat;

    /**
     * Flag to use double data type
     * Specified with the -double command line option
     */
    int UseDouble;

    /**
     * Flag to print GPU performance for all inputs
     * Specified with the -all command line option
     */
    int PerfForAll; 

    /**
     * Flag to specify matrix dimension
     * Specified with the -dim command line option
     */
    int MatDimension;

} Info;


// Function prototype declaration for brook kernel handler of type float
void 
floatKernelHandler(float *finalOutput, float *inputA, 
                        float *bParts[MAX_SPLIT][4], unsigned int M, unsigned int K,
                        unsigned int N, unsigned int loopCount);

// Function prototype declaration for brook kernel handler of type Integer.
void 
intKernelHandler(int *finalOutput, int *inputA, 
                        int *bParts[MAX_SPLIT][4], unsigned int M, unsigned int K,
                        unsigned int N, unsigned int loopCount);

// Function prototype declaration for brook kernel handler of type Integer.
void 
doubleKernelHandler(double *finalOutput, double *inputA, 
                        double *bParts[MAX_SPLIT][4], unsigned int M, unsigned int K,
                        unsigned int N, unsigned int loopCount);

// Prototype declaration for naive matmult implementation for comparison with CPU for float data type.
void 
floatMatMultCPU(float* inputA, float* inputB, float* cpuOutput, int M, int N, int K);

// Prototype declaration for naive matmult implementation for comparison with CPU for integer data type.
void 
intMatMultCPU(int *inputA, int *inputB, int *cpuOutput, int M, int N, int K);

// Prototype declaration for naive matmult implementation for comparison with CPU for integer data type.
void 
doubleMatMultCPU(double *inputA, double *inputB, double *cpuOutput, int M, int N, int K);

void ParseCommandLine(int argc, char** argv, Info* info);

void Usage(char *name);




template <class T>
class matrixMult
{
private:
    T *_bParts[MAX_SPLIT][4];           //For 4 equal parts of B
    T *_inputA;                            //Input Matrix A
    T *_inputB;                            //Input Matrix B
    T *_cpuOutput;                        //Output Matrix for CPU Multiplication
    T *_outCoreOutput;                  //A biger output buffer to hold entire resultant matrix
    double _tolerance;                  //GPU correctness tolerance
    long unsigned int _splitOffsetB;    // For out code implementation
    long unsigned int _M;                //Input Dimension M
    long unsigned int _K;                //Input Dimension K
    long unsigned int _N;                //Input Dimension N

public:
    // Class Constructor with void arguments
    matrixMult(void);

    // Destructor for Class
    ~matrixMult(void);
    
    // Utility function to reset the input matrices dimension according to user input
    void setDimension( long unsigned int dimension );

    // Utility function to reset the input matrices dimension according to user input
    void setDimension(long unsigned int dimension1, long unsigned int dimension2, long unsigned int dimension3);
    
    // Funciton to allocate memory for input and output matrices
    void allocateMem(void);

    // Function for allocation of memory for _bParts
    void allocateMemForMatrixParts(unsigned int dimM, unsigned int dimK, unsigned int dimN);
    
    // Function for de-allocation of memory for_bParts
    void deAllocateMemForMatrixParts(unsigned int splitCount);
    
    // Function to check whether memory allocation is done successfully
    bool checkMemAlloc(void);
    
    // Populating input matrices with random vlaues less than 10
    void fillMatrices(void);
    
    // Function to split Matrics B into 4 equal parts
    void divideMatrixB(T* matrix, T* matrixParts[][4], unsigned int index,
                      unsigned int width, unsigned int height, unsigned int parts);
    
    // Call brook function for floating data type
    void floatBrookFunction(void);

    // Call brook function for int data type
    void intBrookFunction(void);

    // Call brook function for double data type
    void doubleBrookFunction(void);

    // Call brook function for float data type for cpu performance
    void floatCpuMatMultCall(void);

    // Call brook function for integer data type for cpu performance
    void intCpuMatMultCall(void);

    // Call brook function for integer data type for cpu performance
    void doubleCpuMatMultCall(void);

    // Function to calculate GPU performance
    void calculateGPUPerformance(double gpuElapsedTime);

    // Function to calculate CPU performance
    void calculateCPUPerformance( double cpuElapsedTime);

    // Function to check result for correctness.
    void checkResult(char *s);
};

////////////////////////////////////////////////////////////////////////////////////////
// \!brief      class Constructor with void arguments.
//
////////////////////////////////////////////////////////////////////////////////////////

template < class T>
matrixMult<T>::matrixMult()
{
// Initialization of all private class member variables
    _M = 256;
    _K = 256;
    _N = 256;
    _inputA = NULL;
    _inputB = NULL;
    _cpuOutput = NULL;
    _outCoreOutput = NULL;
    _splitOffsetB = 0;
    _tolerance = 1.0 / 10000000;
}

////////////////////////////////////////////////////////////////////////////////////////
// \!brief      class Destructor with void arguments.
//
////////////////////////////////////////////////////////////////////////////////////////

template <class T>
matrixMult<T>::~matrixMult()
{
    delete [] _inputA;
    delete [] _inputB;
    delete [] _cpuOutput;
    delete [] _outCoreOutput;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief              function to reset the input matrices dimension according to user input. 
//
// \!param dimension    the M for M * M * M matrix multiplication.
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::setDimension( long unsigned int dimension )
{
    _M = dimension;
    _K = dimension;
    _N = dimension;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief              function to reset the input matrices dimension according to user input.
//
// \!param dimension1    the M for M * K * N matrix multiplication.
// \!param dimension2    the K for M * K * N matrix multiplication.
// \!param dimension3    the N for M * K * N matrix multiplication.
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::setDimension(long unsigned int dimension1, long unsigned int dimension2,
                            long unsigned int dimension3) 
{
    _M = dimension1;
    _K = dimension2;
    _N = dimension3;
}

////////////////////////////////////////////////////////////////////////////////////////
// \!brief      memory allocation for input and output matrices.
//
////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::allocateMem()
{
    _inputA = new T[_M * _K];
    _inputB = new T[_K * _N];
    _outCoreOutput = new T[_M * _N];
    _cpuOutput = new T[_M * _N];
}

////////////////////////////////////////////////////////////////////////////////////////
// \!brief          allocation of memory for _bParts[].
//
// \!param dimM     height of sub matrix split of input matrix A.
// \!param dimK     heght of sub matrix split of input matrix B.
// \!param dimN     width of sub matrix split of input matrix B.
////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::allocateMemForMatrixParts(unsigned int dimM, unsigned int dimK, unsigned int dimN)
{
    unsigned int splitCount = _M / dimM;
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int parts;

    // For double, B matrix need to be splitted into only two parts.
    if(sizeof(T) == 4)
    {
        parts = 4;
    }
    else
    {
        parts = 2;
    }
    for(; i < splitCount; i++)
    {
        for(j = 0; j < parts; j++)
        {
            _bParts[i][j] = new T[dimK *(dimN / parts)];
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief              De-allocation of memory for _bParts. 
//
// \!param splitCount   the total number of sub matrices for input matrix A or B.
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::deAllocateMemForMatrixParts(unsigned int splitCount)
{
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int parts;
    if(sizeof(T) == 4)
    {
        parts = 4;
    }
    else
    {
        parts = 2;
    }
    for(; i < splitCount; i++)
    {
        for(j = 0; j < parts; j++)
        {
            delete [] _bParts[i][j];
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Function to check whether memory allocation is done successfully. 
//
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
bool
matrixMult<T>::checkMemAlloc()
{
    if(_inputA == NULL || _inputB == NULL)
    {        
        return false;
    }
    else
    {
        return true;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Fill input matrices with random vlaues less than 10. 
//
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::fillMatrices()
{
    long unsigned int i = 0;
    long unsigned int j = 0;
    while(i < ( _M * _K ))
    {
        _inputA[i] = (T)(rand() % 11)/(T)5;
        i++;
    }
    while(j < ( _K * _N ))
    {
        _inputB[j] = (T)(rand() % 11)/(T)7;
        j++;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief              Function to split Matrics B into 4 parts. 
//
// \!param matrix       input matrix B.
// \!param matrixParts  _bParts of input matrix B.
// \!param index        index of sub matrices of input matrix B.
// \!param width        width of a sub matrix of input matrix B.
// \!param height       height of a sub matrix of input matrix B.
// \!param parts        number of parts of _bParts[], typically it is 4.
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::divideMatrixB(T* matrix, T* matrixParts[][4], unsigned int index,
                                  unsigned int width, unsigned int height, unsigned int parts)
{
    long unsigned int i = 0;
    long unsigned int j = 0;
    long unsigned int k = 0;
    for(k = 0; k < height; k++)
    {
        i = k % parts;
        j = k * _N + _splitOffsetB;
        memcpy(&matrixParts[index][i][(k / parts) * width], &matrix[j], width * sizeof(T));
    }
    _splitOffsetB += width;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Call brook kernel handler for float data type. 
//
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::floatBrookFunction()
{
    if((_M * _K) > (SPLIT_FACTOR))
    {
        unsigned int constFactor = 0;
        unsigned int loopCount1 = 0;
        unsigned int loopCount2 = 0;
        unsigned int reductionFactor = 0;
        long unsigned int tempM = _M;
        long unsigned int tempN = _N;
        long unsigned int dimM = 0;
        long unsigned int dimN = 0;
        
        while(((tempM*_K) > (SPLIT_FACTOR))||((tempN * _K) > (SPLIT_FACTOR)))
        {
            tempM = tempM / 2;
            tempN = tempN / 2;
            constFactor++;
        }
        reductionFactor = (unsigned int)(pow((double)2,(double)constFactor));
        dimM = _M / reductionFactor;
        dimN = _N / reductionFactor;

        // Allocate Memory for _bParts[]
        allocateMemForMatrixParts(dimM, _K, dimN);
        for(unsigned int i = 0; i < reductionFactor; i++)
        {
            divideMatrixB(_inputB, _bParts, i, dimN, _K, 4);
        }

        // Call brook kernel for GPU computation
        floatKernelHandler(_outCoreOutput, _inputA, _bParts, dimM, _K, dimN, reductionFactor);

        // De-allocate memory for _bParts[]
        deAllocateMemForMatrixParts(reductionFactor);
    }
    else
    {
        // Allocate memory for _bParts[]
        allocateMemForMatrixParts(_M, _K, _N);

        // Divide _inputB matrix into 4 equal parts to _bParts[]
        divideMatrixB(_inputB, _bParts, 0, _N, _K, 4);

        // Call brook kernel for GPU computation
        floatKernelHandler(_outCoreOutput, _inputA, _bParts, _M, _K, _N, 1);

        // De-allocate memory for _bParts[]
        deAllocateMemForMatrixParts(1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Call brook kernel handler for integer data type. 
//
////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void
matrixMult<T>:: intBrookFunction()
{
    if((_M * _K) > (SPLIT_FACTOR))
    {
        unsigned int constFactor = 0;
        unsigned int loopCount1 = 0;
        unsigned int loopCount2 = 0;
        unsigned int reductionFactor = 0;
        long unsigned int tempM = _M;
        long unsigned int tempN = _N;
        long unsigned int dimM = 0;
        long unsigned int dimN = 0;
        
        while(((tempM*_K) > (SPLIT_FACTOR))||((tempN * _K) > (SPLIT_FACTOR)))
        {
            tempM = tempM / 2;
            tempN = tempN / 2;
            constFactor++;
        }
        reductionFactor = (unsigned int)(pow((double)2,(double)constFactor));
        dimM = _M / reductionFactor;
        dimN = _N / reductionFactor;

        // Allocate Memory for _bParts[]
        allocateMemForMatrixParts(dimM, _K, dimN);
        for(unsigned int i = 0; i < reductionFactor; i++)
        {
            divideMatrixB(_inputB, _bParts, i, dimN, _K, 4);
        }

        // Call brook kernel for GPU computation
        intKernelHandler(_outCoreOutput, _inputA, _bParts, dimM, _K, dimN, reductionFactor);

        // De-allocate memory for _bParts[]
        deAllocateMemForMatrixParts(reductionFactor);
    }
    else
    {
        // Allocate memory for _bParts[]
        allocateMemForMatrixParts(_M, _K, _N);
        
        // Divide _inputB matrix into 4 equal parts to _bParts[]
        divideMatrixB(_inputB, _bParts, 0, _N, _K, 4);
        
        // Call brook kernel for GPU computation
        intKernelHandler(_outCoreOutput, _inputA, _bParts, _M, _K, _N, 1);

        // De-allocate memory for _bParts[]
        deAllocateMemForMatrixParts(1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Call brook kernel handler for double data type. 
//
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::doubleBrookFunction()
{
    if((_M * _K) > (SPLIT_FACTOR_DOUBLE))
    {
        unsigned int constFactor = 0;
        unsigned int loopCount1 = 0;
        unsigned int loopCount2 = 0;
        unsigned int reductionFactor = 0;
        long unsigned int tempM = _M;
        long unsigned int tempN = _N;
        long unsigned int dimM = 0;
        long unsigned int dimN = 0;
        
        while(((tempM*_K) > (SPLIT_FACTOR_DOUBLE))||((tempN * _K) > (SPLIT_FACTOR_DOUBLE)))
        {
            tempM = tempM / 2;
            tempN = tempN / 2;
            constFactor++;
        }
        reductionFactor = (unsigned int)(pow((double)2,(double)constFactor));
        dimM = _M / reductionFactor;
        dimN = _N / reductionFactor;

        // Allocate Memory for _bParts[]
        allocateMemForMatrixParts(dimM, _K, dimN);
        for(unsigned int i = 0; i < reductionFactor; i++)
        {
            divideMatrixB(_inputB, _bParts, i, dimN, _K, 2);
        }

        // Call brook kernel for GPU computation
        doubleKernelHandler(_outCoreOutput, _inputA, _bParts, dimM, _K, dimN, reductionFactor);

        // De-allocate memory for _bParts[]
        deAllocateMemForMatrixParts(reductionFactor);
    }
    else
    {
        // Allocate memory for _bParts[]
        allocateMemForMatrixParts(_M, _K, _N);

        // Divide _inputB matrix into 4 equal parts to _bParts[]
        divideMatrixB(_inputB, _bParts, 0, _N, _K, 2);
                
        // Call brook kernel for GPU computation
        doubleKernelHandler(_outCoreOutput, _inputA, _bParts, _M, _K, _N, 1);

        // De-allocate memory for _bParts[]
        deAllocateMemForMatrixParts(1);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Call naive matrix multiplication implementation function for float. 
//
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void 
matrixMult<T>::floatCpuMatMultCall()
{
    floatMatMultCPU(_inputA, _inputB, _cpuOutput, _M, _N, _K);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Call naive matrix multiplication implementation function for integer. 
//
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void 
matrixMult<T>::intCpuMatMultCall()
{
    intMatMultCPU(_inputA, _inputB, _cpuOutput, _M, _N, _K);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Call naive matrix multiplication implementation function for double. 
//
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void 
matrixMult<T>::doubleCpuMatMultCall()
{
    doubleMatMultCPU(_inputA, _inputB, _cpuOutput, _M, _N, _K);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief                  Function to calculate GPU performance. 
//
// \!param gpuElapsedTime   the amount of time taken for GPU computation.
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::calculateGPUPerformance(double gpuElapsedTime)
{
    double gpuPerformance;
    double constantFactor = pow((double)10,(double)9);
    gpuPerformance = ((2 *(double)_M * _K * _N)/ gpuElapsedTime)/ constantFactor;
    cout<<"\nThe GPU Performance:  "<<gpuPerformance<<"GFlops"<<endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief                  Function to calculate CPU performance. 
//
// \!param cpuElapsedTime   the amount of time taken for CPU computation.
////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void
matrixMult<T>::calculateCPUPerformance( double cpuElapsedTime)
{
    double cpuPerformance;
    double constantFactor = pow((double)10,(double)9);
    cpuPerformance = ((2 * (double)_M * _K * _N) / cpuElapsedTime)/ constantFactor;
    cout<<"\nThe CPU Performance:  "<<cpuPerformance<<"GFlops"<<endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// \!brief      Function to check the correctness of CPU and GPU output. 
//
////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
void
matrixMult<T>:: checkResult(char *s)
{
    unsigned int j = 0;
    unsigned int flag = 0;
    while( j < _M * _N)
    {
        if(fabs(double(_cpuOutput[j] - _outCoreOutput[j]))<= _tolerance)
        {
            j++;
            continue;
        }
        else
        {
            flag++;
            if(flag == 7)
            {
                // Assuming tolerance can not be 1.0f.
                cout<<"\n"<<s<<": FAILED\n";
                break;
            }
            _tolerance *= 10.0;
            j = 0;
        }
    }
    if(flag < 7)
    {
        cout<<"\n\n\n"<<s<<": PASSED\n";
        cout<<"GPU Tolerance: "<<_tolerance<<endl;
    }
}
