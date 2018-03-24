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

#ifndef AMD_SHADERDESCRIPTION_HPP
#define AMD_SHADERDESCRIPTION_HPP

#ifndef HLFRONTENDAPI_NO_RELATIVE_INCLUDES
#include "smartpointer.hpp"
//#include "../GenericUtilities/smartpointer.hpp" earlier was used this
#include "conststring.hpp"
#else
#include "smartpointer.hpp"
#include "conststring.hpp"
#endif

#include <vector>
#include <map>
#include <iostream>

/**
 * Contains metadata about the input/outputs of a compiled shader       
 * Simplified layout is:
 * ShaderDescription
 *   Inputs
 *     Variable
 *       Register
 *       ...
 *     ...
 *   Outputs
 *     Variable
 *       Register
 *       ...
 *     ...
 *   ConstantBuffers
 *     Buffer
 *       Variable
 *       ...
 *     ...
 *   Textures
 *     Texture
 *     ...
 *   Samplers
 *     Sampler
 *     ...
 * Functions
 *   Function
 *   ...
 */
struct ShaderDescription : public RefCountedObj
{
  /**
   * Corresponds to a user semantic declared in the form ": {Name}{Number}" 
   * after a argument on main.
   */
  struct SemanticDesc
  {
    ConstStringPtr Name; //IE: color, sv_target, foo 
    int            Number;    
  };

  /**
   * Corresponds to a datatype normalized to a standardized name
   * Scalar types name will be of the form: (u?int|float|double|bool)
   * Vector types name will be a scalar type name followed by [234]
   * Matrix types name will be a scalar type name followed by [1234]x[234]
           (Nx1 matrix types are automatically converted to vector types)
   * User defined structures may be of any name
   */
  struct DataTypeDesc
  {
    ConstStringPtr Name;
    bool           IsUserDefinedType;
    int            NumArrayElements;       //-1 for non-array types
    int            ElementSizeInRegisters; //Size of this type (Or a single entry if an array)
    //possibly add some enumerations to identify builtin types?
  };

  /**
   * Corresponds to a user defined variable or function argument
   * Name is user provided variable name
   * Note there exists a special Name "return" that is used to store the return
   * value of a function
   */
  struct VariableDesc
  {
    ConstStringPtr Name;
    DataTypeDesc   DataType;
  };

  /**
   * Describes a vN or oN register 
   * If element of ShaderDescription::Inputs register is vN / IL_REGTYPE_INPUT
   * If element of ShaderDescription::Outputs register is oN / IL_REGTYPE_OUTPUT
   * Mask indicates which parts of a register are used by parent variable
   * It is possible for a parts of a single register to belong to two user variables
   */
  struct RegisterDesc
  {
    int          RegisterNumber;  //index for this register
    bool         RegisterMask[4]; //true if a given slot is used    
  };
  typedef std::vector<RegisterDesc> RegisterList;

  /**
   * Represents a single semantic/input or output variable
   * Typically each variable will be assigned a single register/semantic
   * For array, matrix, and struct types more than one register may exist
   * Semantic.Number implicitly increments by one after each register
   * Register numbers must be sequential 
   */
  struct  InputOutputVariableDesc
  {
    VariableDesc Variable;
    SemanticDesc Semantic;
    RegisterList Registers;
    int          ImportUsage; //Value from ILImportUsage enumeration
                              //Only valid for input variables
  };
  typedef std::vector<InputOutputVariableDesc> InputVariableList; 
  typedef std::vector<InputOutputVariableDesc> OutputVariableList; 

  /**
   * Represents a single user variable within a constant buffer array
   * Because buffers are packed, ByteOffset and ByteSize may NOT be aligned to
   * IL registers(16 bytes), however they WILL be aligned to register element 
   * boundaries(4 bytes).  There may be unused gaps in a constant buffer
   * attributed to packing rules.  The packing rules are in short:  If an 
   * element would cross a IL register(16 byte) boundary, skip some space and 
   * start it at that boundary instead.
   */
  struct ConstantBufferFieldDesc
  {
    VariableDesc Variable;
    int          ByteOffset; //offset in buffer in bytes 
    int          ByteSize;   //size in bytes of variable 
  };
  typedef std::vector<ConstantBufferFieldDesc> ConstantBufferFieldList; 

