#ifndef CORE_MATHS_QUATERNION_H_INCLUDED
#define CORE_MATHS_QUATERNION_H_INCLUDED

#include <xmmintrin.h>
#include "define.h"


namespace gce
{
	struct Matrix;

	struct Quaternion
	{
		Quaternion();
		Quaternion(float32 x, float32  y, float32 z, float32 w);

		Quaternion operator+(float32 right) const;
		Quaternion operator+(Quaternion const& right) const;
		Quaternion operator-(Quaternion const& right) const;
		
		Quaternion operator*(Quaternion const& o);
		Quaternion* operator=(Quaternion const& o);
		Quaternion* operator*=(Quaternion const& o);

		bool operator==(Quaternion const& o) const;
		bool operator!=(Quaternion const& o) const;

		[[nodiscard]] Quaternion GetConjugate() const;
		[[nodiscard]] Quaternion GetInverse() const;

		bool Inverse();
		bool Conjugate();
		bool Normalize();

		bool IsIdentity() const;
		bool IsNormZero() const;

		[[nodiscard]] float32 Norm() const;
		[[nodiscard]] float32 SquareNorm() const;
		[[nodiscard]] Quaternion Normalized() const;

		void SetIdentity();
		void SetRotationAxis(Vector3f32 const& axis, float32 const& angle);
		void SetRotationMatrix(Matrix const& mat);
		void SetRotationEuler(float32 angleX, float32 angleY, float32 angleZ);
		void SetRotationEuler(Vector3f32 const& angles);

		void QuaternionToAxisAngle(Vector3f32& axis, float& angle);

		[[nodiscard]] static Quaternion RotationMatrix(Matrix const& mat);
		[[nodiscard]] static Quaternion RotationAxis(Vector3f32 const& axis, float32 const& angle);
		[[nodiscard]] static Quaternion RotationEuler(float32 angleX, float32 angleY, float32 angleZ);
		[[nodiscard]] static Quaternion RotationEuler(Vector3f32 const& angles);

		float32 GetX() const;
		float32 GetY() const;
		float32 GetZ() const;
		float32 GetW() const;

	private:
		float32 pdata[4];
		Quaternion(float32 values[4]);

		static void __vectorcall CosSin(__m128& cos, __m128& sin, __m128 angles);

		static __m128 __vectorcall Permute(__m128 V1, __m128 V2, uint16 PermuteX, uint16 PermuteY, uint16 PermuteZ, uint16 PermuteW);

		friend struct Matrix;
	};
}

#endif