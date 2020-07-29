#pragma once
#include "lutil.h"

#define DEFAULT_SIZE 5

namespace lutil { namespace LUTIL_VERSION {


/*
    _Very_ lightweight vector template

    - TODO: Inline these functions? Probably.
    - Element removal
*/
template<typename T>
class Vec {
public:
    explicit Vec(size_t size) {
        _count = size;
        _size = _count;

        _elements = new T[_size];
        reset();
    }

    Vec() {
        _count = 0;
        _size = DEFAULT_SIZE;

        _elements = new T[_size];
        reset();
    }

    // Vec(const Vec &);
    // Vec(Vec &&);
    ~Vec() {
        delete [] _elements;
    }

    T &operator[](size_t index) {
        // Should we have some form of error?
        return _elements[index];
    }

    const T &operator[](size_t index) const {
        return _elements[index];
    }

    void reset() {
        for (size_t i = 0; i < _size; ++i)
            _elements[i] = T();
    }

    size_t count() const { return _count; }

    void push(T element) {
        if (_size == _count) {
            _expand(_size);
        }
        _elements[_count++] = element;
    }

    T pop(int index) {
        if (index < 0) {
            index = (int)_count + index;
        }
        if (index < 0 || index >= (int)_count) {
            return T(); // Exception?
        }

        T val = _elements[index];
        _remove(index);
        return val;
    }

    bool contains(const T &element) const {
        for (size_t i = 0; i < _count; ++i) {
            if (element == _elements[i])
                return true;
        }
        return false;
    }

    // Maybe... C++11 might be good enough
    // Vec<T>& operator=(const Vec<T> &);
    // Vec<T>& operator=(Vec<T> &&);

private:
    void _expand(size_t size) {
        auto new_size = _size + size;

        T *new_elements = new T[new_size];

        // Copy from one to the other
        ::memcpy(new_elements, _elements, _size);
        delete [] _elements;

        _elements = new_elements;
        _size = new_size;
    }

    void _remove(size_t index) {
        ::memmove(
            &_elements[index],
            &_elements[index + 1],
            (_count - index - 1) * sizeof(T)
        );
        _count--;
    }

    size_t _size;  // Size in mem
    size_t _count; // Number of elements

    T *_elements;
};

}
}