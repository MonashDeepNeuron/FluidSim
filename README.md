# Fluid Simulation

Eulerian fluid simulation program based on the Navier-Stokes equation.

## How to Get Started with CMake

1. Open [CMakeLists.txt](CMakeLists.txt). Rename the project and the executable to whatever name you want. The project and executable names don't have to match.
1. If you want to add or remove any .cpp files, change the source files listed in the [`add_executable`](CMakeLists.txt#L10) call in CMakeLists.txt to match the source files your project requires. If you plan on keeping the default main.cpp file then no changes are required.
1. If you use Linux, install SFML's dependencies using your system package manager. On Ubuntu and other Debian-based distributions you can use the following commands:
   ```
   sudo apt update
   sudo apt install \
       libxrandr-dev \
       libxcursor-dev \
       libudev-dev \
       libfreetype-dev \
       libopenal-dev \
       libflac-dev \
       libvorbis-dev \
       libgl1-mesa-dev \
       libegl1-mesa-dev
   ```
1. Create vspkg.json for you machine, a template has been provied vspkg_template.json. Simply copy paste this template to the new file
1. Configure and build your project. Most popular IDEs support CMake projects with very little effort on your part.

   - [VS Code](https://code.visualstudio.com) via the [CMake extension](https://code.visualstudio.com/docs/cpp/cmake-linux)
   - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
   - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
   - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

   Using CMake from the command line is straightforward as well.

   For a single-configuration generator (typically the case on Linux and macOS):

   ```
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ```

   For a multi-configuration generator (typically the case on Windows):

   ```
   cmake -S . -B build
   cmake --build build --config Release
   ```
