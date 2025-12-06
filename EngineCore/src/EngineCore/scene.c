#include "EngineCore/scene.h"

#include <glad/glad.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/window.h"
#include "EngineCore/texture_manager.h"

void initscene(struct scene *scn, struct window *win)
{
  createtilemap(&scn->map, win->pwin);
}

void destroyscene(struct scene *scn)
{
  destroytilemap(&scn->map);
}

void renderscene(struct window *win)
{
  /*Render tilemap*/
  bind_shader_program(win->shp.gameprog);
  glActiveTexture(GL_TEXTURE0);
  bind_texture_array(win->scn->map.texarr);
  /*Check uniform var*/
  GLint textureUniformLocation = glGetUniformLocation(win->shp.gameprog, "tileTextureArray");
  #ifndef NDEBUG/*Check uniform var*/
  if(textureUniformLocation == -1) {
    LOG_CRITICAL("Uniform 'tileTextureArray' not found in shader program!\n");
    /*Input all uniform var in shader*/
    GLint uniformCount = 0;
    glGetProgramiv(win->shp.gameprog, GL_ACTIVE_UNIFORMS, &uniformCount);
    LOG_CRITICAL("Total active uniforms in shader: %d\n", uniformCount);
    char uniformName[256];
    for(GLint i = 0; i < uniformCount; i++) {
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(win->shp.gameprog, i, sizeof(uniformName), &length, &size, &type, uniformName);
      LOG_CRITICAL("Uniform %d: %s (type: %d, size: %d)\n", i, uniformName, type, size);
    }
  }
  #endif/*Check uniform var*/
  glUniform1i(textureUniformLocation, 0);
  rendertilemap(&win->scn->map, win->shp.gameprog, (float)(win->windata.width)/(float)(win->windata.height));
  unbind_texture_array();
  unbind_shader_program();
}

void updatescene(struct scene *scn)
{
  
}
