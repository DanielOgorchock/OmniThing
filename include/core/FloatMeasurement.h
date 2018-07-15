#ifndef OMNI_FLOATMEASUREMENT_H
#define OMNI_FLOATMEASUREMENT_H

#include "Device.h"

namespace omni
{
    class FloatMeasurement : public Device
    {
        private:
            InputFloat& m_Input;
            float m_fVal;
            const char* m_Attribute;

        protected:
            virtual void sendJsonPacket();
            virtual void read();

        public:
            FloatMeasurement(InputFloat& input, const char* attribute);
            virtual ~FloatMeasurement();


            virtual void recvJson(const char* cmd, const char* json);
            virtual void run();
            virtual void init();

            virtual const char* getType() const {return Type;}
            virtual float getFloat() const {return m_fVal;}


            static FloatMeasurement* createFromJson(const char* json, const char* attribute);

            // json commands
            static const char* Cmd_Poll;

            // events
            // none as of now

            static const char* Type;
    };
}
#endif

