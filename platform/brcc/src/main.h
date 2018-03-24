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

#ifndef INC_MAIN_H
#define INC_MAIN_H

#include "logger.hpp"
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>

#ifdef WINDOWS
#pragma warning(disable: 4996)
#endif

#define TARGET_CPU      (1<<0)
#define TARGET_PS20     (1<<1)
#define TARGET_FP30     (1<<2)
#define TARGET_ARB      (1<<3)
#define TARGET_MULTITHREADED_CPU (1<<4)
#define TARGET_FP40     (1<<5)
#define TARGET_PS30     (1<<6)
#define TARGET_PS2B     (1<<7)
#define TARGET_PS2A     (1<<8)
#define TARGET_CAL      (1<<11)

#define GPU_ARCH_DEFAULT 0
#define GPU_ARCH_X800    1
#define GPU_ARCH_6800    2

#define DX9_MRT 4   //set to 1 for stress test
#define CAL_MRT 8   //set to 1 for stress test


//#define DISABLE_C_STYLE_INDEXING
//#define FORCE_C_STYLE_GATHER_INDEXING

struct globals_struct
{
    globals_struct()
    {
        verbose = false;
        keepFiles = false;
        userTargets = 0;
        target = 0;
        printLineDirectives = true;
        enableGPUAddressTranslation = true;
        arch = 0;
        itemizeShaderString = false;
        enableSematicCheck = false;
        nIncludeDirs = 0;
        warningLevel = 0;
        warningsAsErrors = false;
        enablePreprocessor = false;
        enableStrongTypeChecking = true;
        enableExternCForNonKernelDecls = false;
        disableConstBuffer = false;
        strongTypeCheckNote = true;
        codeGenFailed = false;
    }
    bool    verbose;                      //! prints information
    bool    keepFiles;                    //! keep internal generated files
    char    *sourceName;                  //! .br file name including path
    char    *ppName;                      //! Name of the file containing preprocessed source
    char    *shaderOutputName;            //! Shader name
    char    *cppOutputName;               //! Generated output cpp file name
    int     userTargets;                  //! User requested Backends. 
    int     target;                       //! Generation of code for perticular backend in given time
                                          //! while generating code for backends specified by user

    bool    printLineDirectives;          //! Add line directive information to debug
    bool    enableGPUAddressTranslation;  //! Enable Address translation
    int     arch;                         //! Architecture target
    bool    itemizeShaderString;
    char    *brccVersion;                 //! BRCC version string
    bool    enableSematicCheck;           //! Enable Sematic Cheaker
    int     nIncludeDirs;                 //! Number of include directory search paths to pass to preprocessor
    char    **includeDirs;                //! Pointer to array of paths to pass to preprocessor
    int     warningLevel;                 //! Warning level 0, 1, 2, 3. 0 is default
    bool    warningsAsErrors;             //! Treat warnings as errors
    bool    enablePreprocessor;            //! Enables Preprocessor
    bool    enableStrongTypeChecking;     //! Enables Strong type checking: default is true
    bool    enableExternCForNonKernelDecls;  //! Add non kernel function declarations to generated header
    bool    disableConstBuffer;           //! Disable constant buffer feature
    bool    strongTypeCheckNote;           //! To display note on strong type checking
    bool    codeGenFailed;                 //! Set to true if any backend code gen fails

    //! There is a need to print or skip some information based on backend 
    //! for given parser information. 
    //! setPresentTarget and resetPresentTarget must exists together
    
    //! Get present target
    int getPresentTarget()
    {
        return target;
    }
    
    //! set present target
    void setPresentTarget(int presentTarget){target = presentTarget;}

    //! Reset to None
    void resetPresentTarget(){target = 0;}
};

extern struct globals_struct globals;

#endif // INC_MAIN_H
