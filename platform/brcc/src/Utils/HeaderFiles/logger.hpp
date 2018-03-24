/****************************************************************************

Copyright (c) 2003, Stanford University
All rights reserved.

Copyright (c) 2007, Advanced Micro Devices, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of Stanford University nor the names of any contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

****************************************************************************/

// logger.hpp

#ifndef INC_LOGGER_HPP
#define INC_LOGGER_HPP

#define BROOK_LOGGER_ENABLED  1 // FIXME - move to common config file

#include <stdio.h>
#include <stdarg.h>

class Logger
{
  public:
    static void SetFile(FILE * f)
    {
        logfile = f;
    }
    static void SetMask(unsigned int m)
    {
        mask = m;
    }
    static void Log(unsigned int type, const char *format, ...)
    {
        if (logfile && (type & mask))
        {
            va_list p;

            va_start(p, format);
            vfprintf(logfile, format, p);
            va_end(p);
            fflush(logfile);
        }
    }
  private:
    static FILE *logfile;
    static unsigned int mask;
};

#ifdef BROOK_LOGGER_ENABLED
#define LOG                     Logger::Log
#else
#define LOG                     1?(void)0:(void)
#endif

#define LOG_FN_ENTRY            1
#define LOG_WARNING             (1<<2)
#define LOG_ERROR               (1<<3)
#define LOG_COMMENTARY          (1<<4)

#include <iostream>

#define BROOK_LOG( __level ) if (1) fprintf(stderr, "Old-style logger macro found at %s:%d.\n", __FILE__, __LINE__); else ::std::cout

#define BROOK_LOG_PRINT( __level ) if (1) fprintf(stderr, "Old-style logger macro found at %s:%d.\n", __FILE__, __LINE__); else ::std::cout

#if 0                           // old version

#define BROOK_LOG( __level ) \
  if( 0 ) ::std::cout

#define BROOK_LOG_PRINT( __level ) \
  if( 0 ) ::std::cout

namespace brook
{
    namespace internal
    {

        class Logger
        {
          public:
            static bool isEnabled(int inLevel);
            static std::ostream & getStream();

            static void printPrefix();
            static void printSuffix();

            static void setLevel(int inLevel);
            static void setStream(std::ostream & inStream, bool inAssumeOwnership = false);

          private:
                 Logger();
            static Logger & getInstance();

            const char *prefix;
            const char *path;
                 std::ostream * stream;
            bool ownsStream;
            int level;

#ifdef _MSC_VER
#if _MSC_VER <= 1200
          public:               //bug with destructor protection. Fails to compile with private dest
#endif
#endif
               ~Logger();
        };

        class LogWriter
        {
          public:
            LogWriter(int inLevel):enabled(::brook::internal::Logger::isEnabled(inLevel))
            {
                if (enabled)
                    ::brook::internal::Logger::printPrefix();
            }
                 ~LogWriter()
            {
                if (enabled)
                    ::brook::internal::Logger::printSuffix();
            }

            operator  bool()
            {
                return enabled;
            }

            std::     ostream & operator*()
            {
                return::brook::internal::Logger::getStream();
            }

          private:
            bool enabled;
        };

    }
}

#endif

#endif // INC_LOGGER_HPP
