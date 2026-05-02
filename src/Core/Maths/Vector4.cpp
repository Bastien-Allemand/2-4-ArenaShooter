#include "Vector4.h"
#include <intrin.h>
#include <iostream>


#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)
#define VecPermute(vec, x,y,z,w)		  VecShuffle(vec, vec, x,y,z,w)

namespace gce
{
	///////////////////////////////////////////////////////////////////////
	/// @brief Vector class with 4 elements.
	/// @param x: The first value of this vector.
	/// @param y: The second value of this vector.
	/// @param y: The third value of this vector.
	/// @param w: The fourth value of this vector.
	/// @return A Vector4 object initialized with the specified values.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////
	Vector4::Vector4(float32 x, float32 y, float32 z, float32 w)
	{
		_mm_store_ps(m_data, _mm_set_ps(w, z, y, x));
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Vector class with 4 elements.
	/// @param other: The vector to copy.
	/// @return A Vector4 object initialized with copied values from other.
	///////////////////////////////////////////////////////////////////////////
	Vector4::Vector4(Vector4 const& other)
	{
		*this = other;
	}

	///////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 assignement.
	/// @param other: The vector to copy.
	/// @return A reference to this vector.
	///////////////////////////////////////////////////////////
	Vector4& Vector4::operator=(Vector4 const& other)
	{
		memcpy(m_data, other.m_data, 4 * sizeof(float32));
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 addition.
	/// @param other: The vector to add.
	/// @return The result of the addition. (x1 + x2, y1 + y1, z1 + z2, w1 + w2)
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////////////////
	Vector4 Vector4::operator+(Vector4 const& other)
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 v2 = _mm_load_ps(other.m_data);

		return Vector4(_mm_add_ps(v1, v2));
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 assignement addition.
	/// @param other: The vector to add.
	/// @return The reference to the result of the addition. (x1 + x2, y1 + y1, z1 + z2, w1 + w2)
	/// @note This function uses SIMD instructions.
	/////////////////////////////////////////////////////////////////////////////////////////////////
	Vector4& Vector4::operator+=(Vector4 const& other)
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 v2 = _mm_load_ps(other.m_data);

		*this = _mm_add_ps(v1, v2);
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 substraction.
	/// @param other: The vector to substract.
	/// @return The result of the substraction. (x1 - x2, y1 - y1, z1 - z2, w1 - w2)
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////////////////////
	Vector4 Vector4::operator-(Vector4 const& other)
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 v2 = _mm_load_ps(other.m_data);

		return Vector4(_mm_sub_ps(v1, v2));
	}

	////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 division.
	/// @param scalar: The scalar right to the operation.
	/// @return The result of the division. (x/scalar, y/scalar, z/scalar, w/scalar)
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////////////////////
	Vector4 Vector4::operator/(float32 scalar) const
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 scalarV = _mm_set1_ps(scalar);

		return _mm_div_ps(v1, scalarV);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 assignement substraction.
	/// @param other: The vector right to the operation.
	/// @return The reference to the result of the substraction. (x1 - x2, y1 - y1, z1 - z2, w1 - w2)
	/// @note This function uses SIMD instructions.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	Vector4& Vector4::operator-=(Vector4 const& other)
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 v2 = _mm_load_ps(other.m_data);

		*this = _mm_sub_ps(v1, v2);
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 assignement multiplication.
	/// @param scalar: The scalar to multiply with.
	/// @return The reference to the result of the multiplication. (x * scalar, y * scalar, z * scalar, w * scalar)
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Vector4& Vector4::operator*=(float32 scalar)
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 scalarV = _mm_set1_ps(scalar);

		*this = _mm_mul_ps(v1, scalarV);
		return *this;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 assignement division.
	/// @param scalar: The scalar to divide with.
	/// @return The reference to the result of the division. (x/scalar, y/scalar, z/scalar, w/scalar)
	/// @note This function uses SIMD instructions.
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	Vector4& Vector4::operator/=(float32 scalar)
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 scalarV = _mm_set1_ps(scalar);

		*this = _mm_div_ps(v1, scalarV);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 multiplication.
	/// @param scalar: The scalar to multiply with.
	/// @return The result of the multiplication. (x * scalar, y * scalar, z * scalar, w * scalar)
	/// @note This function uses SIMD instructions.
	//////////////////////////////////////////////////////////////////////////////////////////////////
	Vector4 Vector4::operator*(float32 scalar) const
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 scalarV = _mm_set1_ps(scalar);

		return _mm_mul_ps(v1, scalarV);
	}

	//////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 sign inversion.
	/// @return The result of the inversion. (-x, -y, -z, -w)
	/// @note This function uses SIMD instructions.
	//////////////////////////////////////////////////////////////
	Vector4 Vector4::operator-() const
	{
		__m128 v1 = _mm_load_ps(m_data);
		__m128 scalarV = _mm_set1_ps(-1);

		return _mm_mul_ps(v1, scalarV);
	}

