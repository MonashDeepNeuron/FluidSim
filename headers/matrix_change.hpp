#pragma once

#include "const.hpp"
#include <array>
#include <iostream>
#include <vector>

// Define grid parameters

template <int S> class DensitySolver {
public:
  DensitySolver(float diff, float dt)
      : size_(SIZE), diff_(diff), dt_(dt), u_{0.1f}, v_{0.1f}, x_{0.1f},
        x0_{0.1f} {}

  std::array<float, SIZE> run() {
    // x0_[200] = 10;
    // x0_[326] = 10;
    x0_[120] = 20;
    x0_[57] = 20;
    x0_[20] = 10;

    // First display
    test_display(0);

    // Call the dens_step function
    dens_step();

    // Output the result
    test_display(1);

    return x0_;
  }

private:
  int size_;
  float diff_;
  float dt_;
  int diffuse_changer = 10;
  int iterations = 10;
  std::array<float, S> u_;
  std::array<float, S> v_;
  std::array<float, S> x_;
  std::array<float, S> x0_;

  void add_source() {
    for (int i = 0; i < size_; ++i) {
      x_[i] += dt_ * x0_[i];
      if (x_[i] > 1) {
        x_[i] = 1;
      }
    }
  }

  void set_bnd(int b) {
    // Implementation of setting boundary conditions goes here
    // Replace with your specific implementation
  }

  void diffuse() {
    float a = dt_ * diff_ * N * N;

    for (int k = 0; k < diffuse_changer; k++) {
      for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
          x_[IX(i, j)] =
              (x0_[IX(i, j)] + a * (x_[IX(i - 1, j)] + x_[IX(i + 1, j)] +
                                    x_[IX(i, j - 1)] + x_[IX(i, j + 1)])) /
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
        float x = i - dt0 * u_[IX(i, j)];
        float y = j - dt0 * v_[IX(i, j)];

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

        x_[IX(i, j)] = s0 * (t0 * x0_[IX(i0, j0)] + t1 * x0_[IX(i0, j1)]) +
                       s1 * (t0 * x0_[IX(i1, j0)] + t1 * x0_[IX(i1, j1)]);
      }
    }

    set_bnd(0);
  }

  void dens_step() {
    add_source();
    SWAP(x0_, x_);
    diffuse();
    SWAP(x0_, x_);
    advect();
  }

  void test_display(int k) {
    std::cout << "Iteration " << k << "\n";
    std::cout << "Size " << static_cast<int>(x_.size()) << "\n";

    for (int i = 0; i < N + 2; ++i) {
      for (int j = 0; j < N + 2; ++j) {
        std::cout << x_[IX(i, j)] << ":" << IX(i, j) << ' ';
      }
      std::cout << std::endl;
    }
    std::cout << "\n\n";
  }
};
