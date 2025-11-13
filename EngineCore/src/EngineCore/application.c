#include "EngineCore/application.h"

#include "EngineCore/window.h"
#include "EngineCore/events.h"
#include "EngineCore/events_data.h"

enum APP_EXIT_CODE appstart(void)
{
  /*Window creation*/
  struct window win;
  initwindow(&win, "Sateen Noita", 800, 700);
  if(!win.pwin){
    return APP_EXIT_CRITICAL;
  }
  /*Events system init*/
  init_events_system(win.pwin);
  /*Main game loop*/
  while(win.windata.window_should_not_close){
    onupdate(&win);
  }
  /*Free memory*/
  term_events_system();
  termwindow(&win);
  return APP_EXIT_SUCCESS;
}
