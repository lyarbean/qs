
add_executable(nulltest nulltest/nulltest.cpp)
target_link_libraries(nulltest Qt5::Core qscore)

add_executable(recordsinatick recordsinatick/sinarecorder.cpp ../gateway/sina/sinadatatypes.cpp)
target_link_libraries(recordsinatick Qt5::Core -lcurl qscore)

add_executable(showsinatick showsinatick/showtick.cpp)
target_link_libraries(showsinatick Qt5::Core)

add_executable(plugininfoviewer plugininfoviewer.cpp ../core/strategyabstract.cpp ../core/gatewayabstract.cpp)
target_link_libraries(plugininfoviewer qscore Qt5::Core Qt5::Gui Qt5::Widgets )

add_subdirectory(server)
