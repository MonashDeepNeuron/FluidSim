# Top level CMake project
cmake_minimum_required(VERSION 3.16)
# Consider enforcing a C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# Enforce out-of-source build, fail otherwise
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

# Make the default build type as Debug in single-configuration generators
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()


include(cmake/prelude.cmake)

project(
    FluidSim
    VERSION 0.1.0
    DESCRIPTION "Simulation of Fluid Dynamics"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using Gcc
  set(SFML_ROOT /opt/stow/SFML-2.6.1)


elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  # Use your flatform specific settings here.
  # Concurrent compile support (maybe /FS also?)
  add_compile_options(/MP)  
  set(SFML_ROOT c:/Users/erolc/OneDrive/Documents/GitHub/SFML)
  # end of MSVC
 
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang/LLVM
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
 else()
   message(WARNING "Unknown compiler!") 
 endif()


# ---- External libraries ----

# SFML project
# Please consult to https://www.sfml-dev.org/download.php

set(SFML_DIR ${SMFL_ROOT}/SFML/lib/cmake/SFML)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

# fmt Project: https://github.com/fmtlib/fmt
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        0041a40c1350ba702d475b9c4ad62da77caea164) # 11.0.0
FetchContent_MakeAvailable(fmt)

# Typical cmake commands on Windows
# cmake -S. -B bld_vc -G Ninja
# cmake --build bld_vc -v

# Typical cmake commands on Linux
# cmake -S. -B bld_gcc
# cmake --build bld_gcc -v


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