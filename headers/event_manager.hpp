#pragma once

#include "const.hpp"
#include "gui.hpp"
#include <cmath> // Include cmath for floor function
#include <iostream>

// Adjust the value as needed

class event_manager {
private:
  gui &window;

public:
  explicit event_manager(gui &windowRef) : window(windowRef) {}

  inline void key_pressed() const { std::cout << "key pressed!\n"; }

  int mouse_pressed(sf::Event event) {
    //std::cout << "Mouse button pressed event received\n";
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      //std::cout << "Mouse left pressed\n";
      int x = event.mouseButton.x / CELL_SIZE;
      int y = event.mouseButton.y / CELL_SIZE;
      if (x < 0 || x > N || y < 0 || y > N) {
        return 0;
      }
      std::cout << "Left mouse button was pressed at (" << x << ", " << y
                << ")\n";
      return IX(y, x);
    }
    return 0;
  }

  int handle_event(sf::Event event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
      key_pressed();
      return 0;
    case sf::Event::MouseButtonPressed:
      return mouse_pressed(event);
    default:
      return 0;
    }
  }

  int check_left_mouse_button() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      sf::Vector2i mousePos = sf::Mouse::getPosition(window.getRenderWindow());
      int x = mousePos.x / CELL_SIZE;
      int y = mousePos.y / CELL_SIZE;
      if (x >= 0 && x <= N && y >= 0 && y <= N) {
        
        //std::cout << "Left mouse button is being held down at (" << x << ", "
        //          << y << ")\n";
        return IX(y, x);
      }
    }
    return 0;
  }
};
