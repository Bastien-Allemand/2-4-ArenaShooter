#ifndef TEST_MATHFUNCTIONS_HPP_INCLUDED
#define TEST_MATHFUNCTIONS_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( MathFunctions )

    TestFactorial();
    TestPow();
    TestNormalizeAngle();
    TestSin();
    TestCos();
    TestTan();
    TestDist();
    TestMaxMin();
    TestSqrt();
    TestClamp();

    std::cout << "All MathsFunctions tests passed.\n";

END_TEST_MAIN_FUNCTION( MathFunctions )

static void TestFactorial() 
{
    assert(Factorial(0) == 1);
    assert(Factorial(5) == 120);
}

static void TestPow() 
{
    assert(Pow(2, 0), 1);
    assert(Pow(2, 3), 8);
}

static void TestNormalizeAngle() 
{
    float32 angle = NormalizeAngle(3 * PI);
    assert(angle > -PI && angle <= PI);
}

static void TestSin() 
{
    float32 result = Sin(PI / 2);
    assert(result, 1.0f);
}

static void TestCos() 
{
    float32 result = Cos(0);
    assert(result, 1.0f);
}

static void TestTan() 
{
    float32 result = Tan(PI / 4);
    assert(result, 1.0f);
}

static void TestDist() 
{
    assert(Dist(10, 3), 7);
    assert(Dist(3, 10), -7);
}

static void TestMaxMin() 
{
    assert(Max(10, 20) == 20);
    assert(Min(10, 20) == 10);
}

static void TestSqrt() 
{
    assert(Sqrt(4), 2);
    assert(Sqrt(9), 3);
}

static void TestClamp()
{
    assert(Clamp(0.5f, 0.0f, 1.0f) == 0.5f);
    assert(Clamp(-1.0f, 0.0f, 1.0f) == 0.0f);
}

END_TEST( MathFunctions )


#endif