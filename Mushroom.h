#pragma once
#include "GameObject.h"
#define MUSHROOM_SPRITE_ID 62000
#define MUSHROOM_SCORE_TEXT_SPRITE_ID 61001

#define MUSHROOM_STATE_INACTIVE 0
#define MUSHROOM_STATE_ACTIVE 1
#define MUSHROOM_STATE_INVISIBLE 2
#define	MUSHROOM_STATE_EATEN 3

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 14

#define MUSHROOM_SPEED 0.05f
#define MUSHROOM_GRAVITY 0.001f
#define MUSHROOM_TEXT_GO_UP_SPEED 0.2f
#define MUSHROOM_SPEED_REDUCTION 0.85f
#define MUSHROOM_EPSILON (1e-3)

class CMushroom :
    public CGameObject
{
private:
	float ay;

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return (state == MUSHROOM_STATE_INACTIVE) ? 0 : 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CMushroom(float x, float y, int type);

	void SetState(int state);
};

