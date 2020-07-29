
#include <iostream>

#include "lutil.h"
#include "lu_storage/matrix.h"
#include "lu_storage/vector.h"
#include "lu_storage/map.h"

using namespace lutil::v_1;

int main(int argc, char const *argv[])
{
    // ---------------------------------------------------
    // MATRIX

    Matrix<4, 4> mat;
    mat[0][1] = 12.0f;
    std::cout << mat[0][1] << std::endl;

    Matrix<4, 4> mat2;
    mat2[0][1] = 12.0f;
    std::cout << (mat == mat2) << std::endl;

    // ---------------------------------------------------
    // VECTOR

    Vec<float> vector;
    vector.push(12.0);
    vector.push(1.0);
    vector.push(4.0);

    std::cout  << vector.pop(2) << std::endl; // == 4.0
    std::cout  << vector.pop(-1) << std::endl; // == 1.0

    std::cout << vector[0] << std::endl;

    // ---------------------------------------------------
    // MAP

    Map<int, std::string> map;
    map[1] = "test";
    map[44] = "blue";

    std::cout << map[1].c_str() << std::endl;

    map.remove(44);
    std::cout << (map.contains(44) ? "y": "n") << std::endl;    
    std::cout << (map.contains(1) ? "y": "n") << std::endl;    

    // To keep the console
    throw std::runtime_error("foo");

    return 0;
}