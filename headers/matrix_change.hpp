#pragma once

#include "const.hpp"
#include <array>
#include <iostream>
#include <vector>

// Define grid parameters

template <int S> class FluidSimulator {
public:
  FluidSimulator(float diff, float dt)
      : size_(SIZE), diff_(diff), dt_(dt), u_{0.1f}, v_{0.1f}, x_{0.1f},
        x0_{0.1f} {}

  void run() {
    x0_[200] = 10;
    x0_[326] = 10;
    x0_[120] = 20;
    x0_[20] = 10;

    // First display
    test_display(0);

    // Call the dens_step function
    dens_step();

    // Output the result
    test_display(1);
  }
  // Static method for indexing
  static int IX(int i, int j) { return (i) + (N + 2) * (j); }

  // Static method for swapping arrays
  template <typename T, std::size_t U>
  static void SWAP(std::array<T, U> &arr1, std::array<T, U> &arr2) {
    std::swap(arr1, arr2);
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
    }
  }

  void set_bnd(int b) {
    // Implementation of setting boundary conditions goes here
    // Replace with your specific implementation
  }

  void diffuse() {
    float a = dt_ * diff_ * size_ * size_;

    for (int k = 0; k < diffuse_changer; k++) {
      for (int i = 1; i <= size_; i++) {
        for (int j = 1; j <= size_; j++) {
          int index = i + size_ * j;
          x_[index] =
              (x0_[index] + a * (x_[index - 1] + x_[index + 1] +
                                 x_[index - size_] + x_[index + size_])) /
              (1 + 4 * a);
        }
      }
      set_bnd(0);
    }
  }

  void advect() {
    float dt0 = dt_ * size_;

    for (int i = 1; i <= size_; i++) {
      for (int j = 1; j <= size_; j++) {
        float x = i - dt0 * u_[IX(i, j)];
        float y = j - dt0 * v_[IX(i, j)];

        // Clamp values to ensure they stay within bounds
        x = std::max(0.5f, std::min(static_cast<float>(size_) + 0.5f, x));
        y = std::max(0.5f, std::min(static_cast<float>(size_) + 0.5f, y));

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
    std::cout << " Iteration " << k << "\n";
    for (int i = 1; i <= size_; ++i) {
      for (int j = 1; j <= size_; ++j) {
        int index = i + (size_ + 2) * j;
        std::cout << x_[index] << ' ';
      }
      std::cout << std::endl;
    }
    std::cout << "\n\n";
  }
};
