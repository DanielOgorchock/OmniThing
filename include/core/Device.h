#ifndef OMNI_DEVICE_H
#define OMNI_DEVICE_H
#include "ObjectConfig.h"
#include "Triggerable.h"

namespace omni
{
    class Device : public virtual Triggerable
    {
        private:
            bool m_bRun;
            const char* m_Name;

            static const char EmptyString[];
        protected:

        public:
            Device(bool run);
            virtual ~Device();

            virtual void recvJson(const char* cmd, const char* json) = 0;
            virtual void run() {}
            virtual void init() {}

            const char* getName() const {return m_Name;}
            bool isRunning() const {return m_bRun;}
            virtual const char* getType() const = 0;

            void setRun(bool b) {m_bRun = b;}

            virtual void trigger(void* arg);
            void emit(const char* event);

            bool parseMisc(const char* json);

    };
}

#endif
