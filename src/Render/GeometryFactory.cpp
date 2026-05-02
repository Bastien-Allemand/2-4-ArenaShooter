#include "pch.h"
#include "GeometryFactory.h"
#include "nlohmann.hpp"
#include "Obj.h"
#include "JsonParser.h"
#include "JsonObjects.h"
#include "Geometry.h"
#include "Structs.h"
#include "define.h"
#include "RenderContext.h"
#include "Core/GeoCustom.h"

using namespace DirectX;

namespace gce
{
	Geometry* GeometryFactory::CreateCubeGeo()
	{
Vertex v[24]{
		// FRONT
		{{ -.5f, -.5f, -.5f }, { 0.0f, 0.0f, -1.0f}, { 0, 0, 0 }, {0.0f, 1.0f}},	// 0
		{{ -.5f,  .5f, -.5f }, { 0.0f, 0.0f, -1.0f}, { 0, 1, 0 }, {0.0f, 0.0f}}, 	// 1
		{{  .5f,  .5f, -.5f }, { 0.0f, 0.0f, -1.0f}, { 1, 1, 0 }, {1.0f, 0.0f}},	// 2
		{{  .5f, -.5f, -.5f }, { 0.0f, 0.0f, -1.0f}, { 1, 0, 0 }, {1.0f, 1.0f}},	// 3

		// BACK
		{{ -.5f, -.5f,  .5f },  { 0.0f, 0.0f, 1.0f}, { 0, 0, 1,}, {1.0f, 1.0f}},	// 4
		{{  .5f, -.5f,  .5f },  { 0.0f, 0.0f, 1.0f}, { 1, 0, 1,}, {0.0f, 1.0f}},	// 5
		{{  .5f,  .5f,  .5f },  { 0.0f, 0.0f, 1.0f}, { 1, 1, 1,}, {0.0f, 0.0f}},	// 6
		{{ -.5f,  .5f,  .5f },  { 0.0f, 0.0f, 1.0f}, { 0, 1, 1 }, {1.0f, 0.0f}},	// 7 

		// LEFT
		{{ -.5f, -.5f,  .5f }, { -1.0f, 0.0f, 0.0f}, { 0, 0, 1 }, {1.0f, 1.0f}},	// 8
		{{ -.5f,  .5f,  .5f }, { -1.0f, 0.0f, 0.0f}, { 0, 1, 1 }, {0.0f, 1.0f}},	// 9
		{{ -.5f,  .5f, -.5f }, { -1.0f, 0.0f, 0.0f}, { 0, 1, 0 }, {0.0f, 0.0f}},	// 10
		{{ -.5f, -.5f, -.5f }, { -1.0f, 0.0f, 0.0f}, { 0, 0, 0 }, {1.0f, 0.0f}},	// 11

		// RIGHT
		{{  .5f, -.5f, -.5f } , { 1.0f, 0.0f, 0.0f}, { 1, 0, 1 }, {0.0f, 1.0f}},	// 12
		{{  .5f,  .5f, -.5f } , { 1.0f, 0.0f, 0.0f}, { 1, 1, 1 }, {0.0f, 0.0f}},	// 13
		{{  .5f,  .5f,  .5f } , { 1.0f, 0.0f, 0.0f}, { 1, 1, 1 }, {1.0f, 0.0f}},	// 14
		{{  .5f, -.5f,  .5f } , { 1.0f, 0.0f, 0.0f}, { 1, 0,1  }, {1.0f, 1.0f}},	// 15

		// TOP
		{{ -.5f,  .5f, -.5f }, { 0.0f,1.0f,0.f }, { 0, 1, 0}, {0.0f, 1.0f}},	// 16
		{{ -.5f,  .5f,  .5f }, { 0.0f,1.0f,0.f }, { 0, 1, 1}, {0.0f, 0.0f}},	// 17
		{{  .5f,  .5f,  .5f }, { 0.0f,1.0f,0.f }, { 1, 1, 1}, {1.0f, 0.0f}},	// 18
		{{  .5f,  .5f, -.5f }, { 0.0f,1.0f,0.f }, { 1, 1, 0 },{1.0f, 1.0f}},	// 19

		// BOTTOM											
		{{ -.5f, -.5f,  .5f }, { 0.0f,-1.0f,0.f }, { 0, 0, 1 }, {0.0f, 1.0f}},	// 20
		{{ -.5f, -.5f, -.5f }, { 0.0f,-1.0f,0.f }, { 0, 0, 0 }, {0.0f, 0.0f}},	// 21
		{{  .5f, -.5f, -.5f }, { 0.0f,-1.0f,0.f }, { 1, 0, 0 }, {1.0f, 0.0f}},	// 22
		{{  .5f, -.5f,  .5f }, {0.0f,-1.0f,0.f },  { 1, 0, 1 }, {1.0f, 1.0f}},	// 23
	};

		uint32 i[36]{
			// FRONT
			0, 1, 2,
			0, 2, 3,

			// BACK
			4, 5, 6,
			4, 6, 7,

			// LEFT
			8, 9, 10,
			8, 10, 11,

			// RIGHT
			12, 13, 14,
			12, 14, 15,

			// TOP 
			16, 17, 18,
			16, 18, 19,

			// BOTTOM
			20, 21, 22,
			20, 22, 23
		};

		return new Geometry(v, _countof(v), i, _countof(i));
	}

