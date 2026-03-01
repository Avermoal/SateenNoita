#ifndef MATHEMATICS_PERLIN_NOISE_H
#define MATHEMATICS_PERLIN_NOISE_H

void init_perlin_noise(int seed);

void perlinnoise(int maph, int mapw, float (*map)[mapw]);

#endif/*MATHEMATICS_PERLIN_NOISE_H*/
