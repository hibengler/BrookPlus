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
with �RESTRICTED RIGHTS� and/or �LIMITED RIGHTS� as applicable to computer 
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

#ifndef __ILTABLES_H__
#define __ILTABLES_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IL_BIT
typedef unsigned int Bit;
#define IL_BIT
#endif

#define IL_MINOR_VERSION 0
#define IL_MAJOR_VERSION 2

enum IL_Shader_Type 
{
  IL_SHADER_VERTEX,
  IL_SHADER_PIXEL,
  IL_SHADER_GEOMETRY,
  IL_SHADER_COMPUTE,  
  IL_SHADER_LAST  /* dimension the enumeration */
};

enum IL_Language_Type
{
  IL_LANG_GENERIC,
  IL_LANG_OPENGL,
  IL_LANG_DX8_PS,
  IL_LANG_DX8_VS,
  IL_LANG_DX9_PS,
  IL_LANG_DX9_VS,
  IL_LANG_DX10_PS,
  IL_LANG_DX10_VS,
  IL_LANG_DX10_GS,            
  IL_LANG_LAST    /* dimension the enumeration */
};

enum ILOpCode 
{
  IL_OP_UNKNOWN,
  IL_OP_ABS,
  IL_OP_ACOS,
  IL_OP_ADD,
  IL_OP_ASIN,
  IL_OP_ATAN,
  IL_OP_BREAK,
  IL_OP_BREAKC,
  IL_OP_CALL,
  IL_OP_CALLNZ,
  IL_OP_CLAMP,
  IL_OP_CLG,
  IL_OP_CMOV,
  IL_OP_CMP,
  IL_OP_COLORCLAMP,
  IL_OP_COMMENT,
  IL_OP_CONTINUE,
  IL_OP_CONTINUEC,
  IL_OP_COS,
  IL_OP_CRS,
  IL_OP_DCLARRAY,
  IL_OP_DCLDEF,
  IL_OP_DCLPI,
  IL_OP_DCLPIN,
  IL_OP_DCLPP,
  IL_OP_DCLPT,
  IL_OP_DCLV,
  IL_OP_DCLVOUT,
  IL_OP_DEF,
  IL_OP_DEFB,
  IL_OP_DET,
  IL_OP_DIST,
  IL_OP_DIV,
  IL_OP_DP2ADD,
  IL_OP_DP3,
  IL_OP_DP4,
  IL_OP_DST,
  IL_OP_DSX,
  IL_OP_DSY,
  IL_OP_ELSE,
  IL_OP_END,
  IL_OP_ENDIF,
  IL_OP_ENDLOOP,
  IL_OP_ENDMAIN,
  IL_OP_EXN,
  IL_OP_EXP,
  IL_OP_EXPP,
  IL_OP_FACEFORWARD,
  IL_OP_FLR,
  IL_OP_FRC,
  IL_OP_FUNC,
  IL_OP_FWIDTH,
  IL_OP_IFC,
  IL_OP_IFNZ,
  IL_OP_INITV,
  IL_OP_KILL,
  IL_OP_LEN,
  IL_OP_LIT,
  IL_OP_LN,
  IL_OP_LOD,
  IL_OP_LOG,
  IL_OP_LOGP,
  IL_OP_LOOP,
  IL_OP_LRP,
  IL_OP_MAD,
  IL_OP_MAX,
  IL_OP_MEMEXPORT,
  IL_OP_MEMIMPORT,
  IL_OP_MIN,
  IL_OP_MMUL,
  IL_OP_MOD,
  IL_OP_MOV,
  IL_OP_MOVA,
  IL_OP_MUL,
  IL_OP_NOISE,
  IL_OP_NOP,
  IL_OP_NRM,
  IL_OP_PIREDUCE,
  IL_OP_POW,
  IL_OP_PRECOMP,
  IL_OP_PROJECT,
  IL_OP_RCP,
  IL_OP_REFLECT,
  IL_OP_RET,
  IL_OP_RND,
  IL_OP_RSQ,
  IL_OP_SET,
  IL_OP_SGN,
  IL_OP_SIN,
  IL_OP_SINCOS,
  IL_OP_SQRT,
  IL_OP_SUB,
  IL_OP_TAN,
  IL_OP_TEXLD,
  IL_OP_TEXLDB,
  IL_OP_TEXLDD,
  IL_OP_TEXLDMS,
  IL_OP_TEXWEIGHT,
  IL_OP_TRANSPOSE,
  IL_OP_TRC,
  IL_OP_DXSINCOS,
  // added opcodes
  IL_OP_BREAK_LOGICALZ,
  IL_OP_BREAK_LOGICALNZ,
  IL_OP_CALL_LOGICALZ,
  IL_OP_CALL_LOGICALNZ,
  IL_OP_CASE,
  IL_OP_CONTINUE_LOGICALZ,
  IL_OP_CONTINUE_LOGICALNZ,
  IL_OP_DEFAULT,
  IL_OP_ENDSWITCH,
  IL_OP_ENDFUNC,
  IL_OP_IF_LOGICALZ,
  IL_OP_IF_LOGICALNZ,
  IL_OP_WHILE,
  IL_OP_SWITCH,
  IL_OP_RET_DYN,
  IL_OP_RET_LOGICALZ,
  IL_OP_RET_LOGICALNZ,
  IL_DCL_CONST_BUFFER,
  IL_DCL_INDEXED_TEMP_ARRAY,
  IL_DCL_INPUT_PRIMITIVE,
  IL_DCL_LITERAL,
  IL_DCL_MAX_OUTPUT_VERTEX_COUNT,
  IL_DCL_ODEPTH,
  IL_DCL_OUTPUT_TOPOLOGY,
  IL_DCL_OUTPUT,
  IL_DCL_INPUT,
  IL_DCL_VPRIM,
  IL_DCL_RESOURCE,
  IL_OP_CUT,
  IL_OP_DISCARD_LOGICALZ,
  IL_OP_DISCARD_LOGICALNZ,
  IL_OP_EMIT,
  IL_OP_EMIT_THEN_CUT,
  IL_OP_LOAD,
  IL_OP_RESINFO,
  IL_OP_SAMPLE,
  IL_OP_SAMPLE_B,  
  IL_OP_SAMPLE_G,
  IL_OP_SAMPLE_L,
  IL_OP_SAMPLE_C,
  IL_OP_SAMPLE_C_LZ,
  IL_OP_I_NOT,
  IL_OP_I_OR,
  IL_OP_I_XOR,
  IL_OP_I_ADD,
  IL_OP_I_MAD,
  IL_OP_I_MAX,
  IL_OP_I_MIN,
  IL_OP_I_MUL,
  IL_OP_I_MUL_HIGH,
  IL_OP_I_EQ,
  IL_OP_I_GE,
  IL_OP_I_LT,
  IL_OP_I_NEGATE,
  IL_OP_I_NE,
  IL_OP_I_SHL,
  IL_OP_I_SHR,
  IL_OP_U_SHR,
  IL_OP_U_DIV,
  IL_OP_U_MOD,
  IL_OP_U_MAD,
  IL_OP_U_MAX,
  IL_OP_U_MIN,
  IL_OP_U_LT,
  IL_OP_U_GE,
  IL_OP_U_MUL,
  IL_OP_U_MUL_HIGH,
  IL_OP_FTOI,
  IL_OP_FTOU,
  IL_OP_ITOF,
  IL_OP_UTOF,
  IL_OP_AND,
  IL_OP_CMOV_LOGICAL,
  IL_OP_EQ,
  IL_OP_EXP_VEC,
  IL_OP_GE,
  IL_OP_LOG_VEC,
  IL_OP_LT,
  IL_OP_NE,
  IL_OP_ROUND_NEAR,
  IL_OP_ROUND_NEG_INF,
  IL_OP_ROUND_PLUS_INF,
  IL_OP_ROUND_ZERO,
  IL_OP_RSQ_VEC,
  IL_OP_SIN_VEC,
  IL_OP_COS_VEC,
  IL_OP_SQRT_VEC,
  IL_OP_DP2,
  IL_OP_INV_MOV,
  IL_OP_SCATTER,
  IL_OP_D_FREXP,
  IL_OP_D_ADD,
  IL_OP_D_MUL,
  IL_OP_D_2_F,
  IL_OP_F_2_D,
  IL_OP_D_LDEXP,
  IL_OP_D_FRAC,
  IL_OP_D_MULADD,
  IL_OP_FETCH4,
  IL_OP_SAMPLEINFO,
  IL_OP_GETLOD,  // the dx10.1 version of lod
  IL_DCL_PERSIST,
  IL_OP_DNE, 
  IL_OP_DEQ,
  IL_OP_DGE,
  IL_OP_DLT,
  IL_OP_SAMPLEPOS,
  IL_OP_D_DIV,
  IL_OP_DCL_SHARED_TEMP,
  IL_OP_DCL_NUM_THREAD_PER_GROUP, // thread group = thread block 
  IL_OP_DCL_TOTAL_NUM_THREAD_GROUP,
  IL_OP_DCL_LDS_SIZE_PER_THREAD,
  IL_OP_DCL_LDS_SHARING_MODE,
  IL_OP_LDS_READ_VEC,
  IL_OP_LDS_WRITE_VEC,
  IL_OP_FENCE,  
  IL_OP_LDS_RAW_LD,
  IL_OP_LDS_RAW_STORE,
  IL_OP_LDS_ADD,
  IL_OP_LDS_SUB,
  IL_OP_LDS_RSUB,
  IL_OP_LDS_MIN,
  IL_OP_LDS_MAX,
  IL_OP_LDS_UMIN,
  IL_OP_LDS_UMAX,
  IL_OP_LDS_AND,
  IL_OP_LDS_OR,
  IL_OP_LDS_XOR,
  IL_OP_LDS_READ_ADD,
  IL_OP_LDS_READ_SUB,
  IL_OP_LDS_READ_RSUB,
  IL_OP_LDS_READ_MIN,
  IL_OP_LDS_READ_MAX,
  IL_OP_LDS_READ_UMIN,
  IL_OP_LDS_READ_UMAX,
  IL_OP_LDS_READ_AND,
  IL_OP_LDS_READ_OR,
  IL_OP_LDS_READ_XOR,
  IL_OP_LDS_READ_XCHG,
  IL_OP_LDS_READ_CMP_XCHG,
  IL_OP_LAST  /* dimension the enumeration */
};

