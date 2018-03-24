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
    o+     File:         location.h
    o+
    o+     Programmer:   Brian Chess
    o+     Date:         May 31, 1999
    o+
    o+     Keep track of the line, column, and filename of a token
    o+
    o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o+o  */

#ifndef INC_LOCATION_H
#define INC_LOCATION_H

#include <string>
#include "logger.hpp"

class Location
{
public:
    int line;
    int column;
    const char *file;
    Location()
        {
            line = 0;
            column = 0;
            file = "Internal error: unknown location";
        }
    Location(int l, int c, const char *f):line(l), column(c), file(f)
        {
        }
    friend std::ostream &operator<<(std::ostream &o, const Location &l);
    void printLocation(std::ostream & p) const;
    void printLineDirective(std::ostream & p, int rel=0) const;
    static const Location null_location;
};

#endif // INC_LOCATION_H
