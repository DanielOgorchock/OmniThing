#ifndef OMNI_ARDUINOJSONCONFIG_H
#define OMNI_ARDUINOJSONCONFIG_H

#include <Arduino.h>

namespace omni
{
    const char Config_Network_Receiver[] PROGMEM =
        R"BEGIN({"NetworkReceiver": {"type": "NetworkManagerEsp8266", "port": 1337}})BEGIN";

    const char Config_Network_Sender[] PROGMEM =
        R"BEGIN({"NetworkSender": {"type": "NetworkManagerEsp8266", "ip": "192.168.2.200", "port": 39500}})BEGIN";

    const char Config_Composite_Periphs[] PROGMEM =
        R"BEGIN({"CompositePeriphs": [
                ]})BEGIN";

    const char Config_Input_Bools[] PROGMEM =
        R"BEGIN({"InputBools": [
                    {"type": "DigitalInputPinArduino", "pin": 15, "invert": false, "pullup": true}
                ]})BEGIN";

    const char Config_Input_Floats[] PROGMEM =
        R"BEGIN({"InputFloats": [
                    {"type": "AnalogInputPinArduino", "pin": 96, "minVoltage": 0, "maxVoltage": 5}
                ]})BEGIN";

    const char Config_Input_UInts[] PROGMEM =
        R"BEGIN({"InputUInts": [
                    {"type": "AnalogInputPinArduino", "pin": 97, "minVoltage": 0, "maxVoltage": 5}
                ]})BEGIN";

    const char Config_Output_Voids[] PROGMEM =
        R"BEGIN({"OutputVoids": [
                    {"type": "DigitalOutputPinArduino", "pin": 17, "initial": true, "invert": false}
                ]})BEGIN";

    const char Config_Output_Bools[] PROGMEM =
        R"BEGIN({"OutputBools": [
                    {"type": "DigitalOutputPinArduino", "pin": 18, "initial": true, "invert": false}
                ]})BEGIN";

    const char Config_Output_Floats[] PROGMEM =
        R"BEGIN({"OutputFloats": [
                ]})BEGIN";

    const char Config_Output_Strings[] PROGMEM =
        R"BEGIN({"OutputStrings": [
                ]})BEGIN";

    const char Config_Devices[] PROGMEM =
        R"BEGIN({"Devices": [
                    {"type": "Switch", "outputIndex": 0, "invert": false, "initial": false}
                    {"type": "ContactSensor", "inputIndex": 0, "invert": false, "constantPoll": true}
                ]})BEGIN";

    const char Config_Triggers[] PROGMEM =
        R"BEGIN({"Triggers": [
                    {"deviceIndex": 0, "interval": 30000, "command": "poll"},
                    {"deviceIndex": 1, "interval": 10000, "command": "poll"}
                ]})BEGIN";

}

#endif
