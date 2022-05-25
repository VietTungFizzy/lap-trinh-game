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
#include "ScoreText.h"
#include "PlayScene.h"
#include "ParaKoopa.h"
#include "SwitchBrick.h"
#include "PSwitch.h"

#include "debug.h"
#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	/*
	if (!isSlowFalling) {
		vy += ay * dt;
		vx += ax * dt;
	}
	else {
		vy += MARIO_GRAVITY_SLOW_FALLING * dt;
		vx += ax * dt;
	}
	*/
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
		ResetUntouchable();
	}
	
	// Handling power logic
	if (isMaxPower) {
		if (GetTickCount64() - countdown_timer > MARIO_MAX_POWER_DECREASE_TIME && !isOnPlatform) {
			isMaxPower = false;
			powerCount--;
		}
	}
	else {
		if (abs(vx) != MARIO_RUNNING_SPEED) {
			powerCount--;
			countdown_timer = 0;
		}
		else powerCount++;
	}
	if (powerCount < 0) powerCount = 0;
	if (powerCount > MARIO_MAX_POWER) powerCount = MARIO_MAX_POWER;
	isMaxPower = (powerCount >= MARIO_MAX_POWER);
	if (isMaxPower && countdown_timer == 0) {
		countdown_timer = GetTickCount64();
	}

	// Handling grabbing logic
	if (isGrabbing) {
		CKoopa * koopa = (CKoopa*)grabbedObj;
		if (!koopa->GetGrabbedFlag()) {
			isGrabbing = false;
			grabbedObj = nullptr;
		}
		else {
			float l, t, r, b; // mario bounding box
			float gx, gy; // grabbed object x and y
			GetBoundingBox(l, t, r, b);
			if (nx > 0) {
				gx = r + (grabbedObj->GetType() == OBJECT_TYPE_KOOPA ? KOOPA_BBOX_WIDTH : PARA_KOOPA_BBOX_WIDTH) / 2 - MARIO_GRABBING_OBJ_OFFSET;
			}
			else {
				gx = l - (grabbedObj->GetType() == OBJECT_TYPE_KOOPA ? KOOPA_BBOX_WIDTH : PARA_KOOPA_BBOX_WIDTH) / 2 + MARIO_GRABBING_OBJ_OFFSET;
			}
			gx += vx * dt;
			gy = y + vy * dt;
			if (level == MARIO_LEVEL_SMALL) {
				gy -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 4;
			}
			grabbedObj->SetPosition(gx, gy);
		}
	}

	// Handling tail attack logic
	if (isWaggingTail) {
		if (GetTickCount64() - short_action_timer > MARIO_TAIL_ATTACK_TIME) {
			isWaggingTail = false;
			short_action_timer = 0;
		}
	}

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
		if (e->ny < 0) {
			isOnPlatform = true;
			comboCount = NO_COMBO;
		}
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
	case OBJECT_TYPE_PIRANHA_PLANT:
	case OBJECT_TYPE_BULLET:
	case OBJECT_TYPE_VENUS_FIRE_TRAP:	
		GetHit(); 
		break;
	case OBJECT_TYPE_KOOPA: OnCollisionWithKoopa(e); break;
	case OBJECT_TYPE_PARA_GOOMBA: OnCollisionWithParaGoomba(e); break;
	case OBJECT_TYPE_PARA_KOOPA: OnCollisionWithParaKoopa(e); break;
	case OBJECT_TYPE_SUPER_LEAF: OnCollisionWithSuperLeaf(e); break;
	case OBJECT_TYPE_SWITCH_BRICK: OnCollisionWithSwitchBrick(e); break;
	case OBJECT_TYPE_P_SWITCH: OnCollisionWithPSwitch(e); break;
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetState() != GOOMBA_STATE_DIE_FOR_MARIO && goomba->GetState() != GOOMBA_STATE_DIE_FOR_SHELL) {
		// jump on top >> kill Goomba and deflect a bit 
		if (e->ny < 0)
		{
			goomba->SetState(GOOMBA_STATE_DIE_FOR_MARIO);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else // hit by Goomba
		{
			GetHit();
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	point+= SCORE_POINT_100;
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
		point += SCORE_POINT_100;
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() == MUSHROOM_STATE_ACTIVE) {
		e->obj->SetState(MUSHROOM_STATE_EATEN);
		// Align scoring text to above mario head
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		e->obj->SetPosition(x, t );
		CMushroom* mushroom = (CMushroom*)e->obj;
		if (mushroom->GetMushroomType() == MUSHROOM_TYPE_SUPER) {
			SetLevel(MARIO_LEVEL_BIG);
			StartUntouchable();
			SetState(MARIO_STATE_SMALL_TO_BIG);
			ScoringPointWithoutCombo(SCORE_POINT_1000);
		}
		else {
			DebugOut(L">>>>>>>>> Add 1 life to player <<<<<<<<");
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	int koopaState = koopa->GetState();
	if (e->ny != 0) {
		if (e->ny < 0) {
			switch (koopaState) {
			case KOOPA_STATE_SHELL_MOVING:
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->SetState(KOOPA_STATE_SHELL_STAY_STILL);
				ScoringWithCombo();
				break;
			}
			case KOOPA_STATE_NORMAL: {
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->SetState(KOOPA_STATE_SHELL_STAY_STILL);
				ScoringWithCombo();
				break;
			}
			case KOOPA_STATE_RETURNING_TO_NORMAL:
			case KOOPA_STATE_SHELL_STAY_STILL:
			{
				bool isPlayerLeft = (vx > 0) ? false : true;
				koopa->SetIsPlayerLeft(isPlayerLeft);
				koopa->SetState(KOOPA_STATE_SHELL_MOVING);
				ScoringWithCombo();
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
				koopa->SetIsPlayerLeft((e->nx < 0));
				koopa->SetState(KOOPA_STATE_SHELL_MOVING);
				break;
			}
			case KOOPA_STATE_SHELL_MOVING:
				if (koopa->GetCauseDamageMode()) GetHit();
				break;
			}
		}
	}
	else if (e->nx != 0) {
		if (koopa->GetGrabbedFlag()) return;
		if (koopa->GetState() == KOOPA_STATE_SHELL_STAY_STILL) {
			CGame* game = CGame::GetInstance();
			if (game->IsKeyDown(DIK_A)) {
				koopa->SetGrabbedFlag(true);
				isGrabbing = true;
				grabbedObj = koopa;
				return;
			}
			
			bool isPlayerLeft = (e->nx > 0) ? false : true;
			koopa->SetIsPlayerLeft(isPlayerLeft);
			koopa->SetState(PARA_KOOPA_STATE_SHELL_MOVING);
			return;
		}

		GetHit();
	}
	
}

void CMario::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
	int paraGoombaState = paraGoomba->GetState();
	if (paraGoombaState != PARA_GOOMBA_STATE_DIE) {
		if (e->ny < 0) {

			switch (paraGoombaState) {
			case PARA_GOOMBA_STATE_LOST_WING: paraGoomba->SetState(PARA_GOOMBA_STATE_DIE); break;
			case PARA_GOOMBA_STATE_JUMP:
			case PARA_GOOMBA_STATE_WALK:
			case PARA_GOOMBA_STATE_FLY_UP:
				paraGoomba->SetState(PARA_GOOMBA_STATE_LOST_WING);
				ScoringWithCombo();
				break;
			}

			vy = (vx > 0) ? MARIO_JUMP_DEFLECT_SPEED : -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {
			GetHit();
		}
	}
}

void CMario::OnCollisionWithParaKoopa(LPCOLLISIONEVENT e)
{
	CParaKoopa* koopa = dynamic_cast<CParaKoopa*>(e->obj);
	int koopaState = koopa->GetState();
	if (e->ny != 0) {
		if (e->ny < 0) {
			switch (koopaState) {
			case PARA_KOOPA_STATE_SHELL_MOVING:
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->SetState(PARA_KOOPA_STATE_SHELL_STAY_STILL);
				ScoringWithCombo();
				break;
			}
			case PARA_KOOPA_STATE_NORMAL_WITHOUT_WING: {
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->SetState(PARA_KOOPA_STATE_SHELL_STAY_STILL);
				ScoringWithCombo();
				break;
			}
			case PARA_KOOPA_STATE_NORMAL_WITH_WING: {
				vy = -MARIO_JUMP_DEFLECT_SPEED;
				koopa->SetState(PARA_KOOPA_STATE_NORMAL_WITHOUT_WING);
				ScoringWithCombo();
				break;
			}
			case PARA_KOOPA_STATE_RETURNING_TO_NORMAL:
			case PARA_KOOPA_STATE_SHELL_STAY_STILL:
			{
				bool isPlayerLeft = (vx > 0) ? false : true;
				koopa->SetIsPlayerLeft(isPlayerLeft);
				koopa->SetState(PARA_KOOPA_STATE_SHELL_MOVING);
				ScoringWithCombo();
				break;
			}
			}
		}
		else {
			switch (koopaState) {
			case PARA_KOOPA_STATE_NORMAL_WITHOUT_WING:
			case PARA_KOOPA_STATE_NORMAL_WITH_WING:
				GetHit();
				break;
			case PARA_KOOPA_STATE_RETURNING_TO_NORMAL:
			case PARA_KOOPA_STATE_SHELL_STAY_STILL:
			{
				bool isPlayerLeft = (e->nx > 0) ? false : true;
				koopa->SetIsPlayerLeft(isPlayerLeft);
				koopa->SetState(PARA_KOOPA_STATE_SHELL_MOVING);
				break;
			}
			case PARA_KOOPA_STATE_SHELL_MOVING:
				if (koopa->GetCauseDamageMode()) GetHit();
				break;
			}
		}
	}
	else if (e->nx != 0) {
		if (koopa->GetGrabbedFlag()) return;
		if (koopa->GetState() == PARA_KOOPA_STATE_SHELL_STAY_STILL) {
			CGame* game = CGame::GetInstance();
			if (game->IsKeyDown(DIK_A)) {
				koopa->SetGrabbedFlag(true);
				isGrabbing = true;
				grabbedObj = koopa;
				return;
			}
			bool isPlayerLeft = (e->nx > 0) ? false : true;
			koopa->SetIsPlayerLeft(isPlayerLeft);
			koopa->SetState(PARA_KOOPA_STATE_SHELL_MOVING);
			return;
		}

		GetHit();
	}
}

void CMario::OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	if (level != MARIO_LEVEL_RACCOON) {
		SetState(MARIO_STATE_BIG_TO_RACCOON);
		SetLevel(MARIO_LEVEL_RACCOON);
	}
	else {
		ScoringPointWithoutCombo(SCORE_POINT_1000);
	}
	
}

