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

#include "amuABIMultiBinary.h"
#include "amuABI.h"
#include "amuABIElf.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////////////
//
//  AMUabiEncodingPackData 
//
AMUabiEncodingPackData::AMUabiEncodingPackData()
    : float32ConstsCount(0),
      float32Consts(0),
      int32ConstsCount(0),
      int32Consts(0),
      bool32ConstsCount(0),
      bool32Consts(0)
{
}

AMUabiEncodingPackData::~AMUabiEncodingPackData()
{
}

void AMUabiEncodingPackData::CleanUp()
{
    if ( float32Consts )
    {
        free( float32Consts );
        float32Consts = 0;
    }

    if ( int32Consts )
    {
        free( int32Consts );
        int32Consts = 0;
    }

    if ( bool32Consts )
    {
        free( bool32Consts );
        bool32Consts = 0;
    }
}

/////////////////////////////////////////////////////////////////////////
//
//  AMUabiEncodingUnpackData 
//
AMUabiEncodingUnpackData::AMUabiEncodingUnpackData()
    : offset(0),
      size(0),
      litConstsData(0),
      symbolsData(0)
{
}

AMUabiEncodingUnpackData::~AMUabiEncodingUnpackData()
{
    if (litConstsData != 0)
    {
        free(litConstsData);
        litConstsData = 0;
    }
    
    if (symbolsData != 0)
    {
        free(symbolsData);
        symbolsData = 0;
    }
}

void AMUabiEncodingUnpackData::CleanUp()
{
}

/////////////////////////////////////////////////////////////////////////
//
//  AMUabiEncodingSignature
//
AMUabiEncodingSignature::AMUabiEncodingSignature()
{
}

AMUabiEncodingSignature::~AMUabiEncodingSignature()
{
}

/////////////////////////////////////////////////////////////////////////
//
//  AMUabiEncodingInfo
//
AMUabiEncodingInfo::AMUabiEncodingInfo()
{
}

AMUabiEncodingInfo::AMUabiEncodingInfo(const AMUabiEncodingInfo& src)
{
    signature.machine = src.signature.machine;
    signature.type = src.signature.type;
    inputCount = src.inputCount; 
    inputs = src.inputs;
    outputCount = src.outputCount;
    outputs = src.outputs;
    condOut = src.condOut;
    earlyExit = src.earlyExit;
    globalBuffersCount = src.globalBuffersCount;
    globalBuffers = src.globalBuffers;
    scratchBuffersCount = src.scratchBuffersCount;
    scratchBuffers = src.scratchBuffers;
    persistentBuffersCount = src.persistentBuffersCount;
    persistentBuffers = src.persistentBuffers;
    litConstsCount = src.litConstsCount;
    litConsts = src.litConsts;
    constBuffersCount = src.constBuffersCount; 
    constBuffers = src.constBuffers;
    inputSamplerMapCount = src.inputSamplerMapCount;
    inputSamplerMaps = src.inputSamplerMaps;
    progInfosCount = src.progInfosCount;
    progInfos = src.progInfos;
    textSize = src.textSize;
    textData = src.textData;
    symbolsCount = src.symbolsCount;
    symbols = src.symbols;
    subBufferCount = src.subBufferCount;
    subBuffers = src.subBuffers;
}

AMUabiEncodingInfo::~AMUabiEncodingInfo()
{
}

/////////////////////////////////////////////////////////////////////////
//
//  AMUabiMultiBinaryInfo
//
AMUabiMultiBinaryInfo::AMUabiMultiBinaryInfo()
{
}

AMUabiMultiBinaryInfo::~AMUabiMultiBinaryInfo()
{
    DeleteEncodings();
}

void AMUabiMultiBinaryInfo::DeleteEncodings()
{
    for (unsigned int i = 0; i < encodings.size(); ++i)
    {
        delete encodings[i];
    }
    encodings.clear();
}



// Creation and destruction
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryCreate(AMUabiMultiBinary* binary)
{
    assert(binary != 0);
    *binary = reinterpret_cast<AMUabiMultiBinary> (new AMUabiMultiBinaryInfo);
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryDestroy(AMUabiMultiBinary binary)
{
    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);
    delete binaryInfo;

    return 1;
}

// Adding encodings 
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
        )
{
    assert(binary != 0);
    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);
  
    AMUabiEncodingInfo* tmp = new AMUabiEncodingInfo;
    tmp->signature.machine = machine;
    tmp->signature.type = type;
    tmp->inputCount = inputCount; 
    tmp->inputs = inputs;
    tmp->outputCount = outputCount;
    tmp->outputs = outputs;
    tmp->condOut = condOut;
    tmp->earlyExit = earlyExit;
    tmp->globalBuffersCount = globalBuffersCount;
    tmp->globalBuffers = globalBuffers;
    tmp->scratchBuffersCount = scratchBuffersCount;
    tmp->scratchBuffers = scratchBuffers;
    tmp->persistentBuffersCount = persistentBuffersCount;
    tmp->persistentBuffers = persistentBuffers;
    tmp->litConstsCount = litConstsCount;
    tmp->litConsts = litConsts;
    tmp->constBuffersCount = constBuffersCount; 
    tmp->constBuffers = constBuffers;
    tmp->inputSamplerMapCount = inputSamplerMapCount;
    tmp->inputSamplerMaps = inputSamplerMaps;
    tmp->progInfosCount = progInfosCount;
    tmp->progInfos = progInfos;
    tmp->textSize = textSize;
    tmp->textData = textData;
    tmp->symbolsCount = symbolsCount;
    tmp->symbols = symbols;
    tmp->subBufferCount = 0;
    tmp->subBuffers = 0;
    binaryInfo->encodings.push_back(tmp);

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryAddSubConstantBuffers(
        AMUabiMultiBinary binary,
        unsigned int machine, unsigned int type,
        unsigned int subBufferCount, CALSubConstantBufferEntry* subBuffers
        )
{
    assert(binary != 0);
    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);

    for (unsigned int i=0; i < binaryInfo->encodings.size(); i++)
    {
        AMUabiEncodingInfo* tmp = binaryInfo->encodings[i];
        if ((tmp->signature.machine == machine) && (tmp->signature.type == type))
        {
            tmp->subBufferCount = subBufferCount;
            tmp->subBuffers = subBuffers;
            return 1;
        }
    }
    return 0;
}


CALAPI unsigned int CALAPIENTRY 
amuABIMultiBinaryAddAMUEncoding( AMUabiMultiBinary binary, AMUabiEncoding encoding)
{
    assert(binary != 0);
    assert(encoding != 0);

    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);
    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);

    AMUabiEncodingInfo* tmp = new AMUabiEncodingInfo(*encodingInfo);

    binaryInfo->encodings.push_back(tmp);

    return 1;
}

