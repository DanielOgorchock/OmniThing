#ifndef OMNI_INPUTUINT_H
#define OMNI_INPUTUINT_H

namespace omni
{
    class InputUInt
    {
        private:

        public:
            virtual ~InputUInt();

            virtual unsigned int readUInt() = 0;
    };
}
#endif

