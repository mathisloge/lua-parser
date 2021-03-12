project(lua-parser-lib)
include(GenerateExportHeader)


set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(lua-parser-lib SHARED
    src/base_def.cpp
    src/chunk_def.cpp
    src/chunk.cpp
    src/parser.cpp
)
set_property(TARGET lua-parser-lib PROPERTY CXX_STANDARD 20)

generate_export_header(lua-parser-lib BASE_NAME LuaParser)

target_include_directories(
    lua-parser-lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(lua-parser-lib PRIVATE 
    Boost::headers
)
