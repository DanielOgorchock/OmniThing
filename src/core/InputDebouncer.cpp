#include "InputDebouncer.h"

#include "Logger.h"
#include "OmniUtil.h"
#include "frozen.h"
#include <string.h>

namespace omni
{
//private

//protected

//public
    InputDebouncer::InputDebouncer(InputBool& input, DebounceMode mode, unsigned long duration):
        m_bFirst(true),
        m_Input(input),
        m_Mode(mode),
        m_nDebounceDuration(duration)
    {

    }

    InputDebouncer::~InputDebouncer()
    {

    }

    bool InputDebouncer::readBool()
    {
        unsigned long long time = getMillis();
        if(m_bFirst)
        {
            m_bFirst = false;
            m_bVal = m_Input.readBool();
            m_bPrev = m_bVal;
            m_nLastChanged = time;
            return m_bVal;
        }

        bool val = m_bVal;
        bool prev = m_bPrev;
        bool newVal = m_Input.readBool();
        if(newVal != prev)
            m_nLastChanged = time;
        unsigned long long timeDiff = time - m_nLastChanged;
        if(m_Mode == DebounceMode::Both)
        {
            if(newVal != val)
            {
                if(timeDiff >= m_nDebounceDuration)
                    m_bVal = newVal;
            }
        }
        else if(m_Mode == DebounceMode::Rising)
        {
            if(newVal == false && val == true) // don't debounce falling edge
                m_bVal = newVal;
            else if(newVal != val) // rising edge
            {
                if(timeDiff >= m_nDebounceDuration)
                    m_bVal = newVal;
            }
        }
        else // Falling
        {
            if(newVal == true && val == false) // don't debounce rising edge
                m_bVal = newVal;
            else if(newVal != val) // falling edge
            {
                if(timeDiff >= m_nDebounceDuration)
                    m_bVal = newVal;
            }
        }

        m_bPrev = newVal;
        return m_bVal;
    }

    InputBool* InputDebouncer::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);

        char* mode = nullptr;
        unsigned long duration = 0;
        DebounceMode debMode;
        json_token t;

        if(json_scanf(json, len, "{input: %T, mode: %Q, duration: %lu}", &t, &mode, &duration) != 3)
            return nullptr;

        if(!strcmp(mode, "Both"))
            debMode = DebounceMode::Both;
        else if(!strcmp(mode, "Rising"))
            debMode = DebounceMode::Rising;
        else if(!strcmp(mode, "Falling"))
            debMode = DebounceMode::Falling;
        else
        {
            LOG << "ERROR: invalid debounceMode=" << mode << Logger::endl;
            if(mode)
                delete[] mode;
            return nullptr;
        }
        delete[] mode;

        auto input = OmniThing::getInstance().buildInputBool(t);
        if(!input)
        {
            LOG << F("ERROR: Failed to create input\n");
            return nullptr;
        }

        auto d = new InputDebouncer(*input, debMode, duration);
        return d;
    }

    const char* InputDebouncer::Type = "InputDebouncer";
    ObjectConfig<InputBool> InputDebouncer::InputBoolConf(Type, createFromJson);
}
