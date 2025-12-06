#include "EngineCore/shader_program.h"

#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>

#include "EngineCore/debug_log.h"

enum SHADER_PROGRAM_EXIT_CODE load_shader_programs(struct shaderprogram *shp)
{
  /*Interface shader program*/
  const char *interface_vertsh_source = "res/interface.glslv";
  const char *interface_fragsh_source = "res/interface.glslf";
  size_t interface_vertsh_length = shlength(interface_vertsh_source);
  size_t interface_fragsh_length = shlength(interface_fragsh_source);
  char *interface_vertsh_code = (char*)calloc(interface_vertsh_length, sizeof(char));
  char *interface_fragsh_code = (char*)calloc(interface_fragsh_length, sizeof(char));
  if(!readshader(interface_vertsh_source, interface_vertsh_code, interface_vertsh_length)){
    LOG_CRITICAL("[SHADER]: Interface vertex shader don't load\n");
    return SHADER_PROGRAM_EXIT_FAILURE;
  }
  if(!readshader(interface_fragsh_source, interface_fragsh_code, interface_fragsh_length)){
    LOG_CRITICAL("[SHADER]: Interface fragment shder don't load\n");
    return SHADER_PROGRAM_EXIT_FAILURE;
  }
  shp->interfaceprog = create_shader_program(interface_vertsh_code, interface_fragsh_code);
  free(interface_vertsh_code);
  free(interface_fragsh_code);
  /*Game shader program*/
  const char *game_vertsh_source = "res/game.glslv";
  const char *game_fragsh_source = "res/game.glslf";
  size_t game_vertsh_length = shlength(game_vertsh_source);
  size_t game_fragsh_length = shlength(game_fragsh_source);
  char *game_vertsh_code = (char*)calloc(game_vertsh_length, sizeof(char));
  char *game_fragsh_code = (char*)calloc(game_fragsh_length, sizeof(char));
  if(!readshader(game_vertsh_source, game_vertsh_code, game_vertsh_length)){
    LOG_CRITICAL("[SHADER]: Game vertex shader don't load\n");
    return SHADER_PROGRAM_EXIT_FAILURE;
  }
  if(!readshader(game_fragsh_source, game_fragsh_code, game_fragsh_length)){
    LOG_CRITICAL("[SHADER]: Game fragment shder don't load\n");
    return SHADER_PROGRAM_EXIT_FAILURE;
  }
  shp->gameprog = create_shader_program(game_vertsh_code, game_fragsh_code);
  free(game_vertsh_code);
  free(game_fragsh_code);
  /*Menu shader program*/

  return SHADER_PROGRAM_EXIT_SUCCESS;
}

unsigned int create_shader_program(const char *vertsh_code, const char *fragsh_code)
{
  unsigned int shp = 0;
  /*Create shaders ID*/
  GLuint vertsh_ID = 0, fragsh_ID = 0;
  vertsh_ID = glCreateShader(GL_VERTEX_SHADER);
  fragsh_ID = glCreateShader(GL_FRAGMENT_SHADER);
  /*Shader compile*/
  glShaderSource(vertsh_ID, 1, (const GLchar * const*)&vertsh_code, NULL);
  glCompileShader(vertsh_ID);
  glShaderSource(fragsh_ID, 1, (const GLchar * const*)&fragsh_code, NULL);
  glCompileShader(fragsh_ID);
  /*Shader compile check*/
  GLint success;
  GLchar infolog[512];
  glGetShaderiv(vertsh_ID, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertsh_ID, 512, NULL, infolog);
    LOG_CRITICAL("Vertex shader not compiled\n");
    LOG_CRITICAL(infolog);
    return 0;
  }
  glGetShaderiv(fragsh_ID, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(fragsh_ID, 512, NULL, infolog);
    LOG_CRITICAL("Fragment shader not compiled\n");
    LOG_CRITICAL(infolog);
    return 0;
  }
  /*Create shader program*/
  shp = glCreateProgram();
  /*Attaching shaders*/
  glAttachShader(shp, vertsh_ID);
  glAttachShader(shp, fragsh_ID);
  /*Linking program*/
  glLinkProgram(shp);
  /*Delete shaders*/
  glDetachShader(shp, vertsh_ID);
  glDetachShader(shp, fragsh_ID);
  glDeleteShader(vertsh_ID);
  glDeleteShader(fragsh_ID);
  /*Linking check*/
  glGetProgramiv(shp, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(shp, 512, NULL, infolog);
    LOG_CRITICAL("Shader program not created\n");
    LOG_CRITICAL(infolog);
    return 0;
  }
  return shp;
}

void destroy_shader_programs(struct shaderprogram shp)
{
  glDeleteShader(shp.interfaceprog);
  glDeleteShader(shp.gameprog);
  glDeleteShader(shp.menuprog);
}

void bind_shader_program(unsigned int id)
{
  glUseProgram(id);
}

void unbind_shader_program()
{
  glUseProgram(0);
}

bool readshader(const char *path, char *shader, size_t shaderlength)
{
  FILE *shader_src = fopen(path, "r");
  if(!shader_src){
    LOG_CRITICAL("Shader not read\n");
    return false;
  }
  fread(shader, sizeof(char), shaderlength, shader_src);
  fclose(shader_src);
  return true;
}

size_t shlength(const char *path)
{
  FILE *shader = fopen(path, "r");
  if(!shader){
    LOG_CRITICAL("SHLENGTH LOL\n");
    return 0;
  }
  fseek(shader, 0L, SEEK_END);
	size_t size = ftell(shader);
	rewind(shader);
	fclose(shader);
  return size;
}
