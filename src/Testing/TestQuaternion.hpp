#ifndef TEST_QUATERNION_HPP_INCLUDED
#define TEST_QUATERNION_HPP_INCLUDED

#include <cassert>
#include <iostream>
#include <Maths.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Quaternion )

    TestQuaternionConstructors();
    std::cout << "Quaternion constructors test passed successfully.\n";
    TestQuaternionEquality();
    std::cout << "Quaternion equality test passed successfully.\n";
    TestQuaternionMultiplication();
    std::cout << "Quaternion multiplication test passed successfully.\n";
    TestAssignmentAndCompoundMul();
    std::cout << "Quaternion assignment and compound multiplication test passed successfully.\n";
    TestNormalization();
    std::cout << "Quaternion normalization test passed successfully.\n";
    TestGetInverse();
    std::cout << "Quaternion inverse test passed successfully.\n";
    TestConjugate();
    std::cout << "Quaternion conjugate test passed successfully.\n";
    TestRotationAxis();
    std::cout << "Quaternion rotation axis test passed successfully.\n";
    TestRotationEuler();
    std::cout << "Quaternion rotation Euler test passed successfully.\n";
    TestToAxisAngle();
    std::cout << "Quaternion to axis-angle conversion test passed successfully.\n";
    TestIdentity();
    std::cout << "Quaternion identity test passed successfully.\n";
    TestRotationMatrix();
    std::cout << "Quaternion rotation matrix test passed successfully.\n";
    TestSquareNormAndNorm();
    std::cout << "Quaternion square norm and norm test passed successfully.\n\n";
    std::cout << "All tests passed.\n";

END_TEST_MAIN_FUNCTION( Quaternion )

static bool NearlyEqual(float32 const a, float32 const b) { return std::fabs(a - b) < EPSILON; }

static void Print(const Quaternion& q, const std::string& label)
{
    std::cout << label << "(" << q.GetX() << ", " << q.GetY() << ", " << q.GetZ() << ", " << q.GetW() << ")\n";
}

static void TestQuaternionConstructors()
{
    Quaternion q1;
    assert(q1.GetX() == 0.f && q1.GetY() == 0.f && q1.GetZ() == 0.f && q1.GetW() == 1.f);

    Quaternion q2(1.f, 2.f, 3.f, 4.f);
    assert(q2.GetX() == 1.f && q2.GetY() == 2.f && q2.GetZ() == 3.f && q2.GetW() == 4.f);

    Quaternion q3(q2);
    assert(q3.GetX() == 1.f && q3.GetY() == 2.f && q3.GetZ() == 3.f && q3.GetW() == 4.f);
}

static void TestQuaternionEquality()
{
    Quaternion q1(0.f, 0.f, 0.f, 1.f);
    Quaternion q2;
    assert(q1 == q2);

    Quaternion q3(1.f, 0.f, 0.f, 0.f);
    assert(q1 != q3);
}

static void TestQuaternionMultiplication()
{
    Quaternion q1(0.f, 0.f, 0.f, 1.f);
    Quaternion q2(1.f, 0.f, 0.f, 0.f);
    Quaternion q3 = q1 * q2;
    assert(NearlyEqual(q3.GetX(), 1) && NearlyEqual(q3.GetY(), 0) &&
        NearlyEqual(q3.GetZ(), 0) && NearlyEqual(q3.GetW(), 0));
}

static void TestAssignmentAndCompoundMul()
{
    Quaternion q1(0.f, 0.f, 1.f, 0.f);
    Quaternion q2(0.f, 1.f, 0.f, 0.f);
    Quaternion expected = q2 * q1;
    q2 *= q1;
    assert(q2 == expected); // associative in special cases
}

static void TestNormalization()
{
    Quaternion q(1.f, 2.f, 3.f, 4.f);
    q.Normalize();
    assert(NearlyEqual(q.Norm(), 1.0f));
}

static void TestGetInverse()
{
    Quaternion q(0, 0, std::sqrt(0.5f), std::sqrt(0.5f)); // 90° around Z
    Quaternion inv = q.GetInverse();
    Quaternion result = q * inv;
    assert(result.IsIdentity());
}

static void TestConjugate()
{
    Quaternion q(1.f, 2.f, 3.f, 4.f);
    q.Conjugate();
    assert(NearlyEqual(q.GetX(), -1));
    assert(NearlyEqual(q.GetY(), -2));
    assert(NearlyEqual(q.GetZ(), -3));
    assert(NearlyEqual(q.GetW(), 4));
}

static void TestRotationAxis()
{
    Vector3f32 axis = { 0.f, 1.f, 0.f };
    Quaternion q;
    q.SetRotationAxis(axis, PI/2);
    assert(!q.IsNormZero());
}

static void TestRotationEuler()
{
    Quaternion q;
    q.SetRotationEuler(0, PI/2, 0);
    assert(!q.IsNormZero());
}

static void TestToAxisAngle()
{
    Vector3f32 axis = { 0.f, 1.f, 0.f };
    axis = axis.Normalize();
    Quaternion q;
    q.SetRotationAxis(axis, PI/2);
    float angle = 0;
    q.QuaternionToAxisAngle(axis, angle);
    axis = axis.Normalize();
    assert(NearlyEqual(angle, PI/2));
    assert(NearlyEqual(axis.y, 1.0f));
}

static void TestIdentity()
{
    Quaternion q;
    assert(q.IsIdentity());
}

static void TestSquareNormAndNorm()
{
    Quaternion q(10.f, 8.f, 3.6f, 4.f);
    float norm = q.Norm();
    float sqNorm = q.SquareNorm();
    assert(std::fabs(norm * norm - sqNorm) < 1.f);
}

static void TestRotationMatrix()
{
    Quaternion q;
    Matrix mat({ 0.866f, 0.5f, 0.f, 0.f, -0.5f, 0.866f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    q.SetRotationMatrix(mat);
    assert(NearlyEqual(q.GetZ(), 0.25882f));
    assert(NearlyEqual(q.GetW(), 0.96592f));
}

END_TEST( Quaternion )


#endif