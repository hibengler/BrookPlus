/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
All rights reserved.


The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the
contents of this directory.

****************************************************************************/

#include "logger.hpp"
#include "cgInterface.h"
#include "cgKernelIL.h"
#include "main.h"

//#define SHOW_META_DATA

// ----------------------------------------------------------------------------
#ifndef HLFRONTEND_DIRECT_LINK
#include <calutAMDhlslCompiler.h>
#include <amuABI.h>


static void
extractILmetaData( CgPass& pass, std::ostream& outs, CALobject obj )
{
    LOG(LOG_FN_ENTRY, "extractILmetaData(%p, %p)\n", &outs, obj);

    AMUabiMultiBinary multb = 0;
    amuABIMultiBinaryCreate( &multb );
    if ( !multb )
    {
        return;
    }
    if ( !amuABIMultiBinaryUnpack( multb, obj ) )
    {
        amuABIMultiBinaryDestroy( multb );
        return;
    }
    unsigned int encods;
    if ( !amuABIMultiBinaryGetEncodingCount( &encods, multb ) )
    {
            amuABIMultiBinaryDestroy( multb );
        return;
    }
    for ( unsigned int ii = 0; ii < encods; ++ii )
    {
        AMUabiEncoding encoding;
        if ( !amuABIMultiBinaryGetEncoding( &encoding, multb, ii ) )
        {
            amuABIMultiBinaryDestroy( multb );
            return;
        }
        unsigned int symsnum;
        AMUabiUserSymbol* symbols;
        if ( !amuABIEncodingGetSymbols( &symsnum, &symbols, encoding ) )
        {
            amuABIMultiBinaryDestroy( multb );
            return;
        }
        unsigned int* inputs;
        unsigned int inpsnum;
        if ( !amuABIEncodingGetInputs( &inpsnum, &inputs, encoding ) )
        {
            amuABIMultiBinaryDestroy( multb );
            return;
        }
        unsigned int* outputs;
        unsigned int outpsnum;
        if ( !amuABIEncodingGetOutputs( &outpsnum, &outputs, encoding ) )
        {
            amuABIMultiBinaryDestroy( multb );
            return;
        }
        unsigned int cbufsnum;
        CALConstantBufferMask* cbufs;
        if ( !amuABIEncodingGetConstBuffers( &cbufsnum, &cbufs, encoding ) )
        {
            amuABIMultiBinaryDestroy( multb );
            return;
        }
        unsigned int subsnum;
        CALSubConstantBufferEntry* subs;
        if ( !amuABIEncodingGetSubConstantBuffers( &subsnum, &subs, encoding ) )
        {
            amuABIMultiBinaryDestroy( multb );
            return;
        }
        const char** inpnames = new const char*[inpsnum];
        const char** outnames = new const char*[outpsnum];
        const char** bufnames = new const char*[cbufsnum];
        const char** subnames = new const char*[subsnum];
        for ( unsigned int ii = 0; ii < symsnum; ++ii )
        {
            switch ( symbols[ii].section )
            {
                case ELF_NOTE_AMD_INPUTS:
                    inpnames[symbols[ii].value] = symbols[ii].name;
                    continue;
                case ELF_NOTE_AMD_OUTPUTS:
                    outnames[symbols[ii].value] = symbols[ii].name;
                    continue;
                case ELF_NOTE_AMD_CONSTANT_BUFFERS:
                    bufnames[symbols[ii].value] = symbols[ii].name;
                    continue;
                case ELF_NOTE_AMD_SUB_CONSTANT_BUFFERS:
                    subnames[symbols[ii].value] = symbols[ii].name;
                    continue;
                default: ; // no idea
            }
        }
        for ( unsigned int ii = 0; ii < inpsnum; ++ii )
        {
#ifdef SHOW_META_DATA
            outs << ";shader input: " << inpnames[ii] << " => " << inputs[ii] << std::endl;
#endif
            pass.adjustSampler( inpnames[ii], inputs[ii] );
        }
        for ( unsigned int ii = 0; ii < outpsnum; ++ii )
        {
#ifdef SHOW_META_DATA
            outs << ";shader output: " << outnames[ii] << " => " << outputs[ii] << std::endl;
#endif
            pass.adjustOutput( outnames[ii], outputs[ii] );
        }
        for ( unsigned int ii = 0; ii < cbufsnum; ++ii )
        {
#ifdef SHOW_META_DATA
            outs << ";shader constant buffer: " << bufnames[ii] << " => " << cbufs[ii].index << std::endl;
#endif
            for (unsigned int jj = 0; jj < subsnum; ++jj )
            {
                if ( subs[jj].index == cbufs[ii].index )
                {
#ifdef SHOW_META_DATA
                    outs << ";shader constant var: " << subnames[jj]
                         << " => offset " << subs[jj].offset
                         << ", size " << subs[jj].size << std::endl;
#endif
                    pass.adjustConstant( subnames[jj], subs[jj].offset, subs[jj].size );
                }
            }
        }
        delete [] inpnames;
        delete [] outnames;
        delete [] bufnames;
        delete [] subnames;
    } // loop over the encodings
    amuABIMultiBinaryDestroy( multb );
} // extractILmetaData

static unsigned int
obj2txtIL( CALobject obj, std::ostream& outs )
{
    AMUabiMultiBinary multb = 0;
    amuABIMultiBinaryCreate( &multb );
    if ( !multb )
    {
        return 0;
    }
    if ( !amuABIMultiBinaryUnpack( multb, obj ) )
    {
        amuABIMultiBinaryDestroy( multb );
        return 0;
    }
    unsigned int encods;
    if ( !amuABIMultiBinaryGetEncodingCount( &encods, multb ) )
    {
        amuABIMultiBinaryDestroy( multb );
        return 0;
    }
    // Expect only single encoding.  Can't handle multiple encodings.
    if ( 1 < encods )
    {
        amuABIMultiBinaryDestroy( multb );
        return 0;
    }
    AMUabiEncoding encoding;
    if ( !amuABIMultiBinaryGetEncoding( &encoding, multb, 0 ) )
    {
        amuABIMultiBinaryDestroy( multb );
        return 0;
    }
    unsigned int size;
    void* data;
    if ( !amuABIEncodingGetText( &size, &data, encoding ) )
    {
        amuABIMultiBinaryDestroy( multb );
        return 0;
    }
    outs << (char*)data;
    amuABIMultiBinaryDestroy( multb );
    return size;
} // obj2txtIL

#else

template<class X, class C>
void
foreachMetadata(CgPass &pass, void (*pfunc)(CgPass&, X const&, void*), C const& c, void* data)
{
        typename C::const_iterator i;
        for(i = c.begin(); i != c.end();  ++i)
                pfunc(pass, *i, data);
}

static void
output2Text(CgPass &pass, ShaderDescription::InputOutputVariableDesc const& io, void *data){
    LOG(LOG_FN_ENTRY, "output2Text(%p, %p, %p)\n", &pass, &io, data);
    std::ostream &outs = *((std::ostream*)data);
    outs << ";shader output: " << io.Variable.Name->c_str()
        << " => " << io.Registers.front().RegisterNumber << std::endl;

    pass.adjustOutput(io.Variable.Name->c_str(), io.Registers.front().RegisterNumber);
}

static void
texture2Text(CgPass &pass, ShaderDescription::TextureDesc const& tex, void *data){
    LOG(LOG_FN_ENTRY, "texture2Text(%p, %p, %p)\n", &pass, &tex, data);
    std::ostream &outs = *((std::ostream*)data);
    outs << ";shader input: " << tex.Variable.Name->c_str()
        << " => " << tex.ResourceNumber << std::endl;

    pass.adjustSampler(tex.Variable.Name->c_str(), tex.ResourceNumber);
}

static void
constant2Text(CgPass &pass, ShaderDescription::ConstantBufferDesc const& buf, void *data){
    LOG(LOG_FN_ENTRY, "constant2Text(%p, %p, %p)\n", &pass, &buf, data);
    std::ostream &outs = *((std::ostream*)data);
    outs << ";shader constant buffer: " << buf.Name->c_str()
        << " => " << buf.BufferNumber << std::endl;

    for (int i = 0; i < AsInt(buf.Fields.size()); i ++)
    {
        const ShaderDescription::ConstantBufferFieldDesc & field = buf.Fields[i];
        outs << ";shader constant var: " << field.Variable.Name->c_str()
            << " => offset " << field.ByteOffset
            << ", size " << field.ByteSize << std::endl;

        pass.adjustConstant(field.Variable.Name->c_str(), field.ByteOffset, field.ByteSize);
    }
}

