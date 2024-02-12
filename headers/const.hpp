#pragma once

constexpr int N = 30;
constexpr int SIZE = (N + 2) * (N + 2);
constexpr int CELL_SIZE = 15;

#define IX(i, j) (i + (N + 2) * j)
#define SWAP(arr1, arr2) std::swap(arr1, arr2)
