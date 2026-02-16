#ifndef ENGINECORE_SHADER_PROGRAM_H
#define ENGINECORE_SHADER_PROGRAM_H

#include <stddef.h>

struct shaderprogram{
  unsigned int interfaceprog;
  unsigned int gameprog;
  unsigned int menuprog;
};

enum SHADER_PROGRAM_EXIT_CODE{
  SHADER_PROGRAM_EXIT_SUCCESS = 0,
  SHADER_PROGRAM_EXIT_FAILURE,
};

/*Returns shader program ID*/
unsigned int create_shader_program(const char* vertsh_code, const char* fragsh_code);

enum SHADER_PROGRAM_EXIT_CODE load_shader_programs(struct shaderprogram* shp);

void destroy_shader_programs(struct shaderprogram shp);/*Not pointer cuz it's useless*/

void bind_shader_program(unsigned int id);
void unbind_shader_program();

bool readshader(const char* path, char* shader, size_t shaderlength);

/*For creation array of chars LOL*/
size_t shlength(const char* path);

#endif/*ENGINECORE_SHADER_PROGRAM_H*/
