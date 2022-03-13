#include "Decorated_Obj.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
void CDecoratedObject::Render()
{
	CSprites* s = CSprites::GetInstance();
	s->Get(this->spriteId)->Draw(x, y);
}
