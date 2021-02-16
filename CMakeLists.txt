cmake_minimum_required(VERSION 3.14)
project(LuaParser
        VERSION 0.1.0
        DESCRIPTION "lua parser"
        HOMEPAGE_URL https://github.com/mathisloge/lua-parser
        LANGUAGES CXX)

find_package(Boost REQUIRED COMPONENTS graph)


add_executable(LuaParser 
    src/main.cpp
)

target_link_libraries(LuaParser PRIVATE
    Boost::headers
    Boost::graph
)