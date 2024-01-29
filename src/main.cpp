#include "event_manager.hpp"
#include "gui.hpp"
#include <array>

// ./build/bin/CMakeSFMLProject

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

/*
This function takes in a density-matrix with values from 0-1 and converts each
value in the matrix to a value from 0-255 then prints it on a single pixel in
an SFML window
*/

void GreyScaleMatrixToSFML(
    sf::RenderWindow &window,
    const std::vector<std::vector<float>> &densityArray) {
  for (int i = 0; i < densityArray.size(); i++) {
    for (int j = 0; j < densityArray[i].size(); j++) {
      sf::RectangleShape pixel(sf::Vector2f(25, 25));
      pixel.setPosition(i * 25, j * 25);
      pixel.setFillColor(sf::Color(densityArray[i][j] * 255,
                                   densityArray[i][j] * 255,
                                   densityArray[i][j] * 255));
      window.draw(pixel);
    }
  }
}

int main() {

  // This is a dummy matrix just used for testing functionality
  std::vector<std::vector<float>> densityArray = {
      {0.2f, 0.8f, 0.5f, 0.3f, 0.7f, 0.1f, 0.4f, 0.6f, 0.9f, 0.2f},
      {0.6f, 0.3f, 0.8f, 0.1f, 0.5f, 0.9f, 0.2f, 0.7f, 0.4f, 0.3f},
      {0.4f, 0.7f, 0.2f, 0.9f, 0.3f, 0.6f, 0.8f, 0.5f, 0.1f, 0.7f},
      {0.9f, 0.2f, 0.7f, 0.5f, 0.1f, 0.8f, 0.3f, 0.6f, 0.4f, 0.2f},
      {0.5f, 0.6f, 0.1f, 0.7f, 0.4f, 0.2f, 0.9f, 0.8f, 0.3f, 0.5f},
      {0.1f, 0.4f, 0.9f, 0.2f, 0.8f, 0.7f, 0.5f, 0.3f, 0.6f, 0.9f},
      {0.8f, 0.5f, 0.3f, 0.6f, 0.9f, 0.4f, 0.7f, 0.2f, 0.1f, 0.8f},
      {0.3f, 0.9f, 0.6f, 0.4f, 0.2f, 0.5f, 0.1f, 0.8f, 0.7f, 0.4f},
      {0.7f, 0.1f, 0.4f, 0.8f, 0.6f, 0.3f, 0.2f, 0.9f, 0.5f, 0.1f},
      {0.2f, 0.3f, 0.5f, 0.1f, 0.7f, 0.9f, 0.6f, 0.4f, 0.8f, 0.3f}};

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
    my_event_manager.handle_event(event,fluid_gui);

    /*
    The update display is acting as a final step to display changes to fluid
    on every itteration on the loop

    we can pass in different drawing functions as the project grows
    */
    fluid_gui.update_display(GreyScaleMatrixToSFML, densityArray);
  }
  return 0;
}