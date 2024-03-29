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

            if (x < 0 || S_AXIS_SIZE < x || y < 0 || S_AXIS_SIZE < y) [[unlikely]] {
                return 0;
            }

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

            if (0 <= x && x <= S_AXIS_SIZE && 0 <= y && y <= S_AXIS_SIZE) [[likely]] {
                //fmt::println("Left mouse button is being held down at ({}, {})", x, y);

                return IX(sign_cast(y), sign_cast(x));
            }
        }
        return 0;
    }
};
