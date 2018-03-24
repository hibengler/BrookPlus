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

    /*
     * main.c
     *
     *      Minor bit of code to drive the whole program.  Nothing interesting
     *      should happen here.
     */
#ifdef _WIN32
#pragma warning(disable:4786)
    //the above warning disables visual studio's annoying habit of warning when using the standard set lib
#include <io.h>
#include <process.h>
#else
#include <unistd.h>
#endif

#include <fstream>

    extern "C"
    {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


#include "getopt.h"
    }

#include "logger.hpp"
#include "main.h"
#include "ctool.h"
#include "brtscatter.h"

#define BROOK_VERSION "1.4.1"

extern int yydebug;


extern int yy_flex_debug;
extern int ppyy_flex_debug;

struct globals_struct globals;

/*
 * usage --
 *
 *      Dumps the legitimate commandline options and exits.
 */

static void usage(void)
{
    fprintf(stderr, "Brook+ Compiler\n");
    fprintf(stderr, "Version: %s  Built: %s, %s\n", globals.brccVersion, __DATE__, __TIME__);
    fprintf(stderr,
        "brcc [-hkrbfilxaec] [-D macro] [-n flag] [-w level] [-o prefix] [-p shader ] foo.br\n\n"
            "   -h            Help (print this message)\n"
            "   -k            Keep generated IL program (in foo.il)\n"
            "   -r            Disable address virtualization\n"
            "   -o <prefix>   Prefix prepended to all output files\n"
            "   -p <shader>   cpu or cal (can specify multiple)\n"
            "   -s            Tokenize into char list generated IL program\n"
            "   -b            Turn on bison debugging\n"
            "   -f            Turn on flex debugging\n"
            "   -i            Specify include directory for passing to external preprocessor\n"
            "   -l            Insert #line directives into generated code\n"
            "   -w <level>    Specify level of warning level. level can be 0, 1, 2, 3\n"
            "                 0 level is default \n"
            "   -x            Turn on warnings as errors \n"
            "   -a            Disable strong type checking.\n"
            "   -e            Adds extern C for non kernel function declarations\n"
            "   -c            Disable cached gather array feature\n"
            "   -pp           Enables the preprocessor\n"
            "   -D <name>     Define macro\n"
            "   -D <name>{=}<int-value> Define macro with integer value\n"
            "                 No spaces allowed between macro name and macro value\n"
            "   -n flag       Disable the specified flag\n"
            "                 flag = l -> Disable line directive information to debug\n"
            "                 presently -l flag is only valid flag\n"
            "\n\n"
            "Note : Usage of -x and -w flags are valid only with -a flag\n"
            "\n");

    exit(1);
}

/*
 * parse_args --
 *
 *      Parses argv and sets the global options as a result.
 */

#define MAX_INCLUDE_PATHS       32

