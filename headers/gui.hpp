#pragma once

#include "utils.hpp"
#include <fmt/core.h>

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

public:
    enum class draw_type : short { GREY,
        HSV,
        VEL };

    draw_type current_draw_type = draw_type::GREY;

    draw_type get_current_draw_type() const { return current_draw_type; }
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
    auto update_display(std::array<float, BUFFER_SIZE>& data, draw_type type) -> void
    {
        window.clear();
        switch (type) {
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

    auto HSV_to_SFML(std::array<float, BUFFER_SIZE>& data) -> void
    {
        for (size_t i = 0uL; i < AXIS_SIZE + 2uL; i++) {
            for (size_t j = 0uL; j < AXIS_SIZE + 2uL; j++) {

                auto xpos = static_cast<float>(j * CELL_SIZE);
                auto ypos = static_cast<float>(i * CELL_SIZE);

                sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                pixel.setPosition(xpos, ypos);

                auto value = data.at(IX(i, j));
                auto color = HSV_to_RGB(value * 360.0f, 1.0f, 1.0f); // Assuming value is in the range [0, 1]

                pixel.setFillColor(color);

                window.draw(pixel);
            }
        }
    }

    auto HSV_to_RGB(float H, float S, float V) -> sf::Color
    {
        float C = S * V;
        float X = C * (1.0f - std::abs(fmodf(H / 60.0f, 2.0f) - 1.0f));
        float m = V - C;
        float Rs, Gs, Bs;

        // Ensure H is within the range of 0 to 360
        H = fmodf(H, 360.0f);
        if (H < 0.0f)
            H += 360.0f;

        if (H >= 0.0f && H < 60.0f) {
            Rs = C;
            Gs = X;
            Bs = 0.0f;
        } else if (H >= 60.0f && H < 120.0f) {
            Rs = X;
            Gs = C;
            Bs = 0.0f;
        } else if (H >= 120.0f && H < 180.0f) {
            Rs = 0.0f;
            Gs = C;
            Bs = X;
        } else if (H >= 180.0f && H < 240.0f) {
            Rs = 0.0f;
            Gs = X;
            Bs = C;
        } else if (H >= 240.0f && H < 300.0f) {
            Rs = X;
            Gs = 0.0f;
            Bs = C;
        } else {
            Rs = C;
            Gs = 0.0f;
            Bs = X;
        }

        return sf::Color(static_cast<sf::Uint8>((Rs + m) * 255.0f), static_cast<sf::Uint8>((Gs + m) * 255.0f), static_cast<sf::Uint8>((Bs + m) * 255.0f));
    }
};