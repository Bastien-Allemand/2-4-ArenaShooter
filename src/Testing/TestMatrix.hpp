#ifndef TEST_MATRIX_HPP_INCLUDED
#define TEST_MATRIX_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Matrix )

	Matrix m1 = Matrix::Identity();
    Matrix m2 = Matrix();
    Matrix result = m1 * m2;
    assert(Matrix::IsIdentity(result));
    std::string resultString = Matrix::IsIdentity(result) ? "is Identity Matrix.\n\n" : "is not Identity Matrix.\n\n";
    std::cout << "The first test is a success. The Matrix result " << resultString;

    m1[0][3] = 12;
	std::cout << "The matrix value at the index [0][3] has been successfully set to 12.\n";
    assert(m1.GetValue(0, 3) == 12);
	std::string resultString2 = m1.GetValue(0, 3) == 12 ? "is correct.\n\n" : "is incorrect.\n\n";
	std::cout << "The matrix value at the index [0][3] " << resultString2;

    Matrix m3 = Matrix(
        2, 3, 2, 6,
        7, 1, 1, 5,
        6, 5, 3, 2,
        1, 1, 0, 5);

    Matrix m4;
    
    m4.Set(
        1, 1, 0, 1,
        0, 0, 1, 0,
        1, 0, 0, 0,
        0, 0, 0, 0);
    m3 *= m4;

    Matrix testM3TimesM4(
        4, 2, 3, 2,
        8, 7, 1, 7,
        9, 6, 5, 6,
        1, 1, 1, 1);

    assert(m3 == testM3TimesM4);
	std::string resultString3 = m3 == testM3TimesM4 ? "is equal to the expected result.\n\n" : "is not equal to the expected result.\n\n";
	std::cout << "The matrix m3 * m4 " << resultString3;

    Matrix m5(
        0, 5, 0, 0,
        7, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);

    Matrix transposed = m5.GetTransposed();
    assert(transposed.GetValue(0, 1) == 7);
	std::string resultString4 = transposed.GetValue(0, 1) == 7 ? "is correct.\n\n" : "is incorrect.\n\n";
	std::cout << "The transposed matrix value at the index [0][1] " << resultString4;
    assert(transposed.GetValue(1, 0) == 5);
	std::string resultString5 = transposed.GetValue(1, 0) == 5 ? "is correct.\n\n" : "is incorrect.\n\n";
	std::cout << "The transposed matrix value at the index [1][0] " << resultString5;

    assert(m5[0][1] == 5);
	std::string resultString6 = m5[0][1] == 5 ? "is correct.\n\n" : "is incorrect.\n\n";
	std::cout << "The matrix value at the index [0][1] " << resultString6;
    assert(m5.GetValue(1, 0) == 7);
	std::string resultString7 = m5.GetValue(1, 0) == 7 ? "is correct.\n\n" : "is incorrect.\n\n";
	std::cout << "The matrix value at the index [1][0] " << resultString7;

    m5.Transpose();
    assert(m5.GetValue(0, 1) == 7);
	std::string resultString8 = m5.GetValue(0, 1) == 7 ? "is correct.\n\n" : "is incorrect.\n\n";
	std::cout << "The transposed matrix value at the index [0][1] " << resultString8;
    assert(m5.GetValue(1, 0) == 5);
	std::string resultString9 = m5.GetValue(1, 0) == 5 ? "is correct.\n\n" : "is incorrect.\n\n";
	std::cout << "The transposed matrix value at the index [1][0] " << resultString9;


    Matrix m6(
        1, 1, 0, 2,
        0, 0, 1, 1,
        1, 0, 1, 0,
        1, 0, 0, 0);

    Matrix m6Copy = m6;
    Matrix m6Inverse(
        0, 0, 0, 1,
        1, -2, 2, -3,
        0, 0, 1, -1,
        0, 1, -1, 1
    );

    Matrix m7 = m6.GetInverse();

    assert(m7 == m6Inverse);
	assert(m6 != m7);
	std::cout << "The matrix m6 inverse is correct.\n\n";

    assert(m6 == m6Copy);
	std::cout << "The matrix m6 is equal to its copy.\n\n";

    assert(m6.GetDeterminant() == 1);
	std::string stringResult10 = m6.GetDeterminant() == 1 ? "is correct.\n" : "is incorrect.\n";
	std::cout << "The determinant of matrix m6 " << stringResult10;
    assert(m6.IsDeterminantZero() == false);
	std::cout << "The matrix m6 does not have a determinant of zero.\n\n";

    m6.Inverse();

    assert(m6 != m6Copy);
	std::cout << "The matrix m6 is not equal to its copy after inversion.\n";
    assert(m6 == m6Inverse);
	std::cout << "The matrix m6 is equal to its inverse after inversion.\n\n";

    Matrix m8(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 0);

    assert(m8.IsDeterminantZero() == true);
	std::cout << "The matrix m8 has a determinant of zero.\n";
    assert(m8.Inverse() == false);
	std::cout << "The matrix m8 cannot be inverted as it has a determinant of zero.\n\n";

    Matrix m9(2, 1, 3, 0, 4, 6, 5, 1, 2, 3, 4, 4, 5, 1, 2, 1);

END_TEST_MAIN_FUNCTION( Matrix )
END_TEST( Matrix )


#endif