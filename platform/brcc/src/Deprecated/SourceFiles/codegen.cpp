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
 * codegen.cpp --
 *
 *      Codegen takes already parsed brook input and produces the resulting
 *      .[ch] files required to invoke it.  It converts to CG and feeds that
 *      through a CG compiler.
 */

#ifdef _WIN32
#pragma warning(disable:4786)
#include <ios>
#else
#include <iostream>
#endif // _WIN32

#include <sstream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <string.h>

extern "C"
    {
#include <stdio.h>
//#include <string.h>
#include <assert.h>
    }

#include "logger.hpp"
#include "main.h"
#include "decl.h"
#include "subprocess.h"
#include "project.h"
#include "stemnt.h"
#include "brtkernel.h"
#include "b2ctransform.h"
#include "codegen.h"
#include "brtdecl.h"
#include "fxc.h"
#include "cgc.h"
#include "extract_il.h"
#include "generate_il.h"
#include "types.h"

// structures to store information about the resources
// used in each pass/technique
struct shader_input_info
{
    shader_input_info(int arg, int comp):argumentIndex(arg)
        {
            LOG(LOG_FN_ENTRY, "shader_input_info::shader_input_info(%d, %d)\n", arg, comp);
            std::ostringstream out;
            out << comp;
            componentName = out.str();
        }

    shader_input_info (int arg, const std::string & comp):argumentIndex(arg), componentName(comp)
        {
            LOG(LOG_FN_ENTRY, " shader_input_info(%d, %s)\n", arg, comp.c_str());
        }

    int argumentIndex;

    std::string componentName;
};

struct pass_info
{
    void addConstant(int arg, int comp)
        {
            LOG(LOG_FN_ENTRY, "pass_info::addConstant(%d, %d)\n", arg, comp);
            constants.push_back(shader_input_info(arg, comp));
        }

    void addConstant(int arg, const std::string & comp)
        {
            LOG(LOG_FN_ENTRY, "pass_info::addConstant(%d, %s)\n", arg, comp.c_str());
            constants.push_back(shader_input_info(arg, comp));
        }

    void addSampler(int arg, int comp)
        {
            LOG(LOG_FN_ENTRY, "pass_info::addSampler(%d, %d)\n", arg, comp);
            samplers.push_back(shader_input_info(arg, comp));
        }

    void addInterpolant(int arg, int comp)
        {
            LOG(LOG_FN_ENTRY, "pass_info::addInterpolant(%d, %d)\n", arg, comp);
            interpolants.push_back(shader_input_info(arg, comp));
        }

    void addInterpolant(int arg, const std::string & comp)
        {
            LOG(LOG_FN_ENTRY, "pass_info::addInterpolant(%d, %s)\n", arg, comp.c_str());
            interpolants.push_back(shader_input_info(arg, comp));
        }

    void addOutput(int arg, int comp)
        {
            LOG(LOG_FN_ENTRY, "pass_info::addOutput(%d, %d)\n", arg, comp);
            outputs.push_back(shader_input_info(arg, comp));
        }

    std::string shader;
    std::vector < shader_input_info > constants;
    std::vector < shader_input_info > samplers;
    std::vector < shader_input_info > interpolants;
    std::vector < shader_input_info > outputs;
};

struct technique_info
{
    technique_info():reductionFactor(-1), outputAddressTranslation(false), inputAddressTranslation(false)
        {
            LOG(LOG_FN_ENTRY, "technique_info::technique_info()\n");
        }

    std::vector < pass_info > passes;
    int reductionFactor;
    bool outputAddressTranslation;
    bool inputAddressTranslation;
};

static void emitConstRegFloat4(std::ostream & shader, const std::string & regName, const int regNum, bool comma = true)
{
    LOG(LOG_FN_ENTRY, "emitConstRegFloat4(%p, %s, %d, %d)\n", &shader, regName.c_str(), regNum, comma);
    shader << "uniform float4 " << regName << " : register(c" << regNum << ")";

    if (comma)
        shader << ",";

    shader << "\n\t\t";

    bril::name_extern_constant(regNum, regName);
}

static char *(*shadercompile[CODEGEN_NUM_TARGETS]) (const char *name,
                                                    const char *shader,
                                                    CodeGenTarget target,
                                                    ShaderResourceUsage * outUsage, bool inValidate);

void CodeGen_Init(void)
{
    LOG(LOG_FN_ENTRY, " CodeGen_Init()\n");
    shadercompile[CODEGEN_PS20] = compile_fxc;
    shadercompile[CODEGEN_PS2B] = compile_fxc;
    shadercompile[CODEGEN_PS2A] = compile_fxc;
    shadercompile[CODEGEN_PS30] = compile_fxc;
    shadercompile[CODEGEN_CAL] = passthrough_il;
    //shadercompile[CODEGEN_CAL]  = gpusa_il;
    shadercompile[CODEGEN_FP30] = compile_cgc;  // FIXME - Cg reference, needs removing
    shadercompile[CODEGEN_FP40] = compile_cgc;
    shadercompile[CODEGEN_ARB] = compile_fxc;
}

// ---------------------------------------------------------------------------------------------
// Manage the kernels for IL code generation.
//
// May need to generate multiple instances of functions that take samplers as parameters.

class GPUKernelInfo
{
public:
    GPUKernelInfo(FunctionDef * f)
        {
            LOG(LOG_FN_ENTRY, "GPUKernelInfo::GPUKernelInfo(%p)\n", f);
            fd = f;
        }

    ~GPUKernelInfo()
        {
            LOG(LOG_FN_ENTRY, "GPUKernelInfo::~GPUKernelInfo()\n");
            delete fd;
        }

    FunctionDef *getFunction()
        {
            LOG(LOG_FN_ENTRY, "GPUKernelInfo::getFunction()\n");
            return fd;
        }
    int getInstance(SamplerParamInfo const &params);
#if BROOK_TO_IL_CODE_GEN_PATH
    bool printILCode(std::ostream & outs);
#endif // BROOK_TO_IL_CODE_GEN_PATH
    void print(std::ostream & outs) const;

private:
    FunctionDef * fd;
    std::vector < KernelInstance > instance;
};

static FunctionDef *currentKernel = NULL;
static std::vector < GPUKernelInfo * >allKernelInfo;
static std::map < std::string, GPUKernelInfo * >allReachedKernel;
static int nextInstance = 0;

int GPUKernelInfo::getInstance(SamplerParamInfo const &params)
{
    LOG(LOG_FN_ENTRY, " GPUKernelInfo::getInstance(%p)\n", &params);
    for (int i = 0; i < static_cast < int >(instance.size()); i++)
    {
        if (instance[i].match(params))
            return instance[i].getId();
    }

    // Didn't find a match, add a new one.
    int fnum = nextInstance++;

    instance.push_back(KernelInstance(params, fnum));

    if (instance.size() == 1)
    {
        allReachedKernel[fd->decl->name->name] = this;
    }

    return fnum;
}

#if BROOK_TO_IL_CODE_GEN_PATH
bool GPUKernelInfo::printILCode(std::ostream & outs)
{
    LOG(LOG_FN_ENTRY, " GPUKernelInfo::printILCode(%p)\n", &outs);
    bool generated = false;

    for (int i = 0; i < static_cast < int >(instance.size()); i++)
    {
        if (!instance[i].generated())
        {
            generated = true;
            instance[i].setGenerated();

            currentKernelInstance = &instance[i];
            currentKernel = fd;

            outs << "\n" << IL_COMMENT_STRING << "Function: " << fd->decl->name->name << "( ";
            instance[i].printSampler(outs);
            outs << ")\n";

            BRTPS20KernelCode(*fd).printILCode(outs);
        }
    }
    return generated;
}
#endif // BROOK_TO_IL_CODE_GEN_PATH
void GPUKernelInfo::print(std::ostream & outs) const
{
    LOG(LOG_FN_ENTRY, " GPUKernelInfo::print(%p)\n", &outs);

    for (int i = 0; i < static_cast < int >(instance.size()); i++)
    {
        instance[i].printSampler(outs);
    }
}

//IL routine for address translation

// this routine corresponds to the HLSL __calculateindexof
static void
printILCalculateindexof(std::ostream & shader, bril::store * indexofarg,
                        bril::store * indexofoutput, int factor, int offset)
{
    f32 fpconst = (float)(factor);

    bril::store ilfactor;
    get_scalar_constant_store(&ilfactor, fpconst);

    bril::store temp1;
    bril::allocate_store(&temp1, 4);

    bril::emitOp(shader, "mul", &temp1, indexofoutput, &ilfactor);

    fpconst = (float)(offset);
    bril::store iloffset;
    get_scalar_constant_store(&iloffset, fpconst);

    bril::emitOp(shader, "add", indexofarg, &temp1, &iloffset);
}

// this routine corresponds to the HLSL __calculateindexof
static void
printILCalculateindexof(std::ostream & shader, bril::store * indexofarg,
                        const std::string & argname, bril::store * indexofoutput, bool isstream = true)
{
    LOG(LOG_FN_ENTRY, "printILCalculateindexof(%p, %p, %s, %p, %d)\n", &shader, indexofarg, argname.c_str(), indexofoutput, isstream);
    std::string streamindexofnumerArg;
    std::string streamindexofdenomArg;
    if (isstream)
    {
        streamindexofnumerArg = bril::get_extern_constant("__streamindexofnumer_" + argname);
        streamindexofdenomArg = bril::get_extern_constant("__streamindexofdenom_" + argname);
    }
    else
    {
        streamindexofnumerArg = bril::get_extern_constant("__iterindexofnumer_" + argname);
        streamindexofdenomArg = bril::get_extern_constant("__iterindexofdenom_" + argname);
    }

    //floor( (indexofoutput*streamIndexofNumer + 0.5)*streamIndexofInvDenom );
    u32 half = 0x3F000000;      //.5
    f32 fhalf = *(f32 *) & half;

    bril::store shalf;
    get_scalar_constant_store(&shalf, fhalf);

    bril::store temp1;
    bril::allocate_store(&temp1, 4);
    shader << IL_PREFIX_STRING << "mad "
           << get_gpr_name(&temp1, bril::rs_dst_sel)
           << ", " << get_gpr_name(indexofoutput, bril::rs_src_sel)
           << ", " << streamindexofnumerArg << ", " << get_gpr_name(&shalf, bril::rs_src_sel) << "\n";

    shader << IL_PREFIX_STRING << "mul "
           << get_gpr_name(&temp1, bril::rs_dst_sel)
           << ", " << get_gpr_name(&temp1, bril::rs_src_sel) << ", " << streamindexofdenomArg << "\n";

    bril::emitOp(shader, "flr", indexofarg, &temp1);
}

// this routine corresponds to the HLSL __calculateiteratorvalue

/*
  float4 __calculateiteratorvalue( float4 index,
  float4 valueBase, float4 valueOffset1, float4 valueOffset4 ) {
  return valueBase + index.x*valueOffset1 + index*valueOffset4;
  }
*/
static void
printILCalculateiteratorvalue(std::ostream & shader, bril::store * argil,
                              bril::store * indexof, const std::string & argname)
{
    LOG(LOG_FN_ENTRY, "printILCalculateiteratorvalue(%p, %p, %p, %s)\n", &shader, argil, indexof, argname.c_str());
    std::string itervaluebase = bril::get_extern_constant("__itervaluebase_" + argname);
    std::string itervalueoffset1 = bril::get_extern_constant("__itervalueoffset1_" + argname);
    std::string itervalueoffset4 = bril::get_extern_constant("__itervalueoffset4_" + argname);

    // valuebase + index.x * valueOffset1
    shader << IL_PREFIX_STRING << "mad "
           << get_gpr_name(argil, bril::rs_dst_sel)
           << ", " << get_gpr_name(indexof, bril::rs_base) << ".x"
           << ", " << itervalueoffset1 << ", " << itervaluebase << "\n";

    // += index * valueOffset4;
    shader << IL_PREFIX_STRING << "mad "
           << get_gpr_name(argil, bril::rs_dst_sel)
           << ", " << get_gpr_name(indexof, bril::rs_base)
           << ", " << itervalueoffset4 << ", " << get_gpr_name(argil, bril::rs_base) << "\n";
}

/**
   Implement the following subroutine
   float2 __calculatetexpos( float4 streamIndex, float4 streamDomainMin,
   float4 linearizeConst, float4 textureShapeConst ) {
   float linearIndex = dot( streamIndex + streamDomainMin, linearizeConst ) + 0.5;
   float2 texIndex;
   texIndex.y = floor( linearIndex * textureShapeConst.x );
   texIndex.x = floor( linearIndex - texIndex.y * textureShapeConst.z );
   float2 texCoord = texIndex + 0.5;
   #ifndef USERECT
// convert to 0-to-1 texture space
texCoord *= textureShapeConst.xy;
#endif
return texCoord;
}
*/
void
printILCalculatetexpos(std::ostream & shader, bril::store * texpos,
                       const std::string & index, const std::string & domainmin,
                       const std::string & linearize, const std::string & textureshape)
{
    LOG(LOG_FN_ENTRY, "printILCalculatetexpos(%p, %p, %s, %s, %s, %s)\n", &shader, texpos, index.c_str(), domainmin.c_str(), linearize.c_str(), textureshape.c_str());
    //float linearIndex = dot( streamIndex + streamDomainMin, linearizeConst ) + 0.5;
    bril::store temp1;
    bril::allocate_store(&temp1, 4);
    shader << IL_PREFIX_STRING << "add "
           << bril::get_gpr_name(&temp1, bril::rs_dst_sel) << ", " << index << ", " << domainmin << "\n";

    bril::store linearIndex;
    bril::allocate_store(&linearIndex, 1);
    shader << IL_PREFIX_STRING << "dp4 "
           << bril::get_gpr_name(&linearIndex, bril::rs_dst_sel)
           << ", " << bril::get_gpr_name(&temp1, bril::rs_src_sel) << ", " << linearize << "\n";

    u32 half = 0x3F000000;      //.5
    f32 fhalf = *(f32 *) & half;

    bril::store shalf;
    get_scalar_constant_store(&shalf, fhalf);
    bril::emitOp(shader, "add", &linearIndex, &linearIndex, &shalf);

    //texIndex.y = floor( linearIndex * textureShapeConst.x );
    shader << IL_PREFIX_STRING << "mul "
           << bril::get_gpr_name(texpos, bril::rs_base) << "._y__"
           << ", " << bril::get_gpr_name(&linearIndex, bril::rs_src_sel) << ", " << textureshape << ".x\n";
    shader << IL_PREFIX_STRING << "flr "
           << bril::get_gpr_name(texpos, bril::rs_base) << "._y__"
           << ", " << bril::get_gpr_name(texpos, bril::rs_base) << ".y\n";

    //texIndex.x = floor( linearIndex - texIndex.y * textureShapeConst.z );
    shader << IL_PREFIX_STRING << "mad "
           << bril::get_gpr_name(texpos, bril::rs_base) << ".x___"
           << ", " << bril::get_gpr_name(texpos, bril::rs_base) << ".y"
           << ", " << textureshape << ".z_neg(xyzw)" << ", " << bril::get_gpr_name(&linearIndex, bril::rs_base) << "\n";
    shader << IL_PREFIX_STRING << "flr "
           << bril::get_gpr_name(texpos, bril::rs_base) << ".x___"
           << ", " << bril::get_gpr_name(texpos, bril::rs_base) << ".x\n";

    //float2 texCoord = texIndex + 0.5;
    bril::emitOp(shader, "add", texpos, texpos, &shalf);
}

