#include "Koopa.h"
#include "Animations.h"
#include "Sprites.h"
#include "debug.h"
#include "AssetIDs.h"
#include "Goomba.h"
#include "RewardingBrick.h"
#include "PlayScene.h"
void CKoopa::SetState(int state)
{
	boundaries_left = 0;
	boundaries_right = 0;
	switch (state) {
		case KOOPA_STATE_SHELL_STAY_STILL:
		{
			y -= (KOOPA_BBOX_HEIGHT_NORMAL - KOOPA_BBOX_HEIGHT_SHELL) / 2;
			vx = 0;
			timer = GetTickCount64();
			break;
		}
		case KOOPA_STATE_SHELL_MOVING:
		{
			vx = isPlayerLeft ? KOOPA_SHELL_SPEED : -KOOPA_SHELL_SPEED;
			timer = GetTickCount64();
			isCauseDamageOn = false;
			break;
		}
		case KOOPA_STATE_RETURNING_TO_NORMAL:
		{
			timer = GetTickCount64();
			break;
		}
		case KOOPA_STATE_NORMAL: 
		{
			y -= (KOOPA_BBOX_HEIGHT_NORMAL - KOOPA_BBOX_HEIGHT_SHELL) / 2;
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			float p_x, p_y;
			scene->GetPlayer()->GetPosition(p_x, p_y);
			vx = (x > p_x) ? -KOOPA_NORMAL_SPEED : KOOPA_NORMAL_SPEED;
			break;
		}
	}
	CGameObject::SetState(state);
}
void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_NORMAL) {
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_NORMAL / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_NORMAL;
	}
	else if (state == KOOPA_STATE_SHELL_STAY_STILL ||
		state == KOOPA_STATE_RETURNING_TO_NORMAL || 
		state == KOOPA_STATE_SHELL_MOVING) {

		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_SHELL;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	switch (state) {
		case KOOPA_STATE_NORMAL:
		{
			float temp = x + vx * dt;
			if (temp <= boundaries_left || temp >= boundaries_right) vx = -vx;
			break;
		}
		case KOOPA_STATE_SHELL_STAY_STILL: 
		{
			if (GetTickCount64() - timer > KOOPA_TIME_BEFORE_START_RETURNING_TO_NORMAL) {
				timer = 0;
				SetState(KOOPA_STATE_RETURNING_TO_NORMAL);
			}
			break;
		}
		case KOOPA_STATE_RETURNING_TO_NORMAL:
		{
			if (GetTickCount64() - timer > KOOPA_TIME_RETURNING_TO_NORMAL) {
				timer = 0;
				SetState(KOOPA_STATE_NORMAL);
			}
			break;
		}
		case KOOPA_STATE_SHELL_MOVING: 
		{
			if (GetTickCount64() - timer > KOOPA_TIME_NOT_CAUSE_DAMAGE) {
				timer = GetTickCount64();
				isCauseDamageOn = true;
			}
			break;
		}
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	if (koopaType == KOOPA_TYPE_RED) {
		if (state != KOOPA_STATE_SHELL_STAY_STILL) {
			int aniId = KOOPA_RED_ANI_ID_WALKING_LEFT;
			switch (state) {
			case KOOPA_STATE_NORMAL:
				aniId = (vx > 0) ? KOOPA_RED_ANI_ID_WALKING_RIGHT : KOOPA_RED_ANI_ID_WALKING_LEFT;
				break;
			case KOOPA_STATE_SHELL_MOVING:
				aniId = KOOPA_RED_ANI_ID_SHELL;
				break;
			case KOOPA_STATE_RETURNING_TO_NORMAL:
			{
				ULONGLONG t = GetTickCount64() - timer;
				if (t < KOOPA_TIME_RETURNING_TO_NORMAL / 2) {
					aniId = KOOPA_RED_ANI_ID_TRY_TO_POP_OUT_FULL_TIME;
				}
				else if (t < KOOPA_TIME_RETURNING_TO_NORMAL / 4) {
					aniId = KOOPA_RED_ANI_ID_TRY_TO_POP_OUT_HALF_TIME;
				}
				else if (t < KOOPA_TIME_RETURNING_TO_NORMAL) {
					aniId = KOOPA_RED_ANI_ID_TRY_TO_POP_OUT_QUARTER_TIME;
				}
				break;
			}
			}
			CAnimations* animations = CAnimations::GetInstance();
			animations->Get(aniId)->Render(x, y + KOOPA_RENDER_OFFSET_Y);
		}
		else {
			CSprites* sprites = CSprites::GetInstance();
			sprites->Get(KOOPA_RED_SPRITE_ID_SHELL)->Draw(x, y);
		}
	}
	else if (koopaType == KOOPA_TYPE_GREEN) {
		if (state != KOOPA_STATE_SHELL_STAY_STILL) {
			int aniId = KOOPA_GREEN_ANI_ID_WALKING_LEFT;
			switch (state) {
			case KOOPA_STATE_NORMAL:
				aniId = (vx > 0) ? KOOPA_GREEN_ANI_ID_WALKING_RIGHT : KOOPA_GREEN_ANI_ID_WALKING_LEFT;
				break;
			case KOOPA_STATE_SHELL_MOVING:
				aniId = KOOPA_GREEN_ANI_ID_SHELL;
				break;
			case KOOPA_STATE_RETURNING_TO_NORMAL:
			{
				ULONGLONG t = GetTickCount64() - timer;
				if (t < KOOPA_TIME_RETURNING_TO_NORMAL / 2) {
					aniId = KOOPA_GREEN_ANI_ID_TRY_TO_POP_OUT_FULL_TIME;
				}
				else if (t < KOOPA_TIME_RETURNING_TO_NORMAL / 4) {
					aniId = KOOPA_GREEN_ANI_ID_TRY_TO_POP_OUT_HALF_TIME;
				}
				else if (t < KOOPA_TIME_RETURNING_TO_NORMAL) {
					aniId = KOOPA_GREEN_ANI_ID_TRY_TO_POP_OUT_QUARTER_TIME;
				}
				break;
			}
			}
			CAnimations* animations = CAnimations::GetInstance();
			animations->Get(aniId)->Render(x, y + KOOPA_RENDER_OFFSET_Y);
		}
		else {
			CSprites* sprites = CSprites::GetInstance();
			sprites->Get(KOOPA_GREEN_SPRITE_ID_SHELL)->Draw(x, y);
		}
	}
	
	// RenderBoundingBox();
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->GetType() == OBJECT_TYPE_MARIO ||
		e->obj->GetType() == OBJECT_TYPE_GOOMBA) return;

	if (e->obj->GetType() == OBJECT_TYPE_REWARDING_BRICK && 
		e->obj->GetState() == REWARDING_BRICK_NORMAL_STATE &&
		e->nx != 0 &&
		state == KOOPA_STATE_SHELL_MOVING) {
		CRewardingBrick* obj = (CRewardingBrick*)e->obj;
		obj->SetRewardDirection(-e->nx);
		obj->SetState(REWARDING_BRICK_GO_UP_STATE);
	}

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (boundaries_left == 0 && boundaries_right == 0 && state != KOOPA_STATE_SHELL_MOVING) {
			float l, t, r, b;
			e->obj->GetBoundingBox(l, t, r, b);
			boundaries_left = l + KOOPA_BBOX_WIDTH / 2;
			boundaries_right = r - KOOPA_BBOX_WIDTH / 2;
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
}
