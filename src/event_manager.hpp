#pragma once

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gui.hpp" 

class event_manager {
private:
  gui& window;
  
  void key_pressed() { std::cout << "key pressed!\n"; }
  void mouse_pressed(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
      int x = event.mouseButton.x;
      int y = event.mouseButton.y;
      std::cout << "Left mouse button was pressed at (" << x << ", " << y<< ")\n";
      sf::CircleShape circle(100);
      circle.setFillColor(sf::Color::Transparent); // Transparent fill color
      circle.setOutlineColor(sf::Color::White);    // White outline color
      circle.setOutlineThickness(2);                // Outline thickness
      circle.setPosition(x, y);
      window.draw(circle);
    }
    // update fluids
    

  }

public:
  void handle_event(sf::Event event,gui& fluid_gui) { // TODO: add case when event is none
                                       // (most common case) at the start, so it
                                       // doesn't go through and check them all)
    switch (event.type) {
    case sf::Event::KeyPressed:
      key_pressed();
      break;
    case sf::Event::MouseButtonPressed:
      mouse_pressed(event, fluid_gui);
      break;
    default:
      break;
    }
  }
};