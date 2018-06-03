#ifndef OMNI_OMNITHING_H
#define OMNI_OMNITHING_H

#ifndef OMNI_NOT_ARDUINO
    #include <Arduino.h>
#endif

#include "FixedArray.h"
#include <string.h>

namespace omni
{
    class Triggerable;
    class Device;

    class CompositePeripheral;

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

    class Event
    {
        public:
            Device* src;
            char event[20];

            Event():
                src(nullptr)
            {
                event[0] = 0;
            }

            Event(Device* s, const char* e):
                src(s)
            {
                strncpy(event, e, 20);
            }

    };
    bool operator==(const Event& l, const Event&r);

    class Subscription
    {
        public:
            Event event;
            Device* subscriber;
            char cmd[10];

            Subscription():
                subscriber(0)
            {
                cmd[0] = 0;
            }

            Subscription(const Event& e, Device* s, const char* c):
                event(e),
                subscriber(s)
            {
                strncpy(cmd, c, 10);
            }

            Subscription(const Subscription& s):
                event(s.event)
            {   
                strncpy(cmd, s.cmd, 10);
            }
    };

    class Trigger // used by the scheduler
    {
        public:
            Triggerable* target;
            unsigned long interval;
            unsigned long long triggerTime;
            unsigned long offset;
            void* arg;
            bool repeating;

            Trigger(): // just to allow the array to be built
                target(nullptr)
            {

            }

            Trigger(Triggerable* t, unsigned long inter, void* ar, bool rep = true, unsigned long offs=0):
                target(t),
                interval(inter),
                arg(ar),
                repeating(rep),
                offset(offs)
            {
            }
    };
#define OMNI_OUTGOING_JSON_BUF_SIZE     400
#define OMNI_MAX_DEVICES                40

#define OMNI_MAX_COMPOSITE_PERIPHS      20

#define OMNI_MAX_INPUT_BOOLS            20
#define OMNI_MAX_INPUT_FLOATS           20
#define OMNI_MAX_INPUT_UINTS            20

#define OMNI_MAX_OUTPUT_VOIDS           20
#define OMNI_MAX_OUTPUT_BOOLS           20
#define OMNI_MAX_OUTPUT_FLOATS          20
#define OMNI_MAX_OUTPUT_STRINGS         20

#define OMNI_MAX_TRIGGERS               40

#define OMNI_MAX_EVENTS                 20
#define OMNI_MAX_SUBSCRIPTIONS          20

#define OMNI_MAX_DEVICE_CONFIGS         40

#define OMNI_MAX_COMPOSITE_PERIPH_CONFIGS 20

#define OMNI_MAX_INPUT_BOOL_CONFIGS     20
#define OMNI_MAX_INPUT_FLOAT_CONFIGS    20
#define OMNI_MAX_INPUT_UINT_CONFIGS     20

#define OMNI_MAX_OUTPUT_VOID_CONFIGS    20
#define OMNI_MAX_OUTPUT_BOOL_CONFIGS    20
#define OMNI_MAX_OUTPUT_FLOAT_CONFIGS   20
#define OMNI_MAX_OUTPUT_STRING_CONFIGS  20

#define OMNI_MAX_NETWORK_RECEIVER_CONFIGS   5
#define OMNI_MAX_NETWORK_SENDER_CONFIGS     5



    class OmniThing
    {
        private:
            OmniThing(); //don't allow manual creation
            OmniThing(const OmniThing& ot);

            void printContainersDebug();

            void runScheduler();
            void checkEvents();

            void initDevices();
            void initScheduler();
            void parseJson(const char* json);
            Device* findDevice(unsigned int uid);

            void initOutgoingJson();
            void sendOutgoingJson();
            void addOutgoingJson(const char* json);

            char m_OutgoingJsonBuf[OMNI_OUTGOING_JSON_BUF_SIZE];

            // Devices and Inputs/Outputs
            FixedArray<Device*,         OMNI_MAX_DEVICES>           m_Devices;

            FixedArray<CompositePeripheral*, OMNI_MAX_COMPOSITE_PERIPHS> m_CompositePeriphs;

            FixedArray<InputBool*,      OMNI_MAX_INPUT_BOOLS>       m_InputBools;
            FixedArray<InputFloat*,     OMNI_MAX_INPUT_FLOATS>      m_InputFloats;
            FixedArray<InputUInt*,      OMNI_MAX_INPUT_UINTS>       m_InputUInts;

            FixedArray<OutputVoid*,     OMNI_MAX_OUTPUT_VOIDS>      m_OutputVoids;
            FixedArray<OutputBool*,     OMNI_MAX_OUTPUT_BOOLS>      m_OutputBools;
            FixedArray<OutputFloat*,    OMNI_MAX_OUTPUT_FLOATS>     m_OutputFloats;
            FixedArray<OutputString*,   OMNI_MAX_OUTPUT_STRINGS>    m_OutputStrings;

            // Triggers
            FixedArray<Trigger, OMNI_MAX_TRIGGERS> m_Triggers;

            // Subscriptions/Events
            FixedArray<Event,           OMNI_MAX_EVENTS>        m_Events;
            FixedArray<Subscription,    OMNI_MAX_SUBSCRIPTIONS> m_Subscriptions;

            unsigned int m_nTriggerStringsCount;
            char m_TriggerStrings [OMNI_MAX_TRIGGERS] [10];

            //Configs
            FixedArray<ObjectConfig<Device>*,       OMNI_MAX_DEVICE_CONFIGS>            m_DeviceConfigs;

            FixedArray<ObjectConfig<CompositePeripheral>*, OMNI_MAX_COMPOSITE_PERIPH_CONFIGS> m_CompositePeriphConfigs;

            FixedArray<ObjectConfig<InputBool>*,    OMNI_MAX_INPUT_BOOL_CONFIGS>        m_InputBoolConfigs;
            FixedArray<ObjectConfig<InputFloat>*,   OMNI_MAX_INPUT_FLOAT_CONFIGS>       m_InputFloatConfigs;
            FixedArray<ObjectConfig<InputUInt>*,    OMNI_MAX_INPUT_UINT_CONFIGS>        m_InputUIntConfigs;

            FixedArray<ObjectConfig<OutputVoid>*,   OMNI_MAX_OUTPUT_VOID_CONFIGS>       m_OutputVoidConfigs;
            FixedArray<ObjectConfig<OutputBool>*,   OMNI_MAX_OUTPUT_BOOL_CONFIGS>       m_OutputBoolConfigs;
            FixedArray<ObjectConfig<OutputFloat>*,  OMNI_MAX_OUTPUT_FLOAT_CONFIGS>      m_OutputFloatConfigs;
            FixedArray<ObjectConfig<OutputString>*, OMNI_MAX_OUTPUT_STRING_CONFIGS>     m_OutputStringConfigs;

            FixedArray<ObjectConfig<NetworkReceiver>*, OMNI_MAX_NETWORK_RECEIVER_CONFIGS> m_NetworkReceiverConfigs;
            FixedArray<ObjectConfig<NetworkSender>*, OMNI_MAX_NETWORK_SENDER_CONFIGS>     m_NetworkSenderConfigs;

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

            bool addCompositePeriph(CompositePeripheral* e);

            bool addInputBool(InputBool* e);
            bool addInputFloat(InputFloat* e);
            bool addInputUInt(InputUInt* e);
            
            bool addOutputVoid(OutputVoid* e);
            bool addOutputBool(OutputBool* e);
            bool addOutputFloat(OutputFloat* e);
            bool addOutputString(OutputString* e);

            bool addTrigger(Trigger& t);
            bool addTrigger(Triggerable* t, unsigned long interval, void* arg, bool repeat = true, unsigned long offset = 0);

            bool addEvent(Device* src, const char* event);
            bool addSubscription(Subscription& sub);

            bool addDeviceConfig(ObjectConfig<Device>* dc);
            bool addCompositePeriphConfig(ObjectConfig<CompositePeripheral>* c);
            bool addInputBoolConfig(ObjectConfig<InputBool>* c);
            bool addInputFloatConfig(ObjectConfig<InputFloat>* c);
            bool addInputUIntConfig(ObjectConfig<InputUInt>* c);
            bool addOutputVoidConfig(ObjectConfig<OutputVoid>* c);
            bool addOutputBoolConfig(ObjectConfig<OutputBool>* c);
            bool addOutputFloatConfig(ObjectConfig<OutputFloat>* c);
            bool addOutputStringConfig(ObjectConfig<OutputString>* c);

            bool addNetworkReceiverConfig(ObjectConfig<NetworkReceiver>* c);
            bool addNetworkSenderConfig(ObjectConfig<NetworkSender>* c);

            const FixedArray<Device*, OMNI_MAX_DEVICES>& getDevices() {return m_Devices;}

            const FixedArray<CompositePeripheral*, OMNI_MAX_COMPOSITE_PERIPHS>& getCompositePeriphs() {return m_CompositePeriphs;}

            const FixedArray<InputBool*, OMNI_MAX_INPUT_BOOLS>& getInputBools() {return m_InputBools;}
            const FixedArray<InputFloat*, OMNI_MAX_INPUT_FLOATS>& getInputFloats() {return m_InputFloats;}
            const FixedArray<InputUInt*, OMNI_MAX_INPUT_UINTS>& getInputUInts() {return m_InputUInts;}

            const FixedArray<OutputVoid*, OMNI_MAX_OUTPUT_VOIDS>& getOutputVoids() {return m_OutputVoids;}
            const FixedArray<OutputBool*, OMNI_MAX_OUTPUT_BOOLS>& getOutputBools() {return m_OutputBools;}
            const FixedArray<OutputFloat*, OMNI_MAX_OUTPUT_FLOATS>& getOutputFloats() {return m_OutputFloats;}
            const FixedArray<OutputString*, OMNI_MAX_OUTPUT_STRINGS>& getOutputStrings() {return m_OutputStrings;}

            bool loadJsonConfig(const char* json);


    };
}

#endif
