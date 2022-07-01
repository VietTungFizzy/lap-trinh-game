#include "MarioGoingIntoPipeEffect.h"
#include "AssetIDs.h"

void CMarioGoingIntoPipeEffect::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	int spriteId = ID_ANI_MARIO_GOING_INTO_PIPE_EFFECT_LEVEL_SMALL;
	switch (marioLevel) {
		case MARIO_LEVEL_SMALL: spriteId = ID_ANI_MARIO_GOING_INTO_PIPE_EFFECT_LEVEL_SMALL; break;
		case MARIO_LEVEL_BIG: spriteId = ID_ANI_MARIO_GOING_INTO_PIPE_EFFECT_LEVEL_BIG; break;
		case MARIO_LEVEL_RACCOON: spriteId = ID_ANI_MARIO_GOING_INTO_PIPE_EFFECT_LEVEL_RACCOON; break;
	}
	sprites->Get(spriteId)->Draw(x, y);
}

void CMarioGoingIntoPipeEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
}

void CMarioGoingIntoPipeEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MARIO_GOING_INTO_PIPE_EFFECT_BBOX_WIDTH / 2;
	t = y - MARIO_GOING_INTO_PIPE_EFFECT_BBOX_HEIGHT / 2;
	r = l + MARIO_GOING_INTO_PIPE_EFFECT_BBOX_WIDTH;
	b = t + MARIO_GOING_INTO_PIPE_EFFECT_BBOX_HEIGHT;
}

CMarioGoingIntoPipeEffect::CMarioGoingIntoPipeEffect(float x, float y, int direction, int marioLevel)
	: CGameObject(x, y, OBJECT_TYPE_MARIO_GOING_INTO_PIPE_EFFECT)
{
	this->marioLevel = marioLevel;
	vy = direction * MARIO_GOING_INTO_PIPE_SPEED;
}
