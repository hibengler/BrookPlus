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

#include <stdlib.h>
#include <string.h>
#include "cal.h"
#include "calcl.h"
#include "calutAMDhlslCompiler.h"
#include <sstream>
#include <string>
#include <assert.h>
#include "amuABIMultiBinary.h"
#include "hlfrontendapi.hpp"

static ErrorState     amdHLSLerrst;

void fill_output(ShaderDescription::OutputVariableList const& c, unsigned int** buffer, unsigned int* bufCount)
{
    *bufCount = (unsigned int)c.size();
    *buffer = (unsigned int*)malloc(*bufCount*sizeof(unsigned int));
    ShaderDescription::OutputVariableList::const_iterator iter;
    int count = 0;
    for(iter = c.begin(); iter != c.end();  ++iter)
    {
        (*buffer)[count++] = iter->Registers.front().RegisterNumber;
    }
}

void fill_tex(ShaderDescription::TextureList const& c, unsigned int** buffer, unsigned int* bufCount)
{
    *bufCount = (unsigned int)c.size();
    *buffer = (unsigned int*)malloc(*bufCount*sizeof(unsigned int));
    ShaderDescription::TextureList::const_iterator iter;
    int count = 0;
    for(iter = c.begin(); iter != c.end();  ++iter)
    {
        (*buffer)[count++] = iter->ResourceNumber;
    }
}

unsigned cbuf_size(ShaderDescription::ConstantBufferFieldList const& fields){
    ShaderDescription::ConstantBufferFieldList::const_iterator i;
    unsigned size = 0, sz;
    for(i = fields.begin(); i != fields.end(); ++i){
        if((sz = i->ByteOffset + i->ByteSize) > size)
            size = sz;
    }
    return size;
}

void fill_cbuf( ShaderDescription::ConstantBufferList const& cbufDesc,
                CALConstantBufferMask* cbufMask,
                unsigned int* cbufCount, unsigned int* subsCount )
{
    ShaderDescription::ConstantBufferList::const_iterator iter;
    *cbufCount = (unsigned int)cbufDesc.size();
    *subsCount = 0;
    int count = 0;
    for(iter = cbufDesc.begin(); iter != cbufDesc.end();  ++iter)
    {
        cbufMask[count].index = iter->BufferNumber;
        cbufMask[count++].size = cbuf_size(iter->Fields);
        *subsCount += (unsigned int) iter->Fields.size();
    }
}

void fill_symbols( const ConstShaderDescriptionPtr &info, AMUabiUserSymbol* symbols)
{
    //Outputs
    unsigned int cnt = 0;
    unsigned int size = (unsigned int)info->Outputs.size();
    for(unsigned int i = 0; i < size; i++)
    {
        symbols[cnt].name  = info->Outputs[i].Variable.Name->c_str();
        symbols[cnt].value = i;
        symbols[cnt].section = ELF_NOTE_AMD_OUTPUTS;
        cnt++;
    }

    //texture
    size = (unsigned int)info->Textures.size();
    for(unsigned int i = 0; i < size; i++)
    {
        symbols[cnt].name  = info->Textures[i].Variable.Name->c_str();
        symbols[cnt].value = i;
        symbols[cnt].section = ELF_NOTE_AMD_INPUTS;
        cnt++;
    }

    //constant buffers
    size = (unsigned int)info->ConstantBuffers.size();
    unsigned int subs_ordinal = 0;
    for(unsigned int i = 0; i < size; i++)
    {
        symbols[cnt].name  = info->ConstantBuffers[i].Name->c_str();
        symbols[cnt].value = i;
        symbols[cnt].section = ELF_NOTE_AMD_CONSTANT_BUFFERS;
        cnt++;
        for ( unsigned int jj = 0; jj < info->ConstantBuffers[i].Fields.size(); ++jj )
        {
            //
            symbols[cnt].name = info->ConstantBuffers[i].Fields[jj].Variable.Name->c_str();
            symbols[cnt].value = subs_ordinal++;
            symbols[cnt].section = ELF_NOTE_AMD_SUB_CONSTANT_BUFFERS;
            cnt++;
        }
    }

    if (info->UsesGlobalStorage)
    {
        const char *symbolName = "g[]";
        symbols[cnt].name = symbolName;
        symbols[cnt].value = 0;
        symbols[cnt].section = ELF_NOTE_AMD_GLOBAL_BUFFERS;
        cnt++;
    }
}


void convertToHex(unsigned int* output_hex, const char *str, unsigned int tokens)
{
    for(unsigned int i = 0; i < tokens; i++)
    {
        unsigned int temp = 0;
        unsigned int multiplier = 0;
        for(int j = 9 ; j >= 2; j--)
        {
            char c = str[i*11 + j];
            if( c >= '0' && c <= '9' )
                temp |= (c - '0') << multiplier;
            else
                temp |= ((c -'A')+10) << multiplier;
            multiplier += 4;
        }
        output_hex[i] = temp;
    }
}

