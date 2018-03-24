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

/*  ###############################################################
    ##
    ##     File:         utype.h
    ##
    ##     Programmer:   Shaun Flisakowski
    ##     Date:         July 10, 1997
    ## 
    ##     Define simple types.
    ##
    ###############################################################  */

#ifndef    UTYPE_H
#define    UTYPE_H

#include "config.h"
#include "identify.h"

/*  ###############################################################  */

#if     defined(DJGPP) || defined(_WIN32) || defined(MACOSX)
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
#endif

/*  ###############################################################  */

#endif /* UTYPE_H */
