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

#include "cgKernelIL.h"
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

//return the number of error
int
CgKernelIL::genPass(CgPass &pass, int reduceFactor, bool outaddr, bool inaddr, int passNum, OutputStreamType outType)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genPass(%p, %d, %d, %d, %d)\n", &pass, reduceFactor, outaddr, inaddr, passNum);
    //temporary
    //std::ostream &outs = std::cout;
    std::ostringstream outs;

    cgFunc->setCurrentMain(true);

    genPassOpen(outs);
    //outs << "shader: " << cgFunc->getFuncName() << std::endl;

    genPassMacro(outs);
    cgFunc->genPassTypedef(outs);

    cgFunc->genPassConstAndOutput(outs, "\t", pass, this, outType);

    if (cgFunc->getNumScatterArray() > 0)
        cgFunc->genPassGlobal(outs, "");

    cgFunc->genPassSubroutine(outs, this);

    //! Add thread info structure for compute shader
    if(cgFunc->getShaderType() == ComputeShader)
    {
        outs << "struct csThreadInfo " << std::endl 
             << "{ " << std::endl 
             << "    int tid : SV_RelThreadId; " << std::endl 
             << "    int atid : SV_AbsThreadId; " << std::endl 
             << "    int gid : SV_ThreadGroupId; " << std::endl 
             << "}" << std::endl; 
        //! Specify NumThreads, LocalDataShare, LocalDataShareRel
        outs << "[";
        
        //! NumThreads
        std::ostringstream groupSizeStr;
        GccAttrib *groupSize = cgFunc->getGroupSize(); 
        if(groupSize)
        {
            INumList *tempNumList = groupSize->ns;
            std::vector<int> values;
            while(tempNumList)
            {
                values.push_back(tempNumList->value);
                tempNumList = tempNumList->next;
            }
            
            //! Following code is suitable for ND groupSize
            //for(unsigned int i = 0; i < values.size() - 1; ++i)
            //{
            //    groupSizeStr << values[values.size() - i - 1];
            //    groupSizeStr << ", ";
            //}
            //groupSizeStr << values[0];
            //outs << "NumThreads(" << groupSizeStr.str().c_str() << ")";
            //if(cgFunc->isThereInstanceInGroup())
            //{
            //    outs << ", LocalDataShare(" << cgFunc->getLocalDataShareSize() / values[0] << "), LocalDataShareRel";
            //}

            //! For only 1D groupSize
            outs << "NumThreads(" << values[values.size() - 1] << ")";
            if(cgFunc->isThereInstanceInGroup())
            {
                int ldsStrideInFloats = (cgFunc->getLocalDataShareSize() / values[values.size() - 1]) * cgFunc->getSharedArrayTypeComponents();
                outs << ", LocalDataShare(" << ldsStrideInFloats << "), LocalDataShareRel";
            }
        }
        else
        {
            cgAssert(true, "Internal error: GroupSize missing");
        }
        outs << "]" << std::endl;
    }
    //genShaderEntry(outs, pass, target);
    // Return type of main function
    if(cgFunc->getNumScatterArray() > 0 && outType == SCATTER_OUTPUT_STREAM)
        outs << "void " << std::endl;
    else
        outs << "__output " << std::endl;

    // parameters for main function
    outs << "main (";
    cgFunc->genPassInputs(outs, "\t", pass, this);
    
    if(cgFunc->getShaderType() == ComputeShader)
    {
        outs << "csThreadInfo __threadInfo)" << std::endl;
    }
    else
    {
        outs << "float2 __vPos: SV_POSITION) : SV_TARGET" << std::endl;
    }
    outs << "{" << std::endl;

    //the parameters in the orig shader becomes local variables of main
    //gen such local variables
    if(cgFunc->isThereInstanceInGroup())
    {
        outs << "\tint __instanceInGroup = __threadInfo.tid;" << std::endl;
    }
    cgFunc->genPassLocal(outs, outaddr, inaddr, reduceFactor, "\t", outType);

    cgFunc->genPassIndexofAssign(outs, outaddr, inaddr, reduceFactor, "\t", pass.getFirstOutput(), pass.getLastOutput());
    cgFunc->genPassStreamfetch(outs, outaddr, inaddr, reduceFactor, "\t");

    //gen call to main
    //to do, reduction
    if (cgFunc->IsReduce() == false)
    {
        cgFunc->genPassCallmain(outs, "\t");
    }
    else
    {
        CgResourceParam *reduceout = cgFunc->getReduceoutStream();
        CgResourceParam *reducein = cgFunc->getReduceinStream();

        std::string index;
        if(!outaddr)
        {
            index = reducein->getIndexofName() + " + __it";
        }
        else
        {
            
            index = "__getGatherPosFrom3D(" + reducein->getIndexofName() + " + __it, "
                    + reducein->getGatherStreamShapeName() + ", " 
                    + reducein->getGatherBufferShapeName() + ")";
        }
        outs << "\tint4 __it = int4(0, 0, 0, 0);\n";
        reducein->genFetch(outs, outaddr, inaddr, reduceFactor, "\t\t\t\t\t", index);
        outs << std::endl << "\t"
             << reduceout->getName() << " = "
             << reducein->getName() << ";" << std::endl;

        if(outaddr)
        {
            outs << "\tfor(__it.z = 1; __it.z < __reductionFactor.z; ++__it.z)\n"
                 << "\t{\n";

            reducein->genFetch(outs, outaddr, inaddr, reduceFactor, "\t\t", index);
            cgFunc->genPassCallmain(outs, "\t\t");

            outs << std::endl
                 << "\t}\n\n";
        }

        outs << "\tfor(__it.z = 0; __it.z < __reductionFactor.z; ++__it.z)\n"
             << "\t{\n"
             << "\t    for(__it.y = 1; __it.y < __reductionFactor.y; ++__it.y)\n"
             << "\t    {\n";

        reducein->genFetch(outs, outaddr, inaddr, reduceFactor, "\t\t\t", index);
        cgFunc->genPassCallmain(outs, "\t\t\t");

        outs << std::endl
             << "\t    }\n"
             << "\t}\n\n";

        outs << "\tfor(__it.z = 0; __it.z < __reductionFactor.z; ++__it.z)\n"
             << "\t{\n"
             << "\t    for(__it.y = 0; __it.y < __reductionFactor.y; ++__it.y)\n"
             << "\t    {\n"
             << "\t        for(__it.x = 1; __it.x < __reductionFactor.x; ++__it.x)\n"
             << "\t        {\n";

        reducein->genFetch(outs, outaddr, inaddr, reduceFactor, "\t\t\t\t", index);
        cgFunc->genPassCallmain(outs, "\t\t\t\t");

        outs << std::endl
             << "\t        }\n"
             << "\t    }\n"
             << "\t}\n";
    }

    cgFunc->genPassStreamstore(outs, "\t", pass.getFirstOutput(), pass.getLastOutput(), outType);
    outs << std::endl;

    outs << "}" << std::endl;
    //outs << "end" << std::endl;
    genPassClose(outs);

    cgFunc->setCurrentMain(false);

    std::ostringstream shadername;
    shadername << std::string(globals.shaderOutputName) << "_" << cgFunc->getFuncName();
    if (passNum != 0)
        shadername << "_pass" << passNum;
    if (outaddr)
        shadername << "_addr";

    std::string nstr = shadername.str();
    char *shaderstring = genPassCompile(pass, nstr, outs.str().c_str(), cgFunc->getShaderType());

    if (shaderstring)
    {
        pass.setShader(shaderstring);
        free(shaderstring);
        return 0;
    }

    return 1;
}

