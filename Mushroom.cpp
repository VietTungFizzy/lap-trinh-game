#include "Mushroom.h"
#include "Sprites.h"
#include "debug.h"
#include "InvinsibleBrick.h"
#include "Mario.h"
#include "PlatformTopOnly.h"
void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state == MUSHROOM_STATE_ACTIVE) {
		vy += ay * dt;
		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
	else if (state == MUSHROOM_STATE_EATEN) {
		y += vy * dt;
		vy *= MUSHROOM_SPEED_REDUCTION;
		if (abs(vy) < MUSHROOM_EPSILON) Delete();
	}
}

void CMushroom::Render()
{
	if (state == MUSHROOM_STATE_ACTIVE || state == MUSHROOM_STATE_INACTIVE) {
		CSprites* s = CSprites::GetInstance();
		s->Get(MUSHROOM_SPRITE_ID)->Draw(x, y);
	}
	else if (state == MUSHROOM_STATE_EATEN) {
		CSprites* s = CSprites::GetInstance();
		s->Get(MUSHROOM_SCORE_TEXT_SPRITE_ID)->Draw(x, y);
	}
	
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
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

CMushroom::CMushroom(float x, float y): CGameObject(x, y)
{
	state = MUSHROOM_STATE_INVISIBLE;
	ay = MUSHROOM_GRAVITY;
	vx = -MUSHROOM_SPEED;
	
}

void CMushroom::SetState(int state)
{
	if (state == MUSHROOM_STATE_EATEN) {
		vy = -MUSHROOM_TEXT_GO_UP_SPEED;
	}
	CGameObject::SetState(state);
}
