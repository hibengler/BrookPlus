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

#include "ilregister.hpp"
#include "iltranscriber.hpp"
#include "assertlogging.hpp"

const char BasicILRegister::DEFAULT_SWIZZLE[4] = {'x','y','z','w'};

namespace
{
static char lookupSimpleSwizzles[5][5] = {
  "____",
  "x___",
  "xy__",
  "xyz_",
  "xyzw"
};

bool swizCmpLt(char a, char b)
{
  for(int i=0; i<4; ++i)
  {
    if(b==ILRegister::DEFAULT_SWIZZLE[i]) return false;
    if(a==ILRegister::DEFAULT_SWIZZLE[i]) return true;   
  }
  return false;
}

int swizChToIdx(char ch)
{
  for(int i=0; i<4; ++i)
  {
    if(ch==ILRegister::DEFAULT_SWIZZLE[i]) return i;
  }
  DIE("expected x, y, z, or w")(ch);
  return -1;
}

}

static const char*
regType2Str( ILSpec::ILRegType t )
{
    using namespace ILSpec;
    switch( t )
    {
        case IL_REGTYPE_TEMP:			return "r";
        //It's a hack so that AMD-HLSL doesn't use temp array for constant buffers
        case IL_REGTYPE_ITEMP:			return "cb";
        case IL_REGTYPE_CONST_BUFF:		return "cb";
        case IL_REGTYPE_LITERAL:		return "l";
        case IL_REGTYPE_INPUT:			return "v";
        case IL_REGTYPE_OUTPUT:			return "o";
        case IL_REGTYPE_PERSIST:		return "p";
        case IL_REGTYPE_GLOBAL:			return "g";
        case IL_REGTYPE_THREAD_ID:		return "vTid";    
        case IL_REGTYPE_ABSOLUTE_THREAD_ID:	return "vaTid";    
        case IL_REGTYPE_THREAD_GROUP_ID:	return "vTGroupid";    
        case IL_REGTYPE_GENERIC_MEM:		return "mem";    
        case IL_REGTYPE_WINCOORD:		return "vWinCoord";
        case IL_REGTYPE_CONST_BOOL:
        case IL_REGTYPE_CONST_FLOAT:
        case IL_REGTYPE_CONST_DOUBLE:
        case IL_REGTYPE_CONST_INT:
        case IL_REGTYPE_ADDR:
        case IL_REGTYPE_VERTEX:
        case IL_REGTYPE_INDEX:
        case IL_REGTYPE_OBJECT_INDEX:
        case IL_REGTYPE_BARYCENTRIC_COORD:
        case IL_REGTYPE_PRIMITIVE_INDEX:
        case IL_REGTYPE_QUAD_INDEX:
        case IL_REGTYPE_VOUTPUT:
        case IL_REGTYPE_PINPUT:
        case IL_REGTYPE_SPRITE:
        case IL_REGTYPE_POS:
        case IL_REGTYPE_INTERP:
        case IL_REGTYPE_FOG:
        case IL_REGTYPE_TEXCOORD:
        case IL_REGTYPE_PRICOLOR:
        case IL_REGTYPE_SECCOLOR:
        case IL_REGTYPE_SPRITECOORD:
        case IL_REGTYPE_FACE:
        case IL_REGTYPE_PRIMCOORD:
        case IL_REGTYPE_PRIMTYPE:
        case IL_REGTYPE_PCOLOR:
        case IL_REGTYPE_DEPTH:
        case IL_REGTYPE_STENCIL:
        case IL_REGTYPE_CLIP:
        case IL_REGTYPE_VPRIM:
        case IL_REGTYPE_IMMED_CONST_BUFF:
        case IL_REGTYPE_OMASK:
        default:
            return "!ERR!";
    }
}

std::string BasicILRegister::ToString() const
{
  if(mType == ILSpec::IL_REGTYPE_PERSIST || mType == ILSpec::IL_REGTYPE_GLOBAL)
  {
    ASSERT_WARNING(mNumber==0)(mNumber).Text("g and p registers should have no number");
    return regType2Str(mType);
  }
  else
  {
    return regType2Str(mType)+::ToString(mNumber);
  }
}

std::string BasicILRegister::SwizzleString(bool isDst) const
{
  //in source register '_' is not valid
  if(!isDst)
  {
    char str[2 + sizeof(mSwizzle)] = ".0000";

    for(int i=0; i<4; ++i)
      if(mSwizzle[i] != SLOTNOTUSED)
        str[i+1] = mSwizzle[i];

    return str;
  }
  else 
  {
    char str[2 + sizeof(mSwizzle)] = ".____";

    for(int i=0; i<4; ++i)
      if(mSwizzle[i] != SLOTNOTUSED)
        str[swizChToIdx(mSwizzle[i])+1] = mSwizzle[i];

    return str;
  }
}

