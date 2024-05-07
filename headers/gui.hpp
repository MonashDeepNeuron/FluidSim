#pragma once

#include "utils.hpp"
#include "matrix_to_pixel.hpp"

#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>

#include <random>

using std::size_t;

class gui {
private:
    static constexpr size_t screen_width = (AXIS_SIZE)*CELL_SIZE;
    static constexpr size_t screen_height = (AXIS_SIZE)*CELL_SIZE;
    static constexpr size_t frame_limit = 144;

    const std::string window_text = "Fluid Simulation";
    sf::RenderWindow window;

    enum class draw_type : short { GREY,
        HSV,
        VEL };
    draw_type current_draw_type = draw_type::GREY;

public:
    gui()
    {
        // Used in order to smooth the vector field arrows
        sf::ContextSettings settings;
        settings.antialiasingLevel = 8; // This is a smoothness factor (higher = more computational power)
        window.create(sf::VideoMode(screen_width, screen_height), window_text, sf::Style::Default, settings);

        if (!window.isOpen()) { // We need this for some reason otherwise memory
                                // error.
            fmt::println("????");
        }

        window.setFramerateLimit(frame_limit);
    }

    draw_type get_current_draw_type() const { return current_draw_type; }

    auto check_event() -> sf::Event
    {
        sf::Event event;
        while (window.pollEvent(event)) { // while there are events in the queue
            // Handle the event
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
        }

        return event;
    }
    auto update_display(std::array<float, BUFFER_SIZE>& x_data, std::array<float, BUFFER_SIZE>& u_data, std::array<float, BUFFER_SIZE>& v_data) -> void
    {
        window.clear();
        switch (current_draw_type) {
        case draw_type::GREY:
            GreyScaleMatrixToSFML(x_data);
            break;
        case draw_type::HSV:
            HSV_to_SFML(x_data);
            break;
        case draw_type::VEL:
            VEL_to_SFML(u_data, v_data);
            break;
        default:
            GreyScaleMatrixToSFML(x_data);
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
        for (size_t i = 1uL; i <= AXIS_SIZE; i++) {
            for (size_t j = 1uL; j <= AXIS_SIZE; j++) {

                auto xpos = static_cast<float>((j - 1) * CELL_SIZE);
                auto ypos = static_cast<float>((i - 1) * CELL_SIZE);

                sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                pixel.setPosition(xpos, ypos);

                auto value = grey_scale(data.at(IX(i, j)));

                pixel.setFillColor(sf::Color(value, value, value));

                window.draw(pixel);
        }
        
        // loop the matrix and apply the function to each pixel.
        for (size_t i = 0uL; i < AXIS_SIZE + 2uL; i++) {
            for (size_t j = 0uL; j < AXIS_SIZE + 2uL; j++) {
                window.draw(draw_func(data, i, j));


    auto HSV_to_SFML(std::array<float, BUFFER_SIZE>& data) -> void
    {
        for (size_t i = 1uL; i <= AXIS_SIZE; i++) {
            for (size_t j = 1uL; j <= AXIS_SIZE; j++) {

                auto xpos = static_cast<float>((j - 1) * CELL_SIZE);
                auto ypos = static_cast<float>((i - 1) * CELL_SIZE);

                sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                pixel.setPosition(xpos, ypos);

                auto value = data.at(IX(i, j));
                auto color = HSV_to_RGB(value * 360.0f, 1.0f, 1.0f); // Assuming value is in the range [0, 1]

                pixel.setFillColor(color);

                window.draw(pixel);
            }
        }
    }
    
    auto is_open() -> bool { return window.isOpen(); }

    auto getRenderWindow() -> sf::RenderWindow& { return window; }

        return sf::Color(static_cast<sf::Uint8>((Rs + m) * 255.0f), static_cast<sf::Uint8>((Gs + m) * 255.0f), static_cast<sf::Uint8>((Bs + m) * 255.0f));
    }

    auto VEL_to_SFML(std::array<float, BUFFER_SIZE>& u_data, std::array<float, BUFFER_SIZE>& v_data) -> void
    {
        // std::default_random_engine generator;
        // std::uniform_real_distribution<float> distribution(0.0, 360.0);

        for (size_t i = 1uL; i <= AXIS_SIZE; i += 7) {
            for (size_t j = 1uL; j <= AXIS_SIZE; j += 7) {

                auto xpos = static_cast<float>((j - 1) * CELL_SIZE);
                auto ypos = static_cast<float>((i - 1) * CELL_SIZE);

                [[maybe_unused]] auto u = u_data.at(IX(i, j));
                [[maybe_unused]] auto v = v_data.at(IX(i, j));

                // fmt::println("{}:{} ", u, v);

                [[maybe_unused]] auto r = std::sqrt(u * u + v * v);
                auto theta = std::atan2(v, u); // direction of velocity, in radians
                theta = -theta; // Adjust for SFML's clockwise rotations
                if (theta < 0)
                    theta += 2 * static_cast<float>(M_PI); // Normalize to the range [0, 2π)

                sf::ConvexShape arrow;
                arrow.setPointCount(4); // 3 points for the arrow shape (isosceles triangle)

                // Calculate points for the arrow shape
                sf::Vector2f topPoint(CELL_SIZE * 2, 0); // Top point of the triangle
                sf::Vector2f leftPoint(0, CELL_SIZE * 2); // Left point of the triangle
                sf::Vector2f centerPoint(CELL_SIZE * 2, 0); // Center point of
                sf::Vector2f rightPoint(CELL_SIZE * 4, CELL_SIZE * 2); // Right point of the triangle

                // Set the points of the arrow
                arrow.setPoint(0, topPoint);
                arrow.setPoint(1, leftPoint);
                arrow.setPoint(2, centerPoint);
                arrow.setPoint(3, rightPoint);

                arrow.setFillColor(sf::Color::White);

                // float random_angle = distribution(generator); // Generate a random angle
                // arrow.setRotation(random_angle); // Rotate the arrow

                // arrow.setPosition(xpos + static_cast<float>(1.5) * CELL_SIZE, ypos + static_cast<float>(1.5) * CELL_SIZE);
                // arrow.setOrigin(CELL_SIZE / 2, CELL_SIZE / 2);

                arrow.setPosition(xpos + CELL_SIZE * 2, ypos + CELL_SIZE * 2); // Adjust position for larger size
                arrow.setOrigin(CELL_SIZE * 2, 0); // Set the origin to the base of the arrow

                arrow.setRotation(static_cast<float>(theta * 180 / static_cast<float>(M_PI))); // Rotate the arrow according to the direction of velocity
                // arrow.setRotation(135);
                window.draw(arrow);
            }
        }
    }
};