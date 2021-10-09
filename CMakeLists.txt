
# ------------------------------------------------------------------ // PROJECT NAME
project(lutil)

# ------------------------------------------------------------------ // STANDARDIZATION
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif ()


# ------------------------------------------------------------------ // INCLUDES
include_directories(${CMAKE_SOURCE_DIR}/src)

add_definitions(-DBUILD_LIB)
set (LUTIL_HEADERS
    src/lutil.h
    src/lu_memory/managed_ptr.h
    src/lu_memory/abstract_memory.h

    # src/lu_process/process.h   # Non uC spec
    # src/lu_process/process.cpp # Non uC spec

    src/lu_control/pid.h
    src/lu_control/pid.cpp

    src/lu_math/matrix.h
    src/lu_storage/map.h
    src/lu_storage/vector.h

    # src/lu_state/state.h # Non uC spec
)

set (LUTIL_SOURCES
    src/lu_control/pid.cpp
)

add_library(lutil SHARED
    ${LUTIL_HEADERS}
    ${LUTIL_SOURCES}
)

install (TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin COMPONENT user
    LIBRARY DESTINATION lib COMPONENT user
    ARCHIVE DESTINATION lib
    COMPONENT developer
)

install (
    DIRECTORY src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
