configure_file(sina/sinagateway.json.in  ${CMAKE_CURRENT_BINARY_DIR}/sinagateway.json)
add_library(sinagateway SHARED sina/sinagateway.cpp sina/sinadatatypes.cpp ${CMAKE_SOURCE_DIR}/source/core/ipc/shmtickinfo.cpp  ../core/gatewayabstract.cpp)
target_link_libraries(sinagateway Qt5::Core -lcurl -lrt)


configure_file(xtp/xtpgateway.json.in  ${CMAKE_CURRENT_BINARY_DIR}/xtpgateway.json)
add_library(xtpgateway SHARED xtp/xtpgateway_p.cpp  xtp/xtpgateway.cpp xtp/xtpdatatypes.cpp)
target_link_libraries(xtpgateway Qt5::Core ${CMAKE_CURRENT_SOURCE_DIR}/xtp/XTP_API/bin/linux/libxtpquoteapi.so ${CMAKE_CURRENT_SOURCE_DIR}/xtp/XTP_API/bin/linux/libxtptraderapi.so)



#configure_file(sharedmemorygateway//xtpgateway.json.in  ${CMAKE_CURRENT_BINARY_DIR}/xtpgateway.json)
add_library(sharedmemorygateway SHARED sharedmemorygateway/sharedmemorygateway.cpp sharedmemorygateway/sharedmemorygateway_p.cpp)
target_link_libraries(sharedmemorygateway Qt5::Core)
