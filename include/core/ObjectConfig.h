#ifndef OMNI_OBJECTCONFIG_H
#define OMNI_OBJECTCONFIG_H

#include "Logger.h"

namespace omni
{
    template <class T2> void registerConfig(T2* conf)
    {
        LOG << F("No register overload configured for type: ") << conf->getType() << F("\n");
    }
    void registerConfig(ObjectConfig<Device>* conf);
    void registerConfig(ObjectConfig<InputBool>* conf);
    void registerConfig(ObjectConfig<InputFloat>* conf);
    void registerConfig(ObjectConfig<InputUInt>* conf);
    void registerConfig(ObjectConfig<OutputVoid>* conf);
    void registerConfig(ObjectConfig<OutputBool>* conf);
    void registerConfig(ObjectConfig<OutputFloat>* conf);
    void registerConfig(ObjectConfig<OutputString>* conf);
    void registerConfig(ObjectConfig<NetworkReceiver>* conf);
    void registerConfig(ObjectConfig<NetworkSender>* conf);
    void registerConfig(ObjectConfig<CompositePeripheral>* conf);

    template <class T>
    class ObjectConfig
    {
        private:
            const char* m_Type;
            T* (*m_CreateFromJson) (const char*);

            
        protected:
        public:
            ObjectConfig(const char* type, T* (*createFromJson) (const char*)):
                m_Type(type),
                m_CreateFromJson(createFromJson)
            {
                registerConfig(this); 
            }
            ~ObjectConfig()
            {

            }
             
            T* createFromJson(const char* json)
            {
                return m_CreateFromJson(json);
            }

            const char* getType() const {return m_Type;}
    };
}

#endif
