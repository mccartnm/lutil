#pragma once
#include "lutil.h"
#include "lu_math/axis.h"

#define LUTIL_DEBUG

namespace lutil
{


/*
    Generalized interface for sensors. Can handle multiple types of
    information from a single element.

    The SensorData typename is the data structure that you want to
    populate with your sensor. This can be useful for multi-sensor
    projects that fill the same structure with different information
    (e.g. Gryo, Accel, Temp, Baro, etc.).

    struct MySensorData
    {
        lutil::Axis accel;
        lutil::Axir rotation;
        float temp;
    }

    // Base class
    class MySensorBase : public lutil::lu_Sensor<MySensorData>
    {
        // ...
    }

    // Implementation for specific sensor
    class MyGryo : public MySensorBase
    {
        // Implement read operation:
        void read(MySensorData &into, int value_flags) override;
    }
*/
template<typename SensorData>
class lu_Sensor
{
public:

    /** Active status of a sensor */
    enum Status
    {
        Inactive    = 0x01,
        Calibrating = 0x02,
        Ready       = 0x04,
        Active      = 0x08,
        Error       = 0x10,
    };

    /** Type of protocol this sensor uses */
    enum Protocol
    {
        I2C,
        SPI
    };

    lu_Sensor(Protocol protocol, int address)
        : _protocol(protocol)
        , _address(address)
        , _initialized(false)
    {}

    virtual ~lu_Sensor() {}

    void init()
    {
        if (_initialized) {
            return;
        }

#       ifdef LUTIL_DEBUG
            Serial.print("Initialze: ");
            Serial.println(id());
#       endif

        _initialize();
        _initialized = true;

#       ifdef LUTIL_DEBUG
        Serial.println("Initialized");
#       endif
    }


    /* -----------------------------------------------------------
     *  Virtual interface - overload per-sensor class type.
     ---------------------------------------------------------- */

    virtual const char *id() const = 0;
    virtual int supported_data() const = 0;
    virtual void read(SensorData &into, int value_flags) = 0;

    // In the event we have error data for a given sensor type we
    // can return that information.
    virtual Axis error(int value_type) const {
        return { 0.0, 0.0, 0.0 };
    }

    // Test/complete all checkouts for this device.
    // - This should not block but rather use timing to
    //   determine when to do something.
    virtual Status test(float start_time, uint16_t iteration) {
        return Status::Ready;
    }

protected:
    int address() const { return _address; }

    /* -----------------------------------------------------------
     *  Virtual interface - overload per-sensor class
     ---------------------------------------------------------- */

    virtual void _initialize() = 0;

private:
    Protocol _protocol;
    int _address;
    bool _initialized;
};

}
