#include "SuperLeaf.h"
#include "Sprites.h"

void CSuperLeaf::calculateBezierPoint(float& _x, float& _y, float t)
{
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	_x = uu * x0 + 2 * u * t * x1 + tt * x2;
	_y = uu * y0 + 2 * u * t * y1 + tt * y2;
}

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SUPER_LEAF_BBOX_WIDTH / 2;
	top = y - SUPER_LEAF_BBOX_HEIGHT / 2;
	right = left + SUPER_LEAF_BBOX_WIDTH;
	bottom = top + SUPER_LEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SUPER_LEAF_STATE_GO_UP) {
		x += vx * dt;
		y += vy * dt;
	}
	else if (state == SUPER_LEAF_STATE_SWING_RIGHT) {
		counter++;
		float temp = (float)(counter / SUPER_LEAF_BEZIER_CURVE_COEFFICIENT);
		calculateBezierPoint(x, y, temp);
	}
	
}

void CSuperLeaf::Render()
{
	if (state == SUPER_LEAF_STATE_INVISIBLE) return;
	int spriteId = -1;
	if (state == SUPER_LEAF_STATE_SWING_RIGHT) spriteId = SUPER_LEAF_SPRITE_ID_RIGHT;
	else spriteId = SUPER_LEAF_SPRITE_ID_LEFT;
	CSprites* s = CSprites::GetInstance();
	s->Get(spriteId)->Draw(x, y);
}

CSuperLeaf::CSuperLeaf(float x, float y, int type): CGameObject(x, y, type)
{
	SetState(SUPER_LEAF_STATE_INVISIBLE);
	x0 = x - SUPER_LEAF_BBOX_WIDTH / 2;
	y0 = y;
	x2 = x + SUPER_LEAF_BBOX_WIDTH / 2;
	y2 = y;
	x1 = x;
	y1 = y + SUPER_LEAF_BBOX_HEIGHT / 2;
}

void CSuperLeaf::SetState(int state)
{
	switch (state) {
		case SUPER_LEAF_STATE_GO_UP: {
			vy = -SUPER_LEAF_GO_UP_SPEED;
			break;
		}
		case SUPER_LEAF_STATE_SWING_RIGHT: {
			vy = 0;
			counter = 0;
			break;
		}
		case SUPER_LEAF_STATE_SWING_LEFT: {
			vy = 0;
			counter = SUPER_LEAF_BEZIER_CURVE_COEFFICIENT;
			break;
		}
		case SUPER_LEAF_STATE_EATEN: {
			Delete();
			break;
		}
	}
	CGameObject::SetState(state);
}
