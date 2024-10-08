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
    int iter = 0;

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
            // fmt::println("Mouse left pressed");

            auto x = event.mouseButton.x / S_CELL_SIZE;
            auto y = event.mouseButton.y / S_CELL_SIZE;

            if (x < 0 || y < 0 || y > S_AXIS_SIZE || x > S_AXIS_SIZE) [[unlikely]] { // Don't do anything for boundary conditions
                // fmt::println("Left mouse button was pressed at ({}, {})", x, y);
                // fmt::println("yes");
                return 0;
            }

            // fmt::println("Left mouse button was pressed at ({}, {})", x, y);

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
            auto mouse_pos = sf::Mouse::getPosition(window.getRenderWindow());

            auto x = mouse_pos.x / S_CELL_SIZE;
            auto y = mouse_pos.y / S_CELL_SIZE;

            if (x < 0 || y < 0 || y > S_AXIS_SIZE || x > S_AXIS_SIZE) [[unlikely]] { // Don't do anything for boundary conditions
                // fmt::println("Left mouse button was pressed at ({}, {})", x, y);
                // fmt::println("yes");
                return 0;
            }

            return IX(sign_cast(y), sign_cast(x));
        }
        return 0;
    }

    auto sigmoid(float x) -> float
    {
        // Clamps a value from 0.00 - 1.00
        return 1.00f / (1.00f + expf(-x));
    }

    auto normalize(float x) -> float
    {
        // return 0.1f * sigmoid(x) - 0.05f;

        //
        float range_buffer = 50.0f;
        return range_buffer * sigmoid(x) - range_buffer/2;
    }

    auto mouse_vel() -> std::tuple<float, float, size_t>
    {
        static sf::Vector2i prev_pos;
        auto mouse_pos = sf::Mouse::getPosition(window.getRenderWindow());

        if (iter == 0) {
            prev_pos = mouse_pos;
            iter++;
            return { 0.0f, 0.0f, static_cast<size_t>(1.00f) };
        }

        iter++;

        auto curr_x = static_cast<float>(mouse_pos.x) / S_CELL_SIZE;
        auto curr_y = static_cast<float>(mouse_pos.y) / S_CELL_SIZE;

        if (curr_x > 0 && curr_y > 0 && curr_y < S_AXIS_SIZE && curr_x < S_AXIS_SIZE) {
            auto prev_x = static_cast<float>(prev_pos.x) / S_CELL_SIZE;
            auto prev_y = static_cast<float>(prev_pos.y) / S_CELL_SIZE;

            auto x_dist = normalize(curr_x - prev_x);
            auto y_dist = normalize(curr_y - prev_y);

            prev_pos = mouse_pos;

            auto x = mouse_pos.x / S_CELL_SIZE;
            auto y = mouse_pos.y / S_CELL_SIZE;
            size_t location_index = IX(sign_cast(y), sign_cast(x));
            // std::cout << "X Velocity: " << x_dist << ", Y Velocity: " << y_dist << "index" << location_index << std::endl;

            return { x_dist, y_dist, location_index };
        }

        return { 0.0f, 0.0f, static_cast<size_t>(1.00f) };
    }
};
