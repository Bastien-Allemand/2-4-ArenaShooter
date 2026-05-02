#include "Vector3.h"
#include <emmintrin.h>

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
	/// @brief Vector class with 3 elements.
	/// @param x: The first value of this vector.
	/// @param y: The second value of this vector.
	/// @param y: The third value of this vector.
	/// @return A Vector3 object initialized with the specified values.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////
	Vector3f32::Vector3f32(float32 x, float32 y, float32 z)
	{
		Store(_mm_set_ps(0, z, y, x));
	}

	///////////////////////////////////////////////////////////////////////
	/// @brief Vector class with 3 elements.
	/// @param values: The 3 values stored in order.
	/// @return A Vector3 object initialized with the specified values.
	///////////////////////////////////////////////////////////////////////
	Vector3f32::Vector3f32(float32 values[3])
	{
		memcpy(m_pData, values, 3 * sizeof(float32));
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Vector class with 3 elements.
	/// @param other: The vector to copy.
	/// @return A Vector3 object initialized with copied values from other.
	///////////////////////////////////////////////////////////////////////////
	Vector3f32::Vector3f32(Vector3f32 const& other)
	{
		*this = other;
	}

	///////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 assignement.
	/// @param other: The vector to copy.
	/// @return A reference to this vector.
	///////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::operator=(Vector3f32 const& other)
	{
	memcpy(m_pData, other.m_pData, 3 * sizeof(float32));
		return *this;
	}

	/////////////////////////////////////////////
	/// @brief Sets all vector elements to 0.
	/////////////////////////////////////////////
	void Vector3f32::SetNull()
	{
		m_pData[0] = 0;
		m_pData[1] = 0;
		m_pData[2] = 0;
	}

	/////////////////////////////////////////////
	/// @brief Sets all vector elements to 1.
	/////////////////////////////////////////////
	void Vector3f32::SetOne()
	{
		m_pData[0] = 1;
		m_pData[1] = 1;
		m_pData[2] = 1;
	}

	//////////////////////////////////////////////////////////
	/// @brief Computes the dot product of this and other.
	/// @param other: The vector right of the operation.
	/// @return The dot product of this and other.
	/// @note This function uses SIMD instructions.
	//////////////////////////////////////////////////////////
	float32 Vector3f32::DotProduct(Vector3f32 const& other) const
	{
		__m128 v1 = Load();
		__m128 v2 = other.Load();

		// Perform the dot product
		__m128 vDot = _mm_mul_ps(v1, v2);
		// x=Dot.vector4_f32[1], y=Dot.vector4_f32[2]
		__m128 vTemp = VecPermute(vDot, 1, 2, 1, 2);
		// Result.vector4_f32[0] = x+y
		vDot = _mm_add_ss(vDot, vTemp);
		// x=Dot.vector4_f32[2]
		vTemp = VecPermute(vTemp, 1, 1, 1, 1);
		// Result.vector4_f32[0] = (x+y)+z
		vDot = _mm_add_ss(vDot, vTemp);
		// Return x
		return vDot.m128_f32[0];
	}

	///////////////////////////////////////////////////
	/// @brief Gets this vector's norm.
	/// @return The vector's norm.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////
	float32 Vector3f32::Norm() const
	{
		__m128 v = Load();
		// Perform the dot product on x,y and z
		__m128 vLengthSq = _mm_mul_ps(v, v);
		// vTemp has z and y
		__m128 vTemp = VecPermute(vLengthSq, 2, 1, 2, 1);
		// x+z, y
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		// y,y,y,y
		vTemp = VecPermute(vTemp, 1, 1, 1, 1);
		// x+z+y,??,??,??
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		// Get the length
		vLengthSq = _mm_sqrt_ps(vLengthSq);
		return vLengthSq.m128_f32[0];
	}

	//////////////////////////////////////////////////////////////////////////
	/// @brief Gets this vector's norm squared.
	/// @return The vector's norm squared.
	/// @note This function uses SIMD instructions. Faster than GetNorm().
	//////////////////////////////////////////////////////////////////////////
	float32 Vector3f32::SquareNorm() const
	{
		__m128 v = Load();
		// Perform the dot product on x,y and z
		__m128 vLengthSq = _mm_mul_ps(v, v);
		// vTemp has z and y
		__m128 vTemp = VecPermute(vLengthSq, 2, 1, 2, 1);
		// x+z, y
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		// y,y,y,y
		vTemp = VecPermute(vTemp, 1, 1, 1, 1);
		// x+z+y,??,??,??
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		// Get the length squared
		return vLengthSq.m128_f32[0];
	}

	////////////////////////////////////////////////////////////
	/// @brief Computes the cross product of this and other.
	/// @param other: The vector right of the operation.
	/// @return The cross product of this and other.
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::CrossProduct(Vector3f32 const& other) const
	{
		__m128 v1 = Load();
		__m128 v2 = other.Load();

		// y1,z1,x1,w1
		__m128 vTemp1 = VecPermute(v1, 1, 2, 0, 3);
		// z2,x2,y2,w2
		__m128 vTemp2 = VecPermute(v2, 2, 0, 1, 3);
		// Perform the left operation
		__m128 vResult = _mm_mul_ps(vTemp1, vTemp2);
		// z1,x1,y1,w1
		vTemp1 = VecPermute(vTemp1, 1, 2, 0, 3);
		// y2,z2,x2,w2
		vTemp2 = VecPermute(vTemp2, 2, 0, 1, 3);
		// Perform the right operation
		vResult = _mm_sub_ps(vResult, _mm_mul_ps(vTemp1, vTemp2));
		// Set w to zero
		__m128i temp = _mm_set_epi32( 0x00000000 , 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		__m128 mask3 = _mm_castsi128_ps(temp);
		vResult = _mm_and_ps(vResult, mask3);
		return vResult;
	}

	////////////////////////////////////////////////////////////////////
	/// @brief Gets this vector normalized. (x/norm, y/norm, z/norm)
	/// @return The normalized vector.
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::Normalize() const
	{
		__m128 v = Load();

		// Perform the dot product on x,y and z only
		__m128 vLengthSq = _mm_mul_ps(v, v);
		__m128 vTemp = VecPermute(vLengthSq, 1, 2, 1, 2);
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		vTemp = VecPermute(vTemp, 1, 1, 1, 1);
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		vLengthSq = VecPermute(vLengthSq, 0, 0, 0, 0);
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
		__m128 qNan = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, qNan);
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);
		return vResult;
	}

	///////////////////////////////////////////////////////////////
	/// @brief Normalizes this vector. (x/norm, y/norm, z/norm)
	/// @return The reference to this normalized vector.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::SelfNormalize()
	{
		__m128 v = Load();

		// Perform the dot product on x,y and z only
		__m128 vLengthSq = _mm_mul_ps(v, v);
		__m128 vTemp = VecPermute(vLengthSq, 1, 2, 1, 2);
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		vTemp = VecPermute(vTemp, 1, 1, 1, 1);
		vLengthSq = _mm_add_ss(vLengthSq, vTemp);
		vLengthSq = VecPermute(vLengthSq, 0, 0, 0, 0);
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
		__m128 qNan = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, qNan);
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		*this = _mm_or_ps(vTemp1, vTemp2);
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 substraction.
	/// @param other: The vector to substract.
	/// @return The result of the substraction. (x1 - x2, y1 - y1, z1 - z2)
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::operator-(Vector3f32 const& right) const
	{
		__m128 v1 = Load();
		v1.m128_f32[3] = 0;
		__m128 v2 = right.Load();
		v2.m128_f32[3] = 0;

		return Vector3f32(_mm_sub_ps(v1, v2));
	}

	///////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 addition.
	/// @param other: The vector to add.
	/// @return The result of the addition. (x1 + x2, y1 + y1, z1 + z2)
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::operator+(Vector3f32 const& other) const
	{
		__m128 v1 = Load();
		__m128 v2 = other.Load();

		return Vector3f32(_mm_add_ps(v1, v2));
	}
	
	/////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 multiplication.
	/// @param scalar: The Vector3 to multiply with.
	/// @return The result of the multiplication. (x1 * x2, y1 * y2, z1 * z2)
	/// @note This function uses SIMD instructions.
	/////////////////////////////////////////////////////////////////////////////
	Vector3f32 gce::Vector3f32::operator*(Vector3f32 const& other) const
	{
		__m128 v1 = Load();
		__m128 v2 = other.Load();

		return Vector3f32(_mm_mul_ps(v1, v2));
	}

	/////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 division.
	/// @param scalar: The Vector3 right to the operation.
	/// @return The result of the division. (x1/x2, y1/y2, z1/z2)
	/// @note This function uses SIMD instructions.
	/////////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::operator/(Vector3f32 const& right) const
	{
		__m128 v1 = Load();
		__m128 v2 = right.Load();

		return Vector3f32(_mm_div_ps(v1, v2));
	}

	//////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 multiplication.
	/// @param scalar: The scalar to multiply with.
	/// @return The result of the multiplication. (x * scalar, y * scalar, z * scalar)
	/// @note This function uses SIMD instructions.
	//////////////////////////////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::operator*(float32 const scalar) const
	{
		__m128 v1 = Load();
		__m128 v2 = _mm_set1_ps(scalar);

		return Vector3f32(_mm_mul_ps(v1, v2));
	}

	//////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 division.
	/// @param scalar: The scalar right to the operation.
	/// @return The result of the division. (x/scalar, y/scalar, z/scalar)
	/// @note This function uses SIMD instructions.
	//////////////////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::operator/(float32 const scalar) const
	{
		__m128 v1 = Load();
		__m128 v2 = _mm_set1_ps(scalar);

		return Vector3f32(_mm_div_ps(v1, v2));
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 substraction.
	/// @param other: The vector to substract.
	/// @return The result of the substraction. (x1 - x2, y1 - y1, z1 - z2)
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////////
	Vector3f32 Vector3f32::operator-() const
	{
		__m128 v1 = Load();
		__m128 scalarV = _mm_set1_ps(-1);

		return _mm_mul_ps(v1, scalarV);
	}

	////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 indexing.
	/// @param index: The index of the value to access.
	/// @return The value at the given index.
	////////////////////////////////////////////////////////
	float32& Vector3f32::operator[](int8 const index)
	{
		return m_pData[index];
	}

	////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 indexing.
	/// @param index: The index of the value to access.
	/// @return The value at the given index.
	////////////////////////////////////////////////////////
	float32 const& Vector3f32::operator[](int8 const index) const
	{
		return m_pData[index];
	}

	////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 assignement addition.
	/// @param other: The vector to add.
	/// @return The reference to the result of the addition. (x1 + x2, y1 + y1, z1 + z2)
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::operator+=(Vector3f32 const& other)
	{
		__m128 v1 = Load();
		__m128 v2 = other.Load();

		Store(_mm_add_ps(v1, v2));
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 assignement substraction.
	/// @param other: The vector right to the operation.
	/// @return The reference to the result of the substraction. (x1 - x2, y1 - y1, z1 - z2)
	/// @note This function uses SIMD instructions.
	////////////////////////////////////////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::operator-=(Vector3f32 const& right)
	{
		__m128 v1 = Load();
		__m128 v2 = right.Load();

		Store(_mm_sub_ps(v1, v2));
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 assignement multiplication.
	/// @param scalar: The Vector3 to multiply with.
	/// @return The reference to the result of the multiplication. (x1 * x2, y1 * y2, z1 * z2)
	/// @note This function uses SIMD instructions.
	//////////////////////////////////////////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::operator*=(Vector3f32 const& other)
	{
		__m128 v1 = Load();
		__m128 v2 = other.Load();

		Store(_mm_mul_ps(v1, v2));
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 assignement division.
	/// @param scalar: The Vector3 right to the operation.
	/// @return The reference to the result of the division. (x1/x2, y1/y2, z1/z2)
	/// @note This function uses SIMD instructions.
	//////////////////////////////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::operator/=(Vector3f32 const& right)
	{
		__m128 v1 = Load();
		__m128 v2 = right.Load();

		Store(_mm_div_ps(v1, v2));
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 assignement multiplication.
	/// @param scalar: The scalar to multiply with.
	/// @return The reference to the result of the multiplication. (x * scalar, y * scalar, z * scalar)
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::operator*=(float32 const scalar)
	{
		__m128 v1 = Load();
		__m128 v2 = _mm_set1_ps(scalar);

		Store(_mm_mul_ps(v1, v2));
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 assignement division.
	/// @param scalar: The scalar to divide with.
	/// @return The reference to the result of the division. (x/scalar, y/scalar, z/scalar)
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////////////////////////
	Vector3f32& Vector3f32::operator/=(float32 const scalar)
	{
		__m128 v1 = Load();
		__m128 v2 = _mm_set1_ps(scalar);

		Store(_mm_div_ps(v1, v2));
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 comparison.
	/// @param other: The vector to compare with.
	/// @return True if both vectors have the same values, false otherwise.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////////
	bool Vector3f32::operator==(Vector3f32 const& other) const
	{
		__m128 v1 = Load();
		__m128 v2 = other.Load();
		__m128 res = _mm_cmpeq_ps(v1, v2);
		return (((_mm_movemask_ps(res) & 7) == 7) != 0);
	}

	///////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for Vector3 negative comparison.
	/// @param other: The vector to compare with.
	/// @return False if both vectors have the same values, true otherwise.
	/// @note This function uses SIMD instructions.
	///////////////////////////////////////////////////////////////////////////
	bool Vector3f32::operator!=(Vector3f32 const& other) const
	{
		return (*this == other) == false;
	}

	Vector3f32 Vector3f32::Lerp(Vector3f32 const& a, Vector3f32 const& b, float t)
	{
		__m128 v1 = a.Load();
		__m128 v2 = b.Load();

		__m128 l = _mm_sub_ps(v2, v1);
		__m128 s = _mm_set_ps1(t);
		return _mm_add_ps(_mm_mul_ps(l, s), v1);
	}

	Vector3f32 Vector3f32::Lerp(Vector3f32 const& a, Vector3f32 const& b, Vector3f32 const& t)
	{
		__m128 v1 = a.Load();
		__m128 v2 = b.Load();
		__m128 v3 = t.Load();

		__m128 l = _mm_sub_ps(v2, v1);
		return _mm_add_ps(_mm_mul_ps(l, v3), (v1));
	}

	Vector3f32::Vector3f32(__m128 v)
	{
		Store(v);
	}
	__m128 Vector3f32::Load() const
	{
		__m128 vecXY = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<double const*>(m_pData)));
		__m128 vecZ = _mm_load_ss(&z);
		return _mm_movelh_ps(vecXY, vecZ);
	}

	void _vectorcall Vector3f32::Store(__m128 v)
	{
		_mm_store_sd(reinterpret_cast<double*>(m_pData), _mm_castps_pd(v));
		__m128 vecZ = VecPermute(v, 2, 2, 2, 2);
		_mm_store_ss(&z, vecZ);
	}
}