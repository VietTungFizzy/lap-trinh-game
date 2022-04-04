#pragma once
#include "GameObject.h"

#define INVINSIBLE_BRICK_WIDTH 16
#define INVINSIBLE_BRICK_BBOX_WIDTH 16
#define INVINSIBLE_BRICK_BBOX_HEIGHT 16

class CInvinsibleBrick :
    public CGameObject
{
public:
	CInvinsibleBrick(float x, float y, int type) : CGameObject(x, y, type) {}
	void Render() { 
		//RenderBoundingBox(); 
	}
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

