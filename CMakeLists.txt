cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX12.sdk)
project(crypto-file VERSION 1.0.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_subdirectory(doctest)

include(doctest/scripts/cmake/doctest.cmake)

function(target_common_settings target)
    # `c_std_11` is not supported before cmake-3.21.
    target_compile_features(${target} PUBLIC cxx_std_20 c_std_11)
    target_compile_options(${target} PUBLIC -Werror -Wall -Wextra)
endfunction()

function(add_test target)
    add_executable(${target} ${ARGN})
    target_common_settings(${target})
    target_link_libraries(${target} PRIVATE doctest)

    # Here `ADD_LABELS` must be provided.
    set(prefix ${target})
    string(APPEND prefix " - test:")
    doctest_discover_tests(${target} TEST_PREFIX ${prefix} ADD_LABELS 1)
endfunction()

function(add_exe target)
    add_executable(${target} ${ARGN})
    target_common_settings(${target})
endfunction()

function(add_lib target)
    add_library(${target} STATIC)
    target_sources(${target} PRIVATE ${ARGN})
    target_common_settings(${target})
endfunction()

function(copy_file2 target dst src)
    set(SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    set(DST_FILE ${CMAKE_CURRENT_BINARY_DIR}/${dst})
    add_custom_target(${target} DEPENDS ${DST_FILE})
    add_custom_command(
        OUTPUT ${DST_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy ${SRC_FILE} ${DST_FILE}
        DEPENDS ${SRC_FILE}
    )
endfunction()

function(copy_file target file)
    copy_file2(${target} ${file} ${file})
endfunction()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEV_DIR $ENV{HOME}/workspace/devel)
    include_directories(${DEV_DIR}/include)
    link_directories(${DEV_DIR}/lib)
    set(ENV{PATH} ${DEV_DIR}/bin:$ENV{PATH})
else()
    include_directories($ENV{HOME}/include)
    link_directories($ENV{HOME}/lib)
endif()

include_directories(external)
include_directories(src)

set(LIB_NAME crypto-file)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
