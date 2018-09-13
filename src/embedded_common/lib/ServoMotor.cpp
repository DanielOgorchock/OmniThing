#include "ServoMotor.h"

#include "OmniThing.h"
#include <string.h>
#include "frozen.h"

namespace omni
{
//private
    void ServoMotor::writeFloatNoRevert(float percent)
    {
        if(m_bNoStartup)
            return;

        attach();
        if(percent < 0.f)
            percent = 0;
        if(percent > 100.f)
            percent = 100;

        unsigned long pulseWidth = (percent / 100.f) * (m_nMaxPulse - m_nMinPulse) + m_nMinPulse;
        writePulse(pulseWidth);
        LOG << F("Setting Servo pulseWidth to: ") << pulseWidth << Logger::endl;

        if(m_bShutoff)
        {
            ++m_nShutoffId;
            OmniThing::getInstance().addTrigger(this, m_nShutoffTime, Cmd_Detach, false);
            //LOG << F("Setting up shutoff trigger timed=") << m_nShutoffTime << F("\n");
        }
    }

    void ServoMotor::attach()
    {
    #ifndef OMNI_NOT_ARDUINO
        m_Servo.attach(m_nPin);
    #endif
    }

    void ServoMotor::detach()
    {
    #ifndef OMNI_NOT_ARDUINO
        m_Servo.detach();
    #elif defined(OMNI_PLAT_RPI)
        /*int ret =*/ gpioServo(m_nPin, 0);
        /*if(ret)
            LOG << F("ERROR detaching servo\n");*/
    #endif
    }

    void ServoMotor::writePulse(unsigned long pulseWidth)
    {
    #ifndef OMNI_NOT_ARDUINO
        m_Servo.writeMicroseconds(pulseWidth);
    #elif defined(OMNI_PLAT_RPI)
        int ret = gpioServo(m_nPin, pulseWidth);
        if(ret)
            LOG << F("ERROR writing to servo\n");
    #endif
    }

    char ServoMotor::Cmd_Detach[]  = "detach";
    char ServoMotor::Cmd_Revert[]  = "revert";
    char ServoMotor::Cmd_Startup[] = "startup";
//protected
//public
    ServoMotor::ServoMotor(unsigned short pin, float initialPercent, bool revert, unsigned long revertTime, bool shutoff, unsigned long shutoffTime, bool noStartup, unsigned long minPulse, unsigned long maxPulse) :
        m_nPin(pin),
        m_fInitial(initialPercent),
        m_bRevert(revert),
        m_nRevertTime(revertTime),
        m_bShutoff(shutoff),
        m_nShutoffTime(shutoffTime),
        m_bNoStartup(noStartup),
        m_nMinPulse(minPulse),
        m_nMaxPulse(maxPulse),
        m_nShutoffId(0),
        m_nShutoffIdTriggers(0),
        m_nRevertId(0),
        m_nRevertIdTriggers(0)
    {
        writeFloat(initialPercent);
        OmniThing::getInstance().addTrigger(this, 0, Cmd_Startup, false);
    }

    ServoMotor::~ServoMotor()
    {

    }

    void ServoMotor::writeFloat(float percent)
    {
        if(m_bNoStartup)
            return;
        writeFloatNoRevert(percent);

        if(m_bRevert) // set up revert trigger
        {
            ++m_nRevertId;
            OmniThing::getInstance().addTrigger(this, m_nRevertTime, Cmd_Revert, false);
            //LOG << F("Setting up revert trigger time=") << m_nRevertTime << F("\n");
        }
    }

    void ServoMotor::trigger(void* arg)
    {
        char* cmd = static_cast<char*>(arg);
        if(strcmp(cmd, Cmd_Detach))
            LOG << F("Servo trigger cmd=") << cmd << Logger::endl;

        if(!strcmp(cmd, Cmd_Detach))
        {
            if(++m_nShutoffIdTriggers == m_nShutoffId)
            {
                //LOG << F("Servo shutoff triggered\n");
                detach();
            }
            /*else
                LOG << F("Other shutoff events in progress\n");*/
        }
        else if(!strcmp(cmd, Cmd_Revert))
        {
            if(++m_nRevertIdTriggers == m_nRevertId)
            {
                //LOG << F("Servo revert triggered\n");
                writeFloatNoRevert(m_fInitial);
            }
            /*else
                LOG << F("Other revert events in progress\n");*/
        }
        else if(!strcmp(cmd, Cmd_Startup))
        {
            m_bNoStartup = false;
        }
    }

    OutputFloat* ServoMotor::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        unsigned short pin;
        float initialPercent = 50.f;
        bool revert = false;
        unsigned long revertTime = 0;
        bool shutoff = false;
        unsigned long shutoffTime = 0;
        bool noStartup = false;

        unsigned long minPulse = 544;
        unsigned long maxPulse = 2400;

        if(json_scanf(json, len, "{pin: %hu}", &pin) != 1)
        {
            return nullptr;
        }

        if(json_scanf(json, len, "{revertTime: %lu}", &revertTime) == 1) // optional param
        {
            LOG << F("Servo - enabling revertTime=") << revertTime << Logger::endl;
            revert = true;
        }

        if(json_scanf(json, len, "{shutoffTime: %lu}", &shutoffTime) == 1) // optional param
        {
            shutoff = true;
            LOG << F("Servo - enabling shutoffTime=") << shutoffTime << Logger::endl;
        }

        json_scanf(json, len, "{initialPercent: %f}", &initialPercent); // optional param
        json_scanf(json, len, "{noStartup: %B}", &noStartup);
        json_scanf(json, len, "{minPulse: %lu}", &minPulse);
        json_scanf(json, len, "{maxPulse: %lu}", &maxPulse);

        return new ServoMotor(pin, initialPercent, revert, revertTime, shutoff, shutoffTime, noStartup, minPulse, maxPulse);
    }

    const char* ServoMotor::Type = "ServoMotor";
    ObjectConfig<OutputFloat> ServoMotor::OutputFloatConf(Type, createFromJson);

}
