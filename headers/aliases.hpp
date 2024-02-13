#pragma once

#include <array>
#include <cstddef>
#include <experimental/__p0009_bits/extents.hpp>
#include <experimental/__p0009_bits/layout_stride.hpp>
#include <experimental/__p0009_bits/mdspan.hpp>
#include <experimental/__p1684_bits/mdarray.hpp>
#include <type_traits>

using std::size_t;
using s_size_t = std::make_signed_t<size_t>;

using std::experimental::default_accessor;
using std::experimental::dextents;
using std::experimental::extents;
using std::experimental::layout_right;
using std::experimental::mdarray;
using std::experimental::mdspan;

/**
 * Matrix type with equal fixed axis sizes
 */
template <typename T, size_t AxisLen, typename Layout = layout_right>
using matrix_t = mdarray<
    T,
    extents<size_t, AxisLen, AxisLen>,
    Layout,
    std::array<T, AxisLen * AxisLen>>;

template <
    typename T,
    typename Layout = layout_right,
    typename Accessor = default_accessor<T>>
using matrix_span_t = mdspan<T, dextents<size_t, 2>, Layout, Accessor>;

// Unsigned versions of programs base defaults
constexpr size_t AXIS_SIZE = 100;
constexpr size_t BUFFER_SIZE = (AXIS_SIZE + 2) * (AXIS_SIZE + 2);
constexpr size_t CELL_SIZE = 5;

// Signed versions of programs base defaults
constexpr s_size_t S_AXIS_SIZE = static_cast<s_size_t>(AXIS_SIZE);
constexpr s_size_t S_BUFFER_SIZE = static_cast<s_size_t>(BUFFER_SIZE);
constexpr s_size_t S_CELL_SIZE = static_cast<s_size_t>(CELL_SIZE);
