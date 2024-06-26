#include "event_manager.hpp"
#include "gui.hpp"
#include "matrix_change.hpp"
#include "timer.hpp"
#include <SFML/Graphics.hpp>

#include <array>
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

    // Create SFML print manager
    // auto printing = sfml_printing();

    // Main loop
    std::array<float, BUFFER_SIZE> u_arr;
    std::array<float, BUFFER_SIZE> v_arr;

    u_arr.fill(0.1f);
    v_arr.fill(0.1f);
    
    auto ds = DensitySolver<BUFFER_SIZE>(0.0001f, 0.1f, 0.0001f, u_arr, v_arr);

    auto event_mouse_click = 0uL;

    timer bench_main("main");
    timer bench_update("update_display");
    while (fluid_gui.is_open()) {
        bench_main.start();
        // Check for events and handle them
        fluid_gui.check_event();

        // event_mouse_click = my_event_manager.handle_event(event);
        event_mouse_click = my_event_manager.check_left_mouse_button();
        if (event_mouse_click != 0) {
            ds.add_density(10, event_mouse_click);
        }
        bench_update.start();
        fluid_gui.update_display(ds.x(), ds.u(), ds.v());
        bench_update.stop();

        std::this_thread::sleep_for(ms(5));
        
        ds.dens_step();
        ds.velocity_step();

        my_event_manager.mouse_vel();

        bench_main.stop();
    }
    bench_update.finish();
    bench_main.finish();
    return 0;
} 