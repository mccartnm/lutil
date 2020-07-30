/*
    Lightweight matrix utility to handle different matrix
    operations.

    Matrix<4, 4> transform;
    transform[0][1] = 42;
*/
#pragma once
#include "lutil.h"

#define _SCALAR_MATRIX_OP(op)            \
    for (int x = 0; x < COLUMNS; x++) {  \
        for (int y = 0; y < ROWS; y++) { \
            _data[x][y] op scalar;       \
        }                                \
    }                                    \
    return *this;

namespace lutil { namespace LUTIL_VERSION {


template<size_t COLUMNS, size_t ROWS>
class Matrix {
public:

    static Matrix<COLUMNS, ROWS> identity() {
        return Matrix<COLUMNS, ROWS>();
    }

    Matrix() {
        // Defaults to the identity
        for (size_t x = 0; x < _rows; x++) {
            for (size_t y = 0; y < _columns; y++) {
                float v = y == x ? 1.0f : 0.0f;
                _data[x][y] = v;
            }
        }
    }

    size_t columns() const { return _columns; }
    size_t rows() const { return _rows; }

    void set(size_t row, size_t column, float value) {
        _data[row][column] = value;
    }

    float value(size_t row, size_t column) const {
        return _data[row][column];
    }

    /*
        Proxy object to make assignment quicker.
        With this you can do things like:

        Matrix<4, 4> mat;
        mat[2][3] = 123;

        Matrix<4, 4>::MatrixRow r2 = mat[1];
        r2[0] = 44.5;
    */
    struct MatrixRow {
        MatrixRow(Matrix<COLUMNS, ROWS> *trix, size_t column)
            : matrix(trix)
            , column(column)
        {}

        float &operator[] (size_t row) {
            return matrix->_data[column][row];
        }

        const float &operator[] (size_t row) const {
            return matrix->_data[column][row];
        }

        Matrix<COLUMNS, ROWS> *matrix;
        size_t column;
    };

    MatrixRow row(size_t column) {
        // assert(column < COLUMNS);
        return MatrixRow(this, column);
    }

    MatrixRow operator[] (size_t column) {
        return row(column);
    }

    // I don't love the const cast but it beats adding a ConstMatrixRow
    const MatrixRow operator[] (size_t column) const {
        return MatrixRow(const_cast<Matrix<COLUMNS, ROWS> *>(this), column);
    }

    Matrix<COLUMNS, ROWS> &operator*=(float scalar) { _SCALAR_MATRIX_OP(*=); }
    Matrix<COLUMNS, ROWS> &operator/=(float scalar) { _SCALAR_MATRIX_OP(/=); }

    bool operator== (const Matrix<COLUMNS, ROWS> &other) const {
        for (int x = 0; x < COLUMNS; x++) {
            for (int y = 0; y < ROWS; y++) {
                // Probably need a fuzzy match
                if (!fuzzy_match(_data[x][y], other._data[x][y]))
                    return false;
            }
        }
        return true;
    }

    bool operator!= (const Matrix<COLUMNS, ROWS> &other) const {
        return !(*this == other);
    }

private:
    static constexpr size_t _rows = ROWS;
    static constexpr size_t _columns = COLUMNS;

    float _data[COLUMNS][ROWS];
};

}
}
