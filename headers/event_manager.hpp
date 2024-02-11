#pragma once

#include "const.hpp"
#include "gui.hpp"
#include "utils.hpp"

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

    inline auto key_pressed() const -> void { std::cout << "key pressed!\n"; }

    auto mouse_pressed(sf::Event event) -> size_t
    {
        std::cout << "Mouse button pressed event received\n";
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            std::cout << "Mouse left pressed\n";

            auto x = event.mouseButton.x / S_CELL_SIZE;
            auto y = event.mouseButton.y / S_CELL_SIZE;

            if (x < 0 || S_AXIS_SIZE < x || y < 0 || S_AXIS_SIZE < y) [[unlikely]] {
                return 0;
            }

            std::cout << "Left mouse button was pressed at ("
                      << x
                      << ", "
                      << y
                      << ")\n";

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
                std::cout << "Left mouse button is being held down at ("
                          << x
                          << ", "
                          << y
                          << ")\n";

                return IX(sign_cast(y), sign_cast(x));
            }
        }
        return 0;
    }
};
