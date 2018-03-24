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

#include <math.h>
#include "parameter.hpp"
#include "positionaware.hpp"
#include "convert.hpp"
#include <fstream>
#include "astmodifier.hpp"
#include "astdeclaration.hpp"
#include "astroot.hpp"

namespace ILFormat
{
#include "ILFormat.h"
}

//#define Efloat 2.71828182845904523536f
#define  PI  3.14159265358979323846f

#include "iltranscriber.hpp"
#include "parameter.hpp"

PARAMETER(p_dumpsymbols, "dumpsymbols");
PARAMETER(p_nolines,     "nolinenos");
PARAMETER(p_nocomments,  "nocomments");
PARAMETER(p_cal, "cal" );

#ifndef DISABLE_DUMPSYMBOLS
#define DUMPSYMBOLS if(!p_dumpsymbols){}else theDumpSymbolsStream()
#else
#define DUMPSYMBOLS if(true){}else theDumpSymbolsStream()
#endif




static std::ostream& theDumpSymbolsStream()
{
  DEBUG_CHECK(p_dumpsymbols)(p_dumpsymbols);
  static std::ofstream file(p_dumpsymbols.ValueStr());
  return file;
}


ILCommentWrapper::ILCommentWrapper(ILTranscriber& ts) : mTranscriber(ts)
{
  os << ' ';
}

ILCommentWrapper::~ILCommentWrapper()
{
  mTranscriber.Comment(os.str());
}

bool ILCommentWrapper::IsDisabled()
{
  return p_nocomments;
}
void 
ILTranscriber::SetupConstRegisters()
{
  DclLiteral(ConstIntZero(),0, RegDT::Invalid);
  //make sure these get in the table
  ConstIntOne();
  ConstIntNegOne();
  ConstFloatExpFracBitMask();
  ConstFloatExpBitMask();
  ConstFloatSignMask();
  ConstFloatLog2();
  ConstFloatLn2();
  ConstFloatPi();
  ConstFloatHalfPi();
  ConstIntThree();
  ConstIntTwo();
}

//Fixed allocated registers
static unsigned int
calc_log2()
{
    static float f = log10f(2.0);
    return *(unsigned int*)&f;
}

static unsigned int
calc_ln2()
{
    static float f = logf(2.0);
    return *(unsigned int*)&f;
}

static unsigned int
calc_fone()
{
    static float f = 1.0;
    return *(unsigned int*)&f;
}

static unsigned int
calc_pi()
{
    static float f = PI;
    return *(unsigned int*)&f;
}

static unsigned int
calc_halfpi()
{
    static float f = PI/2;
    return *(unsigned int*)&f;
}

ILRegister
ILTranscriber::ConstIntZero		(RegDT::RegDT dt, int slots)
{ return ILRegister(0, ILSpec::IL_REGTYPE_LITERAL ,dt, slots); }

ILRegister
ILTranscriber::ConstIntOne		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0x00000001, dt,slots); }

ILRegister
ILTranscriber::ConstIntThree		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0x00000003, dt,slots); }

ILRegister
ILTranscriber::ConstIntTwo		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0x00000002, dt,slots); }

ILRegister
ILTranscriber::ConstIntNegOne		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0xFFFFFFFF, dt,slots); } 

ILRegister
ILTranscriber::ConstIntThirtyOne	(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0x0000001F, dt,slots); }

ILRegister
ILTranscriber::ConstFloatExpFracBitMask	(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0x7FFFFFFF, dt,slots); } 

ILRegister
ILTranscriber::ConstFloatExpBitMask	(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0x7F800000, dt,slots); } 

ILRegister
ILTranscriber::ConstFloatSignMask	(RegDT::RegDT dt, int slots)
{ return LookupLiteral(0x80000000, dt,slots); } 

ILRegister
ILTranscriber::ConstFloatLog2		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(calc_log2(),dt,slots); }

ILRegister
ILTranscriber::ConstFloatLn2		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(calc_ln2(), dt,slots); }

ILRegister
ILTranscriber::ConstFloatOne		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(calc_fone(),dt,slots); }

ILRegister
ILTranscriber::ConstFloatPi		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(calc_pi(),  dt,slots); }

ILRegister
ILTranscriber::ConstFloatHalfPi		(RegDT::RegDT dt, int slots)
{ return LookupLiteral(calc_halfpi(), dt,slots); }

