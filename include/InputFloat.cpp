#ifndef OMNI_INPUT_FLOAT_H
#define OMNI_INPUT_FLOAT_H



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
