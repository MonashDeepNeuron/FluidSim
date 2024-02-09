#pragma once

constexpr int N = 100;
constexpr int SIZE = (N + 2) * (N + 2);
constexpr int CELL_SIZE = 5;

#define IX(i, j) (i + (N + 2) * j)
#define SWAP(arr1, arr2) std::swap(arr1, arr2)
