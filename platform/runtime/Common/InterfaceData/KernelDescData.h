#ifndef _KERNELDESC_DATA_H_
#define _KERNELDESC_DATA_H_

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
//! \file KernelDescData.h
//!
//! \brief Contains kernelDesc class definition.
//!
////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "ExportDef.h"
#include "KernelDesc.h"


#ifdef _WIN32
//The warning disables visual studio's
//annoying habit of warning when using the standard set lib
#pragma warning (disable : 4251)
#endif

namespace brook
{
    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \struct ArgumentDesc
    //!
    //! \brief Structure to contain the usage of input parameter and 
    //! its associated parameter.
    //! Index shows for which stream argument we need the constsnt.
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    struct ArgumentDesc
    {
        inline ArgumentDesc(int index, int usage, int type = 0);

        //! \brief data showing argument index for different arguments.
        int Index;

        //! \brief Usage needed for different extra constant and struct support
        int Usage;

        //! \brief Type needed to differenciate between gather and normal streams
        // Brook semantics for stream resizing are different for gather and normal stream
        int Type;
    };

    inline
    ArgumentDesc::ArgumentDesc(int index, int usage, int type)
    {
        Index = index;
        Usage = usage;
        Type = type;
    };

    typedef std::vector<ArgumentDesc> ArgDesc;

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class PassData
    //!
    //! \brief Contain the metadata and backend specific Kernel program.
    //! 
    //! Metadata constains information for number of inputs, outputs, constants.
    //! 
    ////////////////////////////////////////////////////////////////////////////////

    class EXPORT PassData
    {
        public:

            explicit PassData(const char* name);
            void ref();
            void unref();
            ~PassData();

        public:

            //! ref count
            unsigned int _refCount;

            //! \brief Name of the pass. This information is used
            //! for avoiding creation of CALProgram again and again.
            const char* Name;
            
            //! \brief Generated device specific assembly
            const char* Image;

            //! \brief computeFlag desides whether shader is compute shader.
            bool computeFlag;

            //! \brief Metadata of all the constant arrays.
            ArgDesc* ConstArrays;

            //! \brief Metadata of all the constants.
            ArgDesc* Constants;

            //! \brief Metadata of all the input streams.
            ArgDesc* Inputs;

            //! \brief Metadata of all the output streams.
            ArgDesc* Outputs;

    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class TechniqueData
    //!
    //! \brief Contain different passes associated to a technique.
    //! Contains technique properties.
    //! The only property needed is whether technique is Address translated ot not.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    class EXPORT TechniqueData
    {
        public:

            TechniqueData();
            void ref();
            void unref();
            ~TechniqueData();

        public:

            //! ref count
            unsigned int _refCount;

            //! \brief Contains all the passes in a technique
            std::vector<Pass>* Passes;

            //! \brief Flag to indicate the case for addressTranslation.
            bool AddressTranslation;
    };

    ////////////////////////////////////////////////////////////////////////////////
    //!
    //! \class KernelDesc
    //!
    //! \brief Class that contains different techniques.
    //!
    ////////////////////////////////////////////////////////////////////////////////

    class EXPORT KernelDescData
    {
        public:

            KernelDescData();
            void ref();
            void unref();
            ~KernelDescData();

        public:

            //! ref count
            unsigned int _refCount;

            //! \brief Stores the technique information.
            std::vector<Technique>* Techniques;

    };

} //end brook namespace

#endif //_KERNELDESC_DATA_H_
