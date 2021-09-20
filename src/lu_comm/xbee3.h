/**
 * XBee 3 API mode compatible construction.
 * 
 * For setting up xbee3: youtube.com/watch?v=AemzSO5EDy0
 * 
 * Once you have the basic down, this can help communicate between
 * series 3 XBee's. The common arduino xbee lib doesn't quite support
 * this proceedure.
 */
#pragma once
#include "lutil.h"
#include "lu_memory/managed_ptr.h"

namespace lutil
{

struct RawData
{
    uint8_t *data;
    uint16_t size;
};

/**
 * All API requests need an address.
 * 
 * .. code-block:: cpp
 *
 *     // Pulled from the MAC address in XCTU
 *     XBee3Address addr{ 0x0013A200, 0x41BDFAFB }; 
 */
struct XBee3Address
{
    uint32_t high;
    uint32_t low;
};

/**
 * A request packages up a payload and address for the actual
 * module to send. This can format the 
 */
class XBee3Request
{
public:
    XBee3Request(const XBee3Address &address);

    // Set the request payload. This is copied into the request
    // and cleaned up once the request has been destroyed.
    void setPayload(uint8_t *payload, uint16_t size);
    void setPayload(const managed_data &payload);

    // Data formated in a way that XBee 3 modules can consume
    RawData data() const;

private:
    XBee3Address _address;
    managed_data _payload;
};

/**
 * When an XBee3 module obtains a payload, this object can
 * decompse the data.
 */
class Xbee3Response
{
public:
    enum Status
    {
        Valid,
        InProgress,
        Invalid,
        TooLarge,
    };

    Xbee3Response();

    // Return true if there is a valid paylaod
    bool isValid() const;

    uint8_t checksum() const { return _checksum; }
    uint8_t apiId() const { return _api_id; }
    uint16_t size() const { return _size; }
    const managed_data &payload() const { return _payload; }

    XBee3Address sender() const;
    managed_data data() const;

protected:
    // Internal API for building the response;
    friend class XBee3;
    uint8_t &operator[] (uint16_t pos);
    void allocate();
    void add_checksum(uint8_t cs);
    void set_api_id(uint8_t id);
    void add_size(uint16_t s);

private:
    XBee3Address _origin;
    managed_data _payload;
    uint8_t _api_id;
    uint8_t _checksum;
    uint16_t _size;
};

/**
 * Wrapper class for an XBee Series 3.
 */
class XBee3
{
public:
    XBee3();

    Stream *stream() const;
    void setStream(Stream *stream);

    // Send a payload request to another XBee3
    void send(const XBee3Request &request);

    // Check for an incoming transmission
    Xbee3Response::Status poll();

    // Get the latest tranmission (if any)
    const Xbee3Response &response() const;

private:
    Stream *_stream = nullptr;

    // When we poll, if the response is valid, this
    // is populated
    Xbee3Response _latest_response;

    // While we're building a response we populate
    // this (pushing it to the _latest_response once
    // all data is collected)
    Xbee3Response _working_response;
    uint32_t _pos;
};

} // namespace lutil