// Extracting encodings
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryGetEncodingCount(unsigned int* encodingCount, AMUabiMultiBinary binary)
{
    assert(encodingCount != 0);
    assert(binary != 0);

    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);
    *encodingCount = binaryInfo->encodings.size();
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryGetEncoding(AMUabiEncoding* encoding, AMUabiMultiBinary binary, unsigned int index)
{
    assert(encoding != 0);
    assert(binary != 0);

    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);
    assert(index < binaryInfo->encodings.size());

    *encoding = reinterpret_cast<AMUabiEncoding>(binaryInfo->encodings[index]);
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryFindEncoding(AMUabiEncoding* encoding, AMUabiMultiBinary binary, unsigned int machine, unsigned int type)
{
    assert(encoding != 0);
    assert(binary != 0);

    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);

    *encoding = 0;
    for (unsigned int i = 0; i < binaryInfo->encodings.size(); ++i)
    {
        AMUabiEncodingInfo* tmp = binaryInfo->encodings[i];
        if ((tmp->signature.machine == machine) && (tmp->signature.type == type))
        {
            *encoding = reinterpret_cast<AMUabiEncoding>(binaryInfo->encodings[i]);
            return 1;
        }
    }
    return 0;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetSignature(unsigned int* machine, unsigned int* type, AMUabiEncoding encoding)
{
    assert(machine != 0);
    assert(type != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *machine = encodingInfo->signature.machine;
    *type = encodingInfo->signature.type;

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetInputs(unsigned int* inputCount, unsigned int** inputs, AMUabiEncoding encoding)
{
    assert(inputCount != 0);
    assert(inputs != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *inputCount = encodingInfo->inputCount;
    *inputs = encodingInfo->inputs;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetOutputs(unsigned int* outputCount, unsigned int** outputs, AMUabiEncoding encoding)
{
    assert(outputCount != 0);
    assert(outputs != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *outputCount = encodingInfo->outputCount;
    *outputs = encodingInfo->outputs;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetCondOut(unsigned int* condOut, AMUabiEncoding encoding)
{
    assert(condOut != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *condOut = encodingInfo->condOut;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetEarlyExit(unsigned int* earlyExit, AMUabiEncoding encoding)
{
    assert(earlyExit != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *earlyExit = encodingInfo->earlyExit;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetGlobalBuffers(unsigned int* globalBuffersCount, unsigned int** globalBuffers, AMUabiEncoding encoding)
{
    assert(globalBuffersCount != 0);
    assert(globalBuffers != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *globalBuffersCount = encodingInfo->globalBuffersCount;
    *globalBuffers = encodingInfo->globalBuffers;

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetScratchBuffers(unsigned int* scratchBuffersCount, unsigned int** scratchBuffers, AMUabiEncoding encoding)
{
    assert(scratchBuffersCount != 0);
    assert(scratchBuffers != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *scratchBuffersCount = encodingInfo->scratchBuffersCount;
    *scratchBuffers = encodingInfo->scratchBuffers;

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetPersistentBuffers(unsigned int* persistentBuffersCount, unsigned int** persistentBuffers, AMUabiEncoding encoding)
{
    assert(persistentBuffersCount != 0);
    assert(persistentBuffers != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *persistentBuffersCount = encodingInfo->persistentBuffersCount;
    *persistentBuffers = encodingInfo->persistentBuffers;

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetLitConsts(unsigned int* litConstsCount, AMUabiLiteralConst** litConsts, AMUabiEncoding encoding)
{
    assert(litConstsCount != 0);
    assert(litConsts != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *litConstsCount = encodingInfo->litConstsCount;
    *litConsts = encodingInfo->litConsts;

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetConstBuffers(unsigned int* constBuffersCount, CALConstantBufferMask** constBuffers, AMUabiEncoding encoding)
{
    assert(constBuffersCount != 0);
    assert(constBuffers != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *constBuffersCount = encodingInfo->constBuffersCount;
    *constBuffers = encodingInfo->constBuffers;

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetInputSamplerMaps(unsigned int* inputSamplerMapCount, CALSamplerMapEntry** inputSamplerMaps, 
                                               AMUabiEncoding encoding)
{
    assert(inputSamplerMapCount != 0);
    assert(inputSamplerMaps != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *inputSamplerMapCount = encodingInfo->inputSamplerMapCount;
    *inputSamplerMaps = encodingInfo->inputSamplerMaps;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetProgInfos(unsigned int* progInfosCount, CALProgramInfoEntry** progInfos, AMUabiEncoding encoding)
{
    assert(progInfosCount != 0);
    assert(progInfos != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *progInfosCount = encodingInfo->progInfosCount;
    *progInfos = encodingInfo->progInfos;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetText(unsigned int* textSize, void** textData, AMUabiEncoding encoding)
{
    assert(textSize != 0);
    assert(textData != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *textSize = encodingInfo->textSize;
    *textData = encodingInfo->textData;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetSymbols(unsigned int* symbolsCount, AMUabiUserSymbol** symbols, AMUabiEncoding encoding)
{
    assert(symbolsCount != 0);
    assert(symbols != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *symbolsCount = encodingInfo->symbolsCount;
    *symbols = encodingInfo->symbols;
    
    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIEncodingGetSubConstantBuffers(unsigned int* bufferCount, CALSubConstantBufferEntry** subBuffers, AMUabiEncoding encoding)
{
    assert(bufferCount != 0);
    assert(subBuffers != 0);
    assert(encoding != 0);

    AMUabiEncodingInfo* encodingInfo = reinterpret_cast<AMUabiEncodingInfo*>(encoding);
    *bufferCount = encodingInfo->subBufferCount;
    *subBuffers = encodingInfo->subBuffers;
    
    return 1;
}

static const char noteNameStr[] = ".note";
static const char textNameStr[] = ".text";
static const char dataNameStr[] = ".data";
static const char shstrtabNameStr[] = ".shstrtab";
static const char symtabNameStr[] = ".symtab";
static const char strtabNameStr[] = ".strtab";

static unsigned int 
ComputeNoteSize(unsigned int noteDataSize)
{
    unsigned int noteSize = 4             // namesz
                          + 4             // descsz
                          + 4             // type
                          + 8             // "AMD CAL"
                          + noteDataSize; // note data
    return noteSize;
}

static char*
AddElfHeader(
    char* ptr, unsigned int abiver, unsigned int type, unsigned int machine,
    unsigned int phoff, unsigned int phnum,
    unsigned int shoff, unsigned int shnum, unsigned int shstrndx )
{
    Elf32_Ehdr * ehdr = ( Elf32_Ehdr* ) ptr;

    for ( unsigned int i = 0; i < EI_NIDENT; i++ )
    {
        ehdr->e_ident[ i ] = 0;
    }

    ehdr->e_ident[ EI_MAG0 ] = ELFMAG0;             // magic number and other info
    ehdr->e_ident[ EI_MAG1 ] = ELFMAG1;             // ...
    ehdr->e_ident[ EI_MAG2 ] = ELFMAG2;             // ...
    ehdr->e_ident[ EI_MAG3 ] = ELFMAG3;             // ...
    ehdr->e_ident[ EI_CLASS ] = ELFCLASS32;         // ...
    ehdr->e_ident[ EI_DATA ] = ELFDATA2LSB;         // ...
    ehdr->e_ident[ EI_VERSION ] = EV_CURRENT;       // ...
    ehdr->e_ident[ EI_OSABI ] = ELFOSABI_CALIMAGE;  // ...
    ehdr->e_ident[ EI_ABIVERSION ] = abiver;        // abi version
    ehdr->e_type = type;                            // elf type
    ehdr->e_machine = machine;                      // machine architecture
    ehdr->e_version = EV_CURRENT;                   // object file version
    ehdr->e_entry = 0;                              // entry point virtual address
    ehdr->e_phoff = phoff;                          // program header table file offset
    ehdr->e_shoff = shoff;                          // section table file offset
    ehdr->e_flags = EF_AMD_CALIMAGE_BINARY;         // processor-specific flags
    ehdr->e_ehsize = sizeof( Elf32_Ehdr );          // ELF header size in bytes
    ehdr->e_phentsize = sizeof( Elf32_Phdr );       // program header table entry size
    ehdr->e_phnum = phnum;                          // program header table entry count
    ehdr->e_shentsize = sizeof( Elf32_Shdr );       // section header table entry size
    ehdr->e_shnum = shnum;                          // section header table entry count
    ehdr->e_shstrndx = shstrndx;                    // section header string table index (SHN_UNDEF if not used)

    return ptr + sizeof( Elf32_Ehdr );
}

static char*
AddProgramHeader(
    char* ptr, unsigned int type, unsigned int offset, unsigned int vaddr, unsigned int paddr,
    unsigned int filesz, unsigned int memsz, unsigned int flags, unsigned int align )
{
    Elf32_Phdr * phdr = ( Elf32_Phdr* ) ptr;

    phdr->p_type = type;
    phdr->p_offset = offset;
    phdr->p_vaddr = vaddr;
    phdr->p_paddr = paddr;
    phdr->p_filesz = filesz;
    phdr->p_memsz = memsz;
    phdr->p_flags = flags;
    phdr->p_align = align;

    return ptr + sizeof( Elf32_Phdr );
}

static char*
AddSectionHeader(
    char* ptr, unsigned int name, unsigned int type, unsigned int flags,
    unsigned int addr, unsigned int offset, unsigned int size, unsigned int link,
    unsigned int info, unsigned int addralign, unsigned int entsize )
{
    Elf32_Shdr * shdr = ( Elf32_Shdr* ) ptr;

    shdr->sh_name = name;
    shdr->sh_type = type;
    shdr->sh_flags = flags;
    shdr->sh_addr = addr;
    shdr->sh_offset = offset;
    shdr->sh_size = size;
    shdr->sh_link = link;
    shdr->sh_info = info;
    shdr->sh_addralign = addralign;
    shdr->sh_entsize = entsize;

    return ptr + sizeof( Elf32_Shdr );
}

static char*
AddEncodingDictionaryEntry(char *ptr, unsigned int machine, unsigned int type, unsigned int offset, unsigned int size, unsigned int flags)
{
    CALEncodingDictionaryEntry* entry = (CALEncodingDictionaryEntry*) ptr;
    entry->d_machine = machine;
    entry->d_type = type;
    entry->d_offset = offset;
    entry->d_size = size;
    entry->d_flags = flags;

    return ptr + sizeof(CALEncodingDictionaryEntry);
}

static char*
AddNote( char* ptr, unsigned int namesz, unsigned int descsz, unsigned int type, const char* name, const char* desc )
{
    unsigned int * p = ( unsigned int* ) ptr; // XXX assumes aligned, little endian
    *p++ = namesz;
    *p++ = descsz;
    *p++ = type;
    char* pc = ( char* ) p;
    for ( unsigned int i = 0; i < namesz; i++ )
    {
        *pc++ = name[ i ];
    }
    for ( unsigned int i = 0; i < descsz; i++ )
    {
        *pc++ = desc[ i ];
    }

    return pc;
}

static char*
AddSymbol( char* ptr, unsigned int name, unsigned int value, unsigned int size, unsigned char info, unsigned char other,
           unsigned short section )
{
    Elf32_Sym * sym = ( Elf32_Sym* ) ptr;

    sym->st_name = name;
    sym->st_value = value;
    sym->st_size = size;
    sym->st_info = info;
    sym->st_other = other;
    sym->st_shndx = section;

    return ptr + sizeof( Elf32_Sym );
}

static char*
AddConstData( char* ptr, unsigned int litConstsCount, AMUabiLiteralConst *litConsts )
{
    AMUabiConstsData * data = ( AMUabiConstsDataRec* ) ptr;

    // XXX assumes all consts are allocated to a single 4 component register
    for ( unsigned int j = 0; j < litConstsCount && j < AMUabiMaxLiteralConsts && litConsts; j++ )
    {
        unsigned int r = litConsts[ j ].addr;
        switch ( litConsts[ j ].type )
        {
        case AMU_ABI_SAMPLER:
        case AMU_ABI_FLOAT32:
            {
                if ( r < 256 )
                {
                    data->float32[ r * 4 + 0 ] = litConsts[ j ].value.float32[ 0 ];
                    data->float32[ r * 4 + 1 ] = litConsts[ j ].value.float32[ 1 ];
                    data->float32[ r * 4 + 2 ] = litConsts[ j ].value.float32[ 2 ];
                    data->float32[ r * 4 + 3 ] = litConsts[ j ].value.float32[ 3 ];
                }
                break;
            }
        case AMU_ABI_INT32:
            {
                if ( r < 32 )
                {
                    data->int32[ r * 4 + 0 ] = litConsts[ j ].value.int32[ 0 ];
                    data->int32[ r * 4 + 1 ] = litConsts[ j ].value.int32[ 1 ];
                    data->int32[ r * 4 + 2 ] = litConsts[ j ].value.int32[ 2 ];
                    data->int32[ r * 4 + 3 ] = litConsts[ j ].value.int32[ 3 ];
                }
                break;
            }
        case AMU_ABI_BOOL32:
            {
                if ( r < 32 )
                {
                    data->bool32[ 0 ] = litConsts[ j ].value.bool32[ 0 ];
                }
                break;
            }
        case AMU_ABI_UNDEF:
        case AMU_ABI_MAXTYPES:
        default:
            {
                break;
            }

        }
    }

    return ptr + sizeof( AMUabiConstsData );

}

static void
do_float32_consts(unsigned int& float32ConstsCount, CALDataSegmentDesc*& float32Consts,
                  unsigned int litConstsCount, AMUabiLiteralConst* litConsts)
{
    for ( unsigned int i = 0; i < litConstsCount; i++ )
    {
        if ( litConsts[ i ].type == AMU_ABI_FLOAT32 )
        {
            float32ConstsCount++;
        }
    }

    if (float32ConstsCount > 0)
    {
        float32Consts = reinterpret_cast<CALDataSegmentDesc*> (malloc(float32ConstsCount * sizeof(CALDataSegmentDesc)));
        CALDataSegmentDesc *pfloat32Consts = float32Consts;

        for ( unsigned int i = 0; i < litConstsCount; i++ )
        {
            if ( litConsts[ i ].type == AMU_ABI_FLOAT32 )
            {
                pfloat32Consts->offset = litConsts[ i ].addr;
                pfloat32Consts->size = 4 * sizeof(float);
                pfloat32Consts++;
            }
        }
    }
}

static void
do_int32_consts(unsigned int& int32ConstsCount, CALDataSegmentDesc*& int32Consts,
                unsigned int litConstsCount, AMUabiLiteralConst* litConsts)
{
    for ( unsigned int i = 0; i < litConstsCount; i++ )
    {
        if ( litConsts[ i ].type == AMU_ABI_INT32 )
        {
            int32ConstsCount++;
        }
    }

    if (int32ConstsCount > 0)
    {
        int32Consts = reinterpret_cast<CALDataSegmentDesc*> (malloc(int32ConstsCount * sizeof(CALDataSegmentDesc)));
        CALDataSegmentDesc *pint32Consts = int32Consts;

        for ( unsigned int i = 0; i < litConstsCount; i++ )
        {
            if ( litConsts[ i ].type == AMU_ABI_INT32 )
            {
                pint32Consts->offset = litConsts[ i ].addr;
                pint32Consts->size = 4 * sizeof(unsigned int);
                pint32Consts++;
            }
        }
    }
}

static void
do_bool32_consts(unsigned int& bool32ConstsCount, CALDataSegmentDesc*& bool32Consts,
                 unsigned int litConstsCount, AMUabiLiteralConst* litConsts)
{
    for ( unsigned int i = 0; i < litConstsCount; i++ )
    {
        if ( litConsts[ i ].type == AMU_ABI_BOOL32 )
        {
            bool32ConstsCount++;
        }
    }

    if (bool32ConstsCount > 0)
    {
        bool32Consts = reinterpret_cast<CALDataSegmentDesc*> (malloc(bool32ConstsCount * sizeof(CALDataSegmentDesc)));
        CALDataSegmentDesc *pbool32Consts = bool32Consts;

        for ( unsigned int i = 0; i < litConstsCount; i++ )
        {
            if ( litConsts[ i ].type == AMU_ABI_BOOL32 )
            {
                pbool32Consts->offset = litConsts[ i ].addr;
                pbool32Consts->size = 4 * sizeof(unsigned int);
                pbool32Consts++;
            }
        }
    }
}


// Translation to/from ELF binary
CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryPack(unsigned int* binarySize, void** binaryData, AMUabiMultiBinary binary)
{
    assert(binarySize != 0);
    assert(binaryData != 0);
    assert(binary != 0);


    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);
    unsigned int encodingCount = binaryInfo->encodings.size();

    /////////////////////////////////////////////////////////////////////////
    //
    //  check validity of the encodings  
    //
    calstd::vector<AMUabiEncodingSignature> signatures;
    for (unsigned int i = 0; i < encodingCount; ++i)
    {
        // Look for encodings with duplicate signatures.

        for (unsigned int j = 0; j < signatures.size(); ++j)
        {
            if (binaryInfo->encodings[i]->signature == signatures[j])
            {
                // Found a duplicate, return 0 indicating failure        
                return 0;
            }
        }
   
        // If we get here then we haven't seen this signature yet so
        // add it to the signatures list. 
        signatures.push_back(binaryInfo->encodings[i]->signature);
    }

    /////////////////////////////////////////////////////////////////////////
    //
    //  setup elf header attributes
    //
    unsigned int ehdrOff = 0;
    unsigned int ehdrSize = sizeof( Elf32_Ehdr );

    /////////////////////////////////////////////////////////////////////////
    //
    //  setup program header attributes
    //
    unsigned int phtOff = ehdrOff + ehdrSize;
    unsigned int phtEntries = 1 + 2 * encodingCount;
    unsigned int phtSize = phtEntries * sizeof( Elf32_Phdr );

    /////////////////////////////////////////////////////////////////////////
    //
    //  setup encoding dictionary attributes 
    //
    unsigned int edOff = phtOff + phtSize;
    unsigned int edSize = encodingCount * sizeof(CALEncodingDictionaryEntry);

    /////////////////////////////////////////////////////////////////////////
    //
    //  setup section header string table attributes
    //
    unsigned int shstrtabOff = edOff + edSize;                      // section header string table offset
    unsigned int shstrtabSize = 1                                   // section header string table size
                              + (unsigned int)strlen( shstrtabNameStr ) + 1
                              + (unsigned int)strlen( textNameStr ) + 1
                              + (unsigned int)strlen( dataNameStr ) + 1
                              + (unsigned int)strlen( symtabNameStr ) + 1
                              + (unsigned int)strlen( strtabNameStr ) + 1
                              + 1;
    unsigned int shstrtabName = 1;                                  // offset into shstrtabStr[]
    unsigned int shstrtabIndex = 1;                                 // section index for section header string table (for elf header)

    /////////////////////////////////////////////////////////////////////////
    //
    //  setup section header table attributes
    //
    unsigned int shtEntries = 2 + 4 * encodingCount;                // null start section + section header string table + 4 sections per-encoding
    unsigned int shtOff = shstrtabOff + shstrtabSize;
    unsigned int shtSize = shtEntries * sizeof( Elf32_Shdr );

    unsigned int encodingsSize = 0;
    for (unsigned int i = 0; i < encodingCount; ++i)
    {
        AMUabiEncodingInfo* encoding = binaryInfo->encodings[i];
        AMUabiEncodingPackData &epd = encoding->packData;

        /////////////////////////////////////////////////////////////////////////
        //
        // process user and literal constants by type
        //
        epd.float32ConstsCount = 0;
        epd.float32Consts = 0;
        epd.int32ConstsCount = 0;
        epd.int32Consts = 0;
        epd.bool32ConstsCount = 0;
        epd.bool32Consts = 0;

        do_float32_consts(epd.float32ConstsCount, epd.float32Consts, encoding->litConstsCount, encoding->litConsts);
        do_int32_consts(epd.int32ConstsCount, epd.int32Consts, encoding->litConstsCount, encoding->litConsts);
        do_bool32_consts(epd.bool32ConstsCount, epd.bool32Consts, encoding->litConstsCount, encoding->litConsts);

        /////////////////////////////////////////////////////////////////////////
        //
        //  compute notes offset and size
        //
        epd.notesOff = shtOff + shtSize + encodingsSize;
        epd.notesSize = 0;
        epd.notesSize += ComputeNoteSize(encoding->inputCount * sizeof( unsigned int ));                         // inputs
        epd.notesSize += ComputeNoteSize(encoding->outputCount * sizeof( unsigned int ));                        // outputs
        epd.notesSize += ComputeNoteSize(sizeof( unsigned int ));                                                // conditional output
        epd.notesSize += ComputeNoteSize(epd.float32ConstsCount * sizeof( CALDataSegmentDesc ));                 // float32 constants
        epd.notesSize += ComputeNoteSize(epd.int32ConstsCount * sizeof( CALDataSegmentDesc ));                   // int32 constants
        epd.notesSize += ComputeNoteSize(epd.bool32ConstsCount * sizeof( CALDataSegmentDesc ));                  // bool32 constants
        epd.notesSize += ComputeNoteSize(sizeof( unsigned int ));                                                // early exit 
        epd.notesSize += ComputeNoteSize(encoding->globalBuffersCount * sizeof( unsigned int ));                 // global buffer 
        epd.notesSize += ComputeNoteSize(encoding->constBuffersCount * sizeof( CALConstantBufferMask ));         // constant buffers
        epd.notesSize += ComputeNoteSize(encoding->inputSamplerMapCount * sizeof( CALSamplerMapEntry ));         // input sampler
        epd.notesSize += ComputeNoteSize(encoding->scratchBuffersCount * sizeof( unsigned int ));                // scratch buffer 
        epd.notesSize += ComputeNoteSize(encoding->persistentBuffersCount * sizeof( unsigned int ));             // persistent buffer 
        epd.notesSize += ComputeNoteSize(encoding->progInfosCount * sizeof( CALProgramInfoEntry ));              // program info
        epd.notesSize += ComputeNoteSize(encoding->subBufferCount * sizeof( CALSubConstantBufferEntry ));        // sub constant buffers

        /////////////////////////////////////////////////////////////////////////
        //
        //  setup load segment attributes
        //
        epd.loadOff = epd.notesOff + epd.notesSize;    // load offset
        epd.loadSize = 0;                              // load size (don't know this until all sections are computed)

        /////////////////////////////////////////////////////////////////////////
        //
        //  setup text segment attributes
        //
        epd.textOff = epd.loadOff;                     // text offset
        epd.textSize = encoding->textSize;             // text size

        /////////////////////////////////////////////////////////////////////////
        //
        //  setup data segment attributes
        //
        epd.dataOff = epd.textOff + epd.textSize;      // const data offset
        epd.dataSize = sizeof( AMUabiConstsData );     // const data

        /////////////////////////////////////////////////////////////////////////
        //
        //  setup symbol table attributes
        //
        unsigned int symtabEntries = 1 + encoding->symbolsCount; // symbol table entries (including required null entry at start)
        epd.symtabOff = epd.dataOff + epd.dataSize;              // symbol table offset
        epd.symtabSize = symtabEntries * sizeof( Elf32_Sym );    // symbol table size

        /////////////////////////////////////////////////////////////////////////
        //
        //  setup string table attributes
        //
        epd.strtabOff = epd.symtabOff + epd.symtabSize;          // string table offset
        epd.strtabSize = 2;                                      // string table size (including required null entry at start + end)
        
        for(unsigned i = 0; encoding->symbols && (i < encoding->symbolsCount); i++)
        {
            epd.strtabSize += (unsigned)strlen(encoding->symbols[i].name) + 1;
        }

        /////////////////////////////////////////////////////////////////////////
        //
        //  compute load segment size 
        //
        epd.loadSize = epd.textSize + epd.dataSize + epd.symtabSize + epd.strtabSize;
        
        /////////////////////////////////////////////////////////////////////////
        //
        //  update the total size for all encodings
        //
        encodingsSize += epd.notesSize + epd.loadSize;
    }


    /////////////////////////////////////////////////////////////////////////
    //
    //  setup elf binary attributes
    //
    unsigned int totalSize = ehdrSize
                           + phtSize
                           + edSize
                           + shstrtabSize
                           + shtSize
                           + encodingsSize;

    /////////////////////////////////////////////////////////////////////////
    //
    //  allocate the memory for the binary
    //
    char* _elf = ( char* ) malloc( totalSize );
    memset(_elf, 0, totalSize);

    /////////////////////////////////////////////////////////////////////////
    //
    //  populate the elf header
    //
    unsigned int abiVersion = 1;
    unsigned int elfType = ET_EXEC;
    AddElfHeader( _elf, abiVersion, elfType, EM_AMD_CALIMAGE_BINARY, phtOff, phtEntries, shtOff, shtEntries, shstrtabIndex );

    /////////////////////////////////////////////////////////////////////////
    //
    //  populate the program header table
    //
    char *cur = _elf + phtOff;
    cur = AddProgramHeader(cur, PT_AMD_CAL_ED, edOff, 0, 0, edSize, 0, 0, 0 );

    for (unsigned int i = 0; i < encodingCount; ++i)
    {
        AMUabiEncodingInfo* encoding = binaryInfo->encodings[i];
        AMUabiEncodingPackData& epd = encoding->packData;
        
        cur = AddProgramHeader(cur, PT_NOTE, epd.notesOff, 0, 0, epd.notesSize, 0, 0, 0 );
        cur = AddProgramHeader(cur, PT_LOAD, epd.loadOff, 0, 0, epd.loadSize, epd.loadSize, 0, 0 );
    };

    /////////////////////////////////////////////////////////////////////////
    //
    //  populate the encoding dictionary
    //
    cur = _elf + edOff;
    for (unsigned int i = 0; i < encodingCount; ++i)
    {
        AMUabiEncodingInfo* encoding = binaryInfo->encodings[i];
        AMUabiEncodingPackData& epd = encoding->packData;

        cur = AddEncodingDictionaryEntry(cur, encoding->signature.machine, encoding->signature.type, epd.notesOff, epd.notesSize + epd.loadSize, 0);
    }

    /////////////////////////////////////////////////////////////////////////
    //
    //  add section header string table
    //
    char* shstrtabStart = _elf + shstrtabOff;
    cur = shstrtabStart;
    *cur++ = '\0';

    //
    //  add section header string
    //
    shstrtabName = (unsigned int)(cur - shstrtabStart);
    for ( unsigned int i = 0; i < (unsigned int)strlen( shstrtabNameStr ); i++ )
    {
        *cur++ = shstrtabNameStr[ i ];
    }
    *cur++ = '\0';

    //
    //  add text string
    //
    unsigned int textName = (unsigned int)(cur - shstrtabStart);
    for ( unsigned int i = 0; i < (unsigned int)strlen( textNameStr ); i++ )
    {
        *cur++ = textNameStr[ i ];
    }
    *cur++ = '\0';

    //
    //  add data string
    //
    unsigned int dataName = (unsigned int)(cur - shstrtabStart);
    for ( unsigned int i = 0; i < (unsigned int)strlen( dataNameStr ); i++ )
    {
        *cur++ = dataNameStr[ i ];
    }
    *cur++ = '\0';

    //
    //  add symbol table string
    //
    unsigned int symtabName = (unsigned int)(cur - shstrtabStart);
    for ( unsigned int i = 0; i < (unsigned int)strlen( symtabNameStr ); i++ )
    {
        *cur++ = symtabNameStr[ i ];
    }
    *cur++ = '\0';

    //
    //  add string table string
    //
    unsigned int strtabName = (unsigned int)(cur - shstrtabStart);
    for ( unsigned int i = 0; i < (unsigned int)strlen( strtabNameStr ); i++ )
    {
        *cur++ = strtabNameStr[ i ];
    }
    *cur++ = '\0';

    /////////////////////////////////////////////////////////////////////////
    //
    //  add section header table
    //
    cur = _elf + shtOff;
    cur = AddSectionHeader( cur, 0, SHT_NULL, 0, 0, 0, 0, SHN_UNDEF, 0, 0, 0 );
    cur = AddSectionHeader( cur, shstrtabName, SHT_STRTAB, 0, 0, shstrtabOff, shstrtabSize, SHN_UNDEF, 0, 0, 0 );


    for (unsigned int i = 0; i < encodingCount; ++i)
    {
        AMUabiEncodingInfo* encoding = binaryInfo->encodings[i];
        AMUabiEncodingPackData& epd = encoding->packData;

        unsigned int strtabShdrIndex = 2 + 4*i + 3;  // the symbol table needs the index of the corresponding string table

        cur = AddSectionHeader( cur, textName, SHT_PROGBITS, 0, 0, epd.textOff, epd.textSize, SHN_UNDEF, 0, 0, 0 );
        cur = AddSectionHeader( cur, dataName, SHT_PROGBITS, 0, 0, epd.dataOff, epd.dataSize, SHN_UNDEF, 0, 0, sizeof( AMUabiConstsData ) );
        cur = AddSectionHeader( cur, symtabName, SHT_SYMTAB, 0, 0, epd.symtabOff, epd.symtabSize, strtabShdrIndex, 1, 0, sizeof( Elf32_Sym ) );
        cur = AddSectionHeader( cur, strtabName, SHT_STRTAB, 0, 0, epd.strtabOff, epd.strtabSize, SHN_UNDEF, 0, 0, 0 );
    }

    /////////////////////////////////////////////////////////////////////////
    //
    //  add notes
    //
    for (unsigned int i = 0; i < encodingCount; ++i)
    {
        AMUabiEncodingInfo* encoding = binaryInfo->encodings[i];
        AMUabiEncodingPackData& epd = encoding->packData;
    
        char* cur = _elf + epd.notesOff;
        cur = AddNote( cur, 
                       8, 
                       encoding->inputCount * sizeof( unsigned int ), 
                       ELF_NOTE_AMD_INPUTS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) encoding->inputs );
        cur = AddNote( cur, 
                       8, 
                       encoding->outputCount * sizeof( unsigned int ), 
                       ELF_NOTE_AMD_OUTPUTS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) encoding->outputs );
        cur = AddNote( cur, 
                       8, 
                       sizeof( unsigned int ), 
                       ELF_NOTE_AMD_CONDOUT, 
                       ELF_NOTE_AMD, 
                       ( const char* ) &encoding->condOut );
        cur = AddNote( cur, 
                       8, 
                       epd.float32ConstsCount * sizeof( CALDataSegmentDesc ), 
                       ELF_NOTE_AMD_FLOAT32CONSTS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) epd.float32Consts );
        cur = AddNote( cur, 
                       8, 
                       epd.int32ConstsCount * sizeof( CALDataSegmentDesc ), 
                       ELF_NOTE_AMD_INT32CONSTS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) epd.int32Consts );
        cur = AddNote( cur, 
                       8, 
                       epd.bool32ConstsCount * sizeof( CALDataSegmentDesc ), 
                       ELF_NOTE_AMD_BOOL32CONSTS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) epd.bool32Consts );
        cur = AddNote( cur, 
                       8, 
                       sizeof( unsigned int ), 
                       ELF_NOTE_AMD_EARLYEXIT, 
                       ELF_NOTE_AMD, 
                       ( const char* ) &encoding->earlyExit );
        cur = AddNote( cur, 
                       8, 
                       encoding->globalBuffersCount * sizeof( unsigned int ), 
                       ELF_NOTE_AMD_GLOBAL_BUFFERS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) encoding->globalBuffers );
        cur = AddNote( cur, 
                       8, 
                       encoding->constBuffersCount * sizeof( CALConstantBufferMask ), 
                       ELF_NOTE_AMD_CONSTANT_BUFFERS, 
                       ELF_NOTE_AMD, 
                       (const char* ) encoding->constBuffers );
        cur = AddNote( cur, 
                       8, 
                       encoding->inputSamplerMapCount * sizeof( CALSamplerMapEntry ), 
                       ELF_NOTE_AMD_INPUT_SAMPLER_MAPS, 
                       ELF_NOTE_AMD, 
                       (const char* ) encoding->inputSamplerMaps );
        cur = AddNote( cur, 
                       8, 
                       encoding->scratchBuffersCount * sizeof( unsigned int ), 
                       ELF_NOTE_AMD_SCRATCH_BUFFERS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) encoding->scratchBuffers );
        cur = AddNote( cur, 
                       8, 
                       encoding->persistentBuffersCount * sizeof( unsigned int ), 
                       ELF_NOTE_AMD_PERSISTENT_BUFFERS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) encoding->persistentBuffers );
        cur = AddNote( cur, 
                       8, 
                       encoding->progInfosCount * sizeof( CALProgramInfoEntry ), 
                       ELF_NOTE_AMD_PROGINFO, 
                       ELF_NOTE_AMD, 
                       ( const char* ) encoding->progInfos );
        cur = AddNote( cur, 
                       8, 
                       encoding->subBufferCount * sizeof( CALSubConstantBufferEntry ), 
                       ELF_NOTE_AMD_SUB_CONSTANT_BUFFERS, 
                       ELF_NOTE_AMD, 
                       ( const char* ) encoding->subBuffers );
        /////////////////////////////////////////////////////////////////////////
        //
        //  add text
        //
        cur = _elf + epd.textOff;
        for ( unsigned int i = 0; i < epd.textSize; i++ )
        {
            *cur++ = ( ( char* ) encoding->textData ) [ i ];
        }

        /////////////////////////////////////////////////////////////////////////
        //
        //  add const data
        //
        cur = _elf + epd.dataOff;
        memset( cur, 0, sizeof( AMUabiConstsData ) );
        AddConstData( cur, encoding->litConstsCount, encoding->litConsts );

        /////////////////////////////////////////////////////////////////////////
        //
        //  add string table
        //
        char* strtabStart = _elf + epd.strtabOff;
        cur = strtabStart;
        *cur++ = '\0';
        for(unsigned i = 0; i < encoding->symbolsCount; i++)
        {
            char const* p = encoding->symbols[i].name;
            strcpy(cur, p);
            cur += (unsigned)strlen(p) + 1 ;
        }
        *cur++ = '\0';


        /////////////////////////////////////////////////////////////////////////
        //
        //  add symbol table
        //
        unsigned int symoff = 1;                           // initial string offset (first entry is '\0')
        cur = _elf + epd.symtabOff;                        // start address of symbol table
        cur = AddSymbol( cur, STN_UNDEF, 0, 0, 0, 0, 0 );  // initial symbol
        for(unsigned i = 0; i < encoding->symbolsCount; i++)
        {
            AMUabiUserSymbol const *s = encoding->symbols + i;
            cur = AddSymbol(cur, symoff, s->value, 0, 0, 0, s->section);
            symoff += 1 + (unsigned)strlen(s->name);
        }
    }


    /////////////////////////////////////////////////////////////////////////
    //
    //  update return values
    //
    *binarySize = totalSize;
    *binaryData = _elf;


    for (unsigned int i = 0; i < encodingCount; ++i)
    {
        AMUabiEncodingInfo* encoding = binaryInfo->encodings[i];
        encoding->packData.CleanUp();
    }

    return 1;
}

static void
ExtractEncodingDictionary( char* elfBaseAddr, Elf32_Phdr* phdr, AMUabiMultiBinaryInfo* binaryInfo)
{
    char* edBaseAddr = elfBaseAddr + phdr->p_offset;
    char* edEndAddr = edBaseAddr + phdr->p_filesz;

    CALEncodingDictionaryEntry* entry = reinterpret_cast<CALEncodingDictionaryEntry*>( edBaseAddr );
    while ( ( char* ) entry < edEndAddr )
    {
        AMUabiEncodingInfo* encodingInfo = new AMUabiEncodingInfo;

        encodingInfo->signature.machine = entry->d_machine;
        encodingInfo->signature.type = entry->d_type;
        encodingInfo->unpackData.offset = entry->d_offset;
        encodingInfo->unpackData.size = entry->d_size;

        binaryInfo->encodings.push_back(encodingInfo);

        ++entry;
    }
}

typedef struct AMUabiNoteHeaderRec
{
    unsigned int namesz;
    unsigned int descsz;
    unsigned int type;
}
AMUabiNoteHeader;

static char*
GetNoteData(AMUabiNoteHeader *nhdr)
{
    char *tmp = reinterpret_cast<char*>(nhdr);
    return tmp + sizeof(AMUabiNoteHeader) + nhdr->namesz ;
}

static void
ExtractNotes( char* elfBaseAddr, Elf32_Phdr* phdr, AMUabiEncodingInfo* encoding )
{
    char * noteBaseAddr = elfBaseAddr + phdr->p_offset;
    char* noteEndAddr = noteBaseAddr + phdr->p_filesz;

    unsigned int float32ConstsCount = 0;
    CALDataSegmentDesc* float32Consts = 0;
    unsigned int int32ConstsCount = 0;
    CALDataSegmentDesc* int32Consts = 0;
    unsigned int bool32ConstsCount = 0;
    CALDataSegmentDesc* bool32Consts = 0;

    AMUabiNoteHeader* nhdr = ( AMUabiNoteHeader* ) ( noteBaseAddr );
    while ( ( char* ) nhdr < noteEndAddr )
    {
        switch ( nhdr->type )
        {
        case ELF_NOTE_AMD_PROGINFO:
            encoding->progInfosCount = nhdr->descsz / sizeof(CALProgramInfoEntry);
            encoding->progInfos = (encoding->progInfosCount > 0) ? reinterpret_cast<CALProgramInfoEntry*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_INPUTS:
            encoding->inputCount = nhdr->descsz / sizeof(unsigned int);
            encoding->inputs = (encoding->inputCount > 0) ? reinterpret_cast<unsigned int*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_OUTPUTS:
            encoding->outputCount = nhdr->descsz / sizeof(unsigned int);
            encoding->outputs = (encoding->outputCount > 0) ? reinterpret_cast<unsigned int*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_CONDOUT:
            encoding->condOut = *reinterpret_cast<unsigned int*>(GetNoteData(nhdr));
            break;
        case ELF_NOTE_AMD_FLOAT32CONSTS:
            float32ConstsCount = nhdr->descsz / sizeof(CALDataSegmentDesc);
            float32Consts = (float32ConstsCount > 0) ? reinterpret_cast<CALDataSegmentDesc*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_INT32CONSTS:
            int32ConstsCount = nhdr->descsz / sizeof(CALDataSegmentDesc);
            int32Consts = (int32ConstsCount > 0) ? reinterpret_cast<CALDataSegmentDesc*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_BOOL32CONSTS:
            bool32ConstsCount = nhdr->descsz / sizeof(CALDataSegmentDesc);
            bool32Consts = (bool32ConstsCount > 0) ? reinterpret_cast<CALDataSegmentDesc*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_EARLYEXIT:
            encoding->earlyExit = *reinterpret_cast<unsigned int*>(GetNoteData(nhdr));
            break;
        case ELF_NOTE_AMD_GLOBAL_BUFFERS:
            encoding->globalBuffersCount = nhdr->descsz / sizeof( unsigned int ), 
            encoding->globalBuffers = (encoding->globalBuffersCount > 0) ? reinterpret_cast<unsigned int*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_CONSTANT_BUFFERS:
            encoding->constBuffersCount = nhdr->descsz / sizeof( CALConstantBufferMask ), 
            encoding->constBuffers = (encoding->constBuffersCount > 0) ? reinterpret_cast<CALConstantBufferMask*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_INPUT_SAMPLER_MAPS:
            encoding->inputSamplerMapCount = nhdr->descsz / sizeof( CALSamplerMapEntry ), 
            encoding->inputSamplerMaps = (encoding->inputSamplerMapCount > 0) ? reinterpret_cast<CALSamplerMapEntry*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_SCRATCH_BUFFERS:
            encoding->scratchBuffersCount = nhdr->descsz / sizeof( unsigned int ), 
            encoding->scratchBuffers = (encoding->scratchBuffersCount > 0) ? reinterpret_cast<unsigned int*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_PERSISTENT_BUFFERS:
            encoding->persistentBuffersCount = nhdr->descsz / sizeof( unsigned int ), 
            encoding->persistentBuffers = (encoding->persistentBuffersCount > 0) ? reinterpret_cast<unsigned int*>(GetNoteData(nhdr)) : 0;
            break;
        case ELF_NOTE_AMD_SUB_CONSTANT_BUFFERS:
            encoding->subBufferCount = nhdr->descsz / sizeof( CALSubConstantBufferEntry ), 
            encoding->subBuffers = (encoding->subBufferCount > 0) ? reinterpret_cast<CALSubConstantBufferEntry*>(GetNoteData(nhdr)) : 0;
            break;
        default:
            break;
        }
    
        nhdr = (AMUabiNoteHeader*) (((char*) nhdr) + sizeof(AMUabiNoteHeader) + nhdr->namesz + nhdr->descsz);
    }

    encoding->litConstsCount = float32ConstsCount + int32ConstsCount + bool32ConstsCount;
    encoding->litConsts = 0;
    if (encoding->litConstsCount > 0)
    { 
        encoding->unpackData.litConstsData = reinterpret_cast<AMUabiLiteralConst*>(malloc(encoding->litConstsCount * sizeof(AMUabiLiteralConst)));
        encoding->litConsts = encoding->unpackData.litConstsData;

        for (unsigned int i = 0; i < float32ConstsCount; i++)
        {
            encoding->litConsts[i].type = AMU_ABI_FLOAT32;
            encoding->litConsts[i].addr = float32Consts[i].offset;
        }

        for (unsigned int i = 0; i < int32ConstsCount; i++)
        {
            encoding->litConsts[i].type = AMU_ABI_INT32;
            encoding->litConsts[i].addr = int32Consts[i].offset;
        }

        for (unsigned int i = 0; i < bool32ConstsCount; i++)
        {
            encoding->litConsts[i].type = AMU_ABI_BOOL32;
            encoding->litConsts[i].addr = bool32Consts[i].offset;
        }
    }
}


static void
ExtractText( char* elfBaseAddr, Elf32_Shdr* shdr, AMUabiEncodingInfo* encoding )
{
    encoding->textSize = shdr->sh_size;
    encoding->textData = reinterpret_cast<void*>(elfBaseAddr + shdr->sh_offset);
}

static void
ExtractData( char* elfBaseAddr, Elf32_Shdr* shdr, AMUabiEncodingInfo* encoding )
{
    AMUabiConstsData* data = reinterpret_cast<AMUabiConstsData*> (elfBaseAddr + shdr->sh_offset);

    for (unsigned int i = 0; (i < encoding->litConstsCount) && (i < AMUabiMaxLiteralConsts); ++i)
    {
        unsigned int r = encoding->litConsts[i].addr;
        switch (encoding->litConsts[i].type)
        {
        case AMU_ABI_SAMPLER:
        case AMU_ABI_FLOAT32:
            {
                if ( i < 256 )
                {
                    encoding->litConsts[i].value.float32[ 0 ] = data->float32[ r * 4 + 0 ];
                    encoding->litConsts[i].value.float32[ 1 ] = data->float32[ r * 4 + 1 ];
                    encoding->litConsts[i].value.float32[ 2 ] = data->float32[ r * 4 + 2 ];
                    encoding->litConsts[i].value.float32[ 3 ] = data->float32[ r * 4 + 3 ];
                }
                break;
            }
        case AMU_ABI_INT32:
            {
                if ( r < 32 )
                {
                    encoding->litConsts[i].value.int32[ 0 ] = data->int32[ r * 4 + 0 ];
                    encoding->litConsts[i].value.int32[ 1 ] = data->int32[ r * 4 + 1 ];
                    encoding->litConsts[i].value.int32[ 2 ] = data->int32[ r * 4 + 2 ];
                    encoding->litConsts[i].value.int32[ 3 ] = data->int32[ r * 4 + 3 ];
                }
                break;
            }
        case AMU_ABI_BOOL32:
            {
                if ( r < 32 )
                {
                    encoding->litConsts[i].value.bool32[ 0 ] = data->bool32[ r * 4 + 0 ];
                }
                break;
            }
        case AMU_ABI_UNDEF:
        case AMU_ABI_MAXTYPES:
        default:
            break;
        }
    }
}

static void
ExtractSymbolTable( char* elfBaseAddr, Elf32_Shdr* shdr, Elf32_Shdr* strtab_shdr, AMUabiEncodingInfo* encoding )
{
    // When computing the number of user visible symbols, subtract the null 
    // symbol at the start of the table.
    unsigned int symbolCount = shdr->sh_size / sizeof(AMUabiSymbol);
    assert(symbolCount > 0);
    
    encoding->symbolsCount = symbolCount - 1; 
    encoding->symbols = 0;
    if (encoding->symbolsCount > 0)
    {
        encoding->unpackData.symbolsData = reinterpret_cast<AMUabiUserSymbol*>(malloc(encoding->symbolsCount * sizeof(AMUabiUserSymbol)));
        encoding->symbols = encoding->unpackData.symbolsData;

        AMUabiSymbol* symbols = reinterpret_cast<AMUabiSymbol*> (elfBaseAddr + shdr->sh_offset);
        char* strtab = elfBaseAddr + strtab_shdr->sh_offset;

        // Copy symbol information out of the table.  Once again remembering
        // to skip the beginning null symbol.
        for (unsigned int i = 0; i < encoding->symbolsCount; ++i)
        {
            encoding->symbols[i].name = &strtab[symbols[i+1].name];
            encoding->symbols[i].value = symbols[i+1].value;
            encoding->symbols[i].section = symbols[i+1].section;
        }
    }
}


CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryUnpack(AMUabiMultiBinary binary, void* binaryData)
{
    assert(binary != 0);
    assert(binaryData != 0);

    AMUabiMultiBinaryInfo* binaryInfo = reinterpret_cast<AMUabiMultiBinaryInfo*>(binary);

    if (binaryInfo->encodings.size() > 0)
    {
        binaryInfo->DeleteEncodings();
    }

    Elf32_Ehdr* ehdr = 0;
    Elf32_Phdr* phdr = 0;
    Elf32_Shdr* shdr = 0;

    char* elfBaseAddr = ( char* ) binaryData;

    ehdr = ( Elf32_Ehdr* ) elfBaseAddr;

    // Check the validity of the elf header
    if ((ehdr->e_ident[ EI_MAG0 ]       != ELFMAG0) ||
        (ehdr->e_ident[ EI_MAG1 ]       != ELFMAG1) ||
        (ehdr->e_ident[ EI_MAG2 ]       != ELFMAG2) ||
        (ehdr->e_ident[ EI_MAG3 ]       != ELFMAG3) ||
        (ehdr->e_ident[ EI_CLASS ]      != ELFCLASS32) ||
        (ehdr->e_ident[ EI_DATA ]       != ELFDATA2LSB) ||
        (ehdr->e_ident[ EI_VERSION ]    != EV_CURRENT) ||
        (ehdr->e_ident[ EI_OSABI ]      != ELFOSABI_CALIMAGE) ||
        (ehdr->e_ident[ EI_ABIVERSION ] != 1) ||
        (ehdr->e_type                   != ET_EXEC) ||
        (ehdr->e_machine                != EM_AMD_CALIMAGE_BINARY))
    {
        return 0;
    }


    // Find the program header table from the elf header
    if ( ehdr->e_phoff > 0 )
    {
        phdr = ( Elf32_Phdr* ) ( elfBaseAddr + ehdr->e_phoff );
    }
    else
    {
        return 0;
    }

    // Find the section header table from the elf header
    if ( ehdr->e_shoff > 0 )
    {
        shdr = ( Elf32_Shdr* ) ( elfBaseAddr + ehdr->e_shoff );
    }
    else
    {
        return 0;
    }

    // Find and process the encoding dictionary
    for ( unsigned int i = 0; i < ehdr->e_phnum; i++ )
    {
        switch ( phdr[ i ].p_type )
        {
        case PT_AMD_CAL_ED:
            ExtractEncodingDictionary( elfBaseAddr, &phdr[ i ], binaryInfo );
            break;
        default:
            break;
        }
    }

    // Check to make sure some encodings were found, if not bail.
    unsigned int encodingCount = binaryInfo->encodings.size();
    if (encodingCount == 0)
    {
        return 0;
    }

    // Unpack each encoding.  
    char* elfShStrTabAddr = ( char* ) ( elfBaseAddr + shdr[ ehdr->e_shstrndx ].sh_offset );
    unsigned int elfShStrTabSize = shdr[ ehdr->e_shstrndx ].sh_size;
    for (unsigned int e = 0; e < encodingCount; ++e)
    {
        AMUabiEncodingInfo* encoding = binaryInfo->encodings[e];
        unsigned int encodingStart = encoding->unpackData.offset;
        unsigned int encodingEnd = encoding->unpackData.offset + encoding->unpackData.size;

        // Extract note segment into encoding
        for ( unsigned int i = 0; i < ehdr->e_phnum; i++ )
        {
            switch ( phdr[ i ].p_type )
            {
            case PT_NOTE:
                if ((phdr[i].p_offset >= encodingStart) && (phdr[i].p_offset < encodingEnd))
                {
                    ExtractNotes( elfBaseAddr, &phdr[ i ], encoding );
                }
                break;
            default:
                break;
            }
        }

        // Extract load segment into encoding 
        for ( unsigned int i = 0; i < ehdr->e_shnum; i++ )
        {
            // Does the section have a valid name?
            if ( shdr[ i ].sh_name > elfShStrTabSize )
            {
                continue;
            }

            // Does the section fall within the encoding's segments?
            if ( (shdr[i].sh_offset >= encodingStart) && (shdr[i].sh_offset < encodingEnd))
            {
                if ( strcmp( elfShStrTabAddr + shdr[ i ].sh_name, ".text" ) == 0 )
                {
                    ExtractText( elfBaseAddr, &shdr[ i ], encoding );
                }
                else if ( strcmp( elfShStrTabAddr + shdr[ i ].sh_name, ".data" ) == 0 )
                {
                    ExtractData( elfBaseAddr, &shdr[ i ], encoding );
                }
                else if ( strcmp( elfShStrTabAddr + shdr[ i ].sh_name, ".symtab" ) == 0 )
                {
                    Elf32_Shdr* strtab_shdr = &shdr[ shdr[ i ].sh_link ];
                    ExtractSymbolTable( elfBaseAddr, &shdr[ i ], strtab_shdr, encoding );
                }
            }
        }
    }

    return 1;
}

CALAPI unsigned int CALAPIENTRY amuABIMachineIsISA(unsigned int machine)
{
    switch(machine)
    {
    case ED_AMD_CAL_MACHINE_R600_ISA: 
    case ED_AMD_CAL_MACHINE_R610_ISA:
    case ED_AMD_CAL_MACHINE_R630_ISA:
    case ED_AMD_CAL_MACHINE_R670_ISA:
    case ED_AMD_CAL_MACHINE_R710_ISA:
    case ED_AMD_CAL_MACHINE_R730_ISA:
    case ED_AMD_CAL_MACHINE_R770_ISA:
        return 1;
        break;
    case ED_AMD_CAL_MACHINE_R6XX_IL:
    case ED_AMD_CAL_MACHINE_R7XX_IL:
        return 0;
        break;
    default:
        return 0;
        break;
    }
}

CALAPI unsigned int CALAPIENTRY amuABIMachineIsIL(unsigned int machine)
{
    switch(machine)
    {
    case ED_AMD_CAL_MACHINE_R600_ISA: 
    case ED_AMD_CAL_MACHINE_R610_ISA:
    case ED_AMD_CAL_MACHINE_R630_ISA:
    case ED_AMD_CAL_MACHINE_R670_ISA:
    case ED_AMD_CAL_MACHINE_R770_ISA:
        return 0;
        break;
    case ED_AMD_CAL_MACHINE_R6XX_IL:
    case ED_AMD_CAL_MACHINE_R7XX_IL:
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

CALAPI unsigned int CALAPIENTRY amuABIMultiBinaryGetSize(unsigned int* binarySize, void* binaryData)
{
    assert(binarySize != 0);
    assert(binaryData != 0);


    Elf32_Ehdr* ehdr = 0;
    Elf32_Phdr* phdr = 0;
    Elf32_Shdr* shdr = 0;

    char* elfBaseAddr = ( char* ) binaryData;

    ehdr = ( Elf32_Ehdr* ) elfBaseAddr;

    // Check the validity of the elf header
    if ((ehdr->e_ident[ EI_MAG0 ]       != ELFMAG0) ||
        (ehdr->e_ident[ EI_MAG1 ]       != ELFMAG1) ||
        (ehdr->e_ident[ EI_MAG2 ]       != ELFMAG2) ||
        (ehdr->e_ident[ EI_MAG3 ]       != ELFMAG3) ||
        (ehdr->e_ident[ EI_CLASS ]      != ELFCLASS32) ||
        (ehdr->e_ident[ EI_DATA ]       != ELFDATA2LSB) ||
        (ehdr->e_ident[ EI_VERSION ]    != EV_CURRENT) ||
        (ehdr->e_ident[ EI_OSABI ]      != ELFOSABI_CALIMAGE) ||
        (ehdr->e_ident[ EI_ABIVERSION ] != 1) ||
        (ehdr->e_type                   != ET_EXEC) ||
        (ehdr->e_machine                != EM_AMD_CALIMAGE_BINARY))
    {
        return 0;
    }


    // Find the program header table from the elf header
    if ( ehdr->e_phoff > 0 )
    {
        phdr = ( Elf32_Phdr* ) ( elfBaseAddr + ehdr->e_phoff );
    }
    else
    {
        return 0;
    }

    // Find the section header table from the elf header
    if ( ehdr->e_shoff > 0 )
    {
        shdr = ( Elf32_Shdr* ) ( elfBaseAddr + ehdr->e_shoff );
    }
    else
    {
        return 0;
    }

    // Account for the size of the elf header, the program header table and the section header table.
    unsigned int totalSize = ehdr->e_ehsize + (ehdr->e_phentsize * ehdr->e_phnum) + (ehdr->e_shentsize * ehdr->e_shnum);

    // Add the size of the section header string table.
    totalSize += shdr[ ehdr->e_shstrndx ].sh_size;

    // Add the size of the encoding dictionary and the encodings.
    // There is just one encoding dictionary but each encoding has
    // a note and a load section.  Just identify these sections and
    // add there sizes to the total size.
    for ( unsigned int i = 0; i < ehdr->e_phnum; i++ )
    {
        switch ( phdr[ i ].p_type )
        {
        case PT_NOTE:
        case PT_LOAD:
        case PT_AMD_CAL_ED:
            totalSize += phdr[i].p_filesz;
            break;
        default:
            break;
        }
    }

    *binarySize = totalSize;
    return 1;
}
