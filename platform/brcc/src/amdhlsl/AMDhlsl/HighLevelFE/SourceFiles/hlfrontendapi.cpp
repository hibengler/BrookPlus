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

#include <fstream>
#include <iostream>
#include "hlfrontendapi.hpp"
#include "oscompatibility.hpp"
#include "assertlogging.hpp"
#include "convert.hpp"
#include "parser.hpp"
#include "astroot.hpp"
#include "parameter.hpp"
#include "positionaware.hpp"
#include "astdeclaration.hpp"
#include "astliterals.hpp"
#include "timer.hpp"

//alias some params
PARAMETER_ALIAS("input",   "1"); //first unnamed arg
PARAMETER_ALIAS("input",   "in");
PARAMETER_ALIAS("input",   "i");
PARAMETER_ALIAS("output",  "out");
PARAMETER_ALIAS("output",  "o");
PARAMETER_ALIAS("output",  "2"); //second unnamed arg
PARAMETER_ALIAS("bin",     "binary");
PARAMETER_ALIAS("txt",     "text");
PARAMETER_ALIAS("verbose", "v");


//params declarations

PARAMETER_MUTABLE(p_progname, "0")
PARAMETER_MUTABLE(p_input,    "input");
PARAMETER_MUTABLE(p_ps,       "ps");
PARAMETER_MUTABLE(p_vs,       "vs");
PARAMETER_MUTABLE(p_gs,       "gs");
PARAMETER_MUTABLE(p_cs,       "cs");
PARAMETER_MUTABLE(p_single,   "single");
PARAMETER_MUTABLE(p_dumpall,  "dumpall");
PARAMETER_MUTABLE(p_help,     "help");
PARAMETER_MUTABLE(p_runtests, "runtests");
PARAMETER_MUTABLE(p_output,   "output");
PARAMETER_MUTABLE(p_text,     "text");
PARAMETER_MUTABLE(p_binary,   "binary");
PARAMETER_MUTABLE(p_hex,      "hex");
PARAMETER_MUTABLE(p_mainfunc,   "main");
PARAMETER_MUTABLE(p_dumpsymbols,"dumpsymbols");
PARAMETER_MUTABLE(p_dumpcpp,    "dumpcpp");
PARAMETER_MUTABLE(p_dumptree,   "dumptree");
PARAMETER_MUTABLE(p_select,     "select")
PARAMETER_MUTABLE(p_wordsize,   "wordsize");
PARAMETER_MUTABLE(p_arg3,       "3");
PARAMETER_MUTABLE(p_dumpshaderdesc, "dumpshaderdesc");
PARAMETER_MUTABLE(p_dx9,     "dx9");
PARAMETER_MUTABLE(p_dx10,     "dx10");
PARAMETER_MUTABLE(p_cal,     "cal");

//param descriptions shown in --help message
PARAMETER_DESC("dx9",          "Use dx9 compatibility mode. (Exactly one of -dx9,-dx10,-cal is required.)");
PARAMETER_DESC("dx10",         "Use dx10 compatibility mode. (Exactly one of -dx9,-dx10,-cal is required.)");
PARAMETER_DESC("cal",          "Use cal compatibility mode. (Exactly one of -dx9,-dx10,-cal is required.)");

