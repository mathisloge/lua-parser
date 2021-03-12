cmake_minimum_required(VERSION 3.14)
project(LuaParser
        VERSION 0.1.0
        DESCRIPTION "lua parser"
        HOMEPAGE_URL https://github.com/mathisloge/lua-parser
        LANGUAGES CXX)

find_package(Boost REQUIRED)


add_subdirectory(parser_lib)

add_executable(LuaParser 
    src/main.cpp
    src/ast/printer.cpp
    src/ast/dot_printer.cpp
    src/hasher.cpp
)

if(MSVC)
    set_property(TARGET LuaParser APPEND PROPERTY COMPILE_OPTIONS "/bigobj")
endif()

set_property(TARGET LuaParser PROPERTY CXX_STANDARD 20)

target_link_libraries(LuaParser PRIVATE
    Boost::headers
    lua-parser-lib
)
