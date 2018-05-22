#ifndef OMNI_INPUT_BOOL_H
#define OMNI_INPUT_BOOL_H



namespace omni
{
    class InputBool
    {
        private:

        public:
            virtual ~InputBool();

            virtual bool getBool() = 0;
    };
}
#endif
