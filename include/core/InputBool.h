#ifndef OMNI_INPUTBOOL_H
#define OMNI_INPUTBOOL_H



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
