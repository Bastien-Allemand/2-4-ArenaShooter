#ifndef CORE_MATHS_PHYSICS_H_INCLUDED
#define CORE_MATHS_PHYSICS_H_INCLUDED

#include <Containers/Vector.hpp>
#include "Transform.h"
#include "BoundingBox.h"
#include "Vector2.hpp"
#include "Vector3.h"
#include "define.h"
#include "Sphere.h"

namespace gce
{


struct Force
{
	Vector3f32 relativeApplicationPoint;
	Vector3f32 direction;
	float32 norm;
	bool useApplicationPoint = false;
};

struct CollideResult
{
	bool isColliding = false;
	Vector3f32 hitPoint;
	Vector3f32 overlapVector;
};

struct CollideResult2D
{
	bool isColliding = false;
	Vector2f32 hitPoint;
	Vector2f32 overlapVector;
};

class Plane
{
public:
	Vector3f32 normal;
	float32 d;

	Plane(const Vector3f32& n, const Vector3f32& point);

	float32 DistanceTo(const Vector3f32& point) const;
	bool IsInFront(const Vector3f32& point, float32 tolerance) const;
	bool IsBehind(const Vector3f32& point, float32 tolerance) const;
};

class Physics
{
public : 

	static Vector3f32 ScaleNRotateVect3(Vector3f32 const& vect, Matrix const& matrix);
	static Vector2f32 ScaleNRotateVect2(Vector2f32 const& vect, Matrix const& matrix);

	static void UpdateBoundingBox(Box& const box, Matrix const& world, Vector3f32 const& scale);
	static Vector<Vector3f32> GetListFromMinMax(Vector3f32 const& min, Vector3f32 const& max);
	//2D
	static void UpdateBoundingBox(Square& const box, Matrix const& world, Vector2f32 const& scale);
	static Vector<Vector2f32> GetListFromMinMax(Vector2f32 const& min, Vector2f32 const& max);
	
	// INTERSECTIONS
	static CollideResult IntersectSphereSphere( Sphere& const sphere1, Sphere& const sphere2);
	static CollideResult IntersectBoxBox(Box const& box1, Box const& box2);
	static CollideResult IntersectSphereBox(Sphere& const sphere1, Box& const box2);

	static CollideResult2D IntersectCircleCircle(Circle const& circle1, Circle const& circle2);
	static CollideResult2D IntersectCircleSquare(Circle const& circle, Square const& square);
	static CollideResult2D IntersectSquareSquare(Square const& square1, Square const& square2);

	// Vector
	
	static Vector<Vector3f32> GetCorners(Box const& a);
	static Vector3f32 ReflectVect(Vector3f32 const& v, Vector3f32 const& d);

	// FORCES

	// Collision forces

	/// <summary>
	/// Calculate in 2D dimension the normed vector of the direction after collision with an object
	/// </summary>
	/// <param name="movDirection"> -- Director vect of the previous direction</param>
	/// <param name="collisionDirection"> -- Director vect of the collision </param>
	/// <returns></returns>
	static Vector2f32 CalculateStaticCollisionVect2D(Vector2f32 const& movDirection, Vector2f32 const& collisionDirection);
	
	/// <summary>
	/// Calculate in 3D dimension the normed vector of the direction after collision with an object
	/// </summary>
	/// <param name="movDirection"> -- Director vector of the previous direction</param>
	/// <param name="collisionDirection"> -- Director vector of the collision </param>
	/// <returns></returns>
	static Vector3f32 CalculateStaticCollisionVect(Vector3f32 const& movDirection, Vector3f32 const& collisionDirection);

	/// <summary>
	/// Calculate the reslutant velocity of the collision with another object
	/// </summary>
	/// <param name="massA"> -- Mass of the main object</param>
	/// <param name="massB"> -- Mass of the collided object</param>
	/// <param name="velocityA"> -- Velocity of the main object</param>
	/// <param name="velocityB"> -- Velocity of the collided object</param>
	/// <param name="collisionNormal"> -- Normal vector of the collision between the two objects</param>
	/// <param name="restitution"> -- Proportional value of the bounciness between the two objects</param>
	/// <returns></returns>
	static Vector3f32 CalculateCollisionVelocity(float32 massA, float32 massB, Vector3f32 const& velocityA, Vector3f32 const& velocityB, Vector3f32 const& collisionNormal, float32 restitution);

	static Vector3f32 CalculateAngularCollisionVelocity(float32 inertiaA, float32 inertiaB, Vector3f32 const& angularVelocityA, Vector3f32 const& angularVelocityB, Vector3f32 const& rA, Vector3f32 const& rB, Vector3f32 const& collisionNormal, float32 restitution);

	/// <summary>
	/// Use the second newton law for forces
	/// </summary>
	/// <param name="position"> -- Updated position of the object</param>
	/// <param name="velocity"> -- Updated velocity of the object</param>
	/// <param name="velocity"> -- Angular velocity of the object</param>
	/// <param name="forceSom"> -- Som of all the forces applied to the object</param>
	/// <param name="mass"> -- Mass of the object</param>
	/// <param name="gravityScale"> -- Gravity scale applied to the object</param>
	/// <param name="dragCoefficient"> -- Drag coefficient applied to the object</param>
	/// <param name="deltaTime"> -- Elapsed time between two frames</param>
	static void NewtonForceLaw(Vector3f32& position, Vector3f32& velocity, Vector3f32& angularVelocity, Vector3f32& forceSom, float32 mass, float32 gravityScale, float32 dragCoefficient, Vector3f32 staticForce, float32 deltaTime);

	static Vector3f32 CalculateTorque(Force const& myForce, Vector3f32 const& centerOfMass);

	static void NewtonTorqueLaw(Vector3f32& rotation, Vector3f32& angularVelocity,float32 angularFrictionCoeff, Vector3f32 const& totalTorque, float32 momentOfInertia, float32 deltaTime);

	static constexpr float32 g = 9.806f;

private:
	static CollideResult IntersectAABBAABB(BoundingBox const& a, BoundingBox const& b);
	static CollideResult2D IntersectAABBAABB(BoundingBox2D const& a, BoundingBox2D const& b);
};




}

#endif