#pragma once

#include <SFML/Window/Event.hpp>
#include <iostream>

class event_manager {
private:
  void key_pressed() { std::cout << "key pressed!\n"; }
  void mouse_pressed(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      int x = event.mouseButton.x;
      int y = event.mouseButton.y;
      std::cout << "Left mouse button was pressed at (" << x << ", " << y
                << ")\n";
    }
    // update fluids
  }

public:
  void handle_event(sf::Event event) { // TODO: add case when event is none
                                       // (most common case) at the start, so it
                                       // doesn't go through and check them all)
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
