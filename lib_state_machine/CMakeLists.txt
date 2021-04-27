
add_library(lib_state_machine STATIC
        event_data.h
        state.h
        state_machine.h
        )
target_include_directories(lib_state_machine PUBLIC
        .
        )
set_target_properties(lib_state_machine PROPERTIES LINKER_LANGUAGE CXX)

