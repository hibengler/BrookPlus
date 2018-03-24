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

#include "parameter.hpp"

#include "smartpointer.hpp"
#include "assertlogging.hpp"
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

PARAMETER_MUTABLE(p_progname,"0");
PARAMETER_DESC("0", "Path to the executable for this program.", Parameter::Secret | Parameter::RequireValue);

namespace
{
  typedef SmartPtr<Parameter> ParameterPtr;
  typedef std::map< std::string, ParameterPtr > ParameterMap;
  static ParameterMap& theParameterMap() 
  {
    static ParameterMap pm;
    return pm;
  }

  static Parameter& lookupParam( const std::string& name )
  {
    ParameterPtr& v = theParameterMap()[name];
    if(v==0) v=new Parameter();
    v->AddAlias(name);
    return *v;
  }

  // "invert" our param map choosing the longest version of each arg name
  static std::vector<ParameterPtr> generatedParameterList()
  {
    //set removes duplicates
    std::set<ParameterPtr> params;
    for(ParameterMap::const_iterator i = theParameterMap().begin()
      ; i!=theParameterMap().end()
      ; ++i)
      params.insert(i->second);
    
    std::vector<ParameterPtr> v;
    v.reserve(params.size());
    for(std::set<ParameterPtr>::const_iterator i = params.begin()
       ; i != params.end()
       ; ++i)
      v.push_back(*i);
    return v;
  }

  struct byNameParamPtrStort
  {
    bool operator()(const ParameterPtr& a, const ParameterPtr& b)
    {
      return a->GetName() < b->GetName();
    }
  };

}

void Parameter::Populate( int argc, const char** argv )
{
  if(argc>0)
    p_progname.SetValue(argv[0]);

  int unnamedParamCount = 0;
  while(++argc > 0 && *++argv != 0)
  { //foreach arg
    std::ostringstream accu_name;
    std::ostringstream accu_value;
    
    const char* i = *argv;
    
    if(*i!='-')
    { //handle unnamed args
      Parameter& upv = LookupMutable(ToString(++unnamedParamCount));
      upv.Enable();
      upv.SetValue(i);
      continue;
    }

    //scroll past the '--'
    while(*i=='-') ++i;
    
    //read name
    while(*i!=0 && *i!='=') accu_name << *i++;

    ParameterPtr& pv = theParameterMap()[accu_name.str()];
    ASSERT_ERROR(pv.IsValid())(accu_name.str()).Text("unknown parameter");
    ASSERT_ERROR(!pv->IsEnabled())(*pv).Text("parameter set multiple times");
    pv->Enable();

    if(*i=='=')
    {
      ++i;
      //read value
      while(*i!=0) accu_value << *i++;
      
      pv->SetValue(accu_value.str());
    }

    if(pv->HasValue())
      ASSERT_ERROR(pv->HasFlag(AllowValue))
        (accu_name.str())("-" + pv->GetName() + pv->HelpUsageString())
        .Text("Parameter does not expect a value");
    else
      ASSERT_ERROR(!pv->HasFlag(RequireValue))
        (accu_name.str())("-" + pv->GetName() + pv->HelpUsageString())
        .Text("Parameter requires a value");

  }
}

const Parameter& Parameter::Lookup( const std::string& name )
{
  return lookupParam(name);
}

Parameter& Parameter::LookupMutable( const std::string& name )
{
  return lookupParam(name);
}

int Parameter::Alias( const std::string& a, const std::string& b )
{
  ParameterPtr& pa = theParameterMap()[a];
  ParameterPtr& pb = theParameterMap()[b];

  if(pa.IsValid() && !pb.IsValid())
  {
    pb=pa;
    pb->AddAlias(b);
  }
  else if(!pa.IsValid() && pb.IsValid())
  {
    pa=pb;
    pa->AddAlias(a);
  }
  else if(!pa.IsValid() && !pb.IsValid())
  {
    Lookup(a); //create a
    DEBUG_CHECK(pa.IsValid());
    pb=pa;
    pb->AddAlias(b);
  }
  else if(pa == pb)
  {
    //alias already exists
  }
  else
  {
    DIE("Alias can not be create after both sides have been initialized")(a)(b);
    return false;
  }
  return true;
}

std::string Parameter::SerializeCommandLine()
{
  std::vector<ParameterPtr> params = generatedParameterList();
  std::ostringstream os;
  os << p_progname.Value();

  for(std::vector<ParameterPtr>::const_iterator i = params.begin()
      ; i!=params.end()
      ; ++i)
  {
    const std::string& name = (*i)->GetName();

    //skip program name
    if(name == "0") continue; 

    if((*i)->IsEnabled())
    {
      os << " -" << name;
      if((*i)->Value().size()>0)
      {
        os << "=\"" << (*i)->Value() << "\"";
      }
    }
  }

  return os.str();
}

void Parameter::PrintHelpText( std::ostream& os )
{
  std::vector<ParameterPtr> params = generatedParameterList();
  std::sort(params.begin(), params.end(), byNameParamPtrStort());

  for(std::vector<ParameterPtr>::const_iterator i = params.begin()
    ; i!=params.end()
    ; ++i)
  {
    if(((*i)->mFlags & Secret) == Secret) continue;

    const char* sep = "";
    for(std::set<std::string>::const_iterator n = (*i)->mAliases.begin(); n!=(*i)->mAliases.end(); ++n)
    {
      os << sep;
      sep = " OR ";
      if(n->length()==1 && Between('0',n->at(0),'9'))
        os << "<arg" << *n << '>';
      else
        os << "-" << *n << (*i)->HelpUsageString();
    }
    os << std::endl;

    if((*i)->mDescription != "") 
      os << "  " << (*i)->mDescription << std::endl/* << std::endl*/;
    else 
      os << "  (undocumented)" << std::endl /*<< std::endl*/;
  }
}

int Parameter::SetDescription( const std::string& name, const std::string& desc, int flags )
{
  Parameter& p = lookupParam(name);
  p.SetDescription(desc);
  p.mFlags = flags;
  return 0;
}

const std::string& Parameter::GetName() const
{
  DEBUG_CHECK(mAliases.size()>0);
  const std::string * bestName = &(*mAliases.begin());
  for(std::set<std::string>::const_iterator i = mAliases.begin(); i!=mAliases.end(); ++i)
    if(bestName->length() < i->length())
      bestName = &(*i);
  return *bestName;
}

const char* Parameter::HelpUsageString() const
{
  if(HasFlag(RequireValue))
    return "=...";
  else if(HasFlag(AllowValue))
    return "[=...]";
  else 
    return "";
}

void Parameter::ResetAllParameterState()
{
  std::vector<ParameterPtr> params = generatedParameterList();
  for(std::vector<ParameterPtr>::const_iterator i = params.begin()
    ; i!=params.end()
    ; ++i)
    (*i)->Disable();
}


