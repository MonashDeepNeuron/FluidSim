#pragma once

constexpr int N = 20;
constexpr int SIZE = (N + 2) * (N + 2);
constexpr int CELL_SIZE = 30;

#define IX(i, j) (i + (N + 2) * j)
#define SWAP(arr1, arr2) std::swap(arr1, arr2)
