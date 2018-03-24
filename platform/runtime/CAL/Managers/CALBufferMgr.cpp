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

#include "CALBase.h"
#include "CALBufferMgr.h"
#include "CALBuffer.h"
#include "CALD3D9Buffer.h"
#include "CALD3D10Buffer.h"
#include "CALConstBuffer.h"
#include "CALDevice.h"
#include "StreamImpl.h"
#include "Utility.h"

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Shader to resize a stream with given resizeFactor
//!
////////////////////////////////////////////////////////////////////////////////

#ifdef _USE_HLSL_
    static const char resize_stream_shader[] =
    "Texture2D i0;\n"
    "SamplerState samp0;\n"
    "\n"
    "cbuffer cb0\n"
    "{\n"
    "	float4 resizeFactor;\n"
    "	int4 inStreamShape;\n"
    "	int4 inBufferShape;\n"
    "	int4 outStreamShape;\n"
    "	int4 outBufferShape;\n"
    "}\n"
    "\n"
    "struct output\n"
    "{\n"
    "	float4 o0;\n"
    "};\n"
    "\n"
    "float2 getInputPos(float2 pos)\n"
    "{\n"
    "	int2 intPos = int2(pos.xy);\n"
    "	int index = intPos.y * outBufferShape.x + intPos.x;\n"
    "	int4 outIndex = int4(0, 0, 0, 0);\n"
    "	int4 inIndex = int4(0, 0, 0, 0);\n"
    "	int height = index / outStreamShape.x;\n"
    "	outIndex.x = index - height * outStreamShape.x;\n"
    "	outIndex.z = height / outStreamShape.y;\n"
    "	outIndex.y = height - outIndex.z * outStreamShape.y;\n"
    "	inIndex = outIndex / resizeFactor;\n"
    "	index = inIndex.z * inStreamShape.x * inStreamShape.y \n"
    "			+ inIndex.y * inStreamShape.x + inIndex.x;\n"
    "	int posY = index / inBufferShape.x;\n"
    "	float2 inPos = float2(0.0f, 0.0f);\n"
    "	inPos.x = float(index - posY * inBufferShape.x) + 0.5f;\n"
    "	inPos.y = float(posY) + 0.5f;\n"
    "	return inPos;\n"
    "}\n"
    "\n"
    "output main(float4 vPos : SV_POSITION) : SV_TARGET\n"
    "{\n"
    "	output o;\n"
    "	float2 index = getInputPos(vPos.xy);\n"
    "	o.o0 = i0.Sample(samp0, index.xy);\n"
    "	return o;\n"
    "}\n";
#else
    static const char resize_stream_shader[] = 
    "il_ps_2_0\n"
    "dcl_input_position_interp(linear_noperspective) v0.xy__\n"
    "dcl_output_generic o0\n"
    "dcl_cb cb0[5]\n"
    "dcl_resource_id(0)_type(2d,unnorm)_fmtx(float)_fmty(float)_fmtz(float)_fmtw(float)\n"
    "ftoi r0.xy__, v0.xyxx\n"
    "imad r0.x___, r0.y, cb0[4].x, r0.x\n"
    "ixor r0._y__, r0.x, cb0[3].x\n"
    "; l0 = (-0.000000f -0.000000f -0.000000f -0.000000f)\n"
    "dcl_literal l0, 0x80000000, 0x80000000, 0x80000000, 0x80000000\n"
    "and r0._y__, r0.y, l0\n"
    "imax r0.__z_, r0.x, r0.x_neg(xyzw)\n"
    "imax r1.xy__, cb0[3].xyxx, cb0[3].xyxx_neg(xyzw)\n"
    "udiv r0.__z_, r0.z, r1.x\n"
    "inegate r0.___w, r0.z\n"
    "cmov_logical r0._y__, r0.y, r0.w, r0.z\n"
    "imax r0.__z_, r0.y, r0.y_neg(xyzw)\n"
    "udiv r0.__z_, r0.z, r1.y\n"
    "inegate r0.___w, r0.z\n"
    "ixor r1.x___, r0.y, cb0[3].y\n"
    "; l1 = (-0.000000f -0.000000f -0.000000f -0.000000f)\n"
    "dcl_literal l1, 0x80000000, 0x80000000, 0x80000000, 0x80000000\n"
    "and r1.x___, r1.x, l1\n"
    "cmov_logical r0.__z_, r1.x, r0.w, r0.z\n"
    "imul r0.___w, r0.z, cb0[3].y\n"
    "itof r1.__z_, r0.z\n"
    "iadd r0.__z_, r0.y, r0.w_neg(xyzw)\n"
    "imul r0._y__, r0.y, cb0[3].x\n"
    "itof r1._y__, r0.z\n"
    "iadd r0.x___, r0.x, r0.y_neg(xyzw)\n"
    "itof r1.x___, r0.x\n"
    "div_zeroop(infinity) r0.xyz_, r1.xyzx, cb0[0].xyzx\n"
    "ftoi r0.xyz_, r0.xyzx\n"
    "imul r0._yz_, r0.zzyz, cb0[1].x\n"
    "imad r0._y__, r0.y, cb0[1].y, r0.z\n"
    "iadd r0.x___, r0.y, r0.x\n"
    "ixor r0._y__, r0.x, cb0[2].x\n"
    "; l2 = (-0.000000f -0.000000f -0.000000f -0.000000f)\n"
    "dcl_literal l2, 0x80000000, 0x80000000, 0x80000000, 0x80000000\n"
    "and r0._y__, r0.y, l2\n"
    "imax r0.__z_, r0.x, r0.x_neg(xyzw)\n"
    "imax r0.___w, cb0[2].x, cb0[2].x_neg(xyzw)\n"
    "udiv r0.__z_, r0.z, r0.w\n"
    "inegate r0.___w, r0.z\n"
    "cmov_logical r0._y__, r0.y, r0.w, r0.z\n"
    "imul r0.__z_, r0.y, cb0[2].x\n"
    "itof r0._y__, r0.y\n"
    "iadd r0.x___, r0.x, r0.z_neg(xyzw)\n"
    "itof r0.x___, r0.x\n"
    "; l3 = (0.500000f 0.000000f 0.500000f 0.000000f)\n"
    "dcl_literal l3, 0x3F000000, 0x00000000, 0x3F000000, 0x00000000\n"
    "add r0.x_z_, r0.xxyx, l3\n"
    "sample_resource(0)_sampler(0) o0, r0.xzxx\n"
    "ret_dyn\n"
    "end\n";
#endif

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy shader to copy from a stream to another.
//!
////////////////////////////////////////////////////////////////////////////////
#ifdef _USE_HLSL_
    static const char domain_copy_shader[] =
        "cbuffer cb0\n"
        "{\n"
        "	float width;\n"
        "	float height;\n"
        "};\n"
        "\n"
        "struct output\n"
        "{\n"
        "	float4 o0;\n"
        "};\n"
        "\n"
        "SamplerState s;\n"
        "\n"
        "Texture2D i0;\n"
        "\n"
        "\n"
        "output \n"
        "main(float4 vPos: SV_POSITION) : SV_TARGET\n"
        "{\n"
        "	output o;\n"
        "	float2 index = vPos.xy + float2(width, height);\n"
        "	\n"
        "	o.o0 = i0.Sample(s, index.xy);\n"
        "	\n"
        "	return o;\n"
        "}\n"
        ;
