#ifndef OMNI_LOGGERSTDOUT_H
#define OMNI_LOGGERSTDOUT_H

#include "Logger.h"
#include <iostream>

namespace omni
{
    class LoggerStdout : public Logger
    {
        private:
        protected:
        public:
            virtual ~LoggerStdout();

            virtual void flush();

            virtual Logger& operator<< (bool val);
            virtual Logger& operator<< (short val);
            virtual Logger& operator<< (unsigned short val);
            virtual Logger& operator<< (int val);
            virtual Logger& operator<< (unsigned int val);
            virtual Logger& operator<< (long val);
            virtual Logger& operator<< (unsigned long val);
            virtual Logger& operator<< (long long val);
            virtual Logger& operator<< (unsigned long long val);
            virtual Logger& operator<< (float val);
            virtual Logger& operator<< (double val);
            virtual Logger& operator<< (long double val);
            virtual Logger& operator<< (void* val);
            virtual Logger& operator<< (const char* val);
            virtual Logger& operator<< (char val);

    };
}

#endif
