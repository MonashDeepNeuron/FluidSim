#pragma once

#include "const.hpp"
#include "utils.hpp"

#include <concepts>
#include <cstddef>
#include <type_traits>

constexpr auto IX(size_t i, size_t j) -> size_t { return (i + (AXIS_SIZE + 2) * j); }

template <std::unsigned_integral T>
constexpr auto sign_cast(T const value)
{
    return static_cast<std::make_signed_t<T>>(value);
}

template <std::signed_integral T>
constexpr auto sign_cast(T const value)
{
    return static_cast<std::make_unsigned_t<T>>(value);
}
