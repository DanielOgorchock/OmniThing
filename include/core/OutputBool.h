#ifndef OMNI_OUTPUTBOOL_H
#define OMNI_OUTPUTBOOL_H



namespace omni
{
    class OutputBool
    {
        private:

        public:
            virtual ~OutputBool();

            virtual void writeBool(bool b) = 0;
    };
}
#endif
