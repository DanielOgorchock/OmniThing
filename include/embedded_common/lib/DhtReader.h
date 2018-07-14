// A port of Rob Tillaart's Arduino DHT library to OmniThing
// https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTstable

#ifndef OMNI_DHTREADER_H
#define OMNI_DHTREADER_H

#include "CompositePeripheral.h"
#include "ObjectConfig.h"
#include "DigitalInputPin.h"
#include "DigitalOutputPin.h"

#define DHTLIB_OK               0
#define DHTLIB_ERROR_CHECKSUM  -1
#define DHTLIB_ERROR_TIMEOUT   -2
#define DHTLIB_INVALID_VALUE   -999

#define DHTLIB_DHT11_WAKEUP     18
#define DHTLIB_DHT_WAKEUP       1

#define DHTLIB_TIMEOUT          100 // (microseconds)

#define MIN_DHT_POLL_MS         2000

namespace omni
{
    class DhtReader : public CompositePeripheral
    {
        private:
            unsigned char bits[5]; //buffer to receive data
            float humidity;
            float temperature;

            float last_humidity;
            float last_temperature;

            unsigned long long poll_timer;

            unsigned short pin;
            bool is_dht11;
            bool internal_pullup;
            DigitalInputPin* inputPin;
            DigitalOutputPin* outputPin;

            unsigned int word(unsigned char h, unsigned char l);

            int _readSensor(unsigned char wakeupDelay);

            int read11();
            int read();

            int read21() {return read();}
            int read22() {return read();}
            int read33() {return read();}
            int read44() {return read();}

            int getVals();

        public:
            DhtReader(const char* name, unsigned short pinNum, bool dht11, bool pullup);
            virtual ~DhtReader();

            virtual bool getFloat(const char* name, float& f);

            static CompositePeripheral* createFromJson(const char* json);

            //params
            static const char* Param_Temperature;
            static const char* Param_Humidity;

            static const char* Type;

            static ObjectConfig<CompositePeripheral> PeriphConf;
    };
}
#endif
