#include "event_manager.hpp"
#include "gui.hpp"
#include "matrix_change.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <chrono>
#include <cstddef>
#include <thread>

#include <chrono>
#include <fstream>


// Aliases
using std::size_t;
using ms = std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

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
    auto ds = DensitySolver<BUFFER_SIZE>(0.0001f, 0.1f);

    auto event_mouse_click = 0uL;

    std::ofstream file("output.txt", std::ios_base::app);

    while (fluid_gui.is_open()) {
        auto start = high_resolution_clock::now();

        // Check for events and handle them
        fluid_gui.check_event();

        // event_mouse_click = my_event_manager.handle_event(event);
        event_mouse_click = my_event_manager.check_left_mouse_button();

        if (event_mouse_click != 0) {
            ds.add_density(3, event_mouse_click);
        }

        fluid_gui.update_display(ds.x());

        std::this_thread::sleep_for(ms(5));

        ds.dens_step();
        
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<ms>(stop - start);
        file << "Time taken by function: " << duration.count() << " microseconds\n";
    }
    file.close();
    return 0;
} 