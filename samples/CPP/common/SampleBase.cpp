/* ============================================================

Copyright (c) 2008 Advanced Micro Devices, Inc.  All rights reserved.
 
Redistribution and use of this material is permitted under the following 
conditions:
 
Redistributions must retain the above copyright notice and all terms of this 
license.
 
In no event shall anyone redistributing or accessing or using this material 
commence or participate in any arbitration or legal action relating to this 
material against Advanced Micro Devices, Inc. or any copyright holders or 
contributors. The foregoing shall survive any expiration or termination of 
this license or any agreement or access or use related to this material. 

ANY BREACH OF ANY TERM OF THIS LICENSE SHALL RESULT IN THE IMMEDIATE REVOCATION 
OF ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE THIS MATERIAL.

THIS MATERIAL IS PROVIDED BY ADVANCED MICRO DEVICES, INC. AND ANY COPYRIGHT 
HOLDERS AND CONTRIBUTORS "AS IS" IN ITS CURRENT CONDITION AND WITHOUT ANY 
REPRESENTATIONS, GUARANTEE, OR WARRANTY OF ANY KIND OR IN ANY WAY RELATED TO 
SUPPORT, INDEMNITY, ERROR FREE OR UNINTERRUPTED OPERATION, OR THAT IT IS FREE 
FROM DEFECTS OR VIRUSES.  ALL OBLIGATIONS ARE HEREBY DISCLAIMED - WHETHER 
EXPRESS, IMPLIED, OR STATUTORY - INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED 
WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
ACCURACY, COMPLETENESS, OPERABILITY, QUALITY OF SERVICE, OR NON-INFRINGEMENT. 
IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, REVENUE, DATA, OR PROFITS; OR 
BUSINESS INTERRUPTION) HOWEVER CAUSED OR BASED ON ANY THEORY OF LIABILITY 
ARISING IN ANY WAY RELATED TO THIS MATERIAL, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE. THE ENTIRE AND AGGREGATE LIABILITY OF ADVANCED MICRO DEVICES, 
INC. AND ANY COPYRIGHT HOLDERS AND CONTRIBUTORS SHALL NOT EXCEED TEN DOLLARS 
(US $10.00). ANYONE REDISTRIBUTING OR ACCESSING OR USING THIS MATERIAL ACCEPTS 
THIS ALLOCATION OF RISK AND AGREES TO RELEASE ADVANCED MICRO DEVICES, INC. AND 
ANY COPYRIGHT HOLDERS AND CONTRIBUTORS FROM ANY AND ALL LIABILITIES, 
OBLIGATIONS, CLAIMS, OR DEMANDS IN EXCESS OF TEN DOLLARS (US $10.00). THE 
FOREGOING ARE ESSENTIAL TERMS OF THIS LICENSE AND, IF ANY OF THESE TERMS ARE 
CONSTRUED AS UNENFORCEABLE, FAIL IN ESSENTIAL PURPOSE, OR BECOME VOID OR 
DETRIMENTAL TO ADVANCED MICRO DEVICES, INC. OR ANY COPYRIGHT HOLDERS OR 
CONTRIBUTORS FOR ANY REASON, THEN ALL RIGHTS TO REDISTRIBUTE, ACCESS OR USE 
THIS MATERIAL SHALL TERMINATE IMMEDIATELY. MOREOVER, THE FOREGOING SHALL 
SURVIVE ANY EXPIRATION OR TERMINATION OF THIS LICENSE OR ANY AGREEMENT OR 
ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE IS HEREBY PROVIDED, AND BY REDISTRIBUTING OR ACCESSING OR USING THIS 
MATERIAL SUCH NOTICE IS ACKNOWLEDGED, THAT THIS MATERIAL MAY BE SUBJECT TO 
RESTRICTIONS UNDER THE LAWS AND REGULATIONS OF THE UNITED STATES OR OTHER 
COUNTRIES, WHICH INCLUDE BUT ARE NOT LIMITED TO, U.S. EXPORT CONTROL LAWS SUCH 
AS THE EXPORT ADMINISTRATION REGULATIONS AND NATIONAL SECURITY CONTROLS AS 
DEFINED THEREUNDER, AS WELL AS STATE DEPARTMENT CONTROLS UNDER THE U.S. 
MUNITIONS LIST. THIS MATERIAL MAY NOT BE USED, RELEASED, TRANSFERRED, IMPORTED,
EXPORTED AND/OR RE-EXPORTED IN ANY MANNER PROHIBITED UNDER ANY APPLICABLE LAWS, 
INCLUDING U.S. EXPORT CONTROL LAWS REGARDING SPECIFICALLY DESIGNATED PERSONS, 
COUNTRIES AND NATIONALS OF COUNTRIES SUBJECT TO NATIONAL SECURITY CONTROLS. 
MOREOVER, THE FOREGOING SHALL SURVIVE ANY EXPIRATION OR TERMINATION OF ANY 
LICENSE OR AGREEMENT OR ACCESS OR USE RELATED TO THIS MATERIAL.

NOTICE REGARDING THE U.S. GOVERNMENT AND DOD AGENCIES: This material is 
provided with "RESTRICTED RIGHTS" and/or "LIMITED RIGHTS" as applicable to 
computer software and technical data, respectively. Use, duplication, 
distribution or disclosure by the U.S. Government and/or DOD agencies is 
subject to the full extent of restrictions in all applicable regulations, 
including those found at FAR52.227 and DFARS252.227 et seq. and any successor 
regulations thereof. Use of this material by the U.S. Government and/or DOD 
agencies is acknowledgment of the proprietary rights of any copyright holders 
and contributors, including those of Advanced Micro Devices, Inc., as well as 
the provisions of FAR52.227-14 through 23 regarding privately developed and/or 
commercial computer software.

This license forms the entire agreement regarding the subject matter hereof and 
supersedes all proposals and prior discussions and writings between the parties 
with respect thereto. This license does not affect any ownership, rights, title,
or interest in, or relating to, this material. No terms of this license can be 
modified or waived, and no breach of this license can be excused, unless done 
so in a writing signed by all affected parties. Each term of this license is 
separately enforceable. If any term of this license is determined to be or 
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

#include "SampleBase.h"

#include <ctime>
#include <iostream>
#include <iomanip>

// Defaults used for the struct below. They are defined in the cpp.
unsigned int DEFAULT_WIDTH  = 128;
unsigned int DEFAULT_HEIGHT = 64;
unsigned int DEFAULT_ITERATIONS = 1;
int          DEFAULT_VERBOSE = 0;
int          DEFAULT_VERIFY = 0;
int          DEFAULT_PERFORMANCE = 0;
int          DEFAULT_QUIET = 0;
int          DEFAULT_TIMING = 0;
unsigned int DEFAULT_SEED = 0;

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Constructor
//! 
////////////////////////////////////////////////////////////////////////////////

SampleBase::SampleBase(char* sampleName)
{
    info = new InfoRec();
    timer = new CPerfCounter();
    _name = sampleName;
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Does the command line parsing for the application
//! 
////////////////////////////////////////////////////////////////////////////////

void
SampleBase::ParseCommandLine(int argc, char** argv)
{
    int x;
    info->Width = DEFAULT_WIDTH;
    info->Height = DEFAULT_HEIGHT;
    info->Iterations = DEFAULT_ITERATIONS;
    info->Verbose = DEFAULT_VERBOSE;
    info->Verify = DEFAULT_VERIFY;
    info->Performance = DEFAULT_PERFORMANCE;
    info->Quiet = DEFAULT_QUIET;
    info->Timing = DEFAULT_TIMING;
    info->Seed = DEFAULT_SEED;

    for (x = 1; x < argc; ++x)
    {
        switch (argv[x][1])
        {
        case 'v':
            info->Verbose = 1;
            break;
        case 'e':
            info->Verify = 1;
            break;
        case 'p':
            info->Performance = 1;
            break;
        case 'q':
            info->Quiet = 1;
            break;
        case 't':
            info->Timing = 1;
            break;
        case 'i':
            if (++x < argc)
            {
                sscanf(argv[x], "%u", &info->Iterations);
            }
            else
            {
                fprintf(stderr, "Error: Invalid argument, %s", argv[x-1]);
                Usage(argv[0]);
                exit(-1);
            }
            break;
        case 'x':
            if (++x < argc)
            {
                sscanf(argv[x], "%u", &info->Width);
            }
            else
            {
                fprintf(stderr, "Error: Invalid argument, %s", argv[x-1]);
                Usage(argv[0]);
                exit(-1);
            }
            break;
        case 'y':
            if (++x < argc)
            {
                sscanf(argv[x], "%u", &info->Height);
            }
            else
            {
                fprintf(stderr, "Error: Invalid argument, %s", argv[x-1]);
                Usage(argv[0]);
                exit(-1);
            }
            break;
        case 's':
            if (++x < argc)
            {
                sscanf(argv[x], "%u", &info->Seed);
            }
            else
            {
                fprintf(stderr, "Error: Invalid argument, %s",argv[x-1]);
                Usage(argv[0]);
                exit(-1);
            }
            break;
        case 'h':
            Usage(argv[0]);
            exit(-1);
            break;
        default:
            fprintf(stderr, "Error: Invalid command line parameter, %c\n", argv[x][1]);
            Usage(argv[0]);
            exit(-1);
        }
    }
    if ((info->Verbose + info->Quiet) == 2)
    {
        printf("Verbose and Quiet cancel each other out.\n");
        info->Verbose = 0;
        info->Quiet = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Default usage menu for all samples
//! 
////////////////////////////////////////////////////////////////////////////////

void
SampleBase::Usage(char *name)
{
    printf("\tUsage: %s [-e] [-v] [-h] [-s <int>]\n", name);
    printf("   -h       Print this help menu.\n");
    printf("   -v       Print verbose output.\n");
    printf("   -e       Verify correct output.\n");
    printf("   -p       Compare performance with CPU.\n");
    printf("   -q       Surpress all data output.\n");
    printf("   -s <int> Set the randomization seed.\n");
    printf("   -t       Print out timing information.\n");
    printf("   -x <int> Sets the matrix width.\n");
    printf("   -y <int> Sets the matrix height.\n");
    printf("   -i <int> Number of iterations.\n");

}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Allocates memory for buffers used by application
//! 
////////////////////////////////////////////////////////////////////////////////

void
SampleBase::allocateBuffer()
{
    fprintf(stderr, "Error: Default implementation should not be called\n");
    exit(-1);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Fill the input bufers with random values
//! 
////////////////////////////////////////////////////////////////////////////////

void
SampleBase::fillInputBuffer()
{
    fprintf(stderr, "Error: Default implementation should not be called\n");
    exit(-1);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Print timing information
//! 
////////////////////////////////////////////////////////////////////////////////

void
SampleBase::printTimeInfo()
{
    fprintf(stderr, "Error: Default implementation should not be called\n");
    exit(-1);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Verify against CPU implementation if requested 
//! 
////////////////////////////////////////////////////////////////////////////////

void
SampleBase::verifyResults()
{
    fprintf(stderr, "Error: Default implementation should not be called\n");
    exit(-1);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Compare performance with CPU if requested
//! 
////////////////////////////////////////////////////////////////////////////////
void
SampleBase::comparePerformance()
{
    fprintf(stderr, "Error: Default implementation should not be called\n");
    exit(-1);
}

////////////////////////////////////////////////////////////////////////////////
//!
//! \brief  Destructor
//! 
////////////////////////////////////////////////////////////////////////////////

SampleBase::~SampleBase()
{
    delete info;
    delete timer;
}
