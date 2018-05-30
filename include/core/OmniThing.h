#ifndef OMNI_OMNITHING_H
#define OMNI_OMNITHING_H

#include "FixedArray.h"
#include <string.h>

namespace omni
{
    class Device;

    class InputBool;
    class InputFloat;
    class InputUInt;

    class OutputVoid;
    class OutputBool;
    class OutputFloat;
    class OutputString;

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
            char cmd[10];
            const char* json;
            bool repeating;

            Trigger(): // just to allow the array to be built
                dev(nullptr),
                json(nullptr)
            {

            }

            Trigger(Device* d, unsigned long inter, const char* cm, const char* js=nullptr, bool rep = true):
                dev(d),
                interval(inter),
                json(js),
                repeating(rep)
            {
                strncpy(cmd, cm, 9);
                cmd[9] = 0;
            }
    };

    class OmniThing
    {
        private:
            OmniThing(); //don't allow manual creation

            void printContainersDebug();

            void runScheduler();
            void initDevices();
            void initScheduler();
            void parseJson(const char* json);
            Device* findDevice(unsigned int uid);

            // Devices and Inputs/Outputs
            FixedArray<Device*,         10> m_Devices;

            FixedArray<InputBool*,      10> m_InputBools;
            FixedArray<InputFloat*,     10> m_InputFloats;
            FixedArray<InputUInt*,      10> m_InputUInts;

            FixedArray<OutputVoid*,     10> m_OutputVoids;
            FixedArray<OutputBool*,     10> m_OutputBools;
            FixedArray<OutputFloat*,    10> m_OutputFloats;
            FixedArray<OutputString*,   10> m_OutputStrings;

            // Triggers
            FixedArray<Trigger, 10> m_Triggers;

            //Configs
            FixedArray<ObjectConfig<Device>*,       20> m_DeviceConfigs;

            FixedArray<ObjectConfig<InputBool>*,    10> m_InputBoolConfigs;
            FixedArray<ObjectConfig<InputFloat>*,   10> m_InputFloatConfigs;
            FixedArray<ObjectConfig<InputUInt>*,    10> m_InputUIntConfigs;

            FixedArray<ObjectConfig<OutputVoid>*,   10> m_OutputVoidConfigs;
            FixedArray<ObjectConfig<OutputBool>*,   10> m_OutputBoolConfigs;
            FixedArray<ObjectConfig<OutputFloat>*,  10> m_OutputFloatConfigs;
            FixedArray<ObjectConfig<OutputString>*, 10> m_OutputStringConfigs;

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

            bool addDevice(Device* dev); 

            bool addInputBool(InputBool* e);
            bool addInputFloat(InputFloat* e);
            bool addInputUInt(InputUInt* e);
            
            bool addOutputVoid(OutputVoid* e);
            bool addOutputBool(OutputBool* e);
            bool addOutputFloat(OutputFloat* e);
            bool addOutputString(OutputString* e);

            bool addTrigger(Trigger& t);
            bool addTrigger(Device* d, unsigned long interval, const char* cmd, const char* json = nullptr, bool repeat = true);

            bool addDeviceConfig(ObjectConfig<Device>* dc);
            bool addInputBoolConfig(ObjectConfig<InputBool>* c);
            bool addInputFloatConfig(ObjectConfig<InputFloat>* c);
            bool addInputUIntConfig(ObjectConfig<InputUInt>* c);
            bool addOutputVoidConfig(ObjectConfig<OutputVoid>* c);
            bool addOutputBoolConfig(ObjectConfig<OutputBool>* c);
            bool addOutputFloatConfig(ObjectConfig<OutputFloat>* c);
            bool addOutputStringConfig(ObjectConfig<OutputString>* c);


            const auto& getDevices() {return m_Devices;}

            const auto& getInputBools() {return m_InputBools;}
            const auto& getInputFloats() {return m_InputFloats;}
            const auto& getInputUInts() {return m_InputUInts;}

            const auto& getOutputVoids() {return m_OutputVoids;}
            const auto& getOutputBools() {return m_OutputBools;}
            const auto& getOutputFloats() {return m_OutputFloats;}
            const auto& getOutputStrings() {return m_OutputStrings;}

            bool loadJsonConfig(const char* json);


    };
}

#endif
