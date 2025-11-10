cmake_minimum_required(VERSION 4.0.1)

set(GAME_CORE_NAME GameCore)

add_executable(${GAME_CORE_NAME}
  src/main.c
)

target_link_libraries(${GAME_CORE_NAME} EngineCore)

set_target_properties(${GAME_CORE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
