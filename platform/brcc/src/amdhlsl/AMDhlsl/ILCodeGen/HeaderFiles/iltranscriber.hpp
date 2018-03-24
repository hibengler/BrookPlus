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

#ifndef AMD_ILTRANSCRIBER_HPP
#define AMD_ILTRANSCRIBER_HPP

#include <iostream>
#include <vector>
#include <map>

#include "shaderdescription.hpp"
#include "convert.hpp"
#include "conststring.hpp"
#include "ilregister.hpp"
#include "inputpatcher.hpp"

namespace AST
{
  class Modifier;
  class Symbol;
  typedef SmartPtr< Symbol > SymbolPtr;
  class FunctionDeclaration;  
  typedef SmartPtr< FunctionDeclaration > FunctionDeclarationPtr;  
}

//#define DISABLE_ILCOMMENT
//#define DISABLE_LINENOS
//#define DISABLE_DUMPSYMBOLS

namespace TextureLoadFlags
{
  enum TextureLoadFlags
  {
    RESOURCE,
    RESOURCE_SAMPLER,
    RESOURCE_SAMPLER_ADDROFF
  };
}

#ifndef DISABLE_ILCOMMENT
#define ILCOMMENT(il) if(ILCommentWrapper::IsDisabled()){}else ILCommentWrapper(*il)
#else
#define ILCOMMENT(il) if(true){}else ILCommentWrapper(*il)
#endif

class ILTranscriber;
class SemanticPairT : public std::pair<ILRegister, ILRegister>
{
public:
  SemanticPairT(const ILRegister& a, const ILRegister& b, const ConstStringPtr& name)
    : std::pair<ILRegister, ILRegister>(a,b)
    , mName(name)
  {}

  const ConstStringPtr& GetName() const { return mName; }
private:
  ConstStringPtr mName;
  
};
typedef std::vector< SemanticPairT > SemanticMapT; 
typedef SmartPtr<ILTranscriber> ILTranscriberPtr;

/**
 * Outputs a formatted IL program
 */
class ILTranscriber : public RefCountedObj
{
  enum { IEEE = 1 };
  enum { EXIT_SHADER_FUNC=0 };
public:
  //constructor
  ILTranscriber( std::ostream& os );

  virtual ~ILTranscriber(){}

  int RecodeSampler(int i, int dx9_samplers){
    if (dx9_samplers){
      return dx9_forced_samplers[i];
    }
    return i;
  }
  //Context functions
  enum Context { CTX_GLOBAL, CTX_FUNC_ARGS, CTX_MAIN_ARGS, CTX_LOCAL };
  Context CurrentContext() const { return mCurrentContext; }
  void SetCurrentContext(Context val) { mCurrentContext = val; }
  bool IsFunctionArgContext() const { return mCurrentContext==CTX_FUNC_ARGS || mCurrentContext==CTX_MAIN_ARGS; }

  typedef void (ILTranscriber::*MemberFunction)();
  void CallExitShader() { CallFunction(EXIT_SHADER_FUNC, NULL ); }
  void Null(){}

  enum SpecialAllocMode { ALLOC_NORMAL, ALLOC_PERSISTENT, ALLOC_GLOBAL };
  void SetSpecialAllocMode(SpecialAllocMode v) {mAllocMode=v;}


