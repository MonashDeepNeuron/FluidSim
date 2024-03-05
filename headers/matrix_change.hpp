#pragma once

#include "const.hpp"
#include "utils.hpp"

#include <fmt/core.h>

#include <array>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

using std::swap;

// Define grid parameters

template <size_t BUFFER_SZ>
class DensitySolver {
public:
    template <typename T>
    using array_t = std::array<T, BUFFER_SZ>;

public:
    DensitySolver()
        : m_diffuse_changer { 20 }
        , m_diff { 0.0f }
        , m_dt { 0.0f }
        , m_u { 0.0f }
        , m_v { 0.0f }
        , m_x0 { 0.0f }
        , m_x { 0.0f }
    {
    }

    DensitySolver(float diff, float dt)
        : m_diffuse_changer { 20 }
        , m_diff { diff }
        , m_dt { dt }
        , m_u { 0.01f }
        , m_v { 0.01f }
        , m_x0 { 0.0f }
        , m_x { 0.0f }
    {
    }

    auto x() -> array_t<float>&
    {
        return m_x;
    }

    auto x() const -> array_t<float> const&
    {
        return m_x;
    }

    auto add_density(float d, size_t index) -> size_t
    {
        m_x[index] = d;
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

        for (int i = 0; i < AXIS_SIZE + 2; ++i) {
            for (int j = 0; j < AXIS_SIZE + 2; ++j) {
                fmt::println("{}:{} ", m_x[IX(i, j)], IX(i, j));
            }

            fmt::print("\n");
        }

        fmt::print("\n\n");
    }

private:
    auto _M_add_source() -> void
    {
        for (auto i = 0uL; i < BUFFER_SZ; ++i) {
            m_x[i] += m_dt * m_x0[i];

            if (m_x[i] > 1) {
                m_x[i] = 1;
            }
        }
    }

  void _M_set_bnd(int b) {
    for (size_t i = 1; i <= AXIS_SIZE; i++) {
        m_x[IX(0, i)] = b == 1 ? -m_x[IX(1, i)] : m_x[IX(1, i)];
        m_x[IX(AXIS_SIZE + 1, i)] = b == 1 ? -m_x[IX(AXIS_SIZE, i)] : m_x[IX(AXIS_SIZE, i)];
        m_x[IX(i, 0)] = b == 2 ? -m_x[IX(i, 1)] : m_x[IX(i, 1)];
        m_x[IX(i, AXIS_SIZE + 1)] = b == 2 ? -m_x[IX(i, AXIS_SIZE)] : m_x[IX(i, AXIS_SIZE)];
    }
    m_x[IX(0, 0)] = 0.5f * (m_x[IX(1, 0)] + m_x[IX(0, 1)]);
    m_x[IX(0, AXIS_SIZE + 1)] = 0.5f * (m_x[IX(1, AXIS_SIZE + 1)] + m_x[IX(0, AXIS_SIZE)]);
    m_x[IX(AXIS_SIZE + 1, 0)] = 0.5f * (m_x[IX(AXIS_SIZE, 0)] + m_x[IX(AXIS_SIZE + 1, 1)]);
    m_x[IX(AXIS_SIZE + 1, AXIS_SIZE + 1)] = 0.5f * (m_x[IX(AXIS_SIZE, AXIS_SIZE + 1)] + m_x[IX(AXIS_SIZE + 1, AXIS_SIZE)]);
  }
    auto _M_diffuse() -> void
    {
        float a = m_dt * m_diff * AXIS_SIZE * AXIS_SIZE;

        for (auto k = 0uL; k < 20uL; k++) {
            for (auto i = 1uL; i <= AXIS_SIZE; i++) {
                for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                    auto index = i + (AXIS_SIZE + 2) * j;
                    m_x[index] = (m_x0[index] + a * (m_x[index - 1] + m_x[index + 1] + m_x[index - AXIS_SIZE - 2] + m_x[index + AXIS_SIZE + 2])) / (1 + 4 * a);
                }
            }

            _M_set_bnd(0);
        }
    }

    auto _M_advect() -> void
    {
        auto dt0 = m_dt * AXIS_SIZE;

        for (auto i = 1uL; i <= AXIS_SIZE; i++) {
            for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                auto a = static_cast<float>(i) - dt0 * m_u[IX(i, j)];
                auto b = static_cast<float>(j) - dt0 * m_v[IX(i, j)];

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

                m_x[IX(i, j)] = s0 * (t0 * m_x0[IX(i0, j0)] + t1 * m_x0[IX(i0, j1)]) + s1 * (t0 * m_x0[IX(i1, j0)] + t1 * m_x0[IX(i1, j1)]);
            }
        }

        _M_set_bnd(0);
    }

private:
    int m_diffuse_changer;

    float m_diff;
    float m_dt;

    array_t<float> m_u;
    array_t<float> m_v;
    array_t<float> m_x0;

    // Final array to display
    array_t<float> m_x;
};