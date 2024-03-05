#pragma once

#include "utils.hpp"
#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include <array>
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
    auto update_display(std::array<float, BUFFER_SIZE>& data) -> void
    {
        window.clear();
        GreyScaleMatrixToSFML(data);
        window.display();
    }

    auto is_open() -> bool { return window.isOpen(); }

    auto getRenderWindow() -> sf::RenderWindow& { return window; }

    constexpr auto grey_scale(float value) -> sf::Uint8
    {
        return static_cast<sf::Uint8>(std::clamp(value, 0.0f, 1.0f) * 255);
    }

    /**
     * @brief This function takes in a density-matrix with values from 0-1 and
     * converts each value in the matrix to a value from 0-255 then prints it
     * on a single pixel in an SFML window.
     *
     * @param window
     * @param densityArray
     */
    auto GreyScaleMatrixToSFML(std::array<float, BUFFER_SIZE>& data) -> void
    {
        for (size_t i = 0uL; i < AXIS_SIZE + 2uL; i++) {
            for (size_t j = 0uL; j < AXIS_SIZE + 2uL; j++) {

                auto xpos = static_cast<float>(j * CELL_SIZE);
                auto ypos = static_cast<float>(i * CELL_SIZE);

                sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                pixel.setPosition(xpos, ypos);

                auto value = grey_scale(data.at(IX(i, j)));

                pixel.setFillColor(sf::Color(value, value, value));

                window.draw(pixel);
            }
        }
    }
};