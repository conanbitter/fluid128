cmake_minimum_required(VERSION 3.16)

project(fluid VERSION 0.1 LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(INCLUDE_FILES
    fluid.h
)

set(SRC_FILES
    main.c
    fluid.c
)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

list(TRANSFORM INCLUDE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/include/)
list(TRANSFORM SRC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(fluid ${INCLUDE_FILES} ${SRC_FILES})

target_include_directories(fluid PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(fluid PRIVATE SDL3::SDL3)