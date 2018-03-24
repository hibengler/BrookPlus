#ifndef _FORMATS_H_
#define _FORMATS_H_

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

#include "VectorDataType.h"

namespace brook
{

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \enum BRformat
    //!
    //! \brief Different Brook data types supported
    //!
    ////////////////////////////////////////////////////////////////////////////////

    enum BRformat
    {
        //! do not change order of existing ones
        BR_FORMAT_FLOAT_1,
        BR_FORMAT_FLOAT_2,
        BR_FORMAT_FLOAT_3,
        BR_FORMAT_FLOAT_4,
        BR_FORMAT_UINT_1,
        BR_FORMAT_UINT_2,
        BR_FORMAT_UINT_3,
        BR_FORMAT_UINT_4,
        BR_FORMAT_INT_1,
        BR_FORMAT_INT_2,
        BR_FORMAT_INT_3,
        BR_FORMAT_INT_4,
        BR_FORMAT_DOUBLE_1,
        BR_FORMAT_DOUBLE_2,
        BR_FORMAT_UCHAR_1,
        BR_FORMAT_UCHAR_2,
        BR_FORMAT_UCHAR_3,
        BR_FORMAT_UCHAR_4,
        BR_FORMAT_CHAR_1,
        BR_FORMAT_CHAR_2,
        BR_FORMAT_CHAR_3,
        BR_FORMAT_CHAR_4,

        BR_FORMAT_USHORT_1,
        BR_FORMAT_USHORT_2,
        BR_FORMAT_USHORT_3,
        BR_FORMAT_USHORT_4,
        BR_FORMAT_SHORT_1,
        BR_FORMAT_SHORT_2,
        BR_FORMAT_SHORT_3,
        BR_FORMAT_SHORT_4,
        BR_FORMAT_FORCE_DWORD    = 0x7fffffff, /* force 32-bit size enum */
    };

    template<class T>
    inline BRformat* getStreamType(T*, unsigned int& count);

    template<>
    inline BRformat* getStreamType(float*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_FLOAT_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(float2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_FLOAT_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(float3*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_FLOAT_3;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(float4*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_FLOAT_4;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(unsigned int*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UINT_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(uint2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UINT_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(uint3*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UINT_3;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(uint4*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UINT_4;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(int*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_INT_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(int2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_INT_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(int3*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_INT_3;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(int4*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_INT_4;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(double*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_DOUBLE_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(double2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_DOUBLE_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(uchar*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UCHAR_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(uchar2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UCHAR_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(uchar3*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UCHAR_3;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(uchar4*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_UCHAR_4;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(char*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_CHAR_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(char2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_CHAR_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(char3*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_CHAR_3;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(char4*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_CHAR_4;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(unsigned short*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_USHORT_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(ushort2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_USHORT_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(ushort3*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_USHORT_3;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(ushort4*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_USHORT_4;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(short*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_SHORT_1;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(short2*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_SHORT_2;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(short3*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_SHORT_3;

        return formats;
    }

    template<>
    inline BRformat* getStreamType(short4*, unsigned int& count)
    {
        count = 1;
        BRformat* formats = new BRformat[count];
        formats[0] = BR_FORMAT_SHORT_4;

        return formats;
    }
}
#endif //_FORMATS_H_
