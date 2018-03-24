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

#include "iltokentranscriber.hpp"

#define TOKEN_SIZE ((int)sizeof(TOKEN_TYPE))

namespace ILFormat
{
#include "ILFormat.h"
}

//File local helper functions
namespace 
{
  inline TOKEN_TYPE mkOpToken(TOKEN_TYPE code, TOKEN_TYPE controls = 0, bool hasSecMod = false, bool hasPriMod = false)
  {
    DEBUG_CHECK(code!=0 && code < 1<<16)(code);
    DEBUG_CHECK(controls < 1<<14)(controls);
    ILFormat::IL_OpCode oc;
    oc.bits.code = code;
    oc.bits.controls = controls;
    oc.bits.sec_modifier_present = hasSecMod;
    oc.bits.pri_modifier_present = hasPriMod;
    return oc.u32All;
  }

  ILSpec::ILComponentSelect swizzleChToCompSel(char ch)
  {
    switch(ch)
    {
    case BasicILRegister::SLOT0:       return ILSpec::IL_COMPSEL_X_R;
    case BasicILRegister::SLOT1:       return ILSpec::IL_COMPSEL_Y_G;
    case BasicILRegister::SLOT2:       return ILSpec::IL_COMPSEL_Z_B;
    case BasicILRegister::SLOT3:       return ILSpec::IL_COMPSEL_W_A;
    case BasicILRegister::SLOTNOTUSED: return ILSpec::IL_COMPSEL_0;
    default:
      DIE("Invalid swizzle char")(ch);
      return ILSpec::IL_COMPSEL_0;
    }
  }

  TOKEN_TYPE defaultElementFormat()
  {
    ILFormat::IL_ElementFormat el;
    el.bits.fmt_x = 
      el.bits.fmt_y = 
        el.bits.fmt_z = 
          el.bits.fmt_w = ILSpec::IL_ELEMENTFORMAT_FLOAT;
    el.bitfield.reserved = 0;
    return el.u32All;
  }
  
}


void ILTokenTranscriber::op_3i_1o( const char* /*txt*/ , ILSpec::ILOpCode bin, const ILRegister& d, const ILRegister& a, const ILRegister& b, const ILRegister& c )
{
  commentLineno();
  writeToken(mkOpToken(bin));
  writeDstToken(d);
  writeSrcToken(a);
  writeSrcToken(b);
  writeSrcToken(c);
}


void 
ILTokenTranscriber::op_2i_1o( const char* /*txt*/ , ILSpec::ILOpCode bin, const ILRegister& d, const ILRegister& l, const ILRegister& r, int control )
{
  commentLineno();
  writeToken(mkOpToken(bin, control));
  writeDstToken(d);
  writeSrcToken(l);
  writeSrcToken(r);
}

void 
ILTokenTranscriber::op_1i_1o( const char* /*txt*/ , ILSpec::ILOpCode bin ,ILRegister d, ILRegister l, int control )
{
  commentLineno();
  writeToken(mkOpToken(bin,control));
  writeDstToken(d);
  writeSrcToken(l);
}

void
ILTokenTranscriber::op_resource( const char* /*txt*/ ,
                                 ILSpec::ILOpCode bin ,
                                 ILRegister d,
                                 int resource,
                                 int sampler,
                                 TextureLoadFlags::TextureLoadFlags flags,
                                 unsigned int offset,
                                 ILRegister a0, ILRegister a1, ILRegister a2 )
{
  using namespace TextureLoadFlags;
  using namespace ILFormat;
  commentLineno();
  unsigned int offsets = 0;
  TOKEN_TYPE op = mkOpToken(bin);
  switch(flags)
  {
  case RESOURCE:
    ((IL_LoadOpCode&)op).bits.controls_id     = resource;
	if( offset){
     ((IL_SampleOpCode&)op).bits.addroff_present = 1;
	}
    break;
  case RESOURCE_SAMPLER:
  case RESOURCE_SAMPLER_ADDROFF:
    ((IL_SampleOpCode&)op).bits.resource_id     = resource;
    ((IL_SampleOpCode&)op).bits.sampler_id      = sampler;
    if (offset){
      ((IL_SampleOpCode&)op).bits.addroff_present = 1;
	} else {
      ((IL_SampleOpCode&)op).bits.addroff_present = 0;
    }
    
    break;
  }
  writeToken(op);
  if(offset){
    unsigned int n0 = offset & 0x7f;
	unsigned int n1 = (offset>>8) & 0x7f;
	unsigned int n2 = (offset>> 16) & 0x7f;
	offset = (n0<<1) | (n1 << 9) | (n2 << 17);
    writeToken(offset);
  }
  writeDstToken(d);
  writeSrcToken(a0);
  if(a1) writeSrcToken(a1);
  if(a2) writeSrcToken(a2);
}

void 
ILTokenTranscriber::op_1i_0o( const char* /*txt*/ , ILSpec::ILOpCode bin ,ILRegister d)
{
  commentLineno();
  writeToken(mkOpToken(bin));
  writeSrcToken(d);
}

