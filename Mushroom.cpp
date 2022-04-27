#include "Mushroom.h"
#include "Sprites.h"
#include "debug.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "ScoreText.h"
#include "Mario.h"
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
	if (state == MUSHROOM_STATE_ACTIVE || state == MUSHROOM_STATE_INACTIVE) {
		int spriteId = (type == MUSHROOM_TYPE_SUPER) ? MUSHROOM_SPRITE_ID_SUPER : MUSHROOM_SPRITE_ID_1_UP;
		CSprites* s = CSprites::GetInstance();
		s->Get(spriteId)->Draw(x, y);
	}
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->GetType() == OBJECT_TYPE_INVINSIBLE_BRICK ||
		e->obj->GetType() == OBJECT_TYPE_MARIO) return;
	if(e->obj->GetType() == OBJECT_TYPE_PLATFORM_TOP_ONLY) { vy = 0; return; }
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
}

CMushroom::CMushroom(float x, float y, int dir, int mushroomType, int objectType): CGameObject(x, y, objectType)
{
	state = MUSHROOM_STATE_INVISIBLE;
	ay = MUSHROOM_GRAVITY;
	vx = dir * MUSHROOM_SPEED;
	type = mushroomType;
}

void CMushroom::SetState(int state)
{
	if (state == MUSHROOM_STATE_EATEN) {
		Delete();
	}
	CGameObject::SetState(state);
}
