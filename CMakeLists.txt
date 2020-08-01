
# ------------------------------------------------------------------ // PROJECT NAME
project(lutil)

# ------------------------------------------------------------------ // STANDARDIZATION
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif ()


# ------------------------------------------------------------------ // INCLUDES
include_directories(${CMAKE_SOURCE_DIR}/src)

add_definitions("-D_TEST_CORE_")
set (LUTIL_HEADERS
    src/lutil.h
    src/lu_memory/managed_ptr.h

    # src/lu_process (embedded only)

    src/lu_storage/map.h
    src/lu_storage/matrix.h
    src/lu_storage/vector.h
    src/lu_state/state.h
)

# set (LUTIL_SOURCES) # Nothing yet

add_executable(${PROJECT_NAME} ${LUTIL_HEADERS} ${PROJECT_SOURCE_DIR}/extras/tests/main.cpp)

