#include "Matrix.h"
#include "MathsFunctions.hpp"
#include "Vector2.hpp"
#include "Vector3.h"
#include "Quaternion.h"
#include <assert.h>
#include <intrin.h>

#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)
#define VecPermute(vec, x,y,z,w)		   _mm_shuffle_ps(vec, vec, MakeShuffleMask(x,y,z,w))

#define DecomposeEpsilon 0.0001f
#define RankDecompose(a, b, c, x, y, z)      \
    if((x) < (y))                   \
    {                               \
        if((y) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 1;                \
            (c) = 0;                \
        }                           \
        else                        \
        {                           \
            (a) = 1;                \
                                    \
            if((x) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 0;            \
            }                       \
            else                    \
            {                       \
                (b) = 0;            \
                (c) = 2;            \
            }                       \
        }                           \
    }                               \
    else                            \
    {                               \
        if((x) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 0;                \
            (c) = 1;                \
        }                           \
        else                        \
        {                           \
            (a) = 0;                \
                                    \
            if((y) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 1;            \
            }                       \
            else                    \
            {                       \
                (b) = 1;            \
                (c) = 2;            \
            }                       \
        }                           \
    }


namespace gce
{

////////////////////////////////////////////////
/// @brief 2x2 row major Matrix multiply A*B
/// @param vec1 the first vector (matrix)
/// @param vec2 the second vector (matrix)
/// @return the result of the multiplication
////////////////////////////////////////////////
inline __m128 Mat2Mul(__m128 vec1, __m128 vec2)
{
	return
		_mm_add_ps(_mm_mul_ps(vec1, VecSwizzle(vec2, 0, 3, 0, 3)),
			_mm_mul_ps(VecSwizzle(vec1, 1, 0, 3, 2), VecSwizzle(vec2, 2, 1, 2, 1)));
}

////////////////////////////////////////////////////////////
/// @brief 2x2 row major Matrix adjugate multiply (A#)*B
/// @param vec1 the first vector (matrix)
/// @param vec2 the second vector (matrix)
/// @return the result of the adjugate multiplication
////////////////////////////////////////////////////////////
inline __m128 Mat2AdjMul(__m128 vec1, __m128 vec2)
{
	return
		_mm_sub_ps(_mm_mul_ps(VecSwizzle(vec1, 3, 3, 0, 0), vec2),
			_mm_mul_ps(VecSwizzle(vec1, 1, 1, 2, 2), VecSwizzle(vec2, 2, 3, 0, 1)));
}

////////////////////////////////////////////////////////////
/// @brief 2x2 row major Matrix multiply adjugate A*(B#)
/// @param vec1 the first vector (matrix)
/// @param vec2 the second vector (matrix)
/// @return the result of the multiplication adjugate
////////////////////////////////////////////////////////////
inline __m128 Mat2MulAdj(__m128 vec1, __m128 vec2)
{
	return
		_mm_sub_ps(_mm_mul_ps(vec1, VecSwizzle(vec2, 3, 0, 3, 0)),
			_mm_mul_ps(VecSwizzle(vec1, 1, 0, 3, 2), VecSwizzle(vec2, 2, 1, 2, 1)));
}

//////////////////////////////////////////////////////
/// @brief returns the length of a Vector3
/// @param V the vector to calculate the length of
/// @return the length of the vector
//////////////////////////////////////////////////////
inline __m128 Vector3Length(__m128 V)
{
	// Perform the dot product on x,y and z
	__m128 vLengthSq = _mm_mul_ps(V, V);
	// vTemp has z and y
	__m128 vTemp = VecPermute(vLengthSq, 2, 1, 2, 1);
	// x+z, y
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	// y,y,y,y
	vTemp = VecPermute(vTemp, 1, 1, 1, 1);
	// x+z+y,??,??,??
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	// Splat the length squared
	vLengthSq = VecPermute(vLengthSq, 0, 0, 0, 0);
	// Get the length
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	return vLengthSq;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief normalizes a Vector3
/// @param V the vector to normalize
/// @return the normalized vector
/// @note If the length or the input vector is zero, the result will be a qNaN. 
/// If the length is infinity or the input vector is infinite, the result will be zero.
/// This function does not check for NaN or Inf values in the input vector, as it is assumed that the input vector is a valid finite vector.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline __m128 Vector3Normalize(__m128 V) noexcept
{
	__m128 infinity = {0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000};
	__m128 qNaN = {0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000};

	// Perform the dot product on x,y and z only
	__m128 vLengthSq = _mm_mul_ps(V, V);
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
	vLengthSq = _mm_cmpneq_ps(vLengthSq, infinity);
	// Divide to perform the normalization
	vResult = _mm_div_ps(V, vResult);
	// Any that are infinity, set to zero
	vResult = _mm_and_ps(vResult, vZeroMask);
	// Select qnan or result based on infinite length
	__m128 vTemp1 = _mm_andnot_ps(vLengthSq, qNaN);
	__m128 vTemp2 = _mm_and_ps(vResult, vLengthSq);
	vResult = _mm_or_ps(vTemp1, vTemp2);
	return vResult;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief calculates the cross product of two Vector3s
/// @param V1 the first vector
/// @param V2 the second vector
/// @return the cross product of the two vectors
/// @note The w component of the result will be set to zero.
/// This function does not check for NaN or Inf values in the input vectors, as it is assumed that the input vectors are valid finite vectors.
/// The result will be a valid finite vector if the input vectors are valid finite vectors.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline __m128 Vector3Cross(__m128 V1, __m128 V2)
{
	// y1,z1,x1,w1
	__m128 vTemp1 = VecPermute(V1, 1, 2, 0, 3);
	// z2,x2,y2,w2
	__m128 vTemp2 = VecPermute(V2, 2, 0, 1, 3);
	// Perform the left operation
	__m128 vResult = _mm_mul_ps(vTemp1, vTemp2);
	// z1,x1,y1,w1
	vTemp1 = VecPermute(vTemp1, 1, 2, 0, 3);
	// y2,z2,x2,w2
	vTemp2 = VecPermute(vTemp2, 2, 0, 1, 3);
	// Perform the right operation
	vResult = _mm_sub_ps((vResult), _mm_mul_ps((vTemp1), (vTemp2)));
	// Set w to zero
	__m128i temp = _mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
	__m128 mask3 = _mm_castsi128_ps(temp);
	return _mm_and_ps(vResult, mask3);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief calculates the Scalar Sin and Cos of a value
/// @param pSin pointer to store the sine value
/// @param pCos pointer to store the cosine value
/// @param Value the value to calculate the sine and cosine of
/// @note This function uses a minimax approximation to calculate the sine and cosine values.
/// The sine value is calculated using an 11-degree minimax approximation and the cosine value is calculated using a 10-degree minimax approximation.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ScalarSinCos(float32* pSin, float32* pCos, float32 Value)
{
	assert(pSin);
	assert(pCos);

	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float32 quotient = (1.f / (2.f * PI)) * Value;
	if (Value >= 0.0f)
	{
		quotient = static_cast<float32>(static_cast<int32>(quotient + 0.5f));
	}
	else
	{
		quotient = static_cast<float32>(static_cast<int32>(quotient - 0.5f));
	}
	float32 y = Value - 2.f * PI * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float32 sign;
	if (y > PI/2.f)
	{
		y = PI - y;
		sign = -1.0f;
	}
	else if (y < -PI / 2.f)
	{
		y = -PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float32 y2 = y * y;

	// 11-degree minimax approximation
	*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float32 p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*pCos = sign * p;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief checks if two float values are nearly equal within a given epsilon
/// @param a the first float value
/// @param b the second float value
/// @param epsilon the epsilon value to check against
/// @return true if the two float values are nearly equal, false otherwise
/// @note This function uses the absolute difference between the two float values to check if they are nearly equal.
/// If the absolute difference is less than the epsilon value, the two float values are considered nearly equal.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool NearlyEqual(float32 a, float32 b, float32 epsilon) {
	return Abs(a - b) < epsilon;
}



	/////////////////////////////////////////////////////////////////////
	/// @brief Matrix class for 4x4 matrices
	/// @param m00 the value for row 0, column 0
	/// @param m01 the value for row 0, column 1
	/// [...]
	/// @param m33 the value for row 3, column 3
	/// @return a Matrix object initialized with the specified values
	/////////////////////////////////////////////////////////////////////
	Matrix::Matrix(float32 m00, float32 m01, float32 m02, float32 m03, float32 m10, float32 m11, float32 m12, float32 m13, float32 m20, float32 m21, float32 m22, float32 m23, float32 m30, float32 m31, float32 m32, float32 m33)
	{
		float32 fm[4][4] = { 
			{m00, m01, m02, m03}, 
			{m10, m11, m12, m13}, 
			{m20, m21, m22, m23}, 
			{m30, m31, m32, m33} };
		memcpy(m_matrix, fm, sizeof(float32) * 16);
	}



	/////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for matrix multiplication
	/// @param other the other matrix to multiply with
	/// @return the result of the multiplication
	/// @note This function uses SIMD instructions to perform the multiplication.
	/////////////////////////////////////////////////////////////////////////////////
	Matrix Matrix::operator*(Matrix const& other)
	{
		__m128 row[4];
		__m128 rowOther[4];
		for (int i = 0; i < 4; i++)
		{
			row[i] = _mm_load_ps(m_matrix[i]);
			rowOther[i] = _mm_load_ps(other.m_matrix[i]);
		}

		__m128 result[4];

		float32 resultMatrix[4][4];

		for (int i = 0; i < 4; i++)
		{
			result[i] = _mm_mul_ps(VecPermute(row[i], 0, 0, 0, 0), rowOther[0]);
			result[i] = _mm_add_ps(result[i], _mm_mul_ps(VecPermute(row[i], 1, 1, 1, 1), rowOther[1]));
			result[i] = _mm_add_ps(result[i], _mm_mul_ps(VecPermute(row[i], 2, 2, 2, 2), rowOther[2]));
			result[i] = _mm_add_ps(result[i], _mm_mul_ps(VecPermute(row[i], 3, 3, 3, 3), rowOther[3]));

			_mm_store_ps(resultMatrix[i], result[i]);
		}

		return Matrix(resultMatrix);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for matrix assignment multiplication
	/// @param other the other matrix to multiply with
	/// @return the address result of the multiplication
	/// @note This function uses SIMD instructions to perform the multiplication and assigns the result to the current matrix.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Matrix& Matrix::operator*=(Matrix const& other)
	{
		__m128 row[4];
		__m128 rowOther[4];

		for (int i = 0; i < 4; i++)
		{
			row[i] = _mm_load_ps(m_matrix[i]);
			rowOther[i] = _mm_load_ps(other.m_matrix[i]);
		}

		__m128 result[4];

		for (int i = 0; i < 4; i++)
		{
			result[i] = _mm_mul_ps(VecPermute(row[i], 0, 0, 0, 0), rowOther[0]);
			result[i] = _mm_add_ps(result[i], _mm_mul_ps(VecPermute(row[i], 1, 1, 1, 1), rowOther[1]));
			result[i] = _mm_add_ps(result[i], _mm_mul_ps(VecPermute(row[i], 2, 2, 2, 2), rowOther[2]));
			result[i] = _mm_add_ps(result[i], _mm_mul_ps(VecPermute(row[i], 3, 3, 3, 3), rowOther[3]));

			_mm_store_ps(m_matrix[i], result[i]);
		}

		return *this;
	}

	/////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for matrix equality comparison
	/// @param other the other matrix to compare with
	/// @return true if the matrices are equal, false otherwise
	/// @note This function uses SIMD instructions to perform the comparison.
	/////////////////////////////////////////////////////////////////////////////
	bool Matrix::operator==(Matrix const& other)
	{
		// Load matrix to __m128 to manipulate
		__m128 row0 = _mm_load_ps(m_matrix[0]);
		__m128 row1 = _mm_load_ps(m_matrix[1]);
		__m128 row2 = _mm_load_ps(m_matrix[2]);
		__m128 row3 = _mm_load_ps(m_matrix[3]);

		__m128 otherRow0 = _mm_load_ps(other.m_matrix[0]);
		__m128 otherRow1 = _mm_load_ps(other.m_matrix[1]);
		__m128 otherRow2 = _mm_load_ps(other.m_matrix[2]);
		__m128 otherRow3 = _mm_load_ps(other.m_matrix[3]);

		// Equality masks
		__m128 vTemp1 = _mm_cmpeq_ps(row0, otherRow0);
		__m128 vTemp2 = _mm_cmpeq_ps(row1, otherRow1);
		__m128 vTemp3 = _mm_cmpeq_ps(row2, otherRow2);
		__m128 vTemp4 = _mm_cmpeq_ps(row3, otherRow3);

		// Combine them
		vTemp1 = _mm_and_ps(vTemp1, vTemp2);
		vTemp3 = _mm_and_ps(vTemp3, vTemp4);
		vTemp1 = _mm_and_ps(vTemp1, vTemp3);

		// Compare
		return (_mm_movemask_ps(vTemp1) == 0x0f);
	}

	///////////////////////////////////////////////////////////////////////////////
	/// @brief Overloaded operator for matrix inequality comparison
	/// @param other the other matrix to compare with
	/// @return true if the matrices are not equal, false otherwise
	/// @note This function uses the equality operator to determine inequality.
	///////////////////////////////////////////////////////////////////////////////
	bool Matrix::operator!=(Matrix const& other)
	{
//#ifdef 0
//		// Check row per row
//		__m128 row;
//		__m128 otherRow;
//		for (int i = 0; i < 4; i++)
//		{
//			row = _mm_load_ps(matrix[i]);
//			otherRow = _mm_load_ps(other.matrix[i]);
//			if (_mm_movemask_ps(_mm_cmpeq_ps(row, otherRow)) != 0x0f)
//				return false;
//		}
//		return true;
//		
//#else
		return !this->operator==(other);
	}

	///////////////////////////////////////////////////////////
	/// @brief Overloaded operator for matrix indexing
	/// @param index the index of the row to access
	/// @return a pointer to the row at the specified index
	///////////////////////////////////////////////////////////
	float* Matrix::operator[](int32 index)
	{
		return m_matrix[index];
	}

	//////////////////////////////////////////////////////
	/// @brief Sets the matrix to the specified values
	/// @param m00 the value for row 0, column 0
	/// @param m01 the value for row 0, column 1
	/// @param [...]
	/// @param m33 the value for row 3, column 3
	//////////////////////////////////////////////////////
	void Matrix::Set(float32 m00, float32 m01, float32 m02, float32 m03, float32 m10, float32 m11, float32 m12, float32 m13, float32 m20, float32 m21, float32 m22, float32 m23, float32 m30, float32 m31, float32 m32, float32 m33)
	{
		float32 fm[4][4] = {
			{m00, m01, m02, m03},
			{m10, m11, m12, m13},
			{m20, m21, m22, m23},
			{m30, m31, m32, m33} };
		memcpy(m_matrix, fm, sizeof(float) * 16);
	}
	/////////////////////////////////////////////////////////////
	/// @brief Gets the value at the specified row and column
	/// @param row the row index
	/// @param col the column index
	/// @return the value at the specified row and column
	/////////////////////////////////////////////////////////////
	float32 Matrix::GetValue(int32 row, int32 col) const
	{
		if (row >= 0 && row < 4 || col >= 0 && col < 4)
		{
			return m_matrix[row][col];
		}
		else 
		{
			// Handle out of bounds access
			assert(false && "Matrix access out of bounds");
		}

		return 0.0f;
	}

	/////////////////////////////////////////////////////////////////////
	/// @brief Sets the value at the specified row and column
	/// @param row the row index
	/// @param col the column index
	/// @param value the value to set at the specified row and column
	// @note This function does not check for out of bounds access.
	/////////////////////////////////////////////////////////////////////
	void Matrix::SetValue(int32 row, int32 col, float32 value)
	{
		if (row >= 0 && row < sizeof(m_matrix) || col >= 0 && col < sizeof(m_matrix[0]))
		{
			m_matrix[row][col] = value;
		}
		else
		{
			// Handle out of bounds access
			assert(false && "Matrix access out of bounds");
		}
	}
	
	//////////////////////////////////////////////////////
	/// @brief Sets the matrix to the specified values
	/// @param m00 the value for row 0, column 0
	/// @param m01 the value for row 0, column 1
	/// @param [...]
	/// @param m33 the value for row 3, column 3
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	/// @brief Inverses the matrix.
	/// @return False if the determinant is 0, true otherwise.
	//////////////////////////////////////////////////////////////
	bool Matrix::Inverse()
	{
		Matrix inverseTest = GetInverse();

		// Check if matrix' first element is NaN (Determinant is Zero)
		if (inverseTest.m_matrix[0][0] != inverseTest.m_matrix[0][0])
			return false;

		*this = inverseTest;
		return true;
	}



	//////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Gets the inverse of the matrix
	/// @return the inverse of the matrix
	/// @note This function uses SIMD instructions to calculate the inverse of the matrix.
	//////////////////////////////////////////////////////////////////////////////////////////
	Matrix Matrix::GetInverse() const
	{
		// Load matrix to __m128 to manipulate
		__m128 row0 = _mm_load_ps(m_matrix[0]);
		__m128 row1 = _mm_load_ps(m_matrix[1]);
		__m128 row2 = _mm_load_ps(m_matrix[2]);
		__m128 row3 = _mm_load_ps(m_matrix[3]);

		// Get 4 Sub-Matrices
		__m128 A = VecShuffle_0101(row0, row1);
		__m128 B = VecShuffle_2323(row0, row1);
		__m128 C = VecShuffle_0101(row2, row3);
		__m128 D = VecShuffle_2323(row2, row3);

		// determinant as (|A| |B| |C| |D|)
		__m128 detSub = _mm_sub_ps(
			_mm_mul_ps(VecShuffle(row0, row2, 0, 2, 0, 2), VecShuffle(row1, row3, 1, 3, 1, 3)),
			_mm_mul_ps(VecShuffle(row0, row2, 1, 3, 1, 3), VecShuffle(row1, row3, 0, 2, 0, 2))
		);
		__m128 detA = VecSwizzle1(detSub, 0);
		__m128 detB = VecSwizzle1(detSub, 1);
		__m128 detC = VecSwizzle1(detSub, 2);
		__m128 detD = VecSwizzle1(detSub, 3);

		// let iM = 1/|M| * | X  Y |
		//                  | Z  W |

		// D#C
		__m128 D_C = Mat2AdjMul(D, C);
		// A#B
		__m128 A_B = Mat2AdjMul(A, B);
		// X# = |D|A - B(D#C)
		__m128 X_ = _mm_sub_ps(_mm_mul_ps(detD, A), Mat2Mul(B, D_C));
		// W# = |A|D - C(A#B)
		__m128 W_ = _mm_sub_ps(_mm_mul_ps(detA, D), Mat2Mul(C, A_B));

		// |M| = |A|*|D| + ... (continue later)
		__m128 detM = _mm_mul_ps(detA, detD);

		// Y# = |B|C - D(A#B)#
		__m128 Y_ = _mm_sub_ps(_mm_mul_ps(detB, C), Mat2MulAdj(D, A_B));
		// Z# = |C|B - A(D#C)#
		__m128 Z_ = _mm_sub_ps(_mm_mul_ps(detC, B), Mat2MulAdj(A, D_C));

		// |M| = |A|*|D| + |B|*|C| ... (continue later)
		detM = _mm_add_ps(detM, _mm_mul_ps(detB, detC));

		// tr((A#B)(D#C))
		__m128 tr = _mm_mul_ps(A_B, VecSwizzle(D_C, 0, 2, 1, 3));
		tr = _mm_hadd_ps(tr, tr);
		tr = _mm_hadd_ps(tr, tr);
		// |M| = |A|*|D| + |B|*|C| - tr((A#B)(D#C)
		detM = _mm_sub_ps(detM, tr);

		const __m128 adjSignMask = _mm_setr_ps(1.f, -1.f, -1.f, 1.f);
		// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
		__m128 rDetM = _mm_div_ps(adjSignMask, detM);

		X_ = _mm_mul_ps(X_, rDetM);
		Y_ = _mm_mul_ps(Y_, rDetM);
		Z_ = _mm_mul_ps(Z_, rDetM);
		W_ = _mm_mul_ps(W_, rDetM);

		__m128 r[4];

		// apply adjugate and store, here we combine adjugate shuffle and store shuffle
		r[0] = VecShuffle(X_, Y_, 3, 1, 3, 1);
		r[1] = VecShuffle(X_, Y_, 2, 0, 2, 0);
		r[2] = VecShuffle(Z_, W_, 3, 1, 3, 1);
		r[3] = VecShuffle(Z_, W_, 2, 0, 2, 0);

		return Matrix(r);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Creates a perspective projection matrix with a left-handed coordinate system
	/// @param fovAngleY the field of view angle in radians
	/// @param aspectRatio the aspect ratio of the viewport
	/// @param nearZ the distance to the near clipping plane
	/// @param farZ the distance to the far clipping plane
	/// @return the perspective projection matrix
	/// @note This function uses SIMD instructions to calculate the perspective projection matrix.
	//////////////////////////////////////////////////////////////////////////////////////////////////
	Matrix Matrix::PerspectiveFovLH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ) const
	{
		float32    SinFov;
		float32    CosFov;
		ScalarSinCos(&SinFov, &CosFov, 0.5f * fovAngleY);

		float32 fRange = farZ / (farZ - nearZ);
		// Note: This is recorded on the stack
		float32 Height = CosFov / SinFov;
		__m128 rMem = {
			Height / aspectRatio,
			Height,
			fRange,
			-fRange * nearZ
		};
		// Copy from memory to SSE register
		__m128 vValues = rMem;
		__m128 vTemp = _mm_setzero_ps();
		// Copy x only
		vTemp = _mm_move_ss(vTemp, vValues);
		// Height / aspectRatio,0,0,0
		__m128 M[4];
		M[0] = vTemp;
		// 0,Height,0,0
		vTemp = vValues;
		__m128i temp = _mm_set_epi32(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000);
		__m128 maskY = _mm_castsi128_ps(temp);
		vTemp = _mm_and_ps(vTemp, maskY);
		M[1] = vTemp;
		// x=fRange,y=-fRange * NearZ,0,1.0f
		vTemp = _mm_setzero_ps();
		__m128 IdentityRow4 = { 0.0f, 0.0f, 0.0f, 1.0f };
		vValues = _mm_shuffle_ps(vValues, IdentityRow4, _MM_SHUFFLE(3, 2, 3, 2));
		// 0,0,fRange,1.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
		M[2] = vTemp;
		// 0,0,-fRange * NearZ,0.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
		M[3] = vTemp;
		return M;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Creates an orthographic projection matrix with a left-handed coordinate system
	/// @param viewWidth the width of the view
	/// @param viewHeight the height of the view
	/// @param nearZ the distance to the near clipping plane
	/// @param farZ the distance to the far clipping plane
	/// @return the orthographic projection matrix
	/// @note This function uses SIMD instructions to calculate the orthographic projection matrix.
	///////////////////////////////////////////////////////////////////////////////////////////////////
	Matrix Matrix::OrthographicLH(float32 viewWidth, float32 viewHeight, float32 nearZ, float32 farZ) const
	{
		assert(!NearlyEqual(viewWidth, 0.0f, 0.00001f));
		assert(!NearlyEqual(viewHeight, 0.0f, 0.00001f));
		assert(!NearlyEqual(farZ, nearZ, 0.00001f));

		__m128 M[4];
		float32 fRange = 1.0f / (farZ - nearZ);
		// Note: This is recorded on the stack
		__m128 rMem = {
			2.0f / viewWidth,
			2.0f / viewHeight,
			fRange,
			-fRange * nearZ
		};
		// Copy from memory to SSE register
		__m128 vValues = rMem;
		__m128 vTemp = _mm_setzero_ps();
		// Copy x only
		vTemp = _mm_move_ss(vTemp, vValues);
		// 2.0f / ViewWidth,0,0,0
		M[0] = vTemp;
		// 0,2.0f / ViewHeight,0,0
		vTemp = vValues;
		__m128i temp = _mm_set_epi32(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000);
		__m128 maskY = _mm_castsi128_ps(temp);
		vTemp = _mm_and_ps(vTemp, maskY);
		M[1] = vTemp;
		// x=fRange,y=-fRange * NearZ,0,1.0f
		vTemp = _mm_setzero_ps();
		__m128 IdentityRow4 = { 0.0f, 0.0f, 0.0f, 1.0f };
		vValues = _mm_shuffle_ps(vValues, IdentityRow4, _MM_SHUFFLE(3, 2, 3, 2));
		// 0,0,fRange,0.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
		M[2] = vTemp;
		// 0,0,-fRange * NearZ,1.0f
		vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
		M[3] = vTemp;
		return M;
	}

	////////////////////////////////////////
	/// @brief Transposes a given Matrix
	////////////////////////////////////////
	void Matrix::Transpose()
	{
		*this = GetTransposed();
		return;
	}



	////////////////////////////////////////////////////////
	/// @brief Gets the Transposed matrix of this Matrix
	/// @return the transposed matrix
	////////////////////////////////////////////////////////
	Matrix Matrix::GetTransposed() const
	{
		// Load matrix to __m128 to manipulate
		__m128 row0 = _mm_load_ps(m_matrix[0]);
		__m128 row1 = _mm_load_ps(m_matrix[1]);
		__m128 row2 = _mm_load_ps(m_matrix[2]);
		__m128 row3 = _mm_load_ps(m_matrix[3]);

		// x.x,x.y,y.x,y.y
		__m128 vTemp1 = VecShuffle(row0, row1, 0, 1, 0, 1);
		// x.z,x.w,y.z,y.w
		__m128 vTemp3 = VecShuffle(row0, row1, 2, 3, 2, 3);
		// z.x,z.y,w.x,w.y
		__m128 vTemp2 = VecShuffle(row2, row3, 0, 1, 0, 1);
		// z.z,z.w,w.z,w.w
		__m128 vTemp4 = VecShuffle(row2, row3, 2, 3, 2, 3);

		__m128 mResult[4];
		// x.x,y.x,z.x,w.x
		mResult[0] = VecShuffle(vTemp1, vTemp2, 0, 2, 0, 2);
		// x.y,y.y,z.y,w.y
		mResult[1] = VecShuffle(vTemp1, vTemp2, 1, 3, 1, 3);
		// x.z,y.z,z.z,w.z
		mResult[2] = VecShuffle(vTemp3, vTemp4, 0, 2, 0, 2);
		// x.w,y.w,z.w,w.w
		mResult[3] = VecShuffle(vTemp3, vTemp4, 1, 3, 1, 3);
		return Matrix(mResult);
	}



	///////////////////////////////////////////////////////////////////////
	/// @brief Checks if the determinant of the matrix is equal to zero
	/// @return true if the determinant is zero, false otherwise
	///////////////////////////////////////////////////////////////////////
	bool Matrix::IsDeterminantZero() const
	{
		return GetDeterminant() == 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	/// @brief Decomposes the matrix into translation, scale, and rotation components
	/// @param translation the translation component of the matrix
	/// @param scale the scale component of the matrix
	/// @param rotation the rotation component of the matrix
	/// @return true if the decomposition was successful, false otherwise
	/// @note This function uses SIMD instructions to perform the decomposition.
	/////////////////////////////////////////////////////////////////////////////////////
	bool Matrix::Decompose(Vector3f32& translation, Vector3f32& scale, Quaternion& rotation) const
	{
		static __m128 const identityR0 = { 1.0f, 0.0f, 0.0f, 0.0f };
		static __m128 const identityR1 = { 0.0f, 1.0f, 0.0f, 0.0f };
		static __m128 const identityR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
		static __m128 const identityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };

		static const __m128* pvCanonicalBasis[3] = {
		&identityR0,
		&identityR1,
		&identityR2
		};

		// Load matrix to __m128 to manipulate
		__m128 row0 = _mm_load_ps(this->m_matrix[0]);
		__m128 row1 = _mm_load_ps(this->m_matrix[1]);
		__m128 row2 = _mm_load_ps(this->m_matrix[2]);
		__m128 row3 = _mm_load_ps(this->m_matrix[3]);

		// Get the translation
		translation.x = m_matrix[3][0];
		translation.y = m_matrix[3][1];
		translation.z = m_matrix[3][2];

		__m128* ppvBasis[3];
		__m128 matTemp[4];
		ppvBasis[0] = &matTemp[0];
		ppvBasis[1] = &matTemp[1];
		ppvBasis[2] = &matTemp[2];

		matTemp[0] = row0;
		matTemp[1] = row1;
		matTemp[2] = row2;
		matTemp[3] = identityR3;

		float* pfScales = (float*)malloc(4 * sizeof(float));
		pfScales[0] = scale.x;
		pfScales[1] = scale.y;
		pfScales[2] = scale.z;

		size_t a, b, c;
		_mm_store_ss(&pfScales[0], Vector3Length(ppvBasis[0][0]));
		_mm_store_ss(&pfScales[1], Vector3Length(ppvBasis[1][0]));
		_mm_store_ss(&pfScales[2], Vector3Length(ppvBasis[2][0]));
		pfScales[3] = 0.f;

		RankDecompose(a, b, c, pfScales[0], pfScales[1], pfScales[2])

			if (pfScales[a] < DecomposeEpsilon)
			{
				ppvBasis[a][0] = pvCanonicalBasis[a][0];
			}
		ppvBasis[a][0] = Vector3Normalize(ppvBasis[a][0]);

		if (pfScales[b] < DecomposeEpsilon)
		{
			size_t aa, bb, cc;
			float32 fAbsX, fAbsY, fAbsZ;

			fAbsX = std::fabsf(ppvBasis[a][0].m128_f32[0]);
			fAbsY = std::fabsf(ppvBasis[a][0].m128_f32[1]);
			fAbsZ = std::fabsf(ppvBasis[a][0].m128_f32[2]);

			RankDecompose(aa, bb, cc, fAbsX, fAbsY, fAbsZ)

				ppvBasis[b][0] = Vector3Cross(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
		}

		ppvBasis[b][0] = Vector3Normalize(ppvBasis[b][0]);

		if (pfScales[c] < DecomposeEpsilon)
		{
			ppvBasis[c][0] = Vector3Cross(ppvBasis[a][0], ppvBasis[b][0]);
		}

		ppvBasis[c][0] = Vector3Normalize(ppvBasis[c][0]);

		Matrix temp = matTemp;
		float32 fDet = temp.GetDeterminant();

		// use Kramer's rule to check for handedness of coordinate system
		if (fDet < 0.0f)
		{
			// switch coordinate system by negating the scale and inverting the basis vector on the x-axis
			pfScales[a] = -pfScales[a];
			ppvBasis[a][0] = _mm_sub_ps(_mm_setzero_ps(), ppvBasis[a][0]);

			fDet = -fDet;
		}

		fDet -= 1.0f;
		fDet *= fDet;

		if (DecomposeEpsilon < fDet)
		{
			// Non-SRT matrix encountered
			return false;
		}

		// generate the quaternion from the matrix
		rotation = Quaternion::RotationMatrix(Matrix(matTemp));
		return true;
	}


	/////////////////////////////////////////
	/// @brief Creates an identity matrix
	/// @return the identity matrix
	/////////////////////////////////////////
	Matrix Matrix::Identity()
	{
		return Matrix();
	}

	/////////////////////////////////////////////////////////////////////////
	/// @brief Checks if the matrix is an identity matrix
	/// @param m the matrix to check
	/// @return true if the matrix is an identity matrix, false otherwise
	/// @note This function uses SIMD instructions to perform the check.
	/////////////////////////////////////////////////////////////////////////
	bool Matrix::IsIdentity(Matrix const& m)
	{
		static const __m128 identityR0 = {1.0f, 0.0f, 0.0f, 0.0f};
		static const __m128 identityR1 = {0.0f, 1.0f, 0.0f, 0.0f};
		static const __m128 identityR2 = {0.0f, 0.0f, 1.0f, 0.0f};
		static const __m128 identityR3 = {0.0f, 0.0f, 0.0f, 1.0f};

		// Load matrix to __m128 to manipulate
		__m128 row0 = _mm_load_ps(m.m_matrix[0]);
		__m128 row1 = _mm_load_ps(m.m_matrix[1]);
		__m128 row2 = _mm_load_ps(m.m_matrix[2]);
		__m128 row3 = _mm_load_ps(m.m_matrix[3]);

		__m128 vTemp1 = _mm_cmpeq_ps(row0, identityR0);
		__m128 vTemp2 = _mm_cmpeq_ps(row1, identityR1);
		__m128 vTemp3 = _mm_cmpeq_ps(row2, identityR2);
		__m128 vTemp4 = _mm_cmpeq_ps(row3, identityR3);
		vTemp1 = _mm_and_ps(vTemp1, vTemp2);
		vTemp3 = _mm_and_ps(vTemp3, vTemp4);
		vTemp1 = _mm_and_ps(vTemp1, vTemp3);
		return (_mm_movemask_ps(vTemp1) == 0x0f);
	}

	////////////////////////////////////////////
	/// @brief Gets the Matrix' determinant
	/// @return Float32: Matrix' determinant
	////////////////////////////////////////////
	float32 Matrix::GetDeterminant() const
	{
		static const float32 Sign[4] = { 1.0f, -1.0f, 1.0f, -1.0f };
		__m128 signVec = _mm_load_ps(Sign);

		// Load matrix to __m128 to manipulate
		__m128 row0 = _mm_load_ps(m_matrix[0]);
		__m128 row1 = _mm_load_ps(m_matrix[1]);
		__m128 row2 = _mm_load_ps(m_matrix[2]);
		__m128 row3 = _mm_load_ps(m_matrix[3]);

		__m128 V0 = VecSwizzle(row2, 1, 0, 0, 0);
		__m128 V1 = VecSwizzle(row3, 2, 2, 1, 1);
		__m128 V2 = VecSwizzle(row2, 1, 0, 0, 0);
		__m128 V3 = VecSwizzle(row3, 3, 3, 3, 2);
		__m128 V4 = VecSwizzle(row2, 2, 2, 1, 1);
		__m128 V5 = VecSwizzle(row3, 3, 3, 3, 2);

		__m128 P0 = _mm_mul_ps(V0, V1);
		__m128 P1 = _mm_mul_ps(V2, V3);
		__m128 P2 = _mm_mul_ps(V4, V5);

		V0 = VecSwizzle(row2, 2, 2, 1, 1);
		V1 = VecSwizzle(row3, 1, 0, 0, 0);
		V2 = VecSwizzle(row2, 3, 3, 3, 2);
		V3 = VecSwizzle(row3, 1, 0, 0, 0);
		V4 = VecSwizzle(row2, 3, 3, 3, 2);
		V5 = VecSwizzle(row3, 2, 2, 1, 1);

		P0 = _mm_sub_ps(P0, _mm_mul_ps(V0, V1));
		P1 = _mm_sub_ps(P1, _mm_mul_ps(V2, V3));
		P2 = _mm_sub_ps(P2, _mm_mul_ps(V4, V5));

		V0 = VecSwizzle(row1, 3, 3, 3, 2);
		V1 = VecSwizzle(row1, 2, 2, 1, 1);
		V2 = VecSwizzle(row1, 1, 0, 0, 0);

		__m128 S = _mm_mul_ps(row0, signVec);
		__m128 R = _mm_mul_ps(V0, P0);
		R = _mm_sub_ps(R, _mm_mul_ps(V1, P1));
		R = _mm_add_ps(_mm_mul_ps((V2), (P2)), (R));

		// Vector4 Dot S/R
		__m128 vTemp2 = R;
		__m128 vTemp = _mm_mul_ps(S, vTemp2);
		vTemp2 = VecShuffle(vTemp2, vTemp, 0, 0, 0, 1); // Copy X to the Z position and Y to the W position
		vTemp2 = _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
		vTemp = VecShuffle(vTemp, vTemp2, 0, 0, 3, 0);  // Copy W to the Z position
		vTemp = _mm_add_ps(vTemp, vTemp2);           // Add Z and W together

		return vTemp.m128_f32[2];
	}

	Matrix Matrix::Translation(Vector3f32 const& translation)
	{
		return Matrix(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		translation.x, translation.y, translation.z, 1.f );
	}

	Matrix Matrix::Scale(Vector3f32 const& scale)
	{
		return Matrix(
		 scale.x, 0, 0, 0,
		 0, scale.y, 0, 0,
		 0, 0, scale.z, 0,
		 0, 0, 0, 1  );
	}
	

	///////////////////////////////////////////////////////////
	/// @brief Creates a rotation matrix from a quaternion.
	/// @param Quaternion taken as model
	/// @return Matrix created
	///////////////////////////////////////////////////////////
	Matrix Matrix::Rotation(Quaternion const& quat)
	{
		static const __m128  constant1110 = { 1.0f, 1.0f, 1.0f, 0.0f };
		__m128i temp = _mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		__m128 mask3 = _mm_castsi128_ps(temp);

		// Load quaternion to __m128 to manipulate
		__m128 quaternion = _mm_load_ps(quat.pdata);

		__m128 Q0 = _mm_add_ps(quaternion, quaternion);
		__m128 Q1 = _mm_mul_ps(quaternion, Q0);

		__m128 V0 = VecPermute(Q1, 1, 0, 0, 3);
		V0 = _mm_and_ps(V0, mask3);
		__m128 V1 = VecPermute(Q1, 2, 2, 1, 3);
		V1 = _mm_and_ps(V1, mask3);
		__m128 R0 = _mm_sub_ps(constant1110, V0);
		R0 = _mm_sub_ps(R0, V1);

		V0 = VecPermute(quaternion, 0, 0, 1, 3);
		V1 = VecPermute(Q0, 2, 1, 2, 3);
		V0 = _mm_mul_ps(V0, V1);

		V1 = VecPermute(quaternion, 3, 3, 3, 3);
		__m128 V2 = VecPermute(Q0, 1, 2, 0, 3);
		V1 = _mm_mul_ps(V1, V2);

		__m128 R1 = _mm_add_ps(V0, V1);
		__m128 R2 = _mm_sub_ps(V0, V1);

		V0 = VecShuffle(R1, R2, 1, 2, 0, 1);
		V0 = VecPermute(V0, 0, 2, 3, 1);
		V1 = VecShuffle(R1, R2, 0, 0, 2, 2);
		V1 = VecPermute(V1, 0, 2, 0, 2);

		Q1 = VecShuffle(R0, V0, 0, 3, 0, 1);
		Q1 = VecPermute(Q1, 0, 2, 3, 1);

		__m128 M[4];
		M[0] = Q1;

		Q1 = VecShuffle(R0, V0, 1, 3, 2, 3);
		Q1 = VecPermute(Q1, 2, 0, 3, 1);
		M[1] = Q1;

		Q1 = VecShuffle(V1, R0, 0, 1, 2, 3);
		M[2] = Q1;
		M[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
		return M;
	}

	Matrix Matrix::LookAtLH(Vector3f32 const& eyePosition, Vector3f32 const& focusPosition, Vector3f32 const& up)
	{

		Vector3f32 eyeDirection = focusPosition - eyePosition;
		return LookToLH(eyePosition, eyeDirection, up);
	}

	Matrix Matrix::LookToLH(Vector3f32 const& eyePosition, Vector3f32 const& eyeDirection, Vector3f32 const& up)
	{
		assert(eyeDirection != Vector3f32( 0.0f, 0.0f, 0.0f));
		// TODO: ONCE SSE VECTOR3F32 IS APPROVED vvv
		/*assert(!XMVector3IsInfinite(EyeDirection));
		assert(!XMVector3Equal(UpDirection, XMVectorZero()));
		assert(!XMVector3IsInfinite(UpDirection));*/

		Vector3f32 r2 = eyeDirection.Normalize();
		Vector3f32 uCrossR2 = up.CrossProduct(r2);
		float32 uCrossR2F[3] = { uCrossR2.x, uCrossR2.y, uCrossR2.z };

		Vector3f32 r0 = uCrossR2.Normalize();
		Vector3f32 r1 = r2.CrossProduct(r0);

		Vector3f32 NegEyePosition = -eyePosition;

		float d0 = r0.DotProduct(NegEyePosition);
		float d1 = r1.DotProduct(NegEyePosition);
		float d2 = r2.DotProduct(NegEyePosition);

		__m128 m[4];

		__m128i temp = _mm_set_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
		__m128 mask3 = _mm_castsi128_ps(temp);
		
		// TODO: vvv THIS PART IS TO CHANGE ONCE VECTOR3F32 SSE IS APPROVED vvv 
		float d0F[3] = { d0 };
		float d1F[3] = { d1 };
		float d2F[3] = { d2 };
		__m128 d0M128 = _mm_load_ps(d0F);
		__m128 d1M128 = _mm_load_ps(d1F);
		__m128 d2M128 = _mm_load_ps(d2F);

		float r0F[3] = { r0.x, r0.y, r0.z };
		float r1F[3] = { r1.x, r1.y, r1.z };
		float r2F[3] = { r2.x, r2.y, r2.z };
		__m128 r0M128 = _mm_load_ps(r0F);
		__m128 r1M128 = _mm_load_ps(r1F);
		__m128 r2M128 = _mm_load_ps(r2F);

		__m128 vTemp1 = _mm_andnot_ps(mask3, d0M128);
		__m128 vTemp2 = _mm_and_ps(r0M128, mask3);
		m[0] = _mm_or_ps(vTemp1, vTemp2);

		vTemp1 = _mm_andnot_ps(mask3, d0M128);
		vTemp2 = _mm_and_ps(r0M128, mask3);
		m[0] = _mm_or_ps(vTemp1, vTemp2);
		vTemp1 = _mm_andnot_ps(mask3, d1M128);
		vTemp2 = _mm_and_ps(r1M128, mask3);
		m[1] = _mm_or_ps(vTemp1, vTemp2);
		vTemp1 = _mm_andnot_ps(mask3, d2M128);
		vTemp2 = _mm_and_ps(r2M128, mask3);
		m[2] = _mm_or_ps(vTemp1, vTemp2);
		m[3] = { 0, 0, 0, 1 };
		// ^^^ Part over this comment is temporary ^^^

		Matrix result = m;
		result.Transpose();
		return result;
	}

	///////////////////////////////////////////////////////////
	/// @brief Default constructor for the Matrix class
	/// @return an identity matrix
	/// @note Initializes the matrix to an identity matrix.
	///////////////////////////////////////////////////////////
	Matrix::Matrix(float32 matrix[4][4])
	{
		memcpy(this->m_matrix, matrix, sizeof(float) * 16);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	/// @brief Constructor for the Matrix class with __m128 parameters
	/// @param matrix an array of 4 __m128 values representing the rows of the matrix
	/// @return a Matrix object initialized with the provided __m128 values
	/// @note This constructor initializes the matrix using the provided __m128 values.
	///////////////////////////////////////////////////////////////////////////////////////
	__vectorcall Matrix::Matrix(__m128 matrix[4])
	{
		_mm_store_ps(this->m_matrix[0], matrix[0]);
		_mm_store_ps(this->m_matrix[1], matrix[1]);
		_mm_store_ps(this->m_matrix[2], matrix[2]);
		_mm_store_ps(this->m_matrix[3], matrix[3]);
	}

}


