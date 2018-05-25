#ifndef OMNI_OMNITHING_H
#define OMNI_OMNITHING_H



namespace omni
{
    class Device;
 
    class Trigger // used by the scheduler
    {
        public:
            Device* dev;
            unsigned long interval;
            unsigned long long triggerTime;
            char* cmd;
            char* json;
            bool repeating;

            Trigger(): // just to allow the array to be built
                dev(nullptr),
                cmd(nullptr),
                json(nullptr)
            {

            }

            Trigger(Device* d, unsigned long inter, char* cm, char* js=nullptr, bool rep = true):
                dev(d),
                interval(inter),
                cmd(cm),
                json(js),
                repeating(rep)
            {

            }
    };

    class OmniThing
    {
        private:
            OmniThing(); //don't allow manual creation

            void runScheduler();
            void initDevices();

            unsigned short m_nDeviceCount;
            Device* m_Devices[10]; // TODO: use config file for number

            unsigned short m_nTriggerCount;
            Trigger m_Triggers[10]; // TODO: use config file for number



        public:
            static OmniThing& getInstance();
            void init();
            void run();

            void sendJson(const char* json);
            void addDevice(Device* dev); 
            void addTrigger(Trigger& t);

    };
}

#endif
