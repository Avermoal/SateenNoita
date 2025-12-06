#include "EngineCore/display_manager.h"

#include <stddef.h>

void displayelement(struct element elem)
{
  glBindVertexArray(elem.vao);
  if(elem.indices){
    glDrawElements(GL_TRIANGLES, elem.indexcount, GL_UNSIGNED_INT, 0);
  }else{
    glDrawArrays(GL_TRIANGLES, 0, elem.vertexcount);
  }
  glBindVertexArray(0);
}

struct element createelement(struct vertex *vert, size_t vertexcount, unsigned int *indices, size_t indexcount, bool use_texture_layers, GLenum usage)
{
  struct element elem = {0, 0, 0, NULL, NULL, 0, 0};
  elem.vertexcount = vertexcount;
  elem.indexcount = indexcount;
  /*Fill vertex array*/
  elem.vert = calloc(vertexcount, sizeof(struct vertex));
  for(size_t i = 0; i < vertexcount; ++i){
    elem.vert[i] = vert[i];
  }
  /*Fill indices array if it exist*/
  if(indices && indexcount > 0){
    elem.indices = calloc(indexcount, sizeof(unsigned int));
    for(size_t i = 0; i < indexcount; ++i){
      elem.indices[i] = indices[i];
    }
  }
  /*Create OpenGL buffers*/
  glGenVertexArrays(1, &elem.vao);
  glGenBuffers(1, &elem.vbo);
  glBindVertexArray(elem.vao);
  /*Vertex buffer*/
  glBindBuffer(GL_ARRAY_BUFFER, elem.vbo);
  glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(struct vertex), vert, usage);
  /*Index buffer*/
  if(elem.indices){
    glGenBuffers(1, &elem.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(unsigned int), elem.indices, usage);
  }
  /*Vertex atribute configure*/
  /*Position atribute*/
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*)offsetof(struct vertex, pos));
  /*Texture atribute*/
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*)offsetof(struct vertex, tex));
  /*Texture layer atribute*/
  if(use_texture_layers){
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*)offsetof(struct vertex, texlayer));
  }
  /*OpenGL unbind vertex array*/
  glBindVertexArray(0);
  return elem;
}

void destroyelement(struct element *elem)
{
  if(elem->vert){
    free(elem->vert);
    elem->vert = NULL;
  }
  if(elem->indices){
    free(elem->indices);
    elem->indices = NULL;
  }
  if(elem->vao){
    glDeleteVertexArrays(1, &elem->vao);
  }
  if(elem->vbo){
    glDeleteBuffers(1, &elem->vbo);
  }
  if(elem->ebo){
    glDeleteBuffers(1, &elem->ebo);
  }
}
