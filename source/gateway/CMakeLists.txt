add_library(sinagateway MODULE sina/sinagateway.cpp sina/sinadatatypes.cpp)
target_link_libraries(sinagateway Qt5::Core -lcurl)


