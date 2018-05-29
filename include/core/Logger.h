#ifndef OMNI_LOGGER_H
#define OMNI_LOGGER_H

#include "OmniThing.h"

#define LOG OmniThing::getInstance().getLogger()

namespace omni
{
    class Logger
    {
        private:
        protected:
        public:
            virtual ~Logger();

            static char endl;

            virtual Logger& operator<< (bool val) = 0;
            virtual Logger& operator<< (short val) = 0;
            virtual Logger& operator<< (unsigned short val) = 0;
            virtual Logger& operator<< (int val) = 0;
            virtual Logger& operator<< (unsigned int val) = 0;
            virtual Logger& operator<< (long val) = 0;
            virtual Logger& operator<< (unsigned long val) = 0;
            virtual Logger& operator<< (float val) = 0;
            virtual Logger& operator<< (double val) = 0;
            virtual Logger& operator<< (long double val) = 0;
            virtual Logger& operator<< (void* val) = 0;
            virtual Logger& operator<< (const char* val) = 0;
            virtual Logger& operator<< (char val) = 0;
    };
}
#endif
