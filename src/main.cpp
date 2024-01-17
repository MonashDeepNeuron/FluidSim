#include "event_manager.hpp"
#include "gui.hpp"

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

  /*
  main tells gui about the fluids, and asks it for events
  gui draws the fluids, and tells main about events
  main gives those events to event manager
  event manager changes the fluids based on events
  events manager tells main about the changes to fluids
  repeat
  */

  gui fluid_gui = gui();
  event_manager my_event_manager = event_manager();

  while (fluid_gui.is_open()) {
    sf::Event event = fluid_gui.check_event();
    my_event_manager.handle_event(event);
    fluid_gui.update_display();
  }
  return 0;
}