#include "aliases.hpp"
#include "event_manager.hpp"
#include "gui.hpp"
#include "matrix_change.hpp"

#include <SFML/Graphics.hpp>

#include <chrono>
#include <optional>
#include <thread>

// Aliases
using ms = std::chrono::milliseconds;

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
 * @param densityMtx
 */
auto GreyScaleMatrixToSFML(sf::RenderWindow& window, matrix_span_t<float> densityMtx) -> void
{
    for (auto i = 0uL; i < densityMtx.extent(0); ++i) {
        for (auto j { 0uL }; j < densityMtx.extent(1); ++j) {

            auto xpos = static_cast<float>(j * CELL_SIZE);
            auto ypos = static_cast<float>(i * CELL_SIZE);

            sf::RectangleShape pixel(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            pixel.setPosition(xpos, ypos);

#if MDSPAN_USE_BRACKET_OPERATOR
            auto value = grey_scale(densityMtx[i, j]);
#else
            auto value = grey_scale(densityMtx(i, j));
#endif

            pixel.setFillColor(sf::Color(value, value, value));

            window.draw(pixel);
        }
    }
}

auto main() -> int
{

    // std::array<float, BUFFER_SIZE> densityMtx = {
    //     0.2f, 0.8f, 0.5f, 0.3f, 0.7f, 0.1f, 0.4f, 0.6f, 0.9f, 0.2f, 0.6f, 0.3f,
    //     0.8f, 0.1f, 0.5f, 0.9f, 0.2f, 0.7f, 0.4f, 0.3f, 0.4f, 0.7f, 0.2f, 0.9f,
    //     0.3f, 0.6f, 0.8f, 0.5f, 0.1f, 0.7f, 0.9f, 0.2f, 0.7f, 0.5f, 0.1f, 0.8f,
    //     0.3f, 0.6f, 0.4f, 0.2f, 0.5f, 0.6f, 0.1f, 0.7f, 0.4f, 0.2f, 0.9f, 0.8f,
    //     0.3f, 0.5f, 0.1f, 0.4f, 0.9f, 0.2f, 0.8f, 0.7f, 0.5f, 0.3f, 0.6f, 0.9f,
    //     0.8f, 0.5f, 0.3f, 0.6f, 0.9f, 0.4f, 0.7f, 0.2f, 0.1f, 0.8f, 0.3f, 0.9f,
    //     0.6f, 0.4f, 0.2f, 0.5f, 0.1f, 0.8f, 0.7f, 0.4f, 0.7f, 0.1f, 0.4f, 0.8f,
    //     0.6f, 0.3f, 0.2f, 0.9f, 0.5f, 0.1f, 0.2f, 0.3f, 0.5f, 0.1f, 0.7f, 0.9f,
    //     0.6f, 0.4f, 0.8f, 0.3f};

    /*
    main tells gui about the fluids, and asks it for events
    gui draws the fluids, and tells main about events
    main gives those events to event manager
    event manager changes the fluids based on events
    events manager tells main about the changes to fluids
    repeat
    */

    auto fluid_gui = gui {};

    // Create an event manager and pass the GUI window to its constructor
    auto my_event_manager = EventManager(fluid_gui);

    // Main loop
    auto ds = DensitySolver<BUFFER_SIZE>(0.0001f, 0.1f);

    // Example of one mouse click event
    // ds.add_density(1, 53);

    // Example of a mouse drag event
    // ds.add_density(1, 53);
    // ds.add_density(1, 54);
    // ds.add_density(1, 55);
    // ds.add_density(1, 56);
    // ds.add_density(1, 57);
    // ds.add_density(1, 58);
    // ds.add_density(1, 59);
    // ds.add_density(1, 60);
    // ds.add_density(1, 61);

    auto event_mouse_click = std::optional<std::pair<size_t, size_t>>(std::nullopt);

    while (fluid_gui.is_open()) {

        // Check for events and handle them
        [[maybe_unused]] auto event = fluid_gui.check_event();

        // event_mouse_click = my_event_manager.handle_event(event);
        event_mouse_click = my_event_manager.check_left_mouse_button();

        if (event_mouse_click.has_value()) {
            auto [cidx, ridx] = event_mouse_click.value();
            ds.add_density(10, cidx, ridx);
        }

        // An example of three mouse presses at three individual spots across an
        // even velocity grid
        // ds.add_density(1, 142);
        // ds.add_density(1, 289);
        // ds.add_density(1, 435);

        fluid_gui.update_display(GreyScaleMatrixToSFML, ds.x());

        std::this_thread::sleep_for(ms(5));

        // ds.test_display();
        ds.dens_step();
    }

    return 0;
}
