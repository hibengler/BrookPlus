#ifndef _CAL_D3D10BUFFER_H_
#define _CAL_D3D10BUFFER_H_

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

#ifdef ENABLE_D3D10_INTEROP

////////////////////////////////////////////////////////////////////////////////
//!
//! \file CALD3D10Buffer.h
//!
//! \brief Contains the declaration of CALD3D10Buffer class
//!
////////////////////////////////////////////////////////////////////////////////

#include "CALBuffer.h"
#include "cal_ext.h"
#include "cal_ext_d3d10.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \class CALD3D10Buffer
//!
//! \brief Buffer created from D3D10 buffer
//! 
///////////////////////////////////////////////////////////////////////////////

class CALD3D10Buffer : public CALBuffer
{
    public:

        CALD3D10Buffer(unsigned short rank, unsigned int* dimansions,
                    CALformat format, BufferPool bufferPool, CALuint flag,
                    BaseDevice* device);

        bool initialize();
        const char* setProperty(const char* name, void* value);

        void unref();
        ~CALD3D10Buffer();

    private:

        //! \brief Extesion to associate device
        PFNCALD3D10ASSOCIATEFUNC _calD3D10Associate;

        //! \brief Extesion to map d3d surface
        PFNCALD3D10MAPRESOURCEFUNC _calD3D10MapResource;

        //! \brief Direct3D device to be used
        ID3D10Device* _d3dDevice;

        //! \brief Direct3D resource to map
        ID3D10Resource* _d3dSurface;

        //! \brief share handle got when creating resource
        HANDLE _shareHandle;

        //! \brief have we set share handle property
        bool _setShareHandle;
};

#endif // ENABLE_D3D10_INTEROP

#endif //_CAL_D3D10BUFFER_H_