void CMario::OnCollisionWithSwitchBrick(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() == SWITCH_BRICK_STATE_COIN) {
		e->obj->SetState(SWITCH_BRICK_STATE_COIN_EATEN);
	}
}

void CMario::OnCollisionWithPSwitch(LPCOLLISIONEVENT e)
{
	if (e->obj->GetState() == P_SWITCH_STATE_NORMAL) {
		if (e->ny < 0) {
			e->obj->SetState(P_SWITCH_STATE_PRESSED);
			((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->SetSwitchOn();
		}
		else if (e->nx != 0) {
			vx = 0;
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (state == MARIO_STATE_KICKING) {
		aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_KICKING_RIGHT : ID_ANI_MARIO_SMALL_KICKING_LEFT;
		return aniId;
	}

	if (!isOnPlatform)
	{
		if (isGrabbing) {
			aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_JUMP_WHILE_GRABBING_RIGHT : ID_ANI_MARIO_SMALL_JUMP_WHILE_GRABBING_LEFT;
		}
		else {
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
					if (isGrabbing) {
						aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_GRABBING_IDLE_RIGHT : ID_ANI_MARIO_SMALL_GRABBING_IDLE_LEFT;
					}
					else {
						aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_IDLE_RIGHT : ID_ANI_MARIO_SMALL_IDLE_LEFT;
					}
				}
				else {
					if (isGrabbing) {
						aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_GRABBING_WALK_RIGHT : ID_ANI_MARIO_SMALL_GRABBING_WALK_LEFT;
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
			}
			else if (vx > 0)
			{
				if (isGrabbing) {
					aniId = (isMaxPower) ? ID_ANI_MARIO_SMALL_GRABBING_RUNNING_RIGHT : ID_ANI_MARIO_SMALL_GRABBING_WALK_RIGHT;
				}
				else {
					if (ax < 0)
						aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
						aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
					else
						aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
				}
			}
			else // vx < 0
			{
				if (isGrabbing) {
					aniId = (isMaxPower) ? ID_ANI_MARIO_SMALL_GRABBING_RUNNING_LEFT : ID_ANI_MARIO_SMALL_GRABBING_WALK_LEFT;
				}
				else {
					if (ax > 0)
						aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X)
						aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
					else
						aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
				}
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRaccoon()
{
	int aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
	if (state == MARIO_STATE_KICKING) {
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_KICKING_RIGHT : ID_ANI_MARIO_RACCOON_KICKING_LEFT;
		return aniId;
	}
	if (isWaggingTail) {
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_TURNING_FROM_RIGHT_TO_LEFT : ID_ANI_MARIO_RACCOON_TURNING_FROM_LEFT_TO_RIGHT;
		return aniId;
	}

	if (!isOnPlatform) {
		if (isGrabbing) {
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_JUMP_WHILE_GRABBING_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_WHILE_GRABBING_LEFT;
		}
		else if (!isMaxPower) 
		{
			if(isSlowFalling) {
				aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_SLOW_FALLING_RIGHT : ID_ANI_MARIO_RACCOON_SLOW_FALLING_LEFT;
			}
			else {
				aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_JUMP_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_LEFT;
			}
		}
		else {
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_FLY_RIGHT : ID_ANI_MARIO_RACCOON_FLY_LEFT;
		}
	}
	else {
		if (isSitting) {
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_SIT_RIGHT : ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else {
			if (vx == 0) {
				if (state == MARIO_STATE_IDLE) {
					if (isGrabbing) {
						aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_GRABBING_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_GRABBING_IDLE_LEFT;
					}
					else {
						aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_IDLE_LEFT;
					}
				}
				else {
					if (isGrabbing) {
						aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_GRABBING_WALK_RIGHT : ID_ANI_MARIO_RACCOON_GRABBING_WALK_LEFT;
					}
					else {
						if (nx > 0) {
							aniId = (ax == MARIO_ACCEL_RUN_X) ? ID_ANI_MARIO_RACCOON_RUN_NO_POWER_RIGHT : ID_ANI_MARIO_RACCOON_WALK_RIGHT;
						}
						else {
							aniId = (ax == -MARIO_ACCEL_RUN_X) ? ID_ANI_MARIO_RACCOON_RUN_NO_POWER_LEFT : ID_ANI_MARIO_RACCOON_WALK_LEFT;
						}
					}
				}
			}
			else if (vx > 0) {
				if (isGrabbing) {
					aniId = (isMaxPower) ? ID_ANI_MARIO_RACCOON_GRABBING_RUNNING_RIGHT : ID_ANI_MARIO_RACCOON_GRABBING_WALK_RIGHT;
				}
				else {
					if (ax < 0) {
						aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
					}
					else {
						if (ax == MARIO_ACCEL_RUN_X) {
							aniId = (isMaxPower) ? ID_ANI_MARIO_RACCOON_RUN_FULL_POWER_RIGHT : ID_ANI_MARIO_RACCOON_RUN_NO_POWER_RIGHT;
						}
						else {
							aniId = ID_ANI_MARIO_RACCOON_WALK_RIGHT;
						}
					}
				}
				
			}
			else { // vx < 0
				if (isGrabbing) {
					aniId = (isMaxPower) ? ID_ANI_MARIO_RACCOON_GRABBING_RUNNING_LEFT : ID_ANI_MARIO_RACCOON_GRABBING_WALK_LEFT;
				}
				else {
					if (ax > 0) {
						aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
					}
					else {
						if (ax == -MARIO_ACCEL_RUN_X) {
							aniId = (isMaxPower) ? ID_ANI_MARIO_RACCOON_RUN_FULL_POWER_LEFT : ID_ANI_MARIO_RACCOON_RUN_NO_POWER_LEFT;
						}
						else {
							aniId = ID_ANI_MARIO_RACCOON_WALK_LEFT;
						}
					}
				}
			}
		}
	}
	return aniId;
}

void CMario::GetHit()
{
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			if (level == MARIO_LEVEL_RACCOON) {
				SetLevel(MARIO_LEVEL_BIG);
				SetState(MARIO_STATE_RACCOON_TO_BIG);
			}
			else {
				// SetLevel(MARIO_LEVEL_SMALL);
				SetState(MARIO_STATE_BIG_TO_SMALL);
			}
			
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::ScoringWithCombo()
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	comboCount++;
	switch (comboCount) {
		case COMBO_100:
		{
			point += SCORE_POINT_100;
			scene->AddObjects(new CScoreText(x, y, SCORE_TEXT_100, OBJECT_TYPE_SCORE_TEXT));
			break;
		}
		case COMBO_200:
		{
			point += SCORE_POINT_200;
			scene->AddObjects(new CScoreText(x, y, SCORE_TEXT_200, OBJECT_TYPE_SCORE_TEXT));
			break;
		}
		case COMBO_400:
		{
			point += SCORE_POINT_400;
			scene->AddObjects(new CScoreText(x, y, SCORE_TEXT_400, OBJECT_TYPE_SCORE_TEXT));
			break;
		}
	}
}

//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (state == MARIO_STATE_KICKING) {
		aniId = (nx > 0) ? ID_ANI_MARIO_KICKING_RIGHT : ID_ANI_MARIO_KICKING_LEFT;
		return aniId;
	}

	if (!isOnPlatform)
	{
		if (isGrabbing) {
			aniId = (nx > 0) ? ID_ANI_MARIO_GRABBING_WALK_RIGHT : ID_ANI_MARIO_GRABBING_WALK_LEFT;
		}
		else {
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
					if (isGrabbing) {
						aniId = (nx > 0) ? ID_ANI_MARIO_GRABBING_IDLE_RIGHT : ID_ANI_MARIO_GRABBING_IDLE_LEFT;
					}
					else {
						aniId = (nx > 0) ? ID_ANI_MARIO_IDLE_RIGHT : ID_ANI_MARIO_IDLE_LEFT;
					}
				}
				else {
					if (isGrabbing) {
						aniId = (nx > 0) ? ID_ANI_MARIO_GRABBING_WALK_RIGHT : ID_ANI_MARIO_GRABBING_WALK_LEFT;
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
			}
			else if (vx > 0)
			{
				if (isGrabbing) {
					aniId = (isMaxPower) ? ID_ANI_MARIO_GRABBING_RUNNING_RIGHT : ID_ANI_MARIO_GRABBING_WALK_RIGHT;
				}
				else {
					if (ax < 0)
						aniId = ID_ANI_MARIO_BRACE_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
						aniId = ID_ANI_MARIO_RUNNING_RIGHT;
					else
						aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
				
			}
			else if (vx < 0)
			{
				if (isGrabbing) {
					aniId = (isMaxPower) ? ID_ANI_MARIO_GRABBING_RUNNING_LEFT : ID_ANI_MARIO_GRABBING_WALK_LEFT;
				}
				else {
					if (ax > 0)
						aniId = ID_ANI_MARIO_BRACE_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X)
						aniId = ID_ANI_MARIO_RUNNING_LEFT;
					else
						aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
				
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
		case MARIO_STATE_RACCOON_TO_BIG:
		case MARIO_STATE_BIG_TO_RACCOON:
		{
			aniId = ID_ANI_MARIO_BIG_RACCOON_TRANSITION;
			break;
		}
		default:
		{
			if (level == MARIO_LEVEL_BIG)
				aniId = GetAniIdBig();
			else if (level == MARIO_LEVEL_SMALL)
				aniId = GetAniIdSmall();
			else // level == MARIO_LEVEL_RACCOON
				aniId = GetAniIdRaccoon();
			break;
		}
	}
	// DebugOut(L"[DEBUG] aniId: %d\n", aniId);
	animations->Get(aniId)->Render(x, y);

	// RenderBoundingBox();
	
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	if (this->state == MARIO_STATE_BIG_TO_SMALL) {
		if (GetTickCount64() - transition_timer <= MARIO_TRANSFORM_SMALL_AND_BIG_TIME) return;
		else {
			transition_timer = 0;
			SetLevel(MARIO_LEVEL_SMALL);
		}
	}
	if (this->state == MARIO_STATE_BIG_TO_SMALL ||
		this->state == MARIO_STATE_SMALL_TO_BIG) {
		if (GetTickCount64() - transition_timer <= MARIO_TRANSFORM_SMALL_AND_BIG_TIME) return;
		transition_timer = 0;
	}

	if (this->state == MARIO_STATE_BIG_TO_RACCOON ||
		this->state == MARIO_STATE_RACCOON_TO_BIG) {
		if (GetTickCount64() - transition_timer <= MARIO_TRANSFORM_BIG_AND_RACCOON_TIME) return;
		transition_timer = 0;
	}

	if (this->state == MARIO_STATE_KICKING) {
		if (GetTickCount64() - short_action_timer <= MARIO_KICKING_TIME) return;
		short_action_timer = 0;
	}

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
			isOnPlatform = false;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isGrabbing) break;
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
	case MARIO_STATE_BIG_TO_RACCOON:
	case MARIO_STATE_RACCOON_TO_BIG:
		ax = 0;
		vx = 0;
		transition_timer = GetTickCount64();
		break;
	case MARIO_STATE_FLY_UP:
		vy = -MARIO_FLY_UP_SPEED;
		isOnPlatform = false;
		break;
	case MARIO_STATE_SLOW_FALLING:
		vy = MARIO_SLOW_FALLING_SPEED;
		isSlowFalling = true;
		break;
	case MARIO_STATE_KICKING:
		{
			isGrabbing = false;
			CKoopa* koopa = (CKoopa*)grabbedObj;
			int koopaState = (grabbedObj->GetType() == OBJECT_TYPE_KOOPA ? KOOPA_STATE_SHELL_MOVING : PARA_KOOPA_STATE_SHELL_MOVING);
			koopa->SetIsPlayerLeft((nx > 0));
			koopa->SetState(koopaState);
			short_action_timer = GetTickCount64();
			break;
		}
	}

	CGameObject::SetState(state);
}

void CMario::ScoringPointWithoutCombo(int point)
{
	int scoreTextId = SCORE_TEXT_100;
	switch (point) {
	case SCORE_POINT_100: scoreTextId = SCORE_TEXT_100; break;
	case SCORE_POINT_1000: scoreTextId = SCORE_TEXT_1000; break;
	case SCORE_POINT_200: scoreTextId = SCORE_TEXT_200; break;
	case SCORE_POINT_400: scoreTextId = SCORE_TEXT_400; break;
	}
	this->point += point;
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	scene->AddObjects(new CScoreText(x, y, scoreTextId, OBJECT_TYPE_SCORE_TEXT));
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
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
	if (this->level == MARIO_LEVEL_SMALL ||
		(this->level == MARIO_LEVEL_BIG && l == MARIO_LEVEL_SMALL))
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

