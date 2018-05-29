#ifndef OMNI_OUTPUTSTRING_H
#define OMNI_OUTPUTSTRING_H

namespace omni
{
    class OutputString
    {
        private:
        protected:
        public:
            virtual ~OutputString();

            virtual void writeString(const char*) = 0;
    };
}
#endif
