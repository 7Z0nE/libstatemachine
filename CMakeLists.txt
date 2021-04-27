cmake_minimum_required(VERSION 3.1)
project(StateMachine VERSION 1.0
                  DESCRIPTION "Simple Finit State Machine"
        )

enable_testing()

add_subdirectory(test)
add_subdirectory(lib_state_machine)

add_executable(main)
target_link_libraries(main lib_state_machine)