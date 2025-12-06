#ifndef ENGINECORE_TEXTURE_MANAGER_H
#define ENGINECORE_TEXTURE_MANAGER_H

#include <glad/glad.h>

struct texture{
  GLuint id;
  GLint width, height;
};

struct texturearray{
  GLuint id;
  GLint size_in_pixels;
  GLuint layers;
};

GLuint loadpng(const char *filename, GLint *width, GLint *height);

void loadtexture(struct texture *tex ,const char *filename);

void bindtexture(struct texture tex);
void unbindtexture(void);

void deletetexture(struct texture *tex);

void load_texture_array(struct texturearray *texarr, GLint size_in_pixels, GLuint layers, const char *path_to_dir);

void bind_texture_array(struct texturearray texarr);
void unbind_texture_array(void);

void delete_texture_array(struct texturearray *texarr);

#endif/*ENGINECORE_TEXTURE_MANAGER_H*/
