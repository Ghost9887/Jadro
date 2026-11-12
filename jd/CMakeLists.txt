cmake_minimum_required(VERSION 3.20)

project(jd)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.c
    src/vm.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
