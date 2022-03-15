#include "Decorated_Obj.h"
#include "Sprites.h"

void CDecoratedObject::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->spriteId)->Draw(x, y);
}
