#ifndef ENGINECORE_STATS_SYSTEM_H
#define ENGINECORE_STATS_SYSTEM_H

struct tile;
struct container;

struct stats{
  unsigned short HP;
  unsigned short MP;
  bool isalive;
};

void addHP(struct tile* mob, short HP);
void addMP(struct tile* mob, short MP);

struct stats update_player_stats_on_interface(struct container* cnt, struct tile* mob);

#endif/*ENGINECORE_STATS_SYSTEM_H*/
