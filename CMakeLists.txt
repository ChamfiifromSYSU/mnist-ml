cmake_minimum_required(VERSION 3.20.0)

SET(CMAKE_C_COMPILER /usr/local/gcc-9.3.0/bin/gcc)
SET(CMAKE_CXX_COMPILER /usr/local/gcc-9.3.0/bin/g++)


set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release")
endif()

project(my_project_name VERSION 0.0.1 LANGUAGES C CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(./src)

add_subdirectory(./test)

add_subdirectory(./example)
