cmake_minimum_required(VERSION 4.0.1)

set(GAME_CORE_NAME GameCore)

add_executable(${GAME_CORE_NAME}
  src/main.c
)

add_custom_command(
  TARGET ${GAME_CORE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../res
        $<TARGET_FILE_DIR:${GAME_CORE_NAME}>/res
    COMMAND_EXPAND_LISTS
)

target_link_libraries(${GAME_CORE_NAME} PUBLIC EngineCore)

set_target_properties(${GAME_CORE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
