include_directories(${CMAKE_SOURCE_DIR}/source/)

configure_file(record/recordstrategy.json.in  ${CMAKE_CURRENT_BINARY_DIR}/recordstrategy.json)
add_library(recordstrategy SHARED record/recordstrategy.cpp   ../core/strategyabstract.cpp)
target_link_libraries(recordstrategy Qt5::Core)

configure_file(sina/sinastrategy.json.in  ${CMAKE_CURRENT_BINARY_DIR}/sinastrategy.json)
add_library(sinastrategy SHARED sina/sinastrategy.cpp  ../core/strategyabstract.cpp)
target_link_libraries(sinastrategy Qt5::Core)

#configure_file(sina/sinastrategy.json.in  ${CMAKE_CURRENT_BINARY_DIR}/sinastrategy.json)
add_library(rpcstrategyserver SHARED rpc/rpcstrategyserver.cpp)
target_link_libraries(rpcstrategyserver Qt5::Core)

