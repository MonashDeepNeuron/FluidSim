#pragma once

#include "const.hpp"
#include "gui.hpp"
#include "utils.hpp"

#include <fmt/core.h>

#include <cmath> // Include cmath for floor function
#include <cstddef>
#include <iostream>

using std::size_t;

// Adjust the value as needed

class EventManager {
private:
    gui& window;
    static sf::Vector2i prevPos;

public:
    explicit EventManager(gui& windowRef)
        : window(windowRef)
    {
    }

    inline auto key_pressed() const -> void { fmt::println("key pressed!"); }

    auto mouse_pressed(sf::Event event) -> size_t
    {
        // fmt::println("Mouse button pressed event received");

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            fmt::println("Mouse left pressed");

            auto x = event.mouseButton.x / S_CELL_SIZE;
            auto y = event.mouseButton.y / S_CELL_SIZE;

            // if (x <= 0 || y <= 0 || y >= S_AXIS_SIZE || x >= S_AXIS_SIZE) [[unlikely]] { // Don't do anything for boundary conditions
            //     // fmt::println("Left mouse button was pressed at ({}, {})", x, y);
            //     // fmt::println("yes");
            //     return 0;
            // }

            fmt::println("Left mouse button was pressed at ({}, {})", x, y);

            return IX(static_cast<size_t>(y), static_cast<size_t>(x));
        }

        return 0;
    }

    auto handle_event(sf::Event event) -> size_t
    {
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

    auto check_left_mouse_button() -> size_t
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto mousePos = sf::Mouse::getPosition(window.getRenderWindow());

            auto x = mousePos.x / S_CELL_SIZE;
            auto y = mousePos.y / S_CELL_SIZE;
            
            // if (x <= 0 || y <= 0 || y >= S_AXIS_SIZE || x >= S_AXIS_SIZE) [[unlikely]] { // Don't do anything for boundary conditions
            //     // fmt::println("Left mouse button was pressed at ({}, {})", x, y);
            //     // fmt::println("yes");
            //     return 0;
            // }

            return IX(sign_cast(y), sign_cast(x));
        }
        return 0;
    }

    auto mouse_vel() -> int {
      auto currPos = sf::Mouse::getPosition(window.getRenderWindow());      

      [[maybe_unused]] auto x_dirn = 1;
      [[maybe_unused]] auto y_dirn = 1;

      auto x = currPos.x / S_CELL_SIZE;
      auto y = currPos.y / S_CELL_SIZE;

      if (0 <= x && x <= S_AXIS_SIZE && 0 <= y && y <= S_AXIS_SIZE) [[likely]] {
         if (currPos != prevPos) { // Check if it's not the first call

            auto prevX = prevPos.x / S_CELL_SIZE;
            auto prevY = prevPos.y / S_CELL_SIZE;   

            // fmt::println("Previous mouse position: ({}, {})", prevX, prevY);

            if (x - prevX < 0) {
              x_dirn = -1;
            }

            if (y - prevY > 0) {
              y_dirn = -1;
            }

            auto x_dist = x_dirn * (x + prevX)^2;
            auto y_dist = y_dirn * (y + prevY)^2;

            fmt::println("Distance moved: {}, {}", x_dist, y_dist);   

         }

        prevPos = currPos;

      }
      return 0;
  }
};
