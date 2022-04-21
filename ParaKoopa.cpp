#include "ParaKoopa.h"
#include "RewardingBrick.h"
#include "AssetIDs.h"
#include "Animations.h"
#include "PlayScene.h"
void CParaKoopa::SetState(int state)
{
	switch (state) {
	case PARA_KOOPA_STATE_SHELL_STAY_STILL:
	{
		y -= (PARA_KOOPA_BBOX_HEIGHT_NORMAL - PARA_KOOPA_BBOX_HEIGHT_SHELL) / 2;
		vx = 0;
		timer = GetTickCount64();
		break;
	}
	case PARA_KOOPA_STATE_SHELL_MOVING:
	{
		vx = isPlayerLeft ? PARA_KOOPA_SHELL_SPEED : -PARA_KOOPA_SHELL_SPEED;
		timer = GetTickCount64();
		isCauseDamageOn = false;
		break;
	}
	case PARA_KOOPA_STATE_RETURNING_TO_NORMAL:
	{
		timer = GetTickCount64();
		break;
	}
	case PARA_KOOPA_STATE_NORMAL_WITHOUT_WING:
	{
		// Is previous state is returning to normal
		if(this->state == PARA_KOOPA_STATE_RETURNING_TO_NORMAL) y -= (PARA_KOOPA_BBOX_HEIGHT_NORMAL - PARA_KOOPA_BBOX_HEIGHT_SHELL) / 2;
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		float p_x, p_y;
		scene->GetPlayer()->GetPosition(p_x, p_y);
		vx = (x > p_x) ? -PARA_KOOPA_NORMAL_SPEED : PARA_KOOPA_NORMAL_SPEED;
		break;
	}
	case PARA_KOOPA_STATE_NORMAL_WITH_WING:
	{
		vy = -PARA_KOOPA_JUMP_SPEED;
		break;
	}
	}
	CGameObject::SetState(state);
}

void CParaKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (state) {
		case PARA_KOOPA_STATE_NORMAL_WITH_WING:
		case PARA_KOOPA_STATE_NORMAL_WITHOUT_WING:
			left = x - PARA_KOOPA_BBOX_WIDTH / 2;
			top = y - PARA_KOOPA_BBOX_HEIGHT_NORMAL / 2;
			right = left + PARA_KOOPA_BBOX_WIDTH;
			bottom = top + PARA_KOOPA_BBOX_HEIGHT_NORMAL;
			break;
		case PARA_KOOPA_STATE_SHELL_STAY_STILL:
		case PARA_KOOPA_STATE_RETURNING_TO_NORMAL:
		case PARA_KOOPA_STATE_SHELL_MOVING:
			left = x - PARA_KOOPA_BBOX_WIDTH / 2;
			top = y - PARA_KOOPA_BBOX_HEIGHT_SHELL / 2;
			right = left + PARA_KOOPA_BBOX_WIDTH;
			bottom = top + PARA_KOOPA_BBOX_HEIGHT_SHELL;
			break;
	}
}

void CParaKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	switch (state) {
		case PARA_KOOPA_STATE_SHELL_STAY_STILL:
		{
			if (GetTickCount64() - timer > PARA_KOOPA_TIME_BEFORE_START_RETURNING_TO_NORMAL) {
				timer = 0;
				SetState(PARA_KOOPA_STATE_RETURNING_TO_NORMAL);
			}
			break;
		}
		case PARA_KOOPA_STATE_RETURNING_TO_NORMAL:
		{
			if (GetTickCount64() - timer > PARA_KOOPA_TIME_RETURNING_TO_NORMAL) {
				timer = 0;
				SetState(PARA_KOOPA_STATE_NORMAL_WITHOUT_WING);
			}
			break;
		}
		case PARA_KOOPA_STATE_SHELL_MOVING:
		{
			if (GetTickCount64() - timer > PARA_KOOPA_TIME_NOT_CAUSE_DAMAGE) {
				timer = GetTickCount64();
				isCauseDamageOn = true;
			}
			break;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CParaKoopa::Render()
{
	int aniId = PARA_KOOPA_ANI_ID_WALKING_LEFT;
	CAnimations* animations = CAnimations::GetInstance();
	switch (state) {
		case PARA_KOOPA_STATE_NORMAL_WITH_WING:
		{
			aniId = (vx > 0) ? PARA_KOOPA_ANI_ID_WALKING_RIGHT : PARA_KOOPA_ANI_ID_WALKING_LEFT;
			if (vx > 0) {
				animations->Get(PARA_KOOPA_ANI_ID_WING_RIGHT)->Render(x - PARA_KOOPA_RENDER_WING_OFFSET_X, y - PARA_KOOPA_RENDER_WING_OFFSET_Y);
			}
			else {
				animations->Get(PARA_KOOPA_ANI_ID_WING_LEFT)->Render(x + PARA_KOOPA_RENDER_WING_OFFSET_X, y - PARA_KOOPA_RENDER_WING_OFFSET_Y);
			}
			break;
		}
		case PARA_KOOPA_STATE_NORMAL_WITHOUT_WING:
		{
			aniId = (vx > 0) ? PARA_KOOPA_ANI_ID_WALKING_RIGHT : PARA_KOOPA_ANI_ID_WALKING_LEFT;
			break;
		}
	}
	animations->Get(aniId)->Render(x, y + PARA_KOOPA_RENDER_OFFSET_Y);
}

void CParaKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CParaKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->GetType() == OBJECT_TYPE_MARIO) return;

	if (e->obj->GetType() == OBJECT_TYPE_REWARDING_BRICK &&
		e->obj->GetState() == REWARDING_BRICK_NORMAL_STATE &&
		e->nx != 0 &&
		state == PARA_KOOPA_STATE_SHELL_MOVING) {
		CRewardingBrick* obj = (CRewardingBrick*)e->obj;
		obj->SetRewardDirection(-e->nx);
		obj->SetState(REWARDING_BRICK_GO_UP_STATE);
	}

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = -PARA_KOOPA_JUMP_SPEED;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
}
