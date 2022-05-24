#pragma once
#include "Brick.h"
#define ID_ANI_REWARDING_BRICK_NORMAL_ITEM 10001
#define ID_ANI_REWARDING_BRICK_SPECIAL_ITEM 10002
#define ID_SPRITE_REWARDING_BRICK_FINISHED 20009
#define ID_ANI_REWARDING_BRICK_COIN 11000

#define REWARDING_BRICK_WIDTH 16
#define REWARDING_BRICK_BBOX_WIDTH 16
#define REWARDING_BRICK_BBOX_HEIGHT 16

#define REWARDING_COIN 0
#define REWARDING_SUPER_ITEM 1
#define REWARDING_1_UP_MUSHROOM 2
#define REWARDING_P_SWITCH 3

#define REWARDING_BRICK_NORMAL_STATE 0
#define REWARDING_BRICK_GO_UP_STATE 1
#define REWARDING_BRICK_COIN_GO_UP_STATE 2
#define REWARDING_BRICK_COIN_GO_DOWN_STATE 3
#define REWARDING_BRICK_TEXT_GO_UP_STATE 4
#define REWARDING_BRICK_FINISHED_STATE 6

#define REWARDING_BRICK_GO_UP_SPEED 0.05f
#define REWARDING_BRICK_COIN_SPEED 0.2f
#define REWARDING_BRICK_GO_UP_DISTANCE REWARDING_BRICK_WIDTH / 2
#define REWARING_BRICK_COIN_DISTANCE REWARDING_BRICK_BBOX_HEIGHT * 4

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

	// Only when rewarding type == REWARDING_SUPER_ITEM
	
	int rewardDirection;
public:
	CRewardingBrick(float x, float y, int rewarding,int type, int spriteId = ID_SPRITE_REWARDING_BRICK_FINISHED);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);

	void SetRewardDirection(int direction) { this->rewardDirection = direction; }
};

