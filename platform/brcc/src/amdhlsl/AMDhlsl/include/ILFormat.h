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

#ifndef __ATI_ILFORMAT_H__
#define __ATI_ILFORMAT_H__

#ifndef IL_BIT
typedef unsigned int Bit;
#define IL_BIT
#endif

/***********************************************************************************
** With hardware registers, we need to ensure code which changes a register       **
** as a bifield will produce identical 32 bit register values on both little      **
** and big endian machines. We'll do the same for IL to simplify working on       **
** both little and big endian machines.                                           **
**                                                                                **
** The only known big endian platform this code is compiled to is the PowerPC.    **
** The following preprocessor code code may need to be modified if any other      **
** big endian processors become clients of SC                                     **
***********************************************************************************/

#if !defined( qLittleEndian ) && !defined( __ppc__ ) && !defined( __ppc64__ ) && \
    !defined(__BIG_ENDIAN__)
#define qLittleEndian 1
#endif
                          
/********************************************************* 
** General Purpose IL Tokens:                           **
** Generic formats used for IL tokens                   **
*********************************************************/
typedef union IL_Lang
{
  struct 
  {
  #ifdef qLittleEndian
    Bit client_type      : 8;
    Bit reserved         : 24;
  #else
    Bit reserved         : 24;
    Bit client_type      : 8;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Lang;

typedef union IL_Version 
{
  struct
  {
  #ifdef qLittleEndian
    Bit minor_version : 8;
    Bit major_version : 8;
    Bit shader_type   : 8;
    Bit multipass     : 1;
    Bit realtime      : 1;
    Bit reserved      : 6;
  #else
    Bit reserved      : 6;
    Bit realtime      : 1;
    Bit multipass     : 1;
    Bit shader_type   : 8;
    Bit major_version : 8;
    Bit minor_version : 8;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Version;

typedef union IL_OpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls             : 14;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit controls             : 14;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_OpCode;

typedef union IL_Dst
{
  struct
  {
  #ifdef qLittleEndian
    Bit register_num      : 16;
    Bit register_type     : 6;
    Bit modifier_present  : 1;
    Bit relative_address  : 2;
    Bit dimension         : 1;
    Bit immediate_present : 1;
    Bit reserved          : 4;
    Bit extended          : 1;
  #else
    Bit extended          : 1;
    Bit reserved          : 4;
    Bit immediate_present : 1;
    Bit dimension         : 1;
    Bit relative_address  : 2;
    Bit modifier_present  : 1;
    Bit register_type     : 6;
    Bit register_num      : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Dst;

typedef union IL_Dst_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit component_x_r : 2;
    Bit component_y_g : 2;
    Bit component_z_b : 2;
    Bit component_w_a : 2;
    Bit clamp         : 1;
    Bit shift_scale   : 4;
    Bit reserved      : 19;    
  #else
    Bit reserved      : 19;    
    Bit shift_scale   : 4;
    Bit clamp         : 1;
    Bit component_w_a : 2;
    Bit component_z_b : 2;
    Bit component_y_g : 2;
    Bit component_x_r : 2;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Dst_Mod;

typedef union IL_Src
{
  struct
  {
  #ifdef qLittleEndian
    Bit register_num      : 16;
    Bit register_type     : 6;
    Bit modifier_present  : 1;
    Bit relative_address  : 2;
    Bit dimension         : 1;
    Bit immediate_present : 1;
    Bit reserved          : 4;
    Bit extended          : 1;
  #else
    Bit extended          : 1;
    Bit reserved          : 4;
    Bit immediate_present : 1;
    Bit dimension         : 1;
    Bit relative_address  : 2;
    Bit modifier_present  : 1;
    Bit register_type     : 6;
    Bit register_num      : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Src;

typedef union IL_Src_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit swizzle_x_r     : 3;
    Bit negate_x_r      : 1;
    Bit swizzle_y_g     : 3;
    Bit negate_y_g      : 1;
    Bit swizzle_z_b     : 3;
    Bit negate_z_b      : 1;
    Bit swizzle_w_a     : 3;
    Bit negate_w_a      : 1;
    Bit invert          : 1;
    Bit bias            : 1;
    Bit x2              : 1;
    Bit sign            : 1;
    Bit abs             : 1;
    Bit divComp         : 3;
    Bit clamp           : 1;
    Bit reserved        : 7;
  #else
    Bit reserved        : 7;
    Bit clamp           : 1;
    Bit divComp         : 3;
    Bit abs             : 1;
    Bit sign            : 1;
    Bit x2              : 1;
    Bit bias            : 1;
    Bit invert          : 1;
    Bit negate_w_a      : 1;
    Bit swizzle_w_a     : 3;
    Bit negate_z_b      : 1;
    Bit swizzle_z_b     : 3;
    Bit negate_y_g      : 1;
    Bit swizzle_y_g     : 3;
    Bit negate_x_r      : 1;
    Bit swizzle_x_r     : 3;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Src_Mod;

typedef union IL_Rel_Addr
{
  struct
  {
  #ifdef qLittleEndian
    Bit address_register : 16;
    Bit loop_relative    : 1;
    Bit component        : 3;
    Bit reserved         : 12;
  #else
    Bit reserved         : 12;
    Bit component        : 3;
    Bit loop_relative    : 1;
    Bit address_register : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Rel_Addr;


typedef union IL_Literal
{
  struct
  {
  #ifdef qLittleEndian
    Bit value            : 32;
  #else
    Bit value            : 32;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Literal;


/********************************************************* 
** Multi-Opcode Instuction Tokens:                      **
** Opcode Token formats used by multiple instructions   **
*********************************************************/
/* Used for DSX and DSY */
typedef union IL_CENTROIDOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code              : 16;
    Bit controls_reserved : 8;
    Bit controls_centroid : 1;
    Bit reserved          : 7;
  #else
    Bit reserved          : 7;
    Bit controls_centroid : 1;
    Bit controls_reserved : 8;
    Bit code              : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_CENTROIDOpCode;

/* Used for INITV and MEMIMPORT */
typedef union IL_ELEMENTOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                    : 16;
    Bit controls_element        : 6;
    Bit reserved                : 10;
  #else
    Bit reserved                : 10;
    Bit controls_element        : 6;
    Bit code                    : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_ELEMENTOpCode;

/* Used for MMUL, DET, and TRANSPOSE */
typedef union IL_MATRIXOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_matrix       : 3;
    Bit reserved              : 13;
  #else
    Bit reserved              : 13;
    Bit controls_matrix       : 3;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_MATRIXOpCode;

/* Used for IFC, CONTINUEC, BREAKC, and SET */
typedef union IL_RELOPOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_relop        : 3;
    Bit reserved              : 13;
  #else
    Bit reserved              : 13;
    Bit controls_relop        : 3;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_RELOPOpCode;

/* Used for LOD, PROJECT, and TEXWEIGHT */
typedef union IL_STAGEOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code              : 16;
    Bit controls_stage    : 8;
    Bit reserved          : 8;
  #else
    Bit reserved          : 8;
    Bit controls_stage    : 8;
    Bit code              : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_STAGEOpCode;

/* Used for TEXLD and TEXLDD */
typedef union IL_TEXLDOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_stage       : 8;
    Bit controls_centroid    : 1;
    Bit controls_reserved    : 1;
    Bit controls_shadowmode  : 2;
    Bit reserved             : 2;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit reserved             : 2;
    Bit controls_shadowmode  : 2;
    Bit controls_reserved    : 1;
    Bit controls_centroid    : 1;
    Bit controls_stage       : 8;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_TEXLDOpCode;

/* Used for SAMPLE_C and SAMPLE_C_LZ */
typedef union IL_SAMPLE_COpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_resource    : 8;
    Bit controls_sampler     : 4;
    Bit reserved             : 3;
    Bit sec_modifier_present : 1;
  #else
    Bit sec_modifier_present : 1;
    Bit reserved             : 3;
    Bit controls_sampler     : 4;
    Bit controls_resource    : 8;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_SAMPLE_COpCode;

/* Used for DCLVOUT and DCLPIN */
typedef union IL_USAGEUSAGEINDEXOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_usage       : 5;
    Bit controls_usageindex  : 8;
    Bit reserved             : 2;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit reserved             : 2;
    Bit controls_usageindex  : 8;
    Bit controls_usage       : 5;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_USAGEUSAGEINDEXOpCode;


/* Used for DIV, LN, LOG, LOGP, RCP, and RSQ */
typedef union IL_ZEROOPOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_zeroop       : 2;
    Bit reserved              : 14;
  #else
    Bit reserved              : 14;
    Bit controls_zeroop       : 2;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_ZEROOPOpCode;


/********************************************************* 
** Single-Opcode Instruction Tokens:                    **
** OpCode tokens used for a single instruction          **
*********************************************************/ 
typedef union IL_NRMOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_zeroop       : 2;
    Bit controls_nrm4         : 1;
    Bit reserved              : 13;
  #else
    Bit reserved              : 13;
    Bit controls_nrm4         : 1;
    Bit controls_zeroop       : 2;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_NRMOpCode;

typedef union IL_CMPOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_relop        : 3;
    Bit controls_reserved     : 3;
    Bit controls_cmpval       : 3;
    Bit reserved              : 7;
  #else
    Bit reserved              : 7;
    Bit controls_cmpval       : 3;
    Bit controls_reserved     : 3;
    Bit controls_relop        : 3;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_CMPOpCode;

typedef union IL_DCLDEFOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_xdefault    : 2;
    Bit controls_ydefault    : 2;
    Bit controls_zdefault    : 2;
    Bit controls_wdefault    : 2;
    Bit reserved             : 16;
  #else
    Bit reserved             : 16;
    Bit controls_wdefault    : 2;
    Bit controls_zdefault    : 2;
    Bit controls_ydefault    : 2;
    Bit controls_xdefault    : 2;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_DCLDEFOpCode;

typedef union IL_DCLPIOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_ximport     : 2;
    Bit controls_yimport     : 2;
    Bit controls_zimport     : 2;
    Bit controls_wimport     : 2;
    Bit controls_center      : 1;
    Bit controls_bias        : 1;
    Bit controls_invert      : 1;
    Bit controls_centered    : 1;
    Bit reserved             : 4;
  #else
    Bit reserved             : 4;
    Bit controls_centered    : 1;
    Bit controls_invert      : 1;
    Bit controls_bias        : 1;
    Bit controls_center      : 1;
    Bit controls_wimport     : 2;
    Bit controls_zimport     : 2;
    Bit controls_yimport     : 2;
    Bit controls_ximport     : 2;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_DCLPIOpCode;

typedef union IL_DCLPPOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_param       : 8;
    Bit reserved             : 8;
  #else
    Bit reserved             : 8;
    Bit controls_param       : 8;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_DCLPPOpCode;

typedef union IL_DCLPTOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_stage        : 8;
    Bit controls_type         : 3;
    Bit controls_coordmode    : 2;
    Bit cleartype_mode        : 1;
    Bit reserved              : 2;
  #else
    Bit reserved              : 2;
    Bit cleartype_mode        : 1;    
    Bit controls_coordmode    : 2;
    Bit controls_type         : 3;
    Bit controls_stage        : 8;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_DCLPTOpCode;

typedef union IL_DCLResourceOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_id           : 8;
    Bit controls_type         : 4;
    Bit reserved              : 3;
    Bit controls_unnormalize    : 1;  // 0 is normalized, 1 is un-normalized
  #else
    Bit controls_unnormalize    : 1;    
    Bit reserved              : 3;
    Bit controls_type         : 4;
    Bit controls_id           : 8;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_DCLResourceOpCode;

typedef union IL_DCLVOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_element      : 6;
    Bit reserved              : 9;
    Bit pri_modifier_present  : 1;
  #else
    Bit pri_modifier_present  : 1;
    Bit reserved              : 9;
    Bit controls_element      : 6;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_DCLVOpCode;

typedef union IL_KILLOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_stage        : 8;
    Bit controls_sample       : 1;
    Bit reserved              : 7;
  #else
    Bit reserved              : 7;
    Bit controls_sample       : 1;
    Bit controls_stage        : 8;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_KILLOpCode;

typedef union IL_MEMEXPORTOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                    : 16;
    Bit controls_export_stream  : 6;
    Bit controls_state_stride   : 1;
    Bit controls_elem_offset    : 6;
    Bit reserved                : 3;
  #else
    Bit reserved                : 3;
    Bit controls_elem_offset    : 6;
    Bit controls_state_stride   : 1;
    Bit controls_export_stream  : 6;
    Bit code                    : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_MEMEXPORTOpCode;

typedef union IL_MOVAOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_round        : 1;
    Bit reserved              : 15;
  #else
    Bit reserved              : 15;
    Bit controls_round        : 1;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_MOVAOpCode;

typedef union IL_REFLECTOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                  : 16;
    Bit controls_normalize    : 1;
    Bit reserved              : 15;
  #else
    Bit reserved              : 15;
    Bit controls_normalize    : 1;
    Bit code                  : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_REFLECTOpCode;

typedef union IL_TEXLDBOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_stage       : 8;
    Bit controls_centroid    : 1;
    Bit controls_absolute    : 1;
    Bit controls_shadowmode  : 2;
    Bit reserved             : 2;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit reserved             : 2;
    Bit controls_shadowmode  : 2;
    Bit controls_absolute    : 1;
    Bit controls_centroid    : 1;
    Bit controls_stage       : 8;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_TEXLDBOpCode;

typedef union IL_TEXLDMSOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_stage       : 8;
    Bit reserved             : 6;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit reserved             : 6;
    Bit controls_stage       : 8;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_TEXLDMSOpCode;

typedef union IL_NOISEOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code           : 16;
    Bit controls_noise : 4;
    Bit reserved       : 12;
  #else
    Bit reserved       : 12;
    Bit controls_noise : 4;
    Bit code           : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_NOISEOpCode;

typedef union IL_ResInfoOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_id          : 12;
    Bit controls_uint        : 1;
    Bit reserved             : 1;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit reserved             : 1;
    Bit controls_uint        : 1;
    Bit controls_id          : 12;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_ResInfoOpCode;

typedef union IL_LoadOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_id          : 13;
    Bit addroff_present      : 1;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit addroff_present      : 1;
    Bit controls_id          : 13;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_LoadOpCode;

typedef union IL_SampleOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit resource_id          : 8;
    Bit sampler_id           : 5;
    Bit addroff_present      : 1;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit addroff_present      : 1;
    Bit sampler_id           : 5;
    Bit resource_id          : 8;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_SampleOpCode;


// used for both sampleinfo and samplepos
typedef union IL_SampleInfo
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit resource_id          : 8;
    Bit rasterizer           : 1;
    Bit returns_int         : 1;
  #else
    Bit returns_int         : 1;
    Bit rasterizer           : 1;    
    Bit resource_id          : 8;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_SampleInfo;


typedef union IL_DclInput
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit usage                : 5;
    Bit interp_mode          : 3;
    Bit controls             : 6;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit controls             : 8;
    Bit interp_mode          : 3;
    Bit usage                : 5;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_DclInput;

/* used for R7xx lds read and write */
typedef union IL_LdsReadVecOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                    : 16;
    Bit sharing_mode            : 2;
    Bit neighbor_exch           : 1;
    Bit reserved                : 13;
  #else
    Bit reserved                : 13;
    Bit neighbor_exch           : 1;      
    Bit sharing_mode            : 2;
    Bit code                    : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_LdsReadVecOpCode;

typedef union IL_LdsWriteVecOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                    : 16;
    Bit sharing_mode            : 2;
    Bit lOffset                 : 6;
    Bit reserved                : 8;
  #else
    Bit reserved                : 8;
    Bit lOffset                 : 6;
    Bit sharing_mode            : 2;
    Bit code                    : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_LdsWriteVecOpCode;

typedef union IL_FenceOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                    : 16;
    Bit threads                 : 1;
    Bit lds                     : 1;
    Bit memory                  : 1;
    Bit sr                      : 1;      
    Bit reserved                : 12;
  #else
    Bit reserved                : 12;
    Bit sr                      : 1;            
    Bit memory                  : 1;
    Bit lds                     : 1;
    Bit threads                 : 1;
    Bit code                    : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_FenceOpCode;

/*********************************************************
** Specialized Opcode Argument tokens                   **
*********************************************************/
typedef union IL_Label
{
  struct
  {
  #ifdef qLittleEndian
    Bit label    : 16;
    Bit reserved : 16;
  #else
    Bit reserved : 16;
    Bit label    : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Label;

typedef union IL_Precomp_Descriptor
{
  struct
  {
  #ifdef qLittleEndian
    Bit length   : 16;    
    Bit reserved : 16;
  #else
    Bit reserved : 16;
    Bit length   : 16;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Precomp_Descriptor;

typedef union IL_Comment_Descriptor
{
  struct
  {
  #ifdef qLittleEndian
    Bit length   : 16;    
    Bit reserved : 16;
  #else
    Bit reserved : 16;
    Bit length   : 16;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_Comment_Descriptor;

typedef union IL_ElementFormat
{
  struct
  {
  #ifdef qLittleEndian
    Bit reserved : 20;
    Bit fmt_x    : 3;
    Bit fmt_y    : 3;
    Bit fmt_z    : 3;
    Bit fmt_w    : 3;
  #else
    Bit fmt_w    : 3;
    Bit fmt_z    : 3;
    Bit fmt_y    : 3;
    Bit fmt_x    : 3;
    Bit reserved : 20;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_ElementFormat;

/*********************************************************
** Instruction Modifier Tokens                          **
*********************************************************/
typedef union IL_PrimaryTEXLD_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit mag      : 3;    
    Bit min      : 3;
    Bit volmag   : 3;
    Bit volmin   : 3;
    Bit mip      : 3;
    Bit aniso    : 3;
    Bit lodbias  : 7;
    Bit reserved : 7;
  #else
    Bit reserved : 7;
    Bit lodbias  : 7;
    Bit aniso    : 3;
    Bit mip      : 3;
    Bit volmin   : 3;
    Bit volmag   : 3;
    Bit min      : 3;
    Bit mag      : 3;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_PrimaryTEXLD_Mod;

typedef union IL_SecondaryTEXLD_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit xoffset  : 8;    
    Bit yoffset  : 8;    
    Bit zoffset  : 8;    
    Bit reserved : 8;
  #else
    Bit reserved : 8;
    Bit zoffset  : 8;    
    Bit yoffset  : 8;    
    Bit xoffset  : 8;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_SecondaryTEXLD_Mod;


typedef union IL_SecondarySAMPLE_C_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit xoffset  : 8;    
    Bit yoffset  : 8;    
    Bit zoffset  : 8;    
    Bit reserved : 8;
  #else
    Bit reserved : 8;
    Bit zoffset  : 8;    
    Bit yoffset  : 8;    
    Bit xoffset  : 8;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_SecondarySAMPLE_C_Mod;


typedef union IL_PrimaryTEXLDB_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit mag         : 3;    
    Bit min         : 3;
    Bit volmag      : 3;
    Bit volmin      : 3;
    Bit mip         : 3;
    Bit aniso       : 3;
    Bit lodbias     : 7;
    Bit qualitybias : 1;
    Bit reserved    : 6;
  #else
    Bit reserved    : 6;
    Bit qualitybias : 1;
    Bit lodbias     : 7;
    Bit aniso       : 3;
    Bit mip         : 3;
    Bit volmin      : 3;
    Bit volmag      : 3;
    Bit min         : 3;
    Bit mag         : 3;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_PrimaryTEXLDB_Mod;

typedef union IL_PrimaryTEXLDMS_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit mag       : 3;    
    Bit min       : 3;
    Bit reserved1 : 9;
    Bit aniso     : 3;
    Bit reserved2 : 14;
  #else
    Bit reserved2 : 14;
    Bit aniso     : 3;
    Bit reserved1 : 9;
    Bit min       : 3;
    Bit mag       : 3;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_PrimaryTEXLDMS_Mod;

typedef union IL_SecondaryTEXLDMS_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit xoffset      : 8;    
    Bit yoffset      : 8;    
    Bit reserved1    : 8;    
    Bit sample       : 5;
    Bit reserved2    : 3;  
  #else
    Bit reserved2    : 3;  
    Bit sample       : 5;
    Bit reserved1    : 8;    
    Bit yoffset      : 8;    
    Bit xoffset      : 8;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_SecondaryTEXLDMS_Mod;

typedef union IL_PrimaryDCLVOUT_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit xexport      : 2;    
    Bit yexport      : 2;    
    Bit zexport      : 2;    
    Bit wexport      : 2;
    Bit reserved     : 24;  
  #else
    Bit reserved     : 24;  
    Bit wexport      : 2;
    Bit zexport      : 2;    
    Bit yexport      : 2;    
    Bit xexport      : 2;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
}IL_PrimaryDCLVOUT_Mod;

typedef union IL_PrimaryDCLPIN_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit ximport      : 2;    
    Bit yimport      : 2;    
    Bit zimport      : 2;    
    Bit wimport      : 2;
    Bit centroid     : 1;
    Bit constant     : 1;
    Bit no_persp     : 1;
    Bit reserved     : 21;  
  #else
    Bit reserved     : 21;
    Bit no_persp     : 1;
    Bit constant     : 1;
    Bit centroid     : 1;
    Bit wimport      : 2;
    Bit zimport      : 2;    
    Bit yimport      : 2;    
    Bit ximport      : 2;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
}IL_PrimaryDCLPIN_Mod;

typedef union IL_PrimaryDCLV_Mod
{
  struct
  {
  #ifdef qLittleEndian
    Bit ximport      : 2;    
    Bit yimport      : 2;    
    Bit zimport      : 2;    
    Bit wimport      : 2;
    Bit reserved     : 24;  
  #else
    Bit reserved     : 24;  
    Bit wimport      : 2;
    Bit zimport      : 2;    
    Bit yimport      : 2;    
    Bit ximport      : 2;    
  #endif
  } bits, bitfield;
  unsigned int u32All;
}IL_PrimaryDCLV_Mod;

typedef union IL_LOOPOpCode
{
  struct
  {
  #ifdef qLittleEndian
    Bit code                 : 16;
    Bit controls_rep         : 1;
    Bit controls_reserved    : 12;
    Bit reserved             : 1;
    Bit sec_modifier_present : 1;
    Bit pri_modifier_present : 1;
  #else
    Bit pri_modifier_present : 1;
    Bit sec_modifier_present : 1;
    Bit reserved             : 1;
    Bit controls_reserved    : 12;
    Bit controls_rep         : 1;
    Bit code                 : 16;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_LOOPOpCode;

typedef union IL_ReturnType
{
  struct
  {
  #ifdef qLittleEndian
    Bit reserved             : 20;
    Bit fmt_x                : 3;
    Bit fmt_y                : 3;
    Bit fmt_z                : 3;
    Bit fmt_w                : 3;
  #else
    Bit fmt_w                : 3;
    Bit fmt_z                : 3;
    Bit fmt_y                : 3;
    Bit fmt_x                : 3;
    Bit reserved             : 20;
  #endif
  } bits, bitfield;
  unsigned int u32All;
} IL_ReturnType;

#endif /*__ATI_ILFORMAT_H__*/
