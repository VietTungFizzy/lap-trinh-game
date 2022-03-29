#include "Mushroom.h"
#include "Sprites.h"
#include "debug.h"
#include "InvinsibleBrick.h"
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
}

void CMushroom::Render()
{
	if (state == MUSHROOM_STATE_INVISIBLE) return;
	CSprites* s = CSprites::GetInstance();
	s->Get(MUSHROOM_SPRITE_ID)->Draw(x, y);
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CInvinsibleBrick*>(e->obj)) return;

	if (e->ny != 0)
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
	vx = MUSHROOM_SPEED;
}
