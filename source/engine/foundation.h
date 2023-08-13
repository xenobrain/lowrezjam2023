#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>

auto static constexpr PI = 3.14159265358979323846f;
auto static constexpr DEG2RAD = PI / 180.f;
auto static constexpr RAD2DEG = 180.f / PI;


namespace xc {
    template<typename T, int N> struct vector_t;
    template<typename T> struct vector_t<T,2> { T x, y; };
    template<typename T> struct vector_t<T,3> { T x, y, z; };
    template<typename T> struct vector_t<T,4> { T x, y, z, w; };

    template<typename T, int M, int N> struct matrix_t;
    template<typename T, int M> struct matrix_t<T,M,2> { vector_t<T,M> x, y; };
    template<typename T, int M> struct matrix_t<T,M,3> { vector_t<T,M> x, y, z; };
    template<typename T, int M> struct matrix_t<T,M,4> { vector_t<T,M> x, y, z, w; };

    auto static constexpr op_add = [](auto a, auto b) { return a + b; };
    auto static constexpr op_sub = [](auto a, auto b) { return a - b; };
    auto static constexpr op_mul = [](auto a, auto b) { return a * b; };
    auto static constexpr op_div = [](auto a, auto b) { return a / b; };

    auto clamp(auto a, auto min, auto max) { return a < min ? min : a < max ? a : max; }

    template<typename T> auto normalize(vector_t<T,2> a) -> vector_t<T,2> {
        auto inv_length = 1.f / std::sqrt(a.x * a.x + a.y * a.y);
        return { a.x * inv_length, a.y * inv_length };
    }

    template<typename T> auto static constexpr zip(vector_t<T,2> a, T b, auto op) -> vector_t<T,2> { return {op(a.x, b), op(a.y, b)}; }
    template<typename T> auto static constexpr zip(vector_t<T,3> a, T b, auto op) -> vector_t<T,3> { return {op(a.x, b), op(a.y, b), op(a.z, b)}; }
    template<typename T> auto static constexpr zip(vector_t<T,4> a, T b, auto op) -> vector_t<T,4> { return {op(a.x, b), op(a.y, b), op(a.z, b), op(a.w, b)}; }
    template<typename T> auto static constexpr zip(vector_t<T,2> a, vector_t<T,2> b, auto op) -> vector_t<T,2> { return {op(a.x, b.x), op(a.y, b.y)}; }
    template<typename T> auto static constexpr zip(vector_t<T,3> a, vector_t<T,3> b, auto op) -> vector_t<T,3> { return {op(a.x, b.x), op(a.y, b.y), op(a.z, b.z)}; }
    template<typename T> auto static constexpr zip(vector_t<T,4> a, vector_t<T,4> b, auto op) -> vector_t<T,4> { return {op(a.x, b.x), op(a.y, b.y), op(a.z, b.z), op(a.w, b.w)}; }
    template<typename T, int M> auto static constexpr zip(matrix_t<T,M,2> a, matrix_t<T,M,2> b, auto op) -> matrix_t<T,M,2> { return { zip(a.x, b.x, op), zip(a.y, b.y, op)}; }
    template<typename T, int M> auto static constexpr zip(matrix_t<T,M,3> a, matrix_t<T,M,3> b, auto op) -> matrix_t<T,M,3> { return { zip(a.x, b.x, op), zip(a.y, b.y, op), zip(a.z, b.z, op)}; }
    template<typename T, int M> auto static constexpr zip(matrix_t<T,M,4> a, matrix_t<T,M,4> b, auto op) -> matrix_t<T,M,4> { return { zip(a.x, b.x, op), zip(a.y, b.y, op), zip(a.z, b.z, op), zip(a.w, b.w, op)}; }

    template<typename T> auto static constexpr operator-(vector_t<T,2> a) -> vector_t<T,2> { return {-a.x, -a.y}; }

    auto static constexpr operator+(auto a, auto b) { return zip(a, b, op_add); }
    auto static constexpr operator-(auto a, auto b) { return zip(a, b, op_sub); }

