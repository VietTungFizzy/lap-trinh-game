#include "InvinsibleBrick.h"

void CInvinsibleBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - INVINSIBLE_BRICK_BBOX_WIDTH / 2;
	t = y - INVINSIBLE_BRICK_BBOX_HEIGHT / 2;
	r = l + INVINSIBLE_BRICK_BBOX_WIDTH * length;
	b = t + INVINSIBLE_BRICK_BBOX_HEIGHT * height;
}
