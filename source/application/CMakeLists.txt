
add_executable(nulltest nulltest/nulltest.cpp)
target_link_libraries(nulltest Qt5::Core qscore)

## TODO Sina gateway is a plugin now, these code won't work
add_executable(recordsinatick recordsinatick/sinarecorder.cpp ../gateway/sina/sinadatatypes.cpp)
target_link_libraries(recordsinatick Qt5::Core -lcurl qscore)

add_executable(showsinatick showsinatick/showtick.cpp)
target_link_libraries(showsinatick Qt5::Core)

