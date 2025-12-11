add_library(translator_includes INTERFACE
        lexer.h
        parser.h
        stack.h
        token.h
        evaluator.h
        translator.h
        main.cpp
)

target_include_directories(translator_includes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(include)