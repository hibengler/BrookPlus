/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
All rights reserved.


The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include <set>
#include <map>
#include <string>
#define INF_SENTINEL
typedef std::map < std::string, std::set < unsigned int > >VoutFunctionType;
extern VoutFunctionType voutFunctions;
void transform_vout(class TransUnit *);
