configure_file(sina/sinagateway.json.in  ${CMAKE_CURRENT_BINARY_DIR}/sinagateway.json)
add_library(sinagateway MODULE sina/sinagateway.cpp sina/sinadatatypes.cpp)
target_link_libraries(sinagateway Qt5::Core -lcurl)


