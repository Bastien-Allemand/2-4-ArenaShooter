#include "Quaternion.h"
#include "Matrix.h"
#include "Vector3.h"
#include "Vector4.h"
#include <assert.h>
#include <emmintrin.h>
#include <xmmintrin.h>



namespace gce
{
constexpr float32 GCE_PI = 3.141592654f;
constexpr float32 GCE_2PI = 6.283185307f;
constexpr float32 GCE_1DIVPI = 0.318309886f;
constexpr float32 GCE_1DIV2PI = 0.159154943f;
constexpr float32 GCE_PIDIV2 = 1.570796327f;
constexpr float32 GCE_PIDIV4 = 0.785398163f;
	////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Permutes the components of two vectors based on the specified permutation indices
	/// @param V1 the first vector
	/// @param V2 the second vector
	/// @param PermuteX the permutation index for the X component
	/// @param PermuteY the permutation index for the Y component
	/// @param PermuteZ the permutation index for the Z component
	/// @param PermuteW the permutation index for the W component
	/// @return a new vector with the permuted components
	/// @note The permutation indices are expected to be in the range [0, 3] for each component.
	/// The function uses SIMD instructions to perform the permutation efficiently.
	////////////////////////////////////////////////////////////////////////////////////////////////		
	 __m128 __vectorcall Quaternion::Permute(__m128 V1, __m128 V2, uint16 PermuteX, uint16 PermuteY, uint16 PermuteZ, uint16 PermuteW)
	{
		const uint32_t* aPtr[2];
		aPtr[0] = reinterpret_cast<const uint32_t*>(&V1);
		aPtr[1] = reinterpret_cast<const uint32_t*>(&V2);

		__m128 Result;
		auto pWork = reinterpret_cast<uint32_t*>(&Result);

		const uint32_t i0 = PermuteX & 3;
		const uint32_t vi0 = PermuteX >> 2;
		pWork[0] = aPtr[vi0][i0];

		const uint32_t i1 = PermuteY & 3;
		const uint32_t vi1 = PermuteY >> 2;
		pWork[1] = aPtr[vi1][i1];

		const uint32_t i2 = PermuteZ & 3;
		const uint32_t vi2 = PermuteZ >> 2;
		pWork[2] = aPtr[vi2][i2];

		const uint32_t i3 = PermuteW & 3;
		const uint32_t vi3 = PermuteW >> 2;
		pWork[3] = aPtr[vi3][i3];

		return Result;
	}

	 //////////////////////////////////////////////////////////////////////////////////////////////////////////
	 /// @brief Computes the sine and cosine of angles using SIMD instructions
	 /// @param cos the output vector for cosine values
	 /// @param sin the output vector for sine values
	 /// @param angles the input vector containing angles in radians
	 /// @note This function uses a polynomial approximation to compute sine and cosine values efficiently.
	 //////////////////////////////////////////////////////////////////////////////////////////////////////////
	void __vectorcall Quaternion::CosSin(__m128& cos, __m128& sin, __m128 angles)
	{
		__m128i negativeZero_int = _mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000);
		__m128 negativeZero = _mm_castsi128_ps(negativeZero_int);

		// Force the value within the bounds of pi
		__m128 x;
		{

			__m128 vResult = _mm_mul_ps(angles, { GCE_1DIV2PI, GCE_1DIV2PI, GCE_1DIV2PI, GCE_1DIV2PI });
			// Use the inline function due to complexity for rounding
			{
				__m128 sign = _mm_and_ps(vResult, negativeZero); 
				__m128 sMagic = _mm_or_ps({ 8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f }, sign);
				__m128 R1 = _mm_add_ps(vResult, sMagic);
				R1 = _mm_sub_ps(R1, sMagic);

				__m128i absMask_int = _mm_set_epi32(0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF);
				__m128 absMask = _mm_castsi128_ps(absMask_int);

				__m128 R2 = _mm_and_ps(vResult, absMask);
				__m128 mask = _mm_cmple_ps(R2, { 8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f });
				R2 = _mm_andnot_ps(mask, vResult);
				R1 = _mm_and_ps(R1, mask);
				vResult = _mm_xor_ps(R1, R2);
			}
			x = _mm_sub_ps(angles, _mm_mul_ps(vResult, { GCE_2PI, GCE_2PI, GCE_2PI, GCE_2PI }));
		}

		// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
		__m128 sign = _mm_and_ps(x, negativeZero);
		__m128 c = _mm_or_ps({ GCE_PI, GCE_PI, GCE_PI, GCE_PI }, sign);  // pi when x >= 0, -pi when x < 0
		__m128 absx = _mm_andnot_ps(sign, x);  // |x|
		__m128 rflx = _mm_sub_ps(c, x);
		__m128 comp = _mm_cmple_ps(absx, { GCE_PIDIV2, GCE_PIDIV2, GCE_PIDIV2, GCE_PIDIV2 });
		__m128 select0 = _mm_and_ps(comp, x);
		__m128 select1 = _mm_andnot_ps(comp, rflx);
		x = _mm_or_ps(select0, select1);
		select0 = _mm_and_ps(comp, { 1.f, 1.f, 1.f, 1.f });
		select1 = _mm_andnot_ps(comp, { -1.f, -1.f, -1.f, -1.f });
		sign = _mm_or_ps(select0, select1);

