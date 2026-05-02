#ifndef CORE_MATHS_VECTOR4_HPP_INCLUDED
#define CORE_MATHS_VECTOR4_HPP_INCLUDED

#include "define.h"
#include "Matrix.h"

union __m128;

namespace gce
{
    ////////////////////////////////////////////
    /// @brief Vector class with 4 elements.
    ////////////////////////////////////////////
    struct Vector4 final
    {
    public:
        [[nodiscard]] float32& x();
        [[nodiscard]] float32& y();
        [[nodiscard]] float32& z();
        [[nodiscard]] float32& w();

        [[nodiscard]] float32 const& GetX() const;
        [[nodiscard]] float32 const& GetY() const;
        [[nodiscard]] float32 const& GetZ() const;
        [[nodiscard]] float32 const& GetW() const;

        Vector4() = default;
        Vector4(float32 x, float32 y, float32 z, float32 w);
        Vector4(Vector4 const&);
        Vector4& operator=(Vector4 const& other);

        [[nodiscard]] Vector4 operator+(Vector4 const& other);
        [[nodiscard]] Vector4 operator-(Vector4 const& other);
        [[nodiscard]] Vector4 operator/(float32 scalar) const;
        [[nodiscard]] Vector4 operator*(float32 scalar) const;
        
        Vector4& operator+=(Vector4 const& other);
        Vector4& operator-=(Vector4 const& other);
        Vector4& operator*=(float32 scalar);
        Vector4& operator/=(float32 scalar);

        Vector4 operator-() const;
        float32& operator[](int index);
        bool operator==(Vector4 const& other) const;

        Vector4& Normalize();
        [[nodiscard]] Vector4 GetNormalized() const;

        [[nodiscard]] float32 GetNorm() const;
        [[nodiscard]] float32 GetNormSq() const;

        static Vector4 Lerp(Vector4 const& a, Vector4 const& b, float32 interpolater);
        static float32 Dot(Vector4 const& a, Vector4 const& b);
        Vector4 Vector4Matrix(gce::Matrix _m, gce::Vector4 _v);

    private:
        __vectorcall Vector4(__m128 v);
        float32 m_data[4];
    };


}

#endif
