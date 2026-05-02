#pragma once
#include "define.h"
#include <Engine.h>

class Tile
{
public:
	Tile(gce::Vector3f32 pos);

	bool GetVisited() const { return m_visited; }
	void SetVisited(bool visited) { m_visited = visited; }

	bool IsUnWalkable() const { return m_UnWalkable; }
	void SetUnWalkable(bool unWalkable);

	bool IsPath() const { return m_isPath; }
	void SetIsPath(bool isPath) { m_isPath = isPath; }

	int GetTargetDistance(Tile* _tile) const { return (gce::Abs(_tile->GetPosition().x - m_gridPos.x)) + (gce::Abs(_tile->GetPosition().z - m_gridPos.z)); }
	void SetTargetDistance(int targetDist) { m_targetDist = targetDist; }

	int GetStartDistance() const { return m_startDist; }
	void SetStartDistance(int startDist) { m_startDist = startDist; }

	Tile* GetPreviousTile() const { return m_previousTile; }
	void SetPreviousTile(Tile* previousTile) { m_previousTile = previousTile; }

	gce::Vector3f32 GetPosition() const { return m_gridPos; }
	void SetGridPos(const gce::Vector3f32& gridPos) { m_gridPos = gridPos; }

private:
	gce::Vector3f32 m_gridPos;

	int m_targetDist;
	int m_startDist;
	Tile* m_previousTile;

	//flags
	bool m_playerOn;
	bool m_UnWalkable;
	bool m_wall;
	bool m_visited;
	bool m_isPath;
};
