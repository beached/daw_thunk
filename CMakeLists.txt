cmake_minimum_required( VERSION 3.16 )
project( daw-thunk-a-dunk
         VERSION "0.1.0"
				 DESCRIPTION "Cross platform thunking abstraction"
         HOMEPAGE_URL "https://github.com/beached/thunk_a_dunk"
         LANGUAGES C CXX
         )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested." )

#include( cmake/test_compiler_options.cmake )
add_subdirectory( extern )

#set( Boost_NO_WARN_NEW_VERSIONS ON )

#find_package( Boost COMPONENTS system container REQUIRED )

include_directories( "${CMAKE_SOURCE_DIR}/include" )

set( SOURCE_FILES
     src/daw/thunk.cpp
)

add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )
target_compile_features( ${PROJECT_NAME} INTERFACE cxx_std_17 )
target_include_directories( ${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIRS} )
target_link_libraries( ${PROJECT_NAME} PRIVATE
                       daw::daw-header-libraries
                       )

