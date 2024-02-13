#pragma once

#include "aliases.hpp"

#include <algorithm>
#include <fmt/core.h>

#include <cstddef>
#include <limits>
#include <utility>

using std::swap;

// Define grid parameters

template <size_t BUFFER_SZ>
class DensitySolver {

public:
    DensitySolver()
        : m_diffuse_changer { 20 }
        , m_diff { 0.0f }
        , m_dt { 0.0f }
        , m_u {}
        , m_v {}
        , m_x0 {}
        , m_x {}
    {
        std::ranges::fill(m_u.container(), 0.01f);
        std::ranges::fill(m_v.container(), 0.01f);
        std::ranges::fill(m_x0.container(), 0.0f);
        std::ranges::fill(m_x.container(), 0.0f);
    }

    DensitySolver(float diff, float dt)
        : m_diffuse_changer { 20 }
        , m_diff { diff }
        , m_dt { dt }
        , m_u {}
        , m_v {}
        , m_x0 {}
        , m_x {}
    {
        std::ranges::fill(m_u.container(), 0.01f);
        std::ranges::fill(m_v.container(), 0.01f);
        std::ranges::fill(m_x0.container(), 0.0f);
        std::ranges::fill(m_x.container(), 0.0f);
    }

    auto x() -> matrix_span_t<float>
    {
        return m_x.to_mdspan();
    }

    auto x() const -> matrix_span_t<float const> const
    {
        return m_x.to_mdspan();
    }

    auto add_density(float d, size_t cidx, size_t ridx) -> size_t
    {
#if MDSPAN_USE_BRACKET_OPERATOR
        m_x[cidx, ridx] = d;
#else
        m_x(cidx, ridx) = d;
#endif
        // Remove _M_add_source from dens_step to here so display can properly see the
        // addition of the source before being diffused.

        return 0;
    }

    auto dens_step() -> void
    {
        // _M_add_source();
        swap(m_x0, m_x);
        _M_diffuse();
        // test_display();
        swap(m_x0, m_x);
        _M_advect();
    }

    auto test_display() -> void
    {
        fmt::println("Size {}", static_cast<int>(m_x.size()));

        for (auto i = 0uL; i < m_x.extent(0); ++i) {
            for (auto j = 0uL; j < m_x.extent(1); ++j) {
#if MDSPAN_USE_BRACKET_OPERATOR
                fmt::println("{}:{} ", m_x[i, j]);
#else
                fmt::println("{}:{} ", m_x(i, j));
#endif
            }

            fmt::print("\n");
        }

        fmt::print("\n\n");
    }

private:
    auto _M_add_source() -> void
    {
        std::ranges::transform(
            m_x.container(),
            m_x0.container(),
            m_x.container().begin(),
            [this](auto const& x, auto const& x0) {
                return std::ranges::clamp(
                    x + (m_dt * x0),
                    std::numeric_limits<float>::min(),
                    1.0f);
            });
    }

    auto _M_set_bnd([[maybe_unused]] int b) -> void
    {
        // Implementation of setting boundary conditions goes here
        // Replace with your specific implementation
    }

    auto _M_diffuse() -> void
    {
        float a = m_dt * m_diff * static_cast<float>(m_x.size()) - 4uL;

        for (auto k = 0uL; k < 20uL; k++) {
            for (auto i = 1uL; i < m_x.extent(0) - 1uL; i++) {
                for (auto j = 1uL; j < m_x.extent(1) - 1uL; j++) {
#if MDSPAN_USE_BRACKET_OPERATOR
                    m_x[i, j] = (m_x0[i, j] + a * (m_x[i, j - 1uL] + m_x[i, j + 1uL] + m_x[i - 1uL, j] + m_x[i + 1uL, j])) / (5 * a);
#else
                    m_x(i, j) = (m_x0(i, j) + a * (m_x(i, j - 1uL) + m_x(i, j + 1uL) + m_x(i - 1uL, j) + m_x(i + 1uL, j))) / (5 * a);
#endif
                }
            }

            _M_set_bnd(0);
        }
    }

    auto _M_advect() -> void
    {
        auto dt0 = m_dt * static_cast<float>(m_x.extent(0));

        for (auto i = 1uL; i < m_x.extent(0) - 2uL; i++) {
            for (auto j = 1uL; j < m_x.extent(1) - 2uL; j++) {
#if MDSPAN_USE_BRACKET_OPERATOR
                auto a = static_cast<float>(i) - dt0 * m_u[i, j];
                auto b = static_cast<float>(j) - dt0 * m_v[i, j];
#else
                auto a = static_cast<float>(i) - dt0 * m_u(i, j);
                auto b = static_cast<float>(j) - dt0 * m_v(i, j);
#endif
                // Clamp values to ensure they stay within bounds
                a = std::max(0.5f, std::min(static_cast<float>(AXIS_SIZE) + 0.5f, a));
                b = std::max(0.5f, std::min(static_cast<float>(AXIS_SIZE) + 0.5f, b));

                auto i0 = static_cast<size_t>(a);
                auto i1 = i0 + 1;
                auto j0 = static_cast<size_t>(b);
                auto j1 = j0 + 1;

                auto s1 = a - static_cast<float>(i0);
                auto s0 = 1 - s1;
                auto t1 = b - static_cast<float>(j0);
                auto t0 = 1 - t1;
#if MDSPAN_USE_BRACKET_OPERATOR
                m_x[i, j] = s0 * (t0 * m_x0[i0, j0] + t1 * m_x0[i0, j1]) + s1 * (t0 * m_x0[i1, j0] + t1 * m_x0[i1, j1]);
#else

                m_x(i, j) = s0 * (t0 * m_x0(i0, j0) + t1 * m_x0(i0, j1)) + s1 * (t0 * m_x0(i1, j0) + t1 * m_x0(i1, j1));
#endif
            }
        }

        _M_set_bnd(0);
    }

private:
    int m_diffuse_changer;

    float m_diff;
    float m_dt;

    matrix_t<float, 102> m_u;
    matrix_t<float, 102> m_v;
    matrix_t<float, 102> m_x0;

    // Final array to display
    matrix_t<float, 102> m_x;
};
