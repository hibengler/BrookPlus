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

#ifndef _AMU_ABI_H_
#define _AMU_ABI_H_

///
///  @file  amuABI.h
///  @brief Interfaces Exported by ATI ABI Utility Library
///
#ifdef __cplusplus
extern "C" {
#define CALAPI
#else
#define CALAPI extern
#endif

#ifdef _WIN32
#define CALAPIENTRY  __stdcall
#else
#define CALAPIENTRY
#endif

#define CAL_USE_SC_PRM 1

//
//  ELF Specific Definitions
//
#define ELFOSABI_CALIMAGE          100 

#define EM_AMD_CALIMAGE_BINARY     125

#define EF_AMD_CALIMAGE_BINARY     1

enum CALMachineType {
    ED_AMD_CAL_MACHINE_R600_ISA = 0,
    ED_AMD_CAL_MACHINE_R610_ISA = 1,
    ED_AMD_CAL_MACHINE_R630_ISA = 2,
    ED_AMD_CAL_MACHINE_R670_ISA = 3,
    ED_AMD_CAL_MACHINE_R770_ISA = 4,
    ED_AMD_CAL_MACHINE_R6XX_IL  = 5,
    ED_AMD_CAL_MACHINE_R7XX_IL  = 6,
    ED_AMD_CAL_MACHINE_R6XX_IL_TEXT = 7,
    ED_AMD_CAL_MACHINE_R7XX_IL_TEXT = 8,
    ED_AMD_CAL_MACHINE_R710_ISA = 11,
    ED_AMD_CAL_MACHINE_R730_ISA = 12,
    ED_AMD_CAL_MACHINE_LAST     = 13
};

#define ED_AMD_CAL_TYPE_FRAGMENT    0
#define ED_AMD_CAL_TYPE_VERTEX      1
#define ED_AMD_CAL_TYPE_GEOMETRY    2
#define ED_AMD_CAL_TYPE_FETCH       3

#define ELF_NOTE_AMD                    "AMD CAL"
#define ELF_NOTE_AMD_PROGINFO           1
#define ELF_NOTE_AMD_INPUTS             2
#define ELF_NOTE_AMD_OUTPUTS            3
#define ELF_NOTE_AMD_CONDOUT            4
#define ELF_NOTE_AMD_FLOAT32CONSTS      5
#define ELF_NOTE_AMD_INT32CONSTS        6
#define ELF_NOTE_AMD_BOOL32CONSTS       7
#define ELF_NOTE_AMD_EARLYEXIT          8
#define ELF_NOTE_AMD_GLOBAL_BUFFERS     9
#define ELF_NOTE_AMD_CONSTANT_BUFFERS   10
#define ELF_NOTE_AMD_INPUT_SAMPLER_MAPS 11
#define ELF_NOTE_AMD_PERSISTENT_BUFFERS 12
#define ELF_NOTE_AMD_SCRATCH_BUFFERS    13
#define ELF_NOTE_AMD_SUB_CONSTANT_BUFFERS 14

#define ELF_STT_AMD_LOPROC         13
#define ELF_STB_AMD_LOCAL          0
#define ELF_STB_AMD_GLOBAL         1
#define ELF_ST_AMD_INFO(bind, type)   (((bind) << 4) + ((type) & 0xf))

#define ELF_SYM_AMD_FLOAT32        ELF_ST_AMD_INFO(ELF_STB_AMD_GLOBAL, (ELF_STT_AMD_LOPROC+0))
#define ELF_SYM_AMD_INT32          ELF_ST_AMD_INFO(ELF_STB_AMD_GLOBAL, (ELF_STT_AMD_LOPROC+1))
#define ELF_SYM_AMD_BOOL32         ELF_ST_AMD_INFO(ELF_STB_AMD_GLOBAL, (ELF_STT_AMD_LOPROC+2))
#define ELF_SYM_AMD_SAMPLER        ELF_ST_AMD_INFO(ELF_STB_AMD_GLOBAL, (ELF_STT_AMD_LOPROC+3))

//
///  ABI Fixed Sizes for Extract Interface
//
const unsigned int AMUabiMaxUserConsts = 256;        ///< maximum number of user constants
const unsigned int AMUabiMaxLiteralConsts = 256;     ///< maximum number of literal constants
const unsigned int AMUabiMaxSamplerNames = 16;       ///< maximum number of user samplers
const unsigned int AMUabiMaxFixedStringLength = 256; ///< maximum length of constant or sampler names
const unsigned int AMUabiMaxConstantBuffers = 16;    ///< maximum number of constant buffers

//
///  ABI Datatypes For Constants
//
enum AMUabiDataType {
    AMU_ABI_UNDEF,                  ///< undefined
    AMU_ABI_BOOL32,                 ///< 32bit boolean (stored as unsigned int)
    AMU_ABI_INT32,                  ///< 32bit integer
    AMU_ABI_FLOAT32,                ///< 32bit floating point
    AMU_ABI_SAMPLER,                ///< sampler
    AMU_ABI_MAXTYPES = 255          ///< number of datatypes
};

//
///  ABI program info keys 
//
enum AMUabiProgInfoSpecial
{
    AMU_ABI_PS_INPUT_COUNT          = 0x80000000, ///< Number of PS inputs
    AMU_ABI_PS_INPUT_SPARSE         = 0x80000001, ///< Are PS inputs sparse or not
    AMU_ABI_VS_OUTPUT_COUNT         = 0x8000000c, ///< Number of VS outputs
    AMU_ABI_VS_OUTPUT_SPARSE        = 0x8000000d, ///< Are vs outputs sparse
};

//
/// ABI encoding dictionary entry
//
typedef struct CALEncodingDictionaryEntryRec 
{
    unsigned int d_machine;          ///< Machine identifier for the encoding (ED_AMD_CAL_MACHINE_R600_ISA, etc.)
    unsigned int d_type;             ///< Type identifier of the encoding (ED_AMD_CAL_TYPE_FRAGMENT, etc.)
    unsigned int d_offset;           ///< Offset of the encoding's note and load segments in the ELF
    unsigned int d_size;             ///< Size of the encoding's note and load segments in the ELF
    unsigned int d_flags;            ///< Flags (unused)
} 
CALEncodingDictionaryEntry;

//
/// ABI data segment descriptor
//
typedef struct CALDataSegmentDescRec
{
    unsigned int offset;             ///< Offset in bytes to the start of the data
    unsigned int size;               ///< Size in bytes of the data
}
CALDataSegmentDesc;

//
/// ABI constant buffer descriptor
//
typedef struct CALConstantBufferMaskRec
{
    unsigned int index;              ///< Costant buffer identifier
    unsigned int size;               ///< Number of vec4f entries in the constant buffer
}
CALConstantBufferMask;

typedef struct CALSubConstantBufferRec
{
    unsigned int index;             ///< Constant buffer identifier
    unsigned int size;              ///< size of this subset of the constant buffer
    unsigned int offset;            ///< offset of this subset within the constant buffer
}
CALSubConstantBufferEntry;

//
/// ABI sampler descriptor
//
typedef struct CALSamplerMapEntryRec
{
    unsigned int input;              ///< Input/resource identifier
    unsigned int sampler;            ///< Sampler identifier
}
CALSamplerMapEntry;

//
/// ABI program info entry
//
typedef struct CALProgramInfoEntryRec
{
    unsigned int address;            ///< Device address/key
    unsigned int value;              ///< Value
} 
CALProgramInfoEntry;


//
///  ABI Constants Data Section
//
typedef struct AMUabiConstsDataRec
{
    float float32[ 256 * 4 ];
    unsigned int int32[ 32 * 4 ];
    unsigned int bool32[ 32 ];
}
AMUabiConstsData;

//
///  ABI Symbol Table Entry
//
typedef struct AMUabiSymbolRec
{
    unsigned int name;
    unsigned int value;
    unsigned int size;
    unsigned char info;
    unsigned char other ;
    unsigned short section;
}
AMUabiSymbol;

//
///  ABI Fixed Length String
//
typedef struct AMUabiFixedStringRec
{
    char str[ AMUabiMaxFixedStringLength ]; ///< fixed length NULL-terminated string
}
AMUabiFixedString;

//
///  ABI Constant Value Storage
//
typedef union AMUabiDataValueRec {
    float float32[ 4 ];
    unsigned int int32[ 4 ];
    unsigned int bool32[ 1 ];
} AMUabiDataValue;

//
///  ABI Literal Constant
//
typedef struct AMUabiLiteralConstRec
{
    unsigned int addr;
    AMUabiDataType type;
    AMUabiDataValue value;
}
AMUabiLiteralConst;

//
///  ABI User Constant
//
typedef struct AMUabiUserConstRec
{
    unsigned int addr;
    AMUabiDataType type;
    AMUabiFixedString name;
}
AMUabiUserConst;

//
/// ABI User Symbol
//
typedef struct AMUabiUserSymbolRec
{
    char const* name;
    unsigned int value;
    unsigned int section;
}
AMUabiUserSymbol;


typedef struct AMUabiMultiBinaryRec {}* AMUabiMultiBinary;
typedef struct AMUabiEncodingRec {}* AMUabiEncoding;

//////////////////////////////////////////////////////////////////////////
//
// Creation and destruction
//

/**
 * @fn amuABIMultiBinaryCreate(AMUabiMultiBinary* binary)
 *
 * @brief Create a new AMUabiMultiBinary object.
 *
 * Creates a new AMUabiMultiBinary object.
 *
 * @param binary (out)   - Pointer to created multibinary object handle.
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryDestroy
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryCreate(AMUabiMultiBinary* binary);

/**
 * @fn amuABIMultiBinaryDestroy(AMUabiMultiBinary binary)
 *
 * @brief Destroy a AMUabiMultiBinary object.
 *
 * Destroys the supplied AMUabiMultiBinary object.
 *
 * @param binary (in)   - Multibinary object to destroy.
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryCreate
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryDestroy(AMUabiMultiBinary binary);

//////////////////////////////////////////////////////////////////////////
//
// Adding encodings 
//

/**
 * @fn amuABIMultiBinaryAddEncoding(AMUabiMultiBinary binary,
 *                                  unsigned int machine, unsigned int type,
 *                                  unsigned int inputCount, unsigned int* inputs,
 *                                  unsigned int outputCount, unsigned int* outputs,
 *                                  unsigned int condOut, unsigned int earlyExit,
 *                                  unsigned int globalBuffersCount, unsigned int* globalBuffers,
 *                                  unsigned int scratchBuffersCount, unsigned int* scratchBuffers,
 *                                  unsigned int persistentBuffersCount, unsigned int* persistentBuffers,
 *                                  unsigned int litConstsCount, AMUabiLiteralConst* litConsts,
 *                                  unsigned int constBuffersCount, CALConstantBufferMask* constBuffers,
 *                                  unsigned int inputSamplerMapCount, CALSamplerMapEntry* inputSamplerMaps,
 *                                  unsigned int progInfosCount, CALProgramInfoEntry* progInfos,
 *                                  unsigned int textSize, void* textData,
 *                                  unsigned int symbolsCount, AMUabiUserSymbol* symbols
 *                                  );
 *
 * @brief Add a new encoding to a multibinary.
 *
 * Adds a new encoding with the specified properties to the supplied AMUabiMultiBinary object.  It
 * should be noted that array arguments are not copied when creating the new encoding.  Only the 
 * pointers are stored in the encoding so it is the responsibility of the caller to not free the
 * associated memory until the multibinary object is destroyed.
 *
 * @param binary (out) - Multibinary object to edit
 * @param machine (in) - Machine identifier for the encoding
 * @param type (in) - Type identifier for the encoding
 * @param inputCount (in) - Number of entries in the inputs array
 * @param inputs (in) - Input data
 * @param outputCount (in) - Number of entries in the outputs array
 * @param outputs (in) - Output data
 * @param condOut (in) - Does the program support conditional output
 * @param earlyExit (in) - Does the program support early exit
 * @param globalBuffersCount (in) - Number of entries in the global buffers array
 * @param globalBuffers (in) - Global buffer data
 * @param scratchBuffersCount (in) - Number of entries in the scratch buffers array
 * @param scratchBuffers (in) - Scratch buffer data
 * @param persistentBuffersCount (in) - Number of entries in the persistent buffers array
 * @param persistentBuffers (in) - Persistent buffer data
 * @param litConstsCount (in) - Number of literal constants
 * @param litConsts (in) - Literal constant values
 * @param constBuffersCount (in) - Number of constant buffers
 * @param constBuffers (in) - Constant buffer data
 * @param inputSamplerMapCount (in) - Number of input samplers
 * @param inputSamplerMaps (in) - Input sampler data
 * @param progInfosCount (in) - Number of program info entries
 * @param progInfos (in) - Program info table
 * @param textSize (in) - Size of the compiled program data
 * @param textData (in) - Compiled program data
 * @param symbolsCount (in) - Number of symbols
 * @param symbols (in) - Symbol data
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryAddAMUEncoding
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryAddEncoding(
        AMUabiMultiBinary binary,
        unsigned int machine, unsigned int type,
        unsigned int inputCount, unsigned int* inputs,
        unsigned int outputCount, unsigned int* outputs,
        unsigned int condOut, unsigned int earlyExit,
        unsigned int globalBuffersCount, unsigned int* globalBuffers,
        unsigned int scratchBuffersCount, unsigned int* scratchBuffers,
        unsigned int persistentBuffersCount, unsigned int* persistentBuffers,
        unsigned int litConstsCount, AMUabiLiteralConst* litConsts,
        unsigned int constBuffersCount, CALConstantBufferMask* constBuffers,
        unsigned int inputSamplerMapCount, CALSamplerMapEntry* inputSamplerMaps,
        unsigned int progInfosCount, CALProgramInfoEntry* progInfos,
        unsigned int textSize, void* textData,
        unsigned int symbolsCount, AMUabiUserSymbol* symbols
        );

/**
 * @fn amuABIMultiBinaryAddSubConstantBuffers( AMUabiMultiBinary binary, unsigne
 *                                             unsigned int machine, unsigned int type,
 *                                             unsigned int subBufferCount, CALSubConstantBufferEntry* subBuffers
 *                                            );
 *
 * @brief Adds SubConstantBuffer entries to an already existing encoding within a MultiBinary
 *
 * Adds SubConstantBuffer entries to an already existing encoding within a Multibinary. The encoding
 * must have already been added using amuABIMultiBinaryAddEncoding. Similar restrictions as to freeing
 * of the data apply.
 *
 * @param binary (out)   - MultiBinary object to edit
 * @param machine (in)  - Machine identifier for the encoding to effect
 * @param type (in)     - Type identifier for the encoding to effect
 * @param subBufferCount (in) - Number of Sub Constant Buffers present
 * @param subBuffers (in) - Sub Constant Buffer entries
 *
 * @return Returns 1 on success, 0 on error
 *
 * @sa amuABIMultiBinaryAddEncoding
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryAddSubConstantBuffers(
        AMUabiMultiBinary binary,
        unsigned int machine, unsigned int type,
        unsigned int subBufferCount, CALSubConstantBufferEntry* subBuffers
        );

/**
 * @fn amuABIMultiBinaryAddAMUEncoding( AMUabiMultiBinary binary, AMUabiEncoding encoding)
 *
 * @brief Add an encoding from one multibinary to another.
 *
 * Copy the values stored in encoding to a new encoding.  Generally this is used for
 * copying an encoding from one multibinary object to another but you could use it for
 * copying encodings within a multibinary object.
 *
 * @param binary (out)   - Multibinary object to edit
 * @param encoding (in) - Encoding to copy
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryAddEncoding
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryAddAMUEncoding( AMUabiMultiBinary binary, AMUabiEncoding encoding);

//////////////////////////////////////////////////////////////////////////
//
// Extracting encodings
//

/**
 * @fn amuABIMultiBinaryGetEncodingCount(unsigned int* encodingCount, AMUabiMultiBinary binary)
 *
 * @brief Query the number of encodings in a AMUabiMultiBinary object.
 *
 * Query the number of encodings contained by the supplied AMUabiMultiBinary object.
 *
 * @param encodingCount (out) - Return value
 * @param binary (in)   - Multibinary object to query.
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryGetEncoding amuABIMultiBinaryFindEncoding
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryGetEncodingCount(unsigned int* encodingCount, AMUabiMultiBinary binary);

/**
 * @fn amuABIMultiBinaryGetEncoding(AMUabiEncoding* encoding, AMUabiMultiBinary binary, unsigned int index)
 *
 * @brief Retrieve an encoding from an AMUabiMultiBinary object by index.
 *
 * Retrieve the indexed encoding from the supplied multibinary.
 *
 * @param encoding (out) - Return value
 * @param binary (in)   - Multibinary object to query.
 * @param index (in) - Index of the encoding to return
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryGetEncodingCount amuABIMultiBinaryFindEncoding
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryGetEncoding(AMUabiEncoding* encoding, AMUabiMultiBinary binary, unsigned int index);

/**
 * @fn amuABIMultiBinaryFindEncoding(AMUabiEncoding* encoding, AMUabiMultiBinary binary, unsigned int machine, unsigned int type)
 *
 * @brief Retrieve an encoding from an AMUabiMultiBinary object by machine and type signature.
 *
 * Retrieve an encoding from the supplied multibinary by matching machine and type values.
 * This implies that it is illegal to have a multibinary containing multiple encodings with
 * the same signature as there would be no way to differentiate them.
 *
 * @param encoding (out) - Return value
 * @param binary (in)   - Multibinary object to query.
 * @param machine (in) - Machine indentifier of the encoding to return
 * @param type (in) - Type identifier of the encoding to return
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryGetEncodingCount amuABIMultiBinaryGetEncoding
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryFindEncoding(AMUabiEncoding* encoding, AMUabiMultiBinary binary, unsigned int machine, unsigned int type);

/**
 * @fn amuABIEncodingGetSignature(unsigned int* machine, unsigned int* type, AMUabiEncoding encoding)
 *
 * @brief Retrieve the machine and type signature values from the specified encoding.
 *
 * Retrieve the machine and type signature values from the specified encoding.
 *
 * @param machine (out) - Machine indentifier of the encoding to return
 * @param type (out) - Type identifier of the encoding to return
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetSignature(unsigned int* machine, unsigned int* type, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetInputs(unsigned int* inputCount, unsigned int** inputs, AMUabiEncoding encoding)
 *
 * @brief Retrieve the inputs from an encoding.
 *
 * Retrieve the inputs from the supplied encoding. 
 *
 * @param inputCount (out) - Number of inputs in the encoding
 * @param inputs (out) - Input data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetInputs(unsigned int* inputCount, unsigned int** inputs, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetOutputs(unsigned int* outputCount, unsigned int** outputs, AMUabiEncoding encoding)
 *
 * @brief Retrieve the outputs from an encoding.
 *
 * Retrieve the outputs from the supplied encoding. 
 *
 * @param outputCount (out) - Number of outputs in the encoding
 * @param outputs (out) - Output data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetOutputs(unsigned int* outputCount, unsigned int** outputs, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetCondOut(unsigned int* condOut, AMUabiEncoding encoding)
 *
 * @brief Retrieve the conditional output boolean from an encoding.
 *
 * Retrieve the conditional output boolean from an encoding.
 *
 * @param condOut (out) - Conditional output boolean.
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetCondOut(unsigned int* condOut, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetEarlyExit(unsigned int* earlyExit, AMUabiEncoding encoding)
 *
 * @brief Retrieve the early exit boolean from an encoding.
 *
 * Retrieve the early exit boolean from an encoding.
 *
 * @param earlyExit (out) - Early exit boolean.
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetEarlyExit(unsigned int* earlyExit, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetGlobalBuffers(unsigned int* globalBuffersCount, unsigned int** globalBuffers, AMUabiEncoding encoding)
 *
 * @brief Retrieve the global buffers from an encoding.
 *
 * Retrieve the global buffers from the supplied encoding. 
 *
 * @param globalBuffersCount (out) - Number of global buffers in the encoding
 * @param globalBuffers (out) - Global buffer data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetGlobalBuffers(unsigned int* globalBuffersCount, unsigned int** globalBuffers, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetScratchBuffers(unsigned int* scratchBuffersCount, unsigned int** scratchBuffers, AMUabiEncoding encoding)
 *
 * @brief Retrieve the scratch buffers from an encoding.
 *
 * Retrieve the scratch buffers from the supplied encoding. 
 *
 * @param scratchBuffersCount (out) - Number of scratch buffers in the encoding
 * @param scratchBuffers (out) - Scratch buffer data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetScratchBuffers(unsigned int* scratchBuffersCount, unsigned int** scratchBuffers, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetPersistentBuffers(unsigned int* persistentBuffersCount, unsigned int** persistentBuffers, AMUabiEncoding encoding)
 *
 * @brief Retrieve the persistent buffers from an encoding.
 *
 * Retrieve the persistent buffers from the supplied encoding. 
 *
 * @param persistentBuffersCount (out) - Number of persistent buffers in the encoding
 * @param persistentBuffers (out) - Persistent buffer data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetPersistentBuffers(unsigned int* persistentBuffersCount, unsigned int** persistentBuffers, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetLitConsts(unsigned int* litConstsCount, AMUabiLiteralConst** litConsts, AMUabiEncoding encoding)
 *
 * @brief Retrieve the literal constants from an encoding.
 *
 * Retrieve the literal constants from the supplied encoding. 
 *
 * @param litConstsCount (out) - Number of literal constants in the encoding
 * @param litConsts (out) - Literal constant data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetLitConsts(unsigned int* litConstsCount, AMUabiLiteralConst** litConsts, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetConstBuffers(unsigned int* constBuffersCount, CALConstantBufferMask** constBuffers, AMUabiEncoding encoding)
 *
 * @brief Retrieve the constant buffers from an encoding.
 *
 * Retrieve the constant buffers from the supplied encoding. 
 *
 * @param constBuffersCount (out) - Number of constant buffers in the encoding
 * @param constBuffers (out) - Constant buffer data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetConstBuffers(unsigned int* constBuffersCount, CALConstantBufferMask** constBuffers, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetInputSamplerMaps(unsigned int* inputSamplerMapCount, CALSamplerMapEntry** inputSamplerMaps, AMUabiEncoding encoding)
 *
 * @brief Retrieve the input sampler maps from an encoding.
 *
 * Retrieve the input sampler maps from the supplied encoding. 
 *
 * @param inputSamplerMapCount (out) - Number of input sampler maps in the encoding
 * @param inputSamplerMaps (out) - Input sampler data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetInputSamplerMaps(unsigned int* inputSamplerMapCount, CALSamplerMapEntry** inputSamplerMaps, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetProgInfos(unsigned int* progInfosCount, CALProgramInfoEntry** progInfos, AMUabiEncoding encoding)
 *
 * @brief Retrieve the program info entries from an encoding.
 *
 * Retrieve the program info entries from the supplied encoding. 
 *
 * @param progInfosCount (out) - Number of program info entries in the encoding
 * @param progInfos (out) - Program info data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetProgInfos(unsigned int* progInfosCount, CALProgramInfoEntry** progInfos, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetText(unsigned int* textSize, void** textData, AMUabiEncoding encoding)
 *
 * @brief Retrieve the program text from an encoding.
 *
 * Retrieve the program text from the supplied encoding. 
 *
 * @param textSize (out) - Size of the program text
 * @param textData (out) - Program text
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetText(unsigned int* textSize, void** textData, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetSymbols(unsigned int* symbolsCount, AMUabiUserSymbol** symbols, AMUabiEncoding encoding)
 *
 * @brief Retrieve the symbols from an encoding.
 *
 * Retrieve the symbols the supplied encoding. 
 *
 * @param symbolsCount (out) - Number of symbols in the encoding
 * @param symbols (out) - Symbol data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetSymbols(unsigned int* symbolsCount, AMUabiUserSymbol** symbols, AMUabiEncoding encoding);

/**
 * @fn amuABIEncodingGetSubConstantBuffers(unsigned int* bufferCount, CALSubConstantBufferEntry** subBuffers, AMUabiEncoding encoding)
 *
 * @brief Retrieve the symbols from an encoding.
 *
 * Retrieve the symbols the supplied encoding. 
 *
 * @param bufferCount (out) - Number of symbols in the encoding
 * @param subBuffers (out) - Symbol data 
 * @param encoding (in) - Multibinary to query.
 *
 * @return Returns 1 on success, 0 on error. 
 */
CALAPI unsigned int CALAPIENTRY amuABIEncodingGetSubConstantBuffers(unsigned int* bufferCount, CALSubConstantBufferEntry** subBuffers, AMUabiEncoding encoding);

//////////////////////////////////////////////////////////////////////////
//
// Translation to/from ELF binary
//

/**
 * @fn amuABIMultiBinaryPack(unsigned int* binarySize, void** binaryData, AMUabiMultiBinary binary)
 *
 * @brief Pack a AMUabiMultiBinary object into the ELF binary format.
 *
 * Pack the supplied multibinary object into an ELF binary.  This function allocates
 * a buffer of size binarySize and returns a pointer to it in binaryData.  It is the
 * caller's responsibility to free this buffer once they no longer need it.
 *
 * @param binarySize (out) - Size of the packed ELF binary
 * @param binaryData (out) - Pointer to the buffer containing the packed ELF binary
 * @param binary (in)   - Multibinary object to destroy
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryUnpack
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryPack(unsigned int* binarySize, void** binaryData, AMUabiMultiBinary binary);

/**
 * @fn amuABIMultiBinaryUnpack(AMUabiMultiBinary binary, void* binaryData)
 *
 * @brief Unpack an ELF binary into an AMUabiMultiBinary object.
 *
 * Unpack the ELF binary data stored in binaryData into the multibinary object
 * binary.
 *
 * @param binary (out) - Multibinary object to unpack into
 * @param binaryData (int) - Pointer to the buffer containing the packed ELF binary
 *
 * @return Returns 1 on success, 0 on error. 
 *
 * @sa amuABIMultiBinaryPack
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryUnpack(AMUabiMultiBinary binary, void* binaryData);

//////////////////////////////////////////////////////////////////////////
//
// Misc. utility functions
//

/**
 * @fn amuABIMachineIsISA(unsigned int machine)
 *
 * @brief Determine if a machine identifier corresponds to an ISA encoding.
 *
 * Determine if a machine identifier corresponds to an ISA encoding.
 *
 * @param machine (in) - Machine indentifier to query.
 *
 * @return Returns 1 if the machine idenfitier is an ISA, 0 if not.
 *
 * @sa amuABIMachineIsIL
 */
CALAPI unsigned int CALAPIENTRY amuABIMachineIsISA(unsigned int machine);

/**
 * @fn amuABIMachineIsIL(unsigned int machine)
 *
 * @brief Determine if a machine identifier corresponds to an IL encoding.
 *
 * Determine if a machine identifier corresponds to an IL encoding.
 *
 * @param machine (in) - Machine indentifier to query.
 *
 * @return Returns 1 if the machine idenfitier is an IL, 0 if not.
 *
 * @sa amuABIMachineIsISA
 */
CALAPI unsigned int CALAPIENTRY amuABIMachineIsIL(unsigned int machine);

/**
 * @fn amuABIMultiBinaryGetSize(unsigned int* binarySize, void* binaryData)
 *
 * @brief Determine the size of an ELF binary. 
 *
 * Inspect the headers of the ELF binary stored in binaryData and return the 
 * total size of the binary.  If the data in binaryData doesn't appear to be
 * an ELF then 0 is returned indicating an error.
 *
 * @param binarySize (out) - Return value
 * @param binaryData (in) - Pointer to an ELF binary 
 *
 * @return Returns 1 on success, 0 on error.
 */
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryGetSize(unsigned int* binarySize, void* binaryData);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _AMU_ABI_H_
