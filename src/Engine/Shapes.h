#ifndef ENGINE_SHAPES_H_INCLUDED
#define ENGINE_SHAPES_H_INCLUDED

#include "define.h"
#include <Geometry.h>
#include <GeometryFactory.h>
#include <Shader.h>

namespace gce {


inline struct Shapes
{ 
	Geometry* CUBE = nullptr;
	Geometry* SPHERE = nullptr;
	Geometry* CAPSULE = nullptr;
	Geometry* CYLINDER = nullptr;
	Geometry* HALF_SPHERE = nullptr;
	Geometry* DONUT = nullptr;
	Geometry* PLANE = nullptr;

private:
	void Init()
	{
		CUBE = GeometryFactory::CreateCubeGeo();
		SPHERE = GeometryFactory::CreateSphereGeo( 0.5f, 32, 32 );
		CAPSULE = GeometryFactory::CreateCapsuleGeo( 0.5f, 1.f, 32, 32 );
		CYLINDER = GeometryFactory::CreateCylinderGeo( 0.5f, 0.5f, 1.f, 32, 32 );
		HALF_SPHERE = GeometryFactory::CreateHalfSphereGeo( 0.5f, 32, 32 );
		DONUT = GeometryFactory::CreateDonutGeo( 0.5f, 0.25f, 32, 32 );
		PLANE = GeometryFactory::CreatePlaneGeo( 1.f, 1.f );
	}
	void UnInit() const
	{
		delete CUBE;
		delete SPHERE;
		delete CAPSULE;
		delete CYLINDER;
		delete HALF_SPHERE;
		delete DONUT;
		delete PLANE;
	}
	friend class GameManager;
} SHAPES;


inline struct Shaders
{ 
	Shader VERTEX;
	Shader PIXEL;
	Shader HULL;
	Shader DOMAIN_;
	Shader ROOT_SIGNATURE;

	Shader PIXEL_TEXTURE;
	Shader ROOT_SIGNATURE_TEXTURE;

private:
	void Init()
	{
		VERTEX.Load( "res/Render/Shaders/Lit.vs.cso" );
		PIXEL.Load( "res/Render/Shaders/Lit.ps.cso" );
		HULL.Load( "res/Render/Shaders/Lit.hs.cso" );
		DOMAIN_.Load( "res/Render/Shaders/Lit.ds.cso" );
		ROOT_SIGNATURE.Load( "res/Render/Shaders/Lit.rs.cso" );

		PIXEL_TEXTURE.Load( "res/Render/Shaders/TextureUnlit.ps.cso" );
		ROOT_SIGNATURE_TEXTURE.Load( "res/Render/Shaders/TextureUnlit.rs.cso" );
	}
	void UnInit() const
	{
		//
	}
	friend class GameManager;
} SHADERS;


}

#endif