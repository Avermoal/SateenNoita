#ifndef MATHEMATICS_NOISES_H
#define MATHEMATICS_NOISES_H

void init_perlin_noise(int seed);

void perlin_noise_map_gen(int maph, int mapw, float (*map)[mapw], int lbc);

#endif/*MATHEMATICS_NOISES_H*/
