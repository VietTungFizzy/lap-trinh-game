#include "RewardingBrick.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "debug.h"
void CRewardingBrick::Render()
{
	if (state == REWARDING_BRICK_TOUCHED_STATE) {
		CSprites* s = CSprites::GetInstance();
		s->Get(this->spriteId)->Draw(x, y);
	}
	else {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(REWARDING_BRICK_ANI_ID)->Render(x, y);
	}
}

void CRewardingBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == REWARDING_BRICK_TOUCHED_STATE && !isGoUpEffectFinish) {
		y -= vy * dt;
		if (abs(y - initY) > REWARDING_BRICK_GO_UP_DISTANCE) {
			y = initY;
			vy = 0;
			isGoUpEffectFinish = true;
		}
	}
}

void CRewardingBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - REWARDING_BRICK_BBOX_WIDTH / 2;
	t = y - REWARDING_BRICK_BBOX_HEIGHT / 2;
	r = l + REWARDING_BRICK_BBOX_WIDTH;
	b = t + REWARDING_BRICK_BBOX_HEIGHT;
}

void CRewardingBrick::SetState(int state)
{
	if (state == REWARDING_BRICK_TOUCHED_STATE) {		
		vy = REWARDING_BRICK_GO_UP_SPEED;
		CGameObject::SetState(state);
	}
}
