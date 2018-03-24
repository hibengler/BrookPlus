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

#include "iltranslator.hpp"
#include "assertlogging.hpp"

//Layout a switch on RegDT
#define BIUFD(_B,_I,_U,_F,_D)                     \
switch(dt){                                       \
case RegDT::Bool:   { _B ;} break;              \
case RegDT::Char:                               \
case RegDT::Short:                              \
case RegDT::Int:    { _I ;} break;              \
case RegDT::UChar:                              \
case RegDT::UShort:                             \
case RegDT::UInt:   { _U ;} break;              \
case RegDT::Float:  { _F ;} break;              \
case RegDT::Double: { _D ;} break;              \
default: { DIE("Unknown RegDT")(dt)(op); } break; \
}

#define INVALID DIE("Invalid OpCode/DataType combination")(dt)(op);

//Call a given method on il, this line has a 1to1 mapping with IL
#define IL(cmd) il->cmd(dst,left,right);

//Convert to type and retry operation
#define RETRYAS(type) retryBinaryCommandAs(RegDT::type,op,dst,left,right);

///
/// We put large hacks in handleSpecialCases, so as to not clutter the table
#define SPECIAL handleSpecialCases(op,dst,left,right)

void ILTranslator::BinaryCommand( OpType::OpType op, ILRegister& dst, ILRegister left, ILRegister right ) const
{
  ASSERT_ERROR(left.GetDT() == right.GetDT())(left.GetDT())(right.GetDT())
    .Text("Type mismatch");
  ASSERT_ERROR(left.GetNumSlots() == right.GetNumSlots())(left.GetNumSlots())(right.GetNumSlots())
    .Text("Type mismatch");
  RegDT::RegDT dt = left.GetDT();
  ASSERT_ERROR(dt!=RegDT::Invalid);
  dst.SetDT(left.GetDT());
  int dstSlots =  left.GetNumSlots();
  if(op==OpType::DotProduct) dstSlots = 1; //special case for dp
  if(dst.GetNumSlots() != dstSlots)
    dst.SetNumSlots(left.GetNumSlots());
  if(!dst.IsStandardSwizzle() && op!=OpType::DotProduct)
  {
    left.ApplyDstSwizzle(dst.SwizzleBytes());
    right.ApplyDstSwizzle(dst.SwizzleBytes());
  }

  switch(op)
  {
    ////////////////////////////////////////////////////////////////////////////
    //////BOOL///////////INT////////////UNSIGNED/////////FLOAT/////////DOUBLE///
  case OpType::And: 
    BIUFD(IL(And),       IL(And),       IL(And),       INVALID,        INVALID);
    break;
  case OpType::Or:
    BIUFD(IL(Or),        IL(Or),        IL(Or),        INVALID,        INVALID);
    break;
  case OpType::Xor:
    BIUFD(IL(Xor),       IL(Xor),       IL(Xor),       INVALID,        INVALID);
    break;
  case OpType::Mult:
    BIUFD(RETRYAS(Int),  IL(IMul),      IL(UMul),      IL(FMult),      IL(DMult));
    break;
  case OpType::Div:
    BIUFD(RETRYAS(UInt), IL(IDiv),      IL(UDiv),      IL(FDiv),       IL(DDiv));
    break;
  case OpType::Mod:
    BIUFD(RETRYAS(UInt), IL(IMod),      IL(UMod),      IL(FMod),       IL(FMod));
    break;
  case OpType::Plus:
    BIUFD(RETRYAS(Int),  IL(IAdd),      IL(IAdd),      IL(FAdd),       IL(DAdd));
    break;
  case OpType::Minus:
    BIUFD(RETRYAS(Int),  IL(ISub),     RETRYAS(Int),  IL(FSub),        IL(DSub));
    break;
  case OpType::LShift:
    BIUFD(RETRYAS(Int),  IL(IShl),      IL(IShl),       INVALID,       INVALID);
    break;
  case OpType::RShift:
    BIUFD(RETRYAS(Int),  IL(IShr),      IL(UShr),       INVALID,       INVALID);
    break;
  case OpType::CmpLT:
    BIUFD(RETRYAS(Int),  IL(ILt),       IL(ULt),       IL(FLt),        IL(DLt));
    dst.SetDT(RegDT::Bool);
    break;
  case OpType::CmpGE:
    BIUFD(RETRYAS(Int),  IL(IGe),       IL(UGe),       IL(FGe),        IL(DGe));
    dst.SetDT(RegDT::Bool);
    break;
  case OpType::CmpEQ:
    BIUFD(RETRYAS(Int),  IL(IEq),       IL(IEq),       IL(FEq),        IL(DEq));
    dst.SetDT(RegDT::Bool);
    break;
  case OpType::CmpNE:
    BIUFD(RETRYAS(Int),  IL(INe),       IL(INe),       IL(FNe),        IL(DNe));
    dst.SetDT(RegDT::Bool);
    break;
  case OpType::Min:
    BIUFD(RETRYAS(Int),  IL(IMin),      IL(UMin),      IL(FMin),       IL(DMin));
    break;
  case OpType::Max:
    BIUFD(RETRYAS(Int),  IL(IMax),      IL(UMax),      IL(FMax),       IL(DMax));
    break;
  case OpType::DotProduct:
    BIUFD(RETRYAS(Int),  SPECIAL,       SPECIAL,       IL(FDot),       INVALID);
    break;
  case OpType::MultAddToDst:
    BIUFD(RETRYAS(Int),  IL(IMulAddDst),IL(UMulAddDst),IL(FMulAddDst), IL(DMulAddDst));
    break;
  //Generic opcode rewrites
  case OpType::CmpLE:
    BinaryCommand(OpType::CmpGE,dst,right,left); //flip args
    break;
  case OpType::CmpGT:
    BinaryCommand(OpType::CmpLT,dst,right,left); //flip args
    break;
  default:
    DIE("Unhandled Binary OpType")(dt)(op)(left)(right);
    break;
  }
}

