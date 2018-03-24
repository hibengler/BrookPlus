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

#include "SparseMatrixVector.h"
#include "brookgenfiles/sparse_matrix_vector.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

SparseMatrixVector::SparseMatrixVector(char* name)
          :SampleBase(name)
{
    _csrCols = NULL;
    _csrNz = NULL;
    _csrRowStart = NULL; 
    _Anz = NULL;
    _cIdx = NULL;
    _x = NULL;
    _y = NULL;
    _matrixA = NULL;

    _size = 0;
    _length = 0;
    _nzWidth = 2;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
SparseMatrixVector::allocateBuffer()
{
    unsigned int Width = info->Width;
    unsigned int Height = info->Height;
    
    if (Width  < 2 || Height < 2)
    {
        if (!info->Quiet)
        {
            fprintf(stderr, "Error: Size must be larger than 2.\n");
        }
        else
        {
            std::cout << "Height and Width should be greater than 2 " << std::endl;
        }

        exit(-1);
    }

    _size = Height > Width ? Height : Width;
    info->Width = info->Height = _size;
    _nzWidth = 5;
    _length = _size * _nzWidth;

    _csrCols = Util::allocateBuffer<int>(_length, 1);
    _csrNz = Util::allocateBuffer<float>(_length, 1);
    _x = Util::allocateBuffer<float>(_size, 1);
    _y = Util::allocateBuffer<float>(_size, 1);
    _Anz = Util::allocateBuffer<float>(_length, 1);
    _cIdx = Util::allocateBuffer<float>(_length, 1);
    _csrRowStart = Util::allocateBuffer<int>(_size + 1, 1);
    _matrixA = Util::allocateBuffer<float>(_size, _size);
        
    if (!_x || !_y || !_Anz || !_cIdx || !_csrNz || !_csrRowStart || !_csrCols || !_matrixA)
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Create Compressed Sparse Row (CSR) represenation 
//!                        and store in csrNz, csrCols, csrRowStart.
//! \param matrixA            A square sparse matrix
//! \param csrNz            Non-zero elements of matrixA
//! \param csrCols            The columns array for the CSR format 
//! \param csrRowStart        Row index array for the CSR format
//! \param size                Size of the sparse matrix
//! \param length            Length of csrNz and csrCols
//! 
////////////////////////////////////////////////////////////////////////////////

void 
SparseMatrixVector::_createCSRFromSparseMatrix(float* matrixA, float* csrNz, int* csrCols, 
                                               int* csrRowStart, unsigned int size, unsigned int length)
{
    unsigned int i, j, k;
    int count;

    for (i=0; i < length; i++)
    {
        csrNz[i] = 0;
        csrCols[i] = 0;
    }

    csrRowStart[0] = 0;
    k = 0;
    for (i=0; i < size; i++) 
    {
        count = 0;
        for (j=0; j < size; j++) 
        {
            if (fabs(matrixA[i * size + j]) > 0.00001f ) 
            { 
                count++; 
                csrCols[k] = j;
                csrNz[k] = matrixA[i * size + j];
                k++;
                assert (k < length);
            }
        }
        csrRowStart[i+1] = csrRowStart[i] + count;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Convert the CSR format into the padded ITPACK
//!         representation that is easy to stream.
//! 
////////////////////////////////////////////////////////////////////////////////

void 
SparseMatrixVector::_reshuffleData(float* nz, int* cols, int* rowStart, float* Anz,
                                   float* Acols, unsigned int size, unsigned int nzWidth) 
{
    unsigned int i;
    int j;

    for (i = 0; i < size; i++) 
    {
        unsigned int offset = 0;
        for (j = rowStart[i]; j < rowStart[i + 1]; j++) 
        {
            Anz[nzWidth * i + offset] = nz[j];
            Acols[nzWidth * i + offset] = (float)cols[j];
            offset++;
        }

        // Must pad the rest of the row
        while (offset < nzWidth) 
        {
            Anz[nzWidth * i + offset] = 0.0f;

            // Below should be an invalid index.... but doesn't have to be since x multiplied by a zero here
            Acols[nzWidth * i + offset] = (float)0.0f;  
            offset++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
SparseMatrixVector::fillInputBuffer()
{
    unsigned int n;
    unsigned int k;
    
    Util::fillBuffer(_x, _size, 1, 1, 10, Util::RANDOM);
    Util::fillBuffer(_matrixA, _size, _size, 1, 1, Util::ZERO);
    
    // Create partially filled random matrix A 
    for (unsigned int i = 0; i < _size; i++) {
        while ((n = rand() % _nzWidth) == 0);
        for (k = 0; k < n; k++) {
            _matrixA[i * _size + (rand() % _size)] = (float) (rand() % 100);
        }
    }

    // Prepare internal data structures
    _createCSRFromSparseMatrix(_matrixA, _csrNz, _csrCols, _csrRowStart, _size, _length);

    // Convert from pure CSR to a padded scheme better suited for streaming
    _reshuffleData(_csrNz, _csrCols, _csrRowStart, _Anz, _cIdx, _size, _nzWidth);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
SparseMatrixVector::printTimeInfo()
{
    double time = timer->GetElapsedTime();

    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
                  << std::setw(16) << "Iterations" << std::setw(16) << "GPU Total Time" 
                  << std::endl;

    std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
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
SparseMatrixVector::verifyResults()
{
    int res = 0;
    float* vecY = Util::allocateBuffer<float>(_size, 1);

    if (vecY)
    {
        std::cout<<"-e Verify correct output.\n";
        std::cout<<"Performing Prefix Sum on CPU ... ";
        
        for (unsigned int i = 0; i < _size; ++i) 
        {
            vecY[i] = 0;
            for (unsigned int j = 0; j < _size; ++j) {
                vecY[i] += _x[j] * _matrixA[i * _size + j];
            }
        }
        
        std::cout << "Done\n";

        res += Util::compareBuffers<float>(_y, vecY, _size);
        
        if (res)
        {
            std::cout << _name << ": Failed!\n\n";
        }
        else
        {
            std::cout << _name << ": Passed!\n\n" ;
        }

        delete [] vecY;
    }
    else
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compare performance with CPU if requested
//! 
////////////////////////////////////////////////////////////////////////////////

void
SparseMatrixVector::comparePerformance()
{
    double cpuTime = 0.0;
    double gpuTime = 0.0;
    float* vecY = Util::allocateBuffer<float>(_size, 1);

    gpuTime = timer->GetElapsedTime();
    
    if(vecY)
    {
        std::cout << "-p Compare performance with CPU.\n";
        timer->Reset();
        timer->Start();

        for(unsigned int k = 0; k < info->Iterations; k++)
        {
            for (unsigned int i = 0; i < _size; ++i) 
            {
                vecY[i] = 0;
                for (unsigned int j = 0; j < _size; ++j) {
                    vecY[i] += _x[j] * _matrixA[i * _size + j];
                }
            }
        }
        
        timer->Stop();
        delete [] vecY;
    }
    else
    {
        fprintf(stderr, "Error: Memory Allocation.\n");
        exit(-1);
    }

    cpuTime = timer->GetElapsedTime();

    // Print CPU timing and speedup
    std::cout << std::setw(8) << "Width" << std::setw(8) << "Height"
              << std::setw(16) << "Iterations" << std::setw(16) << "CPU Total Time"
              << std::setw(16) << "GPU Total Time"
              << std::setw(16) << "Speedup" << std::endl;

    std::cout << std::setw(8) << info->Width << std::setw(8) << info->Height
              << std::setw(16) << info->Iterations << std::setw(16) << cpuTime
              << std::setw(16) << gpuTime
              << std::setw(16) << cpuTime / gpuTime << std::endl;
    
    std::cout << std::endl << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  backend implementation for simple matmult sample
//! 
////////////////////////////////////////////////////////////////////////////////

int
SparseMatrixVector::run()
{
    unsigned int retVal = 0;
    
    /////////////////////////////////////////////////////////////////////////
    // Brook code block
    /////////////////////////////////////////////////////////////////////////
    
    unsigned int dim1[] = {_length};
    unsigned int dim2[] = {_size};

    ::brook::Stream<float> AStrm(1, dim1);
    ::brook::Stream<float> AStrm2(1, dim1);
    ::brook::Stream<float> indices(1, dim1);
    ::brook::Stream<float> tmp_indices(1, dim1);
    ::brook::Stream<float> xStream(1, dim2);
    ::brook::Stream<float> yStream(1, dim2);

    // Record GPU Total time
    timer->Start();

    for (unsigned int i = 0; i < info->Iterations; ++i)
    {
        // Write to stream 
        AStrm.read(_Anz);
        indices.read(_cIdx);
        xStream.read(_x);
        yStream.read(_y);
        
        // Run Brook program
        gather(indices, xStream, tmp_indices);
        mult(AStrm, tmp_indices, AStrm2);
        sumRows(AStrm2, yStream);

        // Write data back from stream 
        yStream.write(_y);

        //Handle errors if any
        if(yStream.error())
        {
            std::cout << "Error occured" << std::endl;
            std::cout << yStream.errorLog() << std::endl;
            retVal = -1;
        }
    }

    timer->Stop();

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

SparseMatrixVector ::~SparseMatrixVector()
{
    delete [] _csrCols;
    delete [] _csrNz;
    delete [] _csrRowStart; 
    delete [] _Anz;
    delete [] _cIdx;
    delete [] _x;
    delete [] _y;
    delete [] _matrixA;
}