inline HLFrontendFlags::OutputFormat
calut2hlslFlag( CALUT_OUTPUT_FORMAT flag )
{
    switch ( flag )
    {
        case CALUT_OUTPUT_TXT: return HLFrontendFlags::OUTPUT_TXT;
        case CALUT_OUTPUT_BIN: return HLFrontendFlags::OUTPUT_BIN;
        case CALUT_OUTPUT_HEX: return HLFrontendFlags::OUTPUT_HEX;
        default: ; // doesn't matter
    }
    return HLFrontendFlags::OUTPUT_FROMPARAMS;
}

CALAPI CALresult CALAPIENTRY
calutAMDhlslCompileProgram( CALobject*		object,
                            CALCLprogramType	type,
                            const char*		kernel_source,
                            CALtarget		target,
                            CALUT_OUTPUT_FORMAT	target_format )
{
    amdHLSLerrst.reset();
    ////////////////////////////////
    // Validate
    //
    if(type != CAL_PROGRAM_TYPE_PS && type != CAL_PROGRAM_TYPE_CS)
    {
        amdHLSLerrst.set(CAL_RESULT_INVALID_PARAMETER);
        return CAL_RESULT_INVALID_PARAMETER;
    }

    if(target != CAL_TARGET_600 &&
       target != CAL_TARGET_610 &&
       target != CAL_TARGET_630 &&
       target != CAL_TARGET_670 &&
       target != CAL_TARGET_7XX &&
       //target != CAL_TARGET_710 &&
      // target != CAL_TARGET_730 &&
       target != CAL_TARGET_770)
    {
        amdHLSLerrst.set(CAL_RESULT_INVALID_PARAMETER);
        return CAL_RESULT_INVALID_PARAMETER;
    }

    if ( kernel_source == NULL )
    {
        amdHLSLerrst.set(CAL_RESULT_BAD_HANDLE);
        return CAL_RESULT_BAD_HANDLE;
    }

    unsigned int il_target;
    if ( target == CAL_TARGET_600 || target == CAL_TARGET_610
      || target == CAL_TARGET_630 || target == CAL_TARGET_670 )
    {
        il_target = (CALUT_OUTPUT_TXT != target_format)
            ? ED_AMD_CAL_MACHINE_R6XX_IL : ED_AMD_CAL_MACHINE_R6XX_IL_TEXT;
    }
    else
    {
        il_target = (CALUT_OUTPUT_TXT != target_format)
            ? ED_AMD_CAL_MACHINE_R7XX_IL : ED_AMD_CAL_MACHINE_R7XX_IL_TEXT;
    }

    ////////////////////////////////
    // Compile into IL Binary
    //
    std::stringstream input, output;
    ConstShaderDescriptionPtr info;
    HLFrontendFlags::OutputFormat oformat = calut2hlslFlag( target_format );

    input.write( kernel_source, strlen( kernel_source ) );

    HLFrontendSetParameter("nocomments", true);
    // Use cal compatibility mode
    HLFrontendSetParameter("cal", true);
    HLFrontendFlags::ShaderType shaderType = HLFrontendFlags::TYPE_PS;
    if(type == CAL_PROGRAM_TYPE_CS)
        shaderType = HLFrontendFlags::TYPE_CS;
    if ( !HLFrontendCompileBuffer( input, output,
                                   shaderType,
                                   oformat, &info ) )
    {
        amdHLSLerrst.set(CAL_RESULT_ERROR);
        return CAL_RESULT_ERROR;
    }
    // add compiler specific errors in amdHLSLerrst

    // convert hex output in text format to actual HEX tokens.
    unsigned int textSize;
    void* textData;
    if ( CALUT_OUTPUT_TXT != target_format )
    {
        textSize = (unsigned int) strlen( output.str().c_str() ) / 11;
        textData = calloc( textSize, sizeof(unsigned int) );
        convertToHex( (unsigned int*) textData, output.str().c_str(), textSize );
        textSize *= sizeof(unsigned int);
    }
    else
    {
        textSize = (unsigned int) strlen( output.str().c_str() ) + 1;
        textData = strdup( output.str().c_str() );
    }

    ////////////////////////////////
    // Pack data into ELF
    //
    AMUabiMultiBinary m_binary;
    unsigned int inputCount, outputCount, constBufferCount, subBufferCount, symbolCount;
    AMUabiUserSymbol *symbols = NULL;
    unsigned int *inputs = NULL, *outputs = NULL;
    //Hardcoded max constant buffers values as free() segfaults
    CALConstantBufferMask constBuffers[16];

    if (!amuABIMultiBinaryCreate(&m_binary))
    {
        amdHLSLerrst.set(CAL_RESULT_ERROR);
        return CAL_RESULT_ERROR;
    }

    // fill vars
    fill_tex(info->Textures, &inputs, &inputCount);
    fill_output(info->Outputs, &outputs, &outputCount);
    fill_cbuf( info->ConstantBuffers, constBuffers, &constBufferCount, &subBufferCount );
    unsigned int uiGlobalBuffer = 0;
    if (info->UsesGlobalStorage)
    {
        uiGlobalBuffer = 1;
    }
    symbolCount = outputCount + inputCount + constBufferCount + uiGlobalBuffer + subBufferCount;
    symbols = (AMUabiUserSymbol*)malloc(sizeof(AMUabiUserSymbol)*symbolCount);
    fill_symbols(info, symbols);

    // encode
    if (!amuABIMultiBinaryAddEncoding(m_binary, il_target,
                                      ED_AMD_CAL_TYPE_FRAGMENT,
                                      inputCount, inputs,
                                      outputCount, outputs,
                                      0, 0,
                                      uiGlobalBuffer, &uiGlobalBuffer,
                                      0, NULL,
                                      0, NULL,
                                      0, NULL,
                                      constBufferCount, constBuffers,
                                      0, NULL,
                                      0, NULL,
                                      textSize, textData,
                                      symbolCount, symbols))
    {
        free( textData );
        amdHLSLerrst.set( CAL_RESULT_ERROR );
        return CAL_RESULT_ERROR;
    }

    // add constant buffers layout information
    unsigned int subIndex = 0;
    CALSubConstantBufferEntry* subBuffers = new CALSubConstantBufferEntry[subBufferCount];
    ShaderDescription::ConstantBufferList::const_iterator cbuf;
    for ( cbuf = info->ConstantBuffers.begin(); cbuf != info->ConstantBuffers.end(); ++cbuf )
    {
        ShaderDescription::ConstantBufferFieldList const fields = cbuf->Fields;
        ShaderDescription::ConstantBufferFieldList::const_iterator field;
        for ( field = fields.begin(); field != fields.end(); ++field )
        {
            subBuffers[subIndex].index = cbuf->BufferNumber;
            subBuffers[subIndex].offset = field->ByteOffset ;
            subBuffers[subIndex].size = field->ByteSize;
            ++subIndex;
        }
    }
    int success;
    success = amuABIMultiBinaryAddSubConstantBuffers( m_binary, il_target,
                                                      ED_AMD_CAL_TYPE_FRAGMENT,
                                                      subBufferCount, subBuffers );
    if ( !success )
    {
        delete [] subBuffers;
        free( textData );
        amdHLSLerrst.set( CAL_RESULT_ERROR );
        return CAL_RESULT_ERROR;
    }

    // pack into elf (pBinary)
    unsigned int binarySize = 0;
    void* pBinary = NULL;
    amuABIMultiBinaryPack(&binarySize, &pBinary, m_binary);
    amuABIMultiBinaryDestroy(m_binary);
    delete [] subBuffers;

    //  hack up a CALobject from pBinary
    *object = reinterpret_cast<CALobject>(pBinary);

    if (textSize)
        free(textData);
    if (symbols)
        free(symbols);
    if (inputs)
        free(inputs);
    if (outputs)
        free(outputs);

    amdHLSLerrst.set(CAL_RESULT_OK);
    return CAL_RESULT_OK;
}

