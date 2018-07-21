#include "ServoArduino.h"

#include "OmniThing.h"
#include <string.h>
#include "frozen.h"

namespace omni
{
//private
    void ServoArduino::writeFloatNoRevert(float percent)
    {
        if(m_bNoStartup)
            return;

        m_Servo.attach(m_nPin);
        if(percent < 0.f)
            percent = 0;
        if(percent > 100.f)
            percent = 100;

        int angle = percent*180/100.f;
        m_Servo.write(angle);
        LOG << F("Setting Servo to angle: ") << angle << Logger::endl;

        if(m_bShutoff)
        {
            OmniThing::getInstance().addTrigger(this, m_nShutoffTime, Cmd_Detach, false);
        }
    }

    char* ServoArduino::Cmd_Detach  = "detach";
    char* ServoArduino::Cmd_Revert  = "revert";
    char* ServoArduino::Cmd_Startup = "startup";
//protected
//public
    ServoArduino::ServoArduino(unsigned short pin, float initialPercent, bool revert, unsigned long revertTime, bool shutoff, unsigned long shutoffTime, bool noStartup) :
        m_Servo(),
        m_nPin(pin),
        m_fInitial(initialPercent),
        m_bRevert(revert),
        m_nRevertTime(revertTime),
        m_bShutoff(shutoff),
        m_nShutoffTime(shutoffTime),
        m_bNoStartup(noStartup)
    {
        writeFloat(initialPercent);
        OmniThing::getInstance().addTrigger(this, 0, Cmd_Startup, false);
    }

    ServoArduino::~ServoArduino()
    {

    }

    void ServoArduino::writeFloat(float percent)
    {
        if(m_bNoStartup)
            return;
        writeFloatNoRevert(percent);

        if(m_bRevert) // set up revert trigger
        {
            OmniThing::getInstance().addTrigger(this, m_nRevertTime, Cmd_Revert, false);
        }
    }

    void ServoArduino::trigger(void* arg)
    {
        char* cmd = static_cast<char*>(arg);

        if(!strcmp(cmd, Cmd_Detach))
        {
            m_Servo.detach();
        }
        else if(!strcmp(cmd, Cmd_Revert))
        {
            writeFloatNoRevert(m_fInitial);
        }
        else if(!strcmp(cmd, Cmd_Startup))
        {
            m_bNoStartup = false;
        }
    }

    OutputFloat* ServoArduino::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        unsigned short pin;
        float initialPercent = 50.f;
        bool revert = false;
        unsigned long revertTime = 0;
        bool shutoff = false;
        unsigned long shutoffTime = 0;
        bool noStartup = false;

        if(json_scanf(json, len, "{pin: %u}", &pin) != 1)
        {
            return nullptr;
        }

        if(json_scanf(json, len, "{revertTime: %lu}", &revertTime) == 1) // optional param
        {
            revert = true;
        }

        if(json_scanf(json, len, "{shutoffTime: %lu}", &shutoffTime) == 1) // optional param
        {
            shutoff = true;
        }

        json_scanf(json, len, "{initialPercent: %f}", &initialPercent); // optional param
        json_scanf(json, len, "{noStartup: %B}", &noStartup);

        return new ServoArduino(pin, initialPercent, revert, revertTime, shutoff, shutoffTime, noStartup);
    }

    const char* ServoArduino::Type = "ServoArduino";
    ObjectConfig<OutputFloat> ServoArduino::OutputFloatConf(Type, createFromJson);

}
