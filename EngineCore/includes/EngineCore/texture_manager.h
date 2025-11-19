#ifndef ENGINECORE_TEXTURE_MANAGER_H
#define ENGINECORE_TEXTURE_MANAGER_H

#include <glad/glad.h>

struct texture{
  GLuint id;
  GLint width, height;
};

GLuint loadpng(const char *filename, GLint *width, GLint *height);

void loadtexture(struct texture *tex ,const char *filename);

void bindtexture(struct texture tex);
void unbindtexture();

void deletetexture(struct texture *tex);

#endif/*ENGINECORE_TEXTURE_MANAGER_H*/