enum ILRegType 
{
  IL_REGTYPE_CONST_BOOL,
  IL_REGTYPE_CONST_FLOAT,
  IL_REGTYPE_CONST_DOUBLE,
  IL_REGTYPE_CONST_INT,
  IL_REGTYPE_ADDR,
  IL_REGTYPE_TEMP,
  IL_REGTYPE_VERTEX,
  IL_REGTYPE_INDEX,
  IL_REGTYPE_OBJECT_INDEX,
  IL_REGTYPE_BARYCENTRIC_COORD,
  IL_REGTYPE_PRIMITIVE_INDEX,
  IL_REGTYPE_QUAD_INDEX,
  IL_REGTYPE_VOUTPUT,
  IL_REGTYPE_PINPUT,
  IL_REGTYPE_SPRITE,
  IL_REGTYPE_POS,
  IL_REGTYPE_INTERP,
  IL_REGTYPE_FOG,
  IL_REGTYPE_TEXCOORD,
  IL_REGTYPE_PRICOLOR,
  IL_REGTYPE_SECCOLOR,
  IL_REGTYPE_SPRITECOORD,
  IL_REGTYPE_FACE,
  IL_REGTYPE_WINCOORD,
  IL_REGTYPE_PRIMCOORD,
  IL_REGTYPE_PRIMTYPE,
  IL_REGTYPE_PCOLOR,
  IL_REGTYPE_DEPTH,
  IL_REGTYPE_STENCIL,
  IL_REGTYPE_CLIP,
  IL_REGTYPE_VPRIM,
  IL_REGTYPE_ITEMP,
  IL_REGTYPE_CONST_BUFF,
  IL_REGTYPE_LITERAL,
  IL_REGTYPE_INPUT,
  IL_REGTYPE_OUTPUT,
  IL_REGTYPE_IMMED_CONST_BUFF,
  IL_REGTYPE_OMASK,
  IL_REGTYPE_PERSIST,
  IL_REGTYPE_GLOBAL,
  IL_REGTYPE_PS_OUT_FOG,
  IL_REGTYPE_SHARED_TEMP, /*R7xx */
  IL_REGTYPE_THREAD_ID, /* R7xx, thread id within a block */  
  IL_REGTYPE_ABSOLUTE_THREAD_ID, /* R7xx, absolute thread id  */  
  IL_REGTYPE_THREAD_GROUP_ID, /* thread group = thread block */  
  IL_REGTYPE_GENERIC_MEM, /* generic memory type, need this for the mask of lds_write */
  IL_REGTYPE_LAST      /* dimension the enumeration */
};

