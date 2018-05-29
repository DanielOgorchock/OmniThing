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

            virtual Logger& operator<< (bool val) {return *this;}
            virtual Logger& operator<< (short val) {return *this;}
            virtual Logger& operator<< (unsigned short val) {return *this;}
            virtual Logger& operator<< (int val) {return *this;}
            virtual Logger& operator<< (unsigned int val) {return *this;}
            virtual Logger& operator<< (long val) {return *this;}
            virtual Logger& operator<< (unsigned long val) {return *this;}
            virtual Logger& operator<< (float val) {return *this;}
            virtual Logger& operator<< (double val) {return *this;}
            virtual Logger& operator<< (long double val) {return *this;}
            virtual Logger& operator<< (void* val) {return *this;}
            virtual Logger& operator<< (const char* val) {return *this;}
            virtual Logger& operator<< (char val) {return *this;}
    };
}
#endif
