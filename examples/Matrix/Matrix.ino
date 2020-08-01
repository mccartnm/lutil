/*
    Simple example of the Matrix utility. Coordinates are from
    the top left to bottom left, (X, Y)

    (0, 0)
     \/
    +-       -+
    | 1, 0, 0 |
    | 0, 1, 0 |
    | 0, 0, 1 | < (2, 2)
    +-       -+
*/

#include "lutil.h"
#include "lu_storage/matrix.h"

using namespace lutil::v_1;

Matrix<4, 4> mat;

void setup() {
    // Identity (the default)
    // mat == Matrix<4, 4>::identity();

    // Assignment
    mat[2][1] = 55; // (x:2, y:1)

    // Reference Multiplication
    mat *= 2;
    // mat[2][1] == 110;

    Matrix<4, 4> other = mat; // Copy
    // other == mat;

    other[0][0] = 33;
    // other != mat;

    Serial.println(mat[0][2]);
}

void loop() {
    delay(1000);
}
