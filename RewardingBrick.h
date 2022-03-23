#pragma once
#include "Brick.h"
#define REWARDING_BRICK_ANI_ID 10001
#define REWARDING_BRICK_SPRITE_ID 20009

#define REWARDING_BRICK_WIDTH 16
#define REWARDING_BRICK_BBOX_WIDTH 16
#define REWARDING_BRICK_BBOX_HEIGHT 16

#define REWARDING_COIN 0
#define REWARDING_MUSHROOM 1

#define REWARDING_BRICK_NORMAL_STATE 0
#define REWARDING_BRICK_TOUCHED_STATE 1

#define REWARDING_BRICK_GO_UP_SPEED 0.045f
#define REWARDING_BRICK_GO_UP_DISTANCE REWARDING_BRICK_WIDTH / 2
class CRewardingBrick :
    public CBrick
{
private:
	int rewarding;
	bool isGoUpEffectFinish;
	float initY;
public:
	CRewardingBrick(float x, float y,
		int rewarding, int spriteId = REWARDING_BRICK_SPRITE_ID) : CBrick(x, y, spriteId) 
	{
		this->state = REWARDING_BRICK_NORMAL_STATE;
		this->rewarding = rewarding;
		this->isGoUpEffectFinish = false;
		this->initY = y;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

