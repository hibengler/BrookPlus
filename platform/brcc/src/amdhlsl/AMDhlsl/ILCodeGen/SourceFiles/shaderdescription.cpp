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

#include "shaderdescription.hpp"

#define SDFILEFORMATVERSION "1.0"

#include <fstream>
#include <sstream>
#include "assertlogging.hpp"

namespace
{
  class SDSerializer
  { 
  protected:
    virtual void serialize( ConstStringPtr& str ) = 0;
    virtual void serialize( int& i ) = 0;
    virtual void beginElement() { ++mDepth; }
    virtual void endElement() { --mDepth; }

    void serialize( bool& b )
    {
      STATIC_CONSTSTRING(t,"true");
      STATIC_CONSTSTRING(f,"false");
      ConstStringPtr v = b ? t : f;
      serialize(v);
      ASSERT_ERROR(v==t || v==f)(v)
        .Text("expected true or false in ShaderDescription file format");
      b = (v==t);
    }

    template< typename T >
    void serialize( const ConstStringPtr& name, std::vector<T>& vec )
    {
      beginElement();
      { //we do this just for readability
        ConstStringPtr tmp_name = name;
        serialize(tmp_name);
        ASSERT_ERROR(tmp_name==name)(tmp_name)(name)
          .Text("problem in ShaderDescription file format");
      }
      int sz = (int)vec.size(); 
      serialize(sz); //either read or write sz
      vec.resize(sz); //if it changed... record change
      endElement();
      ++mDepth;
      //recursively serialize each element
      for(int i=0; i<sz; ++i)
        serialize( vec[i] );
      --mDepth;
    }

    void serialize( ShaderDescription::SemanticDesc& v )
    {
      //beginElement();
      serialize(v.Name);
      serialize(v.Number);
      //endElement();
    };

    void serialize( ShaderDescription::DataTypeDesc& v )
    {
      //beginElement();
      serialize(v.Name);
      serialize(v.IsUserDefinedType);
      serialize(v.NumArrayElements);
      serialize(v.ElementSizeInRegisters);
      //endElement();
    };

    void serialize( ShaderDescription::VariableDesc& v )
    {
      //beginElement();
      serialize(v.Name);
      serialize(v.DataType);
      //endElement();
    };

    void serialize( ShaderDescription::RegisterDesc& v )
    {
      beginElement();
      serialize(v.RegisterNumber);
      serialize(v.RegisterMask[0]);
      serialize(v.RegisterMask[1]);
      serialize(v.RegisterMask[2]);
      serialize(v.RegisterMask[3]);
      endElement();
    };

    void serialize( ShaderDescription::InputOutputVariableDesc& v )
    {
      STATIC_CONSTSTRING(regStr, "Registers");
      beginElement();
      serialize(v.Variable);
      serialize(v.Semantic);
      serialize(regStr,v.Registers);
      serialize(v.ImportUsage);
      endElement();
    };

    void serialize( ShaderDescription::ConstantBufferFieldDesc& v )
    {
      beginElement();
      serialize(v.Variable);
      serialize(v.ByteOffset);
      serialize(v.ByteSize);
      endElement();
    };

    void serialize( ShaderDescription::ConstantBufferDesc& v )
    {
      STATIC_CONSTSTRING(fldStr, "Fields");
      beginElement();
      serialize(v.Name);
      serialize(v.BufferNumber);
      serialize(fldStr,v.Fields);
      endElement();
    };

    void serialize( ShaderDescription::TextureDesc& v )
    {
      beginElement();
      serialize(v.Variable);
      serialize(v.ElementType);
      serialize(v.ResourceNumber);
      endElement();
    };


    void serialize( ShaderDescription::SamplerDesc& v )
    {
      beginElement();
      serialize(v.Variable);
      serialize(v.SamplerNumber);
      endElement();
    };

    void serialize( ShaderDescription::FunctionDesc& v )
    {
      beginElement();
      serialize(v.Name);
      serialize(v.IsEntryPoint);
      serialize(v.NumRegisterArgs);
      serialize(v.NumRegisterArgsSemanticOrUniform);
      endElement();
    };

  public:

