#include "ParaGoomba.h"
#include "Animations.h"

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARA_GOOMBA_STATE_DIE)
	{
		left = x - PARA_GOOMBA_BBOX_WIDTH / 2;
		top = y - PARA_GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + PARA_GOOMBA_BBOX_WIDTH;
		bottom = top + PARA_GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - PARA_GOOMBA_BBOX_WIDTH / 2;
		top = y - PARA_GOOMBA_BBOX_HEIGHT / 2;
		right = left + PARA_GOOMBA_BBOX_WIDTH;
		bottom = top + PARA_GOOMBA_BBOX_HEIGHT;
	}
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CParaGoomba::Render()
{
	int aniId = PARA_GOOMBA_ID_ANI_DIE;
	
	CAnimations* animations = CAnimations::GetInstance();
	switch (state) {
		case PARA_GOOMBA_STATE_NORMAL:
		{
			aniId = PARA_GOOMBA_ID_ANI_WALK;
			animations->Get(PARA_GOOMBA_ID_ANI_WING_LEFT)->Render(x - PARA_GOOMBA_BBOX_WIDTH / 2 + PARA_GOOMBA_WING_RENDER_OFFSET_X, y - PARA_GOOMBA_WING_RENDER_OFFSET_Y);
			animations->Get(PARA_GOOMBA_ID_ANI_WING_RIGHT)->Render(x + PARA_GOOMBA_BBOX_WIDTH / 2 - PARA_GOOMBA_WING_RENDER_OFFSET_X, y - PARA_GOOMBA_WING_RENDER_OFFSET_Y);
			break;
		}
		case PARA_GOOMBA_STATE_DIE:
		{
			aniId = PARA_GOOMBA_ID_ANI_DIE;
			break;
		}
		case PARA_GOOMBA_STATE_LOST_WING:
		{
			aniId = PARA_GOOMBA_ID_ANI_WALK;
			break;
		}
	}
	animations->Get(aniId)->Render(x, y);
}

void CParaGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
}

CParaGoomba::CParaGoomba(float x, float y, int type) : CGameObject(x, y, type)
{
	state = PARA_GOOMBA_STATE_NORMAL;
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case PARA_GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (PARA_GOOMBA_BBOX_HEIGHT - PARA_GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	}
}
