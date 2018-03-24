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

/*  o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o

    CTool Library
    Copyright (C) 1998-2001     Shaun Flisakowski

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

/*  o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o
    o+
    o+     File:         location.cpp
    o+
    o+     Programmer:   Brian Chess
    o+     Date:         May 31, 1999
    o+
    o+     Keep track of the line, column, and filename of a token
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

#include <iostream>

#include "main.h"
#include "location.h"

const Location Location::null_location;

std::ostream & operator<<(std::ostream & out, const Location & l)
{
    l.printLocation(out);
    return out;
}


void Location::printLocation(std::ostream & p) const
{
    p << file << '(' << line << ") : ";
}


void Location::printLineDirective(std::ostream & p, int rel) const
{
    if(globals.enableSematicCheck == true)
        return;

    // IAN:  I'm really not sure if we want line or line+1
    // experimentally on Windows, line+1 seems to work right
    // in vis studio but more work is needed here...
    p << std::endl << "#line " << (line + rel) << " \"" << file << "\"" << std::endl;
}
