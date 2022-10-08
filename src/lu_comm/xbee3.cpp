#include "xbee3.h"

#define kStartByte 0x7E
#define kApiIndex 3

// Allow up to 65520 byte payload (should never get close)
#define kMaxFrameDataSize 0xFFF0

namespace lutil
{

XBee3Request::XBee3Request(const XBee3Address &address)
    : _address(address)
    , _payload()
{}

void XBee3Request::setPayload(uint8_t *payload, uint16_t size)
{
    size_t s = size;
    _payload.reset(payload, s); // copies data
}

void XBee3Request::setPayload(const managed_data &payload)
{
    _payload = payload;
}

/**
 * Header:
 * 
 * |   byte   |   value    |                 Detail                  |
 * +----------+------------+-----------------------------------------+
 * |     0    |     7E     | Always 7E                               |
 * |    1-2   |   XX  XX   | Full size (at _least_ 00 0E)            |
 * |     3    |     10     | API Frame type (currently always 10)    |
 * |     4    |     00     | Frame ID - don't request reponse        |
 * |    5-12  | 64bit ADDR | Address of XBee to send this request to |
 * |   13-14  |    FF FE   | FF FE because we're only using 64 bit   |
 * |    15    |     00     | Broadcast radius (adjust?)              |
 * |    16    |     00     | Options (currently not used)            |
 * | 17-(N-1) |     --     | RF Data - proper payload                |
 * |   (N-1)  |     XX     | Checksum                                |
 * +----------+------------+-----------------------------------------+
 * 
 * The checksum is:
 *      FF minus 8 bit sum of bytes between length and checksum field
 */
RawData XBee3Request::data() const
{
    uint16_t headerSize = 4; // 0, 1-2, N-1

    // 0x000E is the smallest payload (with addresses, frame ID, etc)
    uint16_t bufferSize = 0x000E + (uint16_t)_payload.size() - 1;

    uint8_t *output = new uint8_t[bufferSize + headerSize];
    uint8_t *ptr = output;

    // -- HEADER -- //
    *ptr++ = 0x7E; // Start Delimiter

    *ptr++ = (bufferSize >> 8) & 0xFF; // Length high
    *ptr++ = bufferSize & 0xFF; // Length low

    // -- FRAME DATA -- //
    *ptr++ = 0x10; // API Frame Type

    *ptr++ = 0x00; // Frame ID

    // Address High
    *ptr++ = (_address.high >> 24) & 0xFF;
    *ptr++ = (_address.high >> 16) & 0xFF;
    *ptr++ = (_address.high >> 8) & 0xFF;
    *ptr++ = (_address.high & 0xFF);

    // Address Low
    *ptr++ = (_address.low >> 24) & 0xFF;
    *ptr++ = (_address.low >> 16) & 0xFF;
    *ptr++ = (_address.low >> 8) & 0xFF;
    *ptr++ = _address.low & 0xFF;

    *ptr++ = 0xFF; // 16bit addr high
    *ptr++ = 0xFE; // 16bit addr low

    *ptr++ = 0x00; // Broadcast Radius

    *ptr++ = 0x00; // Options

    // -- PAYLOAD -- //
    memcpy(ptr, _payload.get(), _payload.size());
    ptr += _payload.size() - 1;

    // -- CHECKSUM -- //
    uint8_t checksum = 0;
    for (uint8_t *i = (output + headerSize - 1); i != ptr; i++)
        checksum += *i;
    *ptr = (uint8_t)0xFF - checksum;

    return { output, (uint16_t)(bufferSize + headerSize) };
}

// --------------------------------------------------------------------

Xbee3Response::Xbee3Response()
    : _origin({0, 0})
    , _api_id(0)
    , _checksum(0)
    , _size(0)
{
}

bool Xbee3Response::isValid() const
{
    return _payload.get();
}

void Xbee3Response::add_checksum(uint8_t cs)
{
    _checksum += cs;
}

void Xbee3Response::set_api_id(uint8_t id)
{
    _api_id = id;
}

void Xbee3Response::add_size(uint16_t s)
{
    _size += s;
}

void Xbee3Response::allocate()
{
    // Assume we have the size already
    uint8_t *newData = new uint8_t[_size];
    _payload.reset(newData, _size);
}

XBee3Address Xbee3Response::sender() const
{
    XBee3Address addr{ 0, 0 };

    addr.high += uint32_t(_payload[0]) << 24;
    addr.high += uint32_t(_payload[1]) << 16;
    addr.high += uint32_t(_payload[2]) << 8;
    addr.high += uint32_t(_payload[3]);

    addr.low += uint32_t(_payload[4]) << 24;
    addr.low += uint32_t(_payload[5]) << 16;
    addr.low += uint32_t(_payload[6]) << 8;
    addr.low += uint32_t(_payload[7]);

    return addr;
}

managed_data Xbee3Response::data() const
{
    // Account for the address, 16bit addr, api id
    static size_t s_offset = (sizeof(uint32_t) * 2) + \
                             sizeof(uint16_t) + \
                             sizeof(uint8_t);

    managed_data content;

    // Data
    uint8_t *c = new uint8_t[
        _payload.size() - s_offset
    ];
    size_t realSize = _payload.size() - s_offset;

    memcpy(
        c,
        _payload.get() + s_offset,
        realSize
    );

    content.take(c, realSize);
    return content;
}

uint8_t &Xbee3Response::operator[] (uint16_t pos)
{
    return _payload[pos];
}

// --------------------------------------------------------------------

XBee3::XBee3()
    : _pos(0)
    , _timeout(1000)
{
}

Stream *XBee3::stream() const
{
    return _stream;
}

void XBee3::setStream(Stream *input, Stream *output)
{
    _stream = input;
    _output_stream = output ? output : input;
}

void XBee3::send(const XBee3Request &request)
{
    if (!_output_stream)
        return;

    RawData d = request.data();
    _output_stream->write(d.data, d.size);
}

Xbee3Response::Status XBee3::poll()
{
    if (!_stream)
        return Xbee3Response::Invalid;

    if (_latest_response.isValid())
        _latest_response = Xbee3Response();

    while (_stream->available() > 0)
    {
        uint8_t b = _stream->read();

        if (_pos == 0)
        {
            _next_timeout = millis() + _timeout;
            _working_response = Xbee3Response();

            if (b != kStartByte)
                continue; // Not a valid start

            _pos++;
            continue;
        }

		// Checksum!
		if (_pos >= kApiIndex)
        {
			_working_response.add_checksum(b);
        }

        switch (_pos)
        {
        case 1:
        {
            // length high
            _working_response.add_size((uint16_t)b << 8);
            break;
        }
        case 2:
        {
            // length low
            _working_response.add_size(b);

            // Once we have the payload size, we can allocate
            _working_response.allocate();
            break;
        }
        case 3:
        {
            _working_response.set_api_id(b);
            uint16_t workpos = _pos - (kApiIndex + 1);
            _working_response[workpos] = b;
            break;
        }
        default:
        {
            // Here, we start the actual payload collection
            if (_pos > kMaxFrameDataSize)
            {
                // Invalid payload (too large!)
                _pos = 0;
                return Xbee3Response::TooLarge;
            }

            // We have new data to punch into the managed
            // data container of our working response
            uint16_t working_position = _pos - (kApiIndex + 1);
            _working_response[working_position] = b;

            if (_pos >= (uint16_t)(_working_response.size() + kApiIndex - 1))
            {
                // We're at the end of the packet. Check
                // on the checksum
                if (_working_response.checksum() == (uint8_t)0xFF)
                {
                    // We have a valid payload!
                    _latest_response = _working_response;
                    return Xbee3Response::Valid;
                }
                _pos = 0; // Even an invalid payload needs to be reset
            }
            break;
        }
        }
        _pos++;
    }

    if (_pos > 0 && _next_timeout < millis())
    {
        // We've timed out this request. Reset
        _pos = 0;
        return Xbee3Response::Timeout;
    }

    if (_pos > 0)
        return Xbee3Response::InProgress;
    else
        return Xbee3Response::None;
}

void XBee3::setTimeout(size_t timeout)
{
    _timeout = timeout;
}

const Xbee3Response &XBee3::response() const
{
    return _latest_response;
}

} // namespace lutil