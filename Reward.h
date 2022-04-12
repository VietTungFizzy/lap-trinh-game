#pragma once
#include "GameObject.h"
#define REWARD_MUSHROOM_SPRITE_ID 62000
#define REWARD_SCORE_TEXT_SPRITE_ID 61001

#define REWARD_STATE_INACTIVE 0
#define REWARD_STATE_ACTIVE 1
#define REWARD_STATE_INVISIBLE 2
#define	REWARD_STATE_EATEN 3

#define REWARD_BBOX_WIDTH 16
#define REWARD_BBOX_HEIGHT 14

#define REWARD_MUSHROOM_SPEED 0.05f
#define REWARD_MUSHROOM_GRAVITY 0.001f
#define REWARD_MUSHROOM_TEXT_GO_UP_SPEED 0.2f
#define REWARD_MUSHROOM_SPEED_REDUCTION 0.85f
#define REWARD_MUSHROOM_EPSILON (1e-3)

#define REWARD_MUSHROOM_DIRECTION_LEFT -1
#define REWARD_MUSHROOM_DIRECTION_RIGHT 1
class CReward :
    public CGameObject
{
private:
	float ay;

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return (state == REWARD_STATE_INACTIVE) ? 0 : 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CReward(float x, float y, int type);

	void SetState(int state);
};