  // const register related stuff...
  void SetupConstRegisters();
  void SetupDx9ForcedSamplers(int required_samplers);
  static ILRegister ConstIntZero    (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstIntOne     (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstIntNegOne  (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstIntThirtyOne (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstFloatExpFracBitMask (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstFloatExpBitMask (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstFloatSignMask (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstFloatLog2 (RegDT::RegDT dt = RegDT::Float, int slots = 4);
  ILRegister ConstFloatLn2 (RegDT::RegDT dt = RegDT::Float, int slots = 4);
  ILRegister ConstFloatOne (RegDT::RegDT dt = RegDT::Float, int slots = 4);
  ILRegister ConstFloatPi (RegDT::RegDT dt = RegDT::Float, int slots = 4);
  ILRegister ConstFloatHalfPi (RegDT::RegDT dt = RegDT::Float, int slots = 4);
  ILRegister ConstIntThree (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  ILRegister ConstIntTwo (RegDT::RegDT dt = RegDT::Int, int slots = 4);
  static ILRegister Scratch();

  // register allocations
  ILRegister AllocTempRegister();
  ILRegister LookupLiteral(long long unsigned int x, RegDT::RegDT dt = RegDT::Int, int slots = 4);
  virtual ILRegister AllocArrayStructure(int size, ILSpec::ILRegType type = ILSpec::IL_REGTYPE_ITEMP, int regNum = -1);
  
  virtual int AllocResource(const ConstStringPtr& name,
                            ILSpec::ILPixTexUsage usg,
                            int requested_reg);
  bool IsNormResource();
  
  int AllocSampler(const ConstStringPtr& name);

  // reverse lookup from r to integer
  int LookupRegister( BasicILRegister r);
  
  

  /// Add comment token
  virtual void Comment(const std::string& str) = 0;

  void AddInputSemantic(const AST::Modifier& sem, ILSpec::ILInterpMode intrp, ILRegister reg);
  void AddOutputSemantic(const AST::Modifier& sem, ILRegister reg);

  void SetupSemantics();
  void SetupThreadInfo(const AST::FunctionDeclarationPtr main_func);
  void SetupExitShader();


#define DCL_2I1O_OP(intrnl_name, txtsyntx, opcode)             \
  void  intrnl_name (ILRegister d, ILRegister l, ILRegister r) \
  {op_2i_1o((txtsyntx),(ILSpec::ILOpCode)(opcode),d,l,r);}

#define DCL_2I1O_OP_CNTRL(intrnl_name, txtsyntx, opcode, contrl)             \
  void  intrnl_name (ILRegister d, ILRegister l, ILRegister r) \
  {op_2i_1o((txtsyntx),(ILSpec::ILOpCode)(opcode),d,l,r, contrl);}

#define DCL_3I1O_OP(intrnl_name, txtsyntx, opcode)             \
  void  intrnl_name (ILRegister d, ILRegister l, ILRegister r, ILRegister x) \
  {op_3i_1o((txtsyntx),(ILSpec::ILOpCode)(opcode),d,l,r,x);}

#define DCL_MULADDTOD(intrnl_name, txtsyntx, opcode)            \
  void  intrnl_name (ILRegister d, ILRegister l, ILRegister r)  \
  { ILRegister c = d; c.ApplyDstSwizzle(d.SwizzleBytes());      \
  op_3i_1o((txtsyntx),(ILSpec::ILOpCode)(opcode),d,l,r,c);}

#define DCL_1I1O_OP(intrnl_name, txtsyntx, opcode) \
  void  intrnl_name (ILRegister d, ILRegister l)   \
  {op_1i_1o((txtsyntx),(ILSpec::ILOpCode)(opcode),d,l);}

#define DCL_1I1O_OP_CNTRL(intrnl_name, txtsyntx, opcode, control) \
  void  intrnl_name (ILRegister d, ILRegister l)   \
  {op_1i_1o((txtsyntx),(ILSpec::ILOpCode)(opcode),d,l, control);}

#define DCL_1I0O_OP(intrnl_name, txtsyntx, opcode) \
  void  intrnl_name (ILRegister d)                 \
  {op_1i_0o((txtsyntx),(ILSpec::ILOpCode)(opcode),d);}

#define DCL_0I0O_OP(intrnl_name, txtsyntx, opcode) \
  void  intrnl_name ()                             \
  {op_0i_0o((txtsyntx),(ILSpec::ILOpCode)(opcode));}

#define DCL_0I0O_OP_CNTRL(intrnl_name, txtsyntx, opcode, control) \
  void  intrnl_name ()                             \
    {op_0i_0o((txtsyntx),(ILSpec::ILOpCode)(opcode), control);}
  
#define DCL_RESOURCE_1I1O_OP(intrnl_name, txtsyntx, opcode, flags)    \
  void intrnl_name (ILRegister d, int resourceNumber,                 \
                    int samplerNumber, int offset, ILRegister a0)     \
  {op_resource((txtsyntx),(ILSpec::ILOpCode)(opcode), d,              \
               resourceNumber, samplerNumber , flags , offset, a0);}


#define DCL_RESOURCE_2I1O_OP(intrnl_name, txtsyntx, opcode, flags)      \
  void intrnl_name (ILRegister d, int resourceNumber,                   \
                    int samplerNumber, int offset, ILRegister a0,       \
                    ILRegister a1 )                                     \
  {DEBUG_CHECK(a1); op_resource((txtsyntx),(ILSpec::ILOpCode)(opcode),  \
                                d, resourceNumber, samplerNumber ,      \
                                flags ,offset, a0, a1);}


#define DCL_RESOURCE_3I1O_OP(intrnl_name, txtsyntx, opcode, flags)          \
  void intrnl_name (ILRegister d, int resourceNumber,                       \
                    int samplerNumber , int offset, ILRegister a0,          \
                    ILRegister a1 , ILRegister a2)                          \
  {DEBUG_CHECK(a1&&a2); op_resource((txtsyntx),(ILSpec::ILOpCode)(opcode),  \
                                    d, resourceNumber, samplerNumber ,      \
                                    flags,  offset, a0,a1,a2);}

  void ILPSHeader() { op_il_hdr("ps",ILSpec::IL_SHADER_PIXEL); }
  void ILVSHeader() { op_il_hdr("vs",ILSpec::IL_SHADER_VERTEX); }
  void ILGSHeader() { op_il_hdr("gs",ILSpec::IL_SHADER_GEOMETRY); }
  void ILCSHeader() { op_il_hdr("cs",ILSpec::IL_SHADER_COMPUTE); }  

  void BeginFunction(int n) { op_with_int("func", ILSpec::IL_OP_FUNC, n); }
  void Case(int n)          { op_with_int("case", ILSpec::IL_OP_CASE, n); }

  void CallFunction(int n, ConstStringPtr comment) { 
    op_call("call", ILSpec::IL_OP_CALL, comment, n); 
  }

  DCL_0I0O_OP(EndSwitch,    "endswitch",  ILSpec::IL_OP_ENDSWITCH);
  DCL_0I0O_OP(ReturnDyn,    "ret_dyn",    ILSpec::IL_OP_RET_DYN);

  //DCL_0I0O_OP(EndFunction,  "endfunc",    ILSpec::IL_OP_ENDFUNC);
  DCL_0I0O_OP(EndFunction,  "ret",    ILSpec::IL_OP_RET);

  DCL_0I0O_OP(Else,         "else",       ILSpec::IL_OP_ELSE);
  DCL_0I0O_OP(EndIf,        "endif",      ILSpec::IL_OP_ENDIF);
  DCL_0I0O_OP(WhileLoop,    "whileloop",  ILSpec::IL_OP_WHILE);
  DCL_0I0O_OP(EndLoop,      "endloop",    ILSpec::IL_OP_ENDLOOP);
  DCL_0I0O_OP(EndProgram,   "end",        ILSpec::IL_OP_END);
  DCL_0I0O_OP(Break,        "break",      ILSpec::IL_OP_BREAK);
  DCL_0I0O_OP(Continue,     "continue",   ILSpec::IL_OP_CONTINUE);
  DCL_0I0O_OP(EndMain,      "endmain",    ILSpec::IL_OP_ENDMAIN);
  DCL_0I0O_OP(GSCut,        "cut",        ILSpec::IL_OP_CUT);
  DCL_0I0O_OP(GSEmit,       "emit",       ILSpec::IL_OP_EMIT);


  DCL_1I0O_OP(If,          "if_logicalnz",      ILSpec::IL_OP_IF_LOGICALNZ);
  DCL_1I0O_OP(IfNot,       "if_logicalz",       ILSpec::IL_OP_IF_LOGICALZ);
  DCL_1I0O_OP(BreakIf,     "break_logicalnz",   ILSpec::IL_OP_BREAK_LOGICALNZ);
  DCL_1I0O_OP(BreakIfNot,  "break_logicalz",    ILSpec::IL_OP_BREAK_LOGICALZ);
  DCL_1I0O_OP(Switch,      "switch",            ILSpec::IL_OP_SWITCH);

  DCL_1I1O_OP(Move,	"mov",      ILSpec::IL_OP_MOV);
  DCL_1I1O_OP(FAbs,     "abs",      ILSpec::IL_OP_ABS);
  DCL_1I1O_OP(FSign,    "sgn",      ILSpec::IL_OP_SGN);
  DCL_1I1O_OP(Normalize,"nrm",      ILSpec::IL_OP_NRM);
  DCL_1I1O_OP(INegate,  "inegate",  ILSpec::IL_OP_I_NEGATE);
  DCL_1I1O_OP(BitNot,   "inot",     ILSpec::IL_OP_I_NOT);
  DCL_1I1O_OP(IToF,     "itof",     ILSpec::IL_OP_ITOF);
  DCL_1I1O_OP(UToF,     "utof",     ILSpec::IL_OP_UTOF);
  DCL_1I1O_OP(FToI,     "ftoi",     ILSpec::IL_OP_FTOI);
  DCL_1I1O_OP(FToU,     "ftou",     ILSpec::IL_OP_FTOU);
  DCL_1I1O_OP(FToD,     "f2d",      ILSpec::IL_OP_F_2_D)
  DCL_1I1O_OP(DToF,     "d2f",      ILSpec::IL_OP_D_2_F)
  DCL_1I1O_OP(FLog2,    "log_vec",  ILSpec::IL_OP_LOG_VEC);
  DCL_1I1O_OP(FExp2,    "exp_vec",  ILSpec::IL_OP_EXP_VEC);
  DCL_1I1O_OP(FSqrt,    "sqrt_vec",	ILSpec::IL_OP_SQRT_VEC);
  DCL_1I1O_OP(FRsqrt,   "rsq_vec",	ILSpec::IL_OP_RSQ_VEC);
  DCL_1I1O_OP(FSin,     "sin_vec",      ILSpec::IL_OP_SIN);
  DCL_1I1O_OP(FCos,     "cos_vec",      ILSpec::IL_OP_COS);
  DCL_1I1O_OP(FTan,     "tan_vec",      ILSpec::IL_OP_TAN);
  DCL_1I1O_OP(FASin,    "asin",     ILSpec::IL_OP_ASIN);
  DCL_1I1O_OP(FACos,    "acos",     ILSpec::IL_OP_ACOS);
  DCL_1I1O_OP(FATan,    "atan",     ILSpec::IL_OP_ATAN);
  DCL_1I1O_OP(FRound,   "rnd",		ILSpec::IL_OP_RND);
  DCL_1I1O_OP(FCeil,    "round_posinf", ILSpec::IL_OP_ROUND_PLUS_INF);
  DCL_1I1O_OP(FFloor,   "round_neginf", ILSpec::IL_OP_ROUND_NEG_INF);
  DCL_1I1O_OP(FRoundZero,"round_z",	ILSpec::IL_OP_ROUND_ZERO);
  DCL_1I1O_OP(FRsq,	"rsq_vec",      ILSpec::IL_OP_RSQ_VEC);
  DCL_1I1O_OP(FFrac,    "frc",          ILSpec::IL_OP_FRC);
  DCL_1I1O_OP(FDerivX,  "dsx",          ILSpec::IL_OP_DSX);
  DCL_1I1O_OP(FDerivY,  "dsy",          ILSpec::IL_OP_DSY);
  

  DCL_1I1O_OP_CNTRL(FRsqSingleIEEE,  "rsq_zeroop(infinity)",  ILSpec::IL_OP_RSQ,  ILSpec::IL_ZEROOP_INFINITY);

  DCL_2I1O_OP(And,    "and",    ILSpec::IL_OP_AND);
  DCL_2I1O_OP(Or,     "ior",    ILSpec::IL_OP_I_OR);
  DCL_2I1O_OP(Xor,    "ixor",   ILSpec::IL_OP_I_XOR);
  DCL_2I1O_OP(IAdd,   "iadd",   ILSpec::IL_OP_I_ADD);
  DCL_2I1O_OP(IMul,   "imul",   ILSpec::IL_OP_I_MUL);
  DCL_2I1O_OP(IShl,   "ishl",   ILSpec::IL_OP_I_SHL);
  DCL_2I1O_OP(IShr,   "ishr",   ILSpec::IL_OP_I_SHR);
  DCL_2I1O_OP(UShr,   "ushr",   ILSpec::IL_OP_U_SHR);
  DCL_2I1O_OP(UDiv,   "udiv",   ILSpec::IL_OP_U_DIV);
  DCL_2I1O_OP(UMod,   "umod",   ILSpec::IL_OP_U_MOD);
  DCL_2I1O_OP(UMul,   "umul",   ILSpec::IL_OP_U_MUL);
  DCL_2I1O_OP(FAdd,   "add",    ILSpec::IL_OP_ADD);
  DCL_2I1O_OP(FSub,   "sub",    ILSpec::IL_OP_SUB);
  DCL_2I1O_OP_CNTRL(FMult, "mul_ieee",             ILSpec::IL_OP_MUL, IEEE);
  DCL_2I1O_OP_CNTRL(FDiv,  "div_zeroop(infinity)", ILSpec::IL_OP_DIV, ILSpec::IL_ZEROOP_INFINITY);
  DCL_2I1O_OP(FMod,   "mod",    ILSpec::IL_OP_MOD);
  DCL_2I1O_OP(DAdd,   "dadd",   ILSpec::IL_OP_D_ADD);
  DCL_2I1O_OP(DMult,  "dmul",   ILSpec::IL_OP_D_MUL);
  DCL_2I1O_OP(DDiv,  "ddiv",   ILSpec::IL_OP_D_DIV);
  DCL_2I1O_OP(ILt,    "ilt",    ILSpec::IL_OP_I_LT);
  DCL_2I1O_OP(IGe,    "ige",    ILSpec::IL_OP_I_GE);
  DCL_2I1O_OP(IEq,    "ieq",    ILSpec::IL_OP_I_EQ);
  DCL_2I1O_OP(INe,    "ine",    ILSpec::IL_OP_I_NE);
  DCL_2I1O_OP(ULt,    "ult",    ILSpec::IL_OP_U_LT);
  DCL_2I1O_OP(UGe,    "uge",    ILSpec::IL_OP_U_GE);
  DCL_2I1O_OP(FLt,    "lt",     ILSpec::IL_OP_LT);
  DCL_2I1O_OP(FGe,    "ge",     ILSpec::IL_OP_GE);
  DCL_2I1O_OP(FEq,    "eq",     ILSpec::IL_OP_EQ);
  DCL_2I1O_OP(FNe,    "ne",     ILSpec::IL_OP_NE);
  DCL_2I1O_OP(DLt,    "dlt",    ILSpec::IL_OP_DLT);
  DCL_2I1O_OP(DGe,    "dge",    ILSpec::IL_OP_DGE);
  DCL_2I1O_OP(DEq,    "deq",    ILSpec::IL_OP_DEQ);
  DCL_2I1O_OP(DNe,    "dne",    ILSpec::IL_OP_DNE);
  DCL_2I1O_OP(IMin,   "imin",   ILSpec::IL_OP_I_MIN);
  DCL_2I1O_OP(UMin,   "umin",   ILSpec::IL_OP_U_MIN);
  DCL_2I1O_OP(FMin,   "min",    ILSpec::IL_OP_MIN);
  DCL_2I1O_OP(IMax,   "imax",   ILSpec::IL_OP_I_MAX);
  DCL_2I1O_OP(UMax,   "umax",   ILSpec::IL_OP_U_MAX);
  DCL_2I1O_OP(FMax,   "max",    ILSpec::IL_OP_MAX);

  DCL_1I1O_OP(DFRExp,   "dfrexp",    ILSpec::IL_OP_D_FREXP);
  DCL_2I1O_OP(DLDExp,   "dldexp",    ILSpec::IL_OP_D_LDEXP);
  
  
  DCL_2I1O_OP(FCrossProduct, "crs",     ILSpec::IL_OP_CRS);
  DCL_2I1O_OP(FReflect,      "reflect", ILSpec::IL_OP_REFLECT);

  DCL_2I1O_OP_CNTRL(FDot2,   "dp2_ieee",    ILSpec::IL_OP_DP2, IEEE);
  DCL_2I1O_OP_CNTRL(FDot3,   "dp3_ieee",    ILSpec::IL_OP_DP3, IEEE);
  DCL_2I1O_OP_CNTRL(FDot4,   "dp4_ieee",    ILSpec::IL_OP_DP4, IEEE);

  DCL_MULADDTOD(IMulAddDst, "imad", ILSpec::IL_OP_I_MAD);
  DCL_MULADDTOD(UMulAddDst, "umad", ILSpec::IL_OP_U_MAD);
  DCL_MULADDTOD(FMulAddDst, "mad", ILSpec::IL_OP_MAD);
  DCL_MULADDTOD(DMulAddDst, "dmad", ILSpec::IL_OP_D_MULADD);

  DCL_3I1O_OP(FMad, "mad", ILSpec::IL_OP_MAD);
  DCL_3I1O_OP(DMad, "dmad", ILSpec::IL_OP_D_MULADD);

  DCL_3I1O_OP(CMovLogical, "cmov_logical", ILSpec::IL_OP_CMOV_LOGICAL);

  DCL_RESOURCE_1I1O_OP(TextureLoad,   "load_resource(R)I",                ILSpec::IL_OP_LOAD,     TextureLoadFlags::RESOURCE);

  
  DCL_RESOURCE_1I1O_OP(TextureLod,    "lod_stage(R)",                    ILSpec::IL_OP_LOD,      TextureLoadFlags::RESOURCE);


  DCL_RESOURCE_1I1O_OP(TextureSample, "sample_resource(R)_sampler(S)I",
                       ILSpec::IL_OP_SAMPLE,
                       TextureLoadFlags::RESOURCE_SAMPLER_ADDROFF);

  DCL_RESOURCE_2I1O_OP(TextureSampleCmp, "sample_c_resource(R)_sampler(S)I",
                       ILSpec::IL_OP_SAMPLE_C,
                       TextureLoadFlags::RESOURCE_SAMPLER_ADDROFF);

  DCL_RESOURCE_2I1O_OP(TextureSampleCmpLevelZero,
                       "sample_clz_resource(R)_sampler(S)I",
                       ILSpec::IL_OP_SAMPLE_C_LZ,
                       TextureLoadFlags::RESOURCE_SAMPLER_ADDROFF);  

  DCL_RESOURCE_3I1O_OP(TextureSampleG,"sample_g_resource(R)_sampler(S)I", ILSpec::IL_OP_SAMPLE_G, TextureLoadFlags::RESOURCE_SAMPLER_ADDROFF);
  DCL_RESOURCE_2I1O_OP(TextureSampleL,"sample_l_resource(R)_sampler(S)I", ILSpec::IL_OP_SAMPLE_L, TextureLoadFlags::RESOURCE_SAMPLER_ADDROFF);

  //DCL_1I1O_OP(DAbs,      "dabs",     0);
  void DAbs(ILRegister d, ILRegister l)
  {
    l.AddFlags(ILRegister::FLAG_ABS);
    DAdd(d,l,this->ConstIntOne(l.GetDT(),l.GetNumSlots()));
  }
  //DCL_1I1O_OP(DNegate,   "dnegate",  0);
  void DNegate(ILRegister d, ILRegister l)
  {
    l.AddFlags(ILRegister::FLAG_NEGATE);
    DAdd(d,l,this->ConstIntOne(l.GetDT(),l.GetNumSlots()));
  }
  //DCL_2I1O_OP(DSub,      "dsub",     0);
  void DSub(ILRegister d, ILRegister l, ILRegister r)
  {
    r.AddFlags(ILRegister::FLAG_NEGATE);
    DAdd(d,l,r);
  }

  DCL_2I1O_OP(DMin,      "dmin",     0);
  DCL_2I1O_OP(DMax,      "dmax",     0);
  DCL_2I1O_OP(DMod,      "dmod",     0);
  
  void Discard()
  {
    op_1i_0o("discard_logicalz", ILSpec::IL_OP_DISCARD_LOGICALZ, ConstIntZero());
  }

  void FDot(ILRegister d, ILRegister l, ILRegister r)
  {
    switch(Minimum(l.GetNumSlots(), r.GetNumSlots()))
    {
    case 1: FMult(d,l,r); break;
    case 2: FDot2(d,l,r); break;
    case 3: FDot3(d,l,r); break;
    case 4: FDot4(d,l,r); break;
    }
  }

  void IfIntEqual(ILRegister l, ILRegister r)
  {
    ILRegister tmp = Scratch();
    tmp.SetNumSlots(1);
    IEq(tmp,l,r);
    If(tmp);
  }

  void ISub(ILRegister d, ILRegister l, ILRegister r)
  {
    r.AddFlags(ILRegister::FLAG_NEGATE);
    IAdd(d,l,r);
  }

  void IDiv(ILRegister d, ILRegister l, ILRegister r)
  {
    //TODO: This handles divide by zero wrong...

    ILRegister t1 = AllocTempRegister();
    t1.SetDT(d.GetDT());
    t1.SetNumSlots(d.GetNumSlots());
    //And(t1, l, this->ConstFloatExpFracBitMask(l.GetDT(), l.GetNumSlots()));

    ILRegister t2 = AllocTempRegister();
    t2.SetDT(d.GetDT());
    t2.SetNumSlots(d.GetNumSlots());
    //And(t2, r, this->ConstFloatExpFracBitMask(l.GetDT(), l.GetNumSlots()));


    ILRegister temp1 = AllocTempRegister();
    temp1.SetDT(l.GetDT());
    temp1.SetNumSlots(l.GetNumSlots());
    
    ILRegister temp2 = AllocTempRegister();
    temp2.SetDT(l.GetDT());
    temp2.SetNumSlots(l.GetNumSlots());

    Move(temp1, l);
    temp1.SetFlags(ILRegister::FLAG_NEGATE);
    IMax(t1, l, temp1);

    Move(temp2, r);
    temp2.SetFlags(ILRegister::FLAG_NEGATE);
    IMax(t2, r, temp2);

    ILRegister t3 = AllocTempRegister();
    t3.SetDT(d.GetDT());
    t3.SetNumSlots(d.GetNumSlots());
    UDiv(t3, t1, t2);

    ILRegister t4 = AllocTempRegister();
    t4.SetDT(d.GetDT());
    t4.SetNumSlots(d.GetNumSlots());
    INegate(t4, t3);
  
    ILRegister t5 = AllocTempRegister();
    t5.SetDT(d.GetDT());
    t5.SetNumSlots(d.GetNumSlots());
    Xor(t5, l, r);
    And(t5, t5, this->ConstFloatSignMask(l.GetDT(), l.GetNumSlots()));

    CMovLogical(d, t5, t4, t3);
  }

  void IMod(ILRegister d, ILRegister l, ILRegister r)
  {
    //TODO: This handles divide by zero wrong...

    ILRegister temp1 = AllocTempRegister();
    temp1.SetDT(l.GetDT());
    temp1.SetNumSlots(l.GetNumSlots());
    
    ILRegister temp2 = AllocTempRegister();
    temp2.SetDT(l.GetDT());
    temp2.SetNumSlots(l.GetNumSlots());

    ILRegister temp3 = AllocTempRegister();
    temp3.SetDT(l.GetDT());
    temp3.SetNumSlots(l.GetNumSlots());

    ILRegister temp4 = AllocTempRegister();
    temp4.SetDT(l.GetDT());
    temp4.SetNumSlots(l.GetNumSlots());

    Move(temp1, l);
    temp1.SetFlags(ILRegister::FLAG_NEGATE);
    IMax(temp1, l, temp1);

    Move(temp2, r);
    temp2.SetFlags(ILRegister::FLAG_NEGATE);
    IMax(temp2, r, temp2);

    temp1.SetFlags(ILRegister::FLAG_NEGATE / 2);
    temp2.SetFlags(ILRegister::FLAG_NEGATE / 2);
    UMod(d, temp1, temp2);

    INegate(temp3, d);
    
    And(temp4, l, this->ConstFloatSignMask(l.GetDT(), l.GetNumSlots()));
    
    CMovLogical(d, temp4, temp3, d);
  }

  void IAbs(ILRegister d, ILRegister s)
  {
    s.AddFlags(ILRegister::FLAG_ABS);
    Move(d,s);
  }


  void FNegate(ILRegister d, ILRegister s)
  {
    //Xor(d,s,ConstFloatSignMask(s.GetDT(),s.GetNumSlots()));
    s.AddFlags(ILRegister::FLAG_NEGATE);
    Move(d,s);
  }

  void SetCurrentConstBuff(const ConstStringPtr& name)
  {
    mCurrentConstBuff = & mConstBuffs[name];
  }

  //IL Flen is broken
  void FLen(ILRegister d, ILRegister s)
  {
    FDot(d,s,s);
    FSqrt(d,d);
  }

  void SourceWithConstBuff(const ShaderDescription::VariableDesc& var, const std::vector<ILRegister>& regs);

  //IL FLRP is broken
  //void FLrp(ILRegister d, ILRegister a, ILRegister b, ILRegister c)
  //{
  //  op_3i_1o("lrp", ILSpec::IL_OP_LRP, d, a, b, c );
  //}

  void ReadLds(ILRegister d, ILRegister l, int do_fft);
  void WriteLds(ILRegister d, ILRegister l, int offset);
  DCL_0I0O_OP_CNTRL(SyncThreads,  "fence_threads", ILSpec::IL_OP_FENCE, 3 /*threads + lds */);
  
  void SetupConstBuffers();
  
  virtual void DclLiteral( ILRegister lit, long long unsigned int x, RegDT::RegDT dt ) = 0;
  virtual void DclInput(ILSpec::ILImportUsage usg, ILSpec::ILInterpMode interp, ILRegister reg) = 0;
  virtual void DclOutput(ILSpec::ILImportUsage usg, ILRegister reg) = 0;

  int HexWordCount() const { return mHexWordCount; }

  InputPatcher& Patcher() { return mPatcher; }
  const InputPatcher& Patcher() const { return mPatcher; }

  int NumSamplers() const { return mAllocCntrs[CNTR_SAMPLER]; }
  int NumResources() const { return mAllocCntrs[CNTR_RESOURCE]; }

  const ShaderDescriptionPtr& GetShaderDescription() { return mShaderDescription; }

protected:
  //print ascii hex of x to os
  void printHex(unsigned int x);

  ///Two input one output format
  virtual void op_2i_1o(const char* txt , ILSpec::ILOpCode bin, const ILRegister& d, const ILRegister& l, const ILRegister& r, int control = 0) = 0;
  ///Three input one output format
  virtual void op_3i_1o(const char* txt , ILSpec::ILOpCode bin, const ILRegister& d, const ILRegister& a, const ILRegister& b, const ILRegister& c) = 0;
  ///One input one output format
  virtual void op_1i_1o(const char* txt , ILSpec::ILOpCode bin ,ILRegister d, ILRegister l, int control=0) = 0;
  ///One input zero output format
  virtual void op_1i_0o(const char* txt , ILSpec::ILOpCode bin ,ILRegister d) = 0;
  ///Zero in/out format
  virtual void op_0i_0o(const char* txt , ILSpec::ILOpCode bin, int control = 0) = 0;
  ///output il header
  virtual void op_il_hdr(const char* txt, ILSpec::IL_Shader_Type t) = 0;
  ///0i_0o op followed by an int
  virtual void op_with_int(const char* txt, ILSpec::ILOpCode bin, unsigned int x) = 0;
  ///call followed by an int for func id, followed by a comment for func name
  virtual void op_call( const char* txt, 
                        ILSpec::ILOpCode bin, 
                        ConstStringPtr comment, 
                        unsigned int x ) = 0;
  ///One input one output format with a resource in the control field
  virtual void op_resource(const char* txt , ILSpec::ILOpCode bin ,ILRegister d, int resource,
	  int sampler, TextureLoadFlags::TextureLoadFlags flags, 
	  unsigned int offset, ILRegister a0, ILRegister a1 = ILRegister(), ILRegister a2 = ILRegister()) = 0;

  virtual void DclLdsSizeOrMode(ILSpec::ILOpCode bin, int n, ILSpec::IL_LDS_SHARING_MODE m) = 0;  

#if !defined(DISABLE_ILCOMMENT) && !defined(DISABLE_LINENOS)
  void commentLineno();
#else
  inline void commentLineno(){}
#endif


  
protected:
  std::ostream& os;

  SemanticMapT mInputSemantics;
  SemanticMapT mOutputSemantics;
  Context mCurrentContext;

  enum ExtraCounters
  {
    CNTR_RESOURCE = ILSpec::IL_REGTYPE_LAST+1,
    CNTR_SAMPLER,
    CNTR_LAST
  };
  typedef std::pair< ShaderDescription::VariableDesc, std::vector<ILRegister> > ConstBuffEntry;
  typedef std::vector< ConstBuffEntry >ConstBuff;
  typedef std::map< ConstStringPtr, ConstBuff > ConstBuffs;

  ConstBuff* mCurrentConstBuff;
  ConstBuffs mConstBuffs;
  int mAllocCntrs[CNTR_LAST];
  SpecialAllocMode mAllocMode;
  typedef std::map< long long unsigned int, BasicILRegister> LiteralMap;

protected:

  // if this is dx9 and the app has forced sampler numbers
  // 

  // map of dx9 assigned samplers
  int dx9_forced_samplers[16];
  
  LiteralMap mLiterals;
  int mHexWordCount;
  InputPatcher mPatcher;

  ShaderDescriptionPtr mShaderDescription;
};

/**
 * Adds "//" before and '\n' after a user comment
 * Used by the ILCOMMENT macro
 */
class ILCommentWrapper
{
public:
  ///
  /// Constructor
  ILCommentWrapper( ILTranscriber& ts );

  ///
  /// Destruct... pass result back to Transcriber
  ~ILCommentWrapper();

  ///
  /// Pass user comment through
  template< typename T > 
  ILCommentWrapper& operator << ( const T& t)
  {
    os << t;
    return *this;
  }

  static bool IsDisabled();

private:
  std::ostringstream os;
  ILTranscriber& mTranscriber;
};

#endif // AMD_ILTRANSCRIBER_HPP
