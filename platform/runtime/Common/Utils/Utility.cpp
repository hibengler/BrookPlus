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

#include "Utility.h"
#include <assert.h>
#include <cmath>

namespace Utility
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Method to find the next-higher power of two
    //!
    ////////////////////////////////////////////////////////////////////////////////

    unsigned int
    ceilPow(unsigned int num)
    {
        --num;
        for(unsigned int i = 1; i < sizeof(unsigned int) * 8; i *= 2)
        {
            num |= num >> i;
        }

        return num + 1;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Method to find the next-lower power of two
    //!
    ////////////////////////////////////////////////////////////////////////////////

    unsigned int
    floorPow(unsigned int num)
    {
        for(unsigned int i = 1; i < sizeof(unsigned int) * 8; i *= 2)
        {
            num |= num >> i;
        }

        return num - (num >> 1);
    }


    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Method to find the lowest prime divisor of a number
    //!
    ////////////////////////////////////////////////////////////////////////////////

    unsigned int
    lowestPrimeDivisor(unsigned int num)
    {
        unsigned int sqrtNum = (unsigned int)(sqrt(double(num)));
        for(unsigned int i = 2; i <= sqrtNum; ++i)
        {
            if((num % i) == 0)
            {
                return i;
            }
        }

        return num;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief Returns bytes in one element
    //!
    ////////////////////////////////////////////////////////////////////////////////

    EXPORT unsigned short 
    getElementBytes(BRformat dataFormat)
    {
        unsigned short numComponents = 0;
        unsigned short bytes = 0;

        switch(dataFormat)
        {
            case BR_FORMAT_UCHAR_1:
            case BR_FORMAT_CHAR_1:
                numComponents = 1;
                bytes = sizeof(char);
                break;

            case BR_FORMAT_USHORT_1:
            case BR_FORMAT_SHORT_1:
                numComponents = 1;
                bytes = sizeof(short int);
                break;

            case BR_FORMAT_UCHAR_2:
            case BR_FORMAT_CHAR_2:
                numComponents = 2;
                bytes = sizeof(char);
                break;

            case BR_FORMAT_USHORT_2:
            case BR_FORMAT_SHORT_2:
                numComponents = 2;
                bytes = sizeof(short int);
                break;

            case BR_FORMAT_UCHAR_3:
            case BR_FORMAT_CHAR_3:
                numComponents = 3;
                bytes = sizeof(char);
                break;

            case BR_FORMAT_USHORT_3:
            case BR_FORMAT_SHORT_3:
                numComponents = 3;
                bytes = sizeof(short);
                break;

            case BR_FORMAT_UCHAR_4:
            case BR_FORMAT_CHAR_4:
                numComponents = 4;
                bytes = sizeof(char);
                break;

            case BR_FORMAT_USHORT_4:
            case BR_FORMAT_SHORT_4:
                numComponents = 4;
                bytes = sizeof(short int);
                break;

            case BR_FORMAT_FLOAT_1:
                numComponents = 1;
                bytes = sizeof(float);
                break;

            case BR_FORMAT_UINT_1:
            case BR_FORMAT_INT_1:
                numComponents = 1;
                bytes = sizeof(int);
                break;

            case BR_FORMAT_DOUBLE_1:
                numComponents = 1;
                bytes = sizeof(double);
                break;

            case BR_FORMAT_FLOAT_2:
                numComponents = 2;
                bytes = sizeof(float);
                break;

            case BR_FORMAT_UINT_2:
            case BR_FORMAT_INT_2:
                numComponents = 2;
                bytes = sizeof(int);
                break;

            case BR_FORMAT_DOUBLE_2:
                numComponents = 2;
                bytes = sizeof(double);
                break;

            case BR_FORMAT_FLOAT_3:
                numComponents = 3;
                bytes = sizeof(float);
                break;

            case BR_FORMAT_UINT_3:
            case BR_FORMAT_INT_3:
                numComponents = 3;
                bytes = sizeof(int);
                break;

            case BR_FORMAT_FLOAT_4:
                numComponents = 4;
                bytes = sizeof(float);
                break;

            case BR_FORMAT_UINT_4:
            case BR_FORMAT_INT_4:
                numComponents = 4;
                bytes = sizeof(int);
                break;

            default:
                assert(numComponents * bytes != 0);
                numComponents = 0;
                bytes = 0;
        }

        return numComponents * bytes;
    }

}
