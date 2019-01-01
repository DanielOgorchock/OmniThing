// A port of Rob Tillaart's Arduino DHT library to OmniThing
// https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTstable

#include "DhtReader.h"
#include "OmniThing.h"
#include "Logger.h"
#include "OmniUtil.h"

#include "frozen.h"
#include <string.h>

namespace omni
{
//private
    unsigned int DhtReader::word(unsigned char h, unsigned char l)
    {
        return ( ( (int) h ) << 8 ) | l;
    }

    int DhtReader::_readSensor(unsigned char wakeupDelay)
    {
        // INIT BUFFERVAR TO RECEIVE DATA
        unsigned char mask = 128;
        unsigned char idx = 0;

        // EMPTY BUFFER
        for (unsigned char i = 0; i < 5; i++) bits[i] = 0;

        // REQUEST SAMPLE
        outputPin->configure();
        outputPin->writeBool(false);
        sleepMillisBusy(wakeupDelay);
        inputPin->configure();
        inputPin->readBool();
        sleepMicrosBusy(40);

        unsigned long long timer;
        // GET ACKNOWLEDGE or TIMEOUT
        timer = getMicros();
        while(inputPin->readBool() == false)
        {
            if (getMicros() - timer >= DHTLIB_TIMEOUT) return DHTLIB_ERROR_TIMEOUT;
        }

        timer = getMicros();
        while(inputPin->readBool() == true)
        {
            if (getMicros() - timer >= DHTLIB_TIMEOUT) return DHTLIB_ERROR_TIMEOUT;
        }

        // READ THE OUTPUT - 40 BITS => 5 BYTES
        for (unsigned char i = 40; i != 0; i--)
        {
            timer = getMicros();
            while(inputPin->readBool() == false)
            {
                if (getMicros() - timer >= DHTLIB_TIMEOUT) return DHTLIB_ERROR_TIMEOUT;
            }

            unsigned long long t = getMicros();

            timer = getMicros();
            while(inputPin->readBool() == true)
            {
                if (getMicros() - timer >= DHTLIB_TIMEOUT) return DHTLIB_ERROR_TIMEOUT;
            }

            if ((getMicros() - t) > 40)
            {
                bits[idx] |= mask;
            }
            mask >>= 1;
            if (mask == 0)   // next byte?
            {
                mask = 128;
                idx++;
            }
        }

        return DHTLIB_OK;
    }

    int DhtReader::read11()
    {
        // READ VALUES
        int rv = _readSensor(DHTLIB_DHT11_WAKEUP);
        if (rv != DHTLIB_OK)
        {
            humidity    = DHTLIB_INVALID_VALUE; // invalid value, or is NaN prefered?
            temperature = DHTLIB_INVALID_VALUE; // invalid value
            return rv;
        }

        // CONVERT AND STORE
        humidity    = bits[0];  // bits[1] == 0;
        temperature = bits[2];  // bits[3] == 0;

        // TEST CHECKSUM
        unsigned char sum = bits[0] + bits[1] + bits[2] + bits[3];
        if (bits[4] != sum)
        {
          return DHTLIB_ERROR_CHECKSUM;
        }
        return DHTLIB_OK;
    }

    int DhtReader::read()
    {
        // READ VALUES
        int rv = _readSensor(DHTLIB_DHT_WAKEUP);
        if (rv != DHTLIB_OK)
        {
            humidity    = DHTLIB_INVALID_VALUE;  // invalid value, or is NaN prefered?
            temperature = DHTLIB_INVALID_VALUE;  // invalid value
            return rv; // propagate error value
        }

        // CONVERT AND STORE
        humidity = word(bits[0], bits[1]) * 0.1;
        temperature = word(bits[2] & 0x7F, bits[3]) * 0.1;
        if (bits[2] & 0x80)  // negative temperature
        {
            temperature = -temperature;
        }

        // TEST CHECKSUM
        unsigned char sum = bits[0] + bits[1] + bits[2] + bits[3];
        if (bits[4] != sum)
        {
            return DHTLIB_ERROR_CHECKSUM;
        }
        return DHTLIB_OK;
    }

    int DhtReader::getVals()
    {
        if(getMillis() - poll_timer >= MIN_DHT_POLL_MS)
        {
            poll_timer = getMillis();
        }
        else
        {
            return DHTLIB_OK;
        }

        int ret;
        if(is_dht11)
            ret = read11();
        else
            ret = read();

        if(ret != DHTLIB_OK)
        {
            humidity = last_humidity;
            temperature = last_temperature;
            LOG << F("ERROR: error reading temp/humidity (") << ret << F(")\n");
        }
        else
        {
            last_humidity = humidity;
            last_temperature = temperature;
        }

        return ret;
    }

//public
    DhtReader::DhtReader(const char* name, unsigned short pinNum, bool dht11, bool pullup):
        CompositePeripheral(name),
        poll_timer(getMillis() - MIN_DHT_POLL_MS),
        pin(pinNum),
        is_dht11(dht11),
        internal_pullup(pullup),
        inputPin(DigitalInputPin::create(pin, false, internal_pullup)),
        outputPin(DigitalOutputPin::create(pin, true, false))
    {

    }

    DhtReader::~DhtReader()
    {

    }

    bool DhtReader::getFloat(const char* name, float& f)
    {
        bool ret;
        if(!strcmp(name, Param_Temperature))
        {
            ret = (getVals() == DHTLIB_OK);
            f = temperature;
            return ret;
        }
        else if(!strcmp(name, Param_Humidity))
        {
            ret = (getVals() == DHTLIB_OK);
            f = humidity;
            return ret;
        }
        else
        {
            LOG << F("ERROR: ") << name << F(" is not a param of ") << Type << F("\n");
            f = DHTLIB_INVALID_VALUE;
            return false;
        }
    }

    CompositePeripheral* DhtReader::createFromJson(const char* json)
    {
        const char* name = parseName(json);
        if(!name)
            return nullptr;

        unsigned int len = strlen(json);
        unsigned short pin;
        bool dht11 = false;
        bool pullup = false;

        if(json_scanf(json, len, "{pin: %hu}", &pin) != 1)
        {
            LOG << F("ERROR: pin number required\n");
            return nullptr;
        }

        json_scanf(json, len, "{dht11: %B}", &dht11);
        json_scanf(json, len, "{pullup: %B}", &pullup);

        return new DhtReader(name, pin, dht11, pullup);
    }

    //statics
    const char* DhtReader::Param_Temperature = "temperature";
    const char* DhtReader::Param_Humidity    = "humidity";

    const char* DhtReader::Type = "DhtReader";

    ObjectConfig<CompositePeripheral> DhtReader::PeriphConf(Type, createFromJson);
}
