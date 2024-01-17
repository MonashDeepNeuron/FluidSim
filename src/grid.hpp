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
    size_x = pixels_x / cell_size - 1; // -1 for boundary condition.
    size_y = pixels_y / cell_size - 1;
    size = (pixels_x + 2) * (pixels_y + 2);

    std::vector<int> grid(size);
  }
  int address(int x, int y) {
    // return ( ((i)+(N+2)*(j)))
  }
};