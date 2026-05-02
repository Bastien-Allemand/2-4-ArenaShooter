#include "Physics.h"
#include <corecrt_math.h>
#include "MathsFunctions.hpp"


namespace gce {


// Math�matiques

Plane::Plane(const Vector3f32& n, const Vector3f32& point)
{
    normal = n.Normalize();
    d = -normal.DotProduct(point); // equation : n · x + d = 0
}

float32 Plane::DistanceTo(const Vector3f32& point) const
{
    return normal.DotProduct(point) + d;
}

bool Plane::IsInFront(const Vector3f32& point, float32 tolerance = 0.0f) const
{
    return DistanceTo(point) > tolerance;
}

bool Plane::IsBehind(const Vector3f32& point, float32 tolerance = 0.0f) const
{
    return DistanceTo(point) < -tolerance;
}

float32 AngleBetweenVectors2D(Vector2f32 const& v1, Vector2f32 const& v2)
{
    float32 dot = v1.DotProduct(v2);
    float32 det = v1.x * v2.y - v1.y * v2.x;
    return atan2(det, dot);
}

Vector2f32 RotateVector2D(Vector2f32 const& v, float32 angle) {
    float32 cosA = cos(angle);
    float32 sinA = sin(angle);

    Vector2f32 rotateVect;
    rotateVect.x = v.x * cosA - v.y * sinA;
    rotateVect.y = v.x * sinA + v.y * cosA;
    return rotateVect;
}

Vector3f32 Physics::ReflectVect(Vector3f32 const& v, Vector3f32 const& d)
{
    return v - (d* v.DotProduct(d) *2.f);
}

Vector3f32 Physics::ScaleNRotateVect3(Vector3f32 const& vect, Matrix const& matrix)
{
    Matrix scaleRot = matrix;  //TODO : Change SetValue to [][]
    scaleRot.SetValue(3, 0, 0);
    scaleRot.SetValue(3, 1, 0);
    scaleRot.SetValue(3, 2, 0);

    Matrix localPos = Matrix::Translation(vect);

    localPos = localPos * scaleRot;

    Vector3f32 newPoint;
    newPoint.x = localPos.GetValue(3, 0);
    newPoint.y = localPos.GetValue(3, 1);
    newPoint.z = localPos.GetValue(3, 2);

    return newPoint;
}

Vector2f32 Physics::ScaleNRotateVect2(Vector2f32 const& vect, Matrix const& matrix)
{
	Matrix scaleRot = matrix;  //TODO : Change SetValue to [][]
	scaleRot.SetValue(3, 0, 0);
	scaleRot.SetValue(3, 1, 0);
    Matrix localPos = Matrix::Translation({ vect.x, vect.y, 0.f });
	localPos = localPos * scaleRot;
	Vector2f32 newPoint;
	newPoint.x = localPos.GetValue(3, 0);
	newPoint.y = localPos.GetValue(3, 1);
	return newPoint;
}

// BOX_FUNCTION
void Physics::UpdateBoundingBox(Box& const box, Matrix const& world, Vector3f32 const& scale)
{
    Vector<Vector3f32> listOfPoint = GetListFromMinMax(box.min, box.max);

    for (Vector3f32 point : listOfPoint)
    {
        point *= scale;
        Vector3f32 newPoint = Physics::ScaleNRotateVect3(point, world);

        if (box.aabb.min.x > newPoint.x) box.aabb.min.x = newPoint.x;
        if (box.aabb.min.y > newPoint.y) box.aabb.min.y = newPoint.y;
        if (box.aabb.min.z > newPoint.z) box.aabb.min.z = newPoint.z;
        if (box.aabb.max.x < newPoint.x) box.aabb.max.x = newPoint.x;
        if (box.aabb.max.y < newPoint.y) box.aabb.max.y = newPoint.y;
        if (box.aabb.max.z < newPoint.z) box.aabb.max.z = newPoint.z;
    }
}
Vector<Vector3f32> Physics::GetListFromMinMax(Vector3f32 const& min, Vector3f32 const& max)
{
    Vector<Vector3f32> list;

    list.PushBack({ min.x, min.y, min.z });
    list.PushBack({ max.x, max.y, max.z });
    list.PushBack({ max.x, max.y, min.z });
    list.PushBack({ max.x, min.y, min.z });
    list.PushBack({ min.x, max.y, min.z });
    list.PushBack({ max.x, min.y, max.z });
    list.PushBack({ min.x, max.y, max.z });
    list.PushBack({ min.x, min.y, max.z });

    return list;
}
// BOX_2D_FUNCTION

void Physics::UpdateBoundingBox(Square& const box, Matrix const& world, Vector2f32 const& scale)
{
	Vector<Vector2f32> listOfPoint = GetListFromMinMax(box.min, box.max);
	for (Vector2f32 point : listOfPoint)
	{
		point *= scale;
		Vector2f32 newPoint = Physics::ScaleNRotateVect2(point, world);
		if (box.aabb.min.x > newPoint.x) box.aabb.min.x = newPoint.x;
		if (box.aabb.min.y > newPoint.y) box.aabb.min.y = newPoint.y;
		if (box.aabb.max.x < newPoint.x) box.aabb.max.x = newPoint.x;
		if (box.aabb.max.y < newPoint.y) box.aabb.max.y = newPoint.y;
	}
}

Vector<Vector2f32> Physics::GetListFromMinMax(Vector2f32 const& min, Vector2f32 const& max)
{
	Vector<Vector2f32> list;
	list.PushBack({ min.x, min.y });
	list.PushBack({ max.x, max.y });
	list.PushBack({ max.x, min.y });
	list.PushBack({ min.x, max.y });
	return list;
}

// Intersections
CollideResult Physics::IntersectSphereSphere( Sphere& const sphere1, Sphere& const sphere2)
{
    CollideResult res;

	Vector3f32 vector = sphere2.center - sphere1.center;
	
	float32 distance = (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);

    res.isColliding = IsLessEqual(distance, (sphere1.ray + sphere2.ray)*(sphere1.ray + sphere2.ray), 1);

    if (res.isColliding)
    {
        res.hitPoint = sphere1.center + (vector.SelfNormalize() * sphere1.ray);
    }
    return res;
}


CollideResult Physics::IntersectAABBAABB(BoundingBox const& a, BoundingBox const& b)
{
    CollideResult res;

    Vector3f32 worldMin1 = a.center + a.min;
    Vector3f32 worldMax1 = a.center + a.max;
    Vector3f32 worldMin2 = b.center + b.min;
    Vector3f32 worldMax2 = b.center + b.max;

    res.isColliding = (IsLessEqual(worldMin1.x, worldMax2.x, 1) && IsMoreEqual(worldMax1.x, worldMin2.x, 1) &&
                       IsLessEqual(worldMin1.y, worldMax2.y, 1) && IsMoreEqual(worldMax1.y, worldMin2.y, 1) &&
                       IsLessEqual(worldMin1.z, worldMax2.z, 1) && IsMoreEqual(worldMax1.z, worldMin2.z, 1));

    if (res.isColliding)
    {
        Vector3f32 contactMin = {
            IsMore(worldMin1.x, worldMin2.x, 1) ? worldMin1.x : worldMin2.x,
            IsMore(worldMin1.y, worldMin2.y, 1) ? worldMin1.y : worldMin2.y,
            IsMore(worldMin1.z, worldMin2.z, 1) ? worldMin1.z : worldMin2.z
        };

        Vector3f32 contactMax = {
            IsLess(worldMax1.x, worldMax2.x, 1) ? worldMax1.x : worldMax2.x,
            IsLess(worldMax1.y, worldMax2.y, 1) ? worldMax1.y : worldMax2.y,
            IsLess(worldMax1.z, worldMax2.z, 1) ? worldMax1.z : worldMax2.z
        };

        res.hitPoint = (contactMin + contactMax) * 0.5f;
    }
    return res;
}

Vector<Vector3f32> Physics::GetCorners(Box const& a)
{
    Vector<Vector3f32> cornersA;

    Vector3f32 extentA = (a.max - a.min) * 0.5f;

    cornersA.PushBack(a.center + a.axisX * extentA.x + a.axisY * extentA.y + a.axisZ * extentA.z);
    cornersA.PushBack(a.center - a.axisX * extentA.x + a.axisY * extentA.y + a.axisZ * extentA.z);
    cornersA.PushBack(a.center + a.axisX * extentA.x - a.axisY * extentA.y + a.axisZ * extentA.z);
    cornersA.PushBack(a.center + a.axisX * extentA.x + a.axisY * extentA.y - a.axisZ * extentA.z);
    cornersA.PushBack(a.center - a.axisX * extentA.x - a.axisY * extentA.y + a.axisZ * extentA.z);
    cornersA.PushBack(a.center - a.axisX * extentA.x + a.axisY * extentA.y - a.axisZ * extentA.z);
    cornersA.PushBack(a.center + a.axisX * extentA.x - a.axisY * extentA.y - a.axisZ * extentA.z);
    cornersA.PushBack(a.center - a.axisX * extentA.x - a.axisY * extentA.y - a.axisZ * extentA.z);

    return cornersA;
}

CollideResult Physics::IntersectBoxBox(Box const& a, Box const& b)
{
    // Check AABB collision first
    CollideResult res_AABB = IntersectAABBAABB(a.aabb, b.aabb);
    if (!res_AABB.isColliding)
    {
        return res_AABB;
    }

    CollideResult res;
    res.isColliding = false;

    Vector3f32 axesA[3] = { a.axisX, a.axisY, a.axisZ };
    Vector3f32 axesB[3] = { b.axisX, b.axisY, b.axisZ };

    // Get all possible separating axes (15 total)
    Vector3f32 axesToTest[15];
    int index = 0;
    for (int i = 0; i < 3; ++i) axesToTest[index++] = axesA[i];
    for (int i = 0; i < 3; ++i) axesToTest[index++] = axesB[i];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            Vector3f32 cross = axesA[i].CrossProduct(axesB[j]);
            if (IsMore(cross.SquareNorm(),0.0f))
                axesToTest[index++] = cross.Normalize();
        }