	Geometry* GeometryFactory::CreateSphereGeo(float32 radius, uint32 sliceCount, uint32 stackCount)
	{
		Vector<Vertex> vertices;
		Vector<uint32> indices;

		Vertex topVertex{ {0.0f, +radius, 0.0f}, {0.0f, +1.0f, 0.0f},{ 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		Vertex bottomVertex{ {0.0f, -radius, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} };

		vertices.PushBack(topVertex);

		float32 phiStep = XM_PI / stackCount;
		float32 thetaStep = 2.0f * XM_PI / sliceCount;

		for (uint32 i = 1; i <= stackCount - 1; ++i)
		{
			float32 phi = i * phiStep;

			for (uint32 j = 0; j <= sliceCount; ++j)
			{
				float32 theta = j * thetaStep;

				Vertex v;

				v.pos.x = radius * sinf(phi) * cosf(theta);
				v.pos.y = radius * cosf(phi);
				v.pos.z = radius * sinf(phi) * sinf(theta);

				//Partial derivative of P with respect to theta
				v.tangent.x = -radius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.0f;
				v.tangent.z = +radius * sinf(phi) * cosf(theta);

				v.tangent =  v.tangent.Normalize();
				v.normal = v.pos.Normalize();

				v.uv.x = theta / XM_2PI;
				v.uv.y = phi / XM_PI;

				vertices.PushBack(v);
			}
		}

		vertices.PushBack(bottomVertex);

		uint32 baseIndex = 1;
		for (uint32 i = 1; i <= sliceCount; ++i)
		{
			indices.PushBack(baseIndex);
			indices.PushBack(baseIndex + i + 1);
			indices.PushBack(baseIndex + i);
		}

		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 i = 0; i < stackCount - 2; ++i)
		{
			for (uint32 j = 0; j < sliceCount; ++j)
			{
				indices.PushBack(baseIndex + i * ringVertexCount + j);
				indices.PushBack(baseIndex + i * ringVertexCount + j + 1);
				indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j);

				indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j);
				indices.PushBack(baseIndex + i * ringVertexCount + j + 1);
				indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		uint32 southPoleIndex = static_cast<uint32>(vertices.Size()) - 1;
		baseIndex = southPoleIndex - ringVertexCount;

		for (uint32 i = 0; i < sliceCount; ++i)
		{
			indices.PushBack(southPoleIndex);
			indices.PushBack(baseIndex + i);
			indices.PushBack(baseIndex + i + 1);
		}

		return new Geometry(vertices.Data(), vertices.Size(), indices.Data(), indices.Size());
	}

	Geometry* GeometryFactory::CreateHalfSphereGeo(float32 radius, uint32 sliceCount, uint32 stackCount)
	{
		Vector<Vertex> vertices;
		Vector<uint32> indices;

		BuildHalfSphere(radius, sliceCount, stackCount, vertices, indices, true);
		BuildCylinderCap(radius, 0.0f, sliceCount, vertices, indices, false);

		return new Geometry(vertices.Data(), vertices.Size(), indices.Data(), indices.Size());
	}

	Geometry* GeometryFactory::CreateCapsuleGeo(float32 radius, float32 height, uint32 slices, uint32 stacks)
	{
		Vector<Vertex> vertices;
		Vector<uint32> indices;

		vertices.Clear();
		indices.Clear();
		
		// body + 2 hemispheres
		uint32 rings = stacks + 2 * (stacks / 2);
		uint32 verticalSegments = rings;
		uint32 horizontalSegments = slices;

		float32 bodyStartY = height / 2.0f;
		float32 bodyEndY = -height / 2.0f;

		for (uint32 y = 0; y <= verticalSegments; ++y)
		{
			float32 v = y / static_cast<float32>(verticalSegments);

			float32 phi;
			float32 yPos;

			if (y < stacks / 2)
			{
				// top hemisphere
				float32 t = static_cast<float32>(y) / (stacks / 2);
				phi = t * (XM_PI / 2.0f);
				yPos = bodyStartY + radius * cosf(phi);
			}
			else if (y <= (stacks / 2 + stacks))
			{
				// cylinder
				float32 t = static_cast<float32>(y - stacks / 2) / stacks;
				yPos = bodyStartY - t * height;
				phi = XM_PI / 2.0f;
			}
			else
			{
				// bottom hemisphere
				float32 t = static_cast<float32>(y - stacks - stacks / 2) / (stacks / 2);
				phi = (XM_PI / 2.0f) * (1.0f - t);
				yPos = bodyEndY - radius * cosf(phi);
			}

			float32 ringRadius = radius * sinf(phi);

			for (uint32 x = 0; x <= horizontalSegments; ++x)
			{
				float32 u = static_cast<float32>(x) / horizontalSegments;
				float32 theta = u * 2.0f * XM_PI;

				float32 cosTheta = cosf(theta);
				float32 sinTheta = sinf(theta);

				float32 px = ringRadius * cosTheta;
				float32 py = yPos;
				float32 pz = ringRadius * sinTheta;

				Vector3f32 pos(px, py, pz);
				Vector3f32 normal = Vector3f32(px, yPos - (y < stacks / 2 ? bodyStartY : (y > stacks / 2 + stacks ? bodyEndY : yPos)), pz).Normalize();

				Vector3f32 tangent(-sinTheta, 0.0f, cosTheta);
				Vector2f32 uv(theta,  py);

				vertices.PushBack({ pos, normal, tangent, uv });
			}
		}

		// Indices
		uint32 ringVertexCount = horizontalSegments + 1;
		for (uint32 y = 0; y < verticalSegments; ++y)
		{
			for (uint32 x = 0; x < horizontalSegments; ++x)
			{
				uint32_t i0 = y * ringVertexCount + x;
				uint32_t i1 = i0 + 1;
				uint32_t i2 = i0 + ringVertexCount;
				uint32_t i3 = i2 + 1;

				indices.PushBack(i0);
				indices.PushBack(i1);
				indices.PushBack(i2);

				indices.PushBack(i2);
				indices.PushBack(i1);
				indices.PushBack(i3);
			}
		}

		return new Geometry(vertices.Data(), vertices.Size(), indices.Data(), indices.Size());
	}

	Geometry* GeometryFactory::CreateIcoSphereGeo(float32 radius, uint32 numSubdivisions)
	{
		Vector<Vertex> vertices;
		Vector<uint32> indices;

		numSubdivisions = std::min<uint32>(numSubdivisions, 6u);
		const float32 X = 0.525731f;
		const float32 Z = 0.850651f;

		Vector3f32 pos[12] =
		{
			Vector3f32(-X, 0.0f, Z), Vector3f32(X, 0.0f, Z),
			Vector3f32(-X, 0.0f, -Z), Vector3f32(X, 0.0f, -Z),
			Vector3f32(0.0f, Z, X), Vector3f32(0.0f, Z, -X),
			Vector3f32(0.0f, -Z, X), Vector3f32(0.0f, -Z, -X),
			Vector3f32(Z, X, 0.0f), Vector3f32(-Z, X, 0.0f),
			Vector3f32(Z, -X, 0.0f), Vector3f32(-Z, -X, 0.0f)
		};

		uint32 k[60] =
		{
			1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4,
			1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2,
			3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
			10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
		};

		vertices.Resize(12);
		indices.std.assign(&k[0], &k[60]);

		for (uint32 i = 0; i < 12; ++i)
			vertices[i].pos = pos[i];
			for (uint32 i = 0; i < numSubdivisions; ++i)
				Subdivide(indices, vertices);

		for (uint32 i = 0; i < vertices.Size(); ++i)
		{
			Vector3f32 n = vertices[i].pos.Normalize();
			Vector3f32 p = n * radius;

			vertices[i].pos = p;
			vertices[i].normal = n;

			float32 theta = atan2f(vertices[i].pos.z, vertices[i].pos.x);
			if (theta < 0.0f) theta += XM_2PI;

			float32 phi = acosf(vertices[i].pos.y / radius);
			vertices[i].uv.x = theta / XM_2PI;
			vertices[i].uv.y = phi / XM_PI;

			vertices[i].tangent.x = -radius * sinf(phi) * sinf(theta);
			vertices[i].tangent.y = 0.0f;
			vertices[i].tangent.z = +radius * sinf(phi) * cosf(theta);

			vertices[i].tangent = vertices[i].tangent.Normalize();

		}

		return new Geometry(vertices.Data(), vertices.Size(), indices.Data(), indices.Size());
	}

	Geometry* GeometryFactory::LoadGeometry(StringView const path, float32 scale)
	{
		Geometry* customGeo = nullptr;

		Vector<Vertex> vertices;
		Vector<uint32> indices;

		std::string pathToSend = RES_PATH;
		pathToSend += path;

		Obj obj(pathToSend.data());
		obj.LoadObj();


		for (int i = 0; i < obj.meshs.Size(); i++)
		{
			for (obj::Vertex& v : obj.vertices)
			{
				Vertex ve;
				ve.pos = { v.position.x * scale, v.position.y * scale, v.position.z * scale };
				ve.normal = { v.normal.x, v.normal.y, v.normal.z };
				ve.uv = { v.textureCoordinate.x, v.textureCoordinate.y };
				vertices.PushBack(ve);
			}
			for (uint32 index : obj.meshs[i].indices)
				indices.PushBack(index);
		}

		customGeo = new Geometry(vertices.Data(), vertices.Size(), indices.Data(), indices.Size());
		return customGeo;
	}

	Geometry* GeometryFactory::CreateCylinderGeo(float32 bottomRadius, float32 topRadius, float32 height, uint32 sliceCount, uint32 stackCount)
	{
		Vector<Vertex> vertices;
		Vector<uint32> indices;

		BuildCylinderBody(topRadius, bottomRadius, height, sliceCount, stackCount, vertices, indices);
		BuildCylinderCap(topRadius, height, sliceCount, vertices, indices, true);
		BuildCylinderCap(bottomRadius, height, sliceCount, vertices, indices, false);

		return new Geometry(vertices.Data(), vertices.Size(), indices.Data(), indices.Size());
	}

	Geometry* GeometryFactory::CreateDonutGeo(float32 majorRadius, float32 minorRadius, uint32 majorSegments,
		uint32 minorSegments)
	{
		Vector<Vertex> vertices;
		Vector<uint32> indices;

		for (uint32 i = 0; i <= majorSegments; ++i)
		{
			float32 u = static_cast<float32>(i) / majorSegments;
			float32 theta = u * 2.0f * XM_PI; // Angle autour de l'axe principal

			for (uint32 j = 0; j <= minorSegments; ++j)
			{
				float32 v = static_cast<float32>(j) / minorSegments;
				float32 phi = v * 2.0f * XM_PI; // Angle autour du tube

				float32 x = (majorRadius + minorRadius * cos(phi)) * cos(theta);
				float32 y = minorRadius * sin(phi);
				float32 z = (majorRadius + minorRadius * cos(phi)) * sin(theta);

				float32 nx = cos(phi) * cos(theta);
				float32 ny = sin(phi);
				float32 nz = cos(phi) * sin(theta);

				Vertex vertex
				{
					{x, y, z},
					{nx, ny, nz},
					{0.0f, 0.0f, 0.0f},
					{u, v}
				};

				vertices.PushBack(vertex);
			}
		}

		// G�n�rer les indices pour les triangles
		for (uint32 i = 0; i < majorSegments; ++i)
		{
			for (uint32 j = 0; j < minorSegments; ++j)
			{
				uint32 first = i * (minorSegments + 1) + j;
				uint32 second = first + minorSegments + 1;

				// Premier triangle
				indices.PushBack(first + 1);
				indices.PushBack(second);
				indices.PushBack(first);

				// Deuxi�me triangle
				indices.PushBack(first + 1);
				indices.PushBack(second + 1);
				indices.PushBack(second);
			}
		}

		return new Geometry(vertices.Data(), vertices.Size(), indices.Data(), indices.Size());
	}

	Geometry* GeometryFactory::CreatePlaneGeo(float32 width, float32 height)
	{
		Vertex v[4]{
			{{-width / 2, 0.0f, -height / 2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Bottom Left
			{{ width / 2, 0.0f, -height / 2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // Bottom Right
			{{ width / 2, 0.0f,  height / 2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Top Right
			{{-width / 2, 0.0f,  height / 2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f }} // Top Left
		};
		uint32 i[6]{
			// Two triangles to form a square
			3, 2, 1,
			3, 1, 0
		};

		return new Geometry(v, _countof(v), i, _countof(i));
	}

	Geometry* GeometryFactory::Create2DPlane()
	{
		Vertex v[] = 
		{
			{{-0.5f, 0.5f, 0.0f} ,{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f} ,{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f} ,{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, 0.0f} ,{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
		};

		uint32 i[] = 
		{
			0, 1, 3,
			1, 2, 3,
		};

		return new Geometry(v, _countof(v), i, _countof(i));
	}

	Vector<GeoCustom> GeometryFactory::LoadMultipleGeometries(StringView json)
	{
		Vector<GeoCustom> geos;

		Vector<Vertex> vertices;

		File file;
		file.Open(json.data(), File::READ_ONLY);
		String jsonContent = file.FullRead();
		file.Close();

		nlohmann::json::parser_callback_t cb = [](int depth, nlohmann::json::parse_event_t event, nlohmann::json& parsed)
			{
				return true;
			};
		nlohmann::json j_filtered = nlohmann::json::parse(jsonContent, cb);

		for (nlohmann::json obj : j_filtered["objects"])
		{
			std::string name = obj["name"];
			std::string type = obj["type"];
			std::string texturepath = "";
			if (!obj["texture"].is_null())
			{
				texturepath = obj["texture"];
			}
			std::vector<float> pos = obj["position"];
			Vector3f32 position = { pos[0], pos[2], pos[1] };
			pos.clear();
			std::vector<float> rot = obj["rotation"];
			gce::Quaternion rotation = { rot[0], rot[2], rot[1],rot[3] };
			rot.clear();
			std::vector<float> scale = obj["scale"];
			Vector3f32 scaling = { scale[0], scale[2], scale[1] };
			scale.clear();
			nlohmann::json mesh = obj["mesh"];


			std::vector<float> verticespos = mesh["vertices"].get<std::vector<float>>();
			std::vector<uint32> indices = mesh["indices"].get<std::vector<uint32>>();
			std::vector<float> uvs = mesh["uvs"].get<std::vector<float>>();


			GeoCustom geoCustom;
			geoCustom.name = name;
			geoCustom.texturepath = texturepath;
			geoCustom.position = position;
			geoCustom.rotation = rotation;
			geoCustom.scale = scaling;

			if (name == "Grid$N")
			{
				nlohmann::json lines = nullptr;

				if (!obj["lines"].is_null())
				{
					lines = obj["lines"];
					std::vector<float> lineVertices = lines["line_vertices"].get<std::vector<float>>();
					std::vector<uint32> lineIndices = lines["line_indices"].get<std::vector<uint32>>();

					geoCustom.lineVertices = lineVertices;
					geoCustom.lineIndices = lineIndices;
				}
			}
			else
			{
				for (size_t i = 0; i < verticespos.size() / 3; i++)
				{
					Vertex v;
					v.pos = { verticespos[i * 3 + 0], verticespos[i * 3 + 1], verticespos[i * 3 + 2] };
					v.normal = { 0.0f, 0.0f, 0.0f };
					v.tangent = { 0.0f, 0.0f, 0.0f };
					v.uv = { uvs[i * 2 + 0], -uvs[i * 2 + 1] };
					vertices.PushBack(v);
				}
				Geometry* customGeo = new Geometry(vertices.Data(), vertices.Size(), indices.data(), indices.size());
				vertices.Clear();
				geoCustom.pGeometry = customGeo;
			}

			geos.PushBack(geoCustom);
		}
		return geos;
	}

	//=============== UTILS ==========================

	Vertex GeometryFactory::MidPoint(Vertex const& v0, Vertex const& v1)
	{

		Vector3f32 pos = (v0.pos + v1.pos) * 0.5f;
		Vector3f32 normal = ((v0.normal + v1.normal) * 0.5f).Normalize();
		Vector2f32 tex = (v0.uv + v1.uv) * 0.5f;

		Vertex v = 
		{
			pos, normal, {0.0f, 0.0f, 0.0f}, tex
		};

		return v;
	}

	void GeometryFactory::Subdivide(Vector<uint32>& indices, Vector<Vertex>& vertices)
	{
		Vector<uint32> indicesCopy = indices;
		Vector<Vertex> verticesCopy = vertices;

		vertices.Resize(0);
		indices.Resize(0);

		uint32 numTris = static_cast<uint32>(indicesCopy.Size()) / 3;
		for (uint32 i = 0; i < numTris; ++i)
		{
			Vertex v0 = verticesCopy[indicesCopy[i * 3 + 0]];
			Vertex v1 = verticesCopy[indicesCopy[i * 3 + 1]];
			Vertex v2 = verticesCopy[indicesCopy[i * 3 + 2]];

			Vertex m0 = MidPoint(v0, v1);
			Vertex m1 = MidPoint(v1, v2);
			Vertex m2 = MidPoint(v0, v2);

			vertices.PushBack(v0); // 0
			vertices.PushBack(v1); // 1
			vertices.PushBack(v2); // 2
			vertices.PushBack(m0); // 3
			vertices.PushBack(m1); // 4
			vertices.PushBack(m2); // 5

			indices.PushBack(i * 6 + 0);
			indices.PushBack(i * 6 + 3);
			indices.PushBack(i * 6 + 5);

			indices.PushBack(i * 6 + 3);
			indices.PushBack(i * 6 + 4);
			indices.PushBack(i * 6 + 5);

			indices.PushBack(i * 6 + 5);
			indices.PushBack(i * 6 + 4);
			indices.PushBack(i * 6 + 2);

			indices.PushBack(i * 6 + 3);
			indices.PushBack(i * 6 + 1);
			indices.PushBack(i * 6 + 4);
		}
	}

	void GeometryFactory::BuildCylinderBody(float32 topRadius, float32 bottomRadius, float32 height, uint32 sliceCount, uint32 stackCount, Vector<Vertex>& vertices,
		Vector<uint32>& indices)
	{
		float32 stackHeight = height / stackCount;
		float32 radiusStep = (topRadius - bottomRadius) / stackCount;
		uint32 ringCount = stackCount + 1;

		for (uint32 i = 0; i < ringCount; ++i)
		{
			float32 y = -0.5f * height + i * stackHeight;
			float32 r = bottomRadius + i * radiusStep;

			float32 dTheta = 2.0f * XM_PI / sliceCount;
			for (uint32 j = 0; j <= sliceCount; ++j)
			{
				Vertex vertex;

				float32 c = cosf(j * dTheta);
				float32 s = sinf(j * dTheta);

				vertex.pos = Vector3f32(r * c, y, r * s);

				vertex.uv.x = static_cast<float32>(j) / sliceCount * 2 * XM_PI * bottomRadius;
				vertex.uv.y = 1.0f - static_cast<float32>(i) / stackCount;

				Vector3f32 tangent(-s, 0.0f, c);
				float32 dr = bottomRadius - topRadius;
				Vector3f32 bitangent(dr * c, -height, dr * s);

				Vector3f32 N = tangent.CrossProduct(bitangent);
				vertex.normal = N.Normalize();

				vertices.PushBack(vertex);
			}
		}

		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 i = 0; i < stackCount; ++i)
		{
			for (uint32 j = 0; j < sliceCount; ++j)
			{
				indices.PushBack(i * ringVertexCount + j);
				indices.PushBack((i + 1) * ringVertexCount + j);
				indices.PushBack((i + 1) * ringVertexCount + j + 1);
				indices.PushBack(i * ringVertexCount + j);
				indices.PushBack((i + 1) * ringVertexCount + j + 1);
				indices.PushBack(i * ringVertexCount + j + 1);
			}
		}
	}

	void GeometryFactory::BuildCylinderCap(float32 radius, float32 height, uint32 sliceCount,
	                      Vector<Vertex>& vertices, Vector<uint32>& indices, bool isTop)
	{
		uint32 baseIndex = static_cast<uint32>(vertices.Size());
		float32 y = 0.5f * height * (isTop ? 1 : -1);
		float32 dTheta = 2.0f * XM_PI / sliceCount;

		Vertex vertex = {};

		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			float32 x = radius * cosf(i * dTheta);
			float32 z = radius * sinf(i * dTheta);

			float32 u = 0.5f * cosf(i * dTheta);
			float32 v = 0.5f * sinf(i * dTheta);

			//float32 u = i / sliceCount;
			//float32 v = 0.0f;

			vertex.pos = { x, y , z };
			vertex.normal = { 0.0f,  isTop ? 1.0f : -1.0f, 0.0f };
			vertex.tangent = { 1.0f, 0.0f, 0.0f };
			vertex.uv = { u + 0.5f , v + 0.5f };

			vertices.PushBack(vertex); // texture coordinates
		}

		vertex.pos = { 0.0f, y , 0.0f };
		vertex.normal = { 0.0f, isTop ? 1.0f : -1.0f, 0.0f };
		vertex.tangent = { 1.0f, 0.0f, 0.0f };
		vertex.uv = { 0.5f, 0.5f };
		vertices.PushBack(vertex);


		uint32 centerIndex = static_cast<uint32>(vertices.Size()) - 1;

		for (uint32 i = 0; i < sliceCount; ++i)
		{
			indices.PushBack(centerIndex);
			indices.PushBack(baseIndex + i + isTop);
			indices.PushBack(baseIndex + i + !isTop);
		}
	}

	void GeometryFactory::BuildHalfSphere(float32 radius, uint32 sliceCount, uint32 stackCount, Vector<Vertex>& vertices,
		Vector<uint32>& indices, bool isTop, float32 height)
	{
		radius = isTop ? radius : -radius;

		Vertex topVertex{ {0.0f, +radius, 0.0f}, {0.0f, +1.0f, 0.0f},{ 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
		vertices.PushBack(topVertex);

		uint32 baseIndex = static_cast<uint32>(vertices.Size());

		for (uint32 i = 1; i <= sliceCount; ++i)
		{
			indices.PushBack(baseIndex);
			indices.PushBack(baseIndex + i + 1);
			indices.PushBack(baseIndex + i);
		}

		float32 phiStep = XM_PI / stackCount / 1.95f;
		float32 thetaStep = 2.0f * XM_PI / sliceCount;
		thetaStep = isTop ? thetaStep : -thetaStep;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (uint32 i = 1; i <= stackCount - 1; ++i)
		{
			float32 phi = i * phiStep;

			// Vertices of ring.
			for (uint32 j = 0; j <= sliceCount; ++j)
			{
				float32 theta = j * thetaStep;

				Vertex v;
				v.pos.x = radius * sinf(phi) * cosf(theta);
				v.pos.y = radius * cosf(phi) + height;
				v.pos.z = radius * sinf(phi) * sinf(theta);

				// Partial derivative of P with respect to theta
				v.tangent.x = -radius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.0f;
				v.tangent.z = +radius * sinf(phi) * cosf(theta);

				v.tangent = v.tangent.Normalize();
				v.normal = v.pos.Normalize();

				v.uv.x = theta / XM_2PI * (2 * XM_PI * radius);
				v.uv.y = phi / XM_PI * (2 * XM_PI * radius);

				vertices.PushBack(v);
			}
		}

		Vertex bottomVertex{ {0.0f, -radius, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f} };
		vertices.PushBack(bottomVertex);

		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 i = 0; i < stackCount - 2; ++i)
		{
			for (uint32 j = 0; j < sliceCount; ++j)
			{
				indices.PushBack(baseIndex + i * ringVertexCount + j);
				indices.PushBack(baseIndex + i * ringVertexCount + j + 1);
				indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j);

				indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j);
				indices.PushBack(baseIndex + i * ringVertexCount + j + 1);
				indices.PushBack(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}
	}
}
