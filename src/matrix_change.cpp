#include "../headers/matrix_change.hpp"
#include <iostream>
#include <vector>

// Define single-dimensional arrays for velocity and density

//=============================================================================

void add_source(std::array<float, size> x, const std::array<float, size> &s,
                float dt) {
  for (int i = 0; i < size; ++i) {
    x[i] += dt * s[i];
    if (x[i] > 1) {
      x[i] = 1;
    }
  }
}

//================================================================================

// Function to set boundary conditions
void set_bnd(int N, int b, std::array<float, size> &x) {
  // Implementation of setting boundary conditions goes here
  // Replace with your specific implementation
}

//==================================================================================

void diffuse(int N, int b, std::array<float, size> x,
             const std::array<float, size> &x0, float diff, float dt) {
  float a = dt * diff * N * N;

  for (int k = 0; k < diffuse_changer; k++) {
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        int index = i + N * j;
        x[index] = (x0[index] + a * (x[index - 1] + x[index + 1] +
                                     x[index - N] + x[index + N])) /
                   (1 + 4 * a);
      }
    }
    set_bnd(N, b, x);
  }
}

//==============================================================================

void advect(int N, int b, std::array<float, size> d,
            const std::array<float, size> &d0, const std::array<float, size> &u,
            const std::array<float, size> &v, float dt) {
  float dt0 = dt * N;

  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      float x = i - dt0 * u[IX(i, j)];
      float y = j - dt0 * v[IX(i, j)];

      // Clamp values to ensure they stay within bounds
      x = std::max(0.5f, std::min(static_cast<float>(N) + 0.5f, x));
      y = std::max(0.5f, std::min(static_cast<float>(N) + 0.5f, y));

      int i0 = static_cast<int>(x);
      int i1 = i0 + 1;
      int j0 = static_cast<int>(y);
      int j1 = j0 + 1;

      float s1 = x - i0;
      float s0 = 1 - s1;
      float t1 = y - j0;
      float t0 = 1 - t1;

      d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                    s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
    }
  }

  set_bnd(N, b, d);
}

//========================================================================

void dens_step(int N, std::array<float, size> x, std::array<float, size> x0,
               std::array<float, size> &u, std::array<float, size> &v,
               float diff, float dt) {
  add_source(x, x0, dt);
  SWAP(x0, x);
  diffuse(N, 0, x, x0, diff, dt);
  SWAP(x0, x);
  advect(N, 0, x, x0, u, v, dt);
}

//==================================================================================

void test_display(int N, std::array<float, size> x, int k) {
  std::cout << " Iteration " << k << "\n";
  for (int i = 1; i <= N; ++i) {
    for (int j = 1; j <= N; ++j) {
      int index = i + (N + 2) * j;
      std::cout << x[index] << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << "\n\n";
}

//=====================================================================================

void __main__() {

  std::array<float, size> u = {0.0};
  std::array<float, size> v = {0.0};
  std::array<float, size> x = {0.0};
  std::array<float, size> x0 = {0.0};

  // Initializing arrays: example test initialization
  for (int i = 0; i < size; ++i) {
    u[i] = 0.5;
    v[i] = 0.5;
  }

  float diff = 0.1; // Replace with your diffusion parameter
  float dt = 0.9;   // Replace with your time step

  // first display
  test_display(N, x, 0);

  for (int i = 0; i < iterations; ++i) {

    // reinitialize x0 to have just a single source location to add.
    for (int k = 0; k < size; ++k) {
      x0[k] = 0.0;
    }

    x0[31] = 0.9;

    // Call the dens_step function
    dens_step(N, x, x0, u, v, diff, dt);

    // Output the result
    test_display(N, x, i);
  }
}