enum ILMatrix
{
  IL_MATRIX_4X4,
  IL_MATRIX_4X3,
  IL_MATRIX_3X4,
  IL_MATRIX_3X3,
  IL_MATRIX_3X2,
  IL_MATRIX_LAST      /* dimension the enumeration */
};

enum ILDivComp 
{
  IL_DIVCOMP_NONE,    /* None */
  IL_DIVCOMP_Y,       /* Divide the x component by y */
  IL_DIVCOMP_Z,       /* Divide the x and y components by z */
  IL_DIVCOMP_W,       /* Divide the x, y, and z components by w */
  IL_DIVCOMP_UNKNOWN, /* Divide each component by the value of AS_**** */
  IL_DIVCOMP_LAST   
};

enum ILZeroOp
{
  IL_ZEROOP_FLTMAX,
  IL_ZEROOP_0,
  IL_ZEROOP_INFINITY,
  IL_ZEROOP_INF_ELSE_MAX,
  IL_ZEROOP_LAST    /* dimension the enumeration */
};

enum ILModDstComponent
{  
  IL_MODCOMP_NOWRITE,
  IL_MODCOMP_WRITE, 
  IL_MODCOMP_0,
  IL_MODCOMP_1,
  IL_MODCOMP_LAST     /* dimension the enumeration */
};

