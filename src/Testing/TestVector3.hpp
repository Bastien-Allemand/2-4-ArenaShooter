#ifndef TEST_VECTOR3_HPP_INCLUDED
#define TEST_VECTOR3_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include <Maths/Vector3.h>
#include <Testing.h>
#include "define.h"
using namespace gce;


BEGIN_TEST(Vector3)

    Console::Log("-------------- Vector3f32 --------------");
    
    Console::Log("\n---------- Assignment Operators ----------\n");
    SelfTest<Vector3f32 const&>("operator-=             ", &Vector3f32::operator-=, { 5.f, 5.f, 5.f }, { 4.f, 3.f, 2.f }, { 1.f, 2.f, 3.f });
    SelfTest<Vector3f32 const&>("operator+=             ", &Vector3f32::operator+=, { 1.f, 2.f, 3.f }, { 1.f, 2.f, 3.f }, { 2.f, 4.f, 6.f });
    SelfTest<Vector3f32 const&>("operator*= (vector3)   ", &Vector3f32::operator*=, { 3.f, 1.f, 2.f }, { 2.f, 3.f, 2.f }, { 6.f, 3.f, 4.f });
    SelfTest<Vector3f32 const&>("operator/= (vector3)   ", &Vector3f32::operator/=, { 6.f, 1.f, 12.f }, { 2.f, 1.f, 3.f }, { 3.f, 1.f, 4.f });
    SelfTest<float32>("operator*= (scalar)    ", &Vector3f32::operator*=, { 2.f, 1.f, 4.f }, 3.f, { 6.f, 3.f, 12.f });
    SelfTest<float32>("operator/= (scalar)    ", &Vector3f32::operator/=, { 0.f, 2.f, 8.f }, 2.f, { 0.f, 1.f, 4.f });

    Console::Log("\n---------- Comparison Operators ----------\n");
    ReturnTest<Vector3f32 const&>("operator== (True)      ", &Vector3f32::operator==, { 5.f, 2.f, 5.f }, { 5.f, 2.f, 5.f }, true);
    ReturnTest<Vector3f32 const&>("operator== (False)     ", &Vector3f32::operator==, { 5.f, 2.f, 5.f }, { 1.f, 0.f, 1.f }, false);
    ReturnTest<Vector3f32 const&>("operator!= (True)      ", &Vector3f32::operator!=, { 5.f, 2.f, 5.f }, { 2.f, 2.f, 5.f }, true);
    ReturnTest<Vector3f32 const&>("operator!= (False)     ", &Vector3f32::operator!=, { 0.f, 1.f, 2.f }, { 0.f, 1.f, 2.f }, false);

    Console::Log("\n------------- Basic Operators -------------\n");
    ReturnTest<Vector3f32 const&>("operator-              ", &Vector3f32::operator-, { 5.f, 5.f, 5.f }, { 4.f, 3.f, 2.f }, { 1.f, 2.f, 3.f });
    ReturnTest<Vector3f32 const&>("operator+              ", &Vector3f32::operator+, { 1.f, 2.f, 3.f }, { 4.f, 3.f, 2.f }, { 5.f, 5.f, 5.f });
    ReturnTest<Vector3f32 const&>("operator* (vector3)    ", &Vector3f32::operator*, { 2.f, 5.f, 3.f }, { 2.f, 0.f, 1.f }, { 4.f, 0.f, 3.f });
    ReturnTest<Vector3f32 const&>("operator/ (vector3)    ", &Vector3f32::operator/, { 8.f, 0.f, 3.f }, { 4.f, 1.f, 2.f }, { 2.f, 0.f, 3.f / 2.f });
    ReturnTest<float32>("operator* (scalar)     ", &Vector3f32::operator*, { 2.f, 2.f, 2.f }, 2.f, { 4.f, 4.f, 4.f });
    ReturnTest<float32>("operator/ (scalar)     ", &Vector3f32::operator/, { 8.f, 0.f, 3.f }, 2.f, { 4.f, 0.f, 3.f / 2.f });

    Console::Log("\n---------------- Accessors ----------------\n");
    ReturnTest<int8>( "operator[]             ", &Vector3f32::operator[], { 1.0f, 2.0f, 3.0f }, 2, 3.0f );

    Console::Log("\n------------- Math Functions -------------\n");
    ReturnTest<>("SquareNorm             ", &Vector3f32::SquareNorm, { 5.f, 2.f, 5.f }, 54.f);
    SelfTest<>("SelfNormalize          ", &Vector3f32::SelfNormalize, { 4.f, 0.f, 3.f }, { 4.f / 5.f, 0.f, 3.f / 5.f });
    ReturnTest<>("Norm                   ", &Vector3f32::Norm, { 4.f, 3.f, 0.f }, 5.f);
    ReturnTest<>("Normalize              ", &Vector3f32::Normalize, { 4.f, 3.f, 0.f }, { 4.f / 5.f, 3.f / 5.f, 0.f });
    ReturnTest<Vector3f32 const&>("DotProduct             ", &Vector3f32::DotProduct, { 5.f, 2.f, 5.f }, { 5.f, 2.f, 5.f }, 54.f);
    ReturnTest<Vector3f32 const&>("CrossProduct           ", &Vector3f32::CrossProduct, { 1.f, 2.f, 3.f }, { 3.f, 4.f, 5.f }, { -2.f, 4.f, -2.f });

    assert(Vector3f32::Lerp({ 1.f, 2.f, 3.f }, { 3.f, 2.f, 8.f }, 0.5f) == Vector3f32({ 2, 2, 5.5f }));
    assert(Vector3f32::Lerp({ 1.f, 2.f, 2.f }, { 3.f, 2.f, 8.f }, {1.f, 0.5f, 0.25f}) == Vector3f32({3.f, 2.f, 3.5f}));

    Console::Log("\n----------------- Other -----------------\n");
    SelfTest<>("SetOne                 ", &Vector3f32::SetOne, { 12.f, 56.f, 33.f }, { 1.f, 1.f, 1.f });
    SelfTest<>("SetNull                ", &Vector3f32::SetNull, { 12.f, 56.f, 33.f }, { 0.f, 0.f, 0.f });

    Console::Log("\n-----------------------------------------");

    system("pause");

END_TEST_MAIN_FUNCTION(Vector3)
END_TEST(Vector3)


#endif