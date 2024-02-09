#include "../headers/const.hpp"
#include "../headers/event_manager.hpp"
#include "../headers/gui.hpp"
#include "../headers/matrix_change.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include <thread>

// ./build/bin/CMakeSFMLProject

/*
This function takes in a density-matrix with values from 0-1 and converts each
value in the matrix to a value from 0-255 then prints it on a single pixel in
an SFML window
*/

void GreyScaleMatrixToSFML(sf::RenderWindow &window,
                           const std::array<float, SIZE> &densityArray) {
  for (int i = 0; i < N + 2; i++) {
    for (int j = 0; j < N + 2; j++) {

      sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
      pixel.setPosition((j * CELL_SIZE), (i * CELL_SIZE));
      pixel.setFillColor(
          sf::Color(static_cast<sf::Uint8>(densityArray[IX(i, j)] * 255),
                    static_cast<sf::Uint8>(densityArray[IX(i, j)] * 255),
                    static_cast<sf::Uint8>(densityArray[IX(i, j)] * 255)));

      window.draw(pixel);
    }
  }
}

int main() {

  // std::array<float, SIZE> densityArray = {
  //     0.2f, 0.8f, 0.5f, 0.3f, 0.7f, 0.1f, 0.4f, 0.6f, 0.9f, 0.2f, 0.6f, 0.3f,
  //     0.8f, 0.1f, 0.5f, 0.9f, 0.2f, 0.7f, 0.4f, 0.3f, 0.4f, 0.7f, 0.2f, 0.9f,
  //     0.3f, 0.6f, 0.8f, 0.5f, 0.1f, 0.7f, 0.9f, 0.2f, 0.7f, 0.5f, 0.1f, 0.8f,
  //     0.3f, 0.6f, 0.4f, 0.2f, 0.5f, 0.6f, 0.1f, 0.7f, 0.4f, 0.2f, 0.9f, 0.8f,
  //     0.3f, 0.5f, 0.1f, 0.4f, 0.9f, 0.2f, 0.8f, 0.7f, 0.5f, 0.3f, 0.6f, 0.9f,
  //     0.8f, 0.5f, 0.3f, 0.6f, 0.9f, 0.4f, 0.7f, 0.2f, 0.1f, 0.8f, 0.3f, 0.9f,
  //     0.6f, 0.4f, 0.2f, 0.5f, 0.1f, 0.8f, 0.7f, 0.4f, 0.7f, 0.1f, 0.4f, 0.8f,
  //     0.6f, 0.3f, 0.2f, 0.9f, 0.5f, 0.1f, 0.2f, 0.3f, 0.5f, 0.1f, 0.7f, 0.9f,
  //     0.6f, 0.4f, 0.8f, 0.3f};

  /*
  main tells gui about the fluids, and asks it for events
  gui draws the fluids, and tells main about events
  main gives those events to event manager
  event manager changes the fluids based on events
  events manager tells main about the changes to fluids
  repeat
  */

  gui fluid_gui;

  // Create an event manager and pass the GUI window to its constructor
  event_manager my_event_manager(fluid_gui);

  // Main loop
  DensitySolver<SIZE> ds(0.0001, 0.1);

  // Example of one mouse click event
  // ds.add_density(1, 53);

  // Example of a mouse drag event
  // ds.add_density(1, 53);
  // ds.add_density(1, 54);
  // ds.add_density(1, 55);
  // ds.add_density(1, 56);
  // ds.add_density(1, 57);
  // ds.add_density(1, 58);
  // ds.add_density(1, 59);
  // ds.add_density(1, 60);
  // ds.add_density(1, 61);

  int event_mouse_click;

  while (fluid_gui.is_open()) {
    // Check for events and handle them
    sf::Event event = fluid_gui.check_event();
    // event_mouse_click = my_event_manager.handle_event(event);
    event_mouse_click = my_event_manager.check_left_mouse_button();
    if (event_mouse_click != 0) {
      ds.add_density(1, event_mouse_click);
    }

    // An example of three mouse presses at three individual spots across an
    // even velocity grid
    // ds.add_density(1, 142);
    // ds.add_density(1, 289);
    // ds.add_density(1, 435);

    fluid_gui.update_display(GreyScaleMatrixToSFML, ds.x);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    // ds.test_display();
    ds.dens_step();
  }

  return 0;
}