    float32 minOverlap = FLT_MAX;
    Vector3f32 minAxis;

    Vector3f32 delta = b.center - a.center;

    Vector3f32 extentA = (a.max - a.min) * 0.5f;
    Vector3f32 extentB = (b.max - b.min) * 0.5f;

    for (int i = 0; i < index; ++i)
    {
        Vector3f32 axis = axesToTest[i].Normalize();

        float32 projA =
            extentA.x * fabs(axis.DotProduct(a.axisX)) +
            extentA.y * fabs(axis.DotProduct(a.axisY)) +
            extentA.z * fabs(axis.DotProduct(a.axisZ));

        float32 projB =
            extentB.x * fabs(axis.DotProduct(b.axisX)) +
            extentB.y * fabs(axis.DotProduct(b.axisY)) +
            extentB.z * fabs(axis.DotProduct(b.axisZ));

        float32 dist = fabs(axis.DotProduct(delta));
        float32 overlap = projA + projB - dist;

        if (overlap < 0)
            return res;

        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            minAxis = axis * ((axis.DotProduct(delta) < 0) ? -1.f : 1.f);
        }
    }

    res.isColliding = true;
    res.overlapVector = minAxis * minOverlap;

    // hit point estimation

    // Get corners of A and B
    Vector<Vector3f32> cornersA = GetCorners(a);
    Vector<Vector3f32> cornersB = GetCorners(b);

    Vector<Vector3f32> penetratingPoints;

    float32 maxProj = -FLT_MAX;
    Vector3f32 contactPoint;

    // Searching the corner where the contact is closer
    for (const Vector3f32& pt : cornersA)
    {
        float32 proj = pt.DotProduct(-minAxis);
        if (proj > maxProj)
        {
            maxProj = proj;
            contactPoint = pt;
        }
    }
    for (const Vector3f32& pt : cornersB)
    {
        float32 proj = pt.DotProduct(minAxis);
        if (proj > maxProj)
        {
            maxProj = proj;
            contactPoint = pt;
        }
    }

    Plane contactPlane(minAxis, contactPoint);

    for (const Vector3f32& pt : cornersA)
    {
        if (Abs(contactPlane.DistanceTo(pt)) < 0.01f) penetratingPoints.PushBack(pt);
    }

    for (const Vector3f32& pt : cornersB)
    {
        if (Abs(contactPlane.DistanceTo(pt)) < 0.01f) penetratingPoints.PushBack(pt);
    }

    if (!penetratingPoints.Empty())
    {
        Vector3f32 avg = { 0,0,0 };
        for (const Vector3f32& pt : penetratingPoints)
        {
            avg += pt;
        }
        res.hitPoint = avg / (float32)penetratingPoints.Size();
    }
    else
    {
        res.hitPoint = (a.center + b.center) * 0.5f - minAxis * (minOverlap * 0.5f);
    }

    return res;
}