	////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 indexing.
	/// @param index: The index of the value to access.
	/// @return The value at the given index.
	////////////////////////////////////////////////////////
	float32& Vector4::operator[](int index)
	{
		return m_data[index];
	}

	/////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector4 comparison.
	/// @param other: The vector to compare with.
	/// @return True if both vectors have the same values, false otherwise.
	/// @note This function uses SIMD instructions.
	/////////////////////////////////////////////////////////////////////////////
	bool Vector4::operator==(Vector4 const& other) const
	{
		return _mm_movemask_ps(_mm_cmpeq_ps(_mm_load_ps(m_data), _mm_load_ps(other.m_data))) == 0x0f;
	}

	///////////////////////////////////////////////
	/// @brief Gets the x value of this vector.
	/// @return The x value. (vector[0])
	///////////////////////////////////////////////
	float32& Vector4::x()
	{
		return m_data[0];
	}

	///////////////////////////////////////////////
	/// @brief Gets the y value of this vector.
	/// @return The y value. (vector[1])
	///////////////////////////////////////////////
	float32& Vector4::y()
	{
		return m_data[1];
	}

	///////////////////////////////////////////////
	/// @brief Gets the z value of this vector.
	/// @return The z value. (vector[2])
	///////////////////////////////////////////////
	float32& Vector4::z()
	{
		return m_data[2];
	}

	///////////////////////////////////////////////
	/// @brief Gets the w value of this vector.
	/// @return The w value. (vector[3])
	///////////////////////////////////////////////
	float32& Vector4::w()
	{
		return m_data[3];
	}

	////////////////////////////////////////////////////////
	/// @brief Gets the x value of this vector, as const
	/// @return The x value. (vector[0])
	////////////////////////////////////////////////////////
	float32 const& Vector4::GetX() const
	{
		return m_data[0];
	}

	////////////////////////////////////////////////////////
	/// @brief Gets the y value of this vector, as const
	/// @return The y value. (vector[1])
	////////////////////////////////////////////////////////
	float32 const& Vector4::GetY() const
	{
		return m_data[1];
	}

	////////////////////////////////////////////////////////
	/// @brief Gets the z value of this vector, as const
	/// @return The z value. (vector[2])
	////////////////////////////////////////////////////////
	float32 const& Vector4::GetZ() const
	{
		return m_data[2];
	}

	////////////////////////////////////////////////////////
	/// @brief Gets the w value of this vector, as const
	/// @return The w value. (vector[3])
	////////////////////////////////////////////////////////
	float32 const& Vector4::GetW() const
	{
		return m_data[3];
	}

