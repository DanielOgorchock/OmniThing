#ifndef OMNI_LOGGER_H
#define OMNI_LOGGER_H

#include "OmniThing.h"

#define LOG omni::OmniThing::getInstance().getLogger()

#ifdef OMNI_FAKE_F_MACRO
	#define F(x) (x)
#endif

#ifndef OMNI_NOT_ARDUINO
    #include <Arduino.h>
#endif

namespace omni
{
    class Logger
    {
        private:
        protected:
        public:
            virtual ~Logger();

            static char endl;

            virtual void flush() {};

            virtual Logger& operator<< (bool val) {return *this;}
            virtual Logger& operator<< (short val) {return *this;}
            virtual Logger& operator<< (unsigned short val) {return *this;}
            virtual Logger& operator<< (int val) {return *this;}
            virtual Logger& operator<< (unsigned int val) {return *this;}
            virtual Logger& operator<< (long val) {return *this;}
            virtual Logger& operator<< (unsigned long val) {return *this;}
            virtual Logger& operator<< (long long val) {return *this;}
            virtual Logger& operator<< (unsigned long long val) {return *this;}
            virtual Logger& operator<< (float val) {return *this;}
            virtual Logger& operator<< (double val) {return *this;}
            virtual Logger& operator<< (long double val) {return *this;}
            virtual Logger& operator<< (void* val) {return *this;}
            virtual Logger& operator<< (const char* val) {return *this;}
            virtual Logger& operator<< (char val) {return *this;}

#ifndef OMNI_NOT_ARDUINO
            virtual Logger& operator<< (const __FlashStringHelper* val) {return *this;}
#endif

            static Logger StubbedLogger;
    };
}
#endif
