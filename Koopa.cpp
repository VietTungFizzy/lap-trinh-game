#include "Koopa.h"
#include "Animations.h"
void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_NORMAL) {
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_NORMAL / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_NORMAL;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_NORMAL) {
		float temp = x + vx * dt;
		if (temp <= boundaries_left || temp >= boundaries_right) vx = -vx;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	if (state != KOOPA_STATE_SHELL_STAY_STILL) {
		int aniId = KOOPA_ANI_ID_WALKING_LEFT;
		switch (state) {
		case KOOPA_STATE_NORMAL:
			aniId = (vx > 0) ? KOOPA_ANI_ID_WALKING_RIGHT : KOOPA_ANI_ID_WALKING_LEFT;
			break;
		case KOOPA_STATE_SHELL_MOVING:
			aniId = KOOPA_ANI_ID_SHELL;
			break;
		case KOOPA_STATE_RETURNING_TO_NORMAL:

			break;
		}
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(aniId)->Render(x, y);
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}
