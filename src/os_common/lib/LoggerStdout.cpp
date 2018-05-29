#include "LoggerStdout.h"

#include <iostream>

namespace omni
{
//private
//protected
//public
    LoggerStdout::~LoggerStdout()
    {

    }

    STREAM_OP_RET LoggerStdout::operator<< (bool val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (short val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (unsigned short val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (int val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (unsigned int val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (long val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (unsigned long val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (float val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (double val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (long double val)
    {
        return std::cout << val;
    }

    STREAM_OP_RET LoggerStdout::operator<< (void* val)
    {
        return std::cout << val;
    }


}
