/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/

// splittypes.h
#ifndef INC_SPLITTYPES_H
#define INC_SPLITTYPES_H

#include <iostream>

enum SplitBasicType
{
    kSplitBasicType_Unknown = -1,
    kSplitBasicType_Float = 0,
    kSplitBasicType_Float2,
    kSplitBasicType_Float3,
    kSplitBasicType_Float4
};

std::ostream & operator<<(std::ostream &, SplitBasicType);

#endif // INC_SPLITTYPES_H
