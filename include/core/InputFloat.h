#ifndef OMNI_INPUTFLOAT_H
#define OMNI_INPUTFLOAT_H



namespace omni
{
    class InputFloat
    {
        private:

        public:
            virtual ~InputFloat();

            virtual bool getFloat() = 0;
    };
}
#endif