/**
   implement the following function call
   __calculatetexpos( __indexof_a, __streamdomainmin_a, __streamlinearize_a, __streamtextureshape_a );
*/
static void
printILCalculatetexpos(std::ostream & shader, bril::store * texpos, const std::string & argname, bril::store * indexof)
{
    LOG(LOG_FN_ENTRY, "printILCalculatetexpos(%p, %p, %s, %p)\n", &shader, texpos, argname.c_str(), indexof);
    std::string streamdomainmin = bril::get_extern_constant("__streamdomainmin_" + argname);
    std::string streamlinearize = bril::get_extern_constant("__streamlinearize_" + argname);
    std::string streamtextureshape = bril::get_extern_constant("__streamtextureshape_" + argname);

    std::ostringstream indexofname;
    indexofname << bril::get_gpr_name(indexof, bril::rs_base);
    printILCalculatetexpos(shader, texpos, indexofname.str(), streamdomainmin, streamlinearize, streamtextureshape);
}

// this routine corresponds to the HLSL __calculateoutputpos
static void printILCalculateoutputpos(std::ostream & shader, bril::store * index)
{
    LOG(LOG_FN_ENTRY, " printILCalculateoutputpos(%p, %p)\n", &shader, index);
    bril::store interpolant;
    allocate_store(&interpolant, 2);
    shader << IL_PREFIX_STRING << "mov "
           << get_gpr_name(&interpolant, bril::rs_dst_sel)
           << ", v" << bril::getTexcoordId("__outputaddrinterpolant") << ".xy00\n";

    std::string outputlinearize = bril::get_extern_constant("__outputlinearize");
    std::string outputstride = bril::get_extern_constant("__outputstride");
    std::string outputinvstride = bril::get_extern_constant("__outputinvstride");
    std::string outputinvextent = bril::get_extern_constant("__outputinvextent");
    std::string outputdomainmin = bril::get_extern_constant("__outputdomainmin");
    std::string outputdomainsize = bril::get_extern_constant("__outputdomainsize");

    bril::store cleaninterpolant;
    allocate_store(&cleaninterpolant, 2);
    //round_neginf
    shader << IL_PREFIX_STRING << "flr "
           << get_gpr_name(&cleaninterpolant, bril::rs_dst_sel)
           << ", " << get_gpr_name(&interpolant, bril::rs_base) << ".xyxx\n";

    bril::store linearindex;
    allocate_store(&linearindex, 1);
    //dot
    shader << IL_PREFIX_STRING << "dp2 "
           << get_gpr_name(&linearindex, bril::rs_dst_sel)
           << ", " << get_gpr_name(&cleaninterpolant, bril::rs_base) << ".xyxx" << ", " << outputlinearize << ".xyxx\n";

    bril::store cleanlinearindex;
    allocate_store(&cleanlinearindex, 1);
    u32 half = 0x3F000000;      //.5
    f32 fhalf = *(f32 *) & half;

    bril::store shalf;
    get_scalar_constant_store(&shalf, fhalf);
    bril::emitOp(shader, "add", &cleanlinearindex, &linearindex, &shalf);

    bril::store temp0;
    allocate_store(&temp0, 4);
    shader << IL_PREFIX_STRING << "mul "
           << get_gpr_name(&temp0, bril::rs_dst_sel)
           << ", " << get_gpr_name(&cleanlinearindex, bril::rs_base) << ".x" << ", " << outputinvstride << "\n";
    bril::emitOp(shader, "flr", &temp0, &temp0);

    bril::store temp1;
    bril::allocate_store(&temp1, 4);
    shader << IL_PREFIX_STRING << "mad "
           << get_gpr_name(&temp1, bril::rs_dst_sel)
           << ", " << get_gpr_name(&temp0, bril::rs_base) << ".xyzw_neg(xyzw)"
           << ", " << outputstride << ", " << get_gpr_name(&linearindex, bril::rs_base) << ".x\n";

    bril::emitOp(shader, "add", &temp1, &temp1, &shalf);
    shader << IL_PREFIX_STRING << "mad "
           << get_gpr_name(index, bril::rs_dst_sel)
           << ", " << get_gpr_name(&temp1, bril::rs_base)
           << ", " << outputinvextent << ", " << outputdomainmin << ".xyzw_neg(xyzw)\n";
    bril::emitOp(shader, "flr", index, index);

    u32 zero = 0x0;             //0
    f32 fzero = *(f32 *) & zero;

    bril::store szero;
    get_scalar_constant_store(&szero, fzero);

    bril::store lesszero;
    bril::allocate_store(&lesszero, 4);
    bril::store anyless;
    bril::allocate_store(&anyless, 4);
    bril::emitOp(shader, "lt", &lesszero, index, &szero);
    bril::emitAny(shader, &anyless, &lesszero);

    shader << IL_PREFIX_STRING << "discard_logicalnz " << get_gpr_name(&anyless, bril::rs_base) << ".xxxx \n";

    bril::store largermax;
    bril::allocate_store(&largermax, 4);
    bril::store anylarger;
    bril::allocate_store(&anylarger, 4);
    shader << IL_PREFIX_STRING << "ge "
           << get_gpr_name(&largermax, bril::rs_base)
           << ", " << get_gpr_name(index, bril::rs_base) << ", " << outputdomainsize << "\n";
    bril::emitAny(shader, &anylarger, &largermax);

    shader << IL_PREFIX_STRING << "discard_logicalnz " << get_gpr_name(&anylarger, bril::rs_base) << ".xxxx \n";
}



void add_extra_kernel_parameters_for_the_gpu(FunctionDef * fd);

GPUKernelInfo *find_gpu_kernel_info_by_name(std::string const &fname)
{
    LOG(LOG_FN_ENTRY, "find_gpu_kernel_info_by_name(%s)\n", fname.c_str());
    int i;

    for (i = 0; i < static_cast < int >(allKernelInfo.size()); i++)
    {
        if (allKernelInfo[i]->getFunction()->decl->name->name == fname)
            return allKernelInfo[i];
    }

    //assert(0);
    return NULL;
}

FunctionDef *find_gpu_kernel_by_name(std::string const &fname)
{
    LOG(LOG_FN_ENTRY, "find_gpu_kernel_by_name(%s)\n", fname.c_str());
    return find_gpu_kernel_info_by_name(fname)->getFunction();
}

int getKernelInstance(std::string const &fname, SamplerParamInfo const &params)
{
    LOG(LOG_FN_ENTRY, " getKernelInstance(%s, %p)\n", fname.c_str(), &params);
    GPUKernelInfo *kinfo = find_gpu_kernel_info_by_name(fname);

    return kinfo->getInstance(params);
}

SymEntry *find_kernel_parameter_by_name(FunctionDef * fd, std::string const &pname)
{
    LOG(LOG_FN_ENTRY, " find_kernel_parameter_by_name(%p, %s)\n", fd, &pname);
    FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);

    for (u32 j = 0; j < ft->nArgs; ++j)
    {
        if (ft->args[j]->name->name == pname)
            return ft->args[j]->name->entry;
    }

    //std::cerr << "can't find parameter " << pname << " in kernel " << fd->decl->name->name << std::endl;

    //assert( 0 );
    return NULL;
}

SymEntry *find_extra_kernel_parameter_by_name(std::string const &fname, std::string const &pname)
{
    LOG(LOG_FN_ENTRY, "find_extra_kernel_parameter_by_name(%s, %s)\n", fname.c_str(), pname.c_str());
    FunctionDef *fd = find_gpu_kernel_by_name(fname);

    return find_kernel_parameter_by_name(fd, pname);
}

FunctionDef *get_current_kernel()
{
    LOG(LOG_FN_ENTRY, "get_current_kernel()\n");
    return currentKernel;
}

int getCurrentInstanceId()
{
    LOG(LOG_FN_ENTRY, "getCurrentInstanceId()\n");
    return currentKernelInstance->getId();
}

SymEntry *find_parameter_in_current_kernel_by_name(std::string const &pname)
{
    LOG(LOG_FN_ENTRY, "find_parameter_in_current_kernel_by_name(%s)\n", pname.c_str());
    return find_kernel_parameter_by_name(currentKernel, pname);
}

//todo:
//     why we duplicate all the kernels each time when we generated a kernel
//     can we duplicate only once ??
void expand_gpu_kernel_parameter_lists()
{
    LOG(LOG_FN_ENTRY, "expand_gpu_kernel_parameter_lists()\n");
    int i;

    for (i = 0; i < static_cast < int >(allKernelInfo.size()); i++)
    {
        delete allKernelInfo[i];
    }

    allKernelInfo.clear();
    allReachedKernel.clear();
    nextInstance = 0;

    Statement *ste, *prev;

    for (ste = gProject->units.back()->head, prev = NULL; ste; prev = ste, ste = ste->next)
    {
        if (ste->isFuncDef())
        {
            FunctionDef *fd = static_cast < FunctionDef * >(ste);

            if (fd->decl && fd->decl->isKernel())
            {
                Statement *copy = ste->dup0();

                allKernelInfo.push_back(new GPUKernelInfo(static_cast < FunctionDef * >(copy)));
                add_extra_kernel_parameters_for_the_gpu(static_cast < FunctionDef * >(copy));
            }
        }
    }

    for (i = 0; i < static_cast < int >(allKernelInfo.size()); i++)
    {
        currentKernel = allKernelInfo[i]->getFunction();
        allKernelInfo[i]->getFunction()->findExpr(BRTGPUKernelCode::ConvertGathers);
    }
}

void instantiate_main_kernel(std::string const &name)
{
    LOG(LOG_FN_ENTRY, " instantiate_main_kernel(%s)\n", name.c_str());
    GPUKernelInfo *k_info = find_gpu_kernel_info_by_name(name);

    FunctionType *ft = static_cast < FunctionType * >(k_info->getFunction()->decl->form);

    SamplerParamInfo samplers;

    for (u32 p_idx = 0; p_idx < ft->nArgs; ++p_idx)
    {
        if (ft->args[p_idx]->isArray() && (ft->args[p_idx]->form->getQualifiers() & TQ_Out) == 0)
        {
            samplers.insert(ft->args[p_idx]->name->name, bril::getTextureId(ft->args[p_idx]->name->name));
        }
    }

    int id = getKernelInstance(name, samplers);

    assert(id == 0);
}

static void generate_shader_subroutines(std::ostream & out)
{
    LOG(LOG_FN_ENTRY, " generate_shader_subroutines(%p)\n", &out);
    // HLSL
    // without a phase that mark which (sub)kernel is actually used,
    // print all the kernels
    // let fxc do select the needed code for the current (main)kernel
    int i;

    for (i = 0; i < static_cast < int >(allKernelInfo.size()); i++)
    {
        FunctionDef *fd = allKernelInfo[i]->getFunction();

        BRTPS20KernelCode(*fd).printInnerCode(out);
    }

#if BROOK_TO_IL_CODE_GEN_PATH
    // IL
    // generate all the sub(kernels) reachable from the current main-kernel
    std::map < std::string, GPUKernelInfo * >::iterator it;
    bool generated = true;

    while (generated)
    {
        generated = false;
        for (it = allReachedKernel.begin(); it != allReachedKernel.end(); it++)
        {
            generated = (*it).second->printILCode(out) || generated;
        }
    }
#endif // BROOK_TO_IL_CODE_GEN_PATH
}

static Symbol *findStructureTag(Type * inType)
{
    LOG(LOG_FN_ENTRY, "findStructureTag(%p)\n", inType);
    BaseType *base = inType->getBase();

    while (true)
    {
        BaseTypeSpec mask = base->typemask;

        if (mask & BT_UserType)
        {
            base = base->typeName->entry->uVarDecl->form->getBase();
        }
        else if (mask & BT_Struct)
            return base->tag;
        else
            break;
    }
    return NULL;
}

static StructDef *findStructureDef(Type * inType)
{
    LOG(LOG_FN_ENTRY, "findStructureDef(%p)\n", inType);
    Symbol *tag = findStructureTag(inType);

    if (tag == NULL)
        return NULL;
    return tag->entry->uStructDef->stDefn;
}

#if 0
static void printShaderStructureDef(std::ostream & out, StructDef * structure)
{
    LOG(LOG_FN_ENTRY, " printShaderStructureDef(%p, %p)\n", &out, structure);
    out << "struct " << structure->tag->name << " {\n";
    int fieldCount = structure->nComponents;

    for (int i = 0; i < fieldCount; i++)
    {
        Decl *fieldDecl = structure->components[i];

        if (fieldDecl->isStatic())
            continue;
        if (fieldDecl->isTypedef())
            continue;

        out << "\t";
        fieldDecl->form->printType(out, fieldDecl->name, true, 0);
        out << ";\n";
    }
    out << "};\n\n";
}
#endif

int getGatherStructureSamplerCount(StructDef * structure)
{
    LOG(LOG_FN_ENTRY, " getGatherStructureSamplerCount(%p)\n", structure);
    int result = 0;
    int fieldCount = structure->nComponents;

    for (int i = 0; i < fieldCount; i++)
    {
        Decl *fieldDecl = structure->components[i];

        if (fieldDecl->isStatic())
            continue;
        if (fieldDecl->isTypedef())
            continue;

        StructDef *subStructure = findStructureDef(fieldDecl->form);

        if (subStructure)
            result += getGatherStructureSamplerCount(subStructure);
        else
            result++;
    }
    return result;
}