void 
ILTokenTranscriber::op_0i_0o( const char* /*txt*/ , ILSpec::ILOpCode bin, int control )
{
  commentLineno();
  writeToken(mkOpToken(bin, control));
}

void 
ILTokenTranscriber::op_il_hdr( const char* /*txt*/, ILSpec::IL_Shader_Type t )
{
  ILFormat::IL_Lang lt;
  lt.bits.client_type = ILSpec::IL_LANG_GENERIC;
  lt.bits.reserved = 0;
  writeToken(lt.u32All);

  ILFormat::IL_Version vt;
  vt.bits.minor_version = 0;
  vt.bits.major_version = 2;
  vt.bits.shader_type   = t;
  vt.bits.multipass     = 0;
  vt.bits.realtime      = 0;
  vt.bits.reserved      = 0;
  writeToken(vt.u32All);
}

void 
ILTokenTranscriber::op_with_int( const char* /*txt*/, ILSpec::ILOpCode bin, unsigned int x )
{
  commentLineno();
  writeToken(mkOpToken(bin));
  writeToken(x);
}

void 
ILTokenTranscriber::op_call( const char* /*txt*/, 
                             ILSpec::ILOpCode bin, 
                             ConstStringPtr /*txt*/, 
                             unsigned int x )
{
  commentLineno();
  writeToken(mkOpToken(bin));
  writeToken(x);
}

void
ILTokenTranscriber::DclLdsSizeOrMode(ILSpec::ILOpCode bin, int n, ILSpec::IL_LDS_SHARING_MODE mode)
{
  int control = (bin == ILSpec::IL_OP_DCL_LDS_SIZE_PER_THREAD)? n: (int)mode;
  commentLineno();
  writeToken(mkOpToken(bin, control));
}

void 
ILTokenTranscriber::DclOutput( ILSpec::ILImportUsage usg, ILRegister reg )
{
  commentLineno();
  writeToken(mkOpToken(ILSpec::IL_DCL_OUTPUT, usg));
  writeDstToken(reg);
}

void 
ILTokenTranscriber::DclInput( ILSpec::ILImportUsage usg, ILSpec::ILInterpMode interp, ILRegister reg )
{
  commentLineno();
  ILFormat::IL_DclInput it;
  it.bits.code                 = ILSpec::IL_DCL_INPUT;
  it.bits.usage                = usg;
  it.bits.interp_mode          = interp;
  it.bits.controls             = 0;
  it.bits.sec_modifier_present = 0;
  it.bits.pri_modifier_present = 0;
  writeToken(it.u32All);
  writeDstToken(reg);
}

void 
ILTokenTranscriber::DclLiteral( ILRegister lit, long long unsigned int x , RegDT::RegDT dt)
{
  unsigned int temp = *(unsigned int *)(&x);
  commentLineno();
  writeToken(mkOpToken(ILSpec::IL_DCL_LITERAL));
  lit.SetNumSlots(4);
  writeSrcToken(lit);
  writeToken(temp);
  writeToken(temp);
  writeToken(temp);
  writeToken(temp);
}


ILRegister 
ILTokenTranscriber::AllocArrayStructure(int size, ILSpec::ILRegType type, int regNum)
{
  commentLineno();
  ILRegister rg(ILTranscriber::AllocArrayStructure(size,type,regNum));
  if(type == ILSpec::IL_REGTYPE_ITEMP) 
    writeToken(mkOpToken(ILSpec::IL_DCL_INDEXED_TEMP_ARRAY));
  else if(type == ILSpec::IL_REGTYPE_CONST_BUFF)
    writeToken(mkOpToken(ILSpec::IL_DCL_CONST_BUFFER));
  else 
    ERROR_UNIMPLEMENTED()(type);
  ILFormat::IL_Dst dt;
  memset(&dt,0,sizeof(dt));
  dt.bits.register_num      = rg.Number();
  dt.bits.register_type     = rg.Type();
  dt.bits.relative_address  = ILSpec::IL_ADDR_ABSOLUTE;
  dt.bits.immediate_present = 1;
  writeToken(dt.u32All);
  writeToken(size);

  return rg;
}

void 
ILTokenTranscriber::writeDstToken( const ILRegister& rg )
{

  ILFormat::IL_Dst dt;
  memset(&dt,0,sizeof(dt));
  dt.bits.register_num      = rg.Number();
  dt.bits.register_type     = rg.Type();
  dt.bits.modifier_present  = 1;



  ILFormat::IL_Dst_Mod mt;
  memset(&mt,0,sizeof(mt));

  mt.bits.component_x_r = 
    mt.bits.component_y_g =
      mt.bits.component_z_b =
        mt.bits.component_w_a = ILSpec::IL_MODCOMP_NOWRITE;

  //handle swizzle 
  for(int i=0; i<4; ++i)
  {
    switch(rg.SwizzleBytes()[i])
    {
    case ILRegister::SLOT0:
      mt.bits.component_x_r = ILSpec::IL_MODCOMP_WRITE;
      break;
    case ILRegister::SLOT1:
      mt.bits.component_y_g = ILSpec::IL_MODCOMP_WRITE;
      break;
    case ILRegister::SLOT2:
      mt.bits.component_z_b = ILSpec::IL_MODCOMP_WRITE;
      break;
    case ILRegister::SLOT3:
      mt.bits.component_w_a = ILSpec::IL_MODCOMP_WRITE;
      break;
    }
  }

  if(rg.IsArrayType())
  {
    dt.bits.relative_address = ILSpec::IL_ADDR_REG_RELATIVE;
    dt.bits.immediate_present = 1;

    writeToken(dt.u32All);
    writeToken(mt.u32All);
    ILRegister base = rg.GetBase();
    base.SetSwizzleBytes("xxxx");
    writeSrcToken(base);
    writeToken(rg.GetOffset());
  }
  else
  {
    writeToken(dt.u32All);
    writeToken(mt.u32All);
  }

}



void 
ILTokenTranscriber::writeSrcToken( const ILRegister& rg )
{

  ILFormat::IL_Src st;
  memset(&st,0,sizeof(st));
  st.bits.register_num      = rg.Number();
  st.bits.register_type     = rg.Type();
  st.bits.modifier_present  = 1;

  ILFormat::IL_Src_Mod mt;
  memset(&mt,0,sizeof(mt));
  mt.bits.swizzle_x_r     = swizzleChToCompSel(rg.SwizzleBytes()[0]);
  mt.bits.swizzle_y_g     = swizzleChToCompSel(rg.SwizzleBytes()[1]);;
  mt.bits.swizzle_z_b     = swizzleChToCompSel(rg.SwizzleBytes()[2]);;
  mt.bits.swizzle_w_a     = swizzleChToCompSel(rg.SwizzleBytes()[3]);;

  if(rg.GetDT() == RegDT::Double)
  {
    mt.bits.negate_x_r = 
      mt.bits.negate_z_b = (rg.GetFlags()&ILRegister::FLAG_NEGATE)!=0;
  }
  else
  {
    mt.bits.negate_x_r = 
      mt.bits.negate_y_g = 
        mt.bits.negate_z_b = 
          mt.bits.negate_w_a = (rg.GetFlags()&ILRegister::FLAG_NEGATE)!=0;
  }
  mt.bits.sign            = (rg.GetFlags()&ILRegister::FLAG_SIGN)!=0;
  mt.bits.abs             = (rg.GetFlags()&ILRegister::FLAG_ABS)!=0;
  //     mt.bits.invert          = 0;
  //     mt.bits.bias            = 0;
  //     mt.bits.x2              = 0;
  //     mt.bits.divComp         = 0;
  //     mt.bits.reserved        = 0;

  if(rg.IsArrayType())
  {
    st.bits.relative_address = ILSpec::IL_ADDR_REG_RELATIVE;
    st.bits.immediate_present = 1;

    writeToken(st.u32All);
    writeToken(mt.u32All);
    ILRegister base = rg.GetBase();
    base.SetSwizzleBytes("xxxx");
    writeSrcToken(base);
    writeToken(rg.GetOffset());
  }
  else
  {
    writeToken(st.u32All);
    writeToken(mt.u32All);
  }

}

void 
ILTokenTranscriber::Comment( const std::string& str )
{
  //output comment optoken
  writeToken(mkOpToken(ILSpec::IL_OP_COMMENT));

  int lenBytes = (int)str.length();
  //add between 1 and 4 bytes to force a multiple of 4 and make room for null terminator
  lenBytes +=  TOKEN_SIZE - (lenBytes%TOKEN_SIZE);

  //output comment length
  ILFormat::IL_Comment_Descriptor cd;
  cd.bits.length = lenBytes / TOKEN_SIZE;
  cd.bits.reserved = 0;
  writeToken(cd.u32All);

  union
  {
    char tokBytes[TOKEN_SIZE];
    TOKEN_TYPE tok;
  };

  //write out string one token at a time
  for(size_t i=0; i<str.length(); ++i)
  {
    tokBytes[i % TOKEN_SIZE] = str[i];
    if( (i+1) % TOKEN_SIZE == 0 )
    {
      writeToken(tok);
      tok = 0;
    }
  }

  //write last token (last token will always exist because loop above neglects trailing zero)
  writeToken(tok);
}

int ILTokenTranscriber::AllocResource(const ConstStringPtr& name,
                                      ILSpec::ILPixTexUsage usg,
                                      int requested_reg)
{
  commentLineno();
  int rid = ILTranscriber::AllocResource(name, usg, requested_reg);
  
  ILFormat::IL_DCLResourceOpCode op;
  op.bits.code = ILSpec::IL_DCL_RESOURCE;
  op.bits.controls_id = rid;
  op.bits.controls_type = usg;
  op.bits.reserved = 0;
  op.bits.controls_unnormalize = IsNormResource() ? 0 : 1;
  writeToken(op.u32All);

  static const TOKEN_TYPE el = defaultElementFormat();
  writeToken(el);

  return rid;
}




void ILBinaryTokenTranscriber::writeToken( TOKEN_TYPE t )
{
  os.write((char*)&t, sizeof t);
}

void ILHexTokenTranscriber::writeToken( TOKEN_TYPE t )
{
  os << "0x"; printHex(t); os << std::endl; 
  mHexWordCount++;
}


