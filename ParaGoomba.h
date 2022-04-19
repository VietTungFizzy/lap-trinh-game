#pragma once
#include "GameObject.h"
#define PARA_GOOMBA_GRAVITY 0.001f
#define PARA_GOOMBA_WALK_SPEED 0.05f
#define PARA_GOOMBA_SMALL_JUMP_SPEED 0.1f
#define PARA_GOOMBA_FLY_SPEED 0.3f

#define PARA_GOOMBA_BBOX_WIDTH 16
#define PARA_GOOMBA_BBOX_HEIGHT 14
#define PARA_GOOMBA_BBOX_HEIGHT_DIE 7
#define PARA_GOOMBA_RENDERING_OFFSET_Y 1

#define PARA_GOOMBA_DIE_TIMEOUT 500
#define PARA_GOOMBA_WALK_TIME	400

#define PARA_GOOMBA_STATE_FLY_UP 0
#define PARA_GOOMBA_STATE_JUMP	1
#define PARA_GOOMBA_STATE_WALK 2
#define PARA_GOOMBA_STATE_LOST_WING 3
#define PARA_GOOMBA_STATE_DIE 4

#define PARA_GOOMBA_ID_ANI_WALK	5014

#define PARA_GOOMBA_ID_ANI_WING_LEFT 5012
#define PARA_GOOMBA_ID_ANI_WING_RIGHT 5013
#define PARA_GOOMBA_ID_ANI_DIE	5015

#define PARA_GOOMBA_WING_RENDER_OFFSET_Y 6
#define PARA_GOOMBA_WING_RENDER_OFFSET_X 1
#define PARA_GOOMBA_RENDER_OFFSET_Y 1

#define PARA_GOOMBA_MAXIMUM_SMALL_JUMP 4
class CParaGoomba :
    public CGameObject
{
protected:
	float ay, ax;
	int jumpCount;
	ULONGLONG timer;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CParaGoomba(float x, float y, int type);
	virtual void SetState(int state);
};

