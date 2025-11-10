#include "EngineCore/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void initwindow(struct window *pwindow, const char *title, int width, int height)
{
  /*GLFW init*/
  if(!glfwInit()){
    //LOG_CRITICAL
    return;
  }
  /*Window creation*/
  if(pwindow){
    pwindow->pwin = glfwCreateWindow(width, height, title, NULL, NULL);
  }else{
    //LOG_CRITICAL
    return;
  }
  if(!pwindow->pwin){
    //LOG_CRITICAL
    return;
  }
  /*Make window current context*/
  glfwMakeContextCurrent(pwindow->pwin);
  /*OpenGL function load*/
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    //LOG_CRITICAL
    termwindow(pwindow);
    return;
  }
  /*Window set data*/
  pwindow->windata.width = width;
  pwindow->windata.height = height;
  pwindow->windata.title = title;
  pwindow->windata.window_should_not_close = 1;

}

void termwindow(struct window *pwindow)
{
  glfwDestroyWindow(pwindow->pwin);
	glfwTerminate();
}

void onupdate(struct window *pwindow)
{
  /*Clear window buffer*/
  glClear(GL_COLOR_BUFFER_BIT);
  /*GL swap buffeers*/
  glfwSwapBuffers(pwindow->pwin);
  /*Poll for process events*/
  glfwPollEvents();
}