int BasicILRegister::GetNumSlots() const
{
  DEBUG_CHECK(sizeof(mSwizzle)==4)(sizeof(mSwizzle));
  return (mSwizzle[0]==SLOTNOTUSED ? 0 : 1)
       + (mSwizzle[1]==SLOTNOTUSED ? 0 : 1)
       + (mSwizzle[2]==SLOTNOTUSED ? 0 : 1)
       + (mSwizzle[3]==SLOTNOTUSED ? 0 : 1);
}

void BasicILRegister::SetNumSlots( int val )
{
  DEBUG_CHECK(sizeof(mSwizzle)==4)(sizeof(mSwizzle));
  if(val<0) val = 0;
  ASSERT_ERROR(val<=4)(val).Text("more than 4 slots in a register not supported");
  memcpy(mSwizzle, lookupSimpleSwizzles[val], sizeof(mSwizzle));
}

bool BasicILRegister::IsStandardSwizzle() const
{
  return memcmp(mSwizzle, lookupSimpleSwizzles[GetNumSlots()],sizeof(mSwizzle))==0;
}

void BasicILRegister::SetSwizzleBytes( const char v[4] )
{
  DEBUG_CHECK(4==sizeof(mSwizzle))(sizeof(mSwizzle));
  memcpy(mSwizzle, v, sizeof(mSwizzle));
}

///
/// Re-arrange this swizzle such that it outputs to the following swizzled dest
/// if v.yx , then this.zw becomes this.wz
void BasicILRegister::ApplyDstSwizzle( const char dstOrig[4] )
{
  char dst[4] = {SLOTNOTUSED,SLOTNOTUSED,SLOTNOTUSED,SLOTNOTUSED};
  char* dstI = dst;
  char src[4] = {SLOTNOTUSED,SLOTNOTUSED,SLOTNOTUSED,SLOTNOTUSED};
  char* srcI = src;

  //compact both dst and src to left
  for(int i=0; i<4; ++i)
  {
    if(dstOrig[i]!=SLOTNOTUSED)
      *dstI++ = dstOrig[i];
    if(mSwizzle[i]!=SLOTNOTUSED)
      *srcI++ = mSwizzle[i];
  }

  //now sort them both so dst is in "natural" order
  //note, this N^2 sorting algorithm is likely faster with such a small n 
  //this loop will run at most 6 times
  for(int i=1; i<4; ++i)
  {
    ASSERT_ERROR(dst[i]!=dst[i-1] || dst[i]==SLOTNOTUSED)(dst[i])(dst[i-1])(i)
      .Text("Duplicate values not supported output swizzle");
    if(swizCmpLt(dst[i],dst[i-1]))
    {
      Swap(dst[i],dst[i-1]);
      Swap(src[i],src[i-1]);
      i=0;
    }
  }

  //now write output
  srcI = src;
  dstI = dst;
  memset(mSwizzle,SLOTNOTUSED,sizeof(mSwizzle));
  //(this works because dst is sorted)
  if(*dstI == SLOT0){ mSwizzle[0] = *srcI++; dstI++; }
  if(*dstI == SLOT1){ mSwizzle[1] = *srcI++; dstI++; }
  if(*dstI == SLOT2){ mSwizzle[2] = *srcI++; dstI++; }
  if(*dstI == SLOT3){ mSwizzle[3] = *srcI++; dstI++; }

  return;
}

void ILRegister::WidenSwizzleTo( int slots )
{
  if(slots==1) return;
  ASSERT_ERROR(GetNumSlots() == 1)(GetNumSlots());
  memset(mSwizzle+1, mSwizzle[0], slots-1);
}

std::string ILRegister::ToString() const
{
  if(IsArrayType()) {
    if (mType == ILSpec::IL_REGTYPE_GLOBAL ||
        mType == ILSpec::IL_REGTYPE_PERSIST  ) {
      ASSERT_WARNING(mNumber == 0)(mNumber).Text("g and p registers should have no number");
      return regType2Str(mType) + ::ToString("[") + mBase.ToString() + ".x + " + ::ToString(mOffset) + "]";
    } else {
      return regType2Str(mType) + ::ToString(mNumber) + "[" + mBase.ToString() + ".x + " + ::ToString(mOffset) + "]";      
    }
  } else {
    return BasicILRegister::ToString();
  }
}

std::string ILRegister::FlagString() const
{
  std::string fs;
  if((mFlags & FLAG_ABS) != 0)
    fs += "_abs";
  if((mFlags & FLAG_SIGN) != 0)
    fs += "_sign";
  if((mFlags & FLAG_NEGATE) != 0)
    if(GetDT() == RegDT::Double)
      fs += "_neg(y)";
    else
      fs += "_neg(xyzw)";
  return fs;
}
