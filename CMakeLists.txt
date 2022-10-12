cmake_minimum_required (VERSION 3.10)
project (path_finder)

set(BRIEF "Dummy A* program to test SFML")

function(submodule_update submodule args)
find_package(Git REQUIRED)

if(NOT EXISTS ${submodule}/CMakeLists.txt)
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${submodule}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE STATUS
                    ERROR_VARIABLE UPDATE_ERR)

   if ( STATUS AND NOT STATUS EQUAL 0 )
       message(FATAL_ERROR "Unable to update required submodule '${submodule}'.
           What : '${UPDATE_ERR}'")
   endif()
endif()

add_subdirectory(${submodule} ${args})
endfunction(submodule_update)

find_package(SFML COMPONENTS graphics window system)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "This application requires an out of source build.
        Please create a separate build directory")
endif()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE HEADER_FILES src/*.hpp)

set (INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME})
set (CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ)

submodule_update          (3rd/minijson EXCLUDE_FROM_ALL)
add_executable            (${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries     (${PROJECT_NAME} PRIVATE sfml-graphics sfml-window miniJSON)
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_compile_options    (${PROJECT_NAME} PRIVATE -O3 -Werror -Wall -Wextra -pedantic)
target_compile_features   (${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PRIVATE -DPROG_NAME="${PROJECT_NAME}"
                                                   -DCMDLINE_HELP="-h"
                                                   -DCMDLINE_CONF="-i"
                                                   -DDEFAULT_CONF="${INSTALL_DIR}/default.json")

install (DIRECTORY DESTINATION ${INSTALL_DIR})
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${INSTALL_DIR})
install (DIRECTORY conf/ DESTINATION ${INSTALL_DIR})
