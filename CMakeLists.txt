cmake_minimum_required( VERSION 3.16 )
project( daw-thunk
         VERSION "0.1.0"
         DESCRIPTION "Cross platform thunking abstraction"
         HOMEPAGE_URL "https://github.com/beached/thunk"
         LANGUAGES C CXX
         )

set( CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard whose features are requested." )

option(DAW_USE_PACKAGE_MANAGEMENT "Do not use FetchContent and assume dependencies are installed" OFF)
if (NOT DAW_USE_PACKAGE_MANAGEMENT)
    add_subdirectory(extern)
else ()
    find_package(daw-header-libraries REQUIRED)
endif ()

include_directories( "${CMAKE_SOURCE_DIR}/include" )

include(GNUInstallDirs)
set(daw_thunk_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake"
    CACHE STRING
    "The install directory to use for CMake package config files"
    )

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} INTERFACE)
add_library(daw::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} INTERFACE daw::daw-header-libraries )

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME}
                           INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                           )

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE daw::
        DESTINATION lib/cmake/${PROJECT_NAME}
        )

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME}
        )

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/daw DESTINATION include)


if( DAW_ENABLE_TESTING )
     enable_testing()
     add_subdirectory( tests )
endif()