	///////////////////////////////////////////////////////////////////////
	/// @brief Normalizes this vector. (x/norm, y/norm, z/norm, w/norm)
	/// @return The reference to this normalized vector.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////
	Vector4& Vector4::Normalize()
	{
		__m128 v = _mm_load_ps(m_data);

		// Perform the dot product on x,y,z and w
		__m128 vLengthSq = _mm_mul_ps(v, v);
		// vTemp has z and w
		__m128 vTemp = VecPermute(vLengthSq, 2, 3, 2, 3);
		// x+z, y+w
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// x+z,x+z,x+z,y+w
		vLengthSq = VecPermute(vLengthSq, 0, 0, 0, 1);
		// ??,??,y+w,y+w
		vTemp = _mm_shuffle_ps(vTemp, vLengthSq, MakeShuffleMask(0, 0, 3, 3));
		// ??,??,x+z+y+w,??
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// Splat the length
		vLengthSq = VecPermute(vLengthSq, 2, 2, 2, 2);
		// Prepare for the division
		__m128 vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		__m128 vZeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		__m128 infinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
		vLengthSq = _mm_cmpneq_ps(vLengthSq, infinity);
		// Divide to perform the normalization
		vResult = _mm_div_ps(v, vResult);
		// Any that are infinity, set to zero
		vResult = _mm_and_ps(vResult, vZeroMask);
		// Select qnan or result based on infinite length
		__m128 qNaN = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, qNaN);
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);

		_mm_store_ps(m_data, vResult);

		return *this;
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Gets this vector normalized. (x/norm, y/norm, z/norm, w/norm)
	/// @return The normalized vector.
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////////////
	Vector4 Vector4::GetNormalized() const
	{
		__m128 v = _mm_load_ps(m_data);

		// Perform the dot product on x,y,z and w
		__m128 vLengthSq = _mm_mul_ps(v, v);
		// vTemp has z and w
		__m128 vTemp = VecPermute(vLengthSq, 2, 3, 2, 3);
		// x+z, y+w
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// x+z,x+z,x+z,y+w
		vLengthSq = VecPermute(vLengthSq, 0, 0, 0, 1);
		// ??,??,y+w,y+w
		vTemp = _mm_shuffle_ps(vTemp, vLengthSq, MakeShuffleMask(0, 0, 3, 3));
		// ??,??,x+z+y+w,??
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// Splat the length
		vLengthSq = VecPermute(vLengthSq, 2, 2, 2, 2);
		// Prepare for the division
		__m128 vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		__m128 vZeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		__m128 infinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
		vLengthSq = _mm_cmpneq_ps(vLengthSq, infinity);
		// Divide to perform the normalization
		vResult = _mm_div_ps(v, vResult);
		// Any that are infinity, set to zero
		vResult = _mm_and_ps(vResult, vZeroMask);
		// Select qnan or result based on infinite length
		__m128 qNaN = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, qNaN);
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);
		return vResult;
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Gets this vector's norm.
	/// @return The vector's norm.
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////////////
	float32 Vector4::GetNorm() const
	{
		__m128 v = _mm_load_ps(m_data);

		// (x^2, y^2, z^2, w^2)
		__m128 sq = _mm_mul_ps(v, v);

		// (x^2 + y^2, z^2 + w^2, x^2 + y^2, z^2 + w^2)
		sq = _mm_hadd_ps(sq, sq);

		// Every element is x^ + y^2 + z^2 + w^2
		sq = _mm_hadd_ps(sq, sq);
		sq = _mm_sqrt_ps(sq);

		return sq.m128_f32[0];
	}

	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets this vector's norm squared.
	/// @return The vector's norm squared.
	/// @note This function uses SIMD instructions. Faster than GetNorm().
	//////////////////////////////////////////////////////////////////////////
	float32 Vector4::GetNormSq() const
	{
		__m128 v = _mm_load_ps(m_data);

		// (x^2, y^2, z^2, w^2)
		__m128 sq = _mm_mul_ps(v, v);

		// (x^2 + y^2, z^2 + w^2, x^2 + y^2, z^2 + w^2)
		sq = _mm_hadd_ps(sq, sq);

		// Every element is x^2 + y^2 + z^2 + w^2
		float32 normSq = _mm_hadd_ps(sq, sq).m128_f32[0];
		return normSq;
	}

	///////////////////////////////////////////////////////////////////////
	/// @brief Computes the linear interpolation of a and b by t.
	/// @param a: The vector left of the operation.
	/// @param b: The vector right of the operation.
	/// @param t: The value that linearly interpolates between a and b.
	/// @return The result of the linear interpolation.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////
	Vector4 Vector4::Lerp(Vector4 const& a, Vector4 const& b, float32 t)
	{
		__m128 v1 = _mm_load_ps(a.m_data);
		__m128 v2 = _mm_load_ps(b.m_data);

		__m128 l = _mm_sub_ps(v2, v1);
		__m128 s = _mm_set_ps1(t);
		return _mm_add_ps(_mm_mul_ps(l, s), v1);
	}

	///////////////////////////////////////////////////
	/// @brief Computes the dot product of a and b.
	/// @param a: The vector left of the operation.
	/// @param b: The vector right of the operation.
	/// @return The dot product of a and b.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////
	float32 Vector4::Dot(Vector4 const& a, Vector4 const& b)
	{
		__m128 v1 = _mm_load_ps(a.m_data);
		__m128 v2 = _mm_load_ps(b.m_data);

		__m128 vTemp2 = v2;
		__m128 vTemp = _mm_mul_ps(v1, vTemp2);
		vTemp2 = VecShuffle(vTemp2, vTemp, 0, 0, 0, 1); // Copy X to the Z position and Y to the W position
		vTemp2 = _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
		vTemp = VecShuffle(vTemp, vTemp2, 0, 0, 3, 0);  // Copy W to the Z position
		vTemp = _mm_add_ps(vTemp, vTemp2);           // Add Z and W together
		return vTemp.m128_f32[2];    // return Z
	}

	__vectorcall Vector4::Vector4(__m128 v)
	{
		_mm_store_ps(m_data, v);
	}

	gce::Vector4 Vector4Matrix(gce::Matrix _m, gce::Vector4 _v)
	{
		float x = _m[0][0] * _v.GetX() + _m[0][1] * _v.GetY() + _m[0][2] * _v.GetZ() + _m[0][3] * _v.GetW();
		float y = _m[1][0] * _v.GetX() + _m[1][1] * _v.GetY() + _m[1][2] * _v.GetZ() + _m[1][3] * _v.GetW();
		float z = _m[2][0] * _v.GetX() + _m[2][1] * _v.GetY() + _m[2][2] * _v.GetZ() + _m[2][3] * _v.GetW();
		float w = _m[3][0] * _v.GetX() + _m[3][1] * _v.GetY() + _m[3][2] * _v.GetZ() + _m[3][3] * _v.GetW();

		return gce::Vector4(x, y, z, w);
	}
}