PARAMETER_DESC("input",       "The input file in AMD-HLSL format.",                                                 Parameter::RequireValue);
PARAMETER_DESC("output",      "The output filename or the prefix to the output filenames.",                         Parameter::RequireValue);
PARAMETER_DESC("select",      "Only compile a given shader from the input file.",                                   Parameter::RequireValue);
PARAMETER_DESC("main",        "Name of main routine.  (default=main)",                                              Parameter::RequireValue);
PARAMETER_DESC("dumptree",    "Output a debugging file containing the syntax tree.",                                Parameter::RequireValue);
PARAMETER_DESC("dumpcpp",     "Output a debugging file containing the output of the preprocessor.",                 Parameter::RequireValue);
PARAMETER_DESC("dumpsymbols", "Output a debugging file containing CSV formatted symbol table.",                     Parameter::RequireValue);
PARAMETER_DESC("dumpall",     "Alias for -dumptree=...dumptree.txt -dumpcpp=...dumpcpp.txt -dumpsymbols=...dumpsymbols.txt", Parameter::AllowValue);
PARAMETER_DESC("runtests",    "Run output through Dev/SCDevUtil",                                                   Parameter::Secret);
PARAMETER_DESC("3",           "Throws an error if the user provides a 3rd parameter",                               Parameter::Secret);
PARAMETER_DESC("ps",          "Output a pixel shader.  (Exactly one of -ps,-vs,-gs, -cs is required.)");
PARAMETER_DESC("vs",          "Output a vertex shader.  (Exactly one of -ps,-vs,-gs, -cs is required.)");
PARAMETER_DESC("gs",          "Output a geometry shader.  (Exactly one of -ps,-vs,-gs, -cs is required.)");
PARAMETER_DESC("cs",          "Output a compute shader.  (Exactly one of -ps,-vs,-gs, -cs is required.)");
PARAMETER_DESC("text",        "Output in text IL format.  (Exactly one of -text,-binary,-hex is required.)");
PARAMETER_DESC("binary",      "Output in binary IL format.  (Exactly one of -text,-binary,-hex is required.)");
PARAMETER_DESC("hex",         "Output in hex IL format.  (Exactly one of -text,-binary,-hex is required.)");
PARAMETER_DESC("help",        "Show this message.");
PARAMETER_DESC("single",      "Indicates input is a single file without a 'Shader:...' prefix.");
PARAMETER_DESC("nocomments",  "Omit all comments from output file.");
PARAMETER_DESC("nolinenos",   "Omit '; filename:123' comments from output file.");
PARAMETER_DESC("verbose",     "Enables verbose output to terminal.");
PARAMETER_DESC("strict",      "Disable support for fxc style 'sloppy' type checking and case-insensitive modifiers/semantics.");
PARAMETER_DESC("wordsize",    "Prepend 'wordsize=...' to hex output file.  Must be used with -hex. Not compatable with output to pipe or stdout.");


namespace  //file local namespace
{
  template < typename T , int IO_FLAGS >
  class FileStream : public T , public RefCountedObj
  {
    static int getFileFlags() { return IO_FLAGS | (p_binary ? std::ios::binary : 0); };
  public:
    FileStream(const std::string& fn)
      : T( fn.c_str(), (std::ios::openmode) getFileFlags() )
      , filename(fn)
    {
      ASSERT_ERROR( T::is_open() )(filename)
        .Text("failed to open file");
    }

    const std::string filename;
  };

  typedef FileStream< std::ifstream, std::ios::in >  IFileStream;
  typedef FileStream< std::ofstream, std::ios::out > OFileStream;
  typedef SmartPtr<IFileStream> IFileStreamPtr;
  typedef SmartPtr<OFileStream> OFileStreamPtr;

  class HLFrontend 
  {
  public:

    //called from main
    void Run()
    {
      mInput = new IFileStream(p_input);

      if(p_runtests)
      {
        runOutputTest();
      }
      else if(p_single)
      {
        CompileSingleBuffer(*mInput, getOutput("") );
      }
      else
      {
        SetInputStream(mInput.AsPtr());
        compileMultiShaderInput();
        SetInputStream(NULL);
      }
      cleanupGlobalState();
    }

    ConstShaderDescriptionPtr CompileSingleBuffer(std::istream& input, std::ostream& out)
    {
      ConstShaderDescriptionPtr sd;
      SetInputStream(&input);
      sd = compileToBuffer(out);
      SetInputStream(NULL);
      cleanupGlobalState();
      return sd;
    }


  protected:


    void compileMultiShaderInput()
    {
      int shaderCount = 0;
      for(;;)
      {
        const std::string& name = PreprocScrollToNextShader();
        if(name=="")
        {
           if (!shaderCount)
           {
              NOTICE("no shader: keyword found in input");
           }
           break;
        }
        compileToBuffer(getOutput(name));          
        shaderCount++;
      }
    }

    ConstShaderDescriptionPtr compileToBuffer(std::ostream& out)
    {
      ConstShaderDescriptionPtr shaderDesc;

      BEGIN_TIMER("compile");

      cleanupGlobalState();

      //parse shader
      BEGIN_TIMER("compile_parse")
        zzparse();     
      END_TIMER

      ASSERT_ERROR(!theParserHadError);

      //now do process and export
      shaderDesc = AST::Root::TheCurrentTreeRoot()->CompileShaderToStream( out );

      END_TIMER

      if(p_dumpshaderdesc)
      {
        ASSERT_ERROR(p_dumpshaderdesc.HasValue())(p_dumpshaderdesc).Text("usage: -dumpshaderdesc=filename");
        BEGIN_TIMER("dumpshaderdesc")
        shaderDesc->SerializeAsText(p_dumpshaderdesc.ValueStr());
        END_TIMER
      }

      return shaderDesc;
    }