    void serializeSD( ShaderDescription& v )
    {
      STATIC_CONSTSTRING(sdStr,   "ShaderDescription-v" SDFILEFORMATVERSION);
      STATIC_CONSTSTRING(inptStr, "Inputs");
      STATIC_CONSTSTRING(otptStr, "Outputs");
      STATIC_CONSTSTRING(consStr, "ConstantBuffers");
      STATIC_CONSTSTRING(textStr, "Textures");
      STATIC_CONSTSTRING(sampStr, "Samplers");
      STATIC_CONSTSTRING(funcStr, "Functions");
      beginElement();
      ConstStringPtr tmp;
      tmp = sdStr;
      serialize(tmp);
      ASSERT_ERROR(tmp == sdStr)(tmp)(sdStr)
        .Text("error in ShaderDescription file format");
      serialize(inptStr,v.Inputs);
      serialize(otptStr,v.Outputs);
      serialize(consStr,v.ConstantBuffers);
      serialize(textStr,v.Textures);
      serialize(sampStr,v.Samplers);
      serialize(funcStr,v.Functions);
      endElement();
    };

    SDSerializer() : mDepth(0) {}
  protected:
    int mDepth;
  };

  class SDSerializerWriteText : public SDSerializer
  {
    virtual void serialize( ConstStringPtr& str ) 
    { 
      if(str.IsValid() && str->size()>0)
        os << *str << ' '; 
      else
        os << "(null) ";
    }
    virtual void serialize( int& i )
    { 
      os << i << ' '; 
    }
    virtual void beginElement()                   
    {
      os << '\n';
      for(int i=0; i<mDepth; ++i)
        os << "  ";
      SDSerializer::beginElement();
    }
  public:
    SDSerializerWriteText(std::ostream& _os) : os(_os) {}
  private:
    std::ostream& os;
  };

  class SDSerializerReadText : public SDSerializer
  {
    virtual void serialize( ConstStringPtr& str ) 
    { 
      std::string tmp;
      is >> tmp;
      str = ConstString::Lookup(tmp);
    }
    virtual void serialize( int& i )               
    {
      is >> i; 
    }
    // virtual void beginElement()                   
    // { //todo: add error checking sometime in future
    //    ASSERT(at end of line)
    //    ASSERT(mDepth*2 spaces prefix next line)
    // }
  public:
    SDSerializerReadText(std::istream& _is) : is(_is) {}
  private:
    std::istream& is;
  };

  class SDSerializerWriteBinary : public SDSerializer
  {
    virtual void serialize( ConstStringPtr& str ) 
    {
      if(str.IsValid())
        for(const char* i= str->c_str(); *i!=0; ++i)
          os << *i;
      os << '\0';
    }
    virtual void serialize( int& i )
    { 
      os.write((char*)&i, sizeof(i));
    }
  public:
    SDSerializerWriteBinary(std::ostream& _os) : os(_os) {}
  private:
    std::ostream& os;
  };

  class SDSerializerReadBinary : public SDSerializer
  {
    virtual void serialize( ConstStringPtr& str ) 
    {
      std::ostringstream tmp;
      char c=0;
      while( (is>>c) && c!=0 )
          tmp << c;
      str = ConstString::Lookup( tmp.str() );
    }
    virtual void serialize( int& i )
    { 
      is.read((char*)&i, sizeof(i));
    }
  public:
    SDSerializerReadBinary(std::istream& _is) : is(_is) {}
  private:
    std::istream& is;
  };


  template< typename TSTREAM >
  class TmpStream : public TSTREAM
  {
  public:
    TmpStream(const char* filename)
      : TSTREAM(filename)
    {
      ASSERT_WARNING(this->is_open())(filename).Text("failed to open file"); 
    }
  };
}

void ShaderDescription::SerializeAsText( std::ostream& o ) const
{
  ShaderDescription tmp = *this;
  SDSerializerWriteText(o).serializeSD(tmp);
}

void ShaderDescription::SerializeAsText( const char* filename ) const
{
  TmpStream<std::ofstream> fs(filename);
  SerializeAsText(fs);
}

void ShaderDescription::SerializeAsBinary( std::ostream& o ) const
{
  ShaderDescription tmp = *this;
  SDSerializerWriteBinary(o).serializeSD(tmp);
}

void ShaderDescription::SerializeAsBinary( const char* filename ) const
{
  TmpStream<std::ofstream> fs(filename);
  SerializeAsBinary(fs);
}

void ShaderDescription::UnserializeFromText( std::istream& i )
{
  SDSerializerReadText(i).serializeSD(*this);
}

void ShaderDescription::UnserializeFromText( const char* filename )
{
  TmpStream<std::ifstream> fs(filename);
  UnserializeFromText(fs);
}
void ShaderDescription::UnserializeFromBinary( std::istream& i )
{
  SDSerializerReadBinary(i).serializeSD(*this);
}

void ShaderDescription::UnserializeFromBinary( const char* filename )
{
  TmpStream<std::ifstream> fs(filename);
  UnserializeFromBinary(fs);
}

