#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>

auto matrix_coords_to_greyscale_pixel(std::array<float, BUFFER_SIZE>& data, size_t i, size_t j) -> sf::RectangleShape;
auto matrix_coords_to_HSV_pixel(std::array<float, BUFFER_SIZE>& data, size_t i, size_t j) -> sf::RectangleShape;

namespace{ // Putting these methods in namespace makes them private.
           // They are only used locally by the 2 functions (above) that we want to export.
constexpr auto grey_scale(float value) -> sf::Uint8
{
    return static_cast<sf::Uint8>(std::clamp(value, 0.0f, 1.0f) * 255);
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
}


/**
 * @brief This function takes in a density-matrix with values from 0-1 and
 * converts each value in the matrix to a value from 0-255 then prints it
 * on a single pixel in an SFML window.
 *
 * @param window
 * @param densityArray
 */
auto matrix_coords_to_greyscale_pixel(std::array<float, BUFFER_SIZE>& data, size_t i, size_t j) -> sf::RectangleShape
{
    sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    auto xpos = static_cast<float>(j * CELL_SIZE);
    auto ypos = static_cast<float>(i * CELL_SIZE);

    pixel.setPosition(xpos, ypos);

    auto value = grey_scale(data.at(IX(i, j)));

    pixel.setFillColor(sf::Color(value, value, value));
    
    return pixel;
}

auto matrix_coords_to_HSV_pixel(std::array<float, BUFFER_SIZE>& data, size_t i, size_t j) -> sf::RectangleShape
{
    sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    auto xpos = static_cast<float>(j * CELL_SIZE);
    auto ypos = static_cast<float>(i * CELL_SIZE);

    pixel.setPosition(xpos, ypos);

    auto value = data.at(IX(i, j));
    auto color = HSV_to_RGB(value * 360.0f, 1.0f, 1.0f); // Assuming value is in the range [0, 1]

    pixel.setFillColor(color);

    return pixel;
}