//return the number of error
int
CgKernelIL::genTechnique(CgTechnique &tech)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genTechnique(%p)\n", &tech);
    int totOutput;
    int curOutput = 0;
    int passNum = 0;

    //tech.setReduceFactor(reduceFactor);

    bool inputAT = (cgFunc->getNumScatterArray() > 0) && tech.isOutputAT();
    CgResourceManager resourceManager;
    cgFunc->allocateResource(resourceManager,
        tech.getReduceFactor(),
        tech.isOutputAT(), inputAT);
    totOutput = resourceManager.getNextOutputId();

    int AnyPassFailed = 0;

    if(cgFunc->getNumScatterArray() > 0 && totOutput == 0)
    {
        CgPass pass(curOutput, curOutput + mrt);
        AnyPassFailed = genPass(pass, tech.getReduceFactor(), tech.isOutputAT(), inputAT, passNum ++, SCATTER_OUTPUT_STREAM);
        //! add here some message about failure
        tech.addPass(pass);
    }
    while (curOutput < totOutput)
    {
        OutputStreamType outType = ORDINARY_OUTPUT_STREAM;
        if(cgFunc->getNumScatterArray() > 0)
            outType = MIXED_OUTPUT_STREAM;
        CgPass pass(curOutput, curOutput + mrt);

        AnyPassFailed = genPass(pass, tech.getReduceFactor(), tech.isOutputAT(), inputAT, passNum ++, outType);
        //! add here some message about failure
        tech.addPass(pass);
        curOutput += mrt;
    }

    return AnyPassFailed;
}