#else
    static const char domain_copy_shader[] = 
        "il_ps_2_0\n"
        "dcl_input_position_interp(linear_noperspective) v0.xy__\n"
        "dcl_output_generic o0\n"
        "dcl_cb cb0[1]\n"
        "dcl_resource_id(0)_type(2d,unnorm)_fmtx(float)_fmty(float)_fmtz(float)_fmtw(float)\n"
        "add r0.xy__, v0.xyxx, cb0[0].xyxx\n"
        "sample_resource(0)_sampler(0) o0, r0.xyxx\n"
        "ret_dyn\n"
        "end\n"
        ;
#endif

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy shader to copy from a stream with AT to another stream
//! with AT or non AT.
//!
////////////////////////////////////////////////////////////////////////////////
#ifdef _USE_HLSL_
    static const char domainAT_copy_shader[] =
        "cbuffer cb0\n"
        "{\n"
        "	int4 gatherStream;\n"
        "	int4 gatherBuffer;\n"
        "	int4 srcStartPoint;\n"
        "	int4 dstStartPoint;\n"
        "	int4 dstEndPoint;\n"
        "	int4 outStream;\n"
        "	int4 outBuffer;\n"
        "};\n"
        "\n"
        "struct output {\n"
        "	float4 o0;\n"
        "};\n"
        "\n"
        "SamplerState s;\n"
        "\n"
        "Texture2D i0;\n"
        "\n"
        "int4 getOutputIndex(float4 pos, int4 outStream, int4 outBuffer)\n"
        "{\n"
        "	int4 outIndex = int4(0, 0, 0, 0);\n"
        "	int4 flag = int4(0, 0, 0, 0);\n"
        "	int sumOfFlag = 0;\n"
        "	int2 intPos = int2(pos.xy);\n"
        "	int index = intPos.y * outBuffer.x + intPos.x;\n"
        "	int height = index / outStream.x;\n"
        "	outIndex.x = index - height * outStream.x;\n"
        "	outIndex.z = height / outStream.y;\n"
        "	outIndex.y = height - outIndex.z * outStream.y;\n"
        "	\n"
        "	flag.x = (outIndex.x < dstStartPoint.x)||(outIndex.x > dstEndPoint.x); \n"
        "	flag.y = (outIndex.y < dstStartPoint.y)||(outIndex.y > dstEndPoint.y);\n"
        "	flag.z = (outIndex.z < dstStartPoint.z)||(outIndex.z > dstEndPoint.z);\n"
        "	sumOfFlag = flag.x + flag.y + flag.z;\n"
        "	\n"
        "	if(sumOfFlag != 0)\n"
        "	{\n"
        "		discard;\n"
        "	}\n"
        "	\n"
        "	return outIndex;\n"
        "}\n"
        "\n"
        "float4 getGatherPos(int4 gatherIndex, int4 gatherStream, int4 gatherBuffer)\n"
        "{\n"
        "	float4 gatherPos = float4(0.0f, 0.0f, 0.0f, 0.0f);\n"
        "	int index = gatherIndex.z * gatherStream.x * gatherStream.y + \n"
        "				  gatherIndex.y * gatherStream.x + gatherIndex.x;\n"
        "	int posY = index / gatherBuffer.x;\n"
        "	gatherPos.x = float(index - posY * gatherBuffer.x) + 0.5f;\n"
        "	gatherPos.y = float(posY) + 0.5f;\n"
        "\n"
        "	return gatherPos;\n"
        "}\n"
        "\n"
        "output\n"
        "main(float4 pos: SV_POSITION) : SV_TARGET\n"
        "{\n"
        "	output o;\n"
        "	\n"
        "	float2 inputIndex;\n"
        "\n"
        "	int4 outIndex = getOutputIndex(pos, outStream, outBuffer);\n"
        "	outIndex = outIndex - dstStartPoint + srcStartPoint;\n"
        "	\n"
        "	float4 gatherPos = getGatherPos(outIndex, gatherStream, gatherBuffer);\n"
        "	\n"
        "	inputIndex = gatherPos.xy;\n"
        "	\n"
        "	o.o0 = i0.Sample(s, inputIndex.xy);\n"
        "	\n"
        "	return o;\n"
        "}\n";
