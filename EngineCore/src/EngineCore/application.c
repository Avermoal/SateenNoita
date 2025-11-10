#include "EngineCore/application.h"

#include "EngineCore/window.h"

enum APP_EXIT_CODE appstart(void)
{
  /*Window creation*/
  struct window win;
  initwindow(&win, "Sateen Noita", 800, 700);
  if(!win.pwin){
    return APP_EXIT_CRITICAL;
  }
  while(win.windata.window_should_not_close){
    onupdate(&win);
  }
  return APP_EXIT_SUCCESS;
}
