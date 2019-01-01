#ifndef OMNI_LOGGERSERIAL_H
#define OMNI_LOGGERSERIAL_H

#include "Logger.h"
#include <Arduino.h>

namespace omni
{
    class LoggerSerial : public Logger
    {
        private:
            HardwareSerial& m_Serial;
            unsigned int m_nBaud;
        protected:
        public:
            LoggerSerial(HardwareSerial& ser, unsigned int long = 9600);
            virtual ~LoggerSerial();

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
            virtual Logger& operator<< (const __FlashStringHelper* val);
    };
}

#endif

