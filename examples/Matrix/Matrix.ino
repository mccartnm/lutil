
#include "lutil.h"
#include "lu_storage/matrix.h"

Matrix<4, 4> mat;

// TODO...
void setup() {
    mat[0][2] = 3;
    Serial.println(mat[0][2]);
}

void loop() {
    delay(1000);
}
