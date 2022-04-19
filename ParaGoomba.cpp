#include "ParaGoomba.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "PlayScene.h"
#include "ScoreText.h"
#include "debug.h"
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
	vy += ay * dt;
	vx += ax * dt;
	switch (state) {
		case PARA_GOOMBA_STATE_WALK:
			if (GetTickCount64() - timer > PARA_GOOMBA_WALK_TIME) {
				SetState(PARA_GOOMBA_STATE_JUMP);
			}
			break;
		case PARA_GOOMBA_STATE_JUMP:
			if (jumpCount > PARA_GOOMBA_MAXIMUM_SMALL_JUMP) SetState(PARA_GOOMBA_STATE_FLY_UP);
			break;
		case PARA_GOOMBA_STATE_DIE:
			if (GetTickCount64() - timer > PARA_GOOMBA_DIE_TIMEOUT)
			{
				isDeleted = true;
				return;
			}
			break;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CParaGoomba::Render()
{
	int aniId = PARA_GOOMBA_ID_ANI_DIE;
	
	CAnimations* animations = CAnimations::GetInstance();
	switch (state) {
		case PARA_GOOMBA_STATE_WALK:
		case PARA_GOOMBA_STATE_JUMP:
		case PARA_GOOMBA_STATE_FLY_UP:
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
	animations->Get(aniId)->Render(x, y + PARA_GOOMBA_RENDER_OFFSET_Y);
}

void CParaGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->GetType() == OBJECT_TYPE_MARIO) return;
	if (e->ny !=0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) {
			switch (state) {
			case PARA_GOOMBA_STATE_FLY_UP: SetState(PARA_GOOMBA_STATE_WALK); break;
			case PARA_GOOMBA_STATE_JUMP: SetState(PARA_GOOMBA_STATE_JUMP); break;
			}
		}
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (e->obj->GetType() == OBJECT_TYPE_GOOMBA && state == PARA_GOOMBA_STATE_LOST_WING) vx = -vx;
}

CParaGoomba::CParaGoomba(float x, float y, int type) : CGameObject(x, y, type)
{
	SetState(PARA_GOOMBA_STATE_WALK);
	ay = PARA_GOOMBA_GRAVITY;
	ax = 0;
	jumpCount = 0;
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state) {
	case PARA_GOOMBA_STATE_DIE:
		timer = GetTickCount64();
		y += (PARA_GOOMBA_BBOX_HEIGHT - PARA_GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case PARA_GOOMBA_STATE_FLY_UP:
		vy = -PARA_GOOMBA_FLY_SPEED;
		break;
	case PARA_GOOMBA_STATE_WALK:
		{
			CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			CGameObject* player = scence->GetPlayer();
			if (player == NULL) {
				vx = -PARA_GOOMBA_WALK_SPEED;
			}
			else {
				float p_x, p_y;
				player->GetPosition(p_x, p_y);
				vx = (p_x > x) ? PARA_GOOMBA_WALK_SPEED : -PARA_GOOMBA_WALK_SPEED;
			}
			timer = GetTickCount64();
			jumpCount = 0;
			break;
		}
	case PARA_GOOMBA_STATE_JUMP:
		vy = -PARA_GOOMBA_SMALL_JUMP_SPEED;
		jumpCount++;
		break;
	case PARA_GOOMBA_STATE_LOST_WING:
		{
			CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			CGameObject* player = scence->GetPlayer();
			float p_x, p_y;
			player->GetPosition(p_x, p_y);
			vx = (p_x > x) ? PARA_GOOMBA_WALK_SPEED : -PARA_GOOMBA_WALK_SPEED;
			scence->AddObjects(new CScoreText(x, y - PARA_GOOMBA_BBOX_HEIGHT_DIE / 2, SCORE_TEXT_100, OBJECT_TYPE_SCORE_TEXT));
			break;
		}
	}
}
