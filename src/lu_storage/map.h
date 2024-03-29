#pragma once
#include "lutil.h"

#define DEFAULT_SIZE 5

namespace lutil {

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

    Map(const Map<KEY, VALUE> &other)
        : _size(other._size)
        , _count(other._count)
    {
        _from_other(other);
    }

    Map &operator= (const Map<KEY, VALUE> &other) {
        _count = other._count;
        _size = other._size;
        _from_other(other);
        return *this;
    }

    VALUE &operator[](KEY key) {
        for (size_t i = 0; i < _count; i++) {
            if (_keys[i] == key) {
                return _values[i];
            }
        }
        auto v = VALUE();
        insert(key, v); // EMPTY MAP MIGHT EXPLODE WHEN DESTROYED __START_HERE__
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

    void insert(KEY key, const VALUE &value) {
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


    KEY key_from_value(VALUE val) const {
        for (size_t i = 0; i < _count; i++) {
            if (_values[i] == val) {
                return _keys[i];
            }
        }
        return KEY();
    }


    // ----------------------------------------------------------------
    // ITERATION

    class Iterator {
    public:
        Iterator(Map<KEY, VALUE> *map, int index = 0)
            : _map(map)
            , _index(index)
        {}

        Iterator(const Iterator &it)
            : _map(it._map)
            , _index(it._index)
        {}

        KEY &key() {
            return _map->_keys[_index];
        }

        VALUE &value() {
            return _map->_values[_index];
        }

        Iterator &operator++ () {
            _index++;
            return *this;
        }

        Iterator operator++ (int) {
            Iterator output(*this);
            ++(*this);
            return output;
        }

        bool has_next() const {
            return _index < _map->_count;
        }

        bool operator== (const Iterator &other) const {
            return (this->_map == other._map &&
                    this->_index == other._index);
        }

        bool operator!= (const Iterator &other) const {
            return (this->_map != other._map ||
                    this->_index != other._index);
        }

        bool operator!= (const Iterator *other) const {
            return (this != *other);
        }

    private:
        Map<KEY, VALUE> *_map;
        size_t _index;
    };

    Iterator begin() {
        return Iterator(this);
    }

    Iterator end() {
        return Iterator(this, (int)_count);
    }


private:
    void _expand(size_t size) {
        auto new_size = _size + size;

        KEY *new_keys = new KEY[new_size];
        VALUE *new_values = new VALUE[new_size];

        for (size_t i = 0; i < _count; i++)
        {
            new_keys[i] = _keys[i];
            new_values[i] = _values[i];
        }
        // ::memcpy(new_keys, _keys, _size);
        // delete [] _keys;
        
        // ::memcpy(new_values, _values, _size);
        // delete [] _values;

        _keys = new_keys;
        _values = new_values;
        _size = new_size;
    }

    int _index_of(KEY key) const {
        for (int i = 0; i < (int)_count; i++) {
            if (_keys[i] == key) {
                return i;
            }
        }
        return -1;
    }

    void _remove(size_t index) {
        for (size_t i = index; i < _count - 1; i++)
        {
            _keys[i] = _keys[i + 1];
            _values[i] = _values[i + 1];
        }
        _keys[_count - 1] = KEY();
        _values[_count - 1] = VALUE();
        _count--;
    }

    void _from_other(const Map<KEY, VALUE> &other) {
        _size = other._size;
        _count = other._count;
        _keys = new KEY[_size];
        _values = new VALUE[_size];

        for (size_t i = 0; i < _count; i++)
        {
            _keys[i] = other._keys[i];
            _values[i] = other._values[i];
        }
    }

    size_t _size;
    size_t _count;

    KEY *_keys;
    VALUE *_values;
};


}