void CgKernelIL::genPassMacro(std::ostream &shader)
{
    LOG(LOG_FN_ENTRY, " CgKernelIL::genPassMacro(%p)\n", &shader);
    //to work around the fact that amd-hlsl doesn't seem to have good support for overloading ...

    //_stype is only used for fetchStream, to reduce the difference between amdhlsl and hlsl
    shader << "#define _stype1   sampler2D" << std::endl;
    shader << "#define _stype2   sampler2D" << std::endl;
    shader << "#define _stype3   sampler2D" << std::endl;
    shader << "#define _stype4   sampler2D" << std::endl;

    //__sample1 is used for gatherstream, to reduce the difference between amdhlsl and hlsl
    shader << "#define __sample1 tex1Dlod" << std::endl;
    shader << "#define __sample2 tex2Dlod" << std::endl;
    shader << "#define __sample3 tex3Dlod" << std::endl;

    shader << "#define __fetch1 tex1Dlod" << std::endl;
    shader << "#define __fetch2 tex2Dlod" << std::endl;
    shader << "#define __fetch3 tex3Dlod" << std::endl;

    shader << "#define __FRAGMENTKILL discard\n";

    shader <<"#define __gather_float __fetch_float" << std::endl;
    shader <<"#define __gather_float2 __fetch_float2" << std::endl;
    shader <<"#define __gather_float3 __fetch_float3" << std::endl;
    shader <<"#define __gather_float4 __fetch_float4" << std::endl;

    shader <<"#define __gather_double __fetch_double" << std::endl;
    shader <<"#define __gather_double2 __fetch_double2" << std::endl;

    shader <<"#define __gather_int __fetch_int" << std::endl;
    shader <<"#define __gather_int2 __fetch_int2" << std::endl;
    shader <<"#define __gather_int3 __fetch_int3" << std::endl;
    shader <<"#define __gather_int4 __fetch_int4" << std::endl;

    shader <<"#define __gather_uint __fetch_int" << std::endl;
    shader <<"#define __gather_uint2 __fetch_int2" << std::endl;
    shader <<"#define __gather_uint3 __fetch_int3" << std::endl;
    shader <<"#define __gather_uint4 __fetch_int4" << std::endl;

    shader <<"#define __fetch_uint __fetch_int" << std::endl;
    shader <<"#define __fetch_uint2 __fetch_int2" << std::endl;
    shader <<"#define __fetch_uint3 __fetch_int3" << std::endl;
    shader <<"#define __fetch_uint4 __fetch_int4" << std::endl;

    shader <<"#define __fetch_char __fetch_int" << std::endl;
    shader <<"#define __fetch_char2 __fetch_int2" << std::endl;
    shader <<"#define __fetch_char3 __fetch_int3" << std::endl;
    shader <<"#define __fetch_char4 __fetch_int4" << std::endl;

    shader <<"#define __gather_char __fetch_char" << std::endl;
    shader <<"#define __gather_char2 __fetch_char2" << std::endl;
    shader <<"#define __gather_char3 __fetch_char3" << std::endl;
    shader <<"#define __gather_char4 __fetch_char4" << std::endl;

    shader <<"#define __gather_uchar __fetch_char" << std::endl;
    shader <<"#define __gather_uchar2 __fetch_char2" << std::endl;
    shader <<"#define __gather_uchar3 __fetch_char3" << std::endl;
    shader <<"#define __gather_uchar4 __fetch_char4" << std::endl;

    shader <<"#define __fetch_uchar __fetch_char" << std::endl;
    shader <<"#define __fetch_uchar2 __fetch_char2" << std::endl;
    shader <<"#define __fetch_uchar3 __fetch_char3" << std::endl;
    shader <<"#define __fetch_uchar4 __fetch_char4" << std::endl;

    shader <<"#define __fetch_short __fetch_int" << std::endl;
    shader <<"#define __fetch_short2 __fetch_int2" << std::endl;
    shader <<"#define __fetch_short3 __fetch_int3" << std::endl;
    shader <<"#define __fetch_short4 __fetch_int4" << std::endl;

    shader <<"#define __gather_short __fetch_short" << std::endl;
    shader <<"#define __gather_short2 __fetch_short2" << std::endl;
    shader <<"#define __gather_short3 __fetch_short3" << std::endl;
    shader <<"#define __gather_short4 __fetch_short4" << std::endl;

    shader <<"#define __gather_ushort __fetch_short" << std::endl;
    shader <<"#define __gather_ushort2 __fetch_short2" << std::endl;
    shader <<"#define __gather_ushort3 __fetch_short3" << std::endl;
    shader <<"#define __gather_ushort4 __fetch_short4" << std::endl;

    shader <<"#define __fetch_ushort __fetch_short" << std::endl;
    shader <<"#define __fetch_ushort2 __fetch_short2" << std::endl;
    shader <<"#define __fetch_ushort3 __fetch_short3" << std::endl;
    shader <<"#define __fetch_ushort4 __fetch_short4" << std::endl;

    //HLSL FE doesn't seem to be able to handle typedef
    //shader <<"#define uint unsigned int" << std::endl;
    //shader <<"#define uint2 unsigned int2" << std::endl;
    //shader <<"#define uint3 unsigned int3" << std::endl;
    //shader <<"#define uint4 unsigned int4" << std::endl;

    //relying on amd-hlsl to patch 0, e.g tex2Dlod(s, float i) => tex2Dlod(s, (i,0,0,0))
    shader << "float __fetch_float( sampler2D s, float i ) { return __fetch1(s,i).x; }" << std::endl;
    shader << "float __fetch_float( sampler2D s, float2 i ) { return __fetch2(s,i).x; }" << std::endl;
    shader << "float __fetch_float( sampler2D s, float3 i ) { return __fetch3(s,i).x; }" << std::endl;

    shader << "float2 __fetch_float2( sampler2D s, float i ) { return __fetch1(s,i).xy; }" << std::endl;
    shader << "float2 __fetch_float2( sampler2D s, float2 i ) { return __fetch2(s,i).xy; }" << std::endl;
    shader << "float2 __fetch_float2( sampler2D s, float3 i ) { return __fetch3(s,i).xy; }" << std::endl;

    shader << "float3 __fetch_float3( sampler2D s, float i ) { return __fetch1(s,i).xyz; }" << std::endl;
    shader << "float3 __fetch_float3( sampler2D s, float2 i ) { return __fetch2(s,i).xyz; }" << std::endl;
    shader << "float3 __fetch_float3( sampler2D s, float3 i ) { return __fetch3(s,i).xyz; }" << std::endl;

    shader << "float4 __fetch_float4( sampler2D s, float i ) { return __fetch1(s,i).xyzw; }" << std::endl;
    shader << "float4 __fetch_float4( sampler2D s, float2 i ) { return __fetch2(s,i).xyzw; }" << std::endl;
    shader << "float4 __fetch_float4( sampler2D s, float3 i ) { return __fetch3(s,i).xyzw; }" << std::endl;

    shader << "double __fetch_double( sampler2D s, float i )"
        "{ return asdouble(__fetch1(s,i).xy);}" << std::endl;
    shader << "double __fetch_double( sampler2D s, float2 i )"
        "{ return asdouble(__fetch2(s,i).xy);}" << std::endl;
    shader << "double __fetch_double( sampler2D s, float3 i )"
        "{ return asdouble(__fetch1(s,i).xy);}" << std::endl;

    shader << "double2 __fetch_double2( sampler2D s, float i )"
        "{ float4 f = __fetch1(s,i).xyzw; double2 d; d.x=asdouble(f.xy); d.y=asdouble(f.zw); return d;}"
        << std::endl;
    shader << "double2 __fetch_double2( sampler2D s, float2 i )"
        "{ float4 f = __fetch2(s,i).xyzw; double2 d; d.x=asdouble(f.xy); d.y=asdouble(f.zw); return d;}"
        << std::endl;
    shader << "double2 __fetch_double2( sampler2D s, float3 i )"
        "{ float4 f = __fetch2(s,i).xyzw; double2 d; d.x=asdouble(f.xy); d.y=asdouble(f.zw); return d;}"
        << std::endl;


    shader << "int __fetch_int( sampler2D s, float i )"
        "{ return asint(__fetch1(s,i).x);}" << std::endl;
    shader << "int __fetch_int( sampler2D s, float2 i )"
        "{ return asint(__fetch2(s,i).x);}" << std::endl;
    shader << "int __fetch_int( sampler2D s, float3 i )"
        "{ return asint(__fetch3(s,i).x);}" << std::endl;


    shader << "int2 __fetch_int2( sampler2D s, float i )"
        "{ float2 f2 = __fetch1(s,i).xy; int2 i2; i2.x = asint(f2.x); i2.y = asint(f2.y); return i2;}" << std::endl;
    shader << "int2 __fetch_int2( sampler2D s, float2 i )"
        "{ float2 f2 = __fetch2(s,i).xy; int2 i2; i2.x = asint(f2.x); i2.y = asint(f2.y); return i2;}" << std::endl;
    shader << "int2 __fetch_int2( sampler2D s, float3 i )"
        "{ float2 f2 = __fetch3(s,i).xy; int2 i2; i2.x = asint(f2.x); i2.y = asint(f2.y); return i2;}" << std::endl;

    shader << "int3 __fetch_int3( sampler2D s, float i )"
        "{ float3 f3 = __fetch1(s,i).xyz; int3 i3; i3.x = asint(f3.x); i3.y = asint(f3.y);"
        " i3.z = asint(f3.z); return i3;}" << std::endl;
    shader << "int3 __fetch_int3( sampler2D s, float2 i )"
        "{ float3 f3 = __fetch1(s,i).xyz; int3 i3; i3.x = asint(f3.x); i3.y = asint(f3.y);"
        " i3.z = asint(f3.z); return i3;}" << std::endl;
    shader << "int3 __fetch_int3( sampler2D s, float3 i )"
        "{ float3 f3 = __fetch1(s,i).xyz; int3 i3; i3.x = asint(f3.x); i3.y = asint(f3.y);"
        " i3.z = asint(f3.z); return i3;}" << std::endl;


    shader << "int4 __fetch_int4( sampler2D s, float i )"
        "{ float4 f4 = __fetch1(s,i).xyzw; int4 i4; i4.x = asint(f4.x); i4.y = asint(f4.y);"
        " i4.z = asint(f4.z);  i4.w = asint(f4.w); return i4;}" << std::endl;

    shader << "int4 __fetch_int4( sampler2D s, float2 i )"
        "{ float4 f4 = __fetch2(s,i).xyzw; int4 i4; i4.x = asint(f4.x); i4.y = asint(f4.y);"
        " i4.z = asint(f4.z);  i4.w = asint(f4.w); return i4;}" << std::endl;

    shader << "int4 __fetch_int4( sampler2D s, float3 i )"
        "{ float4 f4 = __fetch3(s,i).xyzw; int4 i4; i4.x = asint(f4.x); i4.y = asint(f4.y);"
        " i4.z = asint(f4.z);  i4.w = asint(f4.w); return i4;}" << std::endl;

    shader << "float __get2DHeight(float2 array2D) {return array2D.y;}\n"
           << "float __get2DWidth(float2 array2D) {return array2D.x;}\n"
           << "float __get3DDepth(float3 array3D) {return array3D.z;}\n"
           << "float __get3DHeight(float3 array3D) {return array3D.y;}\n"
           << "float __get3DWidth(float3 array3D) {return array3D.x;}\n" << std::endl;

    shader << "int __getScatterIndex(int4 scatterIndex, int4 scatterStream)\n"
           << "{\n"
           << "    return scatterIndex.z * scatterStream.x * scatterStream.y + \n"
           << "           scatterIndex.y * scatterStream.x + scatterIndex.x;\n"
           << "}\n" << std::endl;

    if(globals.enableGPUAddressTranslation)
    {
        shader << "int4 __getOutputIndex(float2 pos, int4 outStream, int4 outBuffer)\n"
               << "{\n"
	           << "    int4 outIndex = int4(0, 0, 0, 0);\n"
	           << "    int2 intPos = int2(pos.xy);\n"
	           << "    int index = intPos.y * outBuffer.x + intPos.x;\n"
	           << "    int height = index / outStream.x;\n"
	           << "    outIndex.x = index - height * outStream.x;\n"
	           << "    outIndex.z = height / outStream.y;\n"
	           << "    outIndex.y = height - outIndex.z * outStream.y;\n"
               << "\n"
	           << "    return outIndex;\n"
               << "}" << std::endl;
        shader << "int4 __getOutputIndex(int pos, int4 outStream, int4 outBuffer)\n"
               << "{\n"
	           << "    int4 outIndex = int4(0, 0, 0, 0);\n"
	           << "    int2 intPos = int2(pos, 1);\n"
	           << "    int index = intPos.x;\n"
	           << "    int height = index / outStream.x;\n"
	           << "    outIndex.x = index - height * outStream.x;\n"
	           << "    outIndex.z = height / outStream.y;\n"
	           << "    outIndex.y = height - outIndex.z * outStream.y;\n"
               << "\n"
	           << "    return outIndex;\n"
               << "}" << std::endl;
        shader << "float2 __getGatherPosFrom1D(int4 gatherIndex, int4 gatherStream, int4 gatherBuffer)\n"
               << "{\n"
	           << "    float2 gatherPos = float2(0.0f, 0.0f);\n"
               << "    int posY = 0;\n"
               << "    int index = 0;\n"
               << "    index = gatherIndex.x;\n"
               << "    posY = index / gatherBuffer.x;\n"
	           << "    gatherPos.x = float(index - posY * gatherBuffer.x) + 0.5f;\n"
	           << "    gatherPos.y = float(posY) + 0.5f;\n"
               << "\n"
	           << "    return gatherPos;\n"
               << "}" << std::endl;
        shader << "float2 __getGatherPosFrom3D(int4 gatherIndex, int4 gatherStream, int4 gatherBuffer)\n"
               << "{\n"
	           << "    float2 gatherPos = float2(0.0f, 0.0f);\n"
	           << "    int index = gatherIndex.z * gatherStream.x * gatherStream.y + \n"
		       << "		          gatherIndex.y * gatherStream.x + gatherIndex.x;\n"
               << "    int posY = index / gatherBuffer.x;\n"
	           << "    gatherPos.x = float(index - posY * gatherBuffer.x) + 0.5f;\n"
	           << "    gatherPos.y = float(posY) + 0.5f;\n"
               << "\n"
	           << "    return gatherPos;\n"
               << "}" << std::endl;
    }

}

