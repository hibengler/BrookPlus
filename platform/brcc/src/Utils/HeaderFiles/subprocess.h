/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

The BRCC portion of BrookGPU is derived from the cTool project
(http://ctool.sourceforge.net) and distributed under the GNU Public License.

Additionally, see LICENSE.ctool for information on redistributing the 
contents of this directory.

****************************************************************************/


/*
 * subprocess.h --
 *
 *      Interface to subprocess: a simple utility function for running a
 *      program and collecting its output.
 */
#ifndef INC_SUBPROCESS_H
#define INC_SUBPROCESS_H

extern char *Subprocess_Run(const char * const argv[], const char *input);

#endif // INC_SUBPROCESS_H
