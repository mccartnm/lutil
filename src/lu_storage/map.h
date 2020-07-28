#pragma once
#include "lutil.h"

#define DEFAULT_SIZE 5

namespace lutil {

namespace LUTIL_VERSION {

/*
    _Very_ lightweight map template with linked lists

    - Both keys and values have to have default
      constructors
    - Currently - there is no removal :D TODO
*/
template<typename KEY, typename VALUE>
class Map {
public:
    explicit Map(size_t size) {
        _count = size;
        _size = _count;

        _keys = new KEY[_size];
        _values = new VALUE[_size];
        reset();
    }

    Map() {
        _count = 0;
        _size = DEFAULT_SIZE;

        _keys = new KEY[_size];
        _values = new VALUE[_size];
        reset();
    }

    ~Map() {
        delete [] _keys;
        delete [] _values;
    }

    VALUE &operator[](KEY key) {
        for (size_t i = 0; i < _count; i++) {
            if (_keys[i] == key) {
                return _values[i];
            }
        }
        // Error::print("Invalid Key Error");
        return _values[0];
    }

    const VALUE &operator[](const KEY &key) const {
        // We should obviously make this a hash but I'm
        // not in the mood for the extra math atm.
        for (size_t i = 0; i < _count; i++) {
            if (_keys[i] == key) {
                return _values[i];
            }
        }
        // Error::print("Invalid Key Error");
        return VALUE();
    }

    size_t count() const { return _count; }

    void insert(KEY key, VALUE value) {
        if (_size == _count) {
            _expand(_size);
        }

        _keys[_count] = key;
        _values[_count++] = value;
    }

    bool contains(KEY key) {
        for (size_t i = 0; i < _count; i++) {
            if (_keys[i] == key) {
                return true;
            }
        }
        return false;
    }

    void reset() {
        for (size_t i = 0; i < _size; ++i) {
            _keys[i] = KEY();
            _values[i] = VALUE();
        }
    }


private:
    void _expand(size_t size) {
        auto new_size = _size + size;

        KEY *new_keys = new KEY[new_size];
        VALUE *new_values = new VALUE[new_size];

        memcpy(new_keys, _keys, _size);
        delete [] _keys;
        
        memcpy(new_values, _values, _size);
        delete [] _values;

        _keys = new_keys;
        _values = new_values;
        _size = new_size;
    }

    size_t _size;
    size_t _count;

    KEY *_keys;
    VALUE *_values;
};


}

}