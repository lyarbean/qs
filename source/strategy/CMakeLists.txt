include_directories(${CMAKE_SOURCE_DIR}/source/)
add_library(recordstrategy MODULE record/recordstrategy.cpp)
target_link_libraries(recordstrategy Qt5::Core)

add_library(sinastrategy MODULE sina/sinastrategy.cpp)
target_link_libraries(sinastrategy Qt5::Core)