  typedef std::vector<char> ConstantBufferValue;

  /**
   * Represents a single user defined constant buffer declared like:
   * cbuffer Foo { ... global variables ... };
   * All global variable appearing outside a constant buffer are placed in 
   * a virtual constant buffer "$Globals"
   * Variables flagged "static" are omitted from constant buffers
   */
  struct ConstantBufferDesc
  {
    ConstStringPtr          Name; //Either user supplied or "$Globals"
    int                     BufferNumber;
    ConstantBufferFieldList Fields;
    //ConstantBufferValue     InitialValue; //Initial value, if the user provided initializers
                                            //future version?
  };
  typedef std::vector<ConstantBufferDesc> ConstantBufferList;

  /**
   * Represents a texture input to shader
   * If declared like Texture2D<uint2> 
   *       then Variable.DataType will be "Texture2D"
   *       and ElementType will be "uint2"
   * Legacy texture names will be translated to the newer texture types:
   *  Buffer|Texture[123]D(Array)?|TextureCube
   * If not provided by the user, ElementType defaults to float4
   */
  struct TextureDesc
  {
    VariableDesc Variable; 
    DataTypeDesc ElementType;
    int          ResourceNumber;
  };
  typedef std::vector<TextureDesc> TextureList;

  typedef std::map< ConstStringPtr, ConstStringPtr > SamplierAttributeList;

  /**
   * Represents a user defined sampler
   */ 
  struct SamplerDesc
  {
    VariableDesc          Variable;
    int                   SamplerNumber;
    //SamplierAttributeList Attributes; //future version?
  };
  typedef std::vector<SamplerDesc> SamplerList;

  /**
   * Represents a user declared function                                                                    
   * IsEntryPoint is true if this function was the entry point in this 
   *              compilation; true for exactly one entry 
   * NumRegisterArgs is number of args to function in unpacked (16 byte) registers
   *                 note that this includes return value and output args
   * NumRegisterArgsSemanticOrUniform is same as above except only counting
   *                 those args suitable for a entry point of a shader
   */
  struct FunctionDesc
  {
    ConstStringPtr Name;
    int            FunctionNumber; //IL "func N" declaration
    bool           IsEntryPoint;
    int            NumRegisterArgs; 
    int            NumRegisterArgsSemanticOrUniform;
    //possibly add list of args?

    ///
    /// True if this could be used as a shader entry point 
    inline bool IsEntryPointCanidate() const { return NumRegisterArgs==NumRegisterArgsSemanticOrUniform; }
  };
  typedef std::vector<FunctionDesc> FunctionList;

  ///
  /// Serialize to a text semi-human-readable text format
  void SerializeAsText(std::ostream& o) const;
  void SerializeAsText(const char* filename) const;

  ///
  /// Read in a previously serialized file
  void UnserializeFromText(std::istream& i);
  void UnserializeFromText(const char* filename);


  ///
  /// Serialize to a binary format (very little size difference)
  void SerializeAsBinary(std::ostream& o) const;
  void SerializeAsBinary(const char* filename) const;

  ///
  /// Read in a previously serialized file
  void UnserializeFromBinary(std::istream& i);
  void UnserializeFromBinary(const char* filename);

  ///
  /// Constructor
  ShaderDescription() : UsesGlobalStorage(false) {}

  InputVariableList  Inputs;
  OutputVariableList Outputs;
  ConstantBufferList ConstantBuffers;
  TextureList        Textures;
  SamplerList        Samplers;
  FunctionList       Functions;
  bool               UsesGlobalStorage;
};

typedef SmartPtr<ShaderDescription> ShaderDescriptionPtr;
typedef SmartPtr<const ShaderDescription> ConstShaderDescriptionPtr;

#endif

