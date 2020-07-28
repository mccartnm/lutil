/*
    Lightweight matrix utility to handle different matrix
    operations.

    Matrix<4, 4> transform;
    transform[0][1] = 42;
*/
#pragma once
#include "lutil.h"

namespace lutil { namespace LUTIL_VERSION {


template<size_t COLUMNS, size_t ROWS, typename T = float>
class Matrix {
public:

    static Matrix<COLUMNS, ROWS, T> identity() {
        return Matrix<COLUMNS, ROWS, T>();
    }

    Matrix() {
        // Defaults to the identity
        for (size_t x = 0; x < _rows; x++) {
            for (size_t y = 0; y < _columns; y++) {
                T v = y == x ? 1 : 0;
                output._data[x][y] = v;
            }
        }
    }

    size_t columns() const { return _columns; }
    size_t rows() const { return _rows; }

    void set(size_t row, size_t column, const T &value) {
        _data[row][column] = value;
    }

    T value(size_t row, size_t column) const {
        return _data[row][column];
    }

    /*
        Proxy object to help with assignment and more fluid code
    */
    struct MatrixRow {
        MatrixRow(Matrix<COLUMNS, ROWS, T> *trix, size_t column)
            : matrix(trix)
            , column(column)
        {}

        T &operator[] (size_t row) {
            return matrix._data[column][row];
        }

        const T &operator[] (size_t row) const {
            return matrix._data[column][row];
        }

        Matrix<COLUMNS, ROWS, T> matrix;
        size_t column;
    };

    MatrixRow row(size_t column) const {
        // assert(column < COLUMNS);
        return MatrixRow(this, column);
    }

    MatrixRow operator[] (size_t column) const {
        return row(column)
    }


private:
    static constexpr size_t _rows = ROWS;
    static constexpr size_t _columns = COLUMNS;

    T _data[COLUMNS][ROWS];
};

}
}
