#include "Bullet.h"
#include "Game.h"

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_BBOX_WIDTH / 2;
	top = y - BULLET_BBOX_HEIGHT / 2;
	right = left + BULLET_BBOX_WIDTH;
	bottom = top + BULLET_BBOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	float cam_t, cam_l, cam_r, cam_b;

	CGame* game = CGame::GetInstance();
	game->GetCamPos(cam_l, cam_t);
	cam_r = cam_l + game->GetBackBufferWidth();
	cam_b = cam_t + game->GetBackBufferHeight();
	bool isInCamera = (cam_l < x&& x < cam_r&& cam_t < y&& y < cam_b);
	if (!isInCamera) Delete();
}

void CBullet::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = (vx > 0) ? BULLET_COUNTER_CLOCKWISE_ANI_ID : BULLET_CLOCKWISE_ANI_ID;
	animations->Get(aniId)->Render(x, y);
}

CBullet::CBullet(float x, float y, int type, int direction) : CGameObject(x, y, type)
{
	switch (direction) {
	case BULLET_BOTTOM_LEFT_NEAR: 
		vx = -BULLET_SPEED_X_NEAR;
		vy = BULLET_SPEED_Y_NEAR;
		break;
	case BULLET_BOTTOM_LEFT_FAR: 
		vx = -BULLET_SPEED_X_FAR;
		vy = BULLET_SPEED_Y_FAR;
		break;
	case BULLET_BOTTOM_RIGHT_NEAR: 
		vx = BULLET_SPEED_X_NEAR;
		vy = BULLET_SPEED_Y_NEAR;
		break;
	case BULLET_BOTTOM_RIGHT_FAR: 
		vx = BULLET_SPEED_X_FAR;
		vy = BULLET_SPEED_Y_FAR;
		break;
	case BULLET_TOP_LEFT_NEAR: 
		vx = -BULLET_SPEED_X_NEAR;
		vy = -BULLET_SPEED_Y_NEAR;
		break;
	case BULLET_TOP_LEFT_FAR: 
		vx = -BULLET_SPEED_X_FAR;
		vy = -BULLET_SPEED_Y_FAR;
		break;
	case BULLET_TOP_RIGHT_NEAR: 
		vx = BULLET_SPEED_X_NEAR;
		vy = -BULLET_SPEED_Y_NEAR;
		break;
	case BULLET_TOP_RIGHT_FAR: 
		vx = BULLET_SPEED_X_FAR;
		vy = -BULLET_SPEED_Y_FAR;
		break;
	}
}
