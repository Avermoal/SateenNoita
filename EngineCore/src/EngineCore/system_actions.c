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
  struct windowdata* windata = (struct windowdata*)glfwGetWindowUserPointer(win);
  if(!windata){
    return;
  }
  if(windata->isfullscreen){
    /*FULLSCREEN -> WINDOWED*/
    int winwidth = 800, winheight = 600;
    int winposx = 100, winposy = 100;
    glfwSetWindowMonitor(win, NULL, winposx, winposy, winwidth, winheight, 0);
    glfwSetWindowAttrib(win, GLFW_RESIZABLE, GLFW_TRUE);
    glfwSetWindowAttrib(win, GLFW_DECORATED, GLFW_TRUE);
    windata->width = winwidth;
    windata->height = winheight;
    int fb_w, fb_h;
    glfwGetFramebufferSize(win, &fb_w, &fb_h);
    windata->fb_width = fb_w;
    windata->fb_height = fb_h;
    glViewport(0, 0, fb_w, fb_h);
    windata->isfullscreen = false;
  }else{
    /*WINDOWED -> FULLSCREEN*/
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    glfwSetWindowAttrib(win, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetWindowAttrib(win, GLFW_DECORATED, GLFW_FALSE);
    windata->width = mode->width;
    windata->height = mode->height;
    int fb_w, fb_h;
    glfwGetFramebufferSize(win, &fb_w, &fb_h);
    windata->fb_width = fb_w;
    windata->fb_height = fb_h;
    glViewport(0, 0, fb_w, fb_h);
    windata->isfullscreen = true;
  }
}

