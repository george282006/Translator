
add_executable(test_main
        test_main.cpp
        test_translator.cpp
)

target_include_directories(test_main PRIVATE
        ${CMAKE_SOURCE_DIR}/Translator/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Translator/gtest
)

target_link_libraries(test_main gtest)