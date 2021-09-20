/**
 * Exmaple of using the XBee3 toolkit for reading
 * messages.
 * 
 * For setting up xbee3: youtube.com/watch?v=AemzSO5EDy0
 */
#include "lutil.h"
#include "lu_comm/xbee3.h"

// If you don't have a teensy/mega, something with
// multiple hardware
// SoftwareSerial use_serial(4, 5);

// Radio module
lutil::XBee3 xbee;

// Address to send payloads to. This is the MAC addr
lutil::XBee3Address address{
    0x0013A200, // high
    0x41BDFAFB  // low
};

void setup()
{
    Serial.begin(9600);

    Serial2.begin(9600); // or use_serial
    xbee.setStream(&Serial2); // or &use_serial
}

void loop()
{
    // See if there's something to do
    if (xbee.poll() == lutil::Xbee3Response::Valid)
    {
        // The actual reponse object
        const lutil::Xbee3Response &res = xbee.response();

        //
        // data() is the actual information
        // payload() is the raw data (includes address())
        //
        lutil::managed_data content = res.data();
        for (uint16_t i = 0; i < content.size(); i++)
            Serial.print((char)content[i]);
        Serial.print('\n');
    }
}