#else
    static const char domainAT_copy_shader[] = 
        "il_ps_2_0\n"
        "dcl_input_position_interp(linear_noperspective) v0.xy__\n"
        "dcl_output_generic o0\n"
        "dcl_cb cb0[7]\n"
        "dcl_resource_id(0)_type(2d,unnorm)_fmtx(float)_fmty(float)_fmtz(float)_fmtw(float)\n"
        "ftoi r0.xy__, v0.xyxx\n"
        "imad r0.x___, r0.y, cb0[6].x, r0.x\n"
        "ixor r0._y__, r0.x, cb0[5].x\n"
        "imax r0.__z_, r0.x, r0.x_neg(xyzw)\n"
        "imax r1.xy__, cb0[5].xyxx, cb0[5].xyxx_neg(xyzw)\n"
        "udiv r0.__z_, r0.z, r1.x\n"
        "inegate r0.___w, r0.z\n"
        "; l0 = (-0.000000f -0.000000f -0.000000f -0.000000f)\n"
        "dcl_literal l0, 0x80000000, 0x80000000, 0x80000000, 0x80000000\n"
        "and r0._y__, r0.y, l0\n"
        "cmov_logical r0._y__, r0.y, r0.w, r0.z\n"
        "imul r0.__z_, r0.y, cb0[5].x\n"
        "iadd r0.x___, r0.x, r0.z_neg(xyzw)\n"
        "ixor r1.x___, r0.y, cb0[5].y\n"
        "imax r1.__z_, r0.y, r0.y_neg(xyzw)\n"
        "udiv r1._y__, r1.z, r1.y\n"
        "inegate r1.__z_, r1.y\n"
        "; l1 = (-0.000000f -0.000000f -0.000000f -0.000000f)\n"
        "dcl_literal l1, 0x80000000, 0x80000000, 0x80000000, 0x80000000\n"
        "and r1.x___, r1.x, l1\n"
        "cmov_logical r0.___w, r1.x, r1.z, r1.y\n"
        "imul r1.x___, r0.w, cb0[5].y\n"
        "iadd r0.__z_, r0.y, r1.x_neg(xyzw)\n"
        "ilt r0._y__, r0.x, cb0[3].x\n"
        "ilt r1.x___, cb0[4].x, r0.x\n"
        "ior r0._y__, r0.y, r1.x\n"
        "; l2 = (0.000000f 0.000000f 0.000000f 0.000000f)\n"
        "dcl_literal l2, 0x00000001, 0x00000001, 0x00000001, 0x00000001\n"
        "and r0._y__, r0.y, l2\n"
        "ilt r1.x___, r0.z, cb0[3].y\n"
        "ilt r1._y__, cb0[4].y, r0.z\n"
        "ior r1.x___, r1.x, r1.y\n"
        "; l3 = (0.000000f 0.000000f 0.000000f 0.000000f)\n"
        "dcl_literal l3, 0x00000001, 0x00000001, 0x00000001, 0x00000001\n"
        "and r1.x___, r1.x, l3\n"
        "ilt r1._y__, r0.w, cb0[3].z\n"
        "ilt r1.__z_, cb0[4].z, r0.w\n"
        "ior r1._y__, r1.y, r1.z\n"
        "; l4 = (0.000000f 0.000000f 0.000000f 0.000000f)\n"
        "dcl_literal l4, 0x00000001, 0x00000001, 0x00000001, 0x00000001\n"
        "and r1._y__, r1.y, l4\n"
        "iadd r0._y__, r0.y, r1.x\n"
        "iadd r0._y__, r0.y, r1.y\n"
        "if_logicalnz r0.y\n"
        "    ; l5 = (-1.#QNAN0f -1.#QNAN0f -1.#QNAN0f -1.#QNAN0f)\n"
        "    dcl_literal l5, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF\n"
        "    discard_logicalnz l5\n"
        "endif\n"
        "iadd r0.xyz_, r0.xzwx, cb0[3].xyzx_neg(xyzw)\n"
        "iadd r0.xyz_, r0.xyzx, cb0[2].xyzx\n"
        "imul r0._yz_, r0.zzyz, cb0[0].x\n"
        "imad r0._y__, r0.y, cb0[0].y, r0.z\n"
        "iadd r0.x___, r0.y, r0.x\n"
        "ixor r0._y__, r0.x, cb0[1].x\n"
        "imax r0.__z_, r0.x, r0.x_neg(xyzw)\n"
        "imax r0.___w, cb0[1].x, cb0[1].x_neg(xyzw)\n"
        "udiv r0.__z_, r0.z, r0.w\n"
        "inegate r0.___w, r0.z\n"
        "; l6 = (-0.000000f -0.000000f -0.000000f -0.000000f)\n"
        "dcl_literal l6, 0x80000000, 0x80000000, 0x80000000, 0x80000000\n"
        "and r0._y__, r0.y, l6\n"
        "cmov_logical r0._y__, r0.y, r0.w, r0.z\n"
        "imul r0.__z_, r0.y, cb0[1].x\n"
        "iadd r0.x___, r0.x, r0.z_neg(xyzw)\n"
        "itof r0.x___, r0.x\n"
        "; l7 = (0.500000f 0.500000f 0.500000f 0.500000f)\n"
        "dcl_literal l7, 0x3F000000, 0x3F000000, 0x3F000000, 0x3F000000\n"
        "add r0.x___, r0.x, l7\n"
        "itof r0._y__, r0.y\n"
        "; l8 = (0.500000f 0.500000f 0.500000f 0.500000f)\n"
        "dcl_literal l8, 0x3F000000, 0x3F000000, 0x3F000000, 0x3F000000\n"
        "add r0.__z_, r0.y, l8\n"
        "sample_resource(0)_sampler(0) r0, r0.xzxx\n"
        "mov o0, r0\n"
        "ret_dyn\n"
        "end\n";
#endif

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Kernel descriptor for the copy shaders
//!
////////////////////////////////////////////////////////////////////////////////
static const brook::KernelDesc copy_shader_desc = brook::KernelDesc()
                                        .technique(brook::Technique()
                                            .pass(brook::Pass("__domain_copy_shader")
                                                .image( domain_copy_shader )
                                                .constant(1, brook::CONST_USER_PARAM)
                                            .input(0, 0)
                                                .output(2,0)
                                            )
                                        )
                                        .technique(brook::Technique()
                                            .pass(brook::Pass("__domainAT_copy_shader")
                                                .image(domainAT_copy_shader)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .input(0, 0)
                                                .output(2, 0)
                                            )
                                        )
                                        ;

const brook::KernelDesc* CALBufferMgr::_copy_shader_desc = &copy_shader_desc;


////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Kernel descriptor to resize stream
//!
////////////////////////////////////////////////////////////////////////////////
static const brook::KernelDesc resize_shader_desc = brook::KernelDesc()
                                        .technique(brook::Technique()
                                            .pass(brook::Pass("__resize_stream_shader")
                                                .image(resize_stream_shader)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .constant(1, brook::CONST_USER_PARAM)
                                                .input(0, 0)
                                                .output(2,0)
                                            )
                                        );

const brook::KernelDesc* CALBufferMgr::_resize_shader_desc = &resize_shader_desc;

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Constructor
//!
//! \param device The device associated to a bufferMgr
//!
////////////////////////////////////////////////////////////////////////////////

