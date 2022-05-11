#include "SuperLeaf.h"
#include "Sprites.h"
#include "debug.h"

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SUPER_LEAF_BBOX_WIDTH / 2;
	top = y - SUPER_LEAF_BBOX_HEIGHT / 2;
	right = left + SUPER_LEAF_BBOX_WIDTH;
	bottom = top + SUPER_LEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state) {
		case SUPER_LEAF_STATE_GO_UP:
		{
			if (abs(y - initY) > SUPER_LEAF_GO_UP_BEFORE_SWING_DISTANCE) SetState(SUPER_LEAF_STATE_SWING_RIGHT);
			break;
		}
		case SUPER_LEAF_STATE_GO_UP_INVISIBLE:
		{
			if (abs(y - initY) > SUPER_LEAF_INVISIBLE_DISTANCE) SetState(SUPER_LEAF_STATE_GO_UP);
			break;
		}
		case SUPER_LEAF_STATE_SWING_RIGHT:
		{
			if (abs(initX - x) > SUPER_LEAF_HORIZONTAL_RANGE / 2 && !isHalfWay) {
				isHalfWay = true;
				vy = -SUPER_LEAF_GO_UP_SPEED;
			}
			if(x > boundaryRight) SetState(SUPER_LEAF_STATE_SWING_LEFT);
			break;
		}
		case SUPER_LEAF_STATE_SWING_LEFT:
		{
			if (abs(initX - x) > SUPER_LEAF_HORIZONTAL_RANGE / 2 && !isHalfWay) {
				isHalfWay = true;
				vy = -SUPER_LEAF_GO_UP_SPEED;
			}
			if(x < boundaryLeft) SetState(SUPER_LEAF_STATE_SWING_RIGHT);
			break;
		}
		default: break;
	}
	x += vx * dt;
	y += vy * dt;
}

void CSuperLeaf::Render()
{
	if (state == SUPER_LEAF_STATE_INVISIBLE || state == SUPER_LEAF_STATE_GO_UP_INVISIBLE) return;
	int spriteId = -1;
	if (state == SUPER_LEAF_STATE_SWING_RIGHT) spriteId = SUPER_LEAF_SPRITE_ID_RIGHT;
	else spriteId = SUPER_LEAF_SPRITE_ID_LEFT;
	CSprites* s = CSprites::GetInstance();
	s->Get(spriteId)->Draw(x, y);

	// RenderBoundingBox();
}

CSuperLeaf::CSuperLeaf(float x, float y, int type): CGameObject(x, y, type)
{
	SetState(SUPER_LEAF_STATE_INVISIBLE);
	initY = y;
	boundaryLeft = x;
	boundaryRight = x + SUPER_LEAF_HORIZONTAL_RANGE;
}

void CSuperLeaf::SetState(int state)
{
	switch (state) {
		case SUPER_LEAF_STATE_GO_UP_INVISIBLE:
		case SUPER_LEAF_STATE_GO_UP: {
			vy = -SUPER_LEAF_GO_UP_BEFORE_SWING_SPEED;
			break;
		}
		case SUPER_LEAF_STATE_SWING_RIGHT: {
			vx = SUPER_LEAF_GO_HORIZONTAL_SPEED;
			vy = SUPER_LEAF_GRAVITY;
			isHalfWay = false;
			initX = x;
			break;
		}
		case SUPER_LEAF_STATE_SWING_LEFT: {
			vx = -SUPER_LEAF_GO_HORIZONTAL_SPEED;
			vy = SUPER_LEAF_GRAVITY;
			isHalfWay = false;
			initX = x;
			break;
		}
		case SUPER_LEAF_STATE_EATEN: {
			Delete();
			break;
		}
	}
	CGameObject::SetState(state);
}