enum ILComponentSelect
{
  IL_COMPSEL_X_R,
  IL_COMPSEL_Y_G,
  IL_COMPSEL_Z_B,
  IL_COMPSEL_W_A,
  IL_COMPSEL_0,
  IL_COMPSEL_1,
  IL_COMPSEL_LAST     /* dimension the enumeration */
};

enum ILShiftScale 
{
  IL_SHIFT_NONE = 0,
  IL_SHIFT_X2,
  IL_SHIFT_X4,
  IL_SHIFT_X8,
  IL_SHIFT_D2,
  IL_SHIFT_D4,
  IL_SHIFT_D8,
  IL_SHIFT_LAST       /* dimension the enumeration */
};

enum ILRelOp 
{
  IL_RELOP_NE,  /* != */
  IL_RELOP_EQ,  /* == */
  IL_RELOP_GE,  /* >= */
  IL_RELOP_GT,  /* > */
  IL_RELOP_LE,  /* <= */
  IL_RELOP_LT,  /* < */
  IL_RELOP_LAST /* dimension the enumeration */
};

enum ILDefaultVal
{
  IL_DEFVAL_NONE = 0,
  IL_DEFVAL_0,
  IL_DEFVAL_1,
  IL_DEFVAL_LAST /* dimension the enumeration */
};

enum ILImportComponent
{
  IL_IMPORTSEL_UNUSED = 0,
  IL_IMPORTSEL_DEFAULT0,
  IL_IMPORTSEL_DEFAULT1,
  IL_IMPORTSEL_UNDEFINED,
  IL_IMPORTSEL_LAST  /*  dimension the enum */
};

