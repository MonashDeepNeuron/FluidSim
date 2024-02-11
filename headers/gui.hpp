#pragma once

#include "const.hpp"
#include "utils.hpp"

#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>

using std::size_t;

class gui {
private:
    static constexpr size_t screen_width = (AXIS_SIZE + 2) * CELL_SIZE;
    static constexpr size_t screen_height = (AXIS_SIZE + 2) * CELL_SIZE;
    static constexpr size_t frame_limit = 144;

    const std::string window_text = "Fluid Simulation";
    sf::RenderWindow window;

public:
    gui()
        : window(sf::VideoMode(screen_width, screen_height), window_text)
    {
        if (!window.isOpen()) { // We need this for some reason otherwise memory
                                // error.
            fmt::println("????");
        }

        window.setFramerateLimit(frame_limit);
    }

    auto check_event() -> sf::Event
    {
        sf::Event event {};

        if (!window.isOpen() || !window.pollEvent(event)) {
            // Return an empty event if the window is closed or
            // there are no more events
            return sf::Event();
        }

        // not part of event manager since it deals with
        // the window, not the fluids.
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        return event;
    }

    template <typename DrawFunction>
    auto update_display(DrawFunction draw_function, const std::array<float, BUFFER_SIZE>& data) -> void
    {
        window.clear();
        draw_function(window, data);
        window.display();
    }

    auto is_open() -> bool { return window.isOpen(); }

    auto getRenderWindow() -> sf::RenderWindow& { return window; }
};