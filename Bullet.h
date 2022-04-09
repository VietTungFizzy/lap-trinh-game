#pragma once
#include "GameObject.h"

#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 9

#define BULLET_ANI_ID 5500

#define BULLET_BOTTOM_LEFT_NEAR 0
#define BULLET_BOTTOM_LEFT_FAR 1
#define BULLET_BOTTOM_RIGHT_NEAR 2
#define BULLET_BOTTOM_RIGHT_FAR 3
#define BULLET_TOP_LEFT_NEAR 4
#define BULLET_TOP_LEFT_FAR 5
#define BULLET_TOP_RIGHT_NEAR 6
#define BULLET_TOP_RIGHT_FAR 7

#define BULLET_SPEED_X_FAR 0.07f
#define BULLET_SPEED_Y_FAR 0.025f
#define BULLET_SPEED_X_NEAR 0.04f
#define BULLET_SPEED_Y_NEAR 0.04f
class CBullet :
    public CGameObject
{
private:
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	virtual int IsBlocking() { return 0; }
	virtual int IsCollidable() { return 1; };
	CBullet(float x, float y, int type, int direction);
};

