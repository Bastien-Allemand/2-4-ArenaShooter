#ifndef CORE_MATHS_VECTOR2_HPP_INCLUDED
#define CORE_MATHS_VECTOR2_HPP_INCLUDED

#include <cmath>
#include "define.h"
#include "MathsFunctions.hpp"

namespace gce {


template <typename T>
union Vector2
{
    struct { T x, y; };
    [[nodiscard]] constexpr T& operator[]( uint8 const index ) { return m_data[index]; }
    [[nodiscard]] constexpr T const& operator[]( uint8 const index ) const { return m_data[index]; }

    constexpr Vector2() noexcept = default;
    ~Vector2() noexcept = default;

    constexpr Vector2( T const& x, T const& y ) noexcept: x(x), y(y) {}
    constexpr Vector2( Vector2 const& o ) noexcept = default;
    constexpr Vector2( Vector2&& o ) noexcept = default;

    constexpr Vector2& operator=( Vector2 const& o ) noexcept = default;
    constexpr Vector2& operator=( Vector2&& o ) noexcept = default;

    [[nodiscard]] constexpr T Norm() const noexcept { return sqrt( x * x + y * y ); } // Sqrt from MathFunctions won't work
    [[nodiscard]] constexpr T SquareNorm() const noexcept { return x * x + y * y; }
    [[nodiscard]] constexpr T DotProduct( Vector2 const& o ) const noexcept { return x * o.x + y * o.y; }

    [[nodiscard]] Vector2 Normalize() const { T const norm = Norm(); if ( norm == T{} ) return {}; return { x / norm, y / norm }; }
    Vector2& SelfNormalize() { T const norm = Norm(); if ( norm == T{} ) return *this; x /= norm; y /= norm; return *this; }

    constexpr Vector2& operator+=( Vector2 const& o ) noexcept { x += o.x; y += o.y; return *this; }
    constexpr Vector2& operator-=( Vector2 const& o ) noexcept { x -= o.x; y -= o.y; return *this; }
    constexpr Vector2& operator*=( Vector2 const& o ) noexcept { x *= o.x; y *= o.y; return *this; }
    constexpr Vector2& operator/=( Vector2 const& o )          { x /= o.x; y /= o.y; return *this; }
    constexpr Vector2& operator*=( T const scalar ) noexcept { x *= scalar; y *= scalar; return *this; }
    constexpr Vector2& operator/=( T const scalar )          { x /= scalar; y /= scalar; return *this; }

    [[nodiscard]] constexpr Vector2 operator+( Vector2 const& o ) const noexcept { return { x + o.x, y + o.y }; }
    [[nodiscard]] constexpr Vector2 operator-( Vector2 const& o ) const noexcept { return { x - o.x, y - o.y }; }
    [[nodiscard]] constexpr Vector2 operator*( Vector2 const& o ) const noexcept { return { x * o.x, y * o.y }; }
    [[nodiscard]] constexpr Vector2 operator/( Vector2 const& o ) const          { return { x / o.x, y / o.y }; }
    [[nodiscard]] constexpr Vector2 operator*( T const scalar ) const noexcept { return { x * scalar, y * scalar }; }
    [[nodiscard]] constexpr Vector2 operator/( T const scalar ) const          { return { x / scalar, y / scalar }; }
    [[nodiscard]] constexpr Vector2 operator-() const noexcept { return { -x, -y }; }

    [[nodiscard]] constexpr bool operator==( Vector2 const& o ) const noexcept { return &o == this || ( x == o.x && y == o.y ); }
    [[nodiscard]] constexpr bool operator!=( Vector2 const& o ) const noexcept { return &o != this && ( x != o.x || y != o.y ); }

    template <typename T2>
    [[nodiscard]] constexpr operator Vector2<T2>() const { return { static_cast<T2>(x), static_cast<T2>(y) }; }
    [[nodiscard]] constexpr operator bool() const noexcept { return x != T{} || y != T{}; }

private:
    T m_data[2];
};


template <>
inline [[nodiscard]] Vector2<float32> Vector2<float32>::Normalize() const { float32 const norm = Norm(); if ( IsZero( norm ) ) return { 0.0f, 0.0f }; return { x / norm, y / norm }; }

template <>
inline Vector2<float32>& Vector2<float32>::SelfNormalize() { float32 const norm = Norm(); if ( IsZero( norm ) ) return *this; x /= norm; y /= norm; return *this; }


}

#endif