int getGatherStructureSamplerCount(Type * form)
{
    LOG(LOG_FN_ENTRY, " getGatherStructureSamplerCount(%p)\n", form);
    StructDef *structure = findStructureDef(form);

    if (!structure)
        return 1;
    return getGatherStructureSamplerCount(structure);
}

int getGatherRank(Type * form)
{
    LOG(LOG_FN_ENTRY, " getGatherRank(%p)\n", form);
    Type *f = form;
    int rank = 0;

    while (f->isArray())
    {
        rank++;
        f = ((ArrayType *) f)->subType;
    }
    return rank;
}

static bool printGatherStructureFunctionBody(std::ostream & out, const std::string & name, StructDef * structure,
                                             int &ioIndex)
{
    LOG(LOG_FN_ENTRY, " printGatherStructureFunctionBody(%p, %s, %p, %p)\n", &out, name.c_str(), structure, &ioIndex);
    int fieldCount = structure->nComponents;

    for (int i = 0; i < fieldCount; i++)
    {
        Decl *fieldDecl = structure->components[i];

        if (fieldDecl->isStatic())
            continue;
        if (fieldDecl->isTypedef())
            continue;

        std::string subName = name + "." + fieldDecl->name->name;

        Type *form = fieldDecl->form;
        StructDef *subStructure = findStructureDef(form);

        if (subStructure)
        {
            if (!printGatherStructureFunctionBody(out, subName, subStructure, ioIndex))
                return false;
        }
        else
        {
            out << "result" << subName << " = ";

            BaseType *base = form->getBase();

            const char *shname = getBaseTypeName(base->typemask, BaseTypeSHName);

            assert(shname != NULL);
            out << "__fetch_" << shname;

            out << "( ";
            out << "samplers[" << ioIndex++ << "], index );\n";
        }
    }
    return true;
}

static void printGatherStructureFunction(std::ostream & out, const std::string & name, Type * form)
{
    LOG(LOG_FN_ENTRY, " printGatherStructureFunction(%p, %s, %p)\n", &out, name.c_str(), form);
    StructDef *structure = findStructureDef(form);

    if (!structure)
        return;

    std::stringstream s;
    int index = 0;

    if (!printGatherStructureFunctionBody(s, "", structure, index))
        return;
    std::string body = s.str();

    out << name << " __gather_" << name << "( _stype1 samplers[" << getGatherStructureSamplerCount(form);
    out << "], float index ) {\n";
    out << name << " result;\n";
    out << body;
    out << "\treturn result;\n}\n\n";

    out << name << " __gather_" << name << "( _stype2 samplers[" << getGatherStructureSamplerCount(form);
    out << "], float2 index ) {\n";
    out << name << " result;\n";
    out << body;
    out << "\treturn result;\n}\n\n";

    out << name << " __gather_" << name << "( _stype3 samplers[" << getGatherStructureSamplerCount(form);
    out << "], float3 index ) {\n";
    out << name << " result;\n";
    out << body;
    out << "\treturn result;\n}\n\n";
}

static void generate_shader_type_declaration(std::ostream & out, DeclStemnt * inStmt)
{
    LOG(LOG_FN_ENTRY, " generate_shader_type_declaration(%p, %p)\n", &out, inStmt);
    for (DeclVector::iterator i = inStmt->decls.begin(); i != inStmt->decls.end(); ++i)
    {
        Decl *decl = *i;
        Type *form = decl->form;

        /*
          Symbol* structureTag = findStructureTag( form );
          if( structureTag != NULL ) {
          StructDef* structure = structureTag->entry->uStructDef->stDefn;
          printShaderStructureDef( out, structure );
          } */

        if (decl->isTypedef())
        {
            out << "typedef ";
            form->printBase(out, 0);
            out << " " << decl->name->name;
            out << ";";
            out << "\n";

            printGatherStructureFunction(out, decl->name->name, decl->form);
        }
    }
}

static void generate_shader_structure_definitions(std::ostream & out)
{
    LOG(LOG_FN_ENTRY, " generate_shader_structure_definitions(%p)\n", &out);
    TransUnit *tu = gProject->units.back();
    Statement *ste, *prev;

    for (ste = tu->head, prev = NULL; ste; prev = ste, ste = ste->next)
    {
        if (ste->isDeclaration() || ste->isTypedef())
        {
            DeclStemnt *decl = static_cast < DeclStemnt * >(ste);

            generate_shader_type_declaration(out, decl);
        }
    }
}

static bool expandOutputArgumentStructureDecl(std::ostream & shader,
                                              const std::string & argumentName,
                                              int inArgumentIndex,
                                              int inComponentIndex,
                                              StructDef * structure,
                                              int &ioOutputReg,
                                              int inFirstOutput, int inOutputCount, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, " expandOutputArgumentStructureDecl(%p, %s, %d, %d, %p, %p, %d, %d, %p)\n",
        &shader, argumentName.c_str(), inArgumentIndex, inComponentIndex, structure, &ioOutputReg,
        inFirstOutput, inOutputCount, &outPass);
    if (structure->isUnion())
        semanticCheckRecord(NULL, SCRDataType, argumentName.c_str(), true);

    bool used = false;

    int elementCount = structure->nComponents;
    int componentIndex = inComponentIndex;

    for (int i = 0; i < elementCount; i++)
    {
        Decl *elementDecl = structure->components[i];

        if (elementDecl->storage & ST_Static)
            continue;
        Type *form = elementDecl->form;

        // TIM: for now
        if (form->isBaseType() == false)
            semanticCheckRecord(NULL, SCRDataType, argumentName.c_str(), true);
        BaseType *base = form->getBase();
        StructDef *structure = findStructureDef(base);

        if (structure)
            used = expandOutputArgumentStructureDecl(shader, argumentName,
                                                     inArgumentIndex,
                                                     componentIndex,
                                                     structure, ioOutputReg,
                                                     inFirstOutput, inOutputCount, outPass) || used;
        else
        {
            int outr = ioOutputReg++;

            if (outr >= inFirstOutput && outr < inFirstOutput + inOutputCount)
            {
                used = true;
                // it had better be just a floatN
                shader << "#ifdef DXPIXELSHADER\n\t\t";
                shader << "out float4 __output_" << outr;
                shader << " : COLOR" << (outr - inFirstOutput);
                shader << ",\n\t\t";
                shader << "#else\n\t\t";

                //const char* rtname = getBaseTypeName(base->typemask, BaseTypeRTName);
                const char *shname = getBaseTypeName(base->typemask, BaseTypeSHName);

                assert(shname != NULL);
                shader << "out " << shname;

                shader << " __output_" << outr;
                shader << " : COLOR" << (outr - inFirstOutput);
                shader << ",\n\t\t";
                shader << "#endif\n\t\t";

                outPass.addOutput(inArgumentIndex, componentIndex);
            }
            componentIndex++;
        }
    }
    return used;
}

static bool expandOutputArgumentDecl(std::ostream & shader,
                                     const std::string & argumentName,
                                     int inArgumentIndex,
                                     int inComponentIndex,
                                     Type * form,
                                     int &ioOutputReg, int inFirstOutput, int inOutputCount, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "expandOutputArgumentDecl(%p, %s, %d, %d, %p, %p, %d, %d, %p)\n",
        &shader, argumentName.c_str(), inArgumentIndex, inComponentIndex, form, &ioOutputReg, inFirstOutput,
        inOutputCount, &outPass);
    StructDef *structure = NULL;

    if (form->isStream() || form->isArray())
    {
        BrtStreamType *streamType = (BrtStreamType *) (form);

        // TIM: can't handle arrays with a BaseType
        BaseType *elementType = streamType->getBase();

        structure = findStructureDef(elementType);
    }
    else
    {
        assert((form->getQualifiers() & TQ_Reduce) != 0);
        structure = findStructureDef(form);
    }

    if (structure)
    {
        return expandOutputArgumentStructureDecl(shader, argumentName, inArgumentIndex,
                                                 inComponentIndex, structure, ioOutputReg,
                                                 inFirstOutput, inOutputCount, outPass);
    }
    else
    {
        BaseTypeSpec mask = form->getBase()->typemask;
        int outr = ioOutputReg++;

        if (outr < inFirstOutput)
            return false;
        if (outr >= inFirstOutput + inOutputCount)
            return false;

        shader << "#ifdef DXPIXELSHADER\n";

        shader << "\t\tout float4 __output_" << outr;
        shader << " : COLOR" << (outr - inFirstOutput);
        shader << ",\n\t\t";

        shader << "#else\n";

        // it had better be just a floatN

        if (mask & BT_UserType)
        {
            mask = form->getBase()->typeName->entry->uVarDecl->form->getBase()->typemask;
        }

        //const char* rtname = getBaseTypeName(mask, BaseTypeRTName);
        const char *shname = getBaseTypeName(mask, BaseTypeSHName);

        assert(shname != NULL);
        shader << "\t\tout " << shname;

        shader << " __output_" << outr;
        shader << " : COLOR" << (outr - inFirstOutput);
        shader << ",\n\t\t";

        shader << "#endif\n\t\t";

        outPass.addOutput(inArgumentIndex, inComponentIndex);

        return true;
    }
}

static void expandSimpleOutputArgumentWrite(std::ostream & shader,
                                            const std::string & argumentName,
                                            Type * form, int &outputReg, int inFirstOutput, int inOutputCount)
{
    LOG(LOG_FN_ENTRY, " expandSimpleOutputArgumentWrite(%p, %s, %p, %p, %d, %d)\n",
        &shader, argumentName.c_str(), form, &outputReg, inFirstOutput, inOutputCount);
    int outr = outputReg++;

    if (outr < inFirstOutput || outr >= inFirstOutput + inOutputCount)
        return;

    assert(form);
    BaseType *base = form->getBase();

    assert(base);

    if (base->typemask & BT_UserType)
    {
        base = base->typeName->entry->uVarDecl->form->getBase();
    }

    shader << "\t#ifdef DXPIXELSHADER\n";

    shader << "\t__output_" << outr << " = ";

    switch (base->typemask)
    {
    case BT_Float:
    case BT_Fixed:
    case BT_ShortFixed:
        shader << "float4( " << argumentName << ", 0, 0, 0);\n";
        break;
    case BT_Float2:
    case BT_Fixed2:
    case BT_ShortFixed2:
        shader << "float4( " << argumentName << ", 0, 0);\n";
        break;
    case BT_Float3:
    case BT_Fixed3:
    case BT_ShortFixed3:
        shader << "float4( " << argumentName << ", 0);\n";
        break;
    case BT_Float4:
    case BT_Fixed4:
    case BT_ShortFixed4:
        shader << argumentName << ";\n";
        break;
    case BT_Double:
        shader << "float4( " << argumentName << ".x, 0, 0)\n";
        break;
    case BT_Double2:
        shader << "float4( " << argumentName << ".xy)\n";
        break;
    default:
        fprintf(stderr, "Strange stream base type: ");
        base->printBase(std::cerr, 0);
        abort();
    }

    shader << "\t#else\n";
    shader << "\t__output_" << outr << " = ";
    switch (base->typemask)
    {
    case BT_Double:
        shader << argumentName << ".x;\n";
        break;
    case BT_Double2:
        shader << "float4 (" << argumentName << ".xy);\n";
        break;
    default:
        shader << argumentName << ";\n";
    }
    shader << "\t#endif\n";

}

static void
expandOutputArgumentStructureWrite(std::ostream & shader,
                                   const std::string & fieldName,
                                   StructDef * structure, int &ioOutputReg, int inFirstOutput, int inOutputCount)
{
    LOG(LOG_FN_ENTRY, "expandOutputArgumentStructureWrite(%p, %s, %p, %p, %d, %d)\n",
        &shader, fieldName.c_str(), structure, &ioOutputReg, inFirstOutput, inOutputCount);
    assert(!structure->isUnion());

    int elementCount = structure->nComponents;

    for (int i = 0; i < elementCount; i++)
    {
        Decl *elementDecl = structure->components[i];

        if (elementDecl->storage & ST_Static)
            continue;
        Type *form = elementDecl->form;

        std::string subFieldName = fieldName + "." + elementDecl->name->name;

        // TIM: for now
        assert(form->isBaseType());
        BaseType *base = form->getBase();
        StructDef *structure = findStructureDef(base);

        if (structure)
        {
            expandOutputArgumentStructureWrite(shader, subFieldName,
                                               structure, ioOutputReg, inFirstOutput, inOutputCount);
        }
        else
        {
            expandSimpleOutputArgumentWrite(shader, subFieldName, base, ioOutputReg, inFirstOutput, inOutputCount);
        }
    }
}

static void expandOutputArgumentWrite(std::ostream & shader, const std::string & argumentName, Type * form,
                                      int &ioOutputReg, int inFirstOutput, int inOutputCount)
{
    LOG(LOG_FN_ENTRY, " expandOutputArgumentWrite(%p, %s, %p, %p, %d, %d)\n",
        &shader, argumentName.c_str(), form, &ioOutputReg, inFirstOutput, inOutputCount);
    StructDef *structure = NULL;
    Type *elementType = NULL;

    if (form->isStream() || form->isArray())
    {
        BrtStreamType *streamType = (BrtStreamType *) (form);

        // TIM: can't handle arrays with a BaseType
        elementType = streamType->getBase();
        structure = findStructureDef(elementType);
    }
    else
    {
        assert((form->getQualifiers() & TQ_Reduce) != 0);
        elementType = form;
        structure = findStructureDef(form);
    }

    bril::emitOutput(shader, bril::getNameStore(argumentName), ioOutputReg);

    if (structure)
    {
        expandOutputArgumentStructureWrite(shader, argumentName, structure, ioOutputReg, inFirstOutput, inOutputCount);
    }
    else
    {
        expandSimpleOutputArgumentWrite(shader, argumentName, elementType, ioOutputReg, inFirstOutput, inOutputCount);
    }
}

