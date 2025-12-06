#include "EngineCore/texture_manager.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "EngineCore/debug_log.h"
#include "EngineCore/tiles_info.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>

GLuint loadpng(const char *filename, GLint *width, GLint *height)
{
  int channels = 0;
	GLuint texid = 0;
  /*Flip stbi loading image configure*/
	stbi_set_flip_vertically_on_load(true);
  /*Load image*/
	unsigned char *img = stbi_load(filename, width, height, &channels, 0);
	if (!img) {
		LOG_CRITICAL("[TEXTURE LOAD]: png file not load");
		return 0;
	}
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	switch (channels) {
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)img);
		break;
	case 3: 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)img);
		break;
	default :
	  LOG_CRITICAL("[TEXTURE LOAD]: texture channels not 4 or 3");
		glBindTexture(GL_TEXTURE_2D, 0);
		return 0;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
  /*Free section*/
	stbi_image_free(img);
	return texid;
}

void loadtexture(struct texture *tex, const char *filename)
{
  tex->id = loadpng(filename, &tex->width, &tex->height);
  if(!tex){
    LOG_CRITICAL("[TEXTURE]: Texture not load");
    return;
  }
}

void bindtexture(struct texture tex)
{
  glBindTexture(GL_TEXTURE_2D, tex.id);
}

void unbindtexture(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void deletetexture(struct texture *tex)
{
  glDeleteTextures(1, &tex->id);
  tex->width = 0;
  tex->height = 0;
}

void load_texture_array(struct texturearray *texarr, GLint size_in_pixels, GLuint layers, const char *path_to_dir)
{
  GLuint texarr_id = 0;
  /*Flip stbi loading image configure*/
  stbi_set_flip_vertically_on_load(true);
  /*Texture array creation*/
  glGenTextures(1, &texarr_id);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texarr_id);
  /*Memory allocation*/
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, size_in_pixels, size_in_pixels, layers);
  /*Load textures in order*/
  const char *texture_files[] = {
    "ground.png",
    "grass.png", 
    "wall.png"
  };
  for(GLuint i = 0; i < layers; ++i){
    /*Full path to images*/
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s/%s", path_to_dir, texture_files[i]);
    int width, height, channels;
    unsigned char *img = stbi_load(full_path, &width, &height, &channels, 4);
    if(!img){
      LOG_CRITICAL("Failed to load texture: %s\n", full_path);
      continue;
    }
    if(width != size_in_pixels || height != size_in_pixels){
      LOG_CRITICAL("Texture %s has wrong size: %dx%d, expected %dx%d\n", 
                   texture_files[i], width, height, size_in_pixels, size_in_pixels);
    }
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, size_in_pixels, size_in_pixels, 1, GL_RGBA, GL_UNSIGNED_BYTE, img);
    stbi_image_free(img);
  }
  /*Set texture array data*/
  texarr->id = texarr_id;
  texarr->size_in_pixels = size_in_pixels;
  texarr->layers = layers;
  /*Texture array settings*/
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  /*Unbind texture array*/
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void bind_texture_array(struct texturearray texarr)
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, texarr.id);
}

void unbind_texture_array(void)
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void delete_texture_array(struct texturearray *texarr)
{
  glDeleteTextures(1, &texarr->id);
  texarr->size_in_pixels = 0;
  texarr->layers = 0;
}
