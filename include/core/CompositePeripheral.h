#ifndef OMNI_COMPOSITEPERIPHERAL_H
#define OMNI_COMPOSITEPERIPHERAL_H

namespace omni
{
    class CompositePeripheral
    {
        private:
        protected:
        public:
            virtual ~CompositePeripheral();

            virtual bool getBool(const char*) = 0;
            virtual float getFloat(const char*) = 0;
            virtual unsigned int getUInt(const char*) = 0;
    };
}
#endif