static void
expandStreamStructureSamplerDecls(std::ostream & shader,
                                  const std::string & argumentName,
                                  int inArgumentIndex, int inComponentIndex,
                                  StructDef * structure, int &ioIndex, int &ioSamplerReg, pass_info & outPass, int rank)
{
    LOG(LOG_FN_ENTRY, "expandStreamStructureSamplerDecls(%p, %s, %d, %d, %p, %p, %p, %p, %d)\n",
        &shader, argumentName.c_str(), inArgumentIndex, inComponentIndex, structure, &ioIndex, &ioSamplerReg,
        &outPass, rank);
    assert(!structure->isUnion());

    int elementCount = structure->nComponents;

    for (int i = 0; i < elementCount; i++)
    {
        Decl *elementDecl = structure->components[i];

        if (elementDecl->storage & ST_Static)
            continue;
        Type *form = elementDecl->form;

        // TIM: for now
        assert(form->isBaseType());
        BaseType *base = form->getBase();
        StructDef *structure = findStructureDef(base);

        if (structure)
            expandStreamStructureSamplerDecls(shader, argumentName, inArgumentIndex, inComponentIndex,
                                              structure, ioIndex, ioSamplerReg, outPass, rank);
        else
        {
            std::ostringstream name;
            name << "__structsampler" << ioIndex++ << "_" << argumentName;

            shader << "uniform _stype" << rank << " " << name.str();
            shader << " : register (s" << ioSamplerReg++ << ")";
            shader << ",\n\t\t";

            bril::addTextureAndEmit(shader, ioSamplerReg - 1, name.str(), rank);

            outPass.addSampler(inArgumentIndex, inComponentIndex + i);
        }
    }
}

static void
expandStreamSamplerDecls(std::ostream & shader,
                         const std::string & inArgumentName,
                         int inArgumentIndex, int inComponentIndex, Type * inForm, int &samplerreg, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "expandStreamSamplerDecls(%p, %s, %d, %d, %p, %p, %p)\n",
        &shader, inArgumentName.c_str(), inArgumentIndex, inComponentIndex, inForm, &samplerreg, &outPass);
    StructDef *structure = NULL;
    int rank = 2;               // TIM: TODO: find out how to get correct rank, for now assume 2

    if (inForm->isStream())
    {
        BrtStreamType *streamType = (BrtStreamType *) (inForm);

        // TIM: can't handle arrays with a BaseType
        BaseType *elementType = streamType->getBase();

        structure = findStructureDef(elementType);
    }
    else
    {
        assert((inForm->getQualifiers() & TQ_Reduce) != 0);
        structure = findStructureDef(inForm);
    }

    if (structure)
    {
        int index = 0;

        expandStreamStructureSamplerDecls(shader, inArgumentName,
                                          inArgumentIndex, inComponentIndex,
                                          structure, index, samplerreg, outPass, rank);
    }
    else
    {
        // it had better be just a floatN
        // Output a sampler, texcoord, and scale_bias for
        // a stream
        shader << "uniform _stype" << rank << " _tex_" << inArgumentName;
        shader << " : register (s" << samplerreg++ << ")";
        shader << ",\n\t\t";

        bril::addTextureAndEmit(shader, samplerreg - 1, inArgumentName, rank);

        outPass.addSampler(inArgumentIndex, inComponentIndex);
    }
}

static void
expandStreamStructureFetches(std::ostream & shader,
                             const std::string & argumentName,
                             const std::string & fieldName,
                             StructDef * structure, int &ioIndex,
                             const std::string & positionName, const std::string & texposName)
{
    LOG(LOG_FN_ENTRY, "expandStreamStructureFetches(%p, %s, %s, %p, %p, %s, %s)\n",
        &shader, argumentName.c_str(), fieldName.c_str(), structure, &ioIndex, positionName.c_str(), texposName.c_str());
    assert(!structure->isUnion());

    int elementCount = structure->nComponents;

    for (int i = 0; i < elementCount; i++)
    {
        Decl *elementDecl = structure->components[i];

        if (elementDecl->storage & ST_Static)
            continue;
        Type *form = elementDecl->form;

        std::string subFieldName = fieldName + "." + elementDecl->name->name;

        // TIM: for now
        assert(form->isBaseType());
        BaseType *base = form->getBase();
        StructDef *structure = findStructureDef(base);

        if (structure)
        {
            expandStreamStructureFetches(shader, argumentName, subFieldName,
                                         structure, ioIndex, positionName, texposName);

        }
        else
        {
            shader << subFieldName << " = ";

            //const char* brname = getBaseTypeName(base->typemask, BaseTypeBRName);
            const char *shname = getBaseTypeName(base->typemask, BaseTypeSHName);

            if (shname == NULL)
                shader << "__fetchrunknown";
            else
                shader << "__fetch_" << shname;

            std::ostringstream name;
            name << "__structsampler" << ioIndex++ << "_" << argumentName;

            shader << "( " << name.str() << ", _tex_" << positionName << "_pos );\n";

            bril::store * s = bril::getNameStore(positionName);
            s = get_struct_elt(s, ioIndex - 1);
            u32 tex = bril::getTextureId(name.str());

            //u32 cds = bril::getTexcoordId("_tex_" + positionName + "_pos");

            bril::emitFetch(shader, s, tex, texposName, getGatherRank(base));

        }
    }
}

static void
expandStreamFetches(std::ostream & shader, const std::string & argumentName,
                    Type * inForm, bril::store * texpos, const char *inPositionName = NULL)
{
    LOG(LOG_FN_ENTRY, "expandStreamFetches(%p, %s, %p, %p, %s)\n",
        &shader, argumentName.c_str(), inForm, texpos, inPositionName);
    StructDef *structure = NULL;
    Type *elementType = NULL;

    std::string positionName = (inPositionName != NULL) ? inPositionName : argumentName;

    if (inForm->isStream())
    {
        BrtStreamType *streamType = (BrtStreamType *) (inForm);

        // TIM: can't handle arrays with a BaseType
        elementType = streamType->getBase();
        structure = findStructureDef(elementType);
    }
    else
    {
        assert((inForm->getQualifiers() & TQ_Reduce) != 0);
        elementType = inForm;
        structure = findStructureDef(inForm);
    }

    std::ostringstream texposName;
    if (texpos)
    {
        texposName << get_gpr_name(texpos, bril::rs_base);
    }
    else
    {
        std::string texcoordName;
        if (globals.enableGPUAddressTranslation)
            texcoordName = "__outputtexcoord";
        else
            texcoordName = "_tex_" + positionName + "_pos";
        u32 cds = bril::getTexcoordId(texcoordName);

        texposName << "v" << cds;
    }

    if (structure)
    {

        bril::store * s = bril::getNameStore(positionName);
        inForm->allocateILStorage(s);

        int index = 0;

        expandStreamStructureFetches(shader, positionName, argumentName,
                                     structure, index, positionName, texposName.str());
    }
    else
    {
        shader << positionName << " = ";
        BaseType *base = inForm->getBase();

        if (base->typemask & BT_UserType)
        {
            base = base->typeName->entry->uVarDecl->form->getBase();
        }

        int UNUSED(rank) = getGatherRank(inForm);

        //const char* brname = getBaseTypeName(base->typemask, BaseTypeBRName);
        const char *shname = getBaseTypeName(base->typemask, BaseTypeSHName);

        if (shname == NULL)
            shader << "__fetchrunknown";
        else
            shader << "__fetch_" << shname;

        shader << "(_tex_" << argumentName << ", _tex_" << positionName << "_pos );\n";

        bril::store * s = bril::getNameStore(positionName);
        inForm->allocateILStorage(s);
        u32 tex = bril::getTextureId(argumentName);

        bril::emitFetch(shader, s, tex, texposName.str(), getGatherRank(inForm));
    }

}

static void generate_shader_support(std::ostream & shader)
{
    LOG(LOG_FN_ENTRY, " generate_shader_support(%p)\n", &shader);
    shader << "#if defined(DXPIXELSHADER)\n";
    shader << "#define fixed float\n";
    shader << "#define fixed2 float2\n";
    shader << "#define fixed3 float3\n";
    shader << "#define fixed4 float4\n";
    shader << "#endif\n";
    shader << "#define shortfixed float\n";
    shader << "#define shortfixed2 float2\n";
    shader << "#define shortfixed3 float3\n";
    shader << "#define shortfixed4 float4\n";
    shader << "#define double real\n";
    shader << "#define double2 real2\n";
    shader << "typedef struct double_struct {float2 x;} real;\n";
    shader << "typedef struct double2_struct {float4 xy;} real2;\n";
    shader << "#if defined(DXPIXELSHADER) || !defined(USERECT)\n";
    shader << "#define _stype1   sampler2D\n";
    shader << "#define _stype2   sampler2D\n";
    shader << "#define _stype3   sampler3D\n";
    shader << "#define _stype4   sampler2D\n";
    shader << "#if defined(SHADERMODEL3)\n";
    shader << "#define __sample1(s,i) tex2Dlod((s),float4(i,0,0,0))\n";
    shader << "#define __sample2(s,i) tex2Dlod((s),float4(i,0,0))\n";
    shader << "#define __sample3(s,i) tex3Dlod((s),float4(i,0))\n";
    shader << "#else\n";
    shader << "#define __sample1(s,i) tex2D((s),float2(i,0))\n";
    shader << "#define __sample2(s,i) tex2D((s),(i))\n";
    shader << "#define __sample3(s,i) tex3D((s),(i))\n";
    shader << "#endif\n";
    shader << "#else\n";
    shader << "#define _stype1  samplerRECT\n";
    shader << "#define _stype2  samplerRECT\n";
    shader << "#define _stype3  sampler3D\n";
    shader << "#define _stype4   sampler2D\n";
    shader << "#define __sample1(s,i) texRECT((s),float2(i,0))\n";
    shader << "#define __sample2(s,i) texRECT((s),(i))\n";
    shader << "#define __sample3(s,i) tex3D((s),(i))\n";
    shader << "#endif\n\n";

    shader << "#define __FRAGMENTKILL discard\n";

    shader << "#ifdef USERECT\n";
    shader << "#define SKIPSCALEBIAS\n";
    shader << "#endif\n\n";

    if (!globals.enableGPUAddressTranslation)
    {
        shader << "#ifdef SKIPSCALEBIAS\n";
        shader << "float __gatherindex1( float index, float4 scalebias ) { ";
        shader << "return (index+scalebias.z); }\n";
        shader << "float2 __gatherindex2( float2 index, float4 scalebias ) { ";
        shader << "return (index+scalebias.zw); }\n";
        shader << "float3 __gatherindex3( float3 index, float4 scalebias ) { ";
        shader << "return index; }\n";
        shader << "#define _computeindexof(a,b) float4(a, 0, 0)\n";
        shader << "#else\n";
        shader << "float __gatherindex1( float index, float4 scalebias ) { ";
        shader << "return index*scalebias.x+scalebias.z; }\n";
        shader << "float2 __gatherindex2( float2 index, float4 scalebias ) { ";
        shader << "return index*scalebias.xy+scalebias.zw; }\n";
        shader << "float3 __gatherindex3( float3 index, float4 scalebias ) { ";
        shader << "return index; }\n";
        shader << "#define _computeindexof(a,b) (b)\n";
        shader << "#endif\n\n";
    }

    // TIM: simple subroutines
    shader << "double __fetch_double( _stype1 s, float i ) { double r; r.x= __sample1(s,i).xy; return r;}\n";
    shader << "double __fetch_double( _stype2 s, float2 i ) { double r; r.x = __sample2(s,i).xy; return r;}\n";
    shader << "double __fetch_double( _stype3 s, float3 i ) { double r; r.x = __sample3(s,i).xy; return r;}\n";

    shader << "double2 __fetch_double2( _stype1 s, float i ) { double2 r; r.xy= __sample1(s,i).xyzw; return r;}\n";
    shader << "double2 __fetch_double2( _stype2 s, float2 i ) { double2 r; r.xy = __sample2(s,i).xyzw; return r;}\n";
    shader << "double2 __fetch_double2( _stype3 s, float3 i ) { double2 r; r.xy = __sample3(s,i).xyzw; return r;}\n";

    shader << "float __fetch_float( _stype1 s, float i ) { return __sample1(s,i).x; }\n";
    shader << "float __fetch_float( _stype2 s, float2 i ) { return __sample2(s,i).x; }\n";
    shader << "float __fetch_float( _stype3 s, float3 i ) { return __sample3(s,i).x; }\n";
    shader << "float2 __fetch_float2( _stype1 s, float i ) { return __sample1(s,i).xy; }\n";
    shader << "float2 __fetch_float2( _stype2 s, float2 i ) { return __sample2(s,i).xy; }\n";
    shader << "float2 __fetch_float2( _stype3 s, float3 i ) { return __sample3(s,i).xy; }\n";
    shader << "float3 __fetch_float3( _stype1 s, float i ) { return __sample1(s,i).xyz; }\n";
    shader << "float3 __fetch_float3( _stype2 s, float2 i ) { return __sample2(s,i).xyz; }\n";
    shader << "float3 __fetch_float3( _stype3 s, float3 i ) { return __sample3(s,i).xyz; }\n";
    shader << "float4 __fetch_float4( _stype1 s, float i ) { return __sample1(s,i).xyzw; }\n";
    shader << "float4 __fetch_float4( _stype2 s, float2 i ) { return __sample2(s,i).xyzw; }\n";
    shader << "float4 __fetch_float4( _stype3 s, float3 i ) { return __sample3(s,i).xyzw; }\n";

    shader << "\n\n";

    shader << "float __gather_float( _stype1 s[1], float i ) { return __sample1(s[0],i).x; }\n";
    shader << "float __gather_float( _stype2 s[1], float2 i ) { return __sample2(s[0],i).x; }\n";
    shader << "float __gather_float( _stype3 s[1], float3 i ) { return __sample3(s[0],i).x; }\n";
    shader << "float2 __gather_float2( _stype1 s[1], float i ) { return __sample1(s[0],i).xy; }\n";
    shader << "float2 __gather_float2( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xy; }\n";
    shader << "float2 __gather_float2( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xy; }\n";
    shader << "float3 __gather_float3( _stype1 s[1], float i ) { return __sample1(s[0],i).xyz; }\n";
    shader << "float3 __gather_float3( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xyz; }\n";
    shader << "float3 __gather_float3( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xyz; }\n";
    shader << "float4 __gather_float4( _stype1 s[1], float i ) { return __sample1(s[0],i).xyzw; }\n";
    shader << "float4 __gather_float4( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xyzw; }\n";
    shader << "float4 __gather_float4( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xyzw; }\n";

    shader << "float __gather_shortfixed( _stype1 s[1], float i ) { return __sample1(s[0],i).x; }\n";
    shader << "float __gather_shortfixed( _stype2 s[1], float2 i ) { return __sample2(s[0],i).x; }\n";
    shader << "float __gather_shortfixed( _stype3 s[1], float3 i ) { return __sample3(s[0],i).x; }\n";
    shader << "float2 __gather_shortfixed2( _stype1 s[1], float i ) { return __sample1(s[0],i).xy; }\n";
    shader << "float2 __gather_shortfixed2( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xy; }\n";
    shader << "float2 __gather_shortfixed2( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xy; }\n";
    shader << "float3 __gather_shortfixed3( _stype1 s[1], float i ) { return __sample1(s[0],i).xyz; }\n";
    shader << "float3 __gather_shortfixed3( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xyz; }\n";
    shader << "float3 __gather_shortfixed3( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xyz; }\n";
    shader << "float4 __gather_shortfixed4( _stype1 s[1], float i ) { return __sample1(s[0],i).xyzw; }\n";
    shader << "float4 __gather_shortfixed4( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xyzw; }\n";
    shader << "float4 __gather_shortfixed4( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xyzw; }\n";

    shader << "float __gather_fixed( _stype1 s[1], float i ) { return __sample1(s[0],i).x; }\n";
    shader << "float __gather_fixed( _stype2 s[1], float2 i ) { return __sample2(s[0],i).x; }\n";
    shader << "float __gather_fixed( _stype3 s[1], float3 i ) { return __sample3(s[0],i).x; }\n";
    shader << "float2 __gather_fixed2( _stype1 s[1], float i ) { return __sample1(s[0],i).xy; }\n";
    shader << "float2 __gather_fixed2( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xy; }\n";
    shader << "float2 __gather_fixed2( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xy; }\n";
    shader << "float3 __gather_fixed3( _stype1 s[1], float i ) { return __sample1(s[0],i).xyz; }\n";
    shader << "float3 __gather_fixed3( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xyz; }\n";
    shader << "float3 __gather_fixed3( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xyz; }\n";
    shader << "float4 __gather_fixed4( _stype1 s[1], float i ) { return __sample1(s[0],i).xyzw; }\n";
    shader << "float4 __gather_fixed4( _stype2 s[1], float2 i ) { return __sample2(s[0],i).xyzw; }\n";
    shader << "float4 __gather_fixed4( _stype3 s[1], float3 i ) { return __sample3(s[0],i).xyzw; }\n";

    if (globals.enableGPUAddressTranslation)
    {
        shader << "\n\n";

        shader <<
            "float4 __calculateindexof( float4 indexofoutput, float4 streamIndexofNumer, float4 streamIndexofInvDenom ) {\n";
        shader << "\treturn floor( (indexofoutput*streamIndexofNumer + 0.5)*streamIndexofInvDenom ); }\n";

        shader << "float2 __calculatetexpos( float4 streamIndex, float4 streamDomainMin,\n";
        shader << "float4 linearizeConst, float4 textureShapeConst ) {\n";
        shader << "float linearIndex = dot( streamIndex + streamDomainMin, linearizeConst ) + 0.5;\n";
        shader << "float2 texIndex;\n";
        shader << "texIndex.y = floor( linearIndex * textureShapeConst.x );\n";
        shader << "texIndex.x = floor( linearIndex - texIndex.y * textureShapeConst.z );\n";
        shader << "float2 texCoord = texIndex + 0.5;\n";
        shader << "#ifndef USERECT\n";
        shader << "// convert to 0-to-1 texture space\n";
        shader << "texCoord *= textureShapeConst.xy;\n";
        shader << "#endif\n";
#ifndef TIMISCRAZY
        shader << "return texCoord;\n}\n\n";
#else
        shader << "return streamIndex.y;\n}\n\n";
#endif

        shader << "void __calculateoutputpos( float2 interpolant, float2 linearize,\n";
        shader <<
            "\tfloat4 stride, float4 invStride, float4 invExtent, float4 domainMin, float4 domainExtent, out float4 index ) {\n";
        shader << "\tfloat2 cleanInterpolant = floor( interpolant );\n";
        shader << "\tfloat linearIndex = dot( cleanInterpolant, linearize );\n";
        shader << "\tfloat4 temp0 = floor( (linearIndex + 0.5) * invStride );\n";
        shader << "\tfloat4 temp1 = linearIndex - temp0*stride;\n";
        shader << "\tindex = floor( (temp1 + 0.5) * invExtent - domainMin );\n";
        shader << "\tif( any( index < 0 ) ) __FRAGMENTKILL;\n";
        shader << "\tif( any( index >= domainExtent ) ) __FRAGMENTKILL;\n";
        shader << "}\n\n";

        shader << "float4 __calculateiteratorvalue( float4 index,\n"
               << "float4 valueBase, float4 valueOffset1, float4 valueOffset4 ) {\n"
               << "return valueBase + index.x*valueOffset1 + index*valueOffset4;\n}\n\n";

#if 0
        shader << "void __calculateoutputpos( float2 interpolant, float4 outputConst, out float4 index ) {\n";
        shader << "\tfloat2 cleanInterpolant = floor(interpolant);\n";
        shader << "\tfloat linearIndex = cleanInterpolant.y*outputConst.x + cleanInterpolant.x;\n";
        shader << "\tfloat scaledIndex = linearIndex * outputConst.y;\n";
        shader << "\tfloat fraction = frac( scaledIndex );\n";
        shader << "\tindex.y = scaledIndex - fraction;\n";
        shader << "\tindex.x = floor( index.y * outputConst.z + linearIndex + outputConst.w );\n";
//    shader << "\tindex.x = floor( fraction * outputConst.z + outputConst.w );\n";
        shader << "\tindex.z = 0;\n";
        shader << "\tindex.w = 0;\n";
        shader << "}\n\n";
#endif

        shader <<
            "float2 __gatherindex1( float1 index, float4 domainConst, float4 linearizeConst, float4 reshapeConst ) {\n";
        shader <<
            "\treturn __calculatetexpos( float4(floor(index+domainConst.x),0,0,0), 0, linearizeConst, reshapeConst ); }\n";
        shader <<
            "float2 __gatherindex2( float2 index, float4 domainConst, float4 linearizeConst, float4 reshapeConst ) {\n";
        shader <<
            "\treturn __calculatetexpos( float4(floor(index+domainConst.xy),0,0), 0, linearizeConst, reshapeConst ); }\n";
        shader <<
            "float2 __gatherindex3( float3 index, float4 domainConst, float4 linearizeConst, float4 reshapeConst ) {\n";
        shader <<
            "\treturn __calculatetexpos( float4(floor(index+domainConst.xyz),0), 0, linearizeConst, reshapeConst ); }\n";
        shader <<
            "float2 __gatherindex4( float4 index, float4 domainConst, float4 linearizeConst, float4 reshapeConst ) {\n";
        shader << "\treturn __calculatetexpos( floor(index+domainConst), 0, linearizeConst, reshapeConst ); }\n";
    }

    shader << "\n\n";
}

static void
generate_shader_iter_arg(std::ostream & shader, Decl * arg, int i, int &texcoord, int &constant, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "generate_shader_iter_arg(%p, %p, %d, %p, %p, %p)\n", &shader, arg, i, &texcoord, &constant, &outPass);
    std::string argName = arg->name->name;
    TypeQual qual = arg->form->getQualifiers();

    if (globals.enableGPUAddressTranslation)
    {
        emitConstRegFloat4(shader, "__iterindexofnumer_" + argName, constant++);
        emitConstRegFloat4(shader, "__iterindexofdenom_" + argName, constant++);
        emitConstRegFloat4(shader, "__itervaluebase_" + argName, constant++);
        emitConstRegFloat4(shader, "__itervalueoffset1_" + argName, constant++);
        emitConstRegFloat4(shader, "__itervalueoffset4_" + argName, constant++);

        outPass.addConstant((i + 1), "kIteratorConstant_ATIndexofNumer");
        outPass.addConstant((i + 1), "kIteratorConstant_ATIndexofDenom");
        outPass.addConstant((i + 1), "kIteratorConstant_ATValueBase");
        outPass.addConstant((i + 1), "kIteratorConstant_ATValueOffset1");
        outPass.addConstant((i + 1), "kIteratorConstant_ATValueOffset4");

        // no real support under address translation yet
    }
    else
    {
        // Just output a texcoord for an iterator
        arg->form->getBase()->qualifier &= ~TQ_Iter;
        arg->form->printBase(shader, 0);
        arg->form->getBase()->qualifier = qual;
        shader << argName << " : TEXCOORD" << texcoord++;
        shader << ",\n";

        bril::addTexcoordAndEmit(shader, texcoord - 1, argName);

        bril::store * store = bril::getNameStore(argName);
        bril::allocate_store(store, 4);

        shader << IL_PREFIX_STRING << "mov " << bril::get_gpr_name(store,
                                                                   bril::rs_dst_sel) << ", v" << texcoord - 1 << "\n";

        shader << "\t\t";

        outPass.addInterpolant((i + 1), "kIteratorInterpolant_Value");
    }
}

static void
generate_shader_out_arg(std::ostream & shader, Decl * arg,
                        bool & hasDoneIndexofOutput, bool needIndexOfArg,
                        int i, int &texcoord, int &constreg, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "generate_shader_out_arg(%p, %p, %p, %d, %d, %p, %p, %p)\n", &shader, arg, &hasDoneIndexofOutput,
        needIndexOfArg, i, &texcoord, &constreg, &outPass);
    std::string argName = arg->name->name;

    if (globals.enableGPUAddressTranslation)
    {
        // index of output should already be available...
    }
    else if (!hasDoneIndexofOutput && needIndexOfArg)
    {
        hasDoneIndexofOutput = true;
        emitConstRegFloat4(shader, "_const_" + argName + "_invscalebias", constreg++);

        shader << "float2 _tex_" << argName << "_pos : TEXCOORD" << texcoord++;
        shader << ",\n\t\t";

        bril::addTexcoordAndEmit(shader, texcoord - 1, "_tex_" + argName + "_pos");

        outPass.addConstant((i + 1), "kOutputConstant_Indexof");
        outPass.addInterpolant((i + 1), "kOutputInterpolant_Position");
    }
}

static void
generate_reduction_stream_arg(std::ostream & shader, Decl * arg,
                              bool & reductionArgumentComesBeforeStreamArgument,
                              std::vector < int >&reductionStreamArguments,
                              int reductionFactor, int i,
                              int &texcoord, int &samplerreg, pass_info & outPass, int &constreg)
{
    LOG(LOG_FN_ENTRY, "generate_reduction_stream_arg(%p, %p, %p, %p, %d, %d, %p, %p, %p, %p)\n",
        &shader, arg, &reductionArgumentComesBeforeStreamArgument, &reductionStreamArguments, reductionFactor,
        i, &texcoord, &samplerreg, &outPass, &constreg);
    std::string argName = arg->name->name;
    TypeQual qual = arg->form->getQualifiers();

    if ((qual & TQ_Reduce) != 0 && reductionStreamArguments.size() == 0)
        reductionArgumentComesBeforeStreamArgument = true;

    reductionStreamArguments.push_back(i);

    if (reductionStreamArguments.size() == 2)
    {
        expandStreamSamplerDecls(shader, argName, 0, 1, arg->form, samplerreg, outPass);
        if (globals.enableGPUAddressTranslation == false)
        {
            for (int r = 2; r < reductionFactor; r++)
            {
                std::stringstream s;
                s << "__reduce" << r;
                std::string adjustedArgName = s.str();

                shader << "float2 _tex_" << adjustedArgName << "_pos : TEXCOORD" << texcoord++;
                shader << ",\n";

                std::ostringstream name;
                name << "_tex_" << adjustedArgName << "_pos";
                bril::addTexcoordAndEmit(shader, texcoord - 1, name.str());

                shader << "\t\t";

                outPass.addInterpolant(0, r - 1);
            }
        }
    }
    else
    {
        expandStreamSamplerDecls(shader, argName, 0, 0, arg->form, samplerreg, outPass);
    }

    if (globals.enableGPUAddressTranslation)
    {
#if 0
        if (reductionFactor >= 2)
        {
            shader << "float2 _tex_pos_reduce_" << argName << " : TEXCOORD" << texcoord++;
            shader << ",\n";

            shader << "\t\t";
            shader << "float2 _tex_pos_reduce_dummy_" << argName << " : TEXCOORD99";
            shader << ",\n";

            std::ostringstream name;
            name << "_tex_pos_reduce_" << argName ;
            bril::addTexcoordAndEmit(shader, texcoord - 1, name.str());

            shader << "\t\t";

            outPass.addInterpolant(0, 0);
        }
#endif
        emitConstRegFloat4(shader, "__streamindexofnumer_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamindexofdenom_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamlinearize_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamtextureshape_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamdomainmin_" + argName, constreg++);

        outPass.addConstant((i + 1), "kStreamConstant_ATIndexofNumer");
        outPass.addConstant((i + 1), "kStreamConstant_ATIndexofDenom");
        outPass.addConstant((i + 1), "kStreamConstant_ATLinearize");
        outPass.addConstant((i + 1), "kStreamConstant_ATTextureShape");
        outPass.addConstant((i + 1), "kStreamConstant_ATDomainMin");
    }
    else
    {

        shader << "float2 _tex_" << argName << "_pos : TEXCOORD" << texcoord++;
        shader << ",\n";

#if 1
        //FIXME - Temporarily adding a dummy texcoord so they go into separate slots
        shader << "\t\t";
        shader << "float2 _tex_dummy_" << argName << "_pos : TEXCOORD99";
        shader << ",\n";
#endif

        std::ostringstream name;
        name << "_tex_" << argName << "_pos";
        bril::addTexcoordAndEmit(shader, texcoord - 1, name.str());

        shader << "\t\t";

        outPass.addInterpolant(0, (reductionStreamArguments.size() == 2) ? reductionFactor - 1 : 0);
    }
}

