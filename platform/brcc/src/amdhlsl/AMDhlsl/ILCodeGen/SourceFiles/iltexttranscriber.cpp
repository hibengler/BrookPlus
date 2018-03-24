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

#include "iltexttranscriber.hpp"

namespace ILSpec
{
#include "ILTables.h"
}

namespace
{
  const char* pixTexUsageToStr( ILSpec::ILPixTexUsage usg )
  {
    using namespace ILSpec;
    switch(usg)
    {
    case IL_USAGE_PIXTEX_4COMP: return "4c";
    case IL_USAGE_PIXTEX_1D: return "1d";
    case IL_USAGE_PIXTEX_2D: return "2d";
    case IL_USAGE_PIXTEX_3D: return "3d";
    case IL_USAGE_PIXTEX_CUBEMAP: return "cubemap";
    case IL_USAGE_PIXTEX_2DMSAA: return "2dmsaa";
    case IL_USAGE_PIXTEX_BUFFER: return "buffer";
    case IL_USAGE_PIXTEX_1DARRAY: return "1darray";
    case IL_USAGE_PIXTEX_2DARRAY: return "2darray";
    case IL_USAGE_PIXTEX_CUBEMAP_ARRAY: return "cubemaparray";
    default:
      NOTICE("unknown usage")(usg);
      return "!!ERR!!";
    };
  }

  const char* 
  importUsageToStr( ILSpec::ILImportUsage usg )
  {
      using namespace ILSpec;
      switch(usg)
      {
          case IL_IMPORTUSAGE_POS:
              return "pos";
          case IL_IMPORTUSAGE_POINTSIZE:
              return "pointsize";
          case IL_IMPORTUSAGE_COLOR:
              return "color";
          case IL_IMPORTUSAGE_BACKCOLOR:
              return "backcolor";
          case IL_IMPORTUSAGE_FOG:
              return "fog";
          case IL_IMPORTUSAGE_PRIMCOORDTYPE:
              return "primcoordtype";
          case IL_IMPORTUSAGE_GENERIC:
              return "generic";
          case IL_IMPORTUSAGE_CLIPDISTANCE:
              return "clipdistance";
          case IL_IMPORTUSAGE_CULLDISTANCE:
              return "culldistance";
          case IL_IMPORTUSAGE_PRIMITIVEID:
              return "primitiveid";
          case IL_IMPORTUSAGE_VERTEXID:
              return "vertexid";
          case IL_IMPORTUSAGE_INSTANCEID:
              return "instanceid";
          case IL_IMPORTUSAGE_ISFRONTFACE:
              return "isfrontface";
          case IL_IMPORTUSAGE_LOD:
              return "lod";
          case IL_IMPORTUSAGE_COLORING:
              return "coloring";
          case IL_IMPORTUSAGE_NODE_COLORING:
              return "node_coloring";
          case IL_IMPORTUSAGE_NORMAL:
              return "normal";
          case IL_IMPORTUSAGE_RENDERTARGET_ARRAY_INDEX:
              return "rendertarget_array_index";
          case IL_IMPORTUSAGE_VIEWPORT_ARRAY_INDEX:
              return "viewport_array_index";
          case IL_IMPORTUSAGE_UNDEFINED:
              return "";
          case IL_IMPORTUSAGE_SAMPLE_INDEX:
              return "sample_index";
          default: WARNING_UNIMPLEMENTED()(usg);
      }
      return "!ERR!";
  }

  const char* 
  interpModeToStr( ILSpec::ILInterpMode intrp )
  {
    using namespace ILSpec;
    switch(intrp)
    {
    case IL_INTERPMODE_NOTUSED:                       return "notused";
    case IL_INTERPMODE_CONSTANT:                      return "constant";
    case IL_INTERPMODE_LINEAR:                        return "linear";
    case IL_INTERPMODE_LINEAR_CENTROID:               return "linear_centroid";
    case IL_INTERPMODE_LINEAR_NOPERSPECTIVE:          return "linear_noperspective";
    case IL_INTERPMODE_LINEAR_NOPERSPECTIVE_CENTROID: return "linear_noperspective_centroid";
    case IL_INTERPMODE_LINEAR_SAMPLE:                 return "linear_sample";
    case IL_INTERPMODE_LINEAR_NOPERSPECTIVE_SAMPLE:   return "linear_noperspective_sample";
    default: WARNING_UNIMPLEMENTED()(intrp);
      return "!ERR!";
    }
  }

}

