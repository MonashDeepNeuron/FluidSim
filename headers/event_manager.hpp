#pragma once

#include "gui.hpp"
#include <SFML/Window/Event.hpp>
#include <cmath> // Include cmath for floor function
#include <iostream>

constexpr int CELL_SIZE = 25; // Adjust the value as needed

class event_manager {
private:
  gui &window;

public:
  explicit event_manager(gui &windowRef) : window(windowRef) {}

  inline void key_pressed() const { std::cout << "key pressed!\n"; }

  void mouse_pressed(sf::Event event) {
    std::cout << "Mouse button pressed event received\n";
    if (event.mouseButton.button == sf::Mouse::Left) {
      int x = event.mouseButton.x;
      int y = event.mouseButton.y;
      std::cout << "Left mouse button was pressed at (" << x << ", " << y
                << ")\n";
      sf::CircleShape circle(50); // Create a circle shape with radius 50
      circle.setFillColor(sf::Color::Blue); // Set circle fill color

      // Adjust circle position to center it on the mouse click
      circle.setPosition(x - circle.getRadius(), y - circle.getRadius());

      window.getRenderWindow().draw(circle); // Draw circle on the window
      window.getRenderWindow().display();    // Display the updated window
    }
  }

  void handle_event(sf::Event event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
      key_pressed();
      break;
    case sf::Event::MouseButtonPressed:
      mouse_pressed(event);
      break;
    default:
      break;
    }
  }
};