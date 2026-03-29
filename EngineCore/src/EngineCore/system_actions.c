#include "EngineCore/system_actions.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/window.h"
#include "EngineCore/events.h"

static void toggle_window_mode(GLFWwindow* win);

void systemactions(struct window* pwindow)
{
  if(jpressed(GLFW_KEY_ESCAPE)){
    pwindow->windata.window_should_not_close = false;
  }
  if(jpressed(GLFW_KEY_F11)){
    toggle_window_mode(pwindow->pwin);
  }
}

static void toggle_window_mode(GLFWwindow* win)
{
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  if(monitor){
    /*FULLSCREEN -> WINDOWED*/
    int winwidth = 800;
    int winheight = 600;
    int winposx = 100;
    int winposy = 100;
    /*NULL --- windowed mode*/
    glfwSetWindowMonitor(win, NULL, winposx, winposy, winwidth, winheight, 0);
    glfwSetWindowAttrib(win, GLFW_RESIZABLE, GLFW_TRUE);
    glfwSetWindowAttrib(win, GLFW_DECORATED, GLFW_TRUE);
  }else{
    /*WINDOWED -> FULLSCREEN*/
    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    /*Set fullscreen mode*/
    glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    glfwSetWindowAttrib(win, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetWindowAttrib(win, GLFW_DECORATED, GLFW_FALSE);
  }
}

