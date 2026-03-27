#ifndef MATHEMATICS_A_STAR_H
#define MATHEMATICS_A_STAR_H

struct gamemap;

/*Calc next step for mob. Returns true and fill next coord if path find*/
bool astar(struct gamemap* gmap, int starty, int startx, int goaly, int goalx, int* nextx, int* nexty);

#endif/*MATHEMATICS_A_STAR_H*/
