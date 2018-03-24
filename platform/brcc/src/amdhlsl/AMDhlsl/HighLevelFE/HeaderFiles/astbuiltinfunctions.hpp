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

#ifndef AMD_ASTBULTINFUNCTIONS_HPP
#define AMD_ASTBULTINFUNCTIONS_HPP

#include "astoperators.hpp"
#include "parameter.hpp"
#include "astroot.hpp"

// p_dx9 is the local to this file name for the parameter identifed by the
// the string "dx9"
PARAMETER(p_dx9, "dx9" );
  
namespace AST
{

  /**
   * The glue that puts builtin functions into the syntax tree                                                                    
   */
  template <int numArgs>
  class BuiltinFunction : public Operation
  {
  public:
    virtual std::string FunctionName() const = 0;
    virtual const AST::DataTypePtr& EffectiveDataType();
    int PostExportToIL(TreeElementPtr& /*self*/, ILTranscriber* il);
    virtual bool IsDestOnlyBuiltinFunction() {return false;} 
  protected:
    //dispatch to the right builtinImpl based on numArgs
    virtual void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args);
    //subclass is expected to reimplement one of the following
    virtual void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0);
    virtual void builtinImpl(ILTranscriber* il, ILRegister& ret, const ILRegister& arg0, const ILRegister& arg1);
    virtual void builtinImpl(ILTranscriber* il, ILRegister& ret, ILRegister& arg0, ILRegister& arg1, ILRegister& arg2);

    virtual void typeCheck(ILTranscriber* /*il*/, ILRegister& /*t*/){}
    virtual void typeCheck(ILTranscriber* /*il*/, ILRegister& /*t1*/, ILRegister& /*t2*/){}
    virtual void checkArgCount() { ASSERT_ERROR(NumChildren()==numArgs)(FunctionName())(numArgs)(NumChildren()).Text("wrong number of arguments to builtin function"); }

  };

  template <int numArgs>
  class FloatsOnlyBuiltinFunction : public BuiltinFunction<numArgs>
  {
  public:
    virtual void typeCheck(ILTranscriber* il, ILRegister& t);
    virtual void typeCheck(ILTranscriber* il, ILRegister& t1, ILRegister& t2);
    virtual const AST::DataTypePtr& EffectiveDataType();;
  };

  template <RegDT::RegDT DT, int SLOTS = -1, typename Base = BuiltinFunction<1> > 
  class ReturnFixedTypeBuiltinFunction : public Base
  {
    virtual const AST::DataTypePtr& EffectiveDataType()
    {
      int slots = SLOTS>0 ? SLOTS : BuiltinFunction<1>::EffectiveDataType()->GetRegSlots();
      static DataTypePtr dt;
      dt = DataType::LookupSimpleType(DT, slots);
      return dt;
    };
  };

  template <int RES_ARG_ID, int SAM_ARG_ID, int NUM_ARGS, int NUM_ARGS_ALT>
  class TextureBuiltinFunction : public BuiltinFunction<NUM_ARGS>
  {
  public:
    virtual const AST::DataTypePtr& EffectiveDataType()
    {
      if(this->mChildElements.size()>0)
      {
        const SymbolPtr& e = this->mChildElements[0]->GetResultSymbol();
        if(e.IsValid() && e->Type()->IsResourceType())
        {
          const DataTypePtr& st = e->AsResourceSymbol().GetSubType();
          if(st.IsValid())
            return st;          
        }
      }

      static DataTypePtr float4t = DataType::LookupSimpleType(RegDT::Float, 4);
      return float4t;
    } 

    virtual void builtinImpl(ILTranscriber* il, const SymbolPtr& ret, const SymbolPtrList& args)
    {
      ILRegister retReg = ret->AsBasicSymbol().GetRegister();
      
      checkArgCount();
      SymbolPtr resource;
      SymbolPtr sampler;
      SymbolPtrList restArgs;
      int offset  = 0;
      int num_ints = 0;
      int numArgs = static_cast<int>(args.size());
      TreeElementPtr p, p1;
      TreeElement::ConstIterator it;
      SymbolPtr s;
      for(int i=0; i<numArgs; ++i) {
        switch(i){
          case RES_ARG_ID:
            resource = args[i];
            break;
        case SAM_ARG_ID:
          sampler = args[i];
          break;
        case (NUM_ARGS_ALT-1):
        {
          // there are two cases - dx10 style takes a literal
          // dx 9 does not
          // \todo we should introduce a real dx9 compatability mode
           
          if(SAM_ARG_ID==-1){  // indicates dx9
            restArgs.push_back(args[i]);
          } else {
            // extra argument which must be a literal or const int
            p = this->mChildElements[i];
            s = p->GetResultSymbol();
            
            if (s->IsExplicitlyDefined()){
              p = s->GetInitialValue();
            }
            if(p->CanSimplifyToInt()){
              offset |= ((p->SimplifyToInt()) & 0xff) << (8*(num_ints++));
            } else {
              // p should be a typecaster to an int of length 1,2, or 3
              // or a const int symbol 
              for(it = p->Begin(); it != p->End(); ++it){
                p1 = *it;
                offset |= ((p1->SimplifyToInt()) & 0xff) << (8*(num_ints++));
              }
            }
            //create a tmp value  
            BasicSymbol *b = new BasicSymbol( p->EffectiveDataType(), 0, 0);
            ILRegister r = il->LookupLiteral(offset);
            b->SetRegister(r);
            restArgs.push_back(b);
          }
        }
        break;
        default:
          restArgs.push_back(args[i]);
        }
      }
      textureExpand(il, retReg, resource, sampler, restArgs);
    }

    ///recursively reduce resource/sampler to constant
    // if either the resource or the sampler is not a constant,
    // expand each sample
    // to a set of samples with if tests around them,
    // hope that sc will clean all this up
    // in dx9 mode the samplers and resources are the same,
    // and no code can acutally talk about a resource, 

    // for dx9 the combined sampler resource number is passed in the
    // resource field
    void textureExpand( ILTranscriber*      il
                    , const ILRegister&     ret
                    , const SymbolPtr&      resource
                    , const SymbolPtr&      sampler
                    , const SymbolPtrList&  args)
    {

      
      if(resource.IsValid() && resource->AsResourceSymbol().IsDynamicResource()){
        //brute force make resource a constant
        ILCOMMENT(il) << "resource id " << resource->AsResourceSymbol().GetDynamicResourceId().ToString() << " is unknown... switched expansion:";
        SymbolPtr tmp = resource->Clone();
        for(int ki=0; ki<il->NumResources(); ++ki) {
          int i = ki;
          // in dx9_forced mode we need to look up the real number sampler id
          i = il->RecodeSampler(ki,
                               AST::Root::TheCurrentTreeRoot()->dx9_samplers);

          il->IfIntEqual(resource->AsResourceSymbol().GetDynamicResourceId(), il->LookupLiteral(i,RegDT::Int, 1));
          tmp->AsResourceSymbol().SetStaticResourceId(i);
          if (p_dx9) {
            textureExpand(il,ret,tmp,tmp,args);
          } else {
            textureExpand(il,ret,tmp,sampler,args);

          }
          il->EndIf();
        }
      return;
      } // valid resource

      if(sampler.IsValid() && sampler->AsResourceSymbol().IsDynamicResource()) {
        //brute force make sampler a constant
        ILCOMMENT(il) << "sampler id " << sampler->AsResourceSymbol().GetDynamicResourceId().ToString() << " is unknown... switched expansion:";
        SymbolPtr tmp = sampler->Clone();
        for(int i=0; i<il->NumSamplers(); ++i) {
          il->IfIntEqual(sampler->AsResourceSymbol().GetDynamicResourceId(), il->LookupLiteral(i,RegDT::Int, 1));
          tmp->AsResourceSymbol().SetStaticResourceId(i);
          textureExpand(il,ret,resource,tmp,args);
          il->EndIf();
        }
	  return;
      } // sampler not a int


      int r=0, s=0;
      
      if(resource.IsValid()) r = resource->AsResourceSymbol().ResourceId();
      if(sampler.IsValid()) s = sampler->AsResourceSymbol().ResourceId();

      textureExpand( il, ret, r, s, args);
    }

    virtual void textureExpand( ILTranscriber*   il
                                , const ILRegister& ret
                                , int resource
                                , int sampler
                                , const SymbolPtrList&  args) = 0;
    
    virtual void checkArgCount() { ASSERT_ERROR(this->NumChildren()==NUM_ARGS||this->NumChildren()==NUM_ARGS_ALT)(this->FunctionName())(this->NumChildren()).Text("wrong number of arguments to builtin function"); }



  };

  TreeElementPtr LookupBuiltinFunction(const ConstStringPtr& name);

}

#endif // AMD_ASTBULTINFUNCTIONS_HPP
