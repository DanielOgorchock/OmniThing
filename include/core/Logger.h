#ifndef OMNI_LOGGER_H
#define OMNI_LOGGER_H

namespace omni
{
    class Logger
    {
        private:
        protected:
        public:
            virtual ~Logger();

            static char endl() {return '\n';}

            virtual STREAM_OP_RET operator<< (bool val) = 0;
            virtual STREAM_OP_RET operator<< (short val) = 0;
            virtual STREAM_OP_RET operator<< (unsigned short val) = 0;
            virtual STREAM_OP_RET operator<< (int val) = 0;
            virtual STREAM_OP_RET operator<< (unsigned int val) = 0;
            virtual STREAM_OP_RET operator<< (long val) = 0;
            virtual STREAM_OP_RET operator<< (unsigned long val) = 0;
            virtual STREAM_OP_RET operator<< (float val) = 0;
            virtual STREAM_OP_RET operator<< (double val) = 0;
            virtual STREAM_OP_RET operator<< (long double val) = 0;
            virtual STREAM_OP_RET operator<< (void* val) = 0;
    };
}
#endif
