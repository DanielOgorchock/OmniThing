#ifndef OMNI_OMNITHING_H
#define OMNI_OMNITHING_H

namespace omni
{
    class Device;
    class NetworkReceiver;
    class NetworkSender;
    class Logger;
    template <class T> class ObjectConfig;

    class Trigger // used by the scheduler
    {
        public:
            Device* dev;
            unsigned long interval;
            unsigned long long triggerTime;
            const char* cmd;
            const char* json;
            bool repeating;

            Trigger(): // just to allow the array to be built
                dev(nullptr),
                cmd(nullptr),
                json(nullptr)
            {

            }

            Trigger(Device* d, unsigned long inter, const char* cm, const char* js=nullptr, bool rep = true):
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
            void initScheduler();
            void parseJson(const char* json);
            Device* findDevice(unsigned int uid);

            unsigned short m_nDeviceCount;
            Device* m_Devices[10]; // TODO: use config file for number

            unsigned short m_nTriggerCount;
            Trigger m_Triggers[10]; // TODO: use config file for number

            unsigned short m_nDeviceConfigCount;
            ObjectConfig<Device>* m_DeviceConfigs[20]; //TODO: use config file for number

            NetworkReceiver* m_pNetworkReceiver;
            NetworkSender* m_pNetworkSender;
            Logger *m_pDefaultLogger; // doesn't do anything but should avoid crashes
            Logger* m_pLogger;


        public:
            static OmniThing& getInstance();
            void init();
            void run();

            void setNetworkReceiver(NetworkReceiver* nr);
            void setNetworkSender(NetworkSender* ns);
            void setLogger(Logger* l);

            Logger& getLogger();

            void sendJson(const char* json);
            void addDevice(Device* dev); 
            void addTrigger(Trigger& t);
            void addTrigger(Device* d, unsigned long interval, const char* cmd, const char* json = nullptr, bool repeat = true);
            void addDeviceConfig(ObjectConfig<Device>* dc);

    };
}

#endif
