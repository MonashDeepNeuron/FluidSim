#pragma once

constexpr int N = 10;
constexpr int SIZE = (N + 2) * (N + 2);
constexpr int CELL_SIZE = 50;

#define IX(i, j) (i + (N + 2) * j)
// #define SWAP(arr1, arr2) std::swap(arr1, arr2)
#define SWAP(x0, x)                                                            \
  {                                                                            \
    auto tmp = x0;                                                             \
    x0 = x;                                                                    \
    x = tmp;                                                                   \
  }
