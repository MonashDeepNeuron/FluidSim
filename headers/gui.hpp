#pragma once

#include "matrix_to_pixel.hpp"
#include "utils.hpp"

#include <fmt/core.h>

#include <SFML/Graphics.hpp>

#include <array>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>

#include <random>

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
        }
    }

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

    auto VEL_to_SFML(std::array<float, BUFFER_SIZE>& u_data, std::array<float, BUFFER_SIZE>& v_data) -> void
    {
        // Define a scaling factor to adjust the size of the arrows
        const float scale_factor = 90.0f; // You can adjust this to get the desired arrow length

        for (size_t i = 1uL; i <= AXIS_SIZE; i += 7) {
            for (size_t j = 1uL; j <= AXIS_SIZE; j += 7) {

                auto xpos = static_cast<float>((j - 1) * CELL_SIZE);
                auto ypos = static_cast<float>((i - 1) * CELL_SIZE);

                auto u = u_data.at(IX(i, j));
                auto v = v_data.at(IX(i, j));

                auto r = std::sqrt((u * u) + (v * v)); // Magnitude of the velocity vector
                auto theta = std::atan2(v, u); // Corrected direction of velocity, in radians

                // Create the arrow shape
                sf::ConvexShape arrow;
                arrow.setPointCount(7); // Arrow with 7 points

                // Define points for the arrow
                float arrow_length = r * scale_factor; // Scale the arrow based on velocity magnitude
                float arrow_width = 40.0f; // Fixed width for the arrow body

                sf::Vector2f point1(arrow_length, 0); // Tip of the arrow
                sf::Vector2f point2(0, arrow_width); // Left-top corner of the body
                sf::Vector2f point3(arrow_length * 0.2f, arrow_width); // Mid-left of the body
                sf::Vector2f point4(arrow_length * 0.2f, arrow_length); // Bottom-left of the arrow
                sf::Vector2f point5(arrow_length * 0.5f, arrow_length); // Bottom-right of the arrow
                sf::Vector2f point6(arrow_length * 0.5f, arrow_width); // Mid-right of the body
                sf::Vector2f point7(arrow_length, arrow_width); // Right-top corner of the body

                // Set the points in the arrow shape
                arrow.setPoint(0, point1);
                arrow.setPoint(1, point2);
                arrow.setPoint(2, point3);
                arrow.setPoint(3, point4);
                arrow.setPoint(4, point5);
                arrow.setPoint(5, point6);
                arrow.setPoint(6, point7);

                arrow.setFillColor(sf::Color::White);

                // Set the position and rotation
                arrow.setPosition(xpos + CELL_SIZE * 0.5f, ypos + CELL_SIZE * 0.5f); // Center the arrow on the pixel
                arrow.setOrigin(0, arrow_width / 2); // Set origin to the tip of the arrow

                // Rotate the arrow to point in the correct direction
                arrow.setRotation(static_cast<float>(theta * 180.0f / static_cast<float>(M_PI))); // Correct angle

                window.draw(arrow);
            }
        }
    }
};