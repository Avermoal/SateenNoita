#ifndef ENGINECORE_DISPLAY_MANAGER_H
#define ENGINECORE_DISPLAY_MANAGER_H

#include <stdlib.h>

#include <glad/glad.h>

struct vertex{
  float pos[3];
  float tex[2];/*Texture coordinates*/
};

struct element{
  GLuint vao, vbo, ebo;
  struct vertex *vert;
  unsigned int *indices;
  size_t vertexcount;
  size_t indexcount;
};

/*Draw 2D element on the screen*/
void displayelement(struct element elem);

struct element createelement(struct vertex *vert, size_t vertexcount, unsigned int *indices, size_t indexcount, GLenum usage);

void destroyelement(struct element *elem);

#endif/*ENGINECORE_DISPLAY_MANAGER_H*/
