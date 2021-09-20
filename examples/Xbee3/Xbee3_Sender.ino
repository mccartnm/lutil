/**
 * Exmaple of using the XBee3 toolkit.
 * 
 * For setting up xbee3: youtube.com/watch?v=AemzSO5EDy0
 * 
 * Then using it like so:
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
    // Make a message
    lutil::XBee3Request request(address);
    uint8_t data[] = "Hello, World!";
    request.setPayload(
        data,
        sizeof(data)
    );

    // Fire it off...
    xbee.send(request);
    delay(5000);
}
