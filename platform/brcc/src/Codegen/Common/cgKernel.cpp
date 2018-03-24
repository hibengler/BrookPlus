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

#include <iomanip>

#include "cgKernel.h"
#include "cgProgram.h"
#include "brtdecl.h"

//fixme:

#include "logger.hpp"
#include "main.h"
#include "brtkernel.h"
#include "brtstemnt.h"
#include "brtreduce.h"
#include "brtexpress.h"

#define shaderLinePrefix "\t\t"

// ----------------------------------------------------------------------------
CgInput::CgInput(int arg, int comp, const std::string &hlsln, int resid, InputType type)
    :resourceId(resid), hlslName(hlsln), inputType(type)
{
    LOG(LOG_FN_ENTRY, "CgInput::CgInput(%d, %d, %s, %d)\n", arg, comp, hlsln.c_str(), resid);
    argumentIndex = arg;

    std::ostringstream out;
    out << comp;
    componentName = out.str();
}

CgInput::CgInput(int arg, const std::string & comp, const std::string &hlsln, int resid, InputType type)
    : argumentIndex(arg), componentName(comp), resourceId(-1), hlslName(hlsln), inputType(type)
{
  LOG(LOG_FN_ENTRY, "CgInput::CgInput(%d, %s, %s, %d)\n", arg, comp.c_str(), hlsln.c_str(), resid);
}

void CgInput::gen(std::ostream &outs, char* lineprefix, char* name) const
{
    LOG(LOG_FN_ENTRY, " CgInput::gen(%p, %s, %s)\n", &outs, lineprefix, name);

    //! There may be good idea but if we want use constant buffer for stream also
    std::ostringstream tempStr;
    if(inputType == INPUT_CONSTBUFFER)
        tempStr << ".constArray";
    else
        tempStr << name;
    outs << lineprefix << tempStr.str().c_str() << "("
         << (argumentIndex - 1) << ", " << componentName;

    if(inputType == INPUT_ACCESS_RANDOM)
    {
        outs << ", ACCESS_RANDOM";
    }
    else if(inputType == INPUT_ACCESS_STREAM)
    {
        outs << ", ACCESS_STREAM";
    }
    else if(inputType == OUTPUT_ACCESS_RANDOM)
    {
        outs << ", ACCESS_RANDOM";
    }
    else if(inputType == OUTPUT_ACCESS_STREAM)
    {
        outs << ", ACCESS_STREAM";
    }

    outs << ")" << std::endl;
}

static void
genInput(const CgInputTbl& allinput, std::ostream &outs, char* lineprefix, char* name)
{
    LOG(LOG_FN_ENTRY, "genInput(%p, %p, %s, %s)\n", &allinput, &outs, lineprefix, name);
    //CgInputIter iter;
    //for (iter = allinput.begin(); iter!= allinput.end(); iter++)

    for (int i = 0; i < AsInt(allinput.size()); i ++)
    {
        allinput[i].gen(outs, lineprefix, name);
    }
}

static CgInput *
findInput(CgInputTbl& allinput, const std::string &hlslname)
{
    LOG(LOG_FN_ENTRY, "findInput(%p, %s)\n", &allinput, hlslname.c_str());
    for (int i = 0; i < AsInt(allinput.size()); i ++)
        if (allinput[i].getHlslName() == hlslname)
            return &(allinput[i]);

    return NULL;
}

// ----------------------------------------------------------------------------
void
CgPass::adjustSampler(const std::string &hlslname, int resourceid)
{
    LOG(LOG_FN_ENTRY, "CgPass::adjustSampler(%s, %d)\n", hlslname.c_str(), resourceid);
    CgInput *entry = findInput(samplers, hlslname);
    CGASSERT(entry != NULL, "problem with meta data output by hlsl-compiler");

#if 0 //! This assert has no value if we use const buffer for gather arrays
    if (entry->getResourceId() != resourceid)
    {
        if(entry->isInputasConstBuffer() == true)
            return;
        cgErrorMsg("Adjust sampler allocation. old %d new %d", entry->getResourceId(), resourceid);
        CGASSERT(0, "need a mechanism to feedback the sampler id to runtime");
        entry->setResourceId(resourceid);
    }
#endif 
}

