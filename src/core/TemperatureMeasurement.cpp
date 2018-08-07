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
        FloatMeasurement* fm = FloatMeasurement::createFromJson(json, Attribute);
        if(!fm)
            return nullptr;

        auto d = new TemperatureMeasurement(*fm);
        delete fm;
        if(!d->parseMisc(json))
            return nullptr;
        return d;
    }

    // commands

    //events

    const char* TemperatureMeasurement::Attribute = "temperature";

    const char* TemperatureMeasurement::Type = "TemperatureMeasurement";
    ObjectConfig<Device> TemperatureMeasurement::DevConf(Type, createFromJson);
}
