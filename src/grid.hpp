#pragma once

#include <vector>

class fluid_grid {
private:
  const int cell_size = 1; // how many square pixels make up a cell.
                           // WARNING:
                           // Must divide evenly into pixels_x and pixels_y
  const int pixels_x;
  const int pixels_y;

  int size_x;
  int size_y;
  int size;

  std::vector<int> grid;

public:
  fluid_grid(int pixels_x, int pixels_y)
      : pixels_x(pixels_x), pixels_y(pixels_y) {
    size_x = pixels_x / cell_size;
    size_y = pixels_y / cell_size;
    size = (pixels_x + 2) * (pixels_y + 2);

    grid.resize(size);
  }

  // using getters and setters so that only grid has access to the underlying
  // vector.
  int get(int x, int y) { return grid[(((x) + (pixels_y + 2) * (y)))]; }
  void set(int x, int y, int set) {
    grid[(((x) + (pixels_y + 2) * (y)))] = set;
  }
};