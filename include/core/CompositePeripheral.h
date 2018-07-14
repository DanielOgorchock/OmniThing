#ifndef OMNI_COMPOSITEPERIPHERAL_H
#define OMNI_COMPOSITEPERIPHERAL_H

namespace omni
{
    class CompositePeripheral
    {
        private:
            const char* m_Name;

        protected:
            const char* parseName(const char* json);

        public:
            CompositePeripheral(const char* name);
            virtual ~CompositePeripheral();

            const char* getName() {return m_Name;}

            virtual bool getBool(const char*) = 0;
            virtual float getFloat(const char*) = 0;
            virtual unsigned int getUInt(const char*) = 0;
    };
}
#endif
