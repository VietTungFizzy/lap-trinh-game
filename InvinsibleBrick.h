#pragma once
#include "GameObject.h"

#define INVINSIBLE_BRICK_WIDTH 16
#define INVINSIBLE_BRICK_BBOX_WIDTH 16
#define INVINSIBLE_BRICK_BBOX_HEIGHT 16

class CInvinsibleBrick :
    public CGameObject
{
private:
	int length, height;
public:
	CInvinsibleBrick(float x, float y, int type, int length = 1, int height = 1) : CGameObject(x, y, type) {
		this->length = length;
		this->height = height;
	}
	void Render() { 
		//RenderBoundingBox(); 
	}
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

};