CollideResult Physics::IntersectSphereBox(Sphere& const sphere, Box& const box)
{
    Vector3f32 extentBox = (box.max - box.min) * 0.5f;
    Vector3f32 offsetBox = (box.max + box.min) * 0.5f;

    CollideResult res;
    res.isColliding = false;

    Vector3f32 d = sphere.center - box.center;
    Vector3f32 closest = box.center;

    for (int i = 0; i < 3; ++i)
    {
        Vector3f32 axis = (i==0) ? box.axisX : (i==1) ? box.axisY : box.axisZ;     // u0, u1, u2
        float distance = d.DotProduct(axis);
        float clamped = Max(-extentBox[i], Min(distance, extentBox[i]));
        closest += axis * clamped;
    }

    Vector3f32 delta = sphere.center - closest;
    float32 distSq = delta.DotProduct(delta);

    if (IsLessEqual(distSq, sphere.ray * sphere.ray, 1))
    {
        res.isColliding = true;

        float32 dist = sqrt(distSq);

        res.hitPoint = closest;
    }

    return res;
}

CollideResult2D Physics::IntersectCircleCircle(Circle const& circle1, Circle const& circle2)
{
	CollideResult2D res;
	Vector2f32 vector = circle2.center - circle1.center;

	float32 distance = (vector.x * vector.x) + (vector.y * vector.y);
	res.isColliding = (distance <= (circle1.radius + circle2.radius) * (circle1.radius + circle2.radius));
	if (res.isColliding)
	{
		res.hitPoint = circle1.center + (vector.SelfNormalize() * circle1.radius);
		res.overlapVector = vector.SelfNormalize() * ((circle1.radius + circle2.radius) - sqrt(distance));
	}
	return res;
}

