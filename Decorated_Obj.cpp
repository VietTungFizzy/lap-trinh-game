#include "Decorated_Obj.h"
#include "Sprites.h"

void CDecoratedObject::Render()
{
	if (length <= 0) return;
	if (isAnimated == 0) {
		CSprites* s = CSprites::GetInstance();
		if (length == 1 && height == 1) {
			s->Get(spriteId)->Draw(x, y);
		}
		else {
			float xx = x;
			float yy = y;
			for (int i = 0; i < height; i++) {
				yy = y + i * cellHeight;
				for (int j = 0; j < length; j++) {
					xx = x + j * cellWidth;
					s->Get(spriteId)->Draw(xx, yy);
				}
			}
		}
	}
	else {
		CAnimations* s = CAnimations::GetInstance();
		if (length == 1 && height == 1) {
			s->Get(spriteId)->Render(x, y);
		}
		else {
			float xx = x;
			float yy = y;
			for (int i = 0; i < height; i++) {
				yy = y + i * cellHeight;
				for (int j = 0; j < length; j++) {
					xx = x + j * cellWidth;
					s->Get(spriteId)->Render(xx, yy);
				}
			}
		}
	}
	
}

void CDecoratedObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - cellWidth / 2;
	t = y - cellHeight / 2;
	r = l + cellWidth * length;
	b = t + cellHeight * height;
}
