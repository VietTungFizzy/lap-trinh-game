#pragma once
#include "GameObject.h"

#define WORLD_MAP_NODE_BBOX 16
#define WORLD_MAP_NODE_MAXIMUM_ADJACENT_NODE 4

#define WORLD_MAP_NODE_ADJACENT_NODE_UP 0
#define WORLD_MAP_NODE_ADJACENT_NODE_LEFT 1
#define WORLD_MAP_NODE_ADJACENT_NODE_RIGHT 2
#define WORLD_MAP_NODE_ADJACENT_NODE_DOWN 3

class CWorldMapNode :
    public CGameObject
{
protected:
	int adjacentNodes[WORLD_MAP_NODE_MAXIMUM_ADJACENT_NODE];
	int spriteId;

public:
	CWorldMapNode(float x, float y, int type, 
		int adj_t, int adj_l, int adj_r, int adj_d,
		int spriteId
	) : CGameObject(x, y, type) {
		adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_UP] = adj_t;
		adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_LEFT] = adj_l;
		adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_RIGHT] = adj_r;
		adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_DOWN] = adj_d;
		this->spriteId = spriteId;
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render();

	bool HasAdjacentNode(int direction);

	int IsBlocking() { return 0; }
};

