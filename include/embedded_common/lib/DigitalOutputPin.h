#ifndef OMNI_DIGITALOUTPUTPIN_H
#define OMNI_DIGITALOUTPUTPIN_H

#include "OutputVoid.h"
#include "OutputBool.h"

namespace omni
{
    class DigitalOutputPin : public OutputVoid, public OutputBool
    {
        private:
            unsigned short m_nPin;
            bool m_bValue;
            bool m_bInvertLogic;

        protected:
            virtual void writePin(bool b) = 0;

        public:
            DigitalOutputPin(unsigned short pin, bool initialValue, bool invertLogic);

            virtual ~DigitalOutputPin();

            virtual bool configure() = 0;

            unsigned short getPin() const {return m_nPin;}
            bool isInverted() const {return m_bInvertLogic;}
            bool getValue() const {return m_bValue;}

            void writeVoid() final;
            void writeBool(bool b) final;

            static DigitalOutputPin* create(unsigned short pin, bool initialValue, bool invertLogic);
    };
}

#endif

