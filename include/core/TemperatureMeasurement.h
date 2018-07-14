#ifndef OMNI_TEMPERATUREMEASUREMENT_H
#define OMNI_TEMPERATUREMEASUREMENT_H

#include "Device.h"

namespace omni
{
    class TemperatureMeasurement : public Device
    {
        private:
            InputFloat& m_Input;
            float m_fVal;

            void sendJsonPacket();
            void read();

        protected:
        public:
            TemperatureMeasurement(InputFloat& input);
            virtual ~TemperatureMeasurement();


            virtual void recvJson(const char* cmd, const char* json);
            virtual void run();
            virtual void init();

            virtual const char* getType() const {return Type;}
            float getTemperature() const {return m_fVal;}


            static Device* createFromJson(const char* json);

            // json commands
            static const char* Cmd_Poll;

            // events
            // none as of now


            static const char* Type;
            static ObjectConfig<Device> DevConf;
    };
}
#endif

