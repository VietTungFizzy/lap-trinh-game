#include "VenusFireTrap.h"
#include "Animations.h"
#include "Sprites.h"
#include "debug.h"
int CVenusFireTrap::decideWhereToLook()
{
	if (player == NULL) return VENUS_FIRE_TRAP_LOOK_UP_LEFT;
	float p_x, p_y;
	player->GetPosition(p_x, p_y);
	// Look down
	if (p_y > y) {
		if (p_x > x) { // Look right
			return VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT;
		}
		else { // Look left
			return VENUS_FIRE_TRAP_LOOK_DOWN_LEFT;
		}
	}
	else { // p_y < y ---- Look up
		if (p_x > x) { // Look right
			return VENUS_FIRE_TRAP_LOOK_UP_RIGHT;
		}
		else { // Look left
			return VENUS_FIRE_TRAP_LOOK_UP_LEFT;
		}
	}
}

bool CVenusFireTrap::isPlayerInRange()
{
	float p_x, p_y, l, t, r, b;
	player->GetPosition(p_x, p_y);
	GetBoundingBox(l, t, r, b);
	return ((p_x > l + VENUS_FIRE_TRAP_MAX_RANGE && p_x < l + VENUS_FIRE_TRAP_MIN_RANGE) || 
		(p_x < r + VENUS_FIRE_TRAP_MAX_RANGE && p_x > r + VENUS_FIRE_TRAP_MIN_RANGE));
}

bool CVenusFireTrap::isPlayerTooClose()
{
	float p_x, p_y, l, t, r, b;
	player->GetPosition(p_x, p_y);
	GetBoundingBox(l, t, r, b);
	return (p_x < r + VENUS_FIRE_TRAP_MIN_RANGE && p_x > l + VENUS_FIRE_TRAP_MIN_RANGE);
}

void CVenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - VENUS_FIRE_TRAP_BBOX_WIDTH / 2;
	top = y - VENUS_FIRE_TRAP_BBOX_HEIGHT / 2;
	right = left + VENUS_FIRE_TRAP_BBOX_WIDTH;
	bottom = top + VENUS_FIRE_TRAP_BBOX_HEIGHT;
}

void CVenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (player == NULL) return;
	switch (state) {
		case VENUS_FIRE_TRAP_INACTIVE_STATE:
			if(!isPlayerTooClose()) SetState(VENUS_FIRE_TRAP_GO_UP_STATE);
			break;
		case VENUS_FIRE_TRAP_GO_UP_STATE:
			y += vy * dt;
			if (abs(y - initY) > VENUS_FIRE_TRAP_GO_UP_DISTANCE) SetState(VENUS_FIRE_TRAP_FIRE_STATE);
			break;
		case VENUS_FIRE_TRAP_FIRE_STATE:
			if (GetTickCount64() - timer_start > VENUS_FIRE_TRAP_WATCHING_TIME) {
				timer_start = 0;
				SetState(VENUS_FIRE_TRAP_GO_DOWN_STATE);
			}
			else {
				DebugOut(L"[DEBUG] FIRE!!!!!\n");
			}
			break;
		case VENUS_FIRE_TRAP_GO_DOWN_STATE:
			y += vy * dt;
			if (abs(y - initY) > VENUS_FIRE_TRAP_GO_UP_DISTANCE) SetState(VENUS_FIRE_TRAP_COOLDOWN_STATE);
			break;
		case VENUS_FIRE_TRAP_COOLDOWN_STATE:
			if (isPlayerTooClose()) {
				SetState(VENUS_FIRE_TRAP_INACTIVE_STATE);
			} else if (GetTickCount64() - timer_start > VENUS_FIRE_TRAP_COOLDOWN) SetState(VENUS_FIRE_TRAP_FIRE_STATE);
			break;
	}
	
}

void CVenusFireTrap::Render()
{
	int direction = decideWhereToLook();
	
	if (state == VENUS_FIRE_TRAP_GO_UP_STATE) {
		int aniId = -1;
		switch (direction) {
		case VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT:
			aniId = ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT;
			break;
		case VENUS_FIRE_TRAP_LOOK_DOWN_LEFT:
			aniId = ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_LEFT;
			break;
		case VENUS_FIRE_TRAP_LOOK_UP_LEFT:
			aniId = ID_ANI_VENUS_FIRE_TRAP_LOOK_UP_LEFT;
			break;
		case VENUS_FIRE_TRAP_LOOK_UP_RIGHT:
			aniId = ID_ANI_VENUS_FIRE_TRAP_LOOK_UP_RIGHT;
			break;
		}
		if (aniId == -1) aniId = ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT;
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(aniId)->Render(x, y);
	}
	if (state == VENUS_FIRE_TRAP_GO_DOWN_STATE || state == VENUS_FIRE_TRAP_FIRE_STATE) {
		int spriteId = -1;
		switch (direction) {
		case VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT:
			spriteId = VENUS_FIRE_TRAP_SPRITE_ID_LOOK_DOWN_RIGHT;
			break;
		case VENUS_FIRE_TRAP_LOOK_DOWN_LEFT:
			spriteId = VENUS_FIRE_TRAP_SPRITE_ID_LOOK_DOWN_LEFT;
			break;
		case VENUS_FIRE_TRAP_LOOK_UP_LEFT:
			spriteId = VENUS_FIRE_TRAP_SPRITE_ID_LOOK_UP_LEFT;
			break;
		case VENUS_FIRE_TRAP_LOOK_UP_RIGHT:
			spriteId = VENUS_FIRE_TRAP_SPRITE_ID_LOOK_UP_RIGHT;
			break;
		}
		if (spriteId == -1) spriteId = ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT;
		CSprites* s = CSprites::GetInstance();
		s->Get(spriteId)->Draw(x, y);
	}
}

void CVenusFireTrap::SetState(int state)
{
	switch (state) {
		case VENUS_FIRE_TRAP_GO_UP_STATE:
			vy = -VENUS_FIRE_TRAP_GO_UP_SPEED;
			break;
		case VENUS_FIRE_TRAP_GO_DOWN_STATE:
			vy = VENUS_FIRE_TRAP_GO_UP_SPEED;
			break;
		case VENUS_FIRE_TRAP_FIRE_STATE:
		case VENUS_FIRE_TRAP_COOLDOWN_STATE:
			timer_start = GetTickCount64();
			vy = 0;
			break;
	}
	CGameObject::SetState(state);
}

CVenusFireTrap::CVenusFireTrap(float x, float y): CGameObject(x, y)
{
	player = NULL;
	state = VENUS_FIRE_TRAP_INACTIVE_STATE;
	initY = y;
}
