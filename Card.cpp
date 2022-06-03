#include "Card.h"

void CCard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CARD_BBOX_WIDTH / 2;
	top = y - CARD_BBOX_HEIGHT / 2;
	right = left + CARD_BBOX_WIDTH;
	bottom = top + CARD_BBOX_HEIGHT;
}

void CCard::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	int spriteId = ID_SPRITE_CARD_EMPTY;
	switch (state) {
	case CARD_STATE_EMPTY: spriteId = ID_SPRITE_CARD_EMPTY; break;
	case CARD_STATE_MUSHROOM: spriteId = ID_SPRITE_CARD_MUSHROOM; break;
	case CARD_STATE_FLOWER: spriteId = ID_SPRITE_CARD_FLOWER; break;
	case CARD_STATE_STAR: spriteId = ID_SPRITE_CARD_STAR; break;
	}
	sprites->Get(spriteId)->Draw(x, y);
}
