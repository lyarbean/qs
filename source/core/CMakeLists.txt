set(QSCORE_SRC defaultengine.cpp  engineabstract.cpp  gatewayabstract.cpp  riskmanager.cpp  strategyabstract.cpp)
add_library(qscore ${QSCORE_SRC})
target_link_libraries(qscore Qt5::Core)
