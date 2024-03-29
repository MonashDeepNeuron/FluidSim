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
find_package(SFML COMPONENTS system window graphics CONFIGS REQUIRED)

find_package(fmt CONFIG REQUIRED)

# ---- Declare internal library ----
# add_library(libFluidSim OBJECT)

# target_include_directories(
# libFluidSim ${warning_guard}
# PUBLIC
# "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/headers>"
# )

# target_compile_features(libFluidSim PUBLIC cxx_std_23)

# target_link_libraries(libFluidSim PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

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
