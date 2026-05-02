#pragma once
#include "define.h"
#include <Engine.h>
#include "Graph.hpp"
#include "Tile.h"

struct GeoCustom;
struct DataGameObject
{
public:

	String name;
	gce::GameObject* gameObject;
	GeoCustom* geo;
	gce::BoxCollider* collider = nullptr;
	gce::MeshRenderer* meshRenderer = nullptr;
};

class Maploader
{
public:
	Maploader();
	void Init();
	void loadMap(int level, gce::Vector3f32 offSet = gce::Vector3f32({0,0,0}));
	Graph<Tile>* mp_currentGraph;

private:
	void GridBuild(GeoCustom* level);

	void WallBuild(DataGameObject* object);

	void SetTexture(DataGameObject* object);

	void SetCollider(DataGameObject* object);


	//Level 1
	String m_1LevelPath = RES_PATH"res/Map/Level1.json";
	//Level 2
	String m_2LevelPath = RES_PATH"res/Map/Level2.json";
	Graph<Tile>* m_grid2;
	Graph<Tile>* m_grid;
	//
	gce::Vector<gce::Vector<GeoCustom>> m_levels;
	std::map<String, Graph<Tile>*> m_graphs;
	String m_data;
	int m_currentLevel = -1;


	gce::Vector<gce::Texture*> m_textures;
	gce::Vector<DataGameObject*> m_drawnObjects;
	gce::Vector<DataGameObject*> m_StoredObjects;

};

