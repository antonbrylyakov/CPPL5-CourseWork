cmake_minimum_required(VERSION 3.10)
project(IniFileParserApp)
set(sources  "main.cpp")
add_executable(IniFileParserApp ${sources})
target_link_libraries(IniFileParserApp IniFileParser)
target_include_directories(IniFileParserApp PRIVATE "../IniFileParser")
# Копируем файл с примером
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/example.ini DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})