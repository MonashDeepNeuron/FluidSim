#pragma once

#include "const.hpp"
#include "utils.hpp"

#include <fmt/core.h>

#include <array>
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
    // DensitySolver()
    //     : m_diffuse_changer { 20 }
    //     , m_diff { 0.0f }
    //     , m_dt { 0.0f }
    //     , m_u { 0.51f }
    //     , m_v { 0.0f }
    //     , m_x0 { 0.0f }
    //     , m_x { 0.0f }
    //     , m_div { 0.0f }
    //     , m_p { 0.0f }
    // {
    // }

    DensitySolver(float diff, float dt, float visc, array_t<float> u_arr, array_t<float> v_arr)
        : m_diffuse_changer { 20 }
        , m_diff { diff }
        , m_dt { dt }
        , m_visc { visc }
        , m_u { u_arr }
        , m_v { v_arr }
        , m_v0 { 0.0f }
        , m_u0 { 0.0f }
        , m_x0 { 0.0f }
        , m_div { 0.0f }
        , m_p { 0.0f }
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

    auto u() -> array_t<float>&
    {
        return m_u;
    }

    auto u() const -> array_t<float> const&
    {
        return m_u;
    }

    auto v() -> array_t<float>&
    {
        return m_v;
    }

    auto v() const -> array_t<float> const&
    {
        return m_v;
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

    auto velocity_step() -> void
    {
        swap(m_u0, m_u);
        swap(m_v0, m_v);

        _M_u_diffuse();
        _M_v_diffuse();

        swap(m_u0, m_u);
        swap(m_v0, m_v);

        _M_v_advect();
        _M_u_advect();

        _M_project();
    }

    auto test_display() -> void
    {
        // fmt::println("Size {}", static_cast<int>(m_x.size()));

        for (size_t i = 0; i < AXIS_SIZE + 2; ++i) {
            for (size_t j = 0; j < AXIS_SIZE + 2; ++j) {
                fmt::println("{}:{} ", m_x[IX(i, j)], IX(i, j));
            }

            fmt::print("\n");
        }

        // fmt::print("\n\n");
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

    void _M_set_bnd(float b, array_t<float>& arr)
    {
        for (size_t i = 0; i <= AXIS_SIZE + 1; i++) {
            arr[IX(i, 0)] = b;
            arr[IX(i, AXIS_SIZE + 1)] = -b;
        }

        // Set the first and last column to 0
        for (size_t j = 0; j <= AXIS_SIZE + 1; j++) {
            arr[IX(0, j)] = b;
            arr[IX(AXIS_SIZE + 1, j)] = -b;
        }

        // Set the corners to 0
        arr[IX(0, 0)] = 0;
        arr[IX(0, AXIS_SIZE + 1)] = 0;
        arr[IX(AXIS_SIZE + 1, 0)] = 0;
        arr[IX(AXIS_SIZE + 1, AXIS_SIZE + 1)] = 0;
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

            _M_set_bnd(0, m_x);
        }
    }

    auto _M_u_diffuse() -> void
    {
        float a = m_dt * m_visc * AXIS_SIZE * AXIS_SIZE;

        for (auto k = 0uL; k < 20uL; k++) {
            for (auto i = 1uL; i <= AXIS_SIZE; i++) {
                for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                    auto index = i + (AXIS_SIZE + 2) * j;
                    m_u[index] = (m_u0[index] + a * (m_u[index - 1] + m_u[index + 1] + m_u[index - AXIS_SIZE - 2] + m_u[index + AXIS_SIZE + 2])) / (1 + 4 * a);
                }
            }

            _M_set_bnd(0, m_u);
        }
    }

    auto _M_v_diffuse() -> void
    {
        float a = m_dt * m_visc * AXIS_SIZE * AXIS_SIZE;

        for (auto k = 0uL; k < 20uL; k++) {
            for (auto i = 1uL; i <= AXIS_SIZE; i++) {
                for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                    auto index = i + (AXIS_SIZE + 2) * j;
                    m_v[index] = (m_v0[index] + a * (m_v[index - 1] + m_v[index + 1] + m_v[index - AXIS_SIZE - 2] + m_v[index + AXIS_SIZE + 2])) / (1 + 4 * a);
                }
            }

            _M_set_bnd(0, m_v);
        }
    }

    auto _M_advect() -> void
    {
        auto dt0 = m_dt * AXIS_SIZE;

        for (auto i = 0uL; i <= AXIS_SIZE + 1; i++) {

            for (auto j = 0uL; j <= AXIS_SIZE + 1; j++) {
                if (i == 0 || j == 0 || i == AXIS_SIZE + 1 || j == AXIS_SIZE + 1) {
                    // fmt::print("m_v[IX({}, {})]: {}; ", i, j, m_v[IX(i, j)]);
                    // fmt::print("m_u[IX({}, {})]: {}", i, j, m_u[IX(i, j)]);
                }
            }
            // fmt::println("");
        }

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
            // fmt::println("");
        }

        _M_set_bnd(0, m_x);
    }

    auto _M_u_advect() -> void
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

                m_u[IX(i, j)] = s0 * (t0 * m_u0[IX(i0, j0)] + t1 * m_u0[IX(i0, j1)]) + s1 * (t0 * m_u0[IX(i1, j0)] + t1 * m_u0[IX(i1, j1)]);
            }
        }

        _M_set_bnd(0, m_u);
    }

    auto _M_v_advect() -> void
    {
        auto dt0 = m_dt * AXIS_SIZE;

        for (auto i = 1uL; i <= AXIS_SIZE; i++) {
            for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                auto a = static_cast<float>(i) - dt0 * m_u[IX(i, j)];
                auto b = static_cast<float>(j) - dt0 * m_v[IX(i, j)];

                // fmt::print("m_v[IX({}, {})] {}",i, j, m_v[IX(i, j)]);

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

                m_v[IX(i, j)] = s0 * (t0 * m_v0[IX(i0, j0)] + t1 * m_v0[IX(i0, j1)]) + s1 * (t0 * m_v0[IX(i1, j0)] + t1 * m_v0[IX(i1, j1)]);
            }
            // fmt::println("");
        }

        _M_set_bnd(0, m_v);
    }

    auto _M_project() -> void
    {
        auto const h = 1.0f / AXIS_SIZE;
        for (auto i = 1uL; i <= AXIS_SIZE; i++) {
            for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                m_div[IX(i, j)] = static_cast<float>(-0.5f * h * (m_u[IX(i + 1, j)] - m_u[IX(i - 1, j)] + m_v[IX(i, j + 1)] - m_v[IX(i, j - 1)]));
                m_p[IX(i, j)] = 0;
            }
        }

        _M_set_bnd(0, m_div);
        _M_set_bnd(0, m_p);

        for (auto k = 0; k < 20; k++) {
            for (auto i = 1uL; i <= AXIS_SIZE; i++) {
                for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                    m_p[IX(i, j)] = static_cast<float>((m_div[IX(i, j)] + m_p[IX(i - 1, j)] + m_p[IX(i + 1, j)] + m_p[IX(i, j - 1)] + m_p[IX(i, j + 1)]) / 4);
                }
            }
            _M_set_bnd(0, m_p);
        }

        for (auto i = 1uL; i <= AXIS_SIZE; i++) {
            for (auto j = 1uL; j <= AXIS_SIZE; j++) {
                m_u[IX(i, j)] -= static_cast<float>(0.5f * (m_p[IX(i + 1, j)] - m_p[IX(i - 1, j)]) / h);
                m_v[IX(i, j)] -= static_cast<float>(0.5f * (m_p[IX(i, j + 1)] - m_p[IX(i, j - 1)]) / h);
            }
        }

        _M_set_bnd(0, m_u);
        _M_set_bnd(0, m_v);
    }

private:
    int m_diffuse_changer;

    float m_diff;
    float m_dt;
    float m_visc;

    array_t<float> m_u;
    array_t<float> m_v;
    array_t<float> m_v0;
    array_t<float> m_u0;
    array_t<float> m_x0;
    array_t<float> m_div;
    array_t<float> m_p;

    // Final array to display
    array_t<float> m_x;
};