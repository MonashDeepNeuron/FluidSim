#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <tuple>

class gui {
private:
  static constexpr int screen_width = 250;
  static constexpr int screen_height = 250;
  static constexpr int frame_limit = 144;
  const std::string window_text = "Fluid Simulation";
  sf::RenderWindow window;

public:
  gui() : window(sf::VideoMode(screen_width, screen_height), window_text) {
    if (!window.isOpen()) { // We need this for some reason otherwise memory
                            // error.
      std::cout << "????";
    }
    window.setFramerateLimit(frame_limit);
  }

  sf::Event check_event() {
    sf::Event event;
    if (!window.isOpen() || !window.pollEvent(event)) {
      return sf::Event(); // Return an empty event if the window is closed or
                          // there are no more events
    }
    if (event.type ==
        sf::Event::Closed) { // not part of event manager since it deals with
                             // the window, not the fluids.
      window.close();
    }
    return event;
  }

  template <typename DrawFunction>
  void update_display(DrawFunction draw_function,
                      const std::vector<std::vector<float>> &data) {
    window.clear();
    draw_function(window, data);
    window.display();
  }

  bool is_open() { return window.isOpen(); }

  sf::RenderWindow &getRenderWindow() { return window; }
};