void
CgPass::adjustOutput(const std::string &hlslname, int resourceid)
{
    LOG(LOG_FN_ENTRY, "CgPass::adjustOutput(%s, %d)\n", hlslname.c_str(), resourceid);
    CgInput *entry = findInput(outputs, hlslname);
    CGASSERT(entry != NULL, "problem with meta data output by hlsl-compiler");

    if (entry->getResourceId() != resourceid)
    {
        cgErrorMsg("Adjust output allocation. old %d new %d", entry->getResourceId(), resourceid);
        CGASSERT(0, "need a mechanism to feedback the output resource id to runtime");
        entry->setResourceId(resourceid);
    }
}

void
CgPass::adjustScatter(const std::string &hlslname, int resourceid)
{
    LOG(LOG_FN_ENTRY, "CgPass::adjustOutput(%s, %d)\n", hlslname.c_str(), resourceid);
    CgInput *entry = findInput(scatter, hlslname);
    CGASSERT(entry != NULL, "problem with meta data output by hlsl-compiler");

    if (entry->getResourceId() != resourceid)
    {
        cgErrorMsg("Adjust scatter allocation. old %d new %d", entry->getResourceId(), resourceid);
        CGASSERT(0, "need a mechanism to feedback the output resource id to runtime");
        entry->setResourceId(resourceid);
    }
}

void
CgPass::adjustConstant(const std::string &hlslname, int byteoffset, int bytesize)
{
    CgInput *entry = findInput(outputs, hlslname);

    //dummy variable is not recorded
    if (entry == NULL)
        return;

    if ((entry->getResourceId()*4) != byteoffset)
    {
        cgErrorMsg("need a mechanism to feedback constant offset to runtime. old %d new %d",
            entry->getResourceId()*4, byteoffset);
        CGASSERT(0, "need a mechanism to feedback constant offset to runtime");
        //cgErrorMsg("Adjust output allocation. old %d new %d", entry->getResourceId(), resourceid);
        //entry->setResourceId(byteoffset/4); assert byteoffset % 4 == 0
    }
}

void
CgPass::genMetadata(std::ostream &outs, char *lineprefix) const
{
    LOG(LOG_FN_ENTRY, "CgPass::genMetadata(%p, %s)\n", &outs, lineprefix);

    genInput(constants, outs, lineprefix, ".constant");
    genInput(samplers, outs, lineprefix, ".input");

    genInput(outputs, outs, lineprefix, ".output");
    genInput(scatter, outs, lineprefix, ".output");

}

void
CgPass::genShader( std::ostream &outs, char *lineprefix ) const
{
    LOG(LOG_FN_ENTRY, "CgPass::genShader(%p, %s)\n", &outs, lineprefix);
    const char *code = shader.c_str();
    char cc = '\0';
    
    //  Some compilers don't allow string literals longer than 64KB.
    //  Hence we output string literals only when we think they're short
    //  enough.
    //  To be safe check the limit a little lower, because we replace each
    //  new-line character with two characters '\' and 'n', increasing the
    //  size of the string literal.
    if ( (shader.size() < 60 * 1024) && (!globals.itemizeShaderString) )
    {
        outs << '"';
        while ( (cc = *code++) != '\0' )
        {
            if ( cc == '\n' )
            {
                outs << "\\n\"" << std::endl << lineprefix << '"';
            }
            else
            {
                outs << cc;
            }
        }
        outs << "\";" << std::endl;
        return;
    }
    int ii = 0;
    outs << "{\n" << lineprefix << " " << std::hex;
    while ( (cc = *code++) != '\0' )
    {
        outs << " 0x" << std::setw(2) << std::setfill('0') << int(cc) << ",";
        if ( 15 == ii++ )
        {
            ii = 0;
            outs << std::endl << lineprefix << " ";
        }
    }
    outs << " 0x0 };\n" << std::dec; // and reset the int format to decimal.
}