void
ILTranslator::handleSpecialCases( OpType::OpType op, ILRegister& dst, ILRegister left, ILRegister right ) const
{
    if ( OpType::DotProduct == op )
    {
        if ( 1 == left.GetNumSlots() )
        {
            BinaryCommand( OpType::Mult, dst, left, right );
            return;
        }
        // generic dot product
        dst.SetNumSlots( 1 );
        ILRegister tmp = il->AllocTempRegister();
        ILRegister tmpl = tmp;
        ILRegister tmpr = tmp;
        ILRegister tmpd = tmp;
        // first do the multiply
        BinaryCommand( OpType::Mult, tmp, left, right );
        tmpl.SetDT( tmp.GetDT() );
        tmpr.SetDT( tmp.GetDT() );
        tmpd.SetDT( tmp.GetDT() );
        
        // reduce the vector to <= 2 elements
        switch( tmp.GetNumSlots() )
        {
            case 4:
                tmpl.SetSwizzleBytes( "xy__" );
                tmpr.SetSwizzleBytes( "zw__" );
                BinaryCommand( OpType::Plus, tmpd, tmpl, tmpr );
                break;
            case 3:
                tmpl.SetSwizzleBytes( "x___" );
                tmpr.SetSwizzleBytes( "z___" );
                BinaryCommand( OpType::Plus, tmpd, tmpl, tmpr );
                break;
        }

        // reduce the vector to 1 element and write output
        tmpl.SetSwizzleBytes( "x___" );
        tmpr.SetSwizzleBytes( "y___" );
        BinaryCommand( OpType::Plus, dst, tmpl, tmpr );
    }
    else if ( OpType::AbsoluteValue == op && RegDT::Int == left.GetDT() )
    {
        ILRegister tmp = il->AllocTempRegister();

        BinaryCommand( OpType::RShift, tmp, left, il->ConstIntThirtyOne(left.GetDT(),left.GetNumSlots()) );
        BinaryCommand( OpType::Xor,    left, left, tmp );
        BinaryCommand( OpType::Minus,  dst, left, tmp );
    }
//   RegDT::RegDT dt = left.GetDT();
// 
//   if(op == OpType::Minus && dt == RegDT::Int)
//   {
//     // l-r === l+(-r)
//     right = tmpNegate(right);
//     BinaryCommand(OpType::Plus, dst, left, right);
//   }
//   else if(op == OpType::Div && dt == RegDT::Int)
//   {
// //     //IL(IDiv) does not exist :(
// //     //first do regular divide
// //     RETRYAS(UInt); 
// //     dst.SetDT(RegDT::Int);
// //     //dst now contains positive result
// //     //now figure out sign
// //     ILRegister isLeftNeg = il->AllocTempRegister();
// //     ILRegister isRightNeg = il->AllocTempRegister();
// //     il->ILt(isLeftNeg, left, il->ConstIntZero(left.GetDT(),left.GetNumSlots()));
// //     il->ILt(isRightNeg, right, il->ConstIntZero(right.GetDT(),right.GetNumSlots()));
// //     il->Xor(il->Scratch(), isRightNeg, isLeftNeg);
// //     il->If(il->Scratch());
// //     il->INegate(dst,dst);
// //     il->EndIf();
//   }
//   else if(op == OpType::Mod && dt == RegDT::Int)
//   {
//     //There is some disagreement over what signed mod is
//     // see: http://en.wikipedia.org/wiki/Modulo_operation
//     // for now we define it same a C
//     RETRYAS(UInt);  
//     dst.SetDT(RegDT::Int);
//     il->ILt(il->Scratch(), left, il->ConstIntZero(left.GetDT(),left.GetNumSlots()));
//     il->If(il->Scratch());
//     il->INegate(dst,dst);
//     il->EndIf();
//   }
    else
    {
        DIE( "Unhandled special case" )( op ); // (dt);
    }
}

