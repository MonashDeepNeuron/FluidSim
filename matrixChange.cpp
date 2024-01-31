#include <iostream>
#include <vector>

// Define grid parameters
const int N = 20;
const int size = (N + 2) * (N + 2);

// Define single-dimensional arrays for velocity and density
float u[size] = {0.0};
float v[size] = {0.0};
float x[size] = {0.0};
float x0[size] = {0.0};

// Macro for indexing
#define IX(i, j) ((i) + (N + 2) * (j))
// macro for switching array pointers
#define SWAP(x0, x)                                                            \
  {                                                                            \
    float *tmp = x0;                                                           \
    x0 = x;                                                                    \
    x = tmp;                                                                   \
  }

// Define how many testing example iterations you want
const int iterations = 3;
// temporary changer of diffusion amount to visualise easier
const int diffuseChanger = 5;

//=============================================================================

void addSource(int N, float *x, const float *s, float dt) {
  int size = (N + 2) * (N + 2);
  for (int i = 0; i < size; ++i) {
    x[i] += dt * s[i];
    if (x[i] > 1) {
      x[i] = 1;
    }
  }
}

//================================================================================

// Function to set boundary conditions
void set_bnd(int N, int b, float *x) {
  // Implementation of setting boundary conditions goes here
  // Replace with your specific implementation
}

//==================================================================================

void diffuse(int N, int b, float *x, const float *x0, float diff, float dt) {
  float a = dt * diff * N * N;

  for (int k = 0; k < diffuseChanger; k++) {
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

void advect(int N, int b, float *d, const float *d0, const float *u,
            const float *v, float dt) {
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

void dens_step(int N, float *x, float *x0, float *u, float *v, float diff,
               float dt) {
  addSource(N, x, x0, dt);
  SWAP(x0, x);
  diffuse(N, 0, x, x0, diff, dt);
  SWAP(x0, x);
  advect(N, 0, x, x0, u, v, dt);
}

//==================================================================================

void testDisplay(int N, float *x, int k) {
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

int main() {

  // Initializing arrays: example test initialisation
  for (int i = 0; i < size; ++i) {
    u[i] = 0.5;
    v[i] = 0.5;
  }

  float diff = 0.1; // Replace with your diffusion parameter
  float dt = 0.9;   // Replace with your time step

  // first display
  testDisplay(N, x, 0);

  for (int i = 0; i < iterations; ++i) {

    // reinitialise x0 to have just a single source location to add.
    for (int k = 0; k < size; ++k) {
      x0[k] = 0.0;
    }
    x0[31] = 0.9;

    // Call the dens_step function
    dens_step(N, x, x0, u, v, diff, dt);

    // Output the result
    testDisplay(N, x, i);
  }

  return 0;
}