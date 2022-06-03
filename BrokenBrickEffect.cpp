#include "BrokenBrickEffect.h"
#include "Sprites.h"
#include "AssetIDs.h"
#include "debug.h"

void CBrokenBrickEffect::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	int spriteId;
	for (int i = 0; i < BROKEN_BRICK_EFFECT_RUBBLES_AMOUNT; i++) {
		spriteId = (rubbles[i].vx > 0) ? ID_SPRITE_BROKEN_BRICK_RIGHT : ID_SPRITE_BROKEN_BRICK_LEFT;
		sprites->Get(spriteId)->Draw(rubbles[i].x, rubbles[i].y);
	}
}

void CBrokenBrickEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	bool is_all_rubble_out_of_camera = true;
	
	float cam_t, cam_l, cam_r, cam_b, x, y;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cam_l, cam_t);
	cam_r = cam_l + game->GetBackBufferWidth();
	cam_b = cam_t + game->GetBackBufferHeight();

	for (int i = 0; i < BROKEN_BRICK_EFFECT_RUBBLES_AMOUNT; i++) {
		if (cam_l < rubbles[i].x && rubbles[i].x < cam_r 
			&& cam_t < rubbles[i].y && rubbles[i].y < cam_b) is_all_rubble_out_of_camera = false;

		rubbles[i].vy += rubbles[i].ay * dt;
		rubbles[i].x += rubbles[i].vx * dt;
		rubbles[i].y += rubbles[i].vy * dt;
	}

	if (is_all_rubble_out_of_camera) {
		Delete();
	}
	// if (GetTickCount64() - timer > BROKEN_BRICK_EFFECT_TIME_DURATION) Delete();
}

void CBrokenBrickEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BROKEN_BRICK_EFFECT_BBOX_WIDTH / 2;
	t = y - BROKEN_BRICK_EFFECT_BBOX_HEIGHT / 2;
	r = l + BROKEN_BRICK_EFFECT_BBOX_WIDTH;
	b = t + BROKEN_BRICK_EFFECT_BBOX_HEIGHT;
}

CBrokenBrickEffect::CBrokenBrickEffect(float x, float y) : CGameObject(x, y, OBJECT_TYPE_BROKEN_BRICK_EFFECT)
{
	for (int i = 0; i < BROKEN_BRICK_EFFECT_RUBBLES_AMOUNT; i++) {
		rubbles[i].x = x;
		rubbles[i].y = y;
		rubbles[i].ay = BROKEN_BRICK_EFFECT_RUBBLE_GRABITY;
	}
	// Assign speed value according to corresponding rubble
	rubbles[RUBBLE_TOP_LEFT].vy = -BROKEN_BRICK_EFFECT_RUBBLE_FLY_SPEED_HIGH;
	rubbles[RUBBLE_TOP_LEFT].vx = -BROKEN_BRICK_EFFECT_RUBBLE_MOVE_SPEED_FAR;

	rubbles[RUBBLE_BOTTOM_LEFT].vy = -BROKEN_BRICK_EFFECT_RUBBLE_FLY_SPEED_LOW;
	rubbles[RUBBLE_BOTTOM_LEFT].vx = -BROKEN_BRICK_EFFECT_RUBBLE_MOVE_SPEED_NEAR;

	rubbles[RUBBLE_TOP_RIGHT].vy = -BROKEN_BRICK_EFFECT_RUBBLE_FLY_SPEED_HIGH;
	rubbles[RUBBLE_TOP_RIGHT].vx = BROKEN_BRICK_EFFECT_RUBBLE_MOVE_SPEED_FAR;

	rubbles[RUBBLE_BOTTOM_RIGHT].vy = -BROKEN_BRICK_EFFECT_RUBBLE_FLY_SPEED_LOW;
	rubbles[RUBBLE_BOTTOM_RIGHT].vx = BROKEN_BRICK_EFFECT_RUBBLE_MOVE_SPEED_NEAR;

	// timer = GetTickCount64();
}
