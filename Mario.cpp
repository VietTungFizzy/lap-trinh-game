#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "RewardingBrick.h"
#include "Mushroom.h"
#include "AssetIDs.h"
#include "VenusFireTrap.h"
#include "Koopa.h"
#include "ParaGoomba.h"

#include "debug.h"
#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (abs(vx) > abs(maxVx)) {
		// Add momentum effect when mario change from walking to idle
		if (state == MARIO_STATE_IDLE) {
			if (abs(ax) < MINIMUM_ACCEL_VALUE) {
				vx = 0;
				ax = 0;
			}
		}
		else {
			vx = maxVx;
		}
	}
	if (y > bottomBoundary) {
		SetState(MARIO_STATE_DIE);
	}
	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;

		if (this->state == MARIO_STATE_BIG_TO_SMALL || this->state == MARIO_STATE_SMALL_TO_BIG) {
			state = MARIO_STATE_IDLE;
		}
	}
	isOnPlatform = false;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
	switch (e->obj->GetType()) {
	case OBJECT_TYPE_GOOMBA: OnCollisionWithGoomba(e); break;
	case OBJECT_TYPE_COIN:	OnCollisionWithCoin(e); break;
	case OBJECT_TYPE_PORTAL: OnCollisionWithPortal(e); break;
	case OBJECT_TYPE_REWARDING_BRICK: OnCollisionWithRewardingBrick(e); break;
	case OBJECT_TYPE_MUSHROOM:	OnCollisionWithMushroom(e); break;
	case OBJECT_TYPE_BULLET:
	case OBJECT_TYPE_VENUS_FIRE_TRAP:	
		GetHit(); 
		break;
	case OBJECT_TYPE_KOOPA: OnCollisionWithKoopa(e); break;
	case OBJECT_TYPE_PARA_GOOMBA: OnCollisionWithParaGoomba(e); break;
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE_FOR_MARIO)
		{
			goomba->SetState(GOOMBA_STATE_DIE_FOR_MARIO);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE_FOR_MARIO)
		{
			GetHit();
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	point+= COIN_POINT;
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithRewardingBrick(LPCOLLISIONEVENT e)
{
	if (e->ny > 0 && e->obj->GetState() == REWARDING_BRICK_NORMAL_STATE) {
		e->obj->SetState(REWARDING_BRICK_GO_UP_STATE);
		point += COIN_POINT;
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() == MUSHROOM_STATE_ACTIVE) {
		e->obj->SetState(MUSHROOM_STATE_EATEN);
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		e->obj->SetPosition(x, t );
		SetLevel(MARIO_LEVEL_BIG);
		StartUntouchable();
		SetState(MARIO_STATE_SMALL_TO_BIG);
		point += MUSHROOM_POINT;
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	int koopaState = koopa->GetState();
	if (e->ny < 0) {
		switch (koopaState) {
			case KOOPA_STATE_SHELL_MOVING:
			case KOOPA_STATE_NORMAL: {
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->SetState(KOOPA_STATE_SHELL_STAY_STILL);
				break;
			}
			case KOOPA_STATE_RETURNING_TO_NORMAL:
			case KOOPA_STATE_SHELL_STAY_STILL: 
			{
				bool isPlayerLeft = (vx > 0) ? false : true;
				koopa->SetIsPlayerLeft(isPlayerLeft);
				koopa->SetState(KOOPA_STATE_SHELL_MOVING);
				break;
			}
		}
	}
	else {
		switch (koopaState) {
			case KOOPA_STATE_NORMAL:
				GetHit();
				break;
			case KOOPA_STATE_RETURNING_TO_NORMAL:
			case KOOPA_STATE_SHELL_STAY_STILL:
			{
				bool isPlayerLeft = (e->nx > 0) ? false : true;
				koopa->SetIsPlayerLeft(isPlayerLeft);
				koopa->SetState(KOOPA_STATE_SHELL_MOVING);
				break;
			}
			case KOOPA_STATE_SHELL_MOVING:
				if(koopa->GetCauseDamageMode()) GetHit();
				break;
		}
	}
}

void CMario::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
	if (e->ny < 0) {
		int paraGoombaState = paraGoomba->GetState();
		switch (paraGoombaState) {
		case PARA_GOOMBA_STATE_LOST_WING: paraGoomba->SetState(PARA_GOOMBA_STATE_DIE); break;
		case PARA_GOOMBA_STATE_JUMP:
		case PARA_GOOMBA_STATE_WALK:
		case PARA_GOOMBA_STATE_FLY_UP:
			paraGoomba->SetState(PARA_GOOMBA_STATE_LOST_WING);
			break;
		}

		vy = (vx > 0) ? MARIO_JUMP_DEFLECT_SPEED : -MARIO_JUMP_DEFLECT_SPEED;
	}
	else {
		GetHit();
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (state == MARIO_STATE_IDLE) {
					aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_IDLE_RIGHT : ID_ANI_MARIO_SMALL_IDLE_LEFT;
				}
				else {
					if (nx > 0) {
						aniId = (ax == MARIO_ACCEL_RUN_X) ? ID_ANI_MARIO_SMALL_RUNNING_RIGHT : ID_ANI_MARIO_SMALL_WALKING_RIGHT;
					}
					else {
						aniId = (ax == -MARIO_ACCEL_RUN_X) ? ID_ANI_MARIO_SMALL_RUNNING_LEFT : ID_ANI_MARIO_SMALL_WALKING_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

void CMario::GetHit()
{
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (state == MARIO_STATE_IDLE) {
					aniId = (nx > 0) ? ID_ANI_MARIO_IDLE_RIGHT : ID_ANI_MARIO_IDLE_LEFT;
				}
				else {
					if (nx > 0) {
						aniId = (ax == MARIO_ACCEL_RUN_X) ? ID_ANI_MARIO_RUNNING_RIGHT : ID_ANI_MARIO_WALKING_RIGHT;
					}
					else {
						aniId = (ax == -MARIO_ACCEL_RUN_X) ? ID_ANI_MARIO_RUNNING_LEFT : ID_ANI_MARIO_WALKING_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else if (vx < 0)
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	switch (state) {
		case MARIO_STATE_DIE:
		{
			aniId = ID_ANI_MARIO_DIE;
			break;
		}
		case MARIO_STATE_SMALL_TO_BIG:
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_TO_BIG_RIGHT : ID_ANI_MARIO_SMALL_TO_BIG_LEFT;
			break;
		}
		case MARIO_STATE_BIG_TO_SMALL:
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_BIG_TO_SMALL_RIGHT : ID_ANI_MARIO_BIG_TO_SMALL_LEFT;
			break;
		}
		default:
		{
			if (level == MARIO_LEVEL_BIG)
				aniId = GetAniIdBig();
			else if (level == MARIO_LEVEL_SMALL)
				aniId = GetAniIdSmall();
			break;
		}
	}

	animations->Get(aniId)->Render(x, y);

	// RenderBoundingBox();
	
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE ||
		this->state == MARIO_STATE_BIG_TO_SMALL ||
		this->state == MARIO_STATE_SMALL_TO_BIG) return;
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax += (0 - ax) / MARIO_SLOW_DOWN_COEFFICIENT;
		maxVx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;

	case MARIO_STATE_BIG_TO_SMALL:
	case MARIO_STATE_SMALL_TO_BIG:
		ax = 0;
		vx = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