void CgKernelIL::genPassClose(std::ostream &outs)
{
    LOG(LOG_FN_ENTRY, " CgKernelIL::genPassClose(%p)\n", &outs);
    //outs << "end" << std::endl;
}

void CgKernelIL::genPassOpen(std::ostream &outs)
{
    LOG(LOG_FN_ENTRY, " CgKernelIL::genPassOpen(%p)\n", &outs);
    //outs << "shader: " << cgFunc->getFuncName() << std::endl;
}

void
CgKernelIL::genConstDeclare(std::ostream &shader, std::string &name, const char* typeName, int id)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genConstDeclare(%p, %s, %s, %d)\n", &shader, name.c_str(), typeName, id);
    shader << typeName << " " << name;
}

void
CgKernelIL::genFetchStreamDeclare(std::ostream &outs,
                                   std::string &name,
                                   int rank,
                                   int id)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genFetchStreamDeclare(%p, %s, %d, %d)\n", &outs, name.c_str(), rank, id);
    outs << "sampler2D " << name;
}

void
CgKernelIL::genGatherStreamDeclare(std::ostream &outs,
                                   std::string &name,
                                   int rank,
                                   int component,
                                   int id)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genGatherStreamDeclare(%p, %s, %d, %d, %d)\n", &outs, name.c_str(), rank, component, id);
    outs << "uniform sampler2D " << name;

    //! For structures only
    if (component > 0)
        outs << "[" << component << "]";
}

