#pragma once
#include "lutil.h"

namespace lutil {

template<typename T>
inline void swap_ptr(T **a, T **b)
{
    T *temp = *a;
    *a = *b;
    *b = temp;
}

template<typename T>
inline void swap_val(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template<class T>
class managed_ptr {

public:
    managed_ptr()
        : _refs(nullptr)
        , _ptr(nullptr)
    {}

    managed_ptr(T *ptr)
        : _refs(new int(1))
        , _ptr(ptr)
    {}

    managed_ptr(const managed_ptr &ptr)
        : _refs(ptr._refs)
        , _ptr(ptr._ptr)
    {
        if (*this)
            (*_refs)++;
    }

    // Desconstructor cleans out reference
    ~managed_ptr() { immolate(); }

    operator bool() const noexcept {
        return _ptr != nullptr;
    }

    T *operator ->() const {
        return _ptr;
    }

    T &operator *() const {
        return *_ptr;
    }

    T *get() const {
        return _ptr;
    }

    managed_ptr &operator= (managed_ptr other) {
        other.swap(*this);
        return *this;
    }

    void swap(managed_ptr &other) {
        swap_ptr<int>(&_refs, &other._refs);
        swap_ptr<T>(&_ptr, &other._ptr);
    }

    void immolate() {
        if (*this) {
            --(*_refs);

            if (*_refs <= 0) {
                delete _refs;
                delete _ptr;
            }

            _refs = nullptr;
            _ptr = nullptr;
        }
    }

    void reset(T *new_data) {
        if (*this) {
            immolate();
        }
        _refs = new int(1);
        _ptr = new_data;
    }

private:
    int *_refs;
    T *_ptr;

};


/*
    Lightweight system for handling fixed length character sets
*/
class managed_string : public managed_ptr<char> {
public:
    managed_string()
        : managed_ptr()
        , _size(0)
    {
    }

    managed_string(const char *data)
        : managed_ptr()
        , _size(0)
    {
        *this = data; // Let the operator have it
    }

    explicit managed_string(size_t size)
        : managed_ptr(new char[size + 1])
        , _size(size)
    {
        memset(get(), '\0', _size + 1);
    }

    bool operator==(const char *data) const {
        return (strncmp(c_str(), data, _size) == 0);
    }

    bool operator==(const managed_string &other) const {
        if (_size != other._size)
            return false;
        return (strncmp(c_str(), other.c_str(), _size) == 0);
    }

    managed_string &operator= (const char *data) {
        if (strlen(data) > _size) {
            // We need to expand
            _size = strlen(data);
            reset(new char[_size + 1]);
        }
        if (_size > 0) {
            ::memset(get(), '\0', _size + 1);
            ::memcpy(get(), data, _size);
        }
        return *this;
    }

    const char *c_str() const { return get(); }
    size_t size() const { return _size; }

private:
    size_t _size;
};


class managed_data : public managed_ptr<uint8_t> {
public:
    managed_data()
        : managed_ptr()
        , _size(0)
    {
    }

    class iterator
    {
    public:
        iterator(managed_data *d, size_t index = 0)
            : _d(d)
            , _index((int)index)
        {}

        iterator(const iterator &it)
            : _d(it._d)
            , _index(it._index)
        {}

        bool operator== (const iterator &other) const {
            return (_d == other._d) && (_index == other._index);
        }

        bool operator!= (const iterator &other) const {
            return !(this == &other);
        }

        uint8_t &operator* ()
        {
            return *(_d->get() + _index);
        }

        iterator operator+(size_t offset)
        {
            return iterator(_d, _index + offset);
        }

        iterator operator-(size_t offset)
        {
            return iterator(_d, _index - offset);
        }

        iterator &operator++()
        {
            _index++;
            return *this;
        }

        iterator operator++(int)
        {
            iterator it(*this);
            _index++;
            return it;
        }
    private:
        managed_data *_d;
        int _index;
    };

    class const_iterator
    {
    public:
        const_iterator(const managed_data *d, size_t index = 0)
            : _d(d)
            , _index((int)index)
        {}

        const_iterator(const const_iterator &it)
            : _d(it._d)
            , _index(it._index)
        {}

        bool operator== (const const_iterator &other) const {
            return (_d == other._d) && (_index == other._index);
        }

        bool operator!= (const const_iterator &other) const {
            return !(this == &other);
        }

        const uint8_t &operator* ()
        {
            return *(_d->get() + _index);
        }

        const_iterator operator+(size_t offset)
        {
            return const_iterator(_d, _index + offset);
        }

        const_iterator operator-(size_t offset)
        {
            return const_iterator(_d, _index - offset);
        }

        const_iterator &operator++()
        {
            _index++;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator it(*this);
            _index++;
            return it;
        }
    private:
        const managed_data *_d;
        int _index;
    };

    iterator begin() {
        return iterator(this);
    }

    const_iterator begin() const {
        return const_iterator(this);
    }

    iterator end() {
        return iterator(this, (int)_size);
    }

    const_iterator end() const {
        return const_iterator(this, (int)_size);
    }

    explicit managed_data(size_t size)
        : managed_ptr(new uint8_t[size])
        , _size(size)
    {
        zero();
    }

    uint8_t operator[](int index) const {
        if (index >= (int)_size) { // :|
            return 0; // Failsafe
        }
        return *(get() + index);
    }

    uint8_t &operator[](int index) {
        return *(get() + index);
    }

    void reset(uint8_t *data, size_t size) {
        _size = size;

        uint8_t *d = new uint8_t[_size];
        memcpy(d, data, _size);

        managed_ptr<uint8_t>::reset(d);
    }

    void take(uint8_t *data, size_t size) {
        _size = size;
        managed_ptr<uint8_t>::reset(data);
    }

    void zero() {
        // Set all data to 0
        if (*this)
            memset(get(), 0, _size);
    }

    size_t size() const { return _size; }

private:
    size_t _size;
};

}