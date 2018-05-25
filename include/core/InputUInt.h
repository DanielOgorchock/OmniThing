#ifndef OMNI_INPUTUINT_H
#define OMNI_INPUTUINT_H

namespace omni
{
    class InputUInt
    {
        private:

        public:
            virtual ~InputUInt();

            virtual bool readUInt() = 0;
    };
}
#endif

