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

/*
 * extract_il.cpp
 *
 *  Currently, IL is generated in comments to the HLSL with the prefix "// IL:".
 */

#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include <sstream>
#include <iomanip>
#include <fstream>
extern "C"
{
#include <stdio.h>
#include <string.h>
#include <assert.h>
}

#include "janitorial.hpp"
#include "logger.hpp"
#include "main.h"
#include "subprocess.h"
#include "project.h"
#include "extract_il.h"
#include "generate_il.h"

using namespace bril;

char *strip_il(const char *shader)
{
    LOG(LOG_FN_ENTRY, "strip_il(%s)\n", shader);
    size_t non_il_bytes = 0;

    // Pass one: count non-IL 
    const size_t il_prefix_length = strlen(IL_PREFIX_STRING);

    char const *last = shader;
    char const *next = shader;

    while ((next = strstr(next, IL_PREFIX_STRING)) != NULL)
    {
        non_il_bytes += next - last;

        next += il_prefix_length;
        while (*next && *next != '\n')
            ++next;

        if (*next)
            ++next;

        last = next;
    }

    while (*last)
    {
        ++non_il_bytes;
        ++last;
    }

    // Pass two: copy non-IL bytes. 

    char *non_il_code = (char *) malloc(non_il_bytes + 1);

    char *dst = non_il_code;

    last = shader;
    next = shader;
    while ((next = strstr(next, IL_PREFIX_STRING)) != NULL)
    {
        while (last < next)
            *dst++ = *last++;

        next += il_prefix_length;
        while (*next && *next != '\n')
            ++next;

        if (*next)
            ++next;

        last = next;
    }

    while (*last)
        *dst++ = *last++;

    *dst = 0;

    return non_il_code;
}

char *extract_il(const char *shader, bool leave_prefix)
{
    LOG(LOG_FN_ENTRY, "extract_il(%s, %d)\n", shader, leave_prefix);
    int il_bytes = 0;

    // Pass one: count IL 
    const size_t prefix_skip = leave_prefix ? 0 : strlen(IL_PREFIX_STRING);

    char const *next = shader;

    while ((next = strstr(next, IL_PREFIX_STRING)) != NULL)
    {
        next += prefix_skip;
        while (*next && *next != '\n')
        {
            ++next;
            ++il_bytes;
        }
        ++il_bytes;
    }

    // Pass two: copy IL bytes. 

    char *il_code = (char *) malloc(il_bytes + 1);

    char *dst = il_code;

    next = shader;
    while ((next = strstr(next, IL_PREFIX_STRING)) != NULL)
    {
        next += prefix_skip;
        while (*next && *next != '\n')
            *dst++ = *next++;

        *dst++ = '\n';
    }

    *dst = 0;

    return il_code;
}

#if 0
//new implementation of shuffle_constant, 
//which move constant relative declaration before it is used
static bool is_constant_line(const char *src)
{
    LOG(LOG_FN_ENTRY, "is_constant_line(%s)\n", src);
    if (strncmp(src, ILLocalConst.c_str(), strlen(ILLocalConst.c_str())) == 0 ||
        strncmp(src, ILConstBuff.c_str(), strlen(ILConstBuff.c_str())) == 0 ||
        strncmp(src, ILLiteralConst.c_str(), strlen(ILLiteralConst.c_str())) == 0)
    {
        return true;
    }

    return false;
}

static void print_line(const char **p_src, char **p_dst, bool doprint)
{
    LOG(LOG_FN_ENTRY, "print_line(%p, %p, %d)\n", p_src, p_dst, doprint);
    const char *src;
    char *dst;

    src = *p_src;
    dst = *p_dst;

    while (*src && *src != '\n')
    {
        if (doprint)
            *dst++ = *src;
        src++;
    }

    if (*src == '\n')
    {
        if (doprint)
            *dst++ = *src;

        src++;
    }

    *p_src = src;
    *p_dst = dst;
}

static char *print_constant_lines(const char *src, char *dst)
{
    LOG(LOG_FN_ENTRY, "print_constant_lines(%s, %p)\n", src, dst);
    //when enter, src pointing to beginning of a line
    assert(src[-1] == '\n');
    while (*src != '\0')
    {
        print_line(&src, &dst, is_constant_line(src));
        assert(*src == '\0' || src[-1] == '\n');
    }
    return dst;
}

