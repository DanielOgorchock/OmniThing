#include "TemperatureMeasurement.h"
namespace omni
{
//private
//protected
//public
    TemperatureMeasurement::TemperatureMeasurement(FloatMeasurement& fm):
        FloatMeasurement(fm)
    {
        read();
    }

    TemperatureMeasurement::~TemperatureMeasurement()
    {

    }

    Device* TemperatureMeasurement::createFromJson(const char* json)
    {
        unsigned int len = strlen(json);
        json_token t;

        FloatMeasurement* fm = FloatMeasurement::createFromJson(json, Attribute);
        if(!fm)
            return nullptr;

        auto d = new TemperatureMeasurement(*fm);
        delete fm;
        d->parseMisc(json);
        return d;
    }

    // commands

    //events

    const char* TemperatureMeasurement::Attribute = "temperature";

    const char* TemperatureMeasurement::Type = "TemperatureMeasurement";
    ObjectConfig<Device> TemperatureMeasurement::DevConf(Type, createFromJson);
}