enum ILImportUsage
{
  IL_IMPORTUSAGE_POS = 0,
  IL_IMPORTUSAGE_POINTSIZE,
  IL_IMPORTUSAGE_COLOR,
  IL_IMPORTUSAGE_BACKCOLOR,
  IL_IMPORTUSAGE_FOG,
  IL_IMPORTUSAGE_PRIMCOORDTYPE,
  IL_IMPORTUSAGE_GENERIC,
  IL_IMPORTUSAGE_CLIPDISTANCE,
  IL_IMPORTUSAGE_CULLDISTANCE,
  IL_IMPORTUSAGE_PRIMITIVEID,
  IL_IMPORTUSAGE_VERTEXID,
  IL_IMPORTUSAGE_INSTANCEID,
  IL_IMPORTUSAGE_ISFRONTFACE,
  IL_IMPORTUSAGE_LOD,
  IL_IMPORTUSAGE_COLORING,
  IL_IMPORTUSAGE_NODE_COLORING,
  IL_IMPORTUSAGE_NORMAL,
  IL_IMPORTUSAGE_RENDERTARGET_ARRAY_INDEX,
  IL_IMPORTUSAGE_VIEWPORT_ARRAY_INDEX,
  IL_IMPORTUSAGE_UNDEFINED,
  IL_IMPORTUSAGE_SAMPLE_INDEX,
  IL_IMPORTUSAGE_LAST  /*  dimension the enum */
};

enum ILCmpVal
{
  IL_CMPVAL_0_0 = 0, /* compare vs. 0.0  */
  IL_CMPVAL_0_5,     /* compare vs. 0.5  */
  IL_CMPVAL_1_0,     /* compare vs. 1.0  */
  IL_CMPVAL_NEG_0_5, /* compare vs. -0.5 */
  IL_CMPVAL_NEG_1_0, /* compare vs. -1.0 */
  IL_CMPVAL_LAST     /* dimension the enumeration */
};

enum ILPixTexUsage
{
  IL_USAGE_PIXTEX_UNKNOWN = 0,
  IL_USAGE_PIXTEX_1D,
  IL_USAGE_PIXTEX_2D,
  IL_USAGE_PIXTEX_3D,
  IL_USAGE_PIXTEX_CUBEMAP,
  IL_USAGE_PIXTEX_2DMSAA,
  // there is another table in channel.cpp that must be kept in order with
  // this table
  // il allows 0-7 values for dclpt (dx9 only)
  // dx10 formats after this point
  IL_USAGE_PIXTEX_4COMP,  
  IL_USAGE_PIXTEX_BUFFER,
  IL_USAGE_PIXTEX_1DARRAY,
  IL_USAGE_PIXTEX_2DARRAY,
  
  // adding this in to exploit a Pele hardware feature
  IL_USAGE_PIXTEX_2D_PLUS_W,
  IL_USAGE_PIXTEX_CUBEMAP_PLUS_W,

  // there is also a table in dx10interpreter.cpp that maps dx names to il names
  // which also needs to be kept in sync
  // dx 10.1 
  IL_USAGE_PIXTEX_CUBEMAP_ARRAY,
  
  IL_USAGE_PIXTEX_LAST    /* dimension the enumeration */
};

enum ILTexCoordMode
{
  IL_TEXCOORDMODE_UNKNOWN = 0,
  IL_TEXCOORDMODE_NORMALIZED,
  IL_TEXCOORDMODE_UNNORMALIZED,
  IL_TEXCOORDMODE_LAST   /* dimension the enumeration */
};

enum ILElementFormat
{
  IL_ELEMENTFORMAT_UNKNOWN = 0,
  IL_ELEMENTFORMAT_SNORM,
  IL_ELEMENTFORMAT_UNORM,
  IL_ELEMENTFORMAT_SINT,
  IL_ELEMENTFORMAT_UINT,
  IL_ELEMENTFORMAT_FLOAT,
  IL_ELEMENTFORMAT_SRGB,
  IL_ELEMENTFORMAT_MIXED,
  IL_ELEMENTFORMAT_LAST
};

