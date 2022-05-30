#include "Decorated_Obj.h"
#include "Sprites.h"

void CDecoratedObject::Render()
{
	if (length <= 0) return;

	CSprites* s = CSprites::GetInstance();
	if (length == 1 && height == 1) {
		s->Get(spriteId)->Draw(x, y);
	}
	else {
		float xx = x;
		float yy = y;
		for (int i = 0; i < height; i++) {
			yy = y + i * DECORATED_OBJECT_HEIGHT;
			for (int j = 0; j < length; j++) {
				xx = x + j * DECORATED_OBJECT_WIDTH;
				s->Get(spriteId)->Draw(xx, yy);
			}
		}
	}
}

void CDecoratedObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - DECORATED_OBJECT_WIDTH / 2;
	t = y - DECORATED_OBJECT_HEIGHT / 2;
	r = l + DECORATED_OBJECT_WIDTH * length;
	b = t + DECORATED_OBJECT_HEIGHT * height;
}