    auto static constexpr operator*(auto a, auto b) { return zip(a, b, op_mul); }
    template<typename T, int M> auto static constexpr operator*(matrix_t<T,M,2> a, vector_t<T,2> b) { return a.x * b.x + a.y * b.y; }
    template<typename T, int M> auto static constexpr operator*(matrix_t<T,M,3> a, vector_t<T,3> b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    template<typename T, int M> auto static constexpr operator*(matrix_t<T,M,4> a, vector_t<T,4> b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

    template<typename T>
    auto static constexpr operator*(matrix_t<T,3,3> a, matrix_t<T,3,3> b) -> matrix_t<T,3,3> {
        matrix_t<T,3,3> result;

        // First row
        result.x.x = a.x.x * b.x.x + a.x.y * b.y.x + a.x.z * b.z.x;
        result.x.y = a.x.x * b.x.y + a.x.y * b.y.y + a.x.z * b.z.y;
        result.x.z = a.x.x * b.x.z + a.x.y * b.y.z + a.x.z * b.z.z;

        // Second row
        result.y.x = a.y.x * b.x.x + a.y.y * b.y.x + a.y.z * b.z.x;
        result.y.y = a.y.x * b.x.y + a.y.y * b.y.y + a.y.z * b.z.y;
        result.y.z = a.y.x * b.x.z + a.y.y * b.y.z + a.y.z * b.z.z;

        // Third row
        result.z.x = a.z.x * b.x.x + a.z.y * b.y.x + a.z.z * b.z.x;
        result.z.y = a.z.x * b.x.y + a.z.y * b.y.y + a.z.z * b.z.y;
        result.z.z = a.z.x * b.x.z + a.z.y * b.y.z + a.z.z * b.z.z;

        return result;
    }

    auto static constexpr operator/(auto a, auto b) { return zip(a, b, op_div); }

    struct identity_t {
        template<typename T> constexpr operator matrix_t<T,2,2>() const { return  {{1,0}, {0,1}}; }
        template<typename T> constexpr operator matrix_t<T,3,3>() const { return  {{1,0,0}, {0,1,0}, {0,0,1}}; }
        template<typename T> constexpr operator matrix_t<T,4,4>() const { return  {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}}; }
    };

    template<typename T> auto translation_matrix(vector_t<T,2> translation) -> matrix_t<T,3,3> { return {{1,0,0},{0,1,0},{translation.x,translation.y,1}}; }

    template<typename T> auto translation_matrix(vector_t<T,3> translation) -> matrix_t<T,4,4> { return {{1,0,0,0},{0,1,0,0},{0,0,1,0},{translation.x, translation.y, translation.z,1}}; }

    template<typename T>
    auto scaling_matrix(vector_t<T,2> scaling) -> matrix_t<T,3,3> {
        return {
                {scaling.x, 0, 0},
                {0, scaling.y, 0},
                {0, 0, 1}
        };
    }

    //template<typename T> auto scaling_matrix(vector_t<T,3> scaling) -> matrix_t<T,4,4> { return {{scaling.x,0,0,0}, {0,scaling.y,0,0}, {0,0,scaling.z,0}, {0,0,0,1}}; }

    template<typename T>
auto rotation_matrix(T angle) -> matrix_t<T,3,3> {
        T cos_angle = static_cast<T>(std::cos(angle));
        T sin_angle = static_cast<T>(std::sin(angle));

    return {
        {cos_angle, -sin_angle, 0},
        {sin_angle, cos_angle, 0},
        {0, 0, 1}
    };
}

/*
    template<typename T> auto rotation_matrix(T angle) -> matrix_t<T, 4, 4> {
        T cos_angle = static_cast<T>(std::cos(angle));
        T sin_angle = static_cast<T>(std::sin(angle));

        return {
                        {cos_angle, -sin_angle, 0, 0},
                        {sin_angle, cos_angle, 0, 0},
                        {0, 0, 1, 0},
                        {0, 0, 0, 1}
                };
    }
    */

    template<typename T> auto static constexpr orthographic(T left, T right, T bottom, T top, T near, T far) -> matrix_t<T,4,4> {
        return {
                {T{2} / (right - left), 0.f, 0.f, 0.f},
                {0.f, T{2} / (top - bottom), 0.f, 0.f},
                {0.f, 0.f, -T{2} / (far - near), 0.f},
                {-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.f}
        };
    }


    using vector2_t = vector_t<float,2>;
    using vector3_t = vector_t<float,3>;
    using vector4_t = vector_t<float,4>;
    using matrix2_t = matrix_t<float,2,2>;
    using matrix3_t = matrix_t<float,3,3>;
    using matrix4_t = matrix_t<float,4,4>;


    struct color_t { float r, g, b, a; };
    struct rectangle_t { std::uint32_t x, y, w, h; };
} // namespace xc