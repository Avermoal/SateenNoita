#include "EngineCore/scene.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/window.h"
#include "EngineCore/texture_manager.h"
#include "EngineCore/text_display_system.h"
#include "EngineCore/game_window_info.h"
#include "EngineCore/tiles_info.h"
#include "EngineCore/stats_system.h"

#include "Mathematics/ortho_projection.h"

#define PROJ_MAT_SIZE 16
static void getprojection(float projection[PROJ_MAT_SIZE], float screenaspect);

void initscene(struct scene* scn, struct window* win)
{
  init_text_system();
  create_game_interface(&scn->gint);
  createtilemap(&scn->map);
}

void destroyscene(struct scene** scn)
{
  if(scn && *scn){
    destroytilemap(&((*scn)->map));
    destroy_game_interface(&((*scn)->gint));
    destroy_text_system();
    free(*scn);
    *scn = nullptr;
  }
}

void renderscene(struct window* win)
{
  float projection[PROJ_MAT_SIZE];
  getprojection(projection, (float)(win->windata.width)/(float)(win->windata.height));
  /*RENDER MENU*/

  /*RENDER TILEMAP*/
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
  if(textureloc == -1){
    LOG_CRITICAL("Uniform 'tileTextureArray' not found in shader program!\n");
    /*Input all uniform var in shader*/
    GLint uniformcount = 0;
    glGetProgramiv(win->shp.gameprog, GL_ACTIVE_UNIFORMS, &uniformcount);
    LOG_CRITICAL("Total active uniforms in shader: %d\n", uniformcount);
    char uniformname[256];
    for(GLint i = 0; i < uniformcount; i++){
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(win->shp.gameprog, i, sizeof(uniformname), &length, &size, &type, uniformname);
      LOG_CRITICAL("Uniform %d: %s (type: %d, size: %d)\n", i, uniformname, type, size);
    }
  }
  #endif/*Check uniform var*/
  glUniform1i(textureloc, 0);
  /*Send projection matrix to shader*/
  GLuint projectionloc = glGetUniformLocation(win->shp.gameprog, "projview");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection);
  rendertilemap(&win->scn->map, win->shp.gameprog, (float)(win->windata.width)/(float)(win->windata.height));
  unbind_texture_array();
  unbind_shader_program();
  /*RENDER GAME INTERFACE*/
  bind_shader_program(win->shp.interfaceprog);
  glActiveTexture(GL_TEXTURE0);
  bind_texture_array(win->scn->gint.texarray);
  textureloc = glGetUniformLocation(win->shp.interfaceprog, "interfaceTextureArray");
  #ifndef NDEBUG/*Check uniform var*/
  if(textureloc == -1){
    LOG_CRITICAL("Uniform 'interfaceTextureArray' not found in shader program!");
    /*Input all uniform var in shader*/
    GLint uniformcount = 0;
    glGetProgramiv(win->shp.interfaceprog, GL_ACTIVE_UNIFORMS, &uniformcount);
    LOG_CRITICAL("Total active uniforms in shader: %d\n", uniformcount);
    char uniformname[256];
    for(GLint i = 0; i < uniformcount; ++i){
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(win->shp.interfaceprog, i, sizeof(uniformname), &length, &size, &type, uniformname);
      LOG_CRITICAL("Uniform %d: %s (type: %d, size: %d)\n", i, uniformname, type, size);
    }
  }
  #endif/*Check uniform var*/
  glUniform1i(textureloc, 0);
  /*Send projection matrix to shader*/
  projectionloc = glGetUniformLocation(win->shp.interfaceprog, "projview");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection);
  render_game_interface(&win->scn->gint, (float)(win->windata.width)/(float)(win->windata.height));
  unbind_texture_array();
  unbind_shader_program();
  /*RENDER TEXT*/
  bind_shader_program(win->shp.textprog);
  glActiveTexture(GL_TEXTURE0);
  bind_texture_array(win->scn->gint.texarray);
  textureloc = glGetUniformLocation(win->shp.textprog, "fontTextureArray");
  #ifndef NDEBUG/*Check uniform var*/
  if(textureloc == -1){
    LOG_CRITICAL("Uniform 'textTextureArray' not found in shader program!");
    /*Input all uniform var in shader*/
    GLint uniformcount = 0;
    glGetProgramiv(win->shp.textprog, GL_ACTIVE_UNIFORMS, &uniformcount);
    char uniformname[256];
    for(GLint i = 0; i < uniformcount; ++i){
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(win->shp.textprog, i, sizeof(uniformname), &length, &size, &type, uniformname);
      LOG_CRITICAL("Uniform %d: %s (type: %d, size: %d)\n", i, uniformname, type, size);
    }
  }
  #endif/*Check uniform var*/
  glUniform1i(textureloc, 0);
  /*Send projection matrix to shader*/
  projectionloc = glGetUniformLocation(win->shp.textprog, "projview");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection);
  glDisable(GL_DEPTH_TEST);
  rendertext(&win->scn->gint.statslist);
  glEnable(GL_DEPTH_TEST);
  unbind_texture_array();
  unbind_shader_program();
  /*RENDER INVENTORY*/
  bind_shader_program(win->shp.interfaceprog);
  glActiveTexture(GL_TEXTURE0);
  bind_texture_array(win->scn->gint.inv.texarray);
  textureloc = glGetUniformLocation(win->shp.interfaceprog, "interfaceTextureArray");
  #ifndef NDEBUG/*Check uniform var*/
  if(textureloc == -1){
    LOG_CRITICAL("Uniform 'interfaceTextureArray' not found in shader program!");
    /*Input all uniform var in shader*/
    GLint uniformcount = 0;
    glGetProgramiv(win->shp.interfaceprog, GL_ACTIVE_UNIFORMS, &uniformcount);
    LOG_CRITICAL("Total active uniforms in shader: %d\n", uniformcount);
    char uniformname[256];
    for(GLint i = 0; i < uniformcount; ++i){
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(win->shp.interfaceprog, i, sizeof(uniformname), &length, &size, &type, uniformname);
      LOG_CRITICAL("Uniform %d: %s (type: %d, size: %d)\n", i, uniformname, type, size);
    }
  }
  #endif/*Check uniform var*/
  glUniform1i(textureloc, 0);
  /*Send projection matrix to shader*/
  projectionloc = glGetUniformLocation(win->shp.interfaceprog, "projview");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection);
  renderinventory(&win->scn->gint.inv);
  unbind_texture_array();
  unbind_shader_program();
  /*RENDER ITEMS in inventory*/
  bind_shader_program(win->shp.interfaceprog);
  glActiveTexture(GL_TEXTURE0);
  bind_texture_array(win->scn->gint.inv.items_tex_array);
  textureloc = glGetUniformLocation(win->shp.interfaceprog, "interfaceTextureArray");
  #ifndef NDEBUG/*Check uniform var*/
  if(textureloc == -1){
    LOG_CRITICAL("Uniform 'interfaceTextureArray' not found in shader program!");
    /*Input all uniform var in shader*/
    GLint uniformcount = 0;
    glGetProgramiv(win->shp.interfaceprog, GL_ACTIVE_UNIFORMS, &uniformcount);
    LOG_CRITICAL("Total active uniforms in shader: %d\n", uniformcount);
    char uniformname[256];
    for(GLint i = 0; i < uniformcount; ++i){
      GLsizei length;
      GLint size;
      GLenum type;
      glGetActiveUniform(win->shp.interfaceprog, i, sizeof(uniformname), &length, &size, &type, uniformname);
      LOG_CRITICAL("Uniform %d: %s (type: %d, size: %d)\n", i, uniformname, type, size);
    }
  }
  #endif/*Check uniform var*/
  glUniform1i(textureloc, 0);
  /*Send projection matrix to shader*/
  projectionloc = glGetUniformLocation(win->shp.interfaceprog, "projview");
  glUniformMatrix4fv(projectionloc, 1, GL_FALSE, projection);
  renderitems(&win->scn->gint.inv);
  unbind_texture_array();
  unbind_shader_program();
}

void updatescene(struct scene* scn)
{
  updatetilemap(&scn->map);
  if(!update_player_stats_on_interface(&scn->gint.statslist, &scn->map.gmap.mobs[scn->map.gmap.pcy][scn->map.gmap.pcx]).isalive){
    LOG_CRITICAL("GAME OVER\n");
  }
}

static void getprojection(float projection[PROJ_MAT_SIZE], float screenaspect)
{
  /*Calculate orthographic projection matrix*/
  float mapwidth = MAP_WIDTH * TILE_SIZE;
  float mapheight = MAP_HEIGHT * TILE_SIZE;
  float mapaspect = mapwidth / mapheight;
  float left = 0, right = 0, bottom = 0, top = 0;
  float visiblewidth = 0, visibleheight = 0;
  if(screenaspect > mapaspect){
    visibleheight = mapheight;
    visiblewidth = mapheight * screenaspect;
  }else{
    visiblewidth = mapwidth;
    visibleheight = mapwidth / screenaspect;
  }

  left = (mapwidth - visiblewidth) / 2.0f;
  right = mapwidth - left;
  bottom = (mapheight - visibleheight) / 2.0f;
  top = mapheight - bottom;
  create_ortho_projection(projection, left, right, bottom, top);
}
