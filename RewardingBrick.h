#pragma once
#include "Brick.h"
#define REWARDING_BRICK_ANI_ID 10001
#define REWARDING_BRICK_SPRITE_ID 20009
#define REWARDING_BRICK_TEXT_SPRIE_ID 61000
#define REWARDING_BRICK_COIN_ANI_ID 11000

#define REWARDING_BRICK_WIDTH 16
#define REWARDING_BRICK_BBOX_WIDTH 16
#define REWARDING_BRICK_BBOX_HEIGHT 16

#define REWARDING_COIN 0
#define REWARDING_MUSHROOM 1

#define REWARDING_BRICK_NORMAL_STATE 0
#define REWARDING_BRICK_GO_UP_STATE 1
#define REWARDING_BRICK_COIN_GO_UP_STATE 2
#define REWARDING_BRICK_COIN_GO_DOWN_STATE 3
#define REWARDING_BRICK_TEXT_GO_UP_STATE 4
#define REWARDING_BRICK_MUSHROOM_GO_UP_STATE 5
#define REWARDING_BRICK_FINISHED_STATE 6

#define REWARDING_BRICK_GO_UP_SPEED 0.05f
#define REWARDING_BRICK_COIN_SPEED 0.2f
#define REWARDING_BRICK_TEXT_SPEED 0.2f
#define REWARDING_BRICK_TEXT_SPEED_REDUCTION_COEFFECIENT 0.9f
#define REWARDING_BRICK_GO_UP_DISTANCE REWARDING_BRICK_WIDTH / 2
#define REWARING_BRICK_COIN_DISTANCE REWARDING_BRICK_BBOX_HEIGHT * 4
#define REWARDING_BRICK_MUSHROOM_GO_UP_DISTANCE 15
#define REWARDING_BRICK_MUSHROOM_GO_UP_SPEED 0.01f

#define REWARDING_BRICK_EPSILON (1e-3)
class CRewardingBrick :
    public CBrick
{
private:
	int rewarding;
	float initY;
	
	float xEffect;
	float yEffect;
	float vyEffect;

	// Only when rewarding type == REWARDING_MUSHROOM
	CGameObject* mushroom;
	CGameObject* player;
public:
	CRewardingBrick(float x, float y, int rewarding,int type, CGameObject* mushroom = NULL, int spriteId = REWARDING_BRICK_SPRITE_ID);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	void SetPlayer(CGameObject* player) { this->player = player; }
};

