include_directories(${CMAKE_SOURCE_DIR}/source/)

configure_file(record/recordstrategy.json.in  ${CMAKE_CURRENT_BINARY_DIR}/recordstrategy.json)
add_library(recordstrategy MODULE record/recordstrategy.cpp)
target_link_libraries(recordstrategy Qt5::Core)

configure_file(sina/sinastrategy.json.in  ${CMAKE_CURRENT_BINARY_DIR}/sinastrategy.json)
add_library(sinastrategy MODULE sina/sinastrategy.cpp)
target_link_libraries(sinastrategy Qt5::Core)

#configure_file(sina/sinastrategy.json.in  ${CMAKE_CURRENT_BINARY_DIR}/sinastrategy.json)
add_library(rpcstrategyserver MODULE rpc/rpcstrategyserver.cpp)
target_link_libraries(rpcstrategyserver Qt5::Core)

