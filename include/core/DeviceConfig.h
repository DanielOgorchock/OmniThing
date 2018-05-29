#ifndef OMNI_DEVICECONFIG_H
#define OMNI_DEVICECONFIG_H


namespace omni
{
    class Device;
    class DeviceConfig
    {
        private:
            const char* m_Type;
            Device* (*m_CreateFromJson) (const char*);
        protected:
        public:
            DeviceConfig(const char* type, Device* (*createFromJson) (const char*));
            ~DeviceConfig();
             
            Device* createFromJson(const char* json);

            const char* getType() const {return m_Type;}
    };
}

#endif
