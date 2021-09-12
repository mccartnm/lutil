#pragma once
#include "lutil.h"

namespace lutil {

/*
    Interface agnostic memory controller class. Overload and
    implment _read and _write for your specific case.

    .. warning::

        This class currenly 
*/

template<typename C>
class abstract_memory {
public:
    abstract_memory(int chip_address)
        : _chip_address(chip_address)
    {}

    /*
        Read back a specific item from memory. This will inspect the
        sizeof(T) and return that.

        .. code-block:: cpp

            MyStruct ms = my_memory.read<MyStruct>(addr);
            int32_t num = my_memory.read<int32_t>(other_addr);
    */
    template<typename T>
    T read(C address) const {
        T val;
        address = address;
        _read(address, (uint8_t *)&val, (C)sizeof(T));
        return val;
    }


    /*
        Record a specific object into memory. Return the next address
        after this object.

        .. code-block:: cpp

            // write one struct
            int16_t new_addr = my_memory.write<MyStruct>(addr, ms);
            
            // write another just after it
            my_memory.write<MyStruct>(new_addr, other_ms);
    */
    template<typename T>
    C write(C address, T value) {
        address = address;
        C size = (C)sizeof(T);
        _write(address, (uint8_t *)&value, size);
        return address + size;
    }

protected:
    int chip_address() const { return _chip_address; }

    /* -----------------------------------------------------------
     *  Virtual interface - overload per-memory class type.
     ---------------------------------------------------------- */

    /*
        Execute a write operation with the given address, data,
        and size.
    */
    virtual void _write(C address,
                        uint8_t *data,
                        C size) = 0;


    /*
        Execute a read operation with the given address, data,
        and size.
    */
    virtual void _read(C address,
                       uint8_t *into,
                       C size) const = 0;


private:
    int _chip_address;

};


/* Use for memory controllers that are limited to uint16_t */
class uint16_memory : public abstract_memory<uint16_t>
{
public:
    uint16_memory(int chip_address)
        : abstract_memory<uint16_t>(chip_address)
    {}
};

/* Use for memory controllers that are limited to uint32_t */
class uint32_memory : public abstract_memory<uint32_t>
{
public:
    uint32_memory(int chip_address)
        : abstract_memory<uint32_t>(chip_address)
    {}
};

}
