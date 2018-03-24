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

#ifdef ENABLE_D3D9_INTEROP

#include "CALBase.h"
#include "CALD3D9Buffer.h"
#include "CALDevice.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
//! \param rank Rank of the Buffer
//! \param dimensions dimension os the Buffer
//! \param format Used CALformat for the buffer
//! \param bufferPool Location of the Buffer
//! \param device Device associated to CALBuffer
//!
////////////////////////////////////////////////////////////////////////////////

CALD3D9Buffer::CALD3D9Buffer(unsigned short rank, unsigned int* dimensions, 
                     CALformat format, BufferPool bufferPool, CALuint flag,
                     BaseDevice* device)
                     : CALBuffer(rank, dimensions, format, bufferPool, flag, device),
                     _calD3D9Associate(NULL), _calD3D9MapSurface(NULL),
                       _d3dDevice(NULL), _d3dSurface(NULL),
                       _shareHandle(0), _setShareHandle(false)
{
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief check if CAL supports the required extensions
//!
////////////////////////////////////////////////////////////////////////////////

bool
CALD3D9Buffer::initialize()
{
    // Is d3d9 extension supported
    CHECK_CAL_RESULT(calExtSupported(CAL_EXT_D3D9), "D3D9 Extension not supported\n");

    // Get function pointer for device and resource mapping
    CHECK_CAL_RESULT(calExtGetProc((CALextproc*)&_calD3D9Associate, CAL_EXT_D3D9, 
                     "calD3D9Associate"), "Failed to get calD3D9Associate handle\n");
    CHECK_CAL_RESULT(calExtGetProc((CALextproc*)&_calD3D9MapSurface, CAL_EXT_D3D9, 
                     "calD3D9MapSurface"), "Failed to get calD3D9MapSurface handle\n");

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Set property on buffer required for interoperable buffers
//!
////////////////////////////////////////////////////////////////////////////////

const char*
CALD3D9Buffer::setProperty(const char* name, void* value)
{
    if(!strcmp(name, "d3dDevice"))
    {
        if(!value)
        {
            return "NULL value passed\n";
        }

        _d3dDevice = static_cast<IDirect3DDevice9*>(value);
    }
    else if(!strcmp(name, "d3dResource"))
    {
        if(!value)
        {
            return "NULL value passed\n";
        }

        _d3dSurface = static_cast<IDirect3DSurface9*>(value);
    }
    else if(!strcmp(name, "shareHandle"))
    {
        _shareHandle = static_cast<HANDLE>(value);
        _setShareHandle = true;
    }
    else
    {
        return "This property is not supported\n";
    }

    if(!_initialized)
    {
        if(_d3dDevice && _d3dSurface && _setShareHandle)
        {
            CALDevice* device = static_cast<CALDevice*>(_device);
            CALdevice calDevice = device->getDevice();

            if(_calD3D9Associate(calDevice, _d3dDevice) != CAL_RESULT_OK)
            {
                return "Error while using d3d9 device\n";
            }

            if(_calD3D9MapSurface(&_res, calDevice, _d3dSurface, _shareHandle) != CAL_RESULT_OK)
            {
                return "Error while using d3d9 surface struct\n";
            }

            if(calCtxGetMem(&_mem, device->getContext(), _res) != CAL_RESULT_OK)
            {
                return "Error while using d3d9 surface struct\n";
            }

            _initialized = true;
        }
    }

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Decrease the ref count
//! Ask BufferMgr to destroy if refCount == 0 only if Buffer is local
//!
////////////////////////////////////////////////////////////////////////////////

void
CALD3D9Buffer::unref()
{
    assert(_refCount > 0);

    if (--_refCount == 0)
    {
        waitInputEvent();
        waitOutputEvent();
        waitWriteThread();
        waitWriteEvent();

        delete this;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//!
////////////////////////////////////////////////////////////////////////////////

CALD3D9Buffer::~CALD3D9Buffer()
{
}

#endif // ENABLE_D3D9_INTEROP
