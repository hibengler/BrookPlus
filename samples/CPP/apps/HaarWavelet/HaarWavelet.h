#ifndef _HAARWAVELET_H_
#define _HAARWAVELET_H_

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
/*****************************************************************************
  \file HaarWavelet.h
  \brief Contains the class definition for HaarWavelet class
 ****************************************************************************/

#include "SampleBase.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class  HaarWavelet
//!
//! \brief  contains application properties for the sample HaarWavelet
//! 
////////////////////////////////////////////////////////////////////////////////

class HaarWavelet : public SampleBase
{
    public:

        HaarWavelet(char* name);
        void haarWavelet_cpu(float* input, int n, float sqrt2);
        void allocateBuffer();
        void fillInputBuffer();
        int run();
        void printTimeInfo();
        void verifyResults();
        void comparePerformance();
        ~HaarWavelet();

    private:

        float* _input;
        float* _output;

        unsigned int _width;
        unsigned int _height;
};

#endif // _HAARWAVELET_H_