ILRegister 
ILTranscriber::Scratch()
{ 
  return ILRegister(0,ILSpec::IL_REGTYPE_TEMP);
}

ILRegister 
ILTranscriber::AllocTempRegister()
{
  if(mAllocMode==ALLOC_NORMAL)
  {
    //add 10 to make room for cost values
    return ILRegister(++mAllocCntrs[ILSpec::IL_REGTYPE_TEMP]+10, ILSpec::IL_REGTYPE_TEMP);
  }
  else if(mAllocMode==ALLOC_PERSISTENT || mAllocMode==ALLOC_GLOBAL)
  {
    ILRegister tmp;
    if(mAllocMode==ALLOC_PERSISTENT)
    {
      tmp=ILRegister(0, ILSpec::IL_REGTYPE_PERSIST);
    }
    else if(mAllocMode==ALLOC_GLOBAL)
    {
      if(mShaderDescription.IsValid())
      {
        mShaderDescription->UsesGlobalStorage = true;
      }
      tmp=ILRegister(0, ILSpec::IL_REGTYPE_GLOBAL);
    }
    tmp.SetBase(ConstIntZero(RegDT::UInt, 1));
    tmp.SetOffset(mAllocCntrs[mAllocMode]++);
    return tmp;
  }
  else
  {
    ERROR_UNIMPLEMENTED()(mAllocMode);
    return -1;
  }
}
int
ILTranscriber::LookupRegister( BasicILRegister r)
{
  for(LiteralMap::const_iterator i = mLiterals.begin(); i != mLiterals.end(); ++i){
    if((*i).second.Number() == r.Number()) {
      return (*i).first;
    }
  }
  return -99;
}


ILRegister ILTranscriber::LookupLiteral( long long unsigned int x, RegDT::RegDT dt /*= RegDT::Int*/, int slots /*= 4*/)
{
  ILRegister tmp;
  LiteralMap::const_iterator i = mLiterals.find(*(long long unsigned int *)(&x));
  if(i!=mLiterals.end()) 
    tmp = i->second;
  else
  {
      if(dt == RegDT::Double)
      {
        tmp = ILRegister(++mAllocCntrs[ILSpec::IL_REGTYPE_LITERAL], ILSpec::IL_REGTYPE_LITERAL);
        commentLineno();
        ILCOMMENT(this) << "literal double=" << (*(double *)&x) << " long long int=" <<*(long long unsigned int *)&(x);
        DclLiteral(tmp, x, dt);
        mLiterals[*(long long unsigned int *)(&x)]=tmp;
      }
      else
      {
        tmp = ILRegister(++mAllocCntrs[ILSpec::IL_REGTYPE_LITERAL], ILSpec::IL_REGTYPE_LITERAL);
        commentLineno();
        ILCOMMENT(this) << "literal float=" << (*(float *)&x) << " int=" << *(long long unsigned int *)(&x);
        DclLiteral(tmp, x, dt);
        mLiterals[*(long long unsigned int *)&(x)]=tmp;
      }
  }
  tmp.SetDT(dt);
  tmp.SetNumSlots(slots);
  return tmp;
}

ILTranscriber::ILTranscriber( std::ostream& _os ) 
  : os(_os)
  , mCurrentContext(CTX_GLOBAL)
  , mAllocMode(ALLOC_NORMAL)
  , mHexWordCount(0)
  , mShaderDescription( new ShaderDescription() )
{
  memset(mAllocCntrs,0,sizeof(mAllocCntrs));
  mLiterals[0] = ConstIntZero();
}

