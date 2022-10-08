#include "lu_7seg.h"
#include <I2Cdev.h>

#define write_to(addr, buff, len) \
    Wire.beginTransmission(addr); \
    Wire.write(buff, len); \
    Wire.endTransmission(true);

namespace lutil
{

static const uint8_t numbertable[] = {
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x77, /* a */
    0x7C, /* b */
    0x39, /* C */
    0x5E, /* d */
    0x79, /* E */
    0x71, /* F */
};

SevenSegment::SevenSegment(uint8_t address)
    : _address(address)
{
}

void SevenSegment::initialize()
{
    // Oscilator
    uint8_t buf[1];
    buf[0] = 0x21;
    write_to(_address, buf, 1);
    
    // --
    clear();
    write();

    set_blink(Off);
    set_brightness(15);
}

void SevenSegment::write()
{
    uint8_t buffer[17];
    buffer[0] = 0x00;

    for (uint8_t b = 0; b < 8; b++)
    {
        buffer[1 + 2 * b] = _buffer[b] & 0xFF;
        buffer[2 + 2 * b] = _buffer[b] >> 8;
    }

    write_to(_address, buffer, 17);
}

void SevenSegment::clear()
{
  for (uint8_t i = 0; i < 8; i++)
        _buffer[i] = 0;
}

void SevenSegment::set_brightness(uint8_t brightness)
{
    uint8_t buffer[1];
    buffer[0] = 0x0E |
                (brightness > 15) ? 15 : brightness;
    write_to(_address, buffer, 1);
}

void SevenSegment::set_blink(BlinkRate rate)
{
    uint8_t rr = (
        rate == Off ?
            0 :
        rate == Slow ?
            1 :
        rate == Medium ?
            2 :
        rate == Fast ?
            3 :
        0
    );

    I2Cdev::writeByte(
        _address,
        0x80,
        rr
    );

    uint8_t buffer[1];
    buffer[0] = 0x80 | 0x01 | rr;
    write_to(_address, buffer, 1);
}

void SevenSegment::enable_colon(bool enable)
{
    _buffer[2] = enable ? 0x02 : 0x00;
    
    uint8_t buf[3];
    buf[0] = 0x04;
    buf[1] = _buffer[2] & 0xFF;
    buf[2] = _buffer[2] >> 8;
    write_to(_address, buf, 3);
}

void SevenSegment::set_number(
    uint8_t digit, uint8_t number, bool dot)
{
    if (digit > 4)
        return;

    set_raw(digit, numbertable[number] | (dot << 7));
}

void SevenSegment::set_raw(uint8_t digit, uint8_t mask)
{
    if (digit > 4)
        return;

    _buffer[digit] = mask;
}


TimeCounter::TimeCounter(uint8_t address)
    : SevenSegment(address)
    , Processable()
    , _direction(Down)
{
}

void TimeCounter::set_direction(Direction direction)
{
    _direction = direction;
}

void TimeCounter::set_seconds(uint16_t seconds)
{
    _milliseconds = seconds * 1000;
    _last_mili = millis();
}

void TimeCounter::init()
{
    initialize();
    enable_colon();
    _last_seconds = 0;
}

void TimeCounter::process()
{
    if (_paused)
        return;

    uint32_t delta = millis() - _last_mili;
    _last_mili = millis();

    if (_direction == Down)
    {
        if (delta > _milliseconds)
        {
            // We're going to wrap, set to zero
            if (_milliseconds != 0)
                event(Zero);
            _milliseconds = 0;
        }
        else
        {
            _milliseconds -= delta;
        }
    }
    else
    {
        _milliseconds += delta;
    }

    uint32_t seconds = _milliseconds / 1000;
    if (_last_seconds == seconds)
        return; // Only write when we change

    _last_seconds = seconds;

    uint8_t remainder = seconds % 60;
    uint16_t mins = (seconds - remainder) / 60;
    mins = min(99U, mins);

    set_number(0, (mins / 10) % 10);
    set_number(1, (mins % 10));

    set_number(3, (remainder / 10) % 10);
    set_number(4, (remainder % 10));

    write();
}

void TimeCounter::pause()
{
    _paused = true;
}

void TimeCounter::resume()
{
    // Reset mili to only consume from here on
    _last_mili = millis();
    _paused = false;
    process();
}

}