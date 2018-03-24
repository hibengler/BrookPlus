#ifndef _KERNELDESC_H_
#define _KERNELDESC_H_

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


////////////////////////////////////////////////////////////////////////////////
//!
//! \file KernelDesc.h
//!
//! \brief Contains kernelDesc class definition.
//!
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "ExportDef.h"

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \enum ConstUsage
    //!
    //! \brief Enum showing usage of different constant arguments.
    //! The usage is needed for Address translation implementation.
    //! Only two extra constants are needed in new runtime 
    //! are streamshape and buffershape for AT.
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    enum ConstUsage
    {
        //! \brief User constants.
        CONST_USER_PARAM,

        //! \brief Stream dimensions.
        CONST_STREAMSHAPE,

        //! \brief Buffer dimensions.
        CONST_BUFFERSHAPE,

        //! \brief Reduction factor used in the kernel
        CONST_REDUCTIONFACTOR,

        //! \brief Pitch of buffer. Required for 2D-Scatter implementation
        CONST_BUFFERPITCH,

        //! \brief force 32-bit size enum
        CONST_FORCE_DWORD    = 0x7fffffff
    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \enum InputType
    //!
    //! \brief Enum showing different input types
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    enum AccessType
    {
        //! \brief random access arrays
        ACCESS_RANDOM = 0,

        //! \brief Normal stream
        ACCESS_STREAM,

        //! \brief force 32-bit size enum
        ACCESS_FORCE_DWORD    = 0x7fffffff
    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class Pass
    //!
    //! \brief Contain the metadata and backend specific Kernel program.
    //! 
    //! Metadata constains information for number of inputs, outputs, constants.
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    class PassData;

    class EXPORT Pass 
    {
        public:

            Pass(const Pass& p);
            Pass(const char* name);
            Pass& operator=(const Pass& p);
            Pass& image(const char* image);
            Pass& constArray(int argumentIndex, int usage);
            Pass& constant(int argumentIndex, ConstUsage usage);
            Pass& input(int argumentIndex, int usage, AccessType type = ACCESS_RANDOM);
            Pass& output(int argumentIndex, int usgae, AccessType type = ACCESS_STREAM);
            Pass& computeShader();

            ~Pass();
            PassData* getData() const;

        private:

            //! \brief data container
            PassData* _data;

    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class Technique
    //!
    //! \brief Contain different passes associated to a technique.
    //! Contains technique properties.
    //! The only property needed is whether technique is Address translated ot not.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    class TechniqueData;

    class EXPORT Technique
    {
        public:
            
            Technique(const Technique& t);
            Technique();
            Technique& operator=(const Technique& t);
            Technique& pass(Pass& p);
            Technique& addressTranslation();

            ~Technique();
            TechniqueData* getData() const;

        private:

            //! \brief data container
            TechniqueData* _data;
    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class KernelDesc
    //!
    //! \brief Class that contains different techniques.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    class KernelDescData;

    class EXPORT KernelDesc 
    {
        public:
            
            KernelDesc(const KernelDesc& desc);
            KernelDesc();
            KernelDesc& operator=(const KernelDesc& desc);
            KernelDesc& technique(Technique& t);

            ~KernelDesc();
            KernelDescData* getData() const;

        private:

            //! \brief data container
            KernelDescData* _data;

    };

} //end brook namespace

#endif //_KERNELDESC_H_
