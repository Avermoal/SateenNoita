#ifndef ENGINECORE_EVENTS_H
#define ENGINECORE_EVENTS_H

struct GLFWwindow;

void init_events_system(struct GLFWwindow* pwindow);

void term_events_system(void);

void pollevents(void);

/*Callback interface*/
bool pressed(int keycode);
bool jpressed(int keycode);/*just pressed*/

bool clicked(int button);
bool jclicked(int button);/*just clicked*/

#endif/**ENGINECORE_EVENTS_H*/