enum ILTexShadowMode
{
  IL_TEXSHADOWMODE_NEVER = 0,
  IL_TEXSHADOWMODE_Z,
  IL_TEXSHADOWMODE_UNKNOWN,
  IL_TEXSHADOWMODE_LAST /* dimension the enumeration */
};

enum ILTexFilterMode
{
  IL_TEXFILTER_UNKNOWN = 0,
  IL_TEXFILTER_POINT,
  IL_TEXFILTER_LINEAR,
  IL_TEXFILTER_ANISO,
  IL_TEXFILTER_LAST    /* dimension the enumeration */
};

enum ILMipFilterMode
{
  IL_MIPFILTER_UNKNOWN = 0,
  IL_MIPFILTER_POINT,
  IL_MIPFILTER_LINEAR,
  IL_MIPFILTER_BASE,
  IL_MIPFILTER_LAST    /* dimension the enumeration */
};

enum ILAnisoFilterMode
{
  IL_ANISOFILTER_UNKNOWN = 0,
  IL_ANISOFILTER_DISABLED,
  IL_ANISOFILTER_MAX_1_TO_1,
  IL_ANISOFILTER_MAX_2_TO_1,
  IL_ANISOFILTER_MAX_4_TO_1,
  IL_ANISOFILTER_MAX_8_TO_1,
  IL_ANISOFILTER_MAX_16_TO_1,
  IL_ANISOFILTER_LAST   /* dimension the enumeration */
};

enum ILNoiseType
{
  IL_NOISETYPE_PERLIN1D = 0,
  IL_NOISETYPE_PERLIN2D,
  IL_NOISETYPE_PERLIN3D,
  IL_NOISETYPE_PERLIN4D,
  IL_NOISETYPE_LAST    /* dimension the enumeration */
};


enum ILAddressing
{
  IL_ADDR_ABSOLUTE = 0,
  IL_ADDR_RELATIVE,
  IL_ADDR_REG_RELATIVE,
  IL_ADDR_LAST    /* dimension the enumeration */
};

enum ILInterpMode
{
  IL_INTERPMODE_NOTUSED = 0,
  IL_INTERPMODE_CONSTANT,
  IL_INTERPMODE_LINEAR,
  IL_INTERPMODE_LINEAR_CENTROID,
  IL_INTERPMODE_LINEAR_NOPERSPECTIVE,
  IL_INTERPMODE_LINEAR_NOPERSPECTIVE_CENTROID,
  IL_INTERPMODE_LINEAR_SAMPLE,
  IL_INTERPMODE_LINEAR_NOPERSPECTIVE_SAMPLE,
  IL_INTERPMODE_LAST    /* dimension the enumeration */
};

// types for scatter
enum IL_SCATTER
{
  IL_SCATTER_BY_PIXEL,
  IL_SCATTER_BY_QUAD 
};

// types that can be input to a gemetry shader

enum IL_TOPOLOGY
{
  IL_TOPOLOGY_POINT,
  IL_TOPOLOGY_LINE,
  IL_TOPOLOGY_TRIANGLE,
  IL_TOPOLOGY_LINE_ADJ,
  IL_TOPOLOGY_TRIANGLE_ADJ,
  IL_TOPOLOGY_LAST  /* dimension the enumeration */
};

enum IL_OUTPUT_TOPOLOGY
{
  IL_OUTPUT_TOPOLOGY_POINT_LIST,
  IL_OUTPUT_TOPOLOGY_LINE_STRIP,
  IL_OUTPUT_TOPOLOGY_TRIANGLE_STRIP,
  IL_OUTPUT_TOPOLOGY_LAST  /* dimension the enumeration */
};

// for R7xx Compute shader
enum IL_LDS_SHARING_MODE
{
    IL_LDS_SHARING_MODE_RELATIVE = 0, // for wavefront_rel
    IL_LDS_SHARING_MODE_ABSOLUTE,  // for wavefront_abs
    IL_LDS_SHARING_MODE_LAST    
};    

#ifdef __cplusplus
}
#endif


#endif

/* ### EOF ### */