#include "../headers/event_manager.hpp"
#include "../headers/gui.hpp"
#include "../headers/matrix_change.hpp"
#include <array>

// ./build/bin/CMakeSFMLProject

/*
This function takes in a density-matrix with values from 0-1 and converts each
value in the matrix to a value from 0-255 then prints it on a single pixel in
an SFML window
*/

void GreyScaleMatrixToSFML(
    sf::RenderWindow &window,
    const std::vector<std::vector<float>> &densityArray) {
  for (uint16_t i = 0; i < static_cast<uint16_t>(densityArray.size()); i++) {
    for (uint16_t j = 0; j < static_cast<uint16_t>(densityArray[i].size());
         j++) {
      sf::RectangleShape pixel(sf::Vector2f(25, 25));
      pixel.setPosition(i * 25, j * 25);
      pixel.setFillColor(sf::Color(
          static_cast<sf::Uint8>(densityArray[i][j] * sf::Uint8(255)),
          static_cast<sf::Uint8>(densityArray[i][j] * sf::Uint8(255)),
          static_cast<sf::Uint8>(densityArray[i][j] * sf::Uint8(255))));
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

  gui fluid_gui;

  // Create an event manager and pass the GUI window to its constructor
  event_manager my_event_manager(fluid_gui);

  // Main loop
  while (fluid_gui.is_open()) {
    // Check for events and handle them
    sf::Event event = fluid_gui.check_event();
    my_event_manager.handle_event(event);

    /*
    The update display is acting as a final step to display changes to fluid
    on every itteration on the loop

    we can pass in different drawing functions as the project grows
    */
    fluid_gui.update_display(GreyScaleMatrixToSFML, densityArray);
  }

  return 0;
}