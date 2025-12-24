cmake_minimum_required(VERSION 3.20)
project(Translator)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
include_directories(gtest)
add_subdirectory(gtest)
add_executable(Translator
        include/main.cpp
        include/lexer.h
        include/parser.h
        include/evaluator.h
        include/stack.h
        include/token.h
        include/translator.h
)

add_executable(TranslatorTests
        test/test_main.cpp
        test/test_translator.cpp
)

target_link_libraries(TranslatorTests gtest)

