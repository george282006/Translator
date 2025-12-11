
cmake_minimum_required(VERSION 3.16)
project(Translator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(translator
        main.cpp
)

target_include_directories(translator PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)