#endif

// ----------------------------------------------------------------------------

char *
CgKernelIL::genPassCompile(CgPass &pass, std::string &shadername, const char *hlslstring, ShaderType shaderType)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genPassCompile(%p, %s, %s)\n", &pass, shadername.c_str(), hlslstring);
    if (globals.keepFiles)
    {
        std::string inputfname = shadername + ".hlsl";

        std::ofstream hlslout;
        hlslout.open(inputfname.c_str());
        if (hlslout.fail())
        {
            CGEXIT("***Unable to open file %s \n", inputfname.c_str());
        }

        hlslout << "shader: " << shadername << std::endl;
        hlslout << hlslstring << std::endl;
        hlslout << "end" << std::endl;

        hlslout.close();
    }

#if 0
    {
        FILE *tmpfp = fopen("tmp.hlsl", "rb");

        cgAssert(tmpfp!=NULL, " problem with opening tmp.hlsl");
        fseek(tmpfp, 0, SEEK_END);
        int lsize = AsInt(ftell(tmpfp));
        char *tmpbuf = (char *) malloc(lsize + 1);

        cgAssert(lsize > 0, " problem with opening tmp.hlsl");

        rewind(tmpfp);
        fread(tmpbuf, 1, lsize, tmpfp);
        tmpbuf[lsize] = 0;
        fclose(tmpfp);

        hlslstring = tmpbuf;
    }
#endif

#ifdef HLFRONTEND_DIRECT_LINK

    std::stringstream input, output;

    ConstShaderDescriptionPtr info;

    input.write(hlslstring, strlen(hlslstring));

    HLFrontendSetParameter("nocomments", true);
    HLFrontendSetParameter("cal", true);
    HLFrontendSetParameter("txt", true);
    //HLFrontendSetErrorMessageStream

    if (HLFrontendCompileBuffer(input, output, HLFrontendFlags::TYPE_PS,
                                HLFrontendFlags::OUTPUT_TXT, &info) == false)
    {
        cgErrorMsg("Problem with compiling %s.hlsl", shadername.c_str());
        return NULL;
    }

    foreachMetadata(pass, output2Text, info->Outputs, &output);
    foreachMetadata(pass, texture2Text, info->Textures, &output);
    foreachMetadata(pass, constant2Text, info->ConstantBuffers, &output);

#else // HLFRONTEND_DIRECT_LINK

    CALobject ilobject;
    std::stringstream output;

    CALCLprogramType programType = CAL_PROGRAM_TYPE_PS;
    switch(shaderType)
    {
        case ComputeShader:
            programType = CAL_PROGRAM_TYPE_CS;
            break;
        case PixelShader:
        default:
            programType = CAL_PROGRAM_TYPE_PS;

    }
    if ( CAL_RESULT_OK != calutAMDhlslCompileProgram( &ilobject,
                                                         programType,
                                                         hlslstring,
                                                         CAL_TARGET_600,
                                                         CALUT_OUTPUT_TXT ) )
    {
        cgErrorMsg("Problem with compiling %s.hlsl", shadername.c_str());
        return NULL;
    }

    obj2txtIL( ilobject, output );
    extractILmetaData( pass, output, ilobject );


#endif // ! HLFRONTEND_DIRECT_LINK

    char *ilstring = strdup( output.str().c_str() );

    if (ilstring && globals.keepFiles)
    {
        std::string outputfname = shadername + ".il";

        FILE *fp = fopen(outputfname.c_str(), "wb+");
        if (fp == NULL)
        {
            CGEXIT("***Unable to open file %s\n", outputfname.c_str());
        }

        fwrite(ilstring, sizeof(char), strlen(ilstring), fp);
        fclose(fp);
    }

    return ilstring;
}

// ----------------------------------------------------------------------------