CALAPI CALchar* CALAPIENTRY calutAMDhlslGetLastError()
{
    return (CALchar*)amdHLSLerrst.getString();
}

ErrorState::ErrorState()
{
    init();
}

ErrorState::~ErrorState()
{
    shutdown();
}

void
ErrorState::reset()
{
    m_loc = 0;
}

void
ErrorState::init()
{
    m_errorString = 0;
    m_allocSize = 0;
    m_loc = 0;

    m_result = CAL_RESULT_OK;
}

void
ErrorState::shutdown()
{
    m_result = CAL_RESULT_OK;

    delete[] m_errorString;
    m_errorString = 0;
    m_allocSize = 0;
    m_loc = 0;
}

CALresult
ErrorState::set(CALresult result)
{
    m_result = result;
    return m_result;
}

const char*
ErrorState::getString()
{
    // If an error string already exists, then return it
    // else return a generic error string based on m_result
    if (m_allocSize > 0)
    {
        // also skip if m_loc = 0
        if ( m_loc != 0 )
        {
            return m_errorString;
        }
    }

    switch ( m_result )
    {
        case CAL_RESULT_OK:
                            return "No error";
        case CAL_RESULT_ERROR:
                            return "Operational error";
        case CAL_RESULT_INVALID_PARAMETER:
                            return "Parameter passed in is invalid";
        case CAL_RESULT_WARNING:
                            return "CAL generated a warning";
    default:
                return "No error";
    }

    //default
    return "No error";
}

