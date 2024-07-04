cmake_minimum_required(VERSION 3.16)

include(cmake/prelude.cmake)

project(
    FluidSim
    VERSION 0.1.0
    DESCRIPTION "Simulation of Fluid Dynamics"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- External libraries ----
set(fmt_DIR C:/Users/erolc/OneDrive/Documents/fmt/lib/cmake/fmt)
set(SFML_DIR C:/Users/erolc/OneDrive/Documents/GitHub/SFML/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# ---- Declare executable ----
add_executable(FluidSim src/main.cpp)
add_executable(FluidSim::exe ALIAS FluidSim)

target_include_directories(
    FluidSim ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/headers>"
)

set_property(TARGET FluidSim PROPERTY OUTPUT_NAME FluidSim)

target_compile_features(FluidSim PRIVATE cxx_std_23)

target_link_libraries(FluidSim PRIVATE fmt::fmt sfml-system sfml-network sfml-graphics sfml-window)

# ---- Ensure consistent runtime library settings ----
if (MSVC)
    # Set /MD for Release builds and /MDd for Debug builds
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

# ---- Install rules ----
if(NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----
if(NOT FluidSim_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(
        AUTHOR_WARNING
        "Developer mode is intended for developers of FluidSim"
    )
endif()

include(cmake/dev-mode.cmake)