    ///Get a handle to the output file... open it if needed
    std::ostream& getOutput(const std::string& shader)
    {
      if(p_text)
      {
        if(!mOutput.IsValid())
        {
          mOutput = new OFileStream(p_output);
        }
        if(!p_single) *mOutput << std::endl << "shader: " << shader << std::endl;
      }
      else 
        mOutput = new OFileStream(shader==""||p_select ? p_output : (p_output + "_" + shader));

      return *mOutput;
    }

    void runDev(const std::string& args)
    {
      int rv = system(("dev -Pele -q -nt -ns -X "+args).c_str());
      ASSERT_ERROR(rv==0)(args).Text("call to dev failed");
    }   
    void runScDevUtil(const std::string& file1,const std::string& file2)
    {
      int rv = system(("scdevutil -ildiff "+file1+" "+file2).c_str());
      if(rv==0)
        NOTICE("Results match!!!");
      else 
        DIE("Results different!!!");
    }

    /// Compile as all output formats, then translate between them
    void runOutputTest()
    {
      ASSERT_WARNING(!p_dumpcpp)(p_dumpcpp).Text("-dumpcpp not supported with --runtests");
      //p_dumpcpp.Enable();
      p_hex.Enable();
      p_wordsize.Enable();
      SetInputStream(mInput.AsPtr());

      for(;;)
      {       
        const std::string& name = PreprocScrollToNextShader();
        if(name=="") break;

        OFileStream ilhex("hlf_tokens");
        compileToBuffer(ilhex);
        ilhex.flush();
        ilhex.close();

        runDev(p_input.Value() + " -select " + name);
        runScDevUtil("il_tokens", ilhex.filename);
      }

        //now some translations
//         runXlateTool(p_input + "2txt",     p_input + "2txt2bin",     "txt2bin");
//         runXlateTool(p_input + "2txt2bin", p_input + "2txt2bin2txt", "bin2txt");
//         runXlateTool(p_input + "2bin",     p_input + "2bin2txt",     "bin2txt");
//         runXlateTool(p_input + "2hex",     p_input + "2hex2txt",     "hex2txt");
    }

    void runXlateTool(const std::string& i, const std::string& o, const char* mode )
    {
      std::ostringstream cmd;
      cmd << "ILTextTranslator.exe " << i << ' ' << o << ' ' << mode << "";
      int r = pclose(popen(cmd.str().c_str(), "r"));
      ASSERT_WARNING(r==0)(r)(cmd.str())
        .Text("ILTextTranslator call returned non zero");
    }

    void cleanupGlobalState()
    {
      AST::Root::TheCurrentTreeRoot() = 0;
      AST::FunctionDeclaration::ResetGlobalState();
    }

  private:
    OFileStreamPtr mOutput;
    IFileStreamPtr mInput;
  };


  static void printUsage()
  {
    std::cerr << "USAGE: " << std::endl << std::endl;
    std::cerr << p_progname.Value() << " OPTIONS [<arg1> [<arg2> [...]]" << std::endl << std::endl;
    std::cerr << "EXAMPLES: " << std::endl << std::endl;
    std::cerr << p_progname.Value() << " -ps -txt -dx10 -input=thisfile -output=thatfile" << std::endl << std::endl;
    std::cerr << p_progname.Value() << " -cs -txt -cal -input=thisfile -output=thatfile" << std::endl << std::endl;    
    std::cerr << "OPTIONS: " << std::endl << std::endl;
    Parameter::PrintHelpText(std::cerr); 
  }

