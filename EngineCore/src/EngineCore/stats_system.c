#include "EngineCore/stats_system.h"

#include <string.h>
#include <stdio.h>

#include "EngineCore/tiles_map.h"
#include "EngineCore/text_display_system.h"
#include "EngineCore/game_interface.h"

#define STR_LEN 32

void addHP(struct tile* mob, short HP)
{
  if(HP >= mob->stts->HP || mob->stts->HP == 0){
    mob->stts->HP = 0;
    mob->stts->isalive = false;
    return;
  }
  mob->stts->HP += HP;
}

void addMP(struct tile* mob, short MP)
{
  mob->stts->MP += MP;
}

struct stats update_player_stats_on_interface(struct container* cnt, struct tile* mob)
{
  char buffer[STR_LEN];
  snprintf(buffer, sizeof(buffer), "HP:%d", mob->stts->HP);
  update_text_content(cnt, buffer);
  return *mob->stts;
}
