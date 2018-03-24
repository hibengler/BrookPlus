#ifndef _UTILDEFS_H_
#define _UTILDEFS_H_

/* ============================================================

Copyright (c) 2008 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the following 
conditions:
 
Redistributions must retain the above copyright notice and all terms of this 
license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERATION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
becomes unenforceable or illegal, such term shall be reformed to the minimum 
extent necessary in order for this license to remain in effect in accordance 
with its terms as modified by such reformation. This license shall be governed 
by and construed in accordance with the laws of the State of Texas without 
regard to rules on conflicts of law of any state or jurisdiction or the United 
Nations Convention on the International Sale of Goods. All disputes arising out 
of this license shall be subject to the jurisdiction of the federal and state 
courts in Austin, Texas, and all defenses are hereby waived concerning personal 
jurisdiction and venue of these courts.

============================================================ */

namespace Util
{   
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Creates a buffer of the given rank and dimensions
    //! 
    //! \param  rank        Rank of the buffer
    //! \param  dimensions  Dimensions of the buffer
    //!
    //! \return Returns the allocated buffer on success. Returns NULL on failure.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T> 
    T*
    allocateBuffer(const unsigned int rank, const unsigned int* dimensions)
    {
        // Checking whether the rank or dimensions are valid
        if(!dimensions || (rank == 0) || (rank > 3))
        {
            return NULL;
        }    

        // Calculating the dimensions of the buffer
        unsigned int width  = dimensions[0];
        unsigned int height = (rank > 1) ? dimensions[1]: 1;
        unsigned int depth  = (rank > 2) ? dimensions[2]: 1;

        return allocateBuffer<T>(width, height, depth);
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Creates a buffer of the given width, height and depth
    //! 
    //! \param  width   Width of the buffer
    //! \param  height  Height of the buffer
    //! \param  depth   Depth of the buffer
    //!
    //! \return Returns the allocated buffer on success. Returns NULL on failure.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    T*
    allocateBuffer(const unsigned int width, const unsigned int height = 1, const unsigned int depth = 1)
    {
        // Generating a new seed for random number generator
        srand ((unsigned int)time(NULL));

        // Allocating buffer
        T* buffer = new T[width * height * depth];

        // Initializing values if allocation successful
        if(buffer)
        {
            memset(buffer, 0, sizeof(T) * height * width * depth);
        }

        return buffer;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Creates a buffer of the given width, height and depth
    //! 
    //! \param  width   Width of the buffer
    //! \param  height  Height of the buffer
    //! \param  depth   Depth of the buffer
    //!
    //! \return Returns the allocated buffer on success. Returns NULL on failure.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    T*
    allocateAndInitializeBuffer(const unsigned int width, const unsigned int height = 1, 
                                const unsigned int depth = 1, const BufferValue type = RANDOM)
    {
        T* buffer = allocateBuffer<T>(width, height, depth);
        fillBuffer(buffer, width, height, depth, type);

        return buffer;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Returns a value based on the type
    //! 
    //! \param  bound   Maximum value that can be generated
    //! \param  type    Type of value to be generated i.e. random, linear etc
    //!
    //! \return Returns the generated value
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void 
    value(T& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type)
    {
        switch(type)
        {
            case RANDOM:
                var = (T)((rand() / ((float)RAND_MAX + 1)) * bound);
                break;
            case PREVRAND:
                var = (T)((rand() / ((float)RAND_MAX + 1)) * bound);
                break;
            case LINEAR:
                var = (T)((y * width + x) % bound);
                break;
            case RLINEAR:
                var = (T)((float)(((width * height) - (y * width + x)) % bound));
                break;
            case ILINEAR:
                var = (T)((float)((x * width + y) % bound));
                break;
            case POS:
                var = (T)((float)(y * width + x));
                break;
            case LINEAR_INT:
                var = (T)(ceil((float)(y * width + x)));
                break;
            case RANDOM_INT:
                var = (T)(ceil((rand() / ((float)RAND_MAX + 1)) * bound));
                break;
            case CONSTANT:
                var = (T)((float)(bound));
                break;
            case POSX:
                var = (T)x;
                break;
            case POSY:
                var = (T)((float)(y));
                break;
            default:
                var = (T)(0.0);
        }
    }

    template<>
    void 
    value(float2& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(float3& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(float4& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(int2& var, 
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(int3& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(int4& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(uint2& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(uint3& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(uint4& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);

    template<>
    void 
    value(double2& var,
            const unsigned int x, const unsigned int y, const unsigned int z,
            const unsigned int width, const unsigned int height, unsigned int depth,
            const unsigned int bound, const BufferValue type);


    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Fills the matrix with values of the specified type
    //!
    //! \param  buffer  Buffer to be filled
    //! \param  size    Size of the buffer
    //! \param  type    Type of values to be generated
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template <class T>
    void
    fillBuffer(T* buffer,
                const unsigned int width, unsigned height, unsigned int depth, 
                const unsigned int bound, const BufferValue type = RANDOM)
    {
        for(unsigned int i = 0; i < height; ++i)
        {
            for(unsigned int j = 0; j < width; ++j)
            {
                if (type == Util::PREVRAND)
                {
                    int index = i * width + j;
                    if (index)
                    {
                        value(buffer[index], j, i, 0, width, height, depth , bound, type);
                        buffer[index] = buffer[index - 1] + buffer[index];
                    }
                    else
                    {
                        value(buffer[i * width + j], j, i, 0, width, height, depth , bound, type);
                    }
                }
                else
                {
                    value(buffer[i * width + j], j, i, 0, width, height, depth , bound, type);
                }
                
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Compares two values for equality
    //!
    //! Template version works for int and uint. Specialized functions for
    //! int2, int3, int4, uint2, uint3, uint4, float, float2, float3, float4,
    //! double, double2 are defined below.
    //!
    //! \return Returns TRUE if values are equal. FALSE otherwise.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>    
    bool
    compare(const T val0, const T val1)
    {
        return (val0 == val1);
    }

    template <>
    bool
    compare(const float val0, const float val1);

    template <>
    bool
    compare(const float2 val0, const float2 val1);

    template <>
    bool
    compare(const float3 val0, const float3 val1);

    template <>
    bool
    compare(const float4 val0, const float4 val1);

    template <>
    bool
    compare(const double val0, const double val1);

    template <>
    bool
    compare(const double2 val0, const double2 val1);


    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Compares two single-precision single component buffers
    //!
    //! \param  mat1    First matrix
    //! \param  mat2    Second matrix
    //! \param  size    Size of the matrices
    //! 
    //! \return Returns TRUE if all elements are equal. FALSE otherwise.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    int
    compareBuffers(const T* mat0, const T* mat1, const unsigned int size)
    {
        int nequal = 0;
        for (unsigned int i = 0; i < size; ++i)
        {
            if ((compare(mat0[i], mat1[i])))
            {
                nequal++;
            }
        }
        return nequal;    
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Compares a subset of buffers
    //!
    //! \param  mat0    First matrix
    //! \param  width0  Width of the first matrix
    //! \param  height0 Height of the first matrix
    //! \param  x0, y0  Starting position for first matrix
    //! \param  mat1    Second matrix
    //! \param  width1  Width of the second matrix
    //! \param  height1 Height of the second matrix
    //! \param  x1, y1  Starting position for second matrix
    //! \param  w       Extents in x direction
    //! \param  h       Extents in y direction
    //!
    //! \return Returns TRUE if all elements are equal. FALSE otherwise.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    int
    compareBuffers(const T* mat0,
                    const unsigned int width0, const unsigned int height0,
                    const unsigned int x0, const unsigned int y0,
                    const T* mat1,
                    const unsigned int width1, const unsigned int height1,
                    const unsigned int x1, const unsigned int y1,
                    const unsigned int w, const unsigned int h)
    {
        // Denotes whether the matrices are equal or not
        int nequal = 0;

        // Check whether the extents are valid
        if(((x0 + w) > width0) ||
            ((x1 + w) > width1) ||
            ((y0 + h) > height0) ||
            ((y1 + h) > height1))
        {
            SHOWERROR("Comparison dimensions are greater than the width or height of the buffers");
            return false;
        }

        // Compare values
        for(unsigned int i = 0; i < h; ++i)
        {
            for(unsigned int j = 0; j < w; ++j)
            {
                unsigned int index0 = (y0 + i) * width0 + (x0 + j);
                unsigned int index1 = (y1 + i) * width0 + (x1 + j);

                if (!(compare(mat0[index0], mat1[index1])))
                {
                    nequal++;
                }
            }
        }

        return nequal;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Prints the value of the data on the screen
    //!
    //! \param  value   Value to be printed
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void
    print(const T value)
    {
        std::cout << value << "\t";
    }

    template<>
    void print(const float value);

    template<>
    void print(const float2 value);

    template<>
    void print(const float3 value);

    template<>
    void print(const float4 value);

    template<>
    void print(const double value);

    template<>
    void print(const double2 value);

    template<>
    void print(const int2 value);

    template<>
    void print(const int3 value);

    template<>
    void print(const int4 value);

    template<>
    void print(const uint2 value);

    template<>
    void print(const uint3 value);

    template<>
    void print(const uint4 value);

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Prints a subset of the buffer
    //!
    //! \param  title   Title of what is being printed
    //! \param  buffer  Buffer to be printed
    //! \param  w       Width of values to be printed
    //! \param  h       Height of values to be printed
    //! \param  x       Starting X
    //! \param  y       Starting Y
    //! \param  pitch   Pitch of the buffer
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template<class T>
    void
    printBuffer(const std::string title,
                const T* buffer,
                const unsigned int x, const unsigned int y,
                const unsigned int w, const unsigned int h,
                const unsigned int width, const unsigned int height)
    {        
        // Checking whether the values are valid
        if(((x + w) > width) || ((y + h) > height))
        {
            return;
        }

        std::cout << title << std::endl;

        // Iterating through the matrix
        for(unsigned int i = y; i < (y + h); ++i)
        {
            for(unsigned int j = x; j < (x + w); ++j)
            {
                unsigned int index = i * width + j;
                print(buffer[index]);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Checks a stream for a brook error
    //!
    //! \param  stream  Stream to be checked
    //!
    //! \return Returns FALSE if error occurs. TRUE otherwise.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template <class T>
    bool
    checkBrookError(::brook::Stream<T>& stream)
    {
        if(stream.error() != ::brook::BR_NO_ERROR)
        {
            std::cout << stream.errorLog() << std::endl;
            return false;
        }

        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Deletes an array
    //!
    //! \param  buffer  Buffer to be deleted
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template <class T>
    void
    deleteArray(T*& buffer)
    {
        if(buffer)
        {
            delete[] buffer;
            buffer = NULL;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \brief  Deletes an object
    //!
    //! \param  object  Object to be deleted
    //!
    ////////////////////////////////////////////////////////////////////////////////

    template <class T>
    void
    deleteObject(T*& object)
    {
        if(object)
        {
            delete object;
            object = NULL;
        }
    }
};

#endif //_UTILDEFS_H_
