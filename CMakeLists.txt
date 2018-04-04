cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 14)
find_package(Qt5 REQUIRED COMPONENTS Gui Qml Core Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -pipe -g0 -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -pipe -g3 -ggdb")
string(TIMESTAMP BUILD_DATETIME "%Y-%m-%d %H:%M")

## Add Options and customized flags

add_subdirectory(source)