CollideResult2D Physics::IntersectCircleSquare(Circle const& circle, Square const& square)
{
    Vector2f32 extentBox = (square.max - square.min) * 0.5f;
    Vector2f32 offsetBox = (square.max + square.min) * 0.5f;

    CollideResult2D res;
    res.isColliding = false;

    Vector2f32 axes[2] = { square.axisX, square.axisY };

    Vector2f32 extent = (square.max - square.min) * 0.5f;
    Vector2f32 offset = (square.max + square.min) * 0.5f;
    Vector2f32 obbCenter = square.center + axes[0] * offset.x + axes[1] * offset.y;

    Vector2f32 d = circle.center - obbCenter;

    Vector2f32 closest = obbCenter;
    for (int i = 0; i < 2; ++i)
    {
        float32 dist = d.DotProduct(axes[i]);
        float32 clamped = Max(-extent[i], Min(dist, extent[i]));
        closest += axes[i] * clamped;
    }

    Vector2f32 delta = circle.center - closest;
    float32 distSq = delta.DotProduct(delta);

    if (distSq <= circle.radius * circle.radius)
    {
        res.isColliding = true;
        float32 dist = sqrt(distSq);

        if (dist < 1e-6f) {
            float32 minDist = FLT_MAX;
            Vector2f32 normal;
            for (int i = 0; i < 2; ++i) {
                float32 proj = d.DotProduct(axes[i]);
                float32 overlap = extent[i] - fabs(proj);
                if (overlap < minDist) {
                    minDist = overlap;
                    normal = (proj > 0) ? axes[i] : axes[i] * -1.f;
                }
            }
            res.overlapVector = normal * (circle.radius + minDist);
            res.hitPoint = circle.center;
        }
        else {
            res.overlapVector = delta.SelfNormalize() * (circle.radius - dist);
            res.hitPoint = closest;
        }
    }

    return res;
}

CollideResult2D Physics::IntersectAABBAABB(BoundingBox2D const& a, BoundingBox2D const& b)
{
    CollideResult2D res;

    Vector2f32 worldMin1 = a.center + a.min;
    Vector2f32 worldMax1 = a.center + a.max;
    Vector2f32 worldMin2 = b.center + b.min;
    Vector2f32 worldMax2 = b.center + b.max;

    res.isColliding = (worldMin1.x <= worldMax2.x && worldMax1.x >= worldMin2.x &&
        worldMin1.y <= worldMax2.y && worldMax1.y >= worldMin2.y);

    if (res.isColliding)
    {
        Vector2f32 contactMin = {
            worldMin1.x > worldMin2.x ? worldMin1.x : worldMin2.x,
            worldMin1.y > worldMin2.y ? worldMin1.y : worldMin2.y
        };
        Vector2f32 contactMax = {
            worldMax1.x < worldMax2.x ? worldMax1.x : worldMax2.x,
            worldMax1.y < worldMax2.y ? worldMax1.y : worldMax2.y
        };
        res.hitPoint = (contactMin + contactMax) * 0.5f;
    }
    return res;
}


