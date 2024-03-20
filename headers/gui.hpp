#pragma once

#include "utils.hpp"
#include <fmt/core.h>
#include "to_SFML.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <cmath>

using std::size_t;

class gui {
private:
    static constexpr size_t screen_width = (AXIS_SIZE + 2) * CELL_SIZE;
    static constexpr size_t screen_height = (AXIS_SIZE + 2) * CELL_SIZE;
    static constexpr size_t frame_limit = 144;

    const std::string window_text = "Fluid Simulation";
    sf::RenderWindow window;

    enum class draw_type : short { GREY,
    HSV,
    VEL };
    draw_type current_draw_type = draw_type::GREY;


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

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) {
            // Toggle to the next draw type
            switch (current_draw_type) {
            case draw_type::GREY:
                current_draw_type = draw_type::HSV;
                break;
            case draw_type::HSV:
                current_draw_type = draw_type::VEL;
                break;
            case draw_type::VEL:
                current_draw_type = draw_type::GREY;
                break;
            }
        }

        return event;
    }
    auto update_display(std::array<float, BUFFER_SIZE>& data) -> void
    {
        window.clear();
        switch (current_draw_type) {
        case draw_type::GREY:
            GreyScaleMatrixToSFML(data);
            break;
        case draw_type::HSV:
            HSV_to_SFML(data);
            break;
        case draw_type::VEL:
            HSV_to_SFML(data);
            break;
        default:
            GreyScaleMatrixToSFML(data);
        }
        window.display();
    }

    auto is_open() -> bool { return window.isOpen(); }

    auto getRenderWindow() -> sf::RenderWindow& { return window; }

    constexpr auto grey_scale(float value) -> sf::Uint8
    {
        return static_cast<sf::Uint8>(std::clamp(value, 0.0f, 1.0f) * 255);
    }

};