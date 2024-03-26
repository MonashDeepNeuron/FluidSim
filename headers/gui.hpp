#pragma once

#include "utils.hpp"
#include <fmt/core.h>
#include "matrix_to_pixel.hpp"

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

        sf::RectangleShape (*draw_func)(std::array<float, BUFFER_SIZE>&, size_t, size_t);

        // identify draw function
        switch (current_draw_type) {
            case draw_type::GREY:
                draw_func = matrix_coords_to_greyscale_pixel;
                break;
            case draw_type::HSV:
                draw_func = matrix_coords_to_HSV_pixel;
                break;
            case draw_type::VEL:
                draw_func = matrix_coords_to_HSV_pixel;
                break;
            default:
                draw_func = matrix_coords_to_greyscale_pixel;
                break;
        }
        
        // loop the matrix and apply the function to each pixel.
        for (size_t i = 0uL; i < AXIS_SIZE + 2uL; i++) {
            for (size_t j = 0uL; j < AXIS_SIZE + 2uL; j++) {
                window.draw(draw_func(data, i, j));
            }
        }
        window.display();
    }

    auto is_open() -> bool { return window.isOpen(); }

    auto getRenderWindow() -> sf::RenderWindow& { return window; }

};