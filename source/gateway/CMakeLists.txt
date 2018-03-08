configure_file(sina/sinagateway.json.in  ${CMAKE_CURRENT_BINARY_DIR}/sinagateway.json)
add_library(sinagateway MODULE sina/sinagateway.cpp sina/sinadatatypes.cpp)
target_link_libraries(sinagateway Qt5::Core -lcurl)


configure_file(xtp/xtpgateway.json.in  ${CMAKE_CURRENT_BINARY_DIR}/xtpgateway.json)
add_library(xtpgateway MODULE xtp/xtpgateway_p.cpp  xtp/xtpgateway.cpp xtp/xtpdatatypes.cpp)
target_link_libraries(xtpgateway Qt5::Core ${CMAKE_CURRENT_SOURCE_DIR}/xtp/XTP_API/bin/linux/libxtpquoteapi.so ${CMAKE_CURRENT_SOURCE_DIR}/xtp/XTP_API/bin/linux/libxtptraderapi.so)