void
CgPass::setShader(const char* sh)
{
    LOG(LOG_FN_ENTRY, "CgPass::setSehader(%s)\n", sh);
    if (sh)
        shader = std::string(sh);
}

// ----------------------------------------------------------------------------

CgTechnique::CgTechnique(int ref, bool outadd, bool inadd)
{
    LOG(LOG_FN_ENTRY, "CgTechnique::CgTechnique(%d, %d, %d)\n", ref, outadd, inadd);
    reduceFactor = ref;
    addressTranslation = outadd;
    //inputAddrVirt = inadd;
}

void CgTechnique::genMetadata(std::ostream &outs, char* lineprefix) const
{
    LOG(LOG_FN_ENTRY, " CgTechnique::genMetadata(%p, %s)\n", &outs, lineprefix);
    if (reduceFactor >=2)
       outs << lineprefix << ".reduction_factor(" << reduceFactor << ")" << std::endl;

   if (addressTranslation)
       outs << lineprefix << ".addressTranslation()" << std::endl;
   //if (inputAddrVirt)
   //    outs << lineprefix << ".input_address_translation()" << std::endl;
}

// ----------------------------------------------------------------------------

CgKernel::CgKernel(CgFunction *func, std::string& name, int rt, int rfactor)
    : kernelVarName(name)
{
    LOG(LOG_FN_ENTRY, "CgKernel::CgKernel(%p, %s, %d, %d)\n", func, name.c_str(), rt, rfactor);
    cgFunc = func;
    mrt = rt;
    maxReductionFactor = rfactor;
}

static void
impIrEdit_5(CgFunction *func, void *dummy)
{
    LOG(LOG_FN_ENTRY, "impIrEdit_3(%p, %p)\n", func, dummy);
    func->irEdit_5(dummy != NULL);
}

//! Returns the kernel function name for which code generating
std::string 
CgKernel::getCurKernelName()
{ 
    if(cgFunc)
        return cgFunc->getFuncName();
    else 
        return std::string("");
}

