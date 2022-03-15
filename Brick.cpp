#include "Brick.h"
#include "Sprites.h"

void CBrick::Render()
{
	if (spriteId == ID_ANI_BRICK) {
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_BRICK)->Render(x, y);
	}
	else {
		CSprites* s = CSprites::GetInstance();
		s->Get(this->spriteId)->Draw(x, y);
	}
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}