static void parse_args(int argc, char *argv[])
{
    extern int stringReplace(char *str, char oldc, char newc);
    int opt, n;
    char *outputprefix = NULL;

    LOG(LOG_FN_ENTRY, "parse_args(%d, %p)\n", argc, argv);

    /*
     * zero initialization from the bss will take care of the rest of the
     * defaults.
     */
    yy_flex_debug = 0;
    ppyy_flex_debug = 0;
    globals.brccVersion = BROOK_VERSION;
    while ((opt = getopt(argc, argv, "d:hkrbfilxaeco:p:svwn:D:zi:")) != EOF)
    {
        switch (opt)
        {
        case 'b':
            yydebug = 1;
            break;
        case 'd':
            Project::gDebug = true;
            break;
        case 'f':
            yy_flex_debug = 1;
            ppyy_flex_debug = 1;
            break;
        case 'h':
            usage();
            break;
        case 'i':
            if (!globals.nIncludeDirs)
            {
                globals.includeDirs = (char **)malloc(MAX_INCLUDE_PATHS * sizeof(char *));
                if (!globals.includeDirs)
                {
                    fputs("Failed to allocate memory for holding include dir list. Aborting.\n", stderr);
                    exit(-1);
                }
            }
            if (globals.nIncludeDirs == MAX_INCLUDE_PATHS)
            {
                fputs("Too many include directories. Aborting.\n", stderr);
                exit(-1);
            }
            globals.includeDirs[globals.nIncludeDirs] = strdup(optarg);
            if (!globals.includeDirs[globals.nIncludeDirs])
            {
                fputs("Failed to allocate memory for include dir. Aborting.\n", stderr);
                exit(-1);
            }
            globals.nIncludeDirs++;
            break;
        case 'k':
            globals.keepFiles = true;
            break;
        case 'l':
            globals.printLineDirectives = true;
            break;
        case 'n':
            if(strcasecmp(optarg, "l") == 0)
            {
                globals.printLineDirectives = false;
            }
            else
            {
                fprintf(stderr, "\n\nWrong usage of -n option \n\n");
                usage();
            }
            break;
        case 'o':
            if (outputprefix)
                usage();
            outputprefix = strdup(optarg);
            break;
        case 'p':
            if (strcasecmp(optarg, "cpu") == 0)
                globals.userTargets |= TARGET_CPU;
            else if (strcasecmp(optarg, "cal") == 0)
                globals.userTargets |= TARGET_CAL;
            else if((strcasecmp(optarg, "p") == 0))
                globals.enablePreprocessor = true;
            else
                usage();
            break;
        case 'R':
        case 'r':
            globals.enableGPUAddressTranslation = false;
            break;
        case 's':
            globals.itemizeShaderString = true;
            break;
        case 'v':
            globals.verbose = 1;
            break;
        case 'w':
            globals.warningLevel = atoi(optarg);
            if(globals.warningLevel < 0 || globals.warningLevel > 3)
                usage();
            break;
        case 'x':
            globals.warningsAsErrors = true;
            break;
        case 'a':
            globals.enableStrongTypeChecking = false;
            break;
        case 'e':
            globals.enableExternCForNonKernelDecls = true;
            break;
        case 'c':
            globals.disableConstBuffer = true;
            break;
        case 'D':
            {
                extern bool processDefineMacro(const char*, std::string&); 
                std::string errorMsg = "";
                if(processDefineMacro(optarg, errorMsg) == false)
                {
                    fprintf(stderr, "\n Problem with -D flag(%s) \n \t%s \n", optarg, errorMsg.c_str());
                    usage();
                }
            }
            break;
        default:
            usage();
        }
    }

    // The default build targets
    if (globals.userTargets == 0)
        globals.userTargets = TARGET_CPU | TARGET_CAL;

    if(globals.enableStrongTypeChecking)
    {
        globals.warningLevel = 3;
        globals.warningsAsErrors = true;
    }

    argv += optind;
    argc -= optind;
    if (argc < 1)
        usage();
    globals.sourceName = (char *) argv[0];
    stringReplace(globals.sourceName, '\\', '/');

    n = (int) strlen(globals.sourceName);
    int suffixLength;
    bool isHeader = false;

    if (n >= 3 && !strcmp(globals.sourceName + n - 3, ".br"))
    {
        suffixLength = 3;
    }
    else if (n >= 4 && !strcmp(globals.sourceName + n - 4, ".brh"))
    {
        suffixLength = 4;
        isHeader = true;
    }
    else
    {
        usage();
    }

    if (!outputprefix)
    {
        outputprefix = strdup(globals.sourceName);
        outputprefix[n - suffixLength] = (char) '\0';
    }

    globals.shaderOutputName = strdup(outputprefix);

    globals.cppOutputName = (char *) malloc(strlen(outputprefix) + suffixLength + 2);
    if (isHeader)
    {
        sprintf(globals.cppOutputName, "%s.h", outputprefix);
    }
    else
    {
        sprintf(globals.cppOutputName, "%s.cpp", outputprefix);
    }

    free(outputprefix);
}

/*
 * preprocess - pipe the source through an external preprocessor
 *
 * This is a lot easier done at the makefile level.
 *
 */

#define MAX_PP_OPTIONS  32
#define MAX_OPTION_SIZE 256
#ifdef WIN32
#define snprintf _snprintf
#define dup      _dup
#define dup2     _dup2
#endif

char *preprocess(const char *pp_var, const char *src_name)
{
    char *opt[MAX_PP_OPTIONS+1];        // Pointers to each option passed, opt[0] is program
    bool in_quotes = false;             // Toggle for param quoting
    static char tmp_name[L_tmpnam];     // Temporary file (not reentrant, but shouldn't need to be)
    char opt_buf[MAX_OPTION_SIZE+1];    // Buffer for building up each option
    int opt_idx = 0;
    const char *s;
    char *d;
    bool end_of_this_opt, end_of_all_opts;
    bool need_stdin_redirection = true;         // Some compilers (MSVC) don't allow us to specify the name of the preprocessed file, so we allow
    bool need_stdout_redirection = true;        // redirection as an alternative
    FILE *redirected_stdin;
    FILE *redirected_stdout;
    int old_stdin;
    int old_stdout;
    int r;
    int ipath_count = 0;
    int option_space_left;

    // Create a temporary file name

    if (!tmpnam(tmp_name))
    {
        fputs("Failed to create temporary file while invoking preprocessor. Aborting.\n", stderr);
        exit(-1);
    }

#ifdef WIN32

    // (windows workround for leading backslash silliness)

    d = tmp_name;
    s = d+1;
    while (*s)
    {
        *d++ = *s++;
    }
    *d++ = 0;

#endif

    printf("tname: %s\n", tmp_name);

    // Walk the config string, chopping into individual options and substituting as needed
    //
    // %S = name of the source file
    // %P = name of the preprocessed file
    //
    // if one or the other is not present (not all compilers have compatible options) we will try to automatically redirect stdin/stdout
    //
    // %I = placeholder for include dirs (currently need multiple occurrences for multiple dirs, will use '.' to fill the ones that aren't provided)

    s = pp_var;
    end_of_all_opts = false;
    while (!end_of_all_opts && opt_idx < MAX_PP_OPTIONS)
    {

        // Start building option string

        d = opt_buf;

        // Copy/process

        end_of_this_opt = false;
        while (!end_of_this_opt && *s && (d - opt_buf) < MAX_OPTION_SIZE)
        {
            option_space_left = MAX_OPTION_SIZE - (int)(d - opt_buf);
            switch(*s)
            {
            case 0:
                end_of_this_opt = true;
                end_of_all_opts = true;
                *d++ = 0;
                break;
            case '\"':
                in_quotes = !in_quotes;
                *d++ = *s;
                break;
            case '%':           // special option

                switch (*++s)
                {
                case 'S':       // input file
                    d += snprintf(d, option_space_left, "%s", src_name);
                    need_stdin_redirection = false;
                    break;
                case 'P':       // output file
                    d += snprintf(d, option_space_left, "%s", tmp_name);
                    need_stdout_redirection = false;
                    break;
                case 'I':       // include path
                    if (ipath_count < globals.nIncludeDirs)
                    {
                        d += snprintf(d, option_space_left, "%s", globals.includeDirs[ipath_count++]);
                    }
                    else
                    {
                        d += snprintf(d, option_space_left, ".");
                    }
                    break;
                case 0:         // oops, string ended on a \0
                    fprintf(stderr, "Error parsing preprocessor config string - last character was a \'%\'. Aborting.\n", *s);
                    exit(-1);
                    break;
                case '%':       // just a %
                    *d++ = '%';
                    break;
                default:        // unknown option
                    fprintf(stderr, "Error parsing preprocessor config string - unknown option '%c'\n. Aborting\n", *s);
                    exit(-1);
                    break;
                }
                break;
            case ' ':
                if (!in_quotes)
                {
                    end_of_this_opt = true;
                    break;
                }
                // drop-through deliberate
            default:
                *d++ = *s;
                break;
            }
            s++;
        }

        // Create option for passing to spawned preprocessor

        *d = 0;
        opt[opt_idx++] = strdup(opt_buf);
    }

    // Finish up building options

    opt[opt_idx++] = NULL;
    for (int i = 0; i < opt_idx; i++)
        {
            printf("%d : %s\n", i, opt[i]);
        }

    // Redirect stdin/stdout if needed

    if (need_stdin_redirection)
    {
        redirected_stdin = fopen(src_name, "rt");
        if (!redirected_stdin)
        {
            fputs("Cannot open \"%s\" for preprocessing. Aborting.\n", stderr);
            exit(-1);
        }
        old_stdin = dup(0);
        r = dup2(fileno(redirected_stdin), 0);
        if (r)
        {
            fputs("Failed to redirect stdin for external preprocessor. Aborting.\n", stderr);
            exit(-1);
        }
    }

    if (need_stdout_redirection)
    {
        redirected_stdout = fopen(tmp_name, "wt");
        if (!redirected_stdout)
        {
            fputs("Cannot open \"%s\" to write preprocessed source. Aborting.\n", stderr);
            exit(-1);
        }
        old_stdout = dup(1);
        r = dup2(fileno(redirected_stdout), 1);
        if (r)
        {
            fputs("Failed to redirect stdout for external preprocessor. Aborting.\n", stderr);
            exit(-1);
        }
    }

    // Execute external tool

#ifdef WIN32
    spawnvp(P_WAIT, opt[0], opt+1);
#else
    execvp(opt[0], opt+1);
#endif

    // Clean up

    if (need_stdin_redirection)
    {
        dup2(old_stdin, 0);
    }
    if (need_stdout_redirection)
    {
        dup2(old_stdout, 1);
    }
    for (opt_idx = 0; opt[opt_idx] && opt_idx < MAX_PP_OPTIONS; opt_idx++)
    {
        free(opt[opt_idx]);
    }

    // Return value is the name of the preprocessed file

    return tmp_name;
}

/*
 * main --
 *
 *      Drive everything.  Parse the arguments, then compile the requested
 *      file.
 */

#ifdef WIN32
#define unlink _unlink
#endif

int main(int argc, char *argv[])
{
    try
    {
        Project *proj;
        TransUnit *tu;
        parse_args(argc, argv);
        if (globals.verbose)
        {
                std::cerr << "***Compiling " << globals.sourceName << "\n";
        }

        // Logger init

        const char *logfile = getenv("BRCC_LOG_FILE");
        if (logfile)
        {
            Logger::SetFile(fopen(strdup(logfile), "wt"));
            if (getenv("BRCC_LOG_MASK"))
            {
                unsigned int mask = atoi(getenv("BRCC_LOG_MASK"));

                Logger::SetMask(mask);
            }
            else
            {
                Logger::SetMask(0xFFFFFFFF);
            }

        }

        // Pipe through external preprocessor if configured to do so

        const char *pp_var = getenv("BRCC_EXT_PP");
        if (pp_var)
        {
            globals.ppName = preprocess(pp_var, globals.sourceName);
        }

        // Start compilation

        proj = new Project();
        tu = proj->parse(globals.ppName ? globals.ppName : globals.sourceName, false, NULL, false, NULL, NULL, NULL);
        if (tu)
        {
            extern void newMain(TransUnit *);
            newMain(tu);
        }
        else
        {
                std::string cppFileName(globals.cppOutputName);

                std::string kernelDeclFileName(globals.shaderOutputName);
                kernelDeclFileName += std::string(".h");

                std::string headername(globals.shaderOutputName);
                headername += std::string("_gpu.h");
                
                //! Delete generated cpp files if already exist
                FILE *tmpPtr = fopen(globals.cppOutputName, "r");
                if(tmpPtr)
                {
                    fclose(tmpPtr);
                    
                    extern void deleteFile(std::string fileName);
                    //! Delete generated cpp file file
                    deleteFile(cppFileName);

                    //! Delete generated .h file 
                    deleteFile(kernelDeclFileName);

                    //! Delete generated _gpu.h file 
                    deleteFile(headername);
                }

                std::cerr << "***Unable to parse " << globals.sourceName << std::endl;
                exit(1);
        }

        if (globals.verbose)
        {
                std::cerr << "***Successfully compiled " << globals.sourceName << "\n";
        }
        delete proj;

        if (globals.ppName)
        {
            unlink(globals.ppName);
        }
    }
    catch(...)
    {
        std::cerr << "***Failed to compile " << globals.sourceName << "\n";
        exit(1);
    }
        exit(0);
    return 0;                   /* Appease CL */
}
