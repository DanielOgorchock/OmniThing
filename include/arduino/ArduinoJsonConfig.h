#ifndef OMNI_ARDUINOJSONCONFIG_H
#define OMNI_ARDUINOJSONCONFIG_H

#include <Arduino.h>

namespace omni
{
    const char Config_Network_Receiver[] PROGMEM =
        R"BEGIN({"NetworkReceiver": {} })BEGIN";

    const char Config_Network_Sender[] PROGMEM =
        R"BEGIN({"NetworkSender": {} })BEGIN";

    const char Config_Composite_Periphs[] PROGMEM =
        R"BEGIN({"CompositePeriphs": [
                ]})BEGIN";

    const char Config_Devices[] PROGMEM =
        R"BEGIN({"Devices": [
                ]})BEGIN";
}

#endif
