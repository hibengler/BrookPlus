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

#ifndef _ASSERT_LOGGING_HPP
#define _ASSERT_LOGGING_HPP

#define CONFIG_ASSERT_HIDE_CONTEXT    0
#define CONFIG_ASSERT_SHOW_SPAM       0
#define CONFIG_ASSERT_SHOW_COMMENTS   1
#define CONFIG_ASSERT_SHOW_WARNINGS   1
#define CONFIG_ASSERT_USE_EXCEPTIONS  1

#include <iostream>
#include <string>
#include <sstream>
#include "convert.hpp"

class PositionAware;

void AssertLoggingSetOutputStream(std::ostream* s);

namespace assert_logging_internal
{

/**
 * Stores all the information about the context of a error
 * stuff like file and line
 * Added to the DebugInfoReciever first
 */
class DebugInfoContext    
{
public:
  DebugInfoContext(const char* file, int line, const char* function);

  friend std::ostream& operator<< (std::ostream& os, const DebugInfoContext& v);
private:
  const char* mFilename;
  int         mLine;
  const char* mFunction;
};


/**
 * For debugging the value of a single variable
 * Stores something like "a = 3" for each user variables
 * Added multiple times to the DebugInfoReciever
 */
class DebugInfoVariable 
{
public:
  /// Constructor
  template< typename T > 
  DebugInfoVariable(const char* name, const T& value)
    : mName(name)
    , mValue( ToString( value ) )
  {}
  
  /// Constructor with user name override
  template< typename T > 
  DebugInfoVariable(const char*, const char* name, const T& value)
    : mName(name)
    , mValue( ToString( value ) )
  {}


  friend std::ostream& operator<<(std::ostream& os, const DebugInfoVariable& v);
private:
  const char* mName;
  std::string mValue;
};

/**
 * An instance of this (dummy) class is always appended to the 
 * DebugInfoReciever at the end of a string of debugging statements
 */
class DebugInfoEnd 
{
public:
  DebugInfoEnd(const std::string& text = "");

  
  bool HasErrorText() const;

  ///
  /// Add some ErrorText to the assertion
  DebugInfoEnd Text(const std::string& text) const;

  friend std::ostream& operator<< (std::ostream& os, const DebugInfoEnd&);
private:
  std::string mErrorText;

};

/**
 * This object will collect logging information and output it to the desired 
 * location
 */
class DebugInfoReciever
{
public:
  DebugInfoReciever(bool isFatal, const char* des);

  DebugInfoReciever& operator<< (const DebugInfoContext& variable);
  DebugInfoReciever& operator<< (const PositionAware& pos);
  DebugInfoReciever& operator<< (const DebugInfoVariable& variable);
  DebugInfoReciever& operator<< (const DebugInfoEnd& variable);

private:
  bool          mIsFatal;
  const char*   mDescription;
};

}

extern const assert_logging_internal::DebugInfoEnd _CONTINUE_ASSERT_BLOCK_A;
extern const assert_logging_internal::DebugInfoEnd _CONTINUE_ASSERT_BLOCK_B;

//forward declare this here so users can compile the following macros w/o more 
//includes
const PositionAware& GetPosition();

//this macro will add a function(file:line) comment to each assert
#if CONFIG_ASSERT_HIDE_CONTEXT
#  define _ADD_ASSERT_CONTEXT 
#else
#  define _ADD_ASSERT_CONTEXT \
  assert_logging_internal::DebugInfoContext(__FILE__, __LINE__, __FUNCTION__) <<
#endif

//these two alternating macros allow for the syntax (var1)(var2)(var3) for 
//compact debug printing
#define _CONTINUE_ASSERT_BLOCK_A( term ) \
  assert_logging_internal::DebugInfoVariable(#term, (term)) \
  << _CONTINUE_ASSERT_BLOCK_B
#define _CONTINUE_ASSERT_BLOCK_B( term ) \
  assert_logging_internal::DebugInfoVariable(#term, (term)) \
  << _CONTINUE_ASSERT_BLOCK_A

//conditionally start a chain of _CONTINUE_ASSERT_BLOCKs
#define _BEGIN_ASSERT_BLOCK(cond, fatal, desc) \
  if( (cond) ) {} \
  else assert_logging_internal::DebugInfoReciever(fatal,desc) \
    << GetPosition() << _ADD_ASSERT_CONTEXT _CONTINUE_ASSERT_BLOCK_A

//here are the actual macros used by the user:

//Assertions that will or wont terminate on error
#define ASSERT_ERROR(cond)    \
  _BEGIN_ASSERT_BLOCK(cond, true,  "ERROR: ASSERT(" #cond ") failed")
#define ASSERT_WARNING(cond)  \
  _BEGIN_ASSERT_BLOCK( CONFIG_ASSERT_SHOW_WARNINGS==0 || (cond)\
                     , false, "WARNING: ASSERT(" #cond ") failed")
//Debug checks are for the paranoid, compiled out in release mode
#ifdef DEBUG
#  define DEBUG_CHECK(cond)   \
  _BEGIN_ASSERT_BLOCK(cond, true,  "ERROR: ASSERT(" #cond ") failed")
#else
#  define DEBUG_CHECK(cond)   _BEGIN_ASSERT_BLOCK(true, false, "")
#endif

//Informational messages
#define DIE(message)     _BEGIN_ASSERT_BLOCK(false, true, "ERROR: " message)
#define NOTICE(message) \
  _BEGIN_ASSERT_BLOCK(CONFIG_ASSERT_SHOW_COMMENTS==0, false, "NOTICE: " message)
#define SPAM(message)    \
  _BEGIN_ASSERT_BLOCK(CONFIG_ASSERT_SHOW_SPAM==0, false, "SPAM: " message)

//Stock unimplemented code warnings/errors
#define WARNING_UNIMPLEMENTED() \
  _BEGIN_ASSERT_BLOCK( CONFIG_ASSERT_SHOW_WARNINGS==0, false \
                     , "WARNING: Reached unimplemented method")
#define ERROR_UNIMPLEMENTED()   \
  _BEGIN_ASSERT_BLOCK(false, true, "ERROR: Reached unimplemented method")


#endif
