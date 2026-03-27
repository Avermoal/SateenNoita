cmake_minimum_required(VERSION 4.0.1)

set(ENGINE_CORE_NAME EngineCore)

set(MATH_PRIVATE_INCLUDES
  includes/Mathematics/vector.h
  includes/Mathematics/ortho_projection.h
  includes/Mathematics/noises.h
  includes/Mathematics/a_star.h
)

set(MATH_PRIVATE_SOURCES
  src/Mathematics/vector.c
  src/Mathematics/ortho_projection.c
  src/Mathematics/noises.c
  src/Mathematics/a_star.c
)

set(ENGINE_PUBLIC_INCLUDES
  includes/EngineCore/application.h
  includes/EngineCore/debug_log.h
)

set(ENGINE_PRIVATE_INCLUDES
  includes/EngineCore/window.h
  includes/EngineCore/shader_program.h
  includes/EngineCore/events.h
  includes/EngineCore/events_data.h
  includes/EngineCore/scene.h
  includes/EngineCore/game_interface.h
  includes/EngineCore/display_manager.h
  includes/EngineCore/tiles_info.h
  includes/EngineCore/texture_manager.h
  includes/EngineCore/tiles_map.h
  includes/EngineCore/map_generator.h
  includes/EngineCore/player_move_system.h
  includes/EngineCore/mobs_move_system.h

  ${MATH_PRIVATE_INCLUDES}
)

set(ENGINE_PRIVATE_SOURCES
  src/EngineCore/application.c
  src/EngineCore/scene.c
  src/EngineCore/events.c
  src/EngineCore/shader_program.c
  src/EngineCore/window.c
  src/EngineCore/game_interface.c
  src/EngineCore/display_manager.c
  src/EngineCore/texture_manager.c
  src/EngineCore/tiles_map.c
  src/EngineCore/map_generator.c
  src/EngineCore/player_move_system.c
  src/EngineCore/mobs_move_system.c

  ${MATH_PRIVATE_SOURCES}
)

set(ENGINE_ALL_SOURCES
  ${ENGINE_PUBLIC_INCLUDES}
  ${ENGINE_PRIVATE_INCLUDES}
  ${ENGINE_PRIVATE_SOURCES}
)

add_library(${ENGINE_CORE_NAME} STATIC ${ENGINE_ALL_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_ALL_SOURCES})

target_include_directories(${ENGINE_CORE_NAME} PUBLIC includes)
target_include_directories(${ENGINE_CORE_NAME} PRIVATE src)
target_include_directories(${ENGINE_CORE_NAME} PRIVATE ../external/stbi)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_CORE_NAME} PRIVATE glfw)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_CORE_NAME} PRIVATE glad)
