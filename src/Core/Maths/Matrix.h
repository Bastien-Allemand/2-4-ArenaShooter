#ifndef CORE_MATHS_MATRIX_H_INCLUDED
#define CORE_MATHS_MATRIX_H_INCLUDED

#include <xmmintrin.h>
#include "define.h"


namespace gce
{
    struct Quaternion;

    struct Matrix
    {
        Matrix() = default;
        Matrix(
            float32 m00, float32 m01, float32 m02, float32 m03,
            float32 m10, float32 m11, float32 m12, float32 m13,
            float32 m20, float32 m21, float32 m22, float32 m23,
            float32 m30, float32 m31, float32 m32, float32 m33);

        Matrix operator*(Matrix const& other);
        Matrix& operator*=(Matrix const& other);
        Matrix& operator=(Matrix const& other) = default;
        bool operator==(Matrix const& other);
        bool operator!=(Matrix const& other);
        float32* operator[](int32 index);
        
        float32 GetValue(int32 row, int32 col) const;
        void SetValue(int32 row, int32 col, float32 value);
        void Set(
            float32 m00, float32 m01, float32 m02, float32 m03,
            float32 m10, float32 m11, float32 m12, float32 m13,
            float32 m20, float32 m21, float32 m22, float32 m23,
            float32 m30, float32 m31, float32 m32, float32 m33);

        bool Inverse();
        void Transpose();
        bool IsDeterminantZero() const;
        bool Decompose(Vector3f32& translation, Vector3f32& scale, Quaternion& rotation) const;

        float32 GetDeterminant() const;
        Matrix GetTransposed() const;
        Matrix GetInverse() const;

        Matrix PerspectiveFovLH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ) const;
        Matrix OrthographicLH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) const;

        static Matrix Identity();
        static bool IsIdentity(Matrix const& m);

        static Matrix Translation(Vector3f32 const& translation);
        static Matrix Scale(Vector3f32 const& scale);
        static Matrix Rotation(Quaternion const& quat);
        
        static Matrix LookAtLH(Vector3f32 const& eyePosition, Vector3f32 const& focusPosition, Vector3f32 const& up);
        static Matrix LookToLH(Vector3f32 const& eyePosition, Vector3f32 const& eyeDirection, Vector3f32 const& up);
    private:
        float32 m_matrix[4][4] = {
            {1, 0, 0, 0}, 
            {0, 1, 0, 0}, 
            {0, 0, 1, 0}, 
            {0, 0, 0, 1}};
        Matrix(float32 matrix[4][4]);
        __vectorcall Matrix(__m128 matrix[4]);

        friend struct Quaternion;
    };


};

#endif
