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

// logger.cpp
#include "logger.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FILE *Logger::logfile = NULL;
unsigned int Logger::mask = 0xFFFFFFFF;

#if 0
namespace brt
{
    namespace internal
    {

        Logger::Logger():prefix(NULL), stream(NULL), ownsStream(false), level(-1)
        {
            const char *levelVariable = getenv("BRT_LOG_LEVEL");

            path = getenv("BRT_LOG_PATH");
            prefix = getenv("BRT_LOG_PREFIX");

            if (levelVariable)
                level = atoi(levelVariable);
        }

        Logger::~Logger()
        {
            if (stream)
            {
                *stream << std::flush;
                if (ownsStream)
                    delete stream;
            }
        }

        Logger & Logger::getInstance()
        {
            static Logger sResult;

            return sResult;
        }

        bool Logger::isEnabled(int inLevel)
        {
            return getInstance().level >= inLevel;
        }

        std::ostream & Logger::getStream()
        {
            Logger & instance = getInstance();

            if (!instance.stream)
            {
                if (instance.path && strlen(instance.path) != 0)
                {
                    instance.ownsStream = true;
                    instance.stream = new std::ofstream(instance.path);
                }
                else
                    instance.stream = &(std::cerr);
            }

            return *instance.stream;
        }

        void Logger::printPrefix()
        {
            Logger & instance = getInstance();
            if (instance.prefix)
                getStream() << instance.prefix;
        }

        void Logger::printSuffix()
        {
            getStream() << std::endl;
        }

        void Logger::setLevel(int inLevel)
        {
            getInstance().level = inLevel;
        }

        void Logger::setStream(std::ostream & inStream, bool inAssumeOwnership)
        {
            Logger & instance = getInstance();

            if (instance.stream == &inStream)
            {
                instance.ownsStream = inAssumeOwnership;
                return;
            }

            if (instance.stream && instance.ownsStream)
                delete instance.stream;

            instance.stream = &inStream;
            instance.ownsStream = inAssumeOwnership;
        }

    }
}
#endif
