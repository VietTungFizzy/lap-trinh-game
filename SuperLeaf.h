#pragma once
#include "GameObject.h"
#define SUPER_LEAF_BBOX_WIDTH 16
#define SUPER_LEAF_BBOX_HEIGHT 14

#define SUPER_LEAF_SPRITE_ID_RIGHT 63001
#define SUPER_LEAF_SPRITE_ID_LEFT 63000

#define SUPER_LEAF_STATE_INVISIBLE 0
#define SUPER_LEAF_STATE_GO_UP 1
#define SUPER_LEAF_STATE_SWING_RIGHT 2
#define SUPER_LEAF_STATE_SWING_LEFT 3
#define SUPER_LEAF_STATE_EATEN 4
#define SUPER_LEAF_STATE_GO_UP_INVISIBLE 5

#define SUPER_LEAF_GO_UP_BEFORE_SWING_SPEED 0.1f
#define SUPER_LEAF_GO_UP_BEFORE_SWING_DISTANCE 40
#define SUPER_LEAF_INVISIBLE_DISTANCE 12
#define SUPER_LEAF_GRAVITY 0.04f
#define SUPER_LEAF_GO_HORIZONTAL_SPEED 0.05f
#define SUPER_LEAF_HORIZONTAL_RANGE 32
#define SUPER_LEAF_GO_UP_SPEED 0.015f
class CSuperLeaf :
    public CGameObject
{
private:
	float initY, initX;
	float boundaryRight, boundaryLeft;
	bool isHalfWay;

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	CSuperLeaf(float _x, float _y, int type);
	void SetState(int state);
};

