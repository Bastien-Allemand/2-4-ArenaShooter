#ifndef CORE_MATHS_TRANSFORM_H_INCLUDED
#define CORE_MATHS_TRANSFORM_H_INCLUDED

#include "define.h"
#include "Maths/Vector3.h"
#include "Maths/Quaternion.h"
#include "Maths/Matrix.h"

namespace gce {


    struct Transform
    {
        static Transform const Identity;

        Vector3f32 position { 0, 0, 0 };
        
        Vector3f32 scale { 1, 1, 1 };
        
        Vector3f32 right { 1, 0, 0 };     // Transform's right vector (local).
        Vector3f32 up { 0, 1, 0 };      // Transform's up vector (local).
        Vector3f32 forward { 0, 0, 1 }; // Transform's forward vector (local).
        
        Quaternion rotation { 0, 0, 0, 1 }; // Transform's quaternion representing its rotation (local).
    };

    inline Transform const Transform::Identity;

}

#endif