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
        unsigned int len = strlen(json);
        json_token t;

        FloatMeasurement* fm = FloatMeasurement::createFromJson(json, Attribute);
        if(!fm)
            return nullptr;

        auto d = new RelativeHumidityMeasurement(*fm);
        delete fm;
        d->parseMisc(json);
        return d;
    }

    // commands

    //events

    const char* RelativeHumidityMeasurement::Attribute = "humidity";

    const char* RelativeHumidityMeasurement::Type = "RelativeHumidityMeasurement";
    ObjectConfig<Device> RelativeHumidityMeasurement::DevConf(Type, createFromJson);
}
