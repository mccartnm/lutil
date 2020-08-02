#pragma once
#include "lutil.h"

namespace lutil {

namespace LUTIL_VERSION {

template<typename T>
inline void swap_ptr(T **a, T **b)
{
    T *temp = *a;
    *a = *b;
    *b = temp;
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

    explicit managed_data(size_t size)
        : managed_ptr(new uint8_t[size])
        , _size(size)
    {
        memset(get(), 0, _size);
    }

    uint8_t operator[](int index) const {
        if (index >= (int)_size) { // :|
            return 0; // Failsafe
        }
        return *(get() + index);
    }

    void set(uint8_t *data) {
        if (_size > 0)
            memcpy(get(), data, _size);
    }

    size_t size() const { return _size; }

private:
    size_t _size;
};

}
}