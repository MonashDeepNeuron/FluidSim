#pragma once

#include "const.hpp"
#include <array>
#include <iostream>
#include <vector>

// Define grid parameters

template <int S> class DensitySolver {
public:
  // Final array to display
  std::array<float, S> x;

  DensitySolver(float diff, float dt)
      : size_(SIZE), diff_(diff), dt_(dt), u_{0.01f}, v_{0.01f}, x{0.0f},
        x0_{0.0f} {}

  void add_density(float d, int index) {
    if (d > 1) {
      d = 1;
    }
    x[index] = d;
    // Remove add_source from dens_step to here so display can properly see the
    // addition of the source before being diffused.
  }

  void dens_step() {
    // add_source();
    SWAP(x0_, x);
    diffuse();
    test_display();
    SWAP(x0_, x);
    advect();
  }

  void test_display() {
    std::cout << "Size " << static_cast<int>(x.size()) << "\n";

    for (int i = 0; i < N + 2; ++i) {
      for (int j = 0; j < N + 2; ++j) {
        std::cout << x[IX(i, j)] << ":" << IX(i, j) << ' ';
      }
      std::cout << std::endl;
    }
    std::cout << "\n\n";
  }

private:
  int size_;
  float diff_;
  float dt_;
  int diffuse_changer = 20;
  std::array<float, S> u_;
  std::array<float, S> v_;
  std::array<float, S> x0_;

  void add_source() {
    for (int i = 0; i < size_; ++i) {
      x[i] += dt_ * x0_[i];
      if (x[i] > 1) {
        x[i] = 1;
      }
    }
  }

  void set_bnd(int b) {
    // Implementation of setting boundary conditions goes here
    // Replace with your specific implementation
  }

  void diffuse() {
    float a = dt_ * diff_ * N * N;

    for (int k = 0; k < 20; k++) {
      for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
          int index = i + (N + 2) * j;
          x[index] = (x0_[index] + a * (x[index - 1] + x[index + 1] +
                                        x[index - N - 2] + x[index + N + 2])) /
                     (1 + 4 * a);
        }
      }
      set_bnd(0);
    }
  }

  void advect() {
    float dt0 = dt_ * N;

    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        float a = i - dt0 * u_[IX(i, j)];
        float b = j - dt0 * v_[IX(i, j)];

        // Clamp values to ensure they stay within bounds
        a = std::max(0.5f, std::min(static_cast<float>(N) + 0.5f, a));
        b = std::max(0.5f, std::min(static_cast<float>(N) + 0.5f, b));

        int i0 = static_cast<int>(a);
        int i1 = i0 + 1;
        int j0 = static_cast<int>(b);
        int j1 = j0 + 1;

        float s1 = a - i0;
        float s0 = 1 - s1;
        float t1 = b - j0;
        float t0 = 1 - t1;

        x[IX(i, j)] = s0 * (t0 * x0_[IX(i0, j0)] + t1 * x0_[IX(i0, j1)]) +
                      s1 * (t0 * x0_[IX(i1, j0)] + t1 * x0_[IX(i1, j1)]);
      }
    }

    set_bnd(0);
  }
};