static void
generate_map_stream_arg(std::ostream & shader, Decl * arg, bool needIndexOfArg, int i,
                        int &texcoord, int &constreg, int &samplerreg, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "generate_map_stream_arg(%p, %p, %d, %d, %p, %p, %p, %p)\n", &shader, arg, i,
        &texcoord, &constreg, &samplerreg, &outPass);
    std::string argName = arg->name->name;

    expandStreamSamplerDecls(shader, argName, (i + 1), 0, arg->form, samplerreg, outPass);

    if (globals.enableGPUAddressTranslation)
    {
        emitConstRegFloat4(shader, "__streamindexofnumer_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamindexofdenom_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamlinearize_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamtextureshape_" + argName, constreg++);
        emitConstRegFloat4(shader, "__streamdomainmin_" + argName, constreg++);

        outPass.addConstant((i + 1), "kStreamConstant_ATIndexofNumer");
        outPass.addConstant((i + 1), "kStreamConstant_ATIndexofDenom");
        outPass.addConstant((i + 1), "kStreamConstant_ATLinearize");
        outPass.addConstant((i + 1), "kStreamConstant_ATTextureShape");
        outPass.addConstant((i + 1), "kStreamConstant_ATDomainMin");
    }
    else
    {
        // Output a texcoord, and optional scale/bias
        if (needIndexOfArg)
        {
            emitConstRegFloat4(shader, "_const_" + argName + "_invscalebias", constreg++);

            outPass.addConstant((i + 1), "kStreamConstant_Indexof");
        }
        shader << "float2 _tex_" << argName << "_pos : TEXCOORD" << texcoord++;
        shader << ",\n";
#if 1
        //FIXME - Temporarily adding a dummy texcoord so they go into separate slots
        shader << "\t\t";
        shader << "float2 _tex_dummy_" << argName << "_pos : TEXCOORD99";
        shader << ",\n";
#endif

        std::ostringstream name;
        name << "_tex_" << argName << "_pos";
        bril::addTexcoordAndEmit(shader, texcoord - 1, name.str());

        shader << "\t\t";

        outPass.addInterpolant((i + 1), "kStreamInterpolant_Position");
    }
}

static void
generate_shader_gather_arg(std::ostream & shader, Decl * arg, int i,
                           int &constreg, int &samplerreg, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "generate_shader_gather_arg(%p, %p, %d, %p, %p, %p)\n",
        &shader, arg, i, &constreg, &samplerreg, &outPass);
    std::string argName = arg->name->name;
    int samplerCount = getGatherStructureSamplerCount(arg->form);
    int rank = getGatherRank(arg->form);

    shader << "uniform _stype" << rank << " " << argName;
    shader << "[" << samplerCount << "] : register (s" << samplerreg << ")" << ",\n\t\t";

    for (int s = 0; s < samplerCount; s++)
    {
        outPass.addSampler((i + 1), s);
        std::string textureName;
        if (s == 0)
            textureName = argName;
        else
            textureName = argName + as_string(s);

        bril::addTextureAndEmit(shader, samplerreg + s, textureName, rank);
    }

    samplerreg += samplerCount;

    if (globals.enableGPUAddressTranslation)
    {
        emitConstRegFloat4(shader, "__gatherlinearize_" + argName, constreg++);
        emitConstRegFloat4(shader, "__gathertexshape_" + argName, constreg++);
        emitConstRegFloat4(shader, "__gatherdomainmin_" + argName, constreg++);

        outPass.addConstant((i + 1), "kGatherConstant_ATLinearize");
        outPass.addConstant((i + 1), "kGatherConstant_ATTextureShape");
        outPass.addConstant((i + 1), "kGatherConstant_ATDomainMin");
    }
    else
    {
        // TIM: TODO: handle multi-sampler array for gathers...

        emitConstRegFloat4(shader, "_const_" + argName + "_scalebias", constreg++);
        outPass.addConstant((i + 1), "kGatherConstant_Shape");
    }
}

static void
generate_shader_uniform_arg(std::ostream & shader, Decl * args, int index, int &constreg, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "generate_shader_uniform_arg(%p, %p, %d, %p, %p)\n", &shader, args, index, &constreg, &outPass);
    shader << "uniform ";
    args->print(shader, true);
    shader << " : register (c" << constreg++ << ")";
    shader << ",\n\t\t";

    {
        // Since the DX10 compiler packs the uniform constants into constant buffers, we introduce dummy uniforms such that
        // every new uniform constant is 16byte (float4) aligned.
        // This is to make the HLSL generated compatible with Microsoft's new fxc compiler when compiling to ps_4_0 targets.
        // todo: Data types other than float are not current take into account.
        static int dummyCounter = 0;
        char dummyType[16];
        bool bDummyNeeded = false;

        switch (args->form->getBase()->typemask)
        {
        case BT_Float:
            strcpy(dummyType, "float3");
            bDummyNeeded = true;
            break;
        case BT_Float2:
            strcpy(dummyType, "float2");
            bDummyNeeded = true;
            break;
        case BT_Float3:
            strcpy(dummyType, "float");
            bDummyNeeded = true;
            break;
        }

        if (bDummyNeeded)
            shader << "uniform " << dummyType << " dummy" << dummyCounter++ << ", \n\t\t";
    }

    bril::name_extern_constant(constreg - 1, args->name->name);

    outPass.addConstant((index + 1), 0);
}

