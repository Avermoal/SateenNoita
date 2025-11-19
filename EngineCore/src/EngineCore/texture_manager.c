#include "EngineCore/texture_manager.h"

#include "EngineCore/debug_log.h"

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
	unsigned char* img = stbi_load(filename, width, height, &channels, 0);
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

void unbindtexture()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void deletetexture(struct texture *tex)
{
  glDeleteTextures(1, &tex->id);
  tex->width = 0;
  tex->height = 0;
}
