#include <SFML/Graphics.hpp>

// Define macro for converting 2D array coordinates into 1D
#define IX(i, j) ((i) + (N + 2) * (j))

// Define grid size
constexpr int N = 100;
constexpr int SIZE = (N + 2) * (N + 2);

// TODO: Define fluid class
/* This class will contain all the variables and methods necessary to solve
    the Navier-Stokes equation for both density and velocity. */
class Fluid {
  std::vector<float> u, v, u_prev, v_prev, dens, dens_prev;
  float dt, diff, visc;
};

int main() {
  auto window = sf::RenderWindow{{1920u, 1080u}, "CMake SFML Project"};
  window.setFramerateLimit(144);

  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    window.display();
  }
}