#undef IL
#undef RETRYAS
#undef SPECIAL
#define IL(cmd) il->cmd(dst,src);
#define RETRYAS(type) src=TmpConvertInput(RegDT::type, src); dt=RegDT::type; UnaryCommand(op,dst,src); return;
#define SPECIAL handleSpecialCases(op,dst,src,src)

void ILTranslator::UnaryCommand( OpType::OpType op, ILRegister& dst, ILRegister src ) const
{
  RegDT::RegDT dt = src.GetDT();
  dst.SetDT(src.GetDT());
  if(dst.GetNumSlots() != src.GetNumSlots())
    dst.SetNumSlots(src.GetNumSlots());
  if(!dst.IsStandardSwizzle())
  {
    src.ApplyDstSwizzle(dst.SwizzleBytes());
  }

  switch(op)
  {
      ////////////////////////////////////////////////////////////////////////////
      //////BOOL////////////////INT//////////UNSIGNED//////FLOAT////////DOUBLE////
      case OpType::AbsoluteValue:
          BIUFD(RETRYAS(Int),	SPECIAL,     IL(Move),     IL(FAbs),    IL(DAbs));
          break;
      case OpType::Negate:
          BIUFD(RETRYAS(Int),	IL(INegate), RETRYAS(Int), IL(FNegate), IL(DNegate));
          break;
      case OpType::Not:
          BIUFD(IL(BitNot),	IL(BitNot),  IL(BitNot),   INVALID,     INVALID);
          break;  
      case OpType::Assign:
          BIUFD(IL(Move),	IL(Move),    IL(Move),     IL(Move),    IL(Move));
          break;
      default:
          DIE("Unhandled Unary OpType")(dt)(op)(dst)(src);
          break;
  }
}

//Generate (compiletime) unique number of the given conversion
#define CONVERT_ENUM(from,to) ((from*RegDT::_count)+to)

//First convert to the given type, then retry the current operation
#define CONVERT_RETRY(type) ConvertTo(RegDT::type,dst,src); ConvertTo(dstdt,dst,dst);

void ILTranslator::ConvertTo( RegDT::RegDT dstdt, ILRegister& dst, ILRegister src ) const
{
  RegDT::RegDT dt = src.GetDT();
  dst.SetDT(dstdt);
  if(dst.GetNumSlots() != src.GetNumSlots())
    dst.SetNumSlots(src.GetNumSlots());
  if(!dst.IsStandardSwizzle())
  {
    src.ApplyDstSwizzle(dst.SwizzleBytes());
  }

  switch(CONVERT_ENUM(dt,dstdt))
  {
  //Trivial cases
  case CONVERT_ENUM(RegDT::Char,  RegDT::Char): break;
  case CONVERT_ENUM(RegDT::UChar,   RegDT::UChar): break;
  case CONVERT_ENUM(RegDT::Short,  RegDT::Short): break;
  case CONVERT_ENUM(RegDT::UShort, RegDT::UShort): break;
  case CONVERT_ENUM(RegDT::Bool,  RegDT::Bool): break;
  case CONVERT_ENUM(RegDT::Int,   RegDT::Int): break;
  case CONVERT_ENUM(RegDT::UInt,  RegDT::UInt): break;
  case CONVERT_ENUM(RegDT::Float, RegDT::Float): break;
  case CONVERT_ENUM(RegDT::Double,RegDT::Double): break;

  //ToBool
  case CONVERT_ENUM(RegDT::Char,  RegDT::Bool):
  case CONVERT_ENUM(RegDT::UChar,   RegDT::Bool):
  case CONVERT_ENUM(RegDT::Short,  RegDT::Bool):
  case CONVERT_ENUM(RegDT::UShort, RegDT::Bool):
  case CONVERT_ENUM(RegDT::Int,   RegDT::Bool): 
  case CONVERT_ENUM(RegDT::UInt,  RegDT::Bool):
  case CONVERT_ENUM(RegDT::Float, RegDT::Bool): 
  case CONVERT_ENUM(RegDT::Double,RegDT::Bool): 
    // compare against zero
    BinaryCommand(OpType::CmpNE, dst, src, il->ConstIntZero(src.GetDT(),src.GetNumSlots()));
    break;

  //ToInt/Unsigned
  case CONVERT_ENUM(RegDT::Bool,  RegDT::Char):
  case CONVERT_ENUM(RegDT::Bool,   RegDT::UChar):
  case CONVERT_ENUM(RegDT::Bool,  RegDT::Short):
  case CONVERT_ENUM(RegDT::Bool, RegDT::UShort):
  case CONVERT_ENUM(RegDT::Bool,  RegDT::Int): 
  case CONVERT_ENUM(RegDT::Bool,  RegDT::UInt):
    //mask out upper bits
    il->And(dst,src,il->ConstIntOne(src.GetDT(),src.GetNumSlots()));
    break;

  case CONVERT_ENUM(RegDT::UChar,  RegDT::UInt):
  case CONVERT_ENUM(RegDT::UShort,  RegDT::UInt):
  case CONVERT_ENUM(RegDT::Char,  RegDT::UInt):
  case CONVERT_ENUM(RegDT::Short,  RegDT::UInt):
  case CONVERT_ENUM(RegDT::Int,   RegDT::UInt):
    //perhaps do nothing?
    IL(Move);
    break;
  
  case CONVERT_ENUM(RegDT::UInt,  RegDT::Char):
  case CONVERT_ENUM(RegDT::UInt,  RegDT::Short):
  case CONVERT_ENUM(RegDT::UInt,  RegDT::UChar):
  case CONVERT_ENUM(RegDT::UInt,  RegDT::UShort):
    //perhaps do nothing?
    IL(Move); 
    break;

  case CONVERT_ENUM(RegDT::Int,  RegDT::UChar):
  case CONVERT_ENUM(RegDT::Int,  RegDT::UShort):
  case CONVERT_ENUM(RegDT::Int,  RegDT::Char):
  case CONVERT_ENUM(RegDT::Int,  RegDT::Short):
    //perhaps do nothing?
    IL(Move); 
    break;

  case CONVERT_ENUM(RegDT::Char,  RegDT::Int):
  case CONVERT_ENUM(RegDT::Short,  RegDT::Int):
  case CONVERT_ENUM(RegDT::UChar,  RegDT::Int):
  case CONVERT_ENUM(RegDT::UShort,  RegDT::Int):
  case CONVERT_ENUM(RegDT::UInt,    RegDT::Int): 
    //perhaps do nothing?
    IL(Move); 
    break;

  case CONVERT_ENUM(RegDT::Float,  RegDT::Char):
  case CONVERT_ENUM(RegDT::Float,  RegDT::Short):
  case CONVERT_ENUM(RegDT::Float,   RegDT::Int):  
    IL(FToI); 
    break;
  
  case CONVERT_ENUM(RegDT::Float,  RegDT::UChar):
  case CONVERT_ENUM(RegDT::Float,  RegDT::UShort):
  case CONVERT_ENUM(RegDT::Float,   RegDT::UInt):  
    IL(FToU); 
    break;

  case CONVERT_ENUM(RegDT::Double,  RegDT::Char): 
  case CONVERT_ENUM(RegDT::Double,  RegDT::UChar):
  case CONVERT_ENUM(RegDT::Double,  RegDT::Short): 
  case CONVERT_ENUM(RegDT::Double,  RegDT::UShort):
  case CONVERT_ENUM(RegDT::Double,  RegDT::Int): 
  case CONVERT_ENUM(RegDT::Double,  RegDT::UInt): 
    CONVERT_RETRY(Float);
    break;

  //ToFloat
  case CONVERT_ENUM(RegDT::Bool,    RegDT::Float): CONVERT_RETRY(Int); break;
  case CONVERT_ENUM(RegDT::Char,     RegDT::Float): IL(IToF); break;
  case CONVERT_ENUM(RegDT::Short,     RegDT::Float): IL(IToF); break;
  case CONVERT_ENUM(RegDT::Int,     RegDT::Float): IL(IToF); break;
  case CONVERT_ENUM(RegDT::UChar,    RegDT::Float): IL(UToF); break;
  case CONVERT_ENUM(RegDT::UShort,    RegDT::Float): IL(UToF); break;
  case CONVERT_ENUM(RegDT::UInt,    RegDT::Float): IL(UToF); break;
  case CONVERT_ENUM(RegDT::Double,  RegDT::Float): IL(DToF); break;
  
  //ToDouble
  case CONVERT_ENUM(RegDT::Char,    RegDT::Double): 
  case CONVERT_ENUM(RegDT::UChar,    RegDT::Double): 
  case CONVERT_ENUM(RegDT::Short,    RegDT::Double): 
  case CONVERT_ENUM(RegDT::UShort,    RegDT::Double): 
  case CONVERT_ENUM(RegDT::Bool,    RegDT::Double): 
  case CONVERT_ENUM(RegDT::Int,     RegDT::Double): 
  case CONVERT_ENUM(RegDT::UInt,    RegDT::Double): 
    CONVERT_RETRY(Float);
    break;
  case CONVERT_ENUM(RegDT::Float,   RegDT::Double): 
    IL(FToD);
    break;

  default:
    DIE("Cant convert")(dt)(dstdt);
  }
}

ILRegister ILTranslator::TmpConvertInput( RegDT::RegDT newdt, ILRegister src ) const
{
  //null case
  if(src.GetDT() == newdt) return src;

  ILRegister tmp = il->AllocTempRegister();
  //convert without modifying our datatype
  ConvertTo(newdt,tmp,src);
  return tmp;
}

ILRegister ILTranslator::TmpReslotInput( int slots, ILRegister src ) const
{
  //null case
  if(src.IsStandardSwizzle() && src.GetNumSlots() == slots) return src;

  ILRegister tmp = il->AllocTempRegister();
  //convert without modifying our datatype
  tmp.SetDT(src.GetDT());
  tmp.SetNumSlots(slots);
  il->Move(tmp,src);
  return tmp;
}

void ILTranslator::retryBinaryCommandAs( RegDT::RegDT newdt, OpType::OpType op, ILRegister& dst, ILRegister left, ILRegister right ) const
{
  left=TmpConvertInput(newdt,left);
  right=TmpConvertInput(newdt,right);
  BinaryCommand(op,dst,left,right);
}

ILRegister ILTranslator::tmpNegate( ILRegister src ) const
{
  ILRegister tmp = il->AllocTempRegister();
  UnaryCommand(OpType::Negate, tmp, src);
  return tmp;
}

ILTranslator::ILTranslator( ILTranscriber* _il) : il(_il)
{}

int ILTranslator::RunOpStatically( OpType::OpType op, int l, int r )
{
  switch(op)
  {
  case OpType::And:      return l & r;
  case OpType::Or:       return l | r;
  case OpType::Xor:      return l ^ r;
  case OpType::Mult:     return l * r;
  case OpType::Div:      return l / r;
  case OpType::Mod:      return l % r;
  case OpType::Plus:     return l + r;
  case OpType::Minus:    return l - r;
  case OpType::LShift:   return l << r;
  case OpType::RShift:   return l >> r;
  case OpType::CmpLT:    return l < r;
  case OpType::CmpGE:    return l >= r;
  case OpType::CmpEQ:    return l == r;
  case OpType::CmpNE:    return l != r;
  case OpType::CmpLE:    return l <= r;
  case OpType::CmpGT:    return l > r;
  default:
    DIE("Unhandled Binary OpType")(op)(l)(r);
    return -1;
  }
}

int ILTranslator::RunUnaryOpStatically( OpType::OpType op, int l)
{
  switch(op)
  {
  case OpType::Negate: return -l;
  case OpType::Not: return !l; 
  default:
    DIE("Unhandled Unary OpType")(op);
    return -1;
  }
}




