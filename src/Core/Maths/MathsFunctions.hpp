#ifndef CORE_MATHS_FUNCTIONS_HPP_INCLUDED
#define CORE_MATHS_FUNCTIONS_HPP_INCLUDED

#include <cmath>
#include "define.h"

namespace gce {

constexpr float32 PI = 3.1415927f;
constexpr float32 EPSILON = FLT_EPSILON;
constexpr float32 PhysicsEPSILON = 0.01f;

    //Abs 
    static float32 Abs(float32 a)
    {
        uint32 bits;
        memcpy(&bits, &a, sizeof(float32));
        bits &= 0x7FFFFFFF;
        memcpy(&a, &bits, sizeof(float32));
        return a;
    }

    template<typename T>
    static T Abs(T a)
    {
        if constexpr (T(-1) > T(0))
            static_assert(sizeof(T) == -1, "Abs<T>: T must be a signed type");

        const int numBits = sizeof(T) * 8;
        T mask = a >> (numBits - 1);
        return (a ^ mask) - mask;
    }

    static bool IsZero(float32 a, bool isForPhysic = false)
    {
        float32 epsilon = isForPhysic ? PhysicsEPSILON : EPSILON;
        return Abs(a) < epsilon;
    }

    static bool IsEqual(float32 a, float32 b, bool isForPhysic = false)
    {
        float32 epsilon = isForPhysic ? PhysicsEPSILON : EPSILON;
        return Abs(a - b) < epsilon;
    }

    static bool IsLess(float32 a, float32 b, bool isForPhysic = false)
    {
        float32 epsilon = isForPhysic ? PhysicsEPSILON : EPSILON;
        return (a - b) < -epsilon;
    }

    static bool IsMore(float32 a, float32 b, bool isForPhysic = false)
    {
        float32 epsilon = isForPhysic ? PhysicsEPSILON : EPSILON;
        return (a - b) > epsilon;
    }

    static bool IsLessEqual(float32 a, float32 b, bool isForPhysic = false)
    {
        float32 epsilon = isForPhysic ? PhysicsEPSILON : EPSILON;
        return (a - b) < epsilon;
    }

    static bool IsMoreEqual(float32 a, float32 b, bool isForPhysic = false)
    {
        float32 epsilon = isForPhysic ? PhysicsEPSILON : EPSILON;
        return (a - b) > -epsilon;
    }

    // Factorial  
    static uint32 Factorial(uint32 n)
    {
        uint32 result = 1;
        for (uint32 i = 2; i <= n; ++i)
            result *= i;

        return result;
    }

    // Power  
    static float32 Pow(float32 x, uint32 n)
    {
        float32 result = 1.0f;
        for (uint32 i = 0; i < n; ++i)
            result *= x;

        return result;
    }

    // Normalised angle between -PI and PI  
    static float32 NormalizeAngle(float32 x)
    {
        while (x > PI)
            x -= 2.0f * PI;
        while (x < -PI)
            x += 2.0f * PI;
        return x;
    }

    // Sinus with taylor approximatly
    static float32 Sin(float32 x)
    {
        x = NormalizeAngle(x);
        float32 result = 0.0f;

        // Taylor: sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ... etc
        for (uint32 i = 0; i < 7; ++i)
        {
            float32 sign = (i % 2 == 0) ? 1.0f : -1.0f;
            uint32 power = 2 * i + 1;
            float32 term = Pow(x, power) / static_cast<float32>(Factorial(power));
            result += sign * term;
        }

        return result;
    }

    // Cosinus
    static float32 Cos(float32 x)
    {
        x = NormalizeAngle(x);
        float32 result = 0.0f;

        // cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + ... etc
        for (uint32 i = 0; i < 7; ++i)
        {
            int32 sign = (i % 2 == 0) ? 1 : -1;
            uint32 power = 2 * i;
            float32 term = Pow(x, power) / static_cast<float32>(Factorial(power));
            result += sign * term;
        }

        return result;
    }

    // Tangente (tan(x) = sin(x)/cos(x))
    static float32 Tan(float32 x)
    {
        float32 cosValue = Cos(x);
        if (cosValue == 0.0f)
            return 0.0f;
        return Sin(x) / cosValue;
    }

    static float32 Dist(float32 a, float32 b)
    {
        return a - b;
    }

    static float32 Max(float32 a, float32 b)
    {
        return (a > b) ? a : b;
    }

    static float32 Min(float32 a, float32 b)
    {
        return (a < b) ? a : b;
    }

    static float32 Sqrt(float32 x)
    {
        if (x < 0.0f)
            return 0.0f; // Handle negative input  

        float32 result = x;
        float32 lastResult = 0.0f;
        // Simple iterative method to approximate square root  
        while (result != lastResult)
        {
            lastResult = result;
            result = (result + x / result) * 0.5f;
        }
        return result;
    }

    static float32 Clamp(float32 value, float32 min = 0.0f, float32 max = 1.0f)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    static float32 RadToDeg(float32 radians)
    {
        return radians * (180.0f / PI);
    }
}

#endif