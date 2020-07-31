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

    ~Vec() {
        delete [] _elements;
    }

    Vec(const Vec<T> &other)
        : _count(other._count)
        , _size(other._size)
    {
        _from_other(other);
    }

    Vec &operator= (const Vec<T> &other) {
        _count = other._count;
        _size = other._size;
        _from_other(other);
        return *this;
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

    // ----------------------------------------------------------------
    // ITERATION

    class Iterator {
    public:
        Iterator(Vec<T> *vec, int index = 0)
            : _vec(vec)
            , _index(index)
        {}

        Iterator(const Iterator &it)
            : _vec(it._vec)
            , _index(it._index)
        {}

        T operator* () {
            return _vec->_elements[_index];
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
            return _index < _vec->_count;
        }

        bool operator== (const Iterator &other) const {
            return (this->_vec == other._vec &&
                    this->_index == other._index);
        }

        bool operator!= (const Iterator &other) const {
            return (this->_vec != other._vec ||
                    this->_index != other._index);
        }

        bool operator!= (const Iterator *other) const {
            return (this != *other);
        }

    private:
        Vec<T> *_vec;
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

    void _from_other(const Vec<T> &other) {
        _elements = new T[_size];
        ::memcpy(_elements, other._elements, _size);
    }

    size_t _size;  // Size in mem
    size_t _count; // Number of elements

    T *_elements;
};

}
}