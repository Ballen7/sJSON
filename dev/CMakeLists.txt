cmake_minimum_required(VERSION 3.4)

project(sjson-dev)

add_executable(${CMAKE_PROJECT_NAME}
    dev.c
    ./../sjson.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ./..
)