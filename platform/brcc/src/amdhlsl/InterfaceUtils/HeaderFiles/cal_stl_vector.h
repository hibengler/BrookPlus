/* ============================================================
Copyright (c) 2007 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the 
following conditions:
 
Redistributions must retain the above copyright notice and all terms of 
this license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE 
REVOCATION OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERATION, OR THAT IT IS 
FREE FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - 
WHETHER EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, 
ANY IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE, ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR 
NON-INFRINGEMENT. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY 
COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, PUNITIVE, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
REVENUE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED 
ON ANY THEORY OF LIABILITY ARISING IN ANY WAY RELATED TO THIS MATERIAL, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE 
LIABILITY OF ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT HOLDERS AND 
CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS (US $10.00). ANYONE REDISTRIBUTING 
OR ACCESSING OR USING THIS MATERIAL ACCEPTS THIS ALLOCATION OF RISK AND AGREES 
TO RELEASE ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT HOLDERS AND 
CONTRIBUTORS FROM ANY AND ALL LIABILITIES, OBLIGATIONS, CLAIMS, OR DEMANDS IN 
EXCESS OF TEN DOLLARS (US $10.00). THE FOREGOING ARE ESSENTIAL TERMS OF THIS 
LICENSE AND, IF ANY OF THESE TERMS ARE CONSTRUED AS UNENFORCEABLE, FAIL IN 
ESSENTIAL PURPOSE, OR BECOME VOID OR DETRIMENTAL TO ADVANCED MICRO DEVICES, 
INC. OR ANY COPYRIGHT HOLDERS OR CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS 
TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL SHALL TERMINATE IMMEDIATELY. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF THIS 
LICENSE OR ANY AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS 
SUCH AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS 
AS DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS,
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is provided 
with “RESTRICTED RIGHTS” and/or “LIMITED RIGHTS” as applicable to computer 
software and technical data, respectively. Use, duplication, distribution or 
disclosure by the U.S. Government and/or DOD agencies is subject to the full 
extent of restrictions in all applicable regulations, including those found at 
FAR52.227 and DFARS252.227 et seq. and any successor regulations thereof. Use 
of this material by the U.S. Government and/or DOD agencies is acknowledgment 
of the proprietary rights of any copyright holders and contributors, including 
those of Advanced Micro Devices, Inc., as well as the provisions of FAR52.227-14 
through 23 regarding privately developed and/or commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, 
title, or interest in, or relating to, this material. No terms of this license 
can be modified or waived, and no breach of this license can be excused, unless 
done so in a writing signed by all affected parties. Each term of this license 
is separately enforceable. If any term of this license is determined to be or 
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

#ifndef _STL_REPLACEMENT_VECTOR_H_
#define _STL_REPLACEMENT_VECTOR_H_

//
/// @file  cm_vector.h
/// @brief C++ dynamically allocated fixed size array
//

namespace calstd {

/// @brief Copy function
template<typename T>
static void copy(T *dst, const T *src, unsigned long count)
{
    for(unsigned long i = 0; i < count; i++)
        dst[i] = src[i];
}

/// Templatized vector class
//
template <typename T>
class vector
{
public:
#ifdef ATI_BITS_32
    typedef unsigned int  size_type;
#else
    typedef unsigned long size_type;
#endif

    typedef T* iterator;

    /// @brief Constructor
    vector() : _data(0), _size(0), _allocated(0) {}

    /// @brief Fill constructor
    vector(size_type size) : _data(0), _size(0), _allocated(0)
    {
        reserve(size);

        // initialize new items
        for(size_type i = 0; i < size; i++)
            _data[i] = T();

        _size = size;
    }

    ~vector() 
    {
        clear();
    }

    vector<T>& operator=(const vector<T>& rhs) 
    {
        clear();
        reserve(rhs.size());
        for(size_type i = 0; i < rhs.size(); i++)
        {
            push_back(rhs[i]);
        }

        return *this;
    }

    size_type size() const
    { 
        return _size;
    }

    void resize(size_type newSize)
    {
        if(newSize != _size)
        {
            size_type itemsToCopy;
            size_type oldSize = _size;
            T* tmp = new T [newSize];

            _allocated = _size = newSize;

            itemsToCopy = oldSize;
            if (newSize < oldSize)
                itemsToCopy = newSize;

            copy(tmp, _data, itemsToCopy);

            delete [ ] _data;

            _data = tmp;
        }
    }

    void reserve(size_type minSize)
    {
        if(minSize > _allocated)
        {
            T* tmp = new T [minSize];

            copy(tmp, _data, _size);

            delete [ ] _data;

            _allocated = minSize;
            _data = tmp;
        }
    }

    void clear()
    {
        // prevents double deletion
        if (_allocated != 0)
        {
            delete [ ] _data;
            _data = 0;
            _allocated = 0;
            _size = 0;
        }
        
    }

    bool empty() const
    {
        return (_size == 0);
    }

    void push_back(const T& value)
    {
        check_alloc();
        _data[_size++] = value;
    }

    void push_back(T& value)
    {
        check_alloc();
        _data[_size++] = value;
    }

    void pop_back()
    {
        if (_size > 0)
            _size--;
    }

    T& operator[](size_type index)
    {
        // osAssert((index < _size) || ((_size == 0) && (index == 0)));
        return _data[index];
    }

    const T& operator[](size_type index) const
    {
        // osAssert((index < _size) || ((_size == 0) && (index == 0)));
        return _data[index];
    }

    iterator begin() const
    {
        return &_data[0];
    }

    iterator end() const
    {
        return &_data[_size];
    }

    iterator erase(iterator element)
    {
        size_type offset = static_cast<size_type>(element - begin());

        // realloc new array and copy elements over

        if((_size > 0) && (offset >= 0))
        {
            if(_size > offset)
            {
                T* tmp = new T [_allocated];

                // copy items before element
                copy(tmp, _data, offset);

                //copy items after element
                copy(&tmp[offset], &_data[offset + 1], _size - 1 - offset);

                _size--; 

                delete [] _data;
                _data = tmp;

                return &_data[offset];
            }
        }
        return end();
    }

private:

    void check_alloc()
    {
        if ((_size + 1) > _allocated)
        {
            size_type newSize;

            if (_allocated < 16)
            {
                newSize = _allocated + 1;
            }
            else
            {
                static const size_type maxVectorIncrement = 512;
                if (_allocated < maxVectorIncrement)
                    newSize = _allocated * 2;
                else
                    newSize = _allocated + maxVectorIncrement;
            }

            reserve(newSize);
        }
    }

    T*        _data;
    size_type _size;
    size_type _allocated;
};

}; // namespace std

#endif // _CM_VECTOR_H_

