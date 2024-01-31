#pragma once

#include <iostream>
#include <vector>

// Define grid parameters
const int N = 20;
const int size = (N + 2) * (N + 2);

// Macro for indexing
#define IX(i, j) ((i) + (N + 2) * (j))
// macro for switching array pointers
#define SWAP(x0, x)                                                            \
  {                                                                            \
    std::array<float, size> tmp = x0;                                          \
    x0 = x;                                                                    \
    x = tmp;                                                                   \
  }

// Define how many testing example iterations you want
const int iterations = 3;
// temporary changer of diffusion amount to visualise easier
const int diffuse_changer = 5;

void add_source(float *x, const float *s, float dt);
void set_bnd(int N, int b, float *x);
void diffuse(int N, int b, float *x, const float *x0, float diff, float dt);
void advect(int N, int b, float *d, const float *d0, const float *u,
            const float *v, float dt);
void dens_step(int N, float *x, float *x0, float *u, float *v, float diff,
               float dt);
void test_display(int N, float *x, int k);