void 
ILTranscriber::printHex( unsigned int x )
{
  static const char hxch[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  //Recursive version
//  if( (x & ~0xF) != 0 ) printHex(x >> 4);
//  os << hxch[x & 0xF];

  //Static version
  os << hxch[x>>28 & 0xF];
  os << hxch[x>>24 & 0xF];
  os << hxch[x>>20 & 0xF];
  os << hxch[x>>16 & 0xF];
  os << hxch[x>>12 & 0xF];
  os << hxch[x>>8  & 0xF];
  os << hxch[x>>4  & 0xF];
  os << hxch[x>>0  & 0xF];
}

void 
ILTranscriber::SetupExitShader()
{
  ILCOMMENT(this) << "Funcion: __write_output (Special)";
  BeginFunction(EXIT_SHADER_FUNC);
  for(SemanticMapT::const_iterator i = mOutputSemantics.begin(); i!=mOutputSemantics.end(); ++i)
  {
    ILCOMMENT(this) << "shader output: " << i->GetName();
    Move(i->first,i->second);
  }
  EndFunction();
}

void 
ILTranscriber::SetupSemantics()
{
  for(SemanticMapT::const_iterator i = mInputSemantics.begin(); i!=mInputSemantics.end(); ++i)
  {
    ILCOMMENT(this) << "shader input: " << i->GetName();
    Move(i->second,i->first);
  }
}

ILRegister 
ILTranscriber::AllocArrayStructure(int size, ILSpec::ILRegType type, int regNum)
{ 
  if(mAllocMode==ALLOC_NORMAL)
  {
    if(regNum<0) regNum = mAllocCntrs[type]++;
    ILRegister rg(regNum, type);
    rg.SetOffset(0);
    rg.SetBase(ConstIntZero(RegDT::UInt, 1));
    return rg;
  }
  else if(mAllocMode==ALLOC_PERSISTENT || mAllocMode==ALLOC_GLOBAL)
  {
    //in persistent/global all registers are allocated from array types
    ILRegister tmp = AllocTempRegister(); 
    mAllocCntrs[mAllocMode] += size-1;
    return tmp;
  }
  else
  {
    ERROR_UNIMPLEMENTED()(mAllocMode);
    return -1;
  }
}

// if requested_reg == -1 assign a number, otherwise use the requested
// number
int ILTranscriber::AllocResource( const ConstStringPtr& name,
                                  ILSpec::ILPixTexUsage,
                                  int requested_reg)
{
  if (requested_reg == -1){
    // if we are not using dx9 :register(sampler id) values
    // just use the next number 
    int dx9_samplers = AST::Root::TheCurrentTreeRoot()->dx9_samplers;  
    if (dx9_samplers == 0) {
      /// no app requested value
      requested_reg = mAllocCntrs[CNTR_RESOURCE]++;
    } else {
      // otherwize scan for a free number
      for(int i = 0; i< 16; i++){
        if ((dx9_samplers & (1 << i)) == 0){
          AST::Root::TheCurrentTreeRoot()->RecordForcedSampler(i);  
          dx9_forced_samplers[mAllocCntrs[CNTR_RESOURCE]++] = i;
          requested_reg = i;
          break;
        }
      }
    }
  }    
  return mPatcher.PatchResourceRegNum(
    AST::Modifier(name,
                  AST::Modifier::MOD_RESOURCE),
    requested_reg).number;
}

bool ILTranscriber::IsNormResource()
{
    return p_cal.IsEnabled() == false;
}

int ILTranscriber::AllocSampler( const ConstStringPtr& name )
{
  return mPatcher.PatchResourceRegNum(AST::Modifier(name,AST::Modifier::MOD_RESOURCE),mAllocCntrs[CNTR_SAMPLER]++).number;
}

void ILTranscriber::SetupConstBuffers()
{
    // constant buffer numbers can get set in 3 ways
	// - application picked a number placing it in a semantic
	// - we are comparing with ms -hlsl and we want their number
	// - we are going to assign the number based on use
  for(ConstBuffs::const_iterator cb = mConstBuffs.begin(); cb!=mConstBuffs.end(); ++cb)
  {
    ShaderDescription::ConstantBufferDesc cbdesc;
    
	
    int regNum = mPatcher.PatchResourceRegNum( AST::Modifier(cb->first,AST::Modifier::MOD_CONSTBUFF)
                                             , mAllocCntrs[ILSpec::IL_REGTYPE_CONST_BUFF]).number;

	if (mAllocCntrs[ILSpec::IL_REGTYPE_CONST_BUFF] <= regNum) {
      mAllocCntrs[ILSpec::IL_REGTYPE_CONST_BUFF] = regNum + 1;
	}
    int neededRegs = 0;
    int leftoverSlots = 0;
    for(ConstBuff::const_iterator var = cb->second.begin(); var!=cb->second.end(); ++var)
    {

      ShaderDescription::ConstantBufferFieldDesc fdesc;
      fdesc.Variable = var->first;
      fdesc.ByteOffset = -1;

      for(std::vector<ILRegister>::const_iterator rg = var->second.begin(); rg!=var->second.end(); ++rg)
      {

        //this implements packing

        int neededSlots = rg->GetNumSlots();

        //start a new register if needed
        if(neededSlots > leftoverSlots)
        { 
          neededRegs++;
          leftoverSlots= 4;
        }

        if(fdesc.ByteOffset == -1)//first time
          fdesc.ByteOffset = (neededRegs-1)*16 + (4-leftoverSlots)*4;

        leftoverSlots -= neededSlots;



        //Calculate location of this data in CB
        ILRegister cbreg(regNum,ILSpec::IL_REGTYPE_CONST_BUFF);
        cbreg.SetBase(ConstIntZero(RegDT::Int, 1));
        cbreg.SetOffset(neededRegs-1);
        char swiz[] = "____";
        // 4-leftoverSlots-neededSlots == 0 if we started a new register
        //  otherwise it offsets us into the swizzle
        memcpy(swiz, ILRegister::DEFAULT_SWIZZLE+4-leftoverSlots-neededSlots , neededSlots);
        cbreg.SetSwizzleBytes(swiz);

        //enqueue the move command
        mInputSemantics.push_back(SemanticPairT(cbreg, *rg, var->first.Name));

        DUMPSYMBOLS << var->first.Name << ", " << cbreg.BasicILRegister::ToString() << '[' << cbreg.GetOffset() << ']' << cbreg.SwizzleString(false) << std::endl;    
      }


      fdesc.ByteSize = (neededRegs-1)*16 + (4-leftoverSlots)*4 - fdesc.ByteOffset;
      cbdesc.Fields.push_back(fdesc);
    }

    if(neededRegs>0)
    {
      ILRegister tmp = AllocArrayStructure(neededRegs, ILSpec::IL_REGTYPE_CONST_BUFF, regNum);
      DUMPSYMBOLS << "cbuffer " << cb->first << ", " << tmp.BasicILRegister::ToString() << std::endl;

      if(GetShaderDescription().IsValid())
      {
        cbdesc.Name = cb->first;
        cbdesc.BufferNumber = regNum;
        GetShaderDescription()->ConstantBuffers.push_back(cbdesc);
      }
    }
  }
}

void ILTranscriber::SourceWithConstBuff( const ShaderDescription::VariableDesc& var, const std::vector<ILRegister>& regs )
{
  if(mAllocMode!=ALLOC_NORMAL) return;
  DEBUG_CHECK(mCurrentConstBuff!=0);
  mCurrentConstBuff->push_back(ConstBuffEntry(var,regs));

}

#if !defined(DISABLE_ILCOMMENT) && !defined(DISABLE_LINENOS)
void ILTranscriber::commentLineno()
{
  if(p_nolines) return;
  static PositionAware lastPos;
  if(lastPos != ::GetPosition())
  {
    lastPos = ::GetPosition();
    ILCOMMENT(this) << lastPos;
  }
}
#endif

static inline void _fillRegisterDesc(ShaderDescription::RegisterDesc& desc, const ILRegister& reg)
{
  desc.RegisterNumber = reg.Number();
  memset(desc.RegisterMask,0,sizeof(desc.RegisterMask));
  for(int i=0; i<4; ++i)
  {
    switch(reg.SwizzleBytes()[i])
    {
    case ILRegister::SLOT0: desc.RegisterMask[0]=true; break;
    case ILRegister::SLOT1: desc.RegisterMask[1]=true; break;
    case ILRegister::SLOT2: desc.RegisterMask[2]=true; break;
    case ILRegister::SLOT3: desc.RegisterMask[3]=true; break;
    }
  }
}

void ILTranscriber::AddOutputSemantic( const AST::Modifier& sem, ILRegister reg )
{
  InputPatcher::Override ovrd = mPatcher.PatchOutputRegNum(sem,(int)mOutputSemantics.size());
  ILRegister outreg(ovrd.number ,ILSpec::IL_REGTYPE_OUTPUT, reg.GetDT(), reg.GetNumSlots());
  outreg.SetSwizzleBytes(ovrd.slots);
  ILCOMMENT(this) << "shader output: " << sem;
  DclOutput(sem.LookupSemanticUsage(),outreg);
  mOutputSemantics.push_back(SemanticPairT(outreg,reg,sem));

  if(GetShaderDescription().IsValid())
  {
    DEBUG_CHECK(GetShaderDescription()->Outputs.size()>0);
    ShaderDescription::RegisterDesc desc;
    _fillRegisterDesc(desc,outreg);
    GetShaderDescription()->Outputs.back().Registers.push_back(desc);
  }
}

void
ILTranscriber::AddInputSemantic( const AST::Modifier& sem, ILSpec::ILInterpMode intrp, ILRegister reg )
{
    InputPatcher::Override ovrd = mPatcher.PatchInputRegNum(sem,(int)mInputSemantics.size());
    ILSpec::ILRegType input_reg = sem.LookupInputSemanticRegister();
    int input_num = ( input_reg == ILSpec::IL_REGTYPE_INPUT)? ovrd.number: 0; 
    ILRegister ir(input_num, input_reg, reg.GetDT(), reg.GetNumSlots());
    ir.SetSwizzleBytes(ovrd.slots);
    if ( ILSpec::IL_REGTYPE_WINCOORD == input_reg || input_reg == ILSpec::IL_REGTYPE_INPUT )
    {
        ILCOMMENT(this) << "shader input: " << sem;
        DclInput( sem.LookupSemanticUsage(), intrp, ir );
    }
  
    mInputSemantics.push_back(SemanticPairT(ir,reg,sem));

    if( GetShaderDescription().IsValid() &&
        input_reg == ILSpec::IL_REGTYPE_INPUT )
    {
        DEBUG_CHECK(GetShaderDescription()->Inputs.size()>0);
        ShaderDescription::RegisterDesc desc;
        _fillRegisterDesc(desc,ir);
        GetShaderDescription()->Inputs.back().Registers.push_back(desc);
    }
}


void
ILTranscriber::SetupDx9ForcedSamplers(int required_samplers)
{
  if (required_samplers ==0) {
    return;
    
  }
  for (int i = 0; i < 16 ;i ++){
    if ((required_samplers & (1 <<i)) != 0){
      int slot = mAllocCntrs[CNTR_RESOURCE];
      dx9_forced_samplers[slot] = i;
      mAllocCntrs[CNTR_RESOURCE]++;
    }
  }
}


void
ILTranscriber::SetupThreadInfo(const AST::FunctionDeclarationPtr m_func)
{
  // For R7xx, NumThreads is one-dimension ,
  int num_thread = m_func->GetNumThreadsX();
  int lds_size = m_func->GetLdsSize();
  int lds_rel = m_func->GetLdsRel();
  int lds_abs = m_func->GetLdsAbs();
  
  if ( num_thread ) {
    op_with_int("dcl_num_thread_per_group", ILSpec::IL_OP_DCL_NUM_THREAD_PER_GROUP, num_thread);
  }
  if ( lds_size ) {
    DclLdsSizeOrMode(ILSpec::IL_OP_DCL_LDS_SIZE_PER_THREAD, lds_size, (ILSpec::IL_LDS_SHARING_MODE) 0 );
  }
  if ( lds_abs ) {
    DclLdsSizeOrMode(ILSpec::IL_OP_DCL_LDS_SHARING_MODE, 0, ILSpec::IL_LDS_SHARING_MODE_ABSOLUTE );    
  }
  if ( lds_rel ) {
    DclLdsSizeOrMode(ILSpec::IL_OP_DCL_LDS_SHARING_MODE, 0, ILSpec::IL_LDS_SHARING_MODE_RELATIVE );    
  }  
  
}

void
ILTranscriber::ReadLds(ILRegister d, ILRegister l, int do_fft)
{
  if ( do_fft ) {
    int control = 1<<2; // neighbor exchange is bit 18
    op_1i_1o("lds_read_vec_neighborExch ", ILSpec::IL_OP_LDS_READ_VEC, d, l, control);
  } else {
    op_1i_1o("lds_read_vec ", ILSpec::IL_OP_LDS_READ_VEC, d, l, 0);
  }
  
}

void
ILTranscriber::WriteLds(ILRegister d, ILRegister l, int offset)
{

  offset = (offset<0)? 0: offset;
  int control = offset<<2; // offset starts at bit 18
  std::string s = "lds_write_vec";
  char extra[128];

  if (offset > 0 ) {
    sprintf(extra, "_lOffset(%d)", offset);
    s += extra;
  }
  
  op_1i_1o(s.c_str(), ILSpec::IL_OP_LDS_WRITE_VEC, d, l, control);
  
}

