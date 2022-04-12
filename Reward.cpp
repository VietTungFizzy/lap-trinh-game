#include "Reward.h"
#include "Sprites.h"
#include "debug.h"
#include "InvinsibleBrick.h"
#include "Mario.h"
#include "PlatformTopOnly.h"
void CReward::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - REWARD_BBOX_WIDTH / 2;
	t = y - REWARD_BBOX_HEIGHT / 2;
	r = l + REWARD_BBOX_WIDTH;
	b = t + REWARD_BBOX_HEIGHT;
}

void CReward::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state == REWARD_STATE_ACTIVE) {
		vy += ay * dt;
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	else if (state == REWARD_STATE_EATEN) {
		y += vy * dt;
		vy *= REWARD_MUSHROOM_SPEED_REDUCTION;
		if (abs(vy) < REWARD_MUSHROOM_EPSILON) Delete();
	}
}

void CReward::Render()
{
	if (state == REWARD_STATE_ACTIVE || state == REWARD_STATE_INACTIVE) {
		CSprites* s = CSprites::GetInstance();
		s->Get(REWARD_MUSHROOM_SPRITE_ID)->Draw(x, y);
	}
	else if (state == REWARD_STATE_EATEN) {
		CSprites* s = CSprites::GetInstance();
		s->Get(REWARD_SCORE_TEXT_SPRITE_ID)->Draw(x, y);
	}
	
}

void CReward::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CReward::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CInvinsibleBrick*>(e->obj)) return;
	if (dynamic_cast<CMario*>(e->obj)) return;
	if (dynamic_cast<CPlatformTopOnly*>(e->obj)) { vy = 0; return; }
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

CReward::CReward(float x, float y, int type): CGameObject(x, y, type)
{
	state = REWARD_STATE_INVISIBLE;
	ay = REWARD_MUSHROOM_GRAVITY;
	vx = -REWARD_MUSHROOM_SPEED;
	
}

void CReward::SetState(int state)
{
	if (state == REWARD_STATE_EATEN) {
		vy = -REWARD_MUSHROOM_TEXT_GO_UP_SPEED;
	}
	CGameObject::SetState(state);
}