CollideResult2D Physics::IntersectSquareSquare(Square const& square1, Square const& square2)
{
    // Check AABB collision first
    CollideResult2D res_AABB = IntersectAABBAABB(square1.aabb, square2.aabb);
    if (!res_AABB.isColliding)
    {
        return res_AABB;
    }

    // If AABB collision => OBB check
    CollideResult2D res;
    res.isColliding = true;
    const float32 EPSILON = 1e-5f;

    Vector2f32 extentA = (square1.max - square1.min) * 0.5f;
    Vector2f32 offsetA = (square1.max + square1.min) * 0.5f;

    Vector2f32 extentB = (square2.max - square2.min) * 0.5f;
    Vector2f32 offsetB = (square2.max + square2.min) * 0.5f;

    Vector2f32 centerA = square1.center +
        square1.axisX * offsetA.x +
        square1.axisY * offsetA.y;

    Vector2f32 centerB = square2.center +
        square2.axisX * offsetB.x +
        square2.axisY * offsetB.y;

    Vector2f32 t = centerB - centerA;
    Vector2f32 tA = {
        t.DotProduct(square1.axisX),
        t.DotProduct(square1.axisY)
    };

    float32 minOverlap = FLT_MAX;
    Vector2f32 minAxis = { 1.0f, 0.0f }; // Correction: axe par défaut valide

    // Créer une matrice R pour les produits scalaires des axes
    float32 R[2][2];
    float32 AbsR[2][2];

    for (int i = 0; i < 2; ++i)
    {
        Vector2f32 aAxis = (i == 0) ? square1.axisX : square1.axisY;
        for (int j = 0; j < 2; ++j)
        {
            Vector2f32 bAxis = (j == 0) ? square2.axisX : square2.axisY;
            R[i][j] = aAxis.DotProduct(bAxis);
            AbsR[i][j] = fabs(R[i][j]) + EPSILON;
        }
    }

    // Check A axes (2)
    for (int i = 0; i < 2; ++i)
    {
        float32 ra = extentA[i];
        float32 rb = extentB[0] * AbsR[i][0] + extentB[1] * AbsR[i][1];
        float32 dist = fabs(tA[i]);

        float32 overlap = ra + rb - dist;
        if (overlap < 0.f)
        {
            res.isColliding = false;
            return res;
        }
        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            Vector2f32 axis = (i == 0) ? square1.axisX : square1.axisY;
            minAxis = (tA[i] > 0) ? axis : axis * -1.f; // Correction: logique inversée
        }
    }

    // Check B axes (2)
    for (int i = 0; i < 2; ++i)
    {
        float32 ra = extentA[0] * AbsR[0][i] + extentA[1] * AbsR[1][i];
        float32 rb = extentB[i];
        float32 dist = fabs(tA[0] * R[0][i] + tA[1] * R[1][i]);

        float32 overlap = ra + rb - dist;
        if (overlap < 0.0f)
        {
            res.isColliding = false;
            return res;
        }
        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            Vector2f32 axis = (i == 0) ? square2.axisX : square2.axisY;
            minAxis = (t.DotProduct(axis) > 0.0f) ? axis : axis * -1.f; // Correction: logique inversée
        }
    }

    res.hitPoint = (centerA + centerB);
    res.overlapVector = minAxis * minOverlap;
    return res;
}




// FORCES

// Collision forces

Vector2f32 Physics::CalculateStaticCollisionVect2D(Vector2f32 const& movDirection, Vector2f32 const& collisionDirection)
{
    float32 collisionAngle = AngleBetweenVectors2D(movDirection, collisionDirection);
    Vector2f32 collisionDir = RotateVector2D(movDirection, (2 * collisionAngle));

    collisionDir.SelfNormalize();

    return -collisionDir;
}

Vector3f32 Physics::CalculateStaticCollisionVect(Vector3f32 const& movDirection, Vector3f32 const& collisionDirection)
{
    return ReflectVect(movDirection, collisionDirection);
}


Vector3f32 Physics::CalculateCollisionVelocity(
    float32 const massA, float32 const massB,
    Vector3f32 const& velocityA, Vector3f32 const& velocityB,
    Vector3f32 const& collisionNormal,
    float32 restitution)
{
    Vector3f32 relativeVelocity = velocityA - velocityB;
    float32 velAlongNormal = relativeVelocity.DotProduct(collisionNormal);

    if (IsMore(velAlongNormal, -0.01f, 1))
    {
        restitution = 0.f;
    }

    if (IsMore(velAlongNormal, 0.f, 1))
        return velocityA;

    float32 impulseScalar;
    if (IsMore(massB, 1e5f, 1))
    {
        impulseScalar = -(1.f + restitution) * velAlongNormal * massA;
    }
    else
    {
        impulseScalar = -(1.f + restitution) * velAlongNormal;
        impulseScalar /= (1.f / massA + 1.f / massB);
    }

    Vector3f32 impulse = collisionNormal * impulseScalar;

    return velocityA + (impulse / massA);
}

Vector3f32 Physics::CalculateAngularCollisionVelocity(
    float32 const inertiaA, float32 const inertiaB,
    Vector3f32 const& angularVelocityA, Vector3f32 const& angularVelocityB,
    Vector3f32 const& rA, Vector3f32 const& rB,
    Vector3f32 const& collisionNormal,
    float32 const restitution)
{

    Vector3f32 velAtContactA = angularVelocityA.CrossProduct(rA);
    Vector3f32 velAtContactB = angularVelocityB.CrossProduct(rB);
    Vector3f32 relativeVel = velAtContactA - velAtContactB;

    float32 velAlongNormal = relativeVel.DotProduct(collisionNormal);

    if (IsLess(velAlongNormal, 0.f, 1))
        return angularVelocityA;

    Vector3f32 rA_crossN = rA.CrossProduct(collisionNormal);
    Vector3f32 rB_crossN = rB.CrossProduct(collisionNormal);

    float32 angularDenominator = (rA_crossN.SquareNorm() / inertiaA) + (rB_crossN.SquareNorm() / inertiaB);

    float32 j = (1.f + restitution) * velAlongNormal;
    j /= angularDenominator;

    Vector3f32 impulse = collisionNormal * j;

    Vector3f32 deltaAngularVelocityA = (rA.CrossProduct(impulse)) / inertiaA;
    
    return angularVelocityA + deltaAngularVelocityA;
}

void Physics::NewtonForceLaw(Vector3f32& position, Vector3f32& velocity, Vector3f32& angularVelocity, Vector3f32& forceSom, float32 mass, float32 gravityScale, float32 airDragCoefficient, Vector3f32 staticForce, float32 deltaTime)
{
    Vector3f32 gravity(0.f, -g * gravityScale, 0.f);
    Vector3f32 drag = -(velocity * velocity.Norm()) * airDragCoefficient;

    Vector3f32 acceleration = gravity + (drag + forceSom) / mass;
    if (IsLess(velocity.Norm(), 0.15f, 1))
    {
        if (IsLess(acceleration.Norm(), staticForce.Norm(), 1))
        {
            velocity = { 0,0,0 };
            angularVelocity *= 0.3f;
            if (IsLess(angularVelocity.Norm(), 0.1, true))
            {
                angularVelocity = { 0,0,0 };
            }
            return;
        }
    }
    
    position += velocity * deltaTime;
    velocity += acceleration * deltaTime;

}

Vector3f32 Physics::CalculateTorque(Force const& myForce, Vector3f32 const& relativeCenterOfMass)
{
    Vector3f32 leverArm = myForce.relativeApplicationPoint - relativeCenterOfMass;
    return leverArm.CrossProduct(myForce.direction * myForce.norm);
}

void Physics::NewtonTorqueLaw(Vector3f32& rotation, Vector3f32& angularVelocity,float32 const angularFrictionCoeff, Vector3f32 const& totalTorque, float32 const momentOfInertia, float32 const deltaTime)
{
    Vector3f32 angularAcceleration = (totalTorque - angularVelocity * angularFrictionCoeff) / momentOfInertia;
    angularVelocity += angularAcceleration * deltaTime;
    if (angularVelocity.Norm() < 0.01f) angularVelocity = { 0,0,0 };
    rotation += angularVelocity * deltaTime;
}


}
