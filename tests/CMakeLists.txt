# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/daw_thunk
#

option( DAW_USE_SANITIZERS "Enable Sanitizers" ON )

if( MSVC )
    #add_compile_options( -DDEBUG /permissive- /EHsc /fsanitize=address /arch:AVX2 -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
    add_compile_options( -DDEBUG /permissive- /EHsc /arch:AVX2 -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00 )
    if( DAW_USE_SANITIZERS )
        add_link_options( /fsanitize=address -fno-omit-frame-pointer )
    endif()
else()
    add_compile_options( -march=native )
    if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 16 )
        add_compile_options( -Wno-c++2b-extensions )
    endif()
    if( DAW_USE_SANITIZERS )
        add_compile_options( -fsanitize=address )#,undefined )
        add_link_options( -fsanitize=address ) #,undefined )
    endif()
endif()

add_library( daw_thunk_test_lib INTERFACE )
target_link_libraries( daw_thunk_test_lib INTERFACE daw::daw-thunk )
target_include_directories( daw_thunk_test_lib INTERFACE include/ )
target_compile_options( daw_thunk_test_lib INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/permissive-> )

add_executable( daw_thunk src/daw_thunk_test.cpp )
target_link_libraries( daw_thunk PRIVATE daw_thunk_test_lib )
add_test( NAME daw_thunk_test COMMAND daw_thunk )

add_executable( signal src/signal_test.cpp )
target_link_libraries( signal PRIVATE daw_thunk_test_lib )

add_executable( stateful_currying src/stateful_currying_test.cpp )
target_link_libraries( stateful_currying PRIVATE daw_thunk_test_lib )
add_test( NAME stateful_currying_test COMMAND stateful_currying )
