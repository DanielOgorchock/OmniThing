#ifndef OMNI_DEVICE_H
#define OMNI_DEVICE_H
#include "ObjectConfig.h"
#include "Triggerable.h"

namespace omni
{
    class Device : public virtual Triggerable
    {
        private:
            unsigned short m_nUid;
            bool m_bRun; 

        protected:

        public:
            Device(bool run);
            virtual ~Device();

            virtual void recvJson(const char* cmd, const char* json) = 0;
            virtual void run() {}
            virtual void init() {}

            unsigned short getUid() const {return m_nUid;}
            bool isRunning() const {return m_bRun;}
            virtual const char* getType() const = 0;

            void setRun(bool b) {m_bRun = b;}

            virtual void trigger(void* arg);
    };
}

#endif