static void
generate_shader_main(std::ostream & shader, Decl ** args, int nArgs, const char *functionName,
                     int inFirstOutput, int inOutputCount,
                     bool fullAddressTrans, int reductionFactor, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "generate_shader_main(%p, %p, %d, %s, %d, %d, %d, %d, %p)\n", &shader, args, nArgs,
        functionName, inFirstOutput, inOutputCount, fullAddressTrans, reductionFactor, &outPass);

    std::vector < int >reductionStreamArguments;
    int texcoord, constreg, samplerreg, outputReg, i;
    bool reductionArgumentComesBeforeStreamArgument = false;
    bool isReduction, hasDoneIndexofOutput;

    isReduction = false;
    for (i = 0; i < nArgs; i++)
    {
        if ((args[i]->form->getQualifiers() & TQ_Reduce) != 0)
        {
            isReduction = true;
            break;
        }
    }

    shader << "void main (\n\t\t";

    // Print the argument list

    hasDoneIndexofOutput = false;
    constreg = texcoord = samplerreg = outputReg = 0;
    for (i = 0; i < nArgs; i++)
    {
        std::string argName = args[i]->name->name;
        TypeQual qual = args[i]->form->getQualifiers();
        bool needIndexOfArg = FunctionProp[functionName].contains(i);

        /* put the output in the argument list */
        if ((qual & TQ_Out) != 0 || (qual & TQ_Reduce) != 0)
        {
            expandOutputArgumentDecl(shader, argName, isReduction ? 0 : (i + 1),
                                     0, args[i]->form, outputReg, inFirstOutput, inOutputCount, outPass);
        }

        if (args[i]->isStream() || (qual & TQ_Reduce) != 0 ||
            (args[i]->isArray() && (qual & TQ_Out) !=0) )
        {
            if ((qual & TQ_Iter) != 0)
            {
                generate_shader_iter_arg(shader, args[i], i, texcoord, constreg, outPass);
            }
            else if ((qual & TQ_Out) != 0)
            {
                generate_shader_out_arg(shader, args[i], hasDoneIndexofOutput,
                                        needIndexOfArg, i, texcoord, constreg, outPass);
            }
            else
            {
                if (isReduction)
                {
                    assert(!needIndexOfArg && "can't use indexof in a reduction");
                    generate_reduction_stream_arg(shader, args[i],
                                                  reductionArgumentComesBeforeStreamArgument,
                                                  reductionStreamArguments,
                                                  reductionFactor, i, texcoord, samplerreg, outPass, constreg);
                }
                else
                {
                    generate_map_stream_arg(shader, args[i], needIndexOfArg, i,
                                            texcoord, constreg, samplerreg, outPass);
                }
            }
        }
        else if (args[i]->isArray())
        {
            generate_shader_gather_arg(shader, args[i], i, constreg, samplerreg, outPass);
        }
        else
        {
            generate_shader_uniform_arg(shader, args[i], i, constreg, outPass);
        }
    }

    /*
     * Output the bonus arguments.
     *
     * Put them at the end so that the parameter numbering isn't perturbed
     * (especially since there's a different number of bonus arguments with
     * and without address translation).
     */

    if (!globals.enableGPUAddressTranslation)
    {
        // Add the workspace variable
        emitConstRegFloat4(shader, "__workspace", constreg++, false);
    }
    else
    {
        shader << "float2 __outputtexcoord : TEXCOORD" << texcoord++;
        shader << ",\n\t\t";
        bril::addTexcoordAndEmit(shader, texcoord - 1, "__outputtexcoord");
#if 1
        //FIXME - Temporarily adding a dummy texcoord so they go into separate slots
        shader << "\t\t";
        shader << "float2 __dummy_outputtexcoord : TEXCOORD99";
        shader << ",\n";
#endif

        // TODO: I think this might be a vertex shader. Gotta look carefully someday. dgessel

        shader << "float2 __outputaddrinterpolant : TEXCOORD" << texcoord++;
        shader << ",\n\t\t";
        bril::addTexcoordAndEmit(shader, texcoord - 1, "__outputaddrinterpolant");
#if 1
        //FIXME - Temporarily adding a dummy texcoord so they go into separate slots
        shader << "\t\t";
        shader << "float2 __dummy_outputaddrinterpolant : TEXCOORD99";
        shader << ",\n";
#endif

        emitConstRegFloat4(shader, "__outputlinearize", constreg++);
        emitConstRegFloat4(shader, "__outputstride", constreg++);
        emitConstRegFloat4(shader, "__outputinvstride", constreg++);
        emitConstRegFloat4(shader, "__outputinvextent", constreg++);
        emitConstRegFloat4(shader, "__outputdomainmin", constreg++);
        emitConstRegFloat4(shader, "__outputdomainsize", constreg++);
        emitConstRegFloat4(shader, "__outputinvshape", constreg++);

        shader << "uniform float __hackconst : register(c" << constreg++ << ")";
        bril::name_extern_constant(constreg - 1, "__hackconst");

        outPass.addInterpolant(0, "kGlobalInterpolant_ATOutputTex");
        outPass.addInterpolant(0, "kGlobalInterpolant_ATOutputAddress");
        outPass.addConstant(0, "kGlobalConstant_ATOutputLinearize");
        outPass.addConstant(0, "kGlobalConstant_ATOutputStride");
        outPass.addConstant(0, "kGlobalConstant_ATOutputInvStride");
        outPass.addConstant(0, "kGlobalConstant_ATOutputInvExtent");
        outPass.addConstant(0, "kGlobalConstant_ATOutputDomainMin");
        outPass.addConstant(0, "kGlobalConstant_ATOutputDomainSize");
        outPass.addConstant(0, "kGlobalConstant_ATOutputInvShape");
        outPass.addConstant(0, "kGlobalConstant_ATHackConstant");
    }

    shader << ") {\n";

    /*
     * Declare the stream variables
     */

    for (i = 0; i < nArgs; i++)
    {
        TypeQual qual = args[i]->form->getQualifiers();

        if ((qual & TQ_Iter) != 0)
            continue;

        if ((qual & TQ_Out) != 0)
        {
            args[i]->form->getBase()->qualifier &= ~TQ_Out;
            shader << "\t";
            args[i]->form->printBase(shader, 0);
            shader << " " << *args[i]->name << ";\n";
            args[i]->form->getBase()->qualifier = qual;
        }
        else if (args[i]->isStream() || (qual & TQ_Reduce) != 0)
        {
            shader << "\t";
            args[i]->form->printBase(shader, 0);
            shader << " " << *args[i]->name << ";\n";
        }
    }

    if (globals.enableGPUAddressTranslation)
    {
        // set up output position values
        shader << "\tfloat4 __indexofoutput;\n";
        shader << "\t__calculateoutputpos( __outputaddrinterpolant, "
               << "__outputlinearize, __outputstride, __outputinvstride, "
               << "__outputinvextent, __outputdomainmin, __outputdomainsize, __indexofoutput );\n";

        bril::store * pilIndexofoutput = bril::getNameStore("__indexofoutput");
        bril::allocate_store(pilIndexofoutput, 4);
        printILCalculateoutputpos(shader, pilIndexofoutput);
    }

    /*
     * Perform stream fetches
     */

    hasDoneIndexofOutput = false;
    for (i = 0; i < nArgs; i++)
    {
        TypeQual qual = args[i]->form->getQualifiers();

        std::string argName = args[i]->name->name;

        if ((qual & TQ_Iter) != 0)
        {
            if (!globals.enableGPUAddressTranslation)
                continue;       /* No texture fetch for iterators */

            bril::store * pilIndexof = bril::getNameStore("__indexof_" + argName);
            bril::allocate_store(pilIndexof, 4);
            bril::store * pilIndexofoutput = bril::getNameStore("__indexofoutput");

            if (!fullAddressTrans)
            {
                shader << "\tfloat4 __indexof_" << argName << " = __indexofoutput;\n";

                bril::emitCopy(shader, pilIndexof, pilIndexofoutput);
            }
            else
            {
                if (reductionFactor >= 2)
                {
                    shader << "\tfloat4 __indexof_" << argName << " = __indexofoutput * "
                        << reductionFactor << " + ("
                        << reductionFactor << " - 1);\n";

                    printILCalculateindexof(shader, pilIndexof, pilIndexofoutput,
                        reductionFactor, reductionFactor-1);
                }
                else
                {
                    shader << "\tfloat4 __indexof_" << argName << " = ";
                    shader << "__calculateindexof( __indexofoutput, __iterindexofnumer_" << argName
                        << ", __iterindexofdenom_" << argName << " );\n";

                    printILCalculateindexof(shader, pilIndexof, argName, pilIndexofoutput, false);
                }
            }

            shader << "\tfloat4 " << argName << " = __calculateiteratorvalue("
                   << "__indexof_" << argName
                   << ", __itervaluebase_" << argName
                   << ", __itervalueoffset1_" << argName << ", __itervalueoffset4_" << argName << ");\n";

            //add the local variable with name argName
            bril::store * pilArg = bril::getNameStore(argName);
            bril::allocate_store(pilArg, 4);
            printILCalculateiteratorvalue(shader, pilArg, pilIndexof, argName);

        }
        else if (args[i]->isStream() ||
            (args[i]->isArray()&&(args[i]->form->getQualifiers()&TQ_Out)!=0) ||
            (qual & TQ_Reduce) != 0)
        {
            if ((qual & TQ_Out) != 0)
            {
                if (globals.enableGPUAddressTranslation)
                {
                    //copy the value from "__indexofoutput" to "_indexof_"[resultvarname].
                    FunctionDef *funcdef = find_gpu_kernel_by_name(functionName);
                    SymEntry *funcvar = find_kernel_parameter_by_name(funcdef, "__indexof_" + args[i]->name->name);

                    if (funcvar)
                    {
                        bril::store * pilIndexofoutput = bril::getNameStore("__indexofoutput");
                        bril::emitCopy(shader, funcvar->get_il_store(), pilIndexofoutput);
                    }
                }
                else if (!hasDoneIndexofOutput && FunctionProp[functionName].contains(i))
                {
                    hasDoneIndexofOutput = true;
                    shader << "\t" << "float4 __indexofoutput = "
                           << "_computeindexof( "
                           << "_tex_" << *args[i]->name << "_pos, "
                           << "round(float4( _tex_" << *args[i]->name << "_pos*"
                           << "_const_" << *args[i]->name << "_invscalebias.xy + "
                           << "_const_" << *args[i]->name << "_invscalebias.zw,0,0)));\n";

                    FunctionDef *fd = find_gpu_kernel_by_name(functionName);
                    SymEntry *sym = find_kernel_parameter_by_name(fd, "__indexof_" + args[i]->name->name);

                    std::string cds_name = "_tex_" + args[i]->name->name + "_pos";
                    shader << IL_PREFIX_STRING << "mov " << sym->
                        getILRegName(bril::rs_dst_sel) << ", v" << bril::getTexcoordId(cds_name) << ".xy00\n";
                    shader << IL_PREFIX_STRING << "round_nearest " << sym->getILRegName(bril::rs_dst_sel) << ", " << sym->
                        getILRegName(bril::rs_src_sel) << "\n";

                }
            }
            else
            {
                //(qual & TQ_Out) == 0
                bril::store * pilTexpos = NULL;

                if (globals.enableGPUAddressTranslation)
                {

                    FunctionDef *funcdef = find_gpu_kernel_by_name(functionName);
                    SymEntry *funcvar = find_kernel_parameter_by_name(funcdef, "__indexof_" + args[i]->name->name);

                    bril::store * pilIndexof;
                    if (funcvar)
                        pilIndexof = funcvar->get_il_store();
                    else
                    {
                        pilIndexof = bril::getNameStore("__indexof_" + argName);
                        bril::allocate_store(pilIndexof, 4);
                    }

                    pilTexpos = bril::getNameStore("_tex_" + argName + "_pos");
                    bril::allocate_store(pilTexpos, 4);

                    bril::store * pilIndexofoutput = bril::getNameStore("__indexofoutput");
                    if (!fullAddressTrans)
                    {
                        shader << "\tfloat4 __indexof_" << argName << " = __indexofoutput;\n";
                        shader << "\tfloat2 _tex_" << argName << "_pos = __outputtexcoord;\n";

                        bril::emitCopy(shader, pilIndexof, pilIndexofoutput);
                        shader << IL_PREFIX_STRING << "mov "
                               << bril::get_gpr_name(pilTexpos, bril::rs_dst_sel)
                               << ", v" << bril::getTexcoordId("__outputtexcoord") << "\n";
                    }
                    else
                    {
                        if (reductionFactor >= 2)
                        {
                            int reduceOffset;
                            if ((qual & TQ_Reduce) != 0)
                                reduceOffset = reductionFactor - 1;
                            else
                                reduceOffset = 0;

                            shader << "\tfloat4 __indexof_" << argName << " = __indexofoutput * "
                                << reductionFactor << " + " << reduceOffset << " ;\n";

                            printILCalculateindexof(shader, pilIndexof, pilIndexofoutput,
                                reductionFactor, reduceOffset);

                        }
                        else
                        {
                            shader << "\tfloat4 __indexof_" << argName << " = ";
                            shader << "__calculateindexof( __indexofoutput, __streamindexofnumer_" << argName <<
                                ", __streamindexofdenom_" << argName;
                            shader << " );\n";

                            printILCalculateindexof(shader, pilIndexof, args[i]->name->name, pilIndexofoutput);
                        }

                        shader << "\tfloat2 _tex_" << argName << "_pos = ";
                        shader << "__calculatetexpos( __indexof_" << argName << ", ";
                        shader << "__streamdomainmin_" << argName << ", ";
                        shader << "__streamlinearize_" << argName << ", ";
                        shader << "__streamtextureshape_" << argName << " );\n";

                        printILCalculatetexpos(shader, pilTexpos, args[i]->name->name, pilIndexof);

                        if ( reductionFactor >= 2 && (qual & TQ_Reduce) == 0)
                        {
                            for (int r = 2; r < reductionFactor; r ++)
                            {
                                std::ostringstream texposname;
                                texposname << "_tex__reduce" << r << "_pos";
                                shader << "\tfloat4 " << texposname.str() << " = ";

                                shader << "__calculatetexpos( __indexof_" << argName << " + " << (r-1) << ", ";
                                shader << "__streamdomainmin_" << argName << ", ";
                                shader << "__streamlinearize_" << argName << ", ";
                                shader << "__streamtextureshape_" << argName << " );\n";

                                //indexof += 1;
                                shader << IL_PREFIX_STRING << "add "
                                    << bril::get_gpr_name(pilIndexof, bril::rs_dst_sel) << ", "
                                    << bril::get_gpr_name(pilIndexof, bril::rs_src_sel) << ", "
                                    << "r1.1111 \n";

                                bril::store *tmpTexpos = bril::getNameStore(texposname.str().c_str());
                                bril::allocate_store(tmpTexpos, 4);
                                printILCalculatetexpos(shader, tmpTexpos, args[i]->name->name, pilIndexof);
                            }
                        }

                    }
                }

                expandStreamFetches(shader, args[i]->name->name, args[i]->form, pilTexpos);
                if (!globals.enableGPUAddressTranslation && FunctionProp[functionName].contains(i))
                {
                    shader << "\t" << "float4 __indexof_" << *args[i]->name << " = "
                           << "_computeindexof( "
                           << "_tex_" << *args[i]->name << "_pos, "
                           << "round(float4( _tex_" << *args[i]->name << "_pos*"
                           << "_const_" << *args[i]->name << "_invscalebias.xy + "
                           << "_const_" << *args[i]->name << "_invscalebias.zw,0,0)));\n";

                    int pos = bril::getTexcoordId("_tex_" + args[i]->name->name + "_pos");

                    //int inv_sb = bril::get_extern_constant(  "_const_" + args[i]->name->name + "_invscalebias" );

                    SymEntry *indexof =
                        find_extra_kernel_parameter_by_name(functionName, "__indexof_" + args[i]->name->name);

                    shader << IL_PREFIX_STRING << "round_nearest "
                        << indexof->getILRegName(bril::rs_dst_sel) << ", "
                        << "v" << pos << ".xy00\n";
                }
            }
        }
    }

    // Print the body of the kernel

    //  shader << body << std::endl;
    // TIM: just call the body as a subroutine
    shader << std::endl;

    std::stringstream kernelBodyStream;

    kernelBodyStream << "\t" << functionName << "(\n";
    kernelBodyStream << "\t\t";

    for (i = 0; i < nArgs; i++)
    {
        if (i != 0)
            kernelBodyStream << ",\n\t\t";

        std::string name = args[i]->name->name;
        if (args[i]->isArray() && (args[i]->form->getQualifiers() & TQ_Out)==0)
        {
            if (globals.enableGPUAddressTranslation)
            {
                kernelBodyStream << name;
                kernelBodyStream << ", __gatherlinearize_" << name;
                kernelBodyStream << ", __gathertexshape_" << name;
                kernelBodyStream << ", __gatherdomainmin_" << name;
            }
            else
            {
                kernelBodyStream << name << ", _const_" + name + "_scalebias";
                //kernelBodyStream << name << ", __gatherconst_" << name;
            }
        }
        else
            kernelBodyStream << name;
    }

    std::set < unsigned int >::iterator indexofIterator = FunctionProp[functionName].begin();
    std::set < unsigned int >::iterator indexofEnd = FunctionProp[functionName].end();

    for (; indexofIterator != indexofEnd; ++indexofIterator)
    {
        if ((args[*indexofIterator]->form->getQualifiers() & TQ_Out) == 0)
            kernelBodyStream << ",\n\t\t__indexof_" << args[*indexofIterator]->name->name;
        else
            kernelBodyStream << ",\n\n\t__indexofoutput";
    }

    kernelBodyStream << " );\n\n";

    FunctionDef *fd = find_gpu_kernel_by_name(functionName);
    FunctionType *ft = static_cast < FunctionType * >(fd->decl->form);

    for (i = 0; i < int (ft->nArgs); i++)
    {
        std::string name = ft->args[i]->name->name;
        if (ft->args[i]->isArray())
        {
            // This is the sampler; we don't pass it in IL.
        }
        else
        {
            if ((ft->args[i]->form->getQualifiers() & TQ_GeneratedGatherConst) == TQ_GeneratedGatherConst)
            {
                kernelBodyStream << "\n" << IL_PREFIX_STRING << "mov " << ft->args[i]->name->
                    getILRegName(bril::rs_dst_sel) << ", " << bril::get_extern_constant(name) << "\n";
            }
            else if ((ft->args[i]->form->getQualifiers() & TQ_Out) != TQ_Out)
            {
                if (ft->args[i]->isStream() || (ft->args[i]->form->getQualifiers() & TQ_Reduce) != 0)
                {
                    bril::store * dst = ft->args[i]->name->get_il_store();
                    bril::store * src = bril::getNameStore(name);
                    bril::emitCopy(kernelBodyStream, dst, src);
                }
                else if ((ft->args[i]->form->getQualifiers() & TQ_GeneratedIndexOf) == 0)
                {
                    kernelBodyStream << IL_PREFIX_STRING << "mov " << ft->args[i]->name->
                        getILRegName(bril::rs_dst_sel) << ", " << bril::get_extern_constant(name) << "\n";
                }
            }
        }
    }

    kernelBodyStream << IL_PREFIX_STRING << "call 0\n";

    // Copy out parameters for IL
    for (i = 0; i < int (ft->nArgs); i++)
    {
        std::string name = ft->args[i]->name->name;
        if (ft->args[i]->isArray())
        {
            // TODO Array?
            if ((ft->args[i]->form->getQualifiers() & (TQ_Out | TQ_Reduce)) != TQ_None)
            {
                bril::store * s = bril::getNameStore(name);
                ft->args[i]->form->allocateILStorage(s);
                bril::emitCopy(kernelBodyStream, s, ft->args[i]->name->get_il_store());
            }
        }
        else
        {
            if ((ft->args[i]->form->getQualifiers() & (TQ_Out | TQ_Reduce)) != TQ_None)
            {
                bril::store * s = bril::getNameStore(name);
                ft->args[i]->form->allocateILStorage(s);
                bril::emitCopy(kernelBodyStream, s, ft->args[i]->name->get_il_store());
            }
        }
    }

    std::string kernelBody = kernelBodyStream.str();

    // if we are doing a reduction, we may want to run the kernel
    // body multiple times to reduce n values...
    if (isReduction)
    {
        int r;

        assert(reductionStreamArguments.size() == 2);
        int leftArgumentIndex = reductionStreamArguments[0];

        std::string leftArgumentName = args[leftArgumentIndex]->name->name;
        int rightArgumentIndex = reductionStreamArguments[1];

        std::string rightArgumentName = args[rightArgumentIndex]->name->name;
        Type *leftArgumentForm = args[leftArgumentIndex]->form;

        // do additional fetches...
        for (r = 2; r < reductionFactor; r++)
        {
            std::stringstream s;
            s << "__reduce" << r;
            std::string argName = s.str();

            shader << "\t";
            leftArgumentForm->printBase(shader, 0);
            shader << " " << argName << ";\n";

            // do a new fetch for the reduction arg
            bril::store *texposstore = NULL;
            if (globals.enableGPUAddressTranslation)
            {
               std::ostringstream texposname;
               texposname << "_tex__reduce" << r << "_pos";
               texposstore = bril::getNameStore(texposname.str().c_str());
            }

            expandStreamFetches(shader, leftArgumentName, leftArgumentForm, texposstore, argName.c_str());
        }

        // save off the right arg...
        shader << "\t";
        leftArgumentForm->printBase(shader, 0);
        shader << " __saved = " << rightArgumentName << ";\n";

        bril::store tmp;
        bril::allocate_store(&tmp, 4);
        shader << IL_PREFIX_STRING << "mov r" << tmp.
            idx << ", " << bril::get_gpr_name(bril::getNameStore(rightArgumentName), bril::rs_dst_sel) << "\n";

        // do additional reduction ops
        for (r = 2; r < reductionFactor; r++)
        {
            std::stringstream s;
            s << "__reduce" << r;
            std::string argName = s.str();

            // shuffle stuff around... :)
            shader << "\t" << rightArgumentName << " = " << argName << ";\n";

            bril::emitCopy(shader, bril::getNameStore(rightArgumentName), bril::getNameStore(argName));

            shader << kernelBody;
            if (!reductionArgumentComesBeforeStreamArgument)
            {
                shader << "\t" << leftArgumentName << " = " << rightArgumentName << ";\n";

                bril::emitCopy(shader, bril::getNameStore(leftArgumentName), bril::getNameStore(rightArgumentName));
            }
        }

        shader << "\t" << rightArgumentName << " = __saved;\n";

        shader << IL_PREFIX_STRING << "mov " << bril::get_gpr_name(bril::getNameStore(rightArgumentName),
                                                                   bril::rs_dst_sel) << ", r" << tmp.idx << "\n";

        shader << kernelBody;
    }
    else
    {
        shader << kernelBody;
    }

    /*
     * Do any output unpacking
     */

    outputReg = 0;
    for (i = 0; i < nArgs; i++)
    {
        TypeQual qual = args[i]->form->getQualifiers();

        if ((qual & TQ_Out) == 0 && (qual & TQ_Reduce) == 0)
            continue;

        expandOutputArgumentWrite(shader, (args[i]->name)->name, args[i]->form, outputReg, inFirstOutput,
                                  inOutputCount);
    }

    shader << "}\n";

    shader << IL_PREFIX_STRING << "ret\n";

    instantiate_main_kernel(functionName);

}

/*
 * generate_hlsl_code --
 *
 *      This function takes a parsed kernel function as input and produces
 *      the CG code reflected, plus the support code required.
 */
static char *generate_shader_code(Decl ** args, int nArgs, const char *functionName,
                                  int inFirstOutput, int inOutputCount,
                                  bool fullAddressTrans, int reductionFactor, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, " *generate_shader_code(%p, %d, %s, %d, %d, %d, %d, %p)\n", args, nArgs, functionName,
        inFirstOutput, inOutputCount, fullAddressTrans, reductionFactor, &outPass);
    std::ostringstream shader_body;

    std::ostringstream shader;
    bril::begin_shader(shader, args, nArgs);

    // Do this first, so main will call with the right parameters
    expand_gpu_kernel_parameter_lists();

    // Generate main, so uniforms are correctly mapped to constant registers.
    // This prevents later immediates from using the wrong constant registers.
    std::ostringstream shader_main;
