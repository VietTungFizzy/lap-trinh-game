#include "PSwitch.h"
#include "Animations.h"

void CPSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - P_SWITCH_BBOX_WIDTH / 2;
	top = y - P_SWITCH_BBOX_HEIGHT / 2;
	right = left + P_SWITCH_BBOX_WIDTH;
	bottom = top + P_SWITCH_BBOX_HEIGHT;
}

void CPSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPSwitch::Render()
{
	int aniId = (state == P_SWITCH_STATE_NORMAL) ? ID_ANI_P_SWITCH_NORMAL : ID_ANI_P_SWITCH_PRESSED;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CPSwitch::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CPSwitch::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
}
