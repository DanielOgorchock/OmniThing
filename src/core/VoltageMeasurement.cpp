#include "VoltageMeasurement.h"
namespace omni
{
//private
//protected
//public
    VoltageMeasurement::VoltageMeasurement(FloatMeasurement& fm):
        FloatMeasurement(fm)
    {
        read();
    }

    VoltageMeasurement::~VoltageMeasurement()
    {

    }

    Device* VoltageMeasurement::createFromJson(const char* json)
    {
        FloatMeasurement* fm = FloatMeasurement::createFromJson(json, Attribute);
        if(!fm)
            return nullptr;

        auto d = new VoltageMeasurement(*fm);
        delete fm;
        if(!d->parseMisc(json))
            return nullptr;
        return d;
    }

    // commands

    //events

    const char* VoltageMeasurement::Attribute = "voltage";

    const char* VoltageMeasurement::Type = "VoltageMeasurement";
    ObjectConfig<Device> VoltageMeasurement::DevConf(Type, createFromJson);
}