void
CgKernelIL::genGatherStreamParameter(std::ostream &outs,
                                   std::string &name,
                                   int rank,
                                   int component)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genGatherStreamParameter(%p, %s, %d, %d)\n", &outs, name.c_str(), rank, component);
    outs << "sampler2D " << name;

    //! For structures only
    if (component > 0)
        outs << "[" << component << "]";
}

int
CgKernelIL::normGatherRank(int v)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::normGatherRank(%d)\n", v);
    if (v < 2 || v >= 3)
        v = 2;

    return v;
}

#if 0

char *
CgKernelIL::genPassCompile(std::string &shadername, const char *hlslstring)
{
    LOG(LOG_FN_ENTRY, "CgKernelIL::genPassCompile(%s, %s)\n", shadername.c_str(), hlslstring);
#define KeepComment 0
#define AddComment(c)  \
    { \
    if (KeepComment == 1) \
        ilstring[pos++] = c; \
    }

    const char *argv[] = { "HLFrontendExe.exe", "-ps", "-txt", "-cal", NULL, NULL, NULL};
    const int kInputIndex = 4;
    const int kOutputIndex = 5;

    std::string inputfname = shadername + ".amdhlsl";
    std::string outputfname = shadername + ".il";

    FILE *fp = fopen(inputfname.c_str(), "wb+");
    if (fp == NULL)
    {
        CGEXIT("***Unable to open file %s\n", inputfname.c_str());
        return NULL;
    }
    fwrite(hlslstring, sizeof(char), strlen(hlslstring), fp);
    fclose(fp);

    argv[kInputIndex] = strdup(("-input=" + inputfname).c_str());
    argv[kOutputIndex] = strdup(("-output=" + outputfname).c_str());

    char *errcode = Subprocess_Run(argv, NULL);

    if (!globals.keepFiles)
        remove(inputfname.c_str());

    free((void *)(argv[kInputIndex]));
    free((void *)(argv[kOutputIndex]));

    if (errcode == NULL)
    {
        cgErrorMsg("%s resulted in an error, skipping CAL backend\n", argv[0]);

        remove(outputfname.c_str());
        return NULL;
    }

    fp = fopen(outputfname.c_str(), "rt");
    if (fp == NULL)
    {
        cgErrorMsg("Unable to open compiler output file %s\n", outputfname.c_str());
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long flen = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    char *ilstring = (char *) malloc(flen + 1);
    char *buffer = (char *) malloc(flen + 1);

    int pos = 0;
    int cpos = 0;
    bool incomment;
    char c;
    ilstring[0] = '\0';

    #define STARTER "il_ps_2_0"
    while (fgets(buffer, flen, fp) != NULL && strncmp(STARTER, buffer, strlen(STARTER)) != 0) ;

    incomment = false;
    while (1)
    {
        cpos = 0;
        while ( (c = buffer[cpos++]) != '\0')
        {
            // Remove comment lines
            if (incomment)
            {
                AddComment(c);
                if (c == '\n')
                {
                    incomment = false;
                }
            }
            else if (c == ';')
            {
                incomment = true;
                AddComment(';');
            }
            else
            {
                ilstring[pos++] = c;
            }
        }
        if (fgets(buffer, flen, fp) == NULL)
            break;
    }

    ilstring[pos] = '\0';

    free(buffer);
    fclose(fp);

    if (!globals.keepFiles)
        remove(outputfname.c_str());

    return ilstring;

}
#endif
// ----------------------------------------------------------------------------
