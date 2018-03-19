cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
find_package(Qt5 REQUIRED COMPONENTS Gui Qml Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -pipe -g -ggdb")
string(TIMESTAMP BUILD_DATETIME "%Y-%m-%d %H:%M")

## Add Options and customized flags

add_subdirectory(source)
