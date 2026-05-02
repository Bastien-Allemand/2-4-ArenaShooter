#include "Tile.h"

Tile::Tile(gce::Vector3f32 pos)
{
	m_gridPos = pos;
	m_targetDist = 0;
	m_startDist = 0;
	m_previousTile = nullptr;
	//flags
	m_playerOn = false;
	m_UnWalkable = false;
	m_wall = false;
	m_visited = false;
	m_isPath = false;
}

void Tile::SetUnWalkable(bool unWalkable)
{
	m_UnWalkable = unWalkable;
}
