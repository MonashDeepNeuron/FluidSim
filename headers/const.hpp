#pragma once

#include <cstddef>
#include <type_traits>

using std::size_t;
using s_size_t = std::make_signed_t<size_t>;

// Unsigned versions of programs base defaults
constexpr size_t AXIS_SIZE = 70;
constexpr size_t BUFFER_SIZE = (AXIS_SIZE + 2) * (AXIS_SIZE + 2);
constexpr size_t CELL_SIZE = 5;

// Signed versions of programs base defaults
constexpr s_size_t S_AXIS_SIZE = static_cast<s_size_t>(AXIS_SIZE);
constexpr s_size_t S_BUFFER_SIZE = static_cast<s_size_t>(BUFFER_SIZE);
constexpr s_size_t S_CELL_SIZE = static_cast<s_size_t>(CELL_SIZE);
