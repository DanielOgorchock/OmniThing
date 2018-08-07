#include "RelativeHumidityMeasurement.h"
namespace omni
{
//private
//protected
//public
    RelativeHumidityMeasurement::RelativeHumidityMeasurement(FloatMeasurement& fm):
        FloatMeasurement(fm)
    {
        read();
    }

    RelativeHumidityMeasurement::~RelativeHumidityMeasurement()
    {

    }

    Device* RelativeHumidityMeasurement::createFromJson(const char* json)
    {
        FloatMeasurement* fm = FloatMeasurement::createFromJson(json, Attribute);
        if(!fm)
            return nullptr;

        auto d = new RelativeHumidityMeasurement(*fm);
        delete fm;
        if(!d->parseMisc(json))
            return nullptr;
        return d;
    }

    // commands

    //events

    const char* RelativeHumidityMeasurement::Attribute = "humidity";

    const char* RelativeHumidityMeasurement::Type = "RelativeHumidityMeasurement";
    ObjectConfig<Device> RelativeHumidityMeasurement::DevConf(Type, createFromJson);
}
