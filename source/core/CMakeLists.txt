set(QSIPC_SRC ipc/shmtickinfo.cpp ipc/sharedobjectmanager.cpp ipc/readwritelocker.cpp)
set(QSCORE_SRC defaultengine.cpp  engineabstract.cpp  gatewayabstract.cpp  riskmanager.cpp  strategyabstract.cpp )
add_library(qscore SHARED ${QSCORE_SRC} ${QSIPC_SRC})
target_link_libraries(qscore Qt5::Core -lrt)