void ILTextTranscriber::op_3i_1o( const char* txt , ILSpec::ILOpCode bin, const ILRegister& d, const ILRegister& a, const ILRegister& b, const ILRegister& c )
{
  commentLineno();
  os<<txt<<' '<<d.DstILString()<<','<<a.SrcILString()<<','<<b.SrcILString()<<','<<c.SrcILString()<<std::endl;
}
void 
ILTextTranscriber::op_2i_1o( const char* txt , ILSpec::ILOpCode bin, const ILRegister& d, const ILRegister& l, const ILRegister& r, int )
{
  commentLineno();
  os<<txt<<' '<<d.DstILString()<<','<<l.SrcILString()<<','<<r.SrcILString()<<std::endl;
}

void 
ILTextTranscriber::op_1i_1o( const char* txt , ILSpec::ILOpCode bin ,ILRegister d, ILRegister l, int )
{
  commentLineno();
  //It's a hack so that AMD-HLSL doesn't use temp array for constant buffers
  if(d.Type() != ILSpec::IL_REGTYPE_ITEMP)
  {
    os<<txt<<' '<<d.DstILString()<<','<<l.SrcILString()<<std::endl;
  }
}

void 
ILTextTranscriber::op_1i_0o( const char* txt , ILSpec::ILOpCode bin ,ILRegister d)
{
  commentLineno();
  os<<txt<<' '<<d.SrcILString()<< std::endl;
}

void 
ILTextTranscriber::op_0i_0o( const char* txt , ILSpec::ILOpCode bin, int /*control*/ )
{
  commentLineno();
  os<<txt<<std::endl;
  if ( bin == ILSpec::IL_OP_RET || bin == ILSpec::IL_OP_ENDMAIN ) {
    os << std::endl;
  }
}

void 
ILTextTranscriber::op_il_hdr( const char* txt, ILSpec::IL_Shader_Type t )
{
  os << "il_" << txt << "_2_0" << std::endl;
}

void 
ILTextTranscriber::op_with_int( const char* txt, ILSpec::ILOpCode bin, unsigned int x )
{
  commentLineno();
  os << txt << ' ' << x <<  std::endl;
}

void 
ILTextTranscriber::op_call( const char* txt, 
                            ILSpec::ILOpCode bin,
                            ConstStringPtr comment,
                            unsigned int x )
{
  commentLineno();
#if !defined(DISABLE_ILCOMMENT)
  os << txt << ' ' << x << ' ';
  if ( !ILCommentWrapper::IsDisabled() && comment.IsValid() ) {
    ILCOMMENT(this) << comment;
  }
  else {
    os << std::endl;
  }
#else
  os << txt << ' ' << x << std::endl;
#endif
}

void
ILTextTranscriber::DclLdsSizeOrMode(ILSpec::ILOpCode bin, int n, ILSpec::IL_LDS_SHARING_MODE mode)
{
  commentLineno();
  if (bin == ILSpec::IL_OP_DCL_LDS_SIZE_PER_THREAD){
    os << "dcl_lds_size_per_thread "  << n <<std::endl;
    return;
  }
  const char* str = (mode == ILSpec::IL_LDS_SHARING_MODE_ABSOLUTE)? "_wavefrontAbs": "_wavefrontRel";
  os << "dcl_lds_sharing_mode " << str <<std::endl;
  return;
}

void 
ILTextTranscriber::DclOutput( ILSpec::ILImportUsage usg, ILRegister reg )
{
  commentLineno();
  os << "dcl_output_usage(" << importUsageToStr(usg) << ") " << reg.DstILString() << std::endl;
}

void 
ILTextTranscriber::DclInput( ILSpec::ILImportUsage usg, ILSpec::ILInterpMode interp, ILRegister reg )
{
  commentLineno();
  if ( ILSpec::IL_IMPORTUSAGE_POS == usg )
  {
      os << "dcl_input_position_interp(linear_noperspective) v0.xy__\n";
      return;
  }
  os << "dcl_input_usage(" << importUsageToStr(usg) << ")";
  if(interp != ILSpec::IL_INTERPMODE_NOTUSED)
    os << "_interp(" << interpModeToStr(interp) << ")";
  os << ' ' << reg.DstILString() << std::endl;
}

