#include "LoggerSerial.h"

#include <Arduino.h>

namespace omni
{
//private
//protected
//public
    LoggerSerial::LoggerSerial(HardwareSerial& ser, unsigned long baud):
        m_Serial(ser),
        m_nBaud(baud)
    {
        ser.begin(baud);
    }

    LoggerSerial::~LoggerSerial()
    {

    }

    void LoggerSerial::flush()
    {
        //do nothing on arduino
    }

    Logger& LoggerSerial::operator<< (bool val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (short val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (unsigned short val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (int val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (unsigned int val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (long val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (unsigned long val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (long long val)
    {
        m_Serial.print((long)val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (unsigned long long val)
    {
        m_Serial.print((unsigned long)val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (float val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (double val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (long double val)
    {
        m_Serial.print(F("LONG_DOUBLE_AMBIGUOUS"));
        return *this;
    }

    Logger& LoggerSerial::operator<< (void* val)
    {
        m_Serial.print(F("VOID_POINTER_AMBIGUOUS"));
        return *this;
    }

    Logger& LoggerSerial::operator<< (const char* val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (char val)
    {
        m_Serial.print(val);
        return *this;
    }

    Logger& LoggerSerial::operator<< (const __FlashStringHelper* val)
    {
        m_Serial.print(val);
        return *this;
    }
}