#if 1
    generate_shader_main(shader_main, args, nArgs, functionName,
                         inFirstOutput, inOutputCount, fullAddressTrans, reductionFactor, outPass);

    generate_shader_support(shader_body);
    generate_shader_structure_definitions(shader_body);
    generate_shader_subroutines(shader_body);
#else
    generate_shader_main(std::cout, args, nArgs, functionName,
                         inFirstOutput, inOutputCount, fullAddressTrans, reductionFactor, outPass);

    generate_shader_support(std::cout);
    generate_shader_structure_definitions(std::cout);
    generate_shader_subroutines(std::cout);
#endif
    char *il = extract_prefixed_il(shader_main.str().c_str());

    shader << il;
    free(il);

    shader << shader_body.str();

    char *non_il = strip_il(shader_main.str().c_str());

    shader << non_il;
    free(non_il);

    bril::end_shader(shader);

    return strdup(shader.str().c_str());
}

/*
 * append_argument_information --
 *
 *      Takes the fp code from the CG compiler and tacks on high level
 *      information from the original kernel function at the bottom.
 */

static char *append_argument_information(const char *commentstring, char *fpcode,
                                         Decl ** args, int nArgs,
                                         const char *functionName, int firstOutput, int outputCount,
                                         bool fullAddressTrans, int reductionFactor)
{
    LOG(LOG_FN_ENTRY, "append_argument_information(%s, %s, %p, %d, %s, %d, %d, %d, %d)\n",
        commentstring, fpcode, args, nArgs, functionName, firstOutput, outputCount,
        fullAddressTrans, reductionFactor);
    std::ostringstream fp;

    fp << fpcode;

    /* Add the brcc flag */
    fp << " \n" << commentstring << "!!BRCC\n";

    /* Include the program aguments */
    fp << commentstring << "narg:" << nArgs << std::endl;

    /* Add the argument information */
    for (int i = 0; i < nArgs; i++)
    {
        char type;
        int dimension = FloatGPUDimension(args[i]->form->getBase()->typemask);

        if ((args[i]->form->getQualifiers() & TQ_Out) != 0)
        {
            type = 'o';
        }
        else if (args[i]->isStream())
        {
            type = 's';
        }
        else
        {
            type = 'c';
        }

        fp << commentstring << type;
        if (FunctionProp[functionName].contains(i))
            fp << "i";
        fp << ":" << dimension << ":" << *args[i]->name << "\n";
    }

    //fp << commentstring << "workspace:" << globals.workspace << std::endl;

    fp << commentstring << "!!multipleOutputInfo:" << firstOutput << ":" << outputCount << ":" << std::endl;
    fp << commentstring << "!!fullAddressTrans:" << (fullAddressTrans ? 1 : 0) << ":" << std::endl;
    fp << commentstring << "!!reductionFactor:" << reductionFactor << ":" << std::endl;

    return strdup(fp.str().c_str());
}

/*
 * generate_c_code --
 *
 *      Spits out the compiled pixel shader
 *      code as a string available to the emitted
 *      C code.
 */

static char *generate_c_code(const std::vector < technique_info > &techniques, const char *name, const char *id)
{
    LOG(LOG_FN_ENTRY, "generate_c_code(%p, %s, %s)\n", &techniques, name, id);
    assert(name);
    assert(id);

    std::string mangledName = std::string("__") + name + "_" + id;

    std::ostringstream out;
    out << std::endl;
    if (techniques.size() == 0)
    {
        out << "static const void* " << mangledName << " = 0;" << std::endl;
    }
    else
    {
        out << "namespace {" << std::endl;
        out << "\tusing namespace ::brook::desc;" << std::endl;
        out << "\tstatic const gpu_kernel_desc " << mangledName << "_desc = gpu_kernel_desc()";

        for (std::vector < technique_info >::const_iterator i = techniques.begin(); i != techniques.end(); ++i)
        {
            out << std::endl;
            out << "\t\t.technique( gpu_technique_desc()" << std::endl;

            const technique_info & t = (*i);

            if (t.reductionFactor >= 2)
                out << "\t\t\t.reduction_factor(" << t.reductionFactor << ")" << std::endl;

            if (t.outputAddressTranslation)
                out << "\t\t\t.output_address_translation()" << std::endl;
            if (t.inputAddressTranslation)
                out << "\t\t\t.input_address_translation()" << std::endl;

            for (std::vector < pass_info >::const_iterator j = t.passes.begin(); j != t.passes.end(); ++j)
            {
                out << "\t\t\t.pass( gpu_pass_desc(" << std::endl;

                const pass_info & p = (*j);

                const char *code = p.shader.c_str();

                out << "\t\t\t\t\"";

                char c;

                while ((c = *code++) != '\0')
                {
                    if (c == '\n')
                        out << "\\n\"" << std::endl << "\t\t\t\t\"";
                    else
                        out << c;
                }

                out << "\")" << std::endl;

                std::vector < shader_input_info >::const_iterator k;
                for (k = p.constants.begin(); k != p.constants.end(); ++k)
                    out << "\t\t\t\t.constant(" << (*k).argumentIndex << ", " << (*k).componentName << ")" << std::endl;
                for (k = p.samplers.begin(); k != p.samplers.end(); ++k)
                    out << "\t\t\t\t.sampler(" << (*k).argumentIndex << ", " << (*k).componentName << ")" << std::endl;
                for (k = p.interpolants.begin(); k != p.interpolants.end(); ++k)
                    out << "\t\t\t\t.interpolant(" << (*k).argumentIndex << ", " << (*k).
                        componentName << ")" << std::endl;
                for (k = p.outputs.begin(); k != p.outputs.end(); ++k)
                    out << "\t\t\t\t.output(" << (*k).argumentIndex << ", " << (*k).componentName << ")" << std::endl;

                out << "\t\t\t)" << std::endl;
            }

            out << "\t\t)";
        }

        out << ";" << std::endl;
        out << "\tstatic const void* " << mangledName << " = &" << mangledName << "_desc;" << std::endl;

        out << "}" << std::endl;
    }

    return strdup(out.str().c_str());
}

int getShaderOutputCount(int argumentCount, Decl ** arguments, bool & outIsReduction)
{
    LOG(LOG_FN_ENTRY, "getShaderOutputCount(%d, %p, %p)\n", argumentCount, arguments, outIsReduction);
    int result = 0;
    bool isReduction = false;

    for (int i = 0; i < argumentCount; i++)
    {
        Type *form = arguments[i]->form;

        if ((form->getQualifiers() & TQ_Out) != 0)
            result += getGatherStructureSamplerCount(form);
        else if ((form->getQualifiers() & TQ_Reduce) != 0)
        {
            isReduction = true;
            result += getGatherStructureSamplerCount(form);
        }
    }
    outIsReduction = isReduction;
    return result;
}

/*
 * CodeGen_GenerateCode --
 *
 *      Takes a parsed kernel and crunches it down to C code:
 *              . Creates and annotates equivalent HLSL
 *              . Compiles the HLSL to ps20/fp30 assembly
 *              . Spits out the fragment program as a C string
 *
 *      Note: The caller is responsible for free()ing the returned string.
 */

static char *generateShaderPass(Decl ** args, int nArgs, const char *name, int firstOutput,
                                int outputCount, CodeGenTarget target, bool fullAddressTrans,
                                int reductionFactor, pass_info & outPass)
{
    LOG(LOG_FN_ENTRY, "generateShaderPass(%p, %d, %s, %d, %d, %d, %d, %d, %p)\n", args, nArgs,
        name, firstOutput, outputCount, target, fullAddressTrans, reductionFactor, &outPass);

    char *fpcode;
    char *fpcode_with_brccinfo;
    char *shadercode =
        generate_shader_code(args, nArgs, name, firstOutput, outputCount, fullAddressTrans, reductionFactor, outPass);
    if (shadercode)
    {
        //if (globals.verbose)
        // std::cerr << "\n***Produced this shader:\n" << shadercode << "\n";

        if (globals.keepFiles && (globals.userTargets & TARGET_PS30))
        {
            std::ofstream out;
            std::string fname = std::string(globals.shaderOutputName) + "_" + name + ".cg";

            out.open(fname.c_str());
            if (out.fail())
            {
                std::cerr << "***Unable to open " << fname << "\n";
            }
            else
            {
                out << shadercode;
                out.close();
            }
        }
        if (globals.verbose)
        {
            fprintf(stderr, "Generating %s code for %s outputs [%d, %d).\n",
                    CodeGen_TargetName(target), name, firstOutput, firstOutput + outputCount);
        }

        assert(target < CODEGEN_NUM_TARGETS && target >= 0);

        if (shadercompile[target] == NULL)
            return NULL;

        fpcode = shadercompile[target] ((std::string(globals.shaderOutputName) + "_" + name).c_str(),
                                        shadercode, target, 0, true);

        if (fpcode == NULL)
        {
            fprintf(stderr, "for kernel %s.\n", name);
        }
        free(shadercode);
    }
    else
    {
        fpcode = NULL;
    }

    if (fpcode)
    {
        // if (globals.verbose)
        //   std::cerr << "***Produced this assembly:\n" << fpcode << "\n";

        const char *commentString = ";;";

        switch (target)
        {
        case CODEGEN_PS20:
        case CODEGEN_PS2B:
        case CODEGEN_PS2A:
        case CODEGEN_PS30:
            commentString = "//";
            break;
        default:
            break;
        }

        // TIM: the argument-info string is obsolete, and should go
        // away once all runtimes parse the new info...
        fpcode_with_brccinfo =
            append_argument_information(commentString,
                                        fpcode, args, nArgs, name, firstOutput, outputCount, fullAddressTrans,
                                        reductionFactor);
        free(fpcode);

        // if (globals.verbose)
        //  std::cerr << "***Produced this instrumented assembly:\n"
        //  << fpcode_with_brccinfo << "\n";
    }
    else
    {
        fpcode_with_brccinfo = NULL;
    }

    return fpcode_with_brccinfo;
}

static bool generateShaderTechnique(Decl ** args, int nArgs, const char *name,
                                    CodeGenTarget target, bool fullAddressTrans,
                                    int reductionFactor, technique_info & outTechnique)
{
    LOG(LOG_FN_ENTRY, " generateShaderTechnique(%p, %d, %s, %d, %d, %d, %p)\n",
        args, nArgs, name, target, fullAddressTrans, reductionFactor, &outTechnique);
    bool isReduction = false;
    int outputCount = getShaderOutputCount(nArgs, args, isReduction);
    int maxOutputsPerPass = 1;

    if (!isReduction)
    {
        if (target == CODEGEN_CAL)
        {
            maxOutputsPerPass = 8;
        }
        else
        {
            maxOutputsPerPass = 4;
        }
    }

    outTechnique.reductionFactor = reductionFactor;

    if (globals.enableGPUAddressTranslation)
    {
        outTechnique.outputAddressTranslation = true;
        if (fullAddressTrans)
            outTechnique.inputAddressTranslation = true;
    }

    int outputsLeft = outputCount;
    int firstOutput = 0;

    while (outputsLeft > 0)
    {
        int outputsToWrite = maxOutputsPerPass;

        if (outputsLeft < maxOutputsPerPass)
            outputsToWrite = outputsLeft;

        char *shaderString = NULL;
        pass_info pass;

        while (outputsToWrite > 0)
        {
            pass_info pass;

            shaderString = generateShaderPass(args, nArgs, name, firstOutput,
                                              outputsToWrite, target, fullAddressTrans, reductionFactor, pass);
            if (shaderString)
            {
                pass.shader = shaderString;
                outTechnique.passes.push_back(pass);
                free(shaderString);
                break;
            }

            // try again with fewer outputs
            outputsToWrite--;

            // we have failed if we can't even do one output
            if (outputsToWrite == 0)
                return false;
        }

        firstOutput += outputsToWrite;
        outputsLeft -= outputsToWrite;
    }
    return true;
}

static bool
generateReductionTechniques(Decl ** args, int nArgs, const char *name,
                            CodeGenTarget target, bool fullAddressTrans, std::vector < technique_info > &ioTechniques)
{
    LOG(LOG_FN_ENTRY, "generateReductionTechniques(%p, %d, %s, %d, %d, %p)\n",
        args, nArgs, name, target, fullAddressTrans, &ioTechniques);
    bool isReduction = false;

    /*  int outputCount = */ getShaderOutputCount(nArgs, args, isReduction);

    if (globals.enableGPUAddressTranslation
        && isReduction && fullAddressTrans == false)
        return true;

    if (!isReduction)
    {
        technique_info technique;

        if (!generateShaderTechnique(args, nArgs, name, target, fullAddressTrans, 0, technique))
            return false;
        ioTechniques.push_back(technique);
        return true;
    }

    int reductionFactor = 2;

    while (reductionFactor <= 8)        // TIM: evil unnamed constant... :)
    {
        technique_info technique;

        if (!generateShaderTechnique(args, nArgs, name, target, fullAddressTrans, reductionFactor, technique))
        {
            if (reductionFactor == 2)
                return false;
        }
        else
        {
            ioTechniques.push_back(technique);
        }

        reductionFactor++;
    }

    return true;
}

char *CodeGen_GenerateCode(Type *retType, const char *name,
                           Decl ** args, int nArgs, const char *body, CodeGenTarget target)
{
    LOG(LOG_FN_ENTRY, "CodeGen_GenerateCode(%p, %s, %p, %d, %s, %d)\n",
        retType, name, args, nArgs, body, target);
    std::vector < technique_info > techniques;

    if (globals.enableGPUAddressTranslation)
    {
        //TIM: huge hack to get a not address-trans version available
        //TIM: we'd *like* to do this, but it won't actually work,
        // because subkernel calls will not have gather arguments converted correctly

        globals.enableGPUAddressTranslation = false;
        generateReductionTechniques( args, nArgs, name, target, false, techniques );
        globals.enableGPUAddressTranslation = true;


        // only address-translate input streams
        generateReductionTechniques(args, nArgs, name, target, false, techniques);

        // address-translate the output stream too... ugly
        generateReductionTechniques(args, nArgs, name, target, true, techniques);
    }
    else
    {
        generateReductionTechniques(args, nArgs, name, target, false, techniques);
    }

    char *c_code = generate_c_code(techniques, name,
                                   CodeGen_TargetName(target));

    // if (globals.verbose)
    //  std::cerr << "***Produced this C code:\n" << c_code;

    return c_code;
}