static char *print_nonconstant_lines(const char *src, char *dst)
{
    LOG(LOG_FN_ENTRY, "print_nonconstant_lines(%s, %p)\n", src, dst);
    //when enter, src pointing to beginning of a line
    assert(src[-1] == '\n');
    while (*src != '\0')
    {
        print_line(&src, &dst, is_constant_line(src) == false);
        assert(*src == '\0' || src[-1] == '\n');
    }
    return dst;
}

char *reshuffle_constants(const char *shader)
{
    LOG(LOG_FN_ENTRY, "reshuffle_constants(%s)\n", shader);
    size_t il_bytes = strlen(shader);

    char *il_code = (char *) malloc(il_bytes + 1);

    char *dst = il_code;
    char const *next = shader;

    // Copy first line.
    while (*next && *next != '\n')
        *dst++ = *next++;

    if (*next)
    {
        *dst++ = '\n';
        ++next;
    }

    dst = print_constant_lines(next, dst);
    dst = print_nonconstant_lines(next, dst);

    *dst = 0;

    assert(il_bytes == strlen(il_code));

    return il_code;
}

#else
char *reshuffle_constants(const char *shader)
{
    LOG(LOG_FN_ENTRY, "reshuffle_constants(%s)\n", shader);
    int il_bytes = strlen(shader);

    char *il_code = (char *) malloc(il_bytes + 1);

    char *dst = il_code;

    char const *next = shader;

    // Copy first line.
    while (*next && *next != '\n')
        *dst++ = *next++;

    if (*next)
    {
        *dst++ = '\n';
        ++next;
    }

    // For copying non-constants.
    char const *last = next;

    // Copy constants
    while ((next = strstr(next, "def c")) != NULL)
    {
        while (*next && *next != '\n')
            *dst++ = *next++;

        if (*next)
        {
            *dst++ = '\n';
            ++next;
        }
    }

    next = last;
    while ((next = strstr(next, "def c")) != NULL)
    {
        //copy the code between the last copied line and the current "def c"
        while (last < next)
            *dst++ = *last++;

        next += 5;              //this is the size of "def c"
        while (*next && *next != '\n')
            ++next;

        if (*next)
            ++next;

        last = next;
    }

    while (*last)
        *dst++ = *last++;

    *dst = 0;

    return il_code;
}
#endif

char *passthrough_il(const char *name,
                     const char *shader,
                     CodeGenTarget target, ShaderResourceUsage * outUsage, bool inValidate)
{
    LOG(LOG_FN_ENTRY, "passthrough_il(%s, %s, %d, %p, %d)\n", name, shader, target, outUsage, inValidate);
    char *tmp = extract_il(shader);
    char *result = reshuffle_constants(tmp);

    if (globals.keepFiles)
    {
        std::string fname = std::string(name) + ".il";
        std::ofstream outfile;

        outfile.open(fname.c_str());
        if (outfile.fail())
        {
            std::cerr << "***Unable to open " << fname << "\n";
        }
        else
        {
            outfile << result;
            outfile.close();
        }
    }

    free(tmp);
    return result;

}

char *gpusa_il(const char *name,
               const char *shader,
               CodeGenTarget target, ShaderResourceUsage * outUsage, bool inValidate)
{
    LOG(LOG_FN_ENTRY, "gpusa_il(%s, %s, %d, %p, %p)\n", name, shader, target, outUsage, inValidate);
    std::string input = shader;

    //Temp fix, shader analyzer defines aren't working
    input.insert(0, "#define DXPIXELSHADER 1\n");

    //char* tmp = extract_il( shader );
    //char* result = reshuffle_constants( tmp );
    //free( tmp );
    //return result;
    const char *argv[] = { "gpushaderanalyzer", 0,
			   "-Disassemble", 0,
			   "-ASIC", "IL",
			   "-Profile", "ps_4_0",
			   "-Function", "main",
			   "-UseDX9Mode",
			   //"-Define", "DXPIXELSHADER", "1",
			   NULL
    };
    char /* unused fpcode , */ *errcode;

    std::string inputfname = std::string(name) + ".cg";
    std::string outputfname = std::string(name) + ".il";

    FILE *fp = fopen(inputfname.c_str(), "wb+");

    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open tmp file %s\n", outputfname.c_str());
        return NULL;
    }
    //fwrite(shader, sizeof(char), strlen(shader), fp);
    fwrite(input.c_str(), sizeof(char), strlen(input.c_str()), fp);
    fclose(fp);

    argv[1] = strdup(inputfname.c_str());
    argv[3] = strdup(outputfname.c_str());

    /* Run FXC */
    errcode = Subprocess_Run(argv, NULL);

    free((void *)(argv[1]));
    free((void *)(argv[3]));