void
CgKernel::codegen(std::ostream &outs, bool targetOn)
{
    LOG(LOG_FN_ENTRY, "CgKernel::codegen(%p, %d)\n", &outs, targetOn);
    if (targetOn && cgFunc->isEntry())
    {
        std::string msg = "";
        msg.append(cgFunc->getTargetString());
        msg.append(" back end failed to compile kernel \"");
        msg.append(cgFunc->getFuncName());
        msg.append("\"");
        
        if (cgFunc->isReduction() == false )
        {
            bool oldvalue = globals.enableGPUAddressTranslation;
            globals.enableGPUAddressTranslation = false;

            cgFunc->getCgProgram()->applyCgFunction(impIrEdit_5, NULL);

            CgTechnique technique(0);
            if (genTechnique(technique) == 0)
                allTechnique.push_back(technique);
            else
            {
                RecordSemanticNote(SemanticChecker::SCBackEndCodeGenFailed, msg.c_str());
                globals.codeGenFailed = true;
            }

            globals.enableGPUAddressTranslation = oldvalue;

            if (globals.enableGPUAddressTranslation == true)
            {
                cgFunc->getCgProgram()->applyCgFunction(impIrEdit_5,(void*)1);

                CgTechnique technique1(0, true);
                if (genTechnique(technique1) == 0)
                    allTechnique.push_back(technique1);
                else
                {
                    RecordSemanticNote(SemanticChecker::SCBackEndCodeGenFailed, msg.c_str());
                    globals.codeGenFailed = true;
                }
                //CgTechnique technique2(0, true, true);
                //if (genTechnique(technique2) == 0)
                //    allTechnique.push_back(technique2);
            }
        }
        else
        {
            bool oldvalue = globals.enableGPUAddressTranslation;
            globals.enableGPUAddressTranslation = false;

            cgFunc->getCgProgram()->applyCgFunction(impIrEdit_5, NULL);
            CgTechnique technique(0);
            if (genTechnique(technique) == 0)
                allTechnique.push_back(technique);
            else
            {
                RecordSemanticNote(SemanticChecker::SCBackEndCodeGenFailed, msg.c_str());
                globals.codeGenFailed = true;
            }

            globals.enableGPUAddressTranslation = oldvalue;

            if (globals.enableGPUAddressTranslation == true)
            {
                cgFunc->getCgProgram()->applyCgFunction(impIrEdit_5, (void*)1);

                CgTechnique technique1(0, true);
                if (genTechnique(technique1) == 0)
                    allTechnique.push_back(technique1);
                else
                {
                    RecordSemanticNote(SemanticChecker::SCBackEndCodeGenFailed, msg.c_str());
                    globals.codeGenFailed = true;
                }

                /*CgTechnique technique2(i, true, true);
                if (genTechnique(technique2) == 0)
                    allTechnique.push_back(technique2);*/
            }
        }
    }

    //output code to construct the kernel desc object
    outs << std::endl;
    if (allTechnique.size() == 0)
    {
        if (cgFunc->isEntry())
            outs << "\tstatic const void* " << kernelVarName << " = 0;" << std::endl;
    }
    else
    {
        outs << "namespace {" << std::endl;
        //outs << "\tusing namespace ::brook::desc;" << std::endl;
        int techNum = 0;
        // first output all the IL shader strings as char arrays.
        for ( CgTechniqueIter iter = allTechnique.begin();
              iter != allTechnique.end(); iter++, techNum++ )
        {
            int passNum = 0;
            const CgTechnique &tech = (*iter);
            const CgPassTbl &allPass = tech.getPass();
            for ( CgPassIter piter = allPass.begin(); piter != allPass.end(); piter++, passNum++ )
            {
                const CgPass &pass = ( *piter );
                outs << "\tconst char "
                     << kernelVarName << "_desc_tech" << techNum << "_pass" << passNum << "[] = ";
                pass.genShader( outs, "\t\t" );
                outs << std::endl;
            }
        }
        techNum = 0;
        // now output all the technique initializers using the names of the above char arrays.
        outs << "\tstatic const brook::KernelDesc " << kernelVarName << "_desc = brook::KernelDesc()";
        for ( CgTechniqueIter iter = allTechnique.begin();
              iter != allTechnique.end(); iter++, techNum++ )
        {
            int passNum = 0;
            outs << std::endl;
            outs << "\t\t.technique( brook::Technique()" << std::endl;
            const CgTechnique &tech = (*iter);
            tech.genMetadata(outs, "\t\t\t");
            const CgPassTbl &allPass = tech.getPass();
            for ( CgPassIter piter = allPass.begin(); piter != allPass.end(); piter++, passNum++ )
            {
                const CgPass &pass = (*piter);
                outs << "\t\t\t.pass( brook::Pass( \"";
                outs << kernelVarName << "_desc_tech" << techNum << "_pass" << passNum;
                outs << " \" )"
                     << std::endl;
                
                outs << "\t\t\t\t"
                     << ".image(" << kernelVarName << "_desc_tech" << techNum << "_pass" << passNum << ")"
                     << std::endl;
                
                pass.genMetadata(outs, "\t\t\t\t");
                if(cgFunc->getShaderType() == ComputeShader)
                {
                    outs << "\t\t\t\t"
                         << ".computeShader()" << std::endl;

                }
                outs << "\t\t\t)" << std::endl;
            }
            outs << "\t\t)";
        }
        outs << ";" << std::endl
             << "\tstatic const void* " << kernelVarName
             << " = &" << kernelVarName << "_desc;" << std::endl
             << "}" << std::endl;
    }
}

//return the number of error
int
CgKernel::genPass(CgPass &pass, int reduceFactor, bool outaddr, bool inaddr, int passNum, OutputStreamType outType)
{
    LOG(LOG_FN_ENTRY, "CgKernel::genPass(%p, %d, %d, %d, %d)\n", &pass, reduceFactor, outaddr, inaddr, passNum);

    return 1;
}

//return the number of error
int
CgKernel::genTechnique(CgTechnique &tech)
{
    LOG(LOG_FN_ENTRY, "CgKernel::genTechnique(%p)\n", &tech);

    return 1;
}

// ----------------------------------------------------------------------------
