cmake_minimum_required (VERSION 3.10)
project (path_finder)

set(BRIEF "Dummy A* program to test SFML")

find_package(SFML COMPONENTS graphics window system)
add_definitions( -DPROG_NAME="${PROJECT_NAME}"
                 -DCMDLINE_HELP="-h"
                 -DCMDLINE_CONF="-i" )

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "This application requires an out of source build.
        Please create a separate build directory")
endif()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.hpp)

add_subdirectory          (3rd/minijson EXCLUDE_FROM_ALL)
add_executable            (${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries     (${PROJECT_NAME} PRIVATE sfml-graphics sfml-window miniJSON)
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_compile_options    (${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -pedantic)
target_compile_features   (${PROJECT_NAME} PRIVATE cxx_std_17)

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install (FILES conf/default.conf TYPE SYSCONF)
