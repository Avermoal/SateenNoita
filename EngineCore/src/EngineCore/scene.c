#include "EngineCore/scene.h"

#include <glad/glad.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/window.h"
#include "EngineCore/texture_manager.h"

void initscene(struct scene* scn, struct window* win)
{
  createtilemap(&scn->map, win->pwin);
}

void destroyscene(struct scene** scn)
{
  if(scn && *scn){
    destroytilemap(&((*scn)->map));
    free(*scn);
    *scn = nullptr;
  }
}

void renderscene(struct window *win)
{
  /*Render tilemap*/
  bind_shader_program(win->shp.gameprog);
  glActiveTexture(GL_TEXTURE0);
  bind_texture_array(win->scn->map.texarr);
  #ifndef NDEBUG/*Check bound textures*/
  GLint boundTexture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, &boundTexture);
  if(boundTexture != win->scn->map.texarr.id) {
    LOG_CRITICAL("Texture array not bound correctly! Bound: %d, Expected: %d\n", 
                 boundTexture, win->scn->map.texarr.id);
  }
  #endif/*Check bound textures*/
  /*Check uniform var*/
  GLint textureloc = glGetUniformLocation(win->shp.gameprog, "tileTextureArray");
  #ifndef NDEBUG/*Check uniform var*/
  if(textureloc == -1) {
    LOG_CRITICAL("Uniform 'tileTextureArray' not found in shader program!\n");
    /*Input all uniform var in shader*/
    GLint uniformcount = 0;
    glGetProgramiv(win->shp.gameprog, GL_ACTIVE_UNIFORMS, &uniformcount);
    LOG_CRITICAL("Total active uniforms in shader: %d\n", uniformcount);
    char uniformname[256];
    for(GLint i = 0; i < uniformcount; i++) {
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(win->shp.gameprog, i, sizeof(uniformname), &length, &size, &type, uniformname);
      LOG_CRITICAL("Uniform %d: %s (type: %d, size: %d)\n", i, uniformname, type, size);
    }
  }
  #endif/*Check uniform var*/
  glUniform1i(textureloc, 0);
  rendertilemap(&win->scn->map, win->shp.gameprog, (float)(win->windata.width)/(float)(win->windata.height));
  unbind_texture_array();
  unbind_shader_program();
}

void updatescene(struct scene *scn)
{
  
}