		__m128 x2 = _mm_mul_ps(x, x);

		// Compute polynomial approximation of sine
		const __m128 SC1 = { -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ };
		__m128 vConstantsB = _mm_shuffle_ps(SC1, SC1, _MM_SHUFFLE(0, 0, 0, 0));
		const __m128 SC0 = { -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f };
		__m128 vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 Result = _mm_add_ps(_mm_mul_ps(vConstantsB, x2), vConstants);

		vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(_mm_mul_ps(Result, x2), vConstants);

		vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(_mm_mul_ps(Result, x2), vConstants);

		vConstants = _mm_shuffle_ps(SC0, SC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(_mm_mul_ps(Result, x2), vConstants);

		Result = _mm_add_ps(_mm_mul_ps(Result, x2), { 1, 1, 1, 1 });
		Result = _mm_mul_ps(Result, x);
		sin = Result;

		// Compute polynomial approximation of cosine
		const __m128 CC1 = { -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ };
		vConstantsB = _mm_shuffle_ps(CC1, CC1, _MM_SHUFFLE(0, 0, 0, 0));
		const __m128 CC0 = { -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f };

		vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(3, 3, 3, 3));
		Result = _mm_add_ps(_mm_mul_ps(vConstantsB, x2), vConstants);

		vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(2, 2, 2, 2));
		Result = _mm_add_ps(_mm_mul_ps(Result, x2), vConstants);

		vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(1, 1, 1, 1));
		Result = _mm_add_ps(_mm_mul_ps(Result, x2), vConstants);

		vConstants = _mm_shuffle_ps(CC0, CC0, _MM_SHUFFLE(0, 0, 0, 0));
		Result = _mm_add_ps(_mm_mul_ps(Result, x2), vConstants);

		Result = _mm_add_ps(_mm_mul_ps(Result, x2), { 1, 1, 1, 1 });
		Result = _mm_mul_ps(Result, sign);
		cos = Result;
	}

	/////////////////////////////////////////////////////////////////////////////////
	/// @brief Default constructor for Quaternion
	/// @note Initializes the quaternion to the identity quaternion (0, 0, 0, 1).
	/////////////////////////////////////////////////////////////////////////////////
	Quaternion::Quaternion()
	{
		SetIdentity();
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Constructor for Quaternion using individual components
	/// @param x the x component of the quaternion
	/// @param y the y component of the quaternion
	/// @param z the z component of the quaternion
	/// @param w the w component of the quaternion
	////////////////////////////////////////////////////////////////////////////
	Quaternion::Quaternion(float32 x, float32 y, float32 z, float32 w)
	{
		pdata[0] = x;
		pdata[1] = y;
		pdata[2] = z;
		pdata[3] = w;
	}

	///////////////////////////////////////////////////////////////////////////////////////
	/// @brief Constructor for Quaternion using an array of 4 float values
	/// @param values an array of 4 float values representing the quaternion components
	///////////////////////////////////////////////////////////////////////////////////////
	Quaternion::Quaternion(float32 values[4])
	{
		memcpy(pdata, values, sizeof(float) * 4);
	}
	
	Quaternion Quaternion::operator+(float32 const right) const
	 {
	 	__m128 Q1 = _mm_load_ps(pdata);
	 	__m128 vResult = _mm_mul_ps(_mm_set_ps1(right), Q1);
	 	
	 	float32 result[4];
	 	_mm_store_ps(result, vResult);
		return Quaternion(result);
	 }

	Quaternion Quaternion::operator+(Quaternion const& right) const
	{
	 	__m128 Q1 = _mm_load_ps(pdata);
	 	__m128 Q2 = _mm_load_ps(right.pdata);
	 	__m128 vResult = _mm_add_ps(Q1, Q2);
	 	
	 	float32 result[4];
	 	_mm_store_ps(result, vResult);
		return Quaternion(result);
	}

	Quaternion Quaternion::operator-(Quaternion const& right) const
	{
	 	__m128 Q1 = _mm_load_ps(pdata);
	 	__m128 Q2 = _mm_load_ps(right.pdata);
	 	__m128 vResult = _mm_sub_ps(Q1, Q2);
	 	
	 	float32 result[4];
	 	_mm_store_ps(result, vResult);
		return Quaternion(result);
	}

	Quaternion Quaternion::operator*(Quaternion const& o)
	 {
	 	__m128 Q1 = _mm_load_ps(pdata);
	 	__m128 Q2 = _mm_load_ps(o.pdata);

	 	static const __m128 ControlWZYX = { 1.0f, -1.0f, 1.0f, -1.0f };
	 	static const __m128 ControlZWXY = { 1.0f, 1.0f, -1.0f, -1.0f };
	 	static const __m128 ControlYXWZ = { -1.0f, 1.0f, 1.0f, -1.0f };
	 	// Copy to SSE registers and use as few as possible for x86
	 	__m128 Q2X = Q2;
	 	__m128 Q2Y = Q2;
	 	__m128 Q2Z = Q2;
	 	__m128 vResult = Q2;
	 	// Splat with one instruction
	 	vResult = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(3, 3, 3, 3));
	 	Q2X = _mm_shuffle_ps(Q2X, Q2X, _MM_SHUFFLE(0, 0, 0, 0));
	 	Q2Y = _mm_shuffle_ps(Q2Y, Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
	 	Q2Z = _mm_shuffle_ps(Q2Z, Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
	 	// Retire Q1 and perform Q1*Q2W
	 	vResult = _mm_mul_ps(vResult, Q1);
	 	__m128 Q1Shuffle = Q1;
	 	// Shuffle the copies of Q1
	 	Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
	 	// Mul by Q1WZYX
	 	Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
	 	Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
	 	// Flip the signs on y and z
	 	vResult = _mm_add_ps(_mm_mul_ps(Q2X, ControlWZYX), vResult);
	 	// Mul by Q1ZWXY
	 	Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
	 	Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
	 	// Flip the signs on z and w
	 	Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
	 	// Mul by Q1YXWZ
	 	Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
	 	// Flip the signs on x and w
	 	Q2Y = _mm_add_ps(_mm_mul_ps(Q2Z, ControlYXWZ), Q2Y);
	 	vResult = _mm_add_ps(vResult, Q2Y);
	 	float32 result[4];
	 	_mm_store_ps(result, vResult);

	 	return Quaternion(result);
	 }

	/////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Copy constructor for Quaternion
	/// @param o the quaternion to copy from
	/// @return a new Quaternion object initialized with the values from the input quaternion
	///*/////////////////////////////////////////////////////////////////////////////////////////
	Quaternion* Quaternion::operator=(Quaternion const& o)
	{
		memcpy(pdata, o.pdata, sizeof(float) * 4);
		return this;
	}

	////////////////////////////////////////////////////////////
	/// @brief Assignment operator for Quaternion
	/// @param o the quaternion to assign from
	/// @return a reference to the current Quaternion object
	////////////////////////////////////////////////////////////
Quaternion* Quaternion::operator*=(Quaternion const& o)
	 {
	 	__m128 Q1 = _mm_load_ps(pdata);
	 	__m128 Q2 = _mm_load_ps(o.pdata);

	 	static const __m128 ControlWZYX = { 1.0f, -1.0f, 1.0f, -1.0f };
	 	static const __m128 ControlZWXY = { 1.0f, 1.0f, -1.0f, -1.0f };
	 	static const __m128 ControlYXWZ = { -1.0f, 1.0f, 1.0f, -1.0f };
	 	// Copy to SSE registers and use as few as possible for x86
	 	__m128 Q2X = Q2;
	 	__m128 Q2Y = Q2;
	 	__m128 Q2Z = Q2;
	 	__m128 vResult = Q2;
	 	// Splat with one instruction
	 	vResult = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(3, 3, 3, 3));
	 	Q2X = _mm_shuffle_ps(Q2X, Q2X, _MM_SHUFFLE(0, 0, 0, 0));
	 	Q2Y = _mm_shuffle_ps(Q2Y, Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
	 	Q2Z = _mm_shuffle_ps(Q2Z, Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
	 	// Retire Q1 and perform Q1*Q2W
	 	vResult = _mm_mul_ps(vResult, Q1);
	 	__m128 Q1Shuffle = Q1;
	 	// Shuffle the copies of Q1
	 	Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
	 	// Mul by Q1WZYX
	 	Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
	 	Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
	 	// Flip the signs on y and z
	 	vResult = _mm_add_ps(_mm_mul_ps(Q2X, ControlWZYX), vResult);
	 	// Mul by Q1ZWXY
	 	Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
	 	Q1Shuffle = _mm_shuffle_ps(Q1Shuffle, Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
	 	// Flip the signs on z and w
	 	Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
	 	// Mul by Q1YXWZ
	 	Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
	 	// Flip the signs on x and w
	 	Q2Y = _mm_add_ps(_mm_mul_ps(Q2Z, ControlYXWZ), Q2Y);
	 	vResult = _mm_add_ps(vResult, Q2Y);

	 	_mm_store_ps(pdata, vResult);

	 	return this;
	 }

	//////////////////////////////////////////////////////////////////
	/// @brief Equality operator for Quaternion
	/// @param o the quaternion to compare with
	/// @return true if the quaternions are equal, false otherwise
	//////////////////////////////////////////////////////////////////
	bool Quaternion::operator==(Quaternion const& o) const
	{
		if (&o == this) return true;

		__m128 vector1 = _mm_load_ps(pdata);
		__m128 vector2 = _mm_load_ps(o.pdata);

		__m128 vTemp = _mm_cmpeq_ps(vector1, vector2);
		return ((_mm_movemask_ps(vTemp) == 0x0f)); // != 0
	}

	//////////////////////////////////////////////////////////////////////
	/// @brief Inequality operator for Quaternion
	/// @param o the quaternion to compare with
	/// @return true if the quaternions are not equal, false otherwise
	//////////////////////////////////////////////////////////////////////
	bool Quaternion::operator!=(Quaternion const& o) const
	{
		if (&o == this) return false;

		__m128 vector1 = _mm_load_ps(pdata);
		__m128 vector2 = _mm_load_ps(o.pdata);

		__m128 vTemp = _mm_cmpneq_ps(vector1, vector2);
		return ((_mm_movemask_ps(vTemp))); // != 0
	}

	//////////////////////////////////////////////////////
	/// @brief Computes the conjugate of a quaternion.
	/// @return Conjugate of the quaternion.
	/// //////////////////////////////////////////////////
	Quaternion Quaternion::GetConjugate() const
	{
		__m128 vector = _mm_load_ps(pdata);

		__m128 substraction = { -1, -1, -1, 1 };

		__m128 vResult = _mm_mul_ps(vector, substraction);

		float32 result[4];
		_mm_store_ps(result, vResult);

		return Quaternion(result);
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Checks if the quaternion is equal to the Identity quaternion.
	/// @return True if it's the Identity quaternion, false otherwise.
	////////////////////////////////////////////////////////////////////////////
	bool Quaternion::IsIdentity() const
	{
		__m128 vector1 = _mm_load_ps(pdata);
		__m128 vector2 = { 0, 0, 0, 1 };

		__m128 vTemp = _mm_cmpeq_ps(vector1, vector2);
		return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
	}

	/////////////////////////////////////////////////////////////
	/// @brief Checks if the quaternion's norm is equal to 0.
	/// @return True if it's 0, false otherwise.
	/////////////////////////////////////////////////////////////
	bool Quaternion::IsNormZero() const
	{
		return SquareNorm() < 1e-5f; // Check if the norm is under 0.00001f
	}

	/////////////////////////////////////////////
	/// @brief Gets the norm of a quaternion.
	/// @return The norm of the quaternion.
	/////////////////////////////////////////////
	float32 Quaternion::Norm() const
	{
		__m128 vector = _mm_load_ps(pdata);

		// Perform the dot product on x,y,z and w
		__m128 vLengthSq = _mm_mul_ps(vector, vector);
		// vTemp has z and w
		__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
		// x+z, y+w
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// x+z,x+z,x+z,y+w
		vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
		// ??,??,y+w,y+w
		vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
		// ??,??,x+z+y+w,??
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// Get the length
		vLengthSq = _mm_sqrt_ps(vLengthSq);
		return vLengthSq.m128_f32[2];
	}

	///////////////////////////////////////////////////////////
	/// @brief Gets the square of the norm of a quaternion.
	/// @return The square of norm of the quaternion.
	///////////////////////////////////////////////////////////
	float32 Quaternion::SquareNorm() const
	{
		__m128 vector = _mm_load_ps(pdata);
		// Perform the dot product on x,y,z and w
		__m128 vLengthSq = _mm_mul_ps(vector, vector);
		// vTemp has z and w
		__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
		// x+z, y+w
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		// x+z,x+z,x+z,y+w
		vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
		// ??,??,y+w,y+w
		vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
		// ??,??,x+z+y+w,??
		vLengthSq = _mm_add_ps(vLengthSq, vTemp);
		return vLengthSq.m128_f32[2];
	}

	//////////////////////////////////////////////
	/// @brief Gets the normalized quaternion.
	/// @return The normalized quaternion.
	//////////////////////////////////////////////
	Quaternion Quaternion::Normalized() const
	{
		__m128 vector =_mm_load_ps(pdata);

		float32 sqNorm = SquareNorm();

		__m128 vLengthSq = { sqNorm, sqNorm, sqNorm, sqNorm };

		__m128 vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		__m128 vZeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		vLengthSq = _mm_cmpneq_ps(vLengthSq, { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } });
		// Divide to perform the normalization
		vResult = _mm_div_ps(vector, vResult);
		// Any that are infinity, set to zero
		vResult = _mm_and_ps(vResult, vZeroMask);
		// Select qnan or result based on infinite length
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, { { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } });
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);

		float32 result[4];
		_mm_store_ps(result, vResult);

		return Quaternion(result);
	}

	//////////////////////////////////////////////////////////////
	/// @brief Sets the quaternion to the Identity quaternion.
	//////////////////////////////////////////////////////////////
	void Quaternion::SetIdentity()
	{
		pdata[0] = 0;
		pdata[1] = 0;
		pdata[2] = 0;
		pdata[3] = 1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Sets the quaternion about an axis.
	/// @param 3D vector describing the axis of rotation.
	/// @param Angle of rotation in radians. Angles are measured clockwise when looking along the rotation axis toward the origin.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Quaternion::SetRotationAxis(Vector3f32 const& axis, float32 const& angle)
	{
		assert(axis != Vector3f32(0, 0, 0));

		Vector3f32 normAxis = axis.Normalize();
		__m128 normVector = { normAxis.x, normAxis.y, normAxis.z };
		{
			__m128i mask3_int = _mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
			__m128 mask3 = _mm_castsi128_ps(mask3_int);

			__m128 N = _mm_and_ps(normVector, mask3);

			N = _mm_or_ps(N, { 0, 0, 0, 1 });

			__m128 Scale = _mm_set_ps1(0.5f * angle);
			__m128 vSine;
			__m128 vCosine;
			CosSin(vCosine, vSine, Scale);
			
			Scale = _mm_and_ps(vSine, mask3); 

			__m128i maskW_int = _mm_set_epi32(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000);
			__m128 maskW = _mm_castsi128_ps(maskW_int);

			vCosine = _mm_and_ps(vCosine, maskW);
			Scale = _mm_or_ps(Scale, vCosine);
			__m128 vResult = _mm_mul_ps(N, Scale);
			_mm_store_ps(pdata, vResult);
		}
	}

	//////////////////////////////////////////////////////////
	/// @brief Sets the quaternion from a rotation matrix.
	/// @param Rotation matrix.
	//////////////////////////////////////////////////////////
	void Quaternion::SetRotationMatrix(Matrix const& mat)
	{
		__m128 r0 =	_mm_load_ps(mat.m_matrix[0]);
		__m128 r1 = _mm_load_ps(mat.m_matrix[1]);
		__m128 r2 = _mm_load_ps(mat.m_matrix[2]);

		__m128 XMPMMP = { +1.0f, -1.0f, -1.0f, +1.0f };
		__m128 XMMPMP = { -1.0f, +1.0f, -1.0f, +1.0f };
		__m128 XMMMPP = { -1.0f, -1.0f, +1.0f, +1.0f };

		// (r00, r00, r00, r00)
		__m128 r00 = _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0));
		// (r11, r11, r11, r11)
		__m128 r11 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1));
		// (r22, r22, r22, r22)
		__m128 r22 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2));

		// x^2 >= y^2 equivalent to r11 - r00 <= 0
		// (r11 - r00, r11 - r00, r11 - r00, r11 - r00)
		__m128 r11mr00 = _mm_sub_ps(r11, r00);
		__m128 x2gey2 = _mm_cmple_ps(r11mr00, {0, 0, 0, 0});

		// z^2 >= w^2 equivalent to r11 + r00 <= 0
		// (r11 + r00, r11 + r00, r11 + r00, r11 + r00)
		__m128 r11pr00 = _mm_add_ps(r11, r00);
		__m128 z2gew2 = _mm_cmple_ps(r11pr00, { 0, 0, 0, 0 });

		// x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
		__m128 x2py2gez2pw2 = _mm_cmple_ps(r22, { 0, 0, 0, 0 });

		// (4*x^2, 4*y^2, 4*z^2, 4*w^2)
		__m128 t0 = _mm_add_ps(_mm_mul_ps(XMPMMP, r00), { 1.f, 1.f, 1.f, 1.f });
		__m128 t1 = _mm_mul_ps(XMMPMP, r11);
		__m128 t2 = _mm_add_ps(_mm_mul_ps(XMMMPP, r22), t0);
		__m128 x2y2z2w2 = _mm_add_ps(t1, t2);

		// (r01, r02, r12, r11)
		t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
		// (r10, r10, r20, r21)
		t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
		// (r10, r20, r21, r10)
		t1 = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 3, 2, 0));
		// (4*x*y, 4*x*z, 4*y*z, unused)
		__m128 xyxzyz = _mm_add_ps(t0, t1);

		// (r21, r20, r10, r10)
		t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
		// (r12, r12, r02, r01)
		t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
		// (r12, r02, r01, r12)
		t1 = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 3, 2, 0));
		// (4*x*w, 4*y*w, 4*z*w, unused)
		__m128 xwywzw = _mm_sub_ps(t0, t1);
		xwywzw = _mm_mul_ps(XMMPMP, xwywzw);

		// (4*x^2, 4*y^2, 4*x*y, unused)
		t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
		// (4*z^2, 4*w^2, 4*z*w, unused)
		t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
		// (4*x*z, 4*y*z, 4*x*w, 4*y*w)
		t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));

		// (4*x*x, 4*x*y, 4*x*z, 4*x*w)
		__m128 tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
		// (4*y*x, 4*y*y, 4*y*z, 4*y*w)
		__m128 tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
		// (4*z*x, 4*z*y, 4*z*z, 4*z*w)
		__m128 tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
		// (4*w*x, 4*w*y, 4*w*z, 4*w*w)
		__m128 tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));

		// Select the row of the tensor-product matrix that has the largest
		// magnitude.
		t0 = _mm_and_ps(x2gey2, tensor0);
		t1 = _mm_andnot_ps(x2gey2, tensor1);
		t0 = _mm_or_ps(t0, t1);
		t1 = _mm_and_ps(z2gew2, tensor2);
		t2 = _mm_andnot_ps(z2gew2, tensor3);
		t1 = _mm_or_ps(t1, t2);
		t0 = _mm_and_ps(x2py2gez2pw2, t0);
		t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
		t2 = _mm_or_ps(t0, t1);

		// Normalize the row.  No division by zero is possible because the
		// quaternion is unit-length (and the row is a nonzero multiple of
		// the quaternion).
		{
			// Perform the dot product on x,y,z and w
			__m128 vLengthSq = _mm_mul_ps(t2, t2);
			// vTemp has z and w
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the length
			t0 = _mm_sqrt_ps(vLengthSq);
		}
		__m128 vResult = _mm_div_ps(t2, t0);

		_mm_store_ps(pdata, vResult);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Sets a rotation quaternion based on the pitch, yaw, and roll (Euler angles).
	/// @param Roll in radians (X axis)
	/// @param Pitch in radians (Y axis)
	/// @param Yaw in radians (Z axis)
	///////////////////////////////////////////////////////////////////////////////////////////
	void Quaternion::SetRotationEuler(float32 angleX, float32 angleY, float32 angleZ)
	{
		__m128 Angles = { angleX, angleY, angleZ , 0.f };

		__m128  signResult = { 1.0f, -1.0f, -1.0f, 1.0f };

		__m128 half = { 0.5f, 0.5f ,0.5f, 0.5f };
		__m128 HalfAngles = _mm_mul_ps(Angles, half);

		__m128 vSine;
		__m128 vCosine;
		
		CosSin(vCosine, vSine, HalfAngles);

		__m128 P0 = Permute(vSine, vCosine, 0, 4, 4, 4);
		__m128 Y0 = Permute(vSine, vCosine, 5, 1, 5, 5);
		__m128 R0 = Permute(vSine, vCosine, 6, 6, 2, 6);
		__m128 P1 = Permute(vCosine, vSine, 0, 4, 4, 4);
		__m128 Y1 = Permute(vCosine, vSine, 5, 1, 5, 5);
		__m128 R1 = Permute(vCosine, vSine, 6, 6, 2, 6);

		__m128 Q1 = _mm_mul_ps(P1, signResult);
		__m128 Q0 = _mm_mul_ps(P0, Y0);
		Q1 = _mm_mul_ps(Q1, Y1);
		Q0 = _mm_mul_ps(Q0, R0);
		__m128 Q = _mm_add_ps(_mm_mul_ps(Q1, R1), Q0);

		_mm_store_ps(pdata, Q);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Sets a rotation quaternion based on a vector containing the Euler angles (pitch, yaw, and roll).
	/// @param 3D vector containing the Euler angles in the order x-axis (pitch), then y-axis (yaw), and then z-axis (roll).
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Quaternion::SetRotationEuler(Vector3f32 const& angles)
	{
		SetRotationEuler(angles.x, angles.y, angles.z);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Computes an axis and angle of rotation about that axis for the quaternion.
	/// @param Reference of a 3D vector describing the axis of rotation for the quaternion.
	/// @param Reference of float describing the radian angle of rotation for the quaternion.
	/////////////////////////////////////////////////////////////////////////////////////////////
	void Quaternion::QuaternionToAxisAngle(Vector3f32& axis, float& angle)
	{
		axis = { pdata[0],pdata[1],pdata[2]};

		bool nonnegative = (pdata[3] >= 0.0f);
		float32 x = fabsf(pdata[3]);
		float32 omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}
		float32 root = sqrtf(omx);

		// 7-degree minimax approximation
		float32 result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
		result *= root;

		// acos(x) = pi - acos(-x) when x < 0
		float32 scalarACos = nonnegative ? result : GCE_PI - result;

		angle = 2.f * scalarACos;
	}

	//////////////////////////////////////////////////////////////
	/// @brief Computes the quaternion from a rotation matrix.
	/// @param Rotation matrix.
	/// @return Rotation quaternion.
	//////////////////////////////////////////////////////////////
	Quaternion Quaternion::RotationMatrix(Matrix const& mat)
	{
		__m128 r0 = _mm_load_ps(mat.m_matrix[0]);
		__m128 r1 = _mm_load_ps(mat.m_matrix[1]);
		__m128 r2 = _mm_load_ps(mat.m_matrix[2]);

		__m128 XMPMMP = { +1.0f, -1.0f, -1.0f, +1.0f };
		__m128 XMMPMP = { -1.0f, +1.0f, -1.0f, +1.0f };
		__m128 XMMMPP = { -1.0f, -1.0f, +1.0f, +1.0f };

		// (r00, r00, r00, r00)
		__m128 r00 = _mm_shuffle_ps(r0, r0, _MM_SHUFFLE(0, 0, 0, 0));
		// (r11, r11, r11, r11)
		__m128 r11 = _mm_shuffle_ps(r1, r1, _MM_SHUFFLE(1, 1, 1, 1));
		// (r22, r22, r22, r22)
		__m128 r22 = _mm_shuffle_ps(r2, r2, _MM_SHUFFLE(2, 2, 2, 2));

		// x^2 >= y^2 equivalent to r11 - r00 <= 0
		// (r11 - r00, r11 - r00, r11 - r00, r11 - r00)
		__m128 r11mr00 = _mm_sub_ps(r11, r00);
		__m128 x2gey2 = _mm_cmple_ps(r11mr00, { 0, 0, 0, 0 });

		// z^2 >= w^2 equivalent to r11 + r00 <= 0
		// (r11 + r00, r11 + r00, r11 + r00, r11 + r00)
		__m128 r11pr00 = _mm_add_ps(r11, r00);
		__m128 z2gew2 = _mm_cmple_ps(r11pr00, { 0, 0, 0, 0 });

		// x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
		__m128 x2py2gez2pw2 = _mm_cmple_ps(r22, { 0, 0, 0, 0 });

		// (4*x^2, 4*y^2, 4*z^2, 4*w^2)
		__m128 t0 = _mm_add_ps(_mm_mul_ps(XMPMMP, r00), { 1.f, 1.f, 1.f, 1.f });
		__m128 t1 = _mm_mul_ps(XMMPMP, r11);
		__m128 t2 = _mm_add_ps(_mm_mul_ps(XMMMPP, r22), t0);
		__m128 x2y2z2w2 = _mm_add_ps(t1, t2);

		// (r01, r02, r12, r11)
		t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
		// (r10, r10, r20, r21)
		t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
		// (r10, r20, r21, r10)
		t1 = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 3, 2, 0));
		// (4*x*y, 4*x*z, 4*y*z, unused)
		__m128 xyxzyz = _mm_add_ps(t0, t1);

		// (r21, r20, r10, r10)
		t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
		// (r12, r12, r02, r01)
		t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
		// (r12, r02, r01, r12)
		t1 = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 3, 2, 0));
		// (4*x*w, 4*y*w, 4*z*w, unused)
		__m128 xwywzw = _mm_sub_ps(t0, t1);
		xwywzw = _mm_mul_ps(XMMPMP, xwywzw);

		// (4*x^2, 4*y^2, 4*x*y, unused)
		t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
		// (4*z^2, 4*w^2, 4*z*w, unused)
		t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
		// (4*x*z, 4*y*z, 4*x*w, 4*y*w)
		t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));

		// (4*x*x, 4*x*y, 4*x*z, 4*x*w)
		__m128 tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
		// (4*y*x, 4*y*y, 4*y*z, 4*y*w)
		__m128 tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
		// (4*z*x, 4*z*y, 4*z*z, 4*z*w)
		__m128 tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
		// (4*w*x, 4*w*y, 4*w*z, 4*w*w)
		__m128 tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));

		// Select the row of the tensor-product matrix that has the largest
		// magnitude.
		t0 = _mm_and_ps(x2gey2, tensor0);
		t1 = _mm_andnot_ps(x2gey2, tensor1);
		t0 = _mm_or_ps(t0, t1);
		t1 = _mm_and_ps(z2gew2, tensor2);
		t2 = _mm_andnot_ps(z2gew2, tensor3);
		t1 = _mm_or_ps(t1, t2);
		t0 = _mm_and_ps(x2py2gez2pw2, t0);
		t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
		t2 = _mm_or_ps(t0, t1);

		// Normalize the row.  No division by zero is possible because the
		// quaternion is unit-length (and the row is a nonzero multiple of
		// the quaternion).
		{
			// Perform the dot product on x,y,z and w
			__m128 vLengthSq = _mm_mul_ps(t2, t2);
			// vTemp has z and w
			__m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
			// x+z, y+w
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// x+z,x+z,x+z,y+w
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
			// ??,??,y+w,y+w
			vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
			// ??,??,x+z+y+w,??
			vLengthSq = _mm_add_ps(vLengthSq, vTemp);
			// Splat the length
			vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
			// Get the length
			t0 = _mm_sqrt_ps(vLengthSq);
		}
		__m128 vResult = _mm_div_ps(t2, t0);

		float32 result[4];
		_mm_store_ps(result, vResult);
		return Quaternion(result);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Computes the quaternion about an axis.
	/// @param 3D vector describing the axis of rotation.
	/// @param Angle of rotation in radians. Angles are measured clockwise when looking along the rotation axis toward the origin.
	/// @return Rotation quaternion.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Quaternion Quaternion::RotationAxis(Vector3f32 const& axis, float32 const& angle)
	{
		assert(axis != Vector3f32(0, 0, 0));

		Vector3f32 normAxis = axis.Normalize();
		__m128 normVector = { normAxis.x, normAxis.y, normAxis.z };
		__m128i mask3_int = _mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		__m128 mask3 = _mm_castsi128_ps(mask3_int);

		__m128 N = _mm_and_ps(normVector, mask3);

		N = _mm_or_ps(N, { 0, 0, 0, 1 });

		__m128 Scale = _mm_set_ps1(0.5f * angle);
		__m128 vSine;
		__m128 vCosine;
		CosSin(vCosine, vSine, Scale);

		Scale = _mm_and_ps(vSine, mask3); 

		__m128i maskW_int = _mm_set_epi32(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000);
		__m128 maskW = _mm_castsi128_ps(maskW_int);

		vCosine = _mm_and_ps(vCosine, maskW);
		Scale = _mm_or_ps(Scale, vCosine);
		__m128 vResult = _mm_mul_ps(N, Scale);

		float32 result[4];
		_mm_store_ps(result, vResult);
		return Quaternion(result);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Computes a rotation quaternion based on a vector containing the Euler angles (pitch, yaw, and roll).
	/// @param Roll in radians (X axis)
	/// @param Pitch in radians (Y axis)
	/// @param Yaw in radians (Z axis)
	/// @return Rotation quaternion.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Quaternion Quaternion::RotationEuler(float32 angleX, float32 angleY, float32 angleZ)
	{
		__m128 Angles = _mm_set_ps(0.f, angleZ, angleY, angleX);

		__m128  signResult = { 1.0f, -1.0f, -1.0f, 1.0f };

		__m128 half = { 0.5f, 0.5f ,0.5f, 0.5f };
		__m128 HalfAngles = _mm_mul_ps(Angles, half);

		__m128 vSine;
		__m128 vCosine;
		CosSin(vCosine, vSine,HalfAngles);

		__m128 P0 = Permute(vSine, vCosine, 0, 4, 4, 4);
		__m128 Y0 = Permute(vSine, vCosine, 5, 1, 5, 5);
		__m128 R0 = Permute(vSine, vCosine, 6, 6, 2, 6);
		__m128 P1 = Permute(vCosine, vSine, 0, 4, 4, 4);
		__m128 Y1 = Permute(vCosine, vSine, 5, 1, 5, 5);
		__m128 R1 = Permute(vCosine, vSine, 6, 6, 2, 6);

		__m128 Q1 = _mm_mul_ps(P1, signResult);
		__m128 Q0 = _mm_mul_ps(P0, Y0);
		Q1 = _mm_mul_ps(Q1, Y1);
		Q0 = _mm_mul_ps(Q0, R0);
		__m128 vResult = _mm_add_ps(_mm_mul_ps(Q1, R1), Q0);

		float32 result[4];
		_mm_store_ps(result, vResult);
		return Quaternion(result);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Computes a rotation quaternion based on a vector containing the Euler angles (pitch, yaw, and roll).
	/// @param 3D vector containing the Euler angles in the order x-axis (pitch), then y-axis (yaw), and then z-axis (roll).
	/// @return Rotation quaternion.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Quaternion Quaternion::RotationEuler(Vector3f32 const& angles)
	{
		return RotationEuler(angles.x, angles.y, angles.z);
	}

	///////////////////////////////////////////////////////
	/// @brief Gets the quaternion's X component value.
	/// @return Quaternion's X component value.
	///////////////////////////////////////////////////////
	float32 Quaternion::GetX() const
	{
		return pdata[0];
	}

	///////////////////////////////////////////////////////
	/// @brief Gets the quaternion's Y component value.
	/// @return Quaternion's Y component value.
	///////////////////////////////////////////////////////
	float32 Quaternion::GetY() const
	{
		return pdata[1];
	}

	///////////////////////////////////////////////////////
	/// @brief Gets the quaternion's Z component value.
	/// @return Quaternion's Z component value.
	///////////////////////////////////////////////////////
	float32 Quaternion::GetZ() const
	{
		return pdata[2];
	}

	///////////////////////////////////////////////////////
	/// @brief Gets the quaternion's W component value.
	/// @return Quaternion's W component value.
	///////////////////////////////////////////////////////
	float32 Quaternion::GetW() const
	{
		return pdata[3];
	}

	//////////////////////////////////////////////////////
	/// @brief Computes the inverse of the quaternion.
	/// @return Quaternion's inverse.
	//////////////////////////////////////////////////////
	Quaternion Quaternion::GetInverse() const
	{
		assert(!IsNormZero());

		float32 squareNorm = SquareNorm();

		__m128 lengthSq = _mm_set_ps1(squareNorm);

		Quaternion conjugate = GetConjugate();

		__m128 Conjugate = _mm_load_ps(conjugate.pdata);

		__m128 vResult = _mm_div_ps(Conjugate, lengthSq);

		float32 result[4];
		_mm_store_ps(result,vResult);
		return Quaternion(result);
	}

	////////////////////////////////////////////////////////////////////////////
	/// @brief Inverses the quaternion.
	/// @return True if inversible, false otherwise (if norm is equal to 0).
	////////////////////////////////////////////////////////////////////////////
	bool Quaternion::Inverse()
	{
		if (IsNormZero())
		{
			return false;
		}

		float32 squareNorm = SquareNorm();

		__m128 lengthSq = _mm_set_ps1(squareNorm);

		Quaternion conjugateQuat = GetConjugate();

		__m128 conjugate = _mm_load_ps(conjugateQuat.pdata);

		__m128 result = _mm_div_ps(conjugate, lengthSq);

		_mm_store_ps(pdata,result);
		return true;
	}

	/////////////////////////////////////////
	/// @brief Conjugates the quaternion.
	/// @return True.
	/// /////////////////////////////////////
	bool Quaternion::Conjugate()
	{
		__m128 vector = _mm_load_ps(pdata);

		__m128 sign = { -1.f, -1.f, -1.f, 1.f };
		__m128 result = _mm_mul_ps(vector, sign);

		_mm_store_ps(pdata, result);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////
	/// @brief Normalize the quaternion.
	/// @return True if normalizable, false otherwise (if norm is equal to 0).
	//////////////////////////////////////////////////////////////////////////////
	bool Quaternion::Normalize()
	{
		if (IsNormZero())
		{
			return false;
		}

		__m128 vector =_mm_load_ps(pdata);

		float32 sqNorm = SquareNorm();

		__m128 vLengthSq = { sqNorm, sqNorm, sqNorm, sqNorm };

		__m128 vResult = _mm_sqrt_ps(vLengthSq);
		// Create zero with a single instruction
		__m128 vZeroMask = _mm_setzero_ps();
		// Test for a divide by zero (Must be FP to detect -0.0)
		vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
		// Failsafe on zero (Or epsilon) length planes
		// If the length is infinity, set the elements to zero
		vLengthSq = _mm_cmpneq_ps(vLengthSq, { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } });
		// Divide to perform the normalization
		vResult = _mm_div_ps(vector, vResult);
		// Any that are infinity, set to zero
		vResult = _mm_and_ps(vResult, vZeroMask);
		// Select qnan or result based on infinite length
		__m128 vTemp1 = _mm_andnot_ps(vLengthSq, { { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } });
		__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
		vResult = _mm_or_ps(vTemp1, vTemp2);

		_mm_store_ps(pdata, vResult);
		return true;
	}
}
