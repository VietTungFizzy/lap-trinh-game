#include "RewardingBrick.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "debug.h"
void CRewardingBrick::Render()
{
	if (state == REWARDING_BRICK_NORMAL_STATE) {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(REWARDING_BRICK_ANI_ID)->Render(x, y);
	}
	else {
		CSprites* s = CSprites::GetInstance();
		s->Get(this->spriteId)->Draw(x, y);

		if (state == REWARDING_BRICK_COIN_GO_UP_STATE || state == REWARDING_BRICK_COIN_GO_DOWN_STATE) {
			CAnimations* animations = CAnimations::GetInstance();
			animations->Get(REWARDING_BRICK_COIN_ANI_ID)->Render(xEffect, yEffect);
		}
		if (state == REWARDING_BRICK_TEXT_GO_UP_STATE) {
			s->Get(REWARDING_BRICK_TEXT_SPRIE_ID)->Draw(xEffect, yEffect);
		}
	}
}

void CRewardingBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state) {
		case REWARDING_BRICK_GO_UP_STATE:
		{
			y -= vy * dt;
			if (abs(y - initY) > REWARDING_BRICK_GO_UP_DISTANCE) {
				float temp = initY;
				y = initY;
				initY = temp - REWARDING_BRICK_BBOX_HEIGHT;
				yEffect = temp - REWARDING_BRICK_BBOX_HEIGHT;
				vy = 0;
				if (rewarding == REWARDING_COIN) {
					SetState(REWARDING_BRICK_COIN_GO_UP_STATE);
				}
			}
			break;
		}
		case REWARDING_BRICK_COIN_GO_UP_STATE:
		{
			yEffect += vyEffect * dt;
			if (abs(yEffect - initY) > REWARING_BRICK_COIN_DISTANCE) {
				SetState(REWARDING_BRICK_COIN_GO_DOWN_STATE);
			}
			break;
		}
		case REWARDING_BRICK_COIN_GO_DOWN_STATE:
		{
			yEffect += vyEffect * dt;
			if (yEffect > initY) {
				SetState(REWARDING_BRICK_TEXT_GO_UP_STATE);
			}
			break;
		}
		case REWARDING_BRICK_TEXT_GO_UP_STATE:
		{
			yEffect += vyEffect * dt;
			vyEffect *= REWARDING_BRICK_TEXT_SPEED_REDUCTION_COEFFECIENT;
			DebugOutTitle(L"vy: %f", vyEffect);
			if (abs(vyEffect) < REWARDING_BRICK_EPSILON) SetState(REWARDING_BRICK_FINISHED_STATE);
			break;
		}
		default: break;
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
	switch (state) {
		case REWARDING_BRICK_GO_UP_STATE:
		{
			vy = REWARDING_BRICK_GO_UP_SPEED;
			break;
		}
		case REWARDING_BRICK_COIN_GO_UP_STATE:
		{
			vyEffect = -REWARDING_BRICK_COIN_SPEED;
			break;
		}
		case REWARDING_BRICK_COIN_GO_DOWN_STATE:
		{
			vyEffect = REWARDING_BRICK_COIN_SPEED;
			break;
		}
		case REWARDING_BRICK_TEXT_GO_UP_STATE:
		{
			vyEffect = -REWARDING_BRICK_TEXT_SPEED;
			break;
		}
		default: break;
	}
	CGameObject::SetState(state);
}