#ifdef _WIN32
    if (fopen_s(&fp, outputfname.c_str(), "rb"))
#else
    if (!(fp = fopen(outputfname.c_str(), "rb")))
#endif
    {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    unsigned int lsize = ftell(fp);

    if (lsize == 0)
    {
        printf("GPUShaderAnalyzer couldn't compile intermediate shader\n");
        return NULL;
    }
    rewind(fp);

    if (!globals.keepFiles)
        remove(inputfname.c_str());

    char *ilcode = (char *) malloc(lsize + 1);

    fread(ilcode, 1, lsize, fp);
    ilcode[lsize] = 0;
    fclose(fp);

    input.clear();
    input = ilcode;
    free(ilcode);

    //formatting issue, shader analyzer does "\r\r\n" vs. "\n"
    size_t a = input.find("\r\r\n", 0);

    while (a != std::string::npos)
    {
        input.replace(a, 3, "\n");
        a = input.find("\r\r\n", a);
    }

#if 0
    //DISABLED - using interpolants
    //Replace interpolants with VPOS (v0)
    a = input.find("sampler", 0);
    while (a != std::string::npos)
    {
        unsigned int b = input.find("v", a);
        unsigned int c = input.find("\n", a);

        if (b < c && input[b + 2] == '.')
        {
            input[b + 1] = '0';
            input[b + 3] = 'x';
            input[b + 4] = 'y';
        }
        a = input.find("sampler", c);
    }
#endif

#if 1
    //Fix to get textures working with ps40/il40

    //Find valid texture resource
    char samp[16] = { 0 };
    a = input.find("dcl_input_interp", 0);
    int i = 0;

    while (a != std::string::npos)
    {
        size_t b = input.find("v", a);
        size_t c = input.find("\n", a);

        if (b < c)
        {
            //samp[(input[b+1])-48] = true;
            samp[i++] = input[b + 1];
        }

        a = input.find("dcl_input_interp", c);
    }

    //remap samplers
    a = input.find("dcl_resource_id(", 0);
    i = 0;
    while (a != std::string::npos)
    {
        if (samp[i] != 0)
            input[a + 16] = samp[i++];
        a = input.find("dcl_resource_id(", a + 16);
    }

    a = input.find("sample_resource(", 0);
    while (a != std::string::npos)
    {
        size_t b = input.find("v", a);
        size_t c = input.find("\n", a);

        if (b < c)
        {
            input[a + 16] = input[b + 1];
            input[a + 27] = input[b + 1];
        }

        a = input.find("sample_resource(", c);
    }
#endif

#if 1                           //ENABLE TO GET INTERPOLATORS TO WORK
    //Replace dcl_input_interp with dcl_input_generic_interp
    std::string sFind = "dcl_input_interp";
    a = input.find(sFind, 0);
    while (a != std::string::npos)
    {
        input.replace(a, sFind.size(), "dcl_input_generic_interp");
        a = input.find("dcl_input_interp", a);
    }
#endif

    //Shove in unnormalzied texture coordinate command
    a = input.find("type(2d)", 0);
    while (a != std::string::npos)
    {
        a += 7;
        input.insert(a, ",unnorm");
        a = input.find("type(2d)", a);
    }

    ilcode = (char *) malloc(input.size() + 1);
    strcpy(ilcode, input.c_str());
    ilcode[input.size()] = 0;

    if (!globals.keepFiles)
        remove(outputfname.c_str());

#if 1
    return ilcode;
#else
    char *result = reshuffle_constants(ilcode);

    free(ilcode);

    return result;
#endif
}