  static void doStdArgHandling()
  {
    //deal with some params
    if(p_help || !p_input || p_arg3) 
    { 
      printUsage();
      throw 1;
    }
    // do all the checks for arguments here
    // set up defaults for missing arguments 

    ASSERT_ERROR(p_input)(p_input).Text("-input=... is required");
    ASSERT_ERROR(p_text.IsEnabled()+p_binary.IsEnabled()+p_hex.IsEnabled()+p_runtests.IsEnabled() == 1)
      (p_text)(p_binary)(p_hex)(p_runtests)
      .Text("Exactly one output type (-text, -binary, or -hex) must be specified");
    ASSERT_ERROR(p_ps.IsEnabled()+p_vs.IsEnabled()+p_gs.IsEnabled()+p_cs.IsEnabled() == 1)(p_ps)(p_vs)(p_gs)(p_cs)
      .Text("Exactly one of -ps, -vs, -gs, or -cs  must be specified ");

    ASSERT_ERROR(p_dx9.IsEnabled()+p_dx10.IsEnabled()+p_cal.IsEnabled() == 1)(p_dx9)(p_dx10)(p_cal)
      .Text("Exactly one of -dx9, -dx10, or -cal must be specified ");
        
    if(!p_mainfunc) p_mainfunc = "main";
    if(!p_output) p_output = p_input + ".il";
    if(p_dumpall)
    {
      p_dumptree       = p_dumpall + "dumptree.txt";
      p_dumpcpp        = p_dumpall + "dumpcpp.txt";
      p_dumpsymbols    = p_dumpall + "dumpsymbols.txt";
      p_dumpshaderdesc = p_dumpall + "dumpshaderdesc.txt";
    }

    ASSERT_ERROR(p_hex || !p_wordsize)(p_hex)(p_wordsize).Text("-wordsize only supported when running in hex mode");
  }

}//file local namespace


bool HLFrontendRun(int argc, const char** argv)
{
  try
  {
    //load args
    Parameter::Populate(argc,argv);
    doStdArgHandling();

    //run program
    HLFrontend().Run();
    return true;
  }
  catch (...)
  {
    return false;
  }
}

bool HLFrontendCompileBuffer( std::istream& input , std::ostream& output , HLFrontendFlags::ShaderType type , HLFrontendFlags::OutputFormat fmt /*= HLFrontendFlags::OUTPUT_BIN*/, ConstShaderDescriptionPtr* shaderDesc /*= NULL*/)
{
  using namespace HLFrontendFlags;
  try
  {
    //setup args
    if(type!=TYPE_FROMPARAMS)
    {
      if(type==TYPE_PS) p_ps.Enable(); else p_ps.Disable();
      if(type==TYPE_VS) p_vs.Enable(); else p_vs.Disable();
      if(type==TYPE_GS) p_gs.Enable(); else p_gs.Disable();
      if(type==TYPE_CS) p_cs.Enable(); else p_cs.Disable();      
    }
    if(fmt!=OUTPUT_FROMPARAMS)
    {
      if(fmt==OUTPUT_TXT) p_text.Enable();   else p_text.Disable();
      if(fmt==OUTPUT_BIN) p_binary.Enable(); else p_binary.Disable();
      if(fmt==OUTPUT_HEX) p_hex.Enable();    else p_hex.Disable();
    }
    if(!p_input)  p_input = "<buffer>";
    if(!p_output) p_output= "<buffer>";
    doStdArgHandling();

    //run program
    ConstShaderDescriptionPtr sd = HLFrontend().CompileSingleBuffer(input,output);
    if(shaderDesc!=0) *shaderDesc = sd;
    return true;
  }
  catch (...)
  {
    return false;
  }
}

void HLFrontendResetParameters()
{
  Parameter::ResetAllParameterState();
}

void HLFrontendSetParameter( const char* name, bool enabled, const char* val /*= ""*/)
{
  Parameter& pv = Parameter::LookupMutable(name);
  pv.SetValue(val);
  if(enabled)
    pv.Enable();
  else 
    pv.Disable();
}

void HLFrontendSetErrorMessageStream( std::ostream* es /*= std::cerr*/ )
{
  AssertLoggingSetOutputStream(es);
}

void HLFrontendParseFXCArgs( const std::string& args )
{
  std::istringstream is(args);
  for( std::string arg; is>>arg && arg.length()>0; arg="")
  {
    if(StartsWith(arg, "/T")) //profile
    {
      arg = arg.substr(2,2);
      if(arg=="ps") p_ps.Enable(); else p_ps.Disable();
      if(arg=="vs") p_vs.Enable(); else p_vs.Disable();
      if(arg=="gs") p_gs.Enable(); else p_gs.Disable();
      if(arg=="cs") p_cs.Enable(); else p_cs.Disable();      
    }
    if(StartsWith(arg, "/E")) //entry point
    {
      arg = arg.substr(2);
      p_mainfunc = arg;
    }
    if(StartsWith(arg, "/D")) //entry point
    {
      //arg = arg.substr(2);
      WARNING_UNIMPLEMENTED()(arg);
    }
  }
}