CALBufferMgr::CALBufferMgr(CALDevice* device) : BufferMgr(device)
{
    _calResCreate2D = NULL;
    _calResCreate1D = NULL;
    memset(_writeData, 0, sizeof(ThreadWriteData*) * MaxThreads);

    if(getenv("ENABLE_DMA"))
    {
        _enableDMA = true;
    }
    else
    {
        _enableDMA = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create backend spefic buffers using CALBuffer.
//!
//! \param stream The associated stream
//! \return Pointer to the newly created buffer
//!
////////////////////////////////////////////////////////////////////////////////

Buffer*
CALBufferMgr::_createBuffer(StreamImpl* stream, unsigned int i)
{
    unsigned short rank = stream->getBufferRank();
    unsigned int* dimensions = stream->getBufferDimensions();

    BRformat brFormat = stream->getDataFormat(i);
    CALformat calFormat = getCALFormat(brFormat);

    // Check if double precision is supported on the underlying hardware
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdeviceattribs attrib = device->getAttribs();
    CALdeviceinfo info = device->getInfo();

    if(!attrib.doublePrecision)
    {
        if(calFormat == CAL_FORMAT_DOUBLE_1 || calFormat == CAL_FORMAT_DOUBLE_2)
        {
            stream->setErrorCode(BR_ERROR_NOT_SUPPORTED);
            stream->setErrorString("Stream Allocation : Double precision not supported " 
                                   "on underlying hardware\n");

            return NULL;
        }
    }

    if(rank == 1 && dimensions[0] > info.maxResource1DWidth)
    {
        stream->setErrorCode(BR_ERROR_NOT_SUPPORTED);
        stream->setErrorString("Stream Allocation : This dimension not supported on undelying"
                               "hardware\n");

        return NULL;
    }
    else if(rank == 2 && (dimensions[0] > info.maxResource2DWidth || 
            dimensions[1] > info.maxResource2DHeight))
    {
        stream->setErrorCode(BR_ERROR_NOT_SUPPORTED);
        stream->setErrorString("Stream Allocation : This dimension not supported on undelying"
                               "hardware\n");

        return NULL;
    }
#ifdef ENABLE_COMPUTE_SHADER
    return new CALBuffer(rank, dimensions, calFormat, 
                         BUFFER_LOCAL, CAL_RESALLOC_GLOBAL_BUFFER, _device);
#else
    return new CALBuffer(rank, dimensions, calFormat, 
                         BUFFER_LOCAL, 0, _device);
#endif
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Do this backend support interoperability with type
//! CAL supports d3d9, d3d10
//!
////////////////////////////////////////////////////////////////////////////////

Buffer*
CALBufferMgr::createTypedBuffer(const char* type, StreamImpl* stream)
{
    unsigned short rank = stream->getBufferRank();
    unsigned int* dimensions = stream->getBufferDimensions();

    BRformat brFormat = stream->getDataFormat(0);
    CALformat calFormat = getCALFormat(brFormat);

#ifdef ENABLE_D3D9_INTEROP

    if(!strcmp("d3d9", type))
    {
        return new CALD3D9Buffer(rank, dimensions, calFormat, 
                                 BUFFER_LOCAL, 0, _device);
    }

#endif

#ifdef ENABLE_D3D10_INTEROP

    if(!strcmp("d3d10", type))
    {
        return new CALD3D10Buffer(rank, dimensions, calFormat, 
                                  BUFFER_LOCAL, 0, _device);
    }

#endif

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Returns function pointer for pinned resource creation
//!
////////////////////////////////////////////////////////////////////////////////

void*
CALBufferMgr::_getPinnedFunctionPointer(unsigned int rank)
{
    void *temp = NULL;

    switch(rank)
    {
        case 1:
            if(_calResCreate1D == NULL)
            {
                CALresult result;
                result = calExtGetProc((CALextproc*)&_calResCreate1D, (CALextid)CAL_EXT_RES_CREATE, "calResCreate1D");
                CHECK_CAL_RESULT(result, "Failed to find CAL extension for pinned 1D resource \n");
            }
            temp = (void*)_calResCreate1D;
            break;
        case 2:
            if(_calResCreate2D == NULL)
            {
                CALresult result;
                result = calExtGetProc((CALextproc*)&_calResCreate2D, (CALextid)CAL_EXT_RES_CREATE, "calResCreate2D");
                CHECK_CAL_RESULT(result, "Failed to find CAL extension for pinned 2D resource \n");
            }
            temp = (void*)_calResCreate2D;
            break;
    }
    return temp;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.read()
//!
//! Contains logic to get CALParity using tile by tile copy
//!
//! \param stream StreamImpl Class
//! \param ptr Application pointer to copy
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBufferMgr::setBufferData(StreamImpl* stream, const void* ptr, const char* flags)
{
    bool nocopy = false;
    if(flags)
    {
        std::string flagString(flags);
        size_t found = flagString.find("nocopy");
        if(found != std::string::npos)
        {
            nocopy = true;
        }
    }

    unsigned int bufferCount = stream->getBufferCount();

    if(!stream->flush())
    {
        stream->setErrorCode(BR_ERROR_READ);
        stream->setErrorString("Stream Read : Could not flush previous events\n");

        return;
    }

    for(unsigned int bufNum = 0; bufNum < bufferCount; ++bufNum)
    {
        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(bufNum));

        unsigned int rank = buffer->getRank();
        unsigned int* dimension = buffer->getDimensions();
        CALformat format = buffer->getFormat();

        // create a host resource to use DMA for data transfer
        CALBuffer* tmpBuffer = NULL;
        void* tmpPtr = NULL;

        if(nocopy && bufferCount == 1)
        {
            BRformat frmt = stream->getDataFormat(bufNum);
            unsigned int elementBytes = Utility::getElementBytes(frmt);
            if(elementBytes % 3 != 0 && dimension[0] % 64 == 0)
            {
                void* funcPtr = _getPinnedFunctionPointer(rank);
                tmpBuffer = _createPinnedBuffer(ptr, rank, dimension, format, funcPtr);
                tmpPtr = const_cast<void*>(ptr);
            }
        }

        unsigned int startPoint[MaxRank];
        memset(startPoint, 0, MaxRank * sizeof(unsigned int));

        if(!tmpBuffer)
        {
            tmpBuffer = _createHostBuffer(rank, dimension, format);
            unsigned int pitch = 0;
            tmpPtr = tmpBuffer->getBufferPointerCPU(pitch);

            if(tmpPtr)
            {
                stream->setData(tmpPtr, ptr, bufNum, pitch, startPoint, dimension);
            }

            tmpBuffer->freeBufferPointerCPU();
        }

        if(tmpBuffer != NULL && tmpPtr != NULL)
        {
            if(_enableDMA)
            {
                //DMA transfer
                CALevent* copyEvent = _getCopyEvent();
                if(!buffer->copyAsync(tmpBuffer, copyEvent))
                {
                    stream->setErrorCode(BR_ERROR_READ);
                    stream->setErrorString("Stream Read : Failed to Initiate DMA\n");

                    return;
                }

                //Set the associated event on local and host buffer
                buffer->setCopyEvent(copyEvent);
                tmpBuffer->setCopyEvent(copyEvent);
            }
            else // Path required by Cyberlink
            {
                BRformat format = stream->getDataFormat(bufNum);

                // We need to create a temporary stream to use copy method
                StreamImpl* tmpStream = new StreamImpl(rank, dimension, &format, 1, _device);
                tmpStream->setBuffer(tmpBuffer, 0);

                // invoke copy shader
                bool success = tmpStream->copy(tmpStream, startPoint, stream, startPoint, 
                                               dimension, false, 0, bufNum);

                delete tmpStream;
            }
        }
        else
        {
            //Start tile by tile copying
            unsigned int tiledDimension[MaxRank];

            unsigned int i = 0;
            for(; i < rank; ++i)
            {
                tiledDimension[i] = (dimension[i] < TileSize) ? dimension[i] : TileSize;
            }
            for(; i < MaxRank; ++i)
            {
                tiledDimension[i] = 1;
            }

            // Create a host resource of tile size
            tmpBuffer = _createHostBuffer(rank, tiledDimension, format);

            if(tmpBuffer == NULL)
            {
                stream->setErrorCode(BR_ERROR_READ);
                stream->setErrorString("Stream Read : Failed to create host resource \n");

                return;
            }

            unsigned int tiledStartPoint[MaxRank];
            memset(tiledStartPoint, 0, MaxRank * sizeof(unsigned int));

            BRformat format = stream->getDataFormat(bufNum);

            // We need to create a temporary stream to use copy method
            StreamImpl* tmpStream = new StreamImpl(rank, tiledDimension, &format, 1, _device);
            tmpStream->setBuffer(tmpBuffer, 0);

            unsigned int width = dimension[0];
            unsigned int height = (rank == 1) ? 1 : dimension[1];

            while(startPoint[1] < height)
            {
                startPoint[0] = 0;
                tiledDimension[0] = (width < TileSize) ? width : TileSize;

                while(startPoint[0] < width)
                {
                    // Set Host resource data with the given application pointer
                    unsigned int pitch = 0;
                    void* tmpPtr = tmpBuffer->getBufferPointerCPU(pitch);

                    if(!tmpPtr)
                    {
                        stream->setErrorCode(BR_ERROR_READ);
                        stream->setErrorString("Stream Read : Failed to map tiled resource\n");

                        delete tmpStream;
                        return;
                    }

                    stream->setData(tmpPtr, ptr, bufNum, pitch, startPoint, tiledDimension);
                    tmpBuffer->freeBufferPointerCPU();

                    // invoke copy shader
                    bool success = tmpStream->copy(tmpStream, tiledStartPoint, stream, startPoint, 
                                                   tiledDimension, false, 0, bufNum);

                    if(!success)
                    {
                        stream->setErrorCode(BR_ERROR_READ);
                        stream->setErrorString("Stream Read : Failed to copy data from tiled host resource to "
                                               " stream\n");

                        delete tmpStream;

                        return;
                    }

                    tmpBuffer->waitInputEvent();

                    // Move to next tile in horizontal direction
                    startPoint[0] = startPoint[0] + tiledDimension[0];
                    tiledDimension[0] = ((width - startPoint[0]) < TileSize) ? 
                                        (width - startPoint[0]) : TileSize;

                }

                // Move to next tile in vertical direction
                startPoint[1] = startPoint[1] + tiledDimension[1];
                tiledDimension[1] = ((height - startPoint[1]) < TileSize) ? 
                                     (height - startPoint[1]) : TileSize;
            }

            delete tmpStream;
        }
    }

    stream->copyDataToTree();
}

void*
CALBufferMgr::writeBufferData(void* args)
{
    WriteData* data = static_cast<WriteData*>(args);

    CALBuffer* srcBuffer = data->srcBuffer;
    CALBuffer* dstBuffer = data->dstBuffer;

    srcBuffer->setWriteSyncStatus(SYNC_THREAD_STARTED);
    dstBuffer->setWriteSyncStatus(SYNC_THREAD_STARTED);

    CALevent* copyEvent = data->copyEvent;
    StreamImpl* stream = data->stream;
    CALcontext context = data->context;
    void* ptr = data->ptr;

    if(!dstBuffer->copyAsync(srcBuffer, copyEvent))
    {
        stream->setErrorCode(BR_ERROR_WRITE);
        stream->setErrorString("Stream Write : Failed to Initiate DMA\n");

        srcBuffer->setWriteSyncStatus(SYNC_THREAD_END);
        dstBuffer->setWriteSyncStatus(SYNC_THREAD_END);

        return NULL;
    }

    //Wait for the event to finish
    while(calCtxIsEventDone(context, *copyEvent) == CAL_RESULT_PENDING)
        ; // Using Sleep(0) causes infinite loop sometimes
    *copyEvent = 0;

    // Get Host resource data in the given application pointer
    unsigned int pitch = 0;
    void* tmpPtr = dstBuffer->getBufferPointerCPU(pitch);

    if(!tmpPtr)
    {
        stream->setErrorCode(BR_ERROR_WRITE);
        stream->setErrorString("Stream Write : Failed to map resource\n");

        srcBuffer->setWriteSyncStatus(SYNC_THREAD_END);
        dstBuffer->setWriteSyncStatus(SYNC_THREAD_END);

        return NULL;
    }

    unsigned int startPoint[MaxRank];
    memset(startPoint, 0, MaxRank * sizeof(unsigned int));
    unsigned int* dimension = srcBuffer->getDimensions();

    stream->getData(tmpPtr, ptr, 0, pitch, startPoint, dimension);
    dstBuffer->freeBufferPointerCPU();

    srcBuffer->setWriteSyncStatus(SYNC_THREAD_END);
    dstBuffer->setWriteSyncStatus(SYNC_THREAD_END);

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Implementation of Stream.write()
//!
//! Contains logic to get CALParity using tile by tile copy
//!
//! \param stream StreamImpl Class
//! \param ptr Application pointer to copy
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBufferMgr::getBufferData(StreamImpl* stream, void* ptr, const char* flags)
{
    // has user asked for asynchronous behavior
    bool async = false;
    bool nocopy = false;

    if(flags)
    {
        std::string flagString(flags);
        size_t found;
        
        found = flagString.find("async");
        if(found != std::string::npos)
        {
            async = true;
        }

        found = flagString.find("nocopy");
        if(found != std::string::npos)
        {
            nocopy = true;
        }
    }

    unsigned int bufferCount = stream->getBufferCount();
    stream->copyDataFromTree();

    for(unsigned int bufNum = 0; bufNum < bufferCount; ++bufNum)
    {
        CALBuffer* buffer = static_cast<CALBuffer*>(stream->getBuffer(bufNum));

        if(buffer == NULL)
        {
            stream->setErrorCode(BR_ERROR_WRITE);
            stream->setErrorString("Stream Write : Uninitialized stream\n");
            return;
        }

        // Wait for all the events
        // No need to wait for input event as stream write is not going to modify buffer data
        buffer->waitCopyEvent();
        buffer->waitOutputEvent();

        unsigned int rank = buffer->getRank();
        unsigned int* dimension = buffer->getDimensions();
        CALformat format = buffer->getFormat();

        if(nocopy && bufferCount == 1)
        {
            BRformat frmt = stream->getDataFormat(bufNum);
            unsigned int elementBytes = Utility::getElementBytes(frmt);
            if(elementBytes % 3 != 0 && dimension[0] % 64 == 0)
            {
                void* funcPtr = _getPinnedFunctionPointer(rank);
                CALBuffer* tempBuffer = _createPinnedBuffer(ptr, rank, dimension, format, funcPtr);  
                if(tempBuffer != NULL)
                {
                    //DMA transfer
                    CALevent* writeEvent = _getCopyEvent();
                    if(!tempBuffer->copyAsync(buffer, writeEvent))
                    {
                        stream->setErrorCode(BR_ERROR_WRITE);
                        stream->setErrorString("Stream Write : Failed to Initiate DMA\n");

                        return;
                    }

                    //Set the associated event on local and host buffer
                    buffer->setWriteEvent(writeEvent);

                    if(!async)
                    {
                        buffer->waitWriteEvent();
                    }

                    return;
                }
            }
        }

        // create a host resource to use DMA for data transfer
        CALBuffer* tmpBuffer = _createPCIeHostBuffer(rank, dimension, format);

        unsigned int startPoint[MaxRank];
        memset(startPoint, 0, MaxRank * sizeof(unsigned int));

        if(tmpBuffer != NULL)
        {
            CALevent* copyEvent = _getCopyEvent();

            // We want to create a seperate thread only for async behavior
            // we don't want to create thread if it is a struct with multiple-buffers
            if(async && bufferCount == 1)
            {
                CALDevice* device = static_cast<CALDevice*>(_device);
                CALcontext context = device->getContext();
                ThreadWriteData* threadData = _getThreadWriteData();
                threadData->data->srcBuffer = buffer;
                threadData->data->dstBuffer = tmpBuffer;
                threadData->data->copyEvent = copyEvent;
                threadData->data->stream = stream;
                threadData->data->ptr = ptr;
                threadData->data->context = context;

                bool success = threadData->thread->create(CALBufferMgr::writeBufferData,
                                                          threadData->data);

                // Failed to create thread
                // fall back to single-threaded path
                if(success)
                {
                    buffer->setWriteThread(threadData->thread);
                    tmpBuffer->setWriteThread(threadData->thread);

                    return;
                }
            }

            if(_enableDMA)
            {
                if(!tmpBuffer->copyAsync(buffer, copyEvent))
                {
                    stream->setErrorCode(BR_ERROR_WRITE);
                    stream->setErrorString("Stream Write : Failed to Initiate DMA\n");

                    return;
                }

                //Wait for the event to finish
                buffer->setCopyEvent(copyEvent);
                buffer->waitCopyEvent();
            }
            else // Path required by Cyberlink
            {
                BRformat format = stream->getDataFormat(bufNum);

                // We need to create a temporary stream to use copy method
                StreamImpl* tmpStream = new StreamImpl(rank, dimension, &format, 1, _device);
                tmpStream->setBuffer(tmpBuffer, 0);

                // invoke copy shader
                bool success = tmpStream->copy(stream, startPoint, tmpStream, startPoint, 
                                               dimension, false, bufNum, 0);
                buffer->waitInputEvent();

                delete tmpStream;
            }

            // Get Host resource data in the given application pointer
            unsigned int pitch = 0;
            void* tmpPtr = tmpBuffer->getBufferPointerCPU(pitch);

            if(!tmpPtr)
            {
                stream->setErrorCode(BR_ERROR_WRITE);
                stream->setErrorString("Stream Write : Failed to map resource\n");

                return;
            }

            stream->getData(tmpPtr, ptr, bufNum, pitch, startPoint, dimension);
            tmpBuffer->freeBufferPointerCPU();
        }
        else
        {
            //Start tile by tile copying
            unsigned int tiledDimension[MaxRank];
            unsigned int i = 0;
            for(; i < rank; ++i)
            {
                tiledDimension[i] = (dimension[i] < TileSize) ? dimension[i] : TileSize;
            }
            for(; i < MaxRank; ++i)
            {
                tiledDimension[i] = 1;
            }

            // Create a host resource of tile size
            tmpBuffer = _createPCIeHostBuffer(rank, tiledDimension, format);
            if(tmpBuffer == NULL)
            {
                stream->setErrorCode(BR_ERROR_WRITE);
                stream->setErrorString("Stream Write : Failed to create host resource \n");

                return;
            }

            unsigned int tiledStartPoint[MaxRank];
            memset(tiledStartPoint, 0, rank * sizeof(unsigned int));

            BRformat format = stream->getDataFormat(bufNum);

            // We need to create a temporary stream to use copy method
            StreamImpl* tmpStream = new StreamImpl(rank, tiledDimension, &format, 1, _device);
            tmpStream->setBuffer(tmpBuffer, 0);

            unsigned int width = dimension[0];
            unsigned int height = (rank == 1) ? 1 : dimension[1];

            while(startPoint[1] < height)
            {
                startPoint[0] = 0;
                tiledDimension[0] = (width < TileSize) ? width : TileSize;

                while(startPoint[0] < width)
                {
                    // invoke copy shader
                    bool success = tmpStream->copy(stream, startPoint, tmpStream, tiledStartPoint, 
                                                   tiledDimension, false, bufNum, 0);

                    if(!success)
                    {
                        stream->setErrorCode(BR_ERROR_READ);
                        stream->setErrorString("Stream Write : Failed to copy data from stream to tiled "
                                               "host resource\n");

                        delete tmpStream;
                        return;
                    }

                    buffer->waitInputEvent();

                    // Get Host resource data in the given application pointer
                    unsigned int pitch = 0;
                    void* tmpPtr = tmpBuffer->getBufferPointerCPU(pitch);

                    if(!tmpPtr)
                    {
                        stream->setErrorCode(BR_ERROR_WRITE);
                        stream->setErrorString("Stream Write : Failed to map tiled resource\n");

                        delete tmpStream;
                        return;
                    }

                    stream->getData(tmpPtr, ptr, bufNum, pitch, startPoint, tiledDimension);
                    tmpBuffer->freeBufferPointerCPU();

                    // Move to next tile in horizontal direction
                    startPoint[0] = startPoint[0] + tiledDimension[0];
                    tiledDimension[0] = ((width - startPoint[0]) < TileSize) ? 
                                        (width - startPoint[0]) : TileSize;

                }

                // Move to next tile in vertical direction
                startPoint[1] = startPoint[1] + tiledDimension[1];
                tiledDimension[1] = ((height - startPoint[1]) < TileSize) ? 
                                     (height - startPoint[1]) : TileSize;
            }
            delete tmpStream;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Copy content of src Buffer to dst Buffer
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBufferMgr::copyBufferData(Buffer* dst, Buffer* src)
{
    src->flush();

    CALBuffer* dstBuffer = static_cast<CALBuffer*>(dst);
    CALBuffer* srcBuffer = static_cast<CALBuffer*>(src);

    //DMA transfer
    CALevent* copyEvent = _getCopyEvent();
    dstBuffer->copyAsync(srcBuffer, copyEvent);

    //Set the associated event on both src and dst buffer
    dstBuffer->setCopyEvent(copyEvent);
    srcBuffer->setInputEvent(copyEvent);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create CAL pinned buffers using CALBuffer.
//!
//! Uses a pinned resource cache to avoid allocation/ de-allocation
//!
//! \return Pointer to the newly created pinned buffer
//!
////////////////////////////////////////////////////////////////////////////////

CALBuffer*
CALBufferMgr::_createPinnedBuffer(const void* cpuPtr, unsigned short rank, unsigned int* dimensions,
                                  const CALformat &format, void* funcPtr)
{
    CALBuffer* tmpBuffer = new CALBuffer(rank, dimensions, format, BUFFER_HOST, 0, _device);

    // Ceate if no free buffer in cache and push it in the cache
    if(!tmpBuffer->initializePinnedBuffer(cpuPtr, funcPtr))
    {
        // If there is no same sized buffer, wait for all the events 
        // associated to host resouces to finish and delete them.
        for(unsigned int i = 0; i < _pinnedBufferCache.size(); ++i)
        {
            CALBuffer* buffer = _pinnedBufferCache[i];
            buffer->waitCopyEvent();
            buffer->waitWriteEvent();

            delete buffer;
        }

        _pinnedBufferCache.clear();

        // Try again to allocate resouce after deallocating all the resources
        if(!tmpBuffer->initializePinnedBuffer(cpuPtr, funcPtr))
        {
            delete tmpBuffer;

            return NULL;
        }
    }

    // Keep it in a cache after creation
    _pinnedBufferCache.push_back(tmpBuffer);

    return tmpBuffer;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create CAL host buffers using CALBuffer.
//!
//! Uses a host resource cache to avoid allocation/ de-allocation
//!
//! \return Pointer to the newly created host buffer
//!
////////////////////////////////////////////////////////////////////////////////

CALBuffer* 
CALBufferMgr::_createHostBuffer(unsigned short rank, unsigned int* dimensions,
                                  const CALformat& format)
{
    CALBuffer* tmpBuffer = new CALBuffer(rank, dimensions, format, BUFFER_HOST,
                                         0, _device);

    // Look into cache if a free resource exist of the same size
    unsigned int i = 0;
    CALBuffer* sameSizedBuffer = NULL;
    for(i = 0; i < _hostBufferCache.size(); ++i)
    {
        // Is size and format same?
        if(*_hostBufferCache[i] == *tmpBuffer)
        {
            // if size is same look if it is being used for another data transfer
            sameSizedBuffer = _hostBufferCache[i];
            CALevent copyEvent = sameSizedBuffer->getCopyEvent();
            CALevent inputEvent = sameSizedBuffer->getInputEvent();
            CALevent outputEvent = sameSizedBuffer->getOutputEvent();
            unsigned int id = sameSizedBuffer->getWriteThreadID();

            if(copyEvent == inputEvent == outputEvent == 0 && id == 0)
            {
                delete tmpBuffer;
                return sameSizedBuffer;
            }
        }
    }

    // Ceate if no free buffer in cache and push it in the cache
    if(!tmpBuffer->initialize())
    {
        // If same size buffer exist in cache. Wait for its events to finish
        // and return the same
        if(sameSizedBuffer)
        {
            sameSizedBuffer->waitCopyEvent();
            sameSizedBuffer->waitInputEvent();
            sameSizedBuffer->waitOutputEvent();
            sameSizedBuffer->waitWriteThread();

            delete tmpBuffer;

            return sameSizedBuffer;
        }
        else
        {
            // If there is no same sized buffer, wait for all the events 
            // associated to host resouces to finish and delete them.
            for(i = 0; i < _hostBufferCache.size(); ++i)
            {
                CALBuffer* buffer = _hostBufferCache[i];
                buffer->waitCopyEvent();
                buffer->waitInputEvent();
                buffer->waitOutputEvent();
                buffer->waitWriteThread();

                delete buffer;
            }

            _hostBufferCache.clear();

            // Try again to allocate resouce after deallocating all the resources
            if(!tmpBuffer->initialize())
            {
                delete tmpBuffer;

                return NULL;
            }
        }
    }

    // Keep it in a cache after creation
    _hostBufferCache.push_back(tmpBuffer);

    return tmpBuffer;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create CAL PCIe host buffers using CALBuffer.
//!
//! Uses a host resource cache to avoid allocation/ de-allocation
//!
//! \return Pointer to the newly created host buffer
//!
////////////////////////////////////////////////////////////////////////////////

CALBuffer* 
CALBufferMgr::_createPCIeHostBuffer(unsigned short rank, unsigned int* dimensions,
                                    const CALformat& format)
{
    CALBuffer* tmpBuffer = new CALBuffer(rank, dimensions, format, BUFFER_HOST,
                                         CAL_RESALLOC_CACHEABLE, _device);

    // Look into cache if a free resource exist of the same size
    unsigned int i = 0;
    CALBuffer* sameSizedBuffer = NULL;
    for(i = 0; i < _hostPCIeBufferCache.size(); ++i)
    {
        // Is size and format same?
        if(*_hostPCIeBufferCache[i] == *tmpBuffer)
        {
            // if size is same look if it is being used for another data transfer
            sameSizedBuffer = _hostPCIeBufferCache[i];
            CALevent copyEvent = sameSizedBuffer->getCopyEvent();
            CALevent inputEvent = sameSizedBuffer->getInputEvent();
            CALevent outputEvent = sameSizedBuffer->getOutputEvent();
            unsigned int id = sameSizedBuffer->getWriteThreadID();

            if(copyEvent == inputEvent == outputEvent == 0 && id == 0)
            {
                delete tmpBuffer;
                return sameSizedBuffer;
            }
        }
    }

    // Ceate if no free buffer in cache and push it in the cache
    if(!tmpBuffer->initialize())
    {
        // If same size buffer exist in cache. Wait for its events to finish
        // and return the same
        if(sameSizedBuffer)
        {
            sameSizedBuffer->waitCopyEvent();
            sameSizedBuffer->waitInputEvent();
            sameSizedBuffer->waitOutputEvent();
            sameSizedBuffer->waitWriteThread();

            delete tmpBuffer;

            return sameSizedBuffer;
        }
        else
        {
            // If there is no same sized buffer, wait for all the events 
            // associated to host resouces to finish and delete them.
            for(i = 0; i < _hostPCIeBufferCache.size(); ++i)
            {
                CALBuffer* buffer = _hostPCIeBufferCache[i];
                buffer->waitCopyEvent();
                buffer->waitInputEvent();
                buffer->waitOutputEvent();
                buffer->waitWriteThread();

                delete buffer;
            }

            _hostPCIeBufferCache.clear();

            // Try again to allocate resouce after deallocating all the resources
            if(!tmpBuffer->initialize())
            {
                delete tmpBuffer;

                return NULL;
            }
        }
    }

    // Keep it in a cache after creation
    _hostPCIeBufferCache.push_back(tmpBuffer);

    return tmpBuffer;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to clear used constant cache.
//!
//! This is used to avoid the case when same constant buffer is retuned for a
//! kernel invocation
//!
//!
////////////////////////////////////////////////////////////////////////////////

void
CALBufferMgr::clearUsedConstCache()
{
    _usedConstBuffers.clear();
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create CALConstBuffer
//!
//! Uses a constant buffer cache to avoid allocation/ de-allocation
//!
//! \return Pointer to the newly created constant buffer
//!
////////////////////////////////////////////////////////////////////////////////

CALConstBuffer*
CALBufferMgr::createConstBuffer(unsigned int numConstants, BRformat format)
{
    CALDevice* device = static_cast<CALDevice*>(_device);
    CALdeviceinfo info = device->getInfo();
    if(numConstants > info.maxResource1DWidth)
    {
        return NULL;
    }

    // Try to use a nearest power 2 dimension so that we can 
    // cover cases when we need < 64 constant in a kernel(very likely)
    // with single constant buffer
    unsigned int size = Utility::ceilPow(numConstants);
    if(size == 0)
    {
        return NULL;
    }

    size = (size > 64) ? size : 64;

    unsigned int dimensions[] = {size};
    CALConstBuffer* tmpBuffer;

    CALformat calFormat = getCALFormat(format);
    tmpBuffer = new CALConstBuffer(dimensions, _device, calFormat);

    // Look into cache if a free resource exist of the same size
    unsigned int i = 0;
    CALConstBuffer* sameSizedBuffer = NULL;
    for(i = 0; i < _constBufferCache.size(); ++i)
    {
        // Is the size same?
        if(*_constBufferCache[i] == *tmpBuffer)
        {
            unsigned int j = 0;
            for(j = 0; j < _usedConstBuffers.size(); ++j)
            {
                if(_constBufferCache[i] == _usedConstBuffers[j])
                {
                    break;
                }
            }

            if(j == _usedConstBuffers.size())
            {
                sameSizedBuffer = _constBufferCache[i];
                CALevent inputEvent = sameSizedBuffer->getInputEvent();

                // If it is free, return it.
                if(inputEvent == 0)
                {
                    delete tmpBuffer;
                    sameSizedBuffer->reuse();
                    _usedConstBuffers.push_back(sameSizedBuffer);

                    return sameSizedBuffer;
                }
            }
        }
    }

    // Ceate if no free buffer in cache and push it in the cache
    if(!tmpBuffer->initialize())
    {
        // If same size buffer exist in cache. Wait for its events to finish
        // and return the same
        if(sameSizedBuffer)
        {
            sameSizedBuffer->waitInputEvent();
            delete tmpBuffer;
            sameSizedBuffer->reuse();
            _usedConstBuffers.push_back(sameSizedBuffer);

            return sameSizedBuffer;
        }
        else
        {
            // If there is no same sized buffer, wait for all the events 
            // associated to host resouces to finish and delete them.
            for(i = 0; i < _constBufferCache.size(); ++i)
            {
                CALConstBuffer* buffer = _constBufferCache[i];
                buffer->waitInputEvent();

                delete buffer;
            }

            _constBufferCache.clear();

            // Try again to allocate resouce after deallocating all the resources
            if(!tmpBuffer->initialize())
            {
                SET_ERROR("Failed to create host cal resource for constant buffer\n");
                delete tmpBuffer;

                return NULL;
            }
        }
    }

    _constBufferCache.push_back(tmpBuffer);
    _usedConstBuffers.push_back(tmpBuffer);

    return tmpBuffer;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief 
//! return backend specific resize kernel, used in input stream resize 
//! to match their size with output streams
//!
////////////////////////////////////////////////////////////////////////////////

const brook::KernelDesc*
CALBufferMgr::getResizeShaderDesc()
{
    return _resize_shader_desc;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief return backend specific copy kernel, used in tile by tile 
//! copying and domain implementation
//!
////////////////////////////////////////////////////////////////////////////////

const brook::KernelDesc*
CALBufferMgr::getCopyShaderDesc()
{
    return _copy_shader_desc;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Destructor
//! Delete data from caches for host buffer and constant buffer
//!
////////////////////////////////////////////////////////////////////////////////

CALBufferMgr::~CALBufferMgr()
{
    // Destroy all the host side, pinned and constant buffers available in cache

    for(unsigned int i = 0; i < _pinnedBufferCache.size(); ++i)
    {
        delete _pinnedBufferCache[i];
    }

    for(unsigned int i = 0; i < _hostBufferCache.size(); ++i)
    {
        delete _hostBufferCache[i];
    }

    for(unsigned int i = 0; i < _hostPCIeBufferCache.size(); ++i)
    {
        delete _hostPCIeBufferCache[i];
    }

    for(unsigned int i = 0; i < _constBufferCache.size(); ++i)
    {
        delete _constBufferCache[i];
    }

    for(unsigned int i = 0; i < _copyEvents.size(); ++i)
    {
        delete _copyEvents[i];
    }

    for(unsigned int i = 0; i < MaxThreads; ++i)
    {
        delete _writeData[i];
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Method to create backend spefic buffers using CALBuffer.
//!
//! \param stream The associated stream
//! \return Pointer to the newly created buffer
//!
////////////////////////////////////////////////////////////////////////////////

CALformat 
CALBufferMgr::getCALFormat(BRformat format) const
{
    switch(format)
    {
        case BR_FORMAT_CHAR_1:
            return CAL_FORMAT_SIGNED_INT8_1;
        case BR_FORMAT_CHAR_2:
            return CAL_FORMAT_SIGNED_INT8_2;
        case BR_FORMAT_CHAR_3:
        case BR_FORMAT_CHAR_4:
            return CAL_FORMAT_SIGNED_INT8_4;

        case BR_FORMAT_UCHAR_1:
            return CAL_FORMAT_UNSIGNED_INT8_1;
        case BR_FORMAT_UCHAR_2:
            return CAL_FORMAT_UNSIGNED_INT8_2;
        case BR_FORMAT_UCHAR_3:
        case BR_FORMAT_UCHAR_4:
            return CAL_FORMAT_UNSIGNED_INT8_4;

        case BR_FORMAT_SHORT_1:
            return CAL_FORMAT_SIGNED_INT16_1;
        case BR_FORMAT_SHORT_2:
            return CAL_FORMAT_SIGNED_INT16_2;
        case BR_FORMAT_SHORT_3:
        case BR_FORMAT_SHORT_4:
            return CAL_FORMAT_SIGNED_INT16_4;

        case BR_FORMAT_USHORT_1:
            return CAL_FORMAT_UNSIGNED_INT16_1;
        case BR_FORMAT_USHORT_2:
            return CAL_FORMAT_UNSIGNED_INT16_2;
        case BR_FORMAT_USHORT_3:
        case BR_FORMAT_USHORT_4:
            return CAL_FORMAT_UNSIGNED_INT16_4;

        case BR_FORMAT_FLOAT_1:
            return CAL_FORMAT_FLOAT_1;
        case BR_FORMAT_FLOAT_2:
            return CAL_FORMAT_FLOAT_2;
        case BR_FORMAT_FLOAT_3:
        case BR_FORMAT_FLOAT_4:
            return CAL_FORMAT_FLOAT_4;

        case BR_FORMAT_UINT_1:
            return CAL_FORMAT_UINT_1;
        case BR_FORMAT_UINT_2:
            return CAL_FORMAT_UINT_2;
        case BR_FORMAT_UINT_3:
        case BR_FORMAT_UINT_4:
            return CAL_FORMAT_UINT_4;

        case BR_FORMAT_INT_1:
            return CAL_FORMAT_INT_1;
        case BR_FORMAT_INT_2:
            return CAL_FORMAT_INT_2;
        case BR_FORMAT_INT_3:
        case BR_FORMAT_INT_4:
            return CAL_FORMAT_INT_4;

        case BR_FORMAT_DOUBLE_1:
            return CAL_FORMAT_DOUBLE_1;
        case BR_FORMAT_DOUBLE_2:
            return CAL_FORMAT_DOUBLE_2;
    }

    return CAL_FORMAT_FLOAT_1;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Return an event that is not being used
//!
////////////////////////////////////////////////////////////////////////////////

CALevent* 
CALBufferMgr::_getCopyEvent()
{
    // Look for a free event in the cache
    unsigned int i;
    for(i = 0; i < _copyEvents.size(); ++i)
    {
        if(*_copyEvents[i] == 0)
        {
            return _copyEvents[i];
        }
    }

    // Create if not found
    CALevent* event = new CALevent();
    *event = 0;
    _copyEvents.push_back(event);

    return event;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief Get CPUThread to run a new stremWrite asynchronously
//!
////////////////////////////////////////////////////////////////////////////////

ThreadWriteData*
CALBufferMgr::_getThreadWriteData()
{
    for(unsigned int i = 0; i < MaxThreads; ++i)
    {
        if(!_writeData[i])
        {
            return new ThreadWriteData;
        }
        else if(_writeData[i]->thread->getID() == 0)
        {
            return _writeData[i];
        }
    }

    // return first thread
    _writeData[0]->thread->join();

    return _writeData[0];
}
