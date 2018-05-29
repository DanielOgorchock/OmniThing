#ifndef OMNI_LOGGERSTDOUT_H
#define OMNI_LOGGERSTDOUT_H

#include "Logger.h"

namespace omni
{
    class LoggerStdout : public Logger
    {
        private:
        protected:
        public:
            virtual ~LoggerStdout();

            virtual STREAM_OP_RET operator<< (bool val);
            virtual STREAM_OP_RET operator<< (short val);
            virtual STREAM_OP_RET operator<< (unsigned short val);
            virtual STREAM_OP_RET operator<< (int val);
            virtual STREAM_OP_RET operator<< (unsigned int val);
            virtual STREAM_OP_RET operator<< (long val);
            virtual STREAM_OP_RET operator<< (unsigned long val);
            virtual STREAM_OP_RET operator<< (float val);
            virtual STREAM_OP_RET operator<< (double val);
            virtual STREAM_OP_RET operator<< (long double val);
            virtual STREAM_OP_RET operator<< (void* val);

    };
}

#endif
