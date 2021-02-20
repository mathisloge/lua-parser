cmake_minimum_required(VERSION 3.14)
project(LuaParser
        VERSION 0.1.0
        DESCRIPTION "lua parser"
        HOMEPAGE_URL https://github.com/mathisloge/lua-parser
        LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS graph)


add_executable(LuaParser 
    src/main.cpp
    src/chunk_def.cpp
    #src/grammar/chunk.cpp
    #src/grammar/chunk_def.cpp
    src/chunk.cpp
)

set_property(TARGET LuaParser PROPERTY CXX_STANDARD 20)

target_link_libraries(LuaParser PRIVATE
    Boost::headers
    Boost::graph
)
