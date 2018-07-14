#ifndef OMNI_COMPOSITEPERIPHERAL_H
#define OMNI_COMPOSITEPERIPHERAL_H

namespace omni
{
    class CompositePeripheral
    {
        private:
            const char* m_Name;

        protected:
            static const char* parseName(const char* json);

        public:
            CompositePeripheral(const char* name);
            virtual ~CompositePeripheral();

            const char* getName() {return m_Name;}

            virtual bool getBool(const char* name, bool& b) {return false;}
            virtual bool getFloat(const char* name, float& f) {return false;}
            virtual bool getUInt(const char* name, unsigned int& ui) {return false;}
    };
}
#endif
