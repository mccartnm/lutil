#pragma once
#include "lutil.h"
#include "lu_memory/managed_ptr.h"

#define DEFAULT_SIZE 5

namespace lutil {

/*
*/
template<typename T>
class Alloc {
public:
    Alloc() {};

    T *allocate(size_t size) {
        return new T[size];
    }

    void deallocate(T *data) {
        delete[] data;
    }

    void copy(T *dest, T *source, size_t size) {
        for (size_t i = 0; i < size; i++)
            *(dest + i) = *(source + i);
    }

    void remove(T *elements, size_t index, size_t count) {
        for (size_t i = index; i < count - 1; i++)
            elements[i] = elements[i + 1];
        elements[count - 1] = T(); // Nuke the last element
    }
};

/* _Very_ lightweight vector template */
template<typename T, typename A = Alloc<T>>
class Vec {
public:
    explicit Vec(size_t size) {
        _count = size;
        _size = _count;

        _elements = _alloc.allocate(_size);
        reset();
    }

    Vec() {
        _count = 0;
        _size = DEFAULT_SIZE;

        _elements = _alloc.allocate(_size);
        reset();
    }

    ~Vec() {
        _alloc.deallocate(_elements);
    }

    Vec(const Vec<T> &other)
        : _size(other._size)
        , _count(other._count)
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

    void push(const T& element) {
        if (_size == _count) {
            _expand(_size);
        }
        _elements[_count++] = element;
    }

    void push(T&& element) {
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

    void swap(Vec<T> &other)
    {
        swap_ptr(&_elements, &other._elements);
        swap_val<size_t>(_size, other._size);
        swap_val<size_t>(_count, other._count);
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

        T &operator* () {
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

        T *new_elements = _alloc.allocate(new_size);

        // Copy from one to the other
        _alloc.copy(new_elements, _elements, _size);
        _alloc.deallocate(_elements);

        _elements = new_elements;
        _size = new_size;
    }

    void _remove(size_t index) {
        _alloc.remove(_elements, index, _count);
        _count--;
    }

    void _from_other(const Vec<T> &other) {
        _elements = _alloc.allocate(_size);
        _alloc.copy(_elements, other._elements, _size);
    }

    size_t _size;  // Size in mem
    size_t _count; // Number of elements

    T *_elements;
    A _alloc;
};

}
