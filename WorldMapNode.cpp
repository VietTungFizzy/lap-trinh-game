#include "WorldMapNode.h"

void CWorldMapNode::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - WORLD_MAP_NODE_BBOX / 2;
	top = y - WORLD_MAP_NODE_BBOX / 2;
	right = left + WORLD_MAP_NODE_BBOX;
	bottom = top + WORLD_MAP_NODE_BBOX;
}

void CWorldMapNode::Render()
{
	CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
}

bool CWorldMapNode::HasAdjacentNode(int direction)
{
	bool result = false;
	switch (direction) {
	case WORLD_MAP_NODE_ADJACENT_NODE_UP: result = (adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_UP] > 0); break;
	case WORLD_MAP_NODE_ADJACENT_NODE_LEFT: result = (adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_LEFT] > 0); break;
	case WORLD_MAP_NODE_ADJACENT_NODE_RIGHT: result = (adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_RIGHT] > 0); break;
	case WORLD_MAP_NODE_ADJACENT_NODE_DOWN: result =(adjacentNodes[WORLD_MAP_NODE_ADJACENT_NODE_DOWN] > 0); break;
	}
	return result;
}
