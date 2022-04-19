#include "Goomba.h"
#include "InvinsibleBrick.h"
#include "Mario.h"
#include "AssetIDs.h"
#include "ParaGoomba.h"
#include "PlayScene.h"
#include "ScoreText.h"
CGoomba::CGoomba(float x, float y, int type) : CGameObject(x, y, type)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	isCollidable = 1;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE_FOR_MARIO)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj)) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

	switch (e->obj->GetType()) {
	case OBJECT_TYPE_KOOPA:
		{
			SetState(GOOMBA_STATE_CONTACT_WITH_SHELL);
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			CMario* player = (CMario*)scene->GetPlayer();
			player->AddPoint(SCORE_POINT_100);
			scene->AddObjects(new CScoreText(x, y, SCORE_TEXT_100, OBJECT_TYPE_SCORE_TEXT));
			this->nx = e->nx;
			break;
		}
	case OBJECT_TYPE_PARA_GOOMBA:
		if(e->obj->GetState() == PARA_GOOMBA_STATE_LOST_WING) vx = -vx;
		break;
	case OBJECT_TYPE_GOOMBA:
		vx = -vx;
		break;
	}
	
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE_FOR_MARIO) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	if ((state == GOOMBA_STATE_CONTACT_WITH_SHELL) && (GetTickCount64() - die_start > GOOMBA_CONTACT_TIMEOUT)) {
		SetState(GOOMBA_STATE_DIE_FOR_SHELL);
		die_start = 0;
		return;
	}
	if (state == GOOMBA_STATE_DIE_FOR_SHELL) {
		float cam_l, cam_t, cam_r, cam_b;
		CGame* game = CGame::GetInstance();
		game->GetCamPos(cam_l, cam_t);
		cam_b = cam_t + game->GetBackBufferHeight();

		if (y > cam_b) {
			isDeleted = true;
			return;
		}
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	switch (state) {
		case GOOMBA_STATE_DIE_FOR_MARIO: aniId = ID_ANI_GOOMBA_DIE; break;
		case GOOMBA_STATE_CONTACT_WITH_SHELL: aniId = ID_ANI_GOOMBA_CONTACT_WITH_SHELL_EFFECT; break;
		case GOOMBA_STATE_DIE_FOR_SHELL: aniId = ID_ANI_GOOMBA_DIE_FOR_SHELL; break;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x,y + GOOMBA_RENDERING_OFFSET_Y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE_FOR_MARIO:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_CONTACT_WITH_SHELL:
			isCollidable = 0;
			ay = 0;
			vx = 0;
			die_start = GetTickCount64();
			break;
		case GOOMBA_STATE_DIE_FOR_SHELL:
			vy = -GOOMBA_DEFLECT_SPEED_Y;
			vx = GOOMBA_DEFLECT_SPEED_X * nx;
			ay = GOOMBA_GRAVITY;
			break;
	}
}
