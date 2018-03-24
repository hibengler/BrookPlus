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

#include "astroot.hpp"
#include "astdeclaration.hpp"
#include "iltranscriber.hpp"
#include "parameter.hpp"
#include "iltexttranscriber.hpp"
#include "iltokentranscriber.hpp"
#include "astliterals.hpp"
#include "timer.hpp"
#include <fstream>

AST::RootPtr& 
AST::Root::TheCurrentTreeRoot()
{
  static RootPtr inst;
  return inst;
}

PARAMETER(p_dumptree, "dumptree");
PARAMETER(p_ps,       "ps");
PARAMETER(p_vs,       "vs");
PARAMETER(p_gs,       "gs");
PARAMETER(p_cs,       "cs");
PARAMETER(p_mainfunc, "main");
PARAMETER(p_text,     "text");
PARAMETER(p_binary,   "binary");
PARAMETER(p_hex,      "hex");
PARAMETER(p_wordsize, "wordsize");
#ifndef DISABLE_INPUTPATCHER
PARAMETER(p_forcelisting, "forcelisting");
#endif

static std::ostream& theDumpTreeStream()
{
  static std::ofstream of(p_dumptree.ValueStr());
  return of;
}

///
/// 1) Resolves DataTypes
/// 2) Resolves Symbols
/// 3) Simplifies tree
/// 4) Exports IL representation to os
ConstShaderDescriptionPtr AST::Root::CompileShaderToStream( std::ostream& os )
{
  try
  {
    SymbolTable globalScope;
    DataTypeTable dataTypes(DataType::TheBuiltinDataTypes());
    TreeElementPtr self = this;
    int errorCount;
    ConstShaderDescriptionPtr shaderDesc = NULL;

    
    BEGIN_TIMER("compile_resolvedatatypes")

    errorCount = WalkTree< DataTypeTable
                          , &AST::TreeElement::ResolveDataTypes
                          , &AST::TreeElement::NullPostAction<DataTypeTable> >
                        (self, &dataTypes);
    ASSERT_ERROR(errorCount==0)(errorCount).Text("Unknown DataTypes exist");

    END_TIMER
    BEGIN_TIMER("compile_resolvesymboles")

    errorCount  = WalkTree< SymbolTable
                          , &AST::TreeElement::ResolveSymbols
                          , &AST::TreeElement::NullPostAction<SymbolTable> >
                        (self, &globalScope);
    ASSERT_ERROR(errorCount==0)(errorCount).Text("Unknown Symbols exist");


    END_TIMER
    
        if ( p_dumptree )
        {
            BEGIN_TIMER("dumptree")
            theDumpTreeStream() << "TreeDump after ResolveSymbols: " << std::endl;
            DebugPrint(theDumpTreeStream());
            theDumpTreeStream() << std::endl;
            END_TIMER
        }
//     /*int simplifications =*/ WalkTree< TreeElement
//                                       , &AST::TreeElement::PreSimplifyTree
//                                       , &AST::TreeElement::PostSimplifyTree>
//                                     (self);

    int wordsizePos = 0;
    if(p_wordsize)
    {
      os << "wordsize=";
      wordsizePos = os.tellp();
      os << "             \n"; //leave enough room to insert a number later
    }

    ILTranscriberPtr il = NULL;

    BEGIN_TIMER("compile_allocregs")

    if(p_text)        il = new ILTextTranscriber(os);
    else if(p_binary) il = new ILBinaryTokenTranscriber(os);
    else if(p_hex)    il = new ILHexTokenTranscriber(os);
    else ERROR_UNIMPLEMENTED();
    
    if(p_ps)      il->ILPSHeader();
    else if(p_vs) il->ILVSHeader();
    else if(p_gs) il->ILGSHeader();
    else if(p_cs) il->ILCSHeader();    
    else ERROR_UNIMPLEMENTED();

#ifndef DISABLE_INPUTPATCHER
    if(p_forcelisting && p_forcelisting.HasValue())
      il->Patcher().ParseFxcListing(p_forcelisting);
#endif
    il->SetupConstRegisters();
    il->SetupDx9ForcedSamplers(AST::Root::TheCurrentTreeRoot()->dx9_samplers);

    il->SetCurrentContext(ILTranscriber::CTX_GLOBAL);
    //globalScope.AllocateRegisters(il);


    errorCount += WalkTree< ILTranscriber
                          , &AST::TreeElement::PreAllocateRegisters
                          , &AST::TreeElement::PostAllocateRegisters >
                        (self, il.AsPtr());

    END_TIMER
    BEGIN_TIMER("compile_exportil")

        if ( p_dumptree )
        {
            BEGIN_TIMER("dumptree")
            theDumpTreeStream() << "TreeDump after AllocateRegisters: " << std::endl;
            DebugPrint(theDumpTreeStream());
            theDumpTreeStream() << std::endl;
            END_TIMER
        }

    //Call main
    SymbolPtr main = globalScope.Lookup(ConstString::Lookup(p_mainfunc));
    ASSERT_ERROR(main!=0)(p_mainfunc).Text("Failed to find main()");
    il->SetupThreadInfo(main->AsFunctionSymbol().ResolveOverloads());
    il->SetupConstBuffers();
    il->SetupSemantics();
    il->CallFunction(main->AsFunctionSymbol().ResolveOverloads()->FunctionId(),
                     main->AsFunctionSymbol().ResolveOverloads()->Name());
    il->CallExitShader();
    il->EndMain();
    il->SetupExitShader();

   

    errorCount += WalkTree< ILTranscriber
                          , &AST::TreeElement::PreExportToIL
                          , &AST::TreeElement::PostExportToIL>
                        (self, il.AsPtr());

    il->EndProgram();

    
    if(p_wordsize)
    { //go back and fix wordsize=
      int endPos = os.tellp();
      os.seekp(wordsizePos);
      os << il->HexWordCount();
      os.seekp(endPos);
    }

    shaderDesc = il->GetShaderDescription().AsPtr();

    END_TIMER

    ASSERT_ERROR(errorCount==0)(errorCount).Text("Errors found");

    if(p_dumptree)
    { 
      BEGIN_TIMER("dumptree")
      theDumpTreeStream() << "TreeDump: "  <<  std::endl;
      DebugPrint(theDumpTreeStream());
      theDumpTreeStream() <<  std::endl;
      END_TIMER
    }

    return shaderDesc;
  }
  catch (...)
  {
    if(p_dumptree)
    {
      theDumpTreeStream() << "TreeDump: (Triggered by Error) "  <<  std::endl;
      DebugPrint(theDumpTreeStream());
    }
    throw;
  }
}


