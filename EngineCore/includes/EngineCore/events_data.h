#ifndef ENGINECORE_EVENTS_DATA_H
#define ENGINECORE_EVENTS_DATA_H

struct eventsdata{
  bool *keys;
	unsigned short *frames;
	unsigned short current;
	float x;
	float y;
};

static struct eventsdata evdata;

#endif/**ENGINECORE_EVENTS_DATA_H*/
