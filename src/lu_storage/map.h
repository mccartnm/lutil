#pragma once
#include "lutil.h"

#define DEFAULT_SIZE 5

namespace lutil {

namespace LUTIL_VERSION {

/*
    Lightweight map template with linked lists. Not as performant
    as other models but very light on the compilation side

    - VALUE needs a default constructor
*/
template<typename KEY, typename VALUE>
class Map {
public:
    explicit Map(size_t size) {
        _count = size;
        _size = _count;

        _keys = new KEY[_size];
        _values = new VALUE[_size];
    }

    Map() {
        _count = 0;
        _size = DEFAULT_SIZE;

        _keys = new KEY[_size];
        _values = new VALUE[_size];
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
        insert(key, VALUE());
        return _values[_count - 1]; // ?!?
    }

    const VALUE &operator[](const KEY &key) const {
        // We should obviously make this a hash but I'm
        // not in the mood for the extra math atm.
        for (size_t i = 0; i < _count; i++) {
            if (_keys[i] == key) {
                return _values[i];
            }
        }
        insert(key, VALUE());
        return _values[_count - 1];
    }

    size_t count() const { return _count; }

    void insert(KEY key, VALUE value) {
        int idx = _index_of(key);
        if (idx < 0) {
            if (_size == _count) {
                _expand(_size);
            }
            _keys[_count] = key;
            _values[_count++] = value;
        }
        else {
            // replace an existing value
            _values[idx] = value;
        }
    }

    bool contains(KEY key) const {
        return (_index_of(key) >= 0);
    }


    void remove(KEY key) {
        int index = _index_of(key);
        if (index < 0)
            return; // Don't have it

        _remove(index);
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

    int _index_of(KEY key) const {
        for (int i = 0; i < _count; i++) {
            if (_keys[i] == key) {
                return i;
            }
        }
        return -1;
    }

    void _remove(size_t index) {
        ::memmove(
            &_keys[index],
            &_keys[index + 1],
            (_count - index - 1) * sizeof(KEY)
        );
        ::memmove(
            &_values[index],
            &_values[index + 1],
            (_count - index - 1) * sizeof(VALUE)
        );
        _count--;
    }

    size_t _size;
    size_t _count;

    KEY *_keys;
    VALUE *_values;
};


}

}