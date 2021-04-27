# https://github.com/kaizouman/gtest-cmake-example/blob/master/test/testfoo/CMakeLists.txt

find_package(FMT)

file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(test_state_machine ${SRCS})

TARGET_LINK_LIBRARIES(test_state_machine
        lib_state_machine
        libgtest
        libgmock
        ${FMT_LIBRARIES}
        )

include_directories(${FMT_INCLUDE_DIRS})

add_test(NAME test_state_machine
        COMMAND test_state_machine)