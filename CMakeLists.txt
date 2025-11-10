cmake_minimum_required(VERSION 4.0.1)

project(SateenNoita)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(EngineCore)
add_subdirectory(GameCore)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameCore)
