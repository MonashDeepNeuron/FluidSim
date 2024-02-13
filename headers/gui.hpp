#pragma once

#include "aliases.hpp"

#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <string_view>

class gui {
private:
    static constexpr size_t screen_width = (AXIS_SIZE + 2) * CELL_SIZE;
    static constexpr size_t screen_height = (AXIS_SIZE + 2) * CELL_SIZE;
    static constexpr size_t frame_limit = 144;
    static constexpr std::string_view window_text = "Fluid Simulation";

    sf::RenderWindow window;

public:
    gui()
        : window(sf::VideoMode(screen_width, screen_height), window_text.data())
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

    template <typename DrawFn>
    auto update_display(DrawFn draw_function, matrix_span_t<float> mtx) -> void
    {
        window.clear();
        draw_function(window, mtx);
        window.display();
    }

    auto is_open() -> bool { return window.isOpen(); }

    auto getRenderWindow() -> sf::RenderWindow& { return window; }
};
