cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
find_package(Qt5 REQUIRED COMPONENTS Gui Qml Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


## Add Options and customized flags

add_subdirectory(source)