void 
ILTextTranscriber::DclLiteral( ILRegister lit, long long unsigned int x, RegDT::RegDT dt)
{
    // Double literal implementation
    if(dt == RegDT::Double)
    {
      // Split the two halves of the Double Literal
      unsigned int lowerHalf = *(unsigned int *)(&x);
      long long unsigned int y = x >> 32;
      unsigned int upperHalf = *(unsigned int *)(&y);
      commentLineno();
      // Write the lower half into .x position
      os << "dcl_literal " << lit.ToString() << ",0x"; 
      printHex(lowerHalf);
      // Write the upper half into .y position.
      os << ",0x"; 
      printHex(upperHalf);
      os << ",0x"; 
      printHex(lowerHalf);
      os << ",0x";
      printHex(upperHalf);
      os << std::endl;
    }
    else
    {
      unsigned int y = *(unsigned int *)(&x);
      commentLineno();
      os << "dcl_literal " << lit.ToString() << ",0x"; 
      printHex(y);
      os << ",0x"; 
      printHex(y);
      os << ",0x"; 
      printHex(y);
      os << ",0x";
      printHex(y);
      os << std::endl;
    }
}


ILRegister 
ILTextTranscriber::AllocArrayStructure(int size, ILSpec::ILRegType type, int regNum)
{
  commentLineno();
  ILRegister rg(ILTranscriber::AllocArrayStructure(size,type,regNum));

  if ( mAllocMode == ALLOC_NORMAL ) {
    if(type==ILSpec::IL_REGTYPE_ITEMP) 
    {
      //It's a hack so that AMD-HLSL doesn't use temp array for constant buffers
      //os << "dcl_indexed_temp_array x" << rg.Number() << '[' << size << ']' << std::endl;
    }
    else if(type==ILSpec::IL_REGTYPE_CONST_BUFF)
      os << "dcl_cb cb" << rg.Number() << '[' << size << ']' << std::endl;
    else
      ERROR_UNIMPLEMENTED()(type);
  } else if ( mAllocMode == ALLOC_GLOBAL ) {
    os <<";global (g) declared, size = " << size << std::endl;
  } else if ( mAllocMode == ALLOC_PERSISTENT ) {
    os <<";persist (p) declared, size = " << size << std::endl;
  } else {
    ERROR_UNIMPLEMENTED() (mAllocMode);
  }
  
  return rg;
}

void 
ILTextTranscriber::Comment( const std::string& str )
{
  os << ";" << str << std::endl;
}

// if requested reg is -1, just select a new one
int
ILTextTranscriber::AllocResource(const ConstStringPtr& name,
                                 ILSpec::ILPixTexUsage usg,
                                 int requested_reg)
{
  commentLineno();
  int rid = ILTranscriber::AllocResource(name, usg, requested_reg);
  os << "dcl_resource_id("<<rid<<")_type("<<pixTexUsageToStr(usg);

  if (IsNormResource() == false)
    os<<",unnorm";

  os << ")_fmtx(float)_fmty(float)_fmtz(float)_fmtw(float)" << std::endl;
  return rid;
}

void
ILTextTranscriber::op_resource( const char* txt ,
                                ILSpec::ILOpCode bin ,
                                ILRegister d,
                                int resource,
                                int sampler,
                                TextureLoadFlags::TextureLoadFlags flags,
                                unsigned int offset, 
                                ILRegister a0, ILRegister a1, ILRegister a2 )
{
  commentLineno();


  for(const char* i=txt; *i!='\0'; ++i) {
    //do replacement
    switch (*i){
    case 'I':
      // hlsl takes integer offsets in range -8 to 7
      // il allows one halfs as well
		{
      if(offset){
        int n0 = (offset) & 0xff;
        n0 = (n0 << 24)>> 24;
        int n1 = (offset>>8) & 0xff;
        n1 = (n1 << 24)>> 24;
        int n2 = (offset>>16) & 0xff;   
        n2 = (n2 << 24)>> 24;
        os << "_aoffimmi(" << n0 <<".0," << n1 << ".0," <<n2 << ".0)";
      }
    }
    break;
    case 'R':
      os << resource;
      break;
    case 'S':
      os << sampler;
      break;
    default:
      os << *i;
      break;
    }
  }
  os << ' ' << d.DstILString() << ',' << a0.SrcILString();
  if(a1) os << ',' << a1.SrcILString();
  if(a2) os << ',' << a2.SrcILString();
  os << std::endl;

}
