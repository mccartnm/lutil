#pragma once
#include "lutil.h"
#include "lu_process/process.h"
#include <Wire.h>

namespace lutil
{

/**
 * I2C 7 segment display. Aligns with the 
 */
class SevenSegment
{
public:
    SevenSegment(uint8_t address = 0x70);

    void initialize();

    // 0-15
    void set_brightness(uint8_t brightness);

    enum BlinkRate
    {
        Off,
        Slow,
        Medium,
        Fast
    };
    void set_blink(BlinkRate rate);

    void enable_colon(bool enable = true);

    void set_number(uint8_t digit, uint8_t number, bool dot = false);
    void set_raw(uint8_t digit, uint8_t mask);

    void write();
    void clear();

private:
    uint8_t _address;
    uint16_t _buffer[8];
};


/**
 * Seven Segment display with the ability to count time
 * 
 * Use set_direction(TimeCounter::Up) to use as a
 * stopwatch, otherwise set_direction(TimeCounter::Down),
 * which is the default, will tick towards 0.
 */
class TimeCounter
    : public SevenSegment
    , public Processable
{
public:
    enum Event { Zero, };
    enum Direction { Down, Up };

    TimeCounter(uint8_t address = 0x70);

    void set_direction(Direction direction);
    void set_seconds(uint16_t seconds);
    void init() override;
    void process() override;

    void pause();
    void resume();

private:
    bool _paused;
    Direction _direction;
    uint32_t _last_mili;
    uint32_t _milliseconds;
    uint32_t _last_seconds;
};

}