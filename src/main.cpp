#include "event_manager.hpp"
#include "gui.hpp"
#include "matrix_change.hpp"
#include "timer.hpp"
#include <SFML/Graphics.hpp>

#include <array>
#include <chrono>
#include <cstddef>
#include <thread>

// Aliases
using std::size_t;
using ms = std::chrono::milliseconds;

auto main() -> int
{
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
    std::array<float, BUFFER_SIZE> u_arr;
    std::array<float, BUFFER_SIZE> v_arr;

    // Initialize the velocity arrays
    u_arr.fill(0.1f); // From first snippet
    v_arr.fill(0.1f); // From first snippet

    // Create a DensitySolver instance
    auto ds = DensitySolver<BUFFER_SIZE>(0.0001f, 0.1f, 0.0001f, u_arr, v_arr);

    auto event_mouse_click = 0uL;

    timer bench_main("main");
    timer bench_update("update_display");

    while (fluid_gui.is_open()) {
        bench_main.start();

        // Check for events and handle them
        fluid_gui.check_event();

        // Handle left mouse button events
        event_mouse_click = my_event_manager.check_left_mouse_button();
        if (event_mouse_click != 0) {
            ds.add_density(10, event_mouse_click); // Can adjust the density value as needed
        }

        // Handle mouse drag events for velocity
        auto event_mouse_drag = my_event_manager.mouse_vel();
        if (event_mouse_drag != std::make_tuple(0.0f, 0.0f, static_cast<size_t>(1.00f))) {
            auto [x_dist, y_dist, location_index] = event_mouse_drag;
            ds.add_velocity(x_dist, y_dist, location_index);
        }

        bench_update.start();
        fluid_gui.update_display(ds.x(), ds.u(), ds.v());
        bench_update.stop();

        // Perform density and velocity steps
        ds.dens_step();
        ds.velocity_step();

        std::this_thread::sleep_for(ms(5)); // Sleep to control update frequency

        bench_main.stop();
    }

    bench_update.finish();
    bench_main.finish();
    return 0;
}
