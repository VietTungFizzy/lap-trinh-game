#include "Text.h"

int CText::GetSpriteId(char c)
{
	int spriteId = -1;
	switch (c) {
	case '0': spriteId = ID_SPRITE_TEXT_0; break;
	case '1': spriteId = ID_SPRITE_TEXT_1; break;
	case '2': spriteId = ID_SPRITE_TEXT_2; break;
	case '3': spriteId = ID_SPRITE_TEXT_3; break;
	case '4': spriteId = ID_SPRITE_TEXT_4; break;
	case '5': spriteId = ID_SPRITE_TEXT_5; break;
	case '6': spriteId = ID_SPRITE_TEXT_6; break;
	case '7': spriteId = ID_SPRITE_TEXT_7; break;
	case '8': spriteId = ID_SPRITE_TEXT_8; break;
	case '9': spriteId = ID_SPRITE_TEXT_9; break;
	case 'A': spriteId = ID_SPRITE_TEXT_A; break;
	case 'B': spriteId = ID_SPRITE_TEXT_B; break;
	case 'C': spriteId = ID_SPRITE_TEXT_C; break;
	case 'D': spriteId = ID_SPRITE_TEXT_D; break;
	case 'E': spriteId = ID_SPRITE_TEXT_E; break;
	case 'F': spriteId = ID_SPRITE_TEXT_F; break;
	case 'G': spriteId = ID_SPRITE_TEXT_G; break;
	case 'H': spriteId = ID_SPRITE_TEXT_H; break;
	case 'I': spriteId = ID_SPRITE_TEXT_I; break;
	case 'J': spriteId = ID_SPRITE_TEXT_J; break;
	case 'K': spriteId = ID_SPRITE_TEXT_K; break;
	case 'L': spriteId = ID_SPRITE_TEXT_L; break;
	case 'M': spriteId = ID_SPRITE_TEXT_M; break;
	case 'N': spriteId = ID_SPRITE_TEXT_N; break;
	case 'O': spriteId = ID_SPRITE_TEXT_O; break;
	case 'P': spriteId = ID_SPRITE_TEXT_P; break;
	case 'Q': spriteId = ID_SPRITE_TEXT_Q; break;
	case 'R': spriteId = ID_SPRITE_TEXT_R; break;
	case 'S': spriteId = ID_SPRITE_TEXT_S; break;
	case 'T': spriteId = ID_SPRITE_TEXT_T; break;
	case 'U': spriteId = ID_SPRITE_TEXT_U; break;
	case 'V': spriteId = ID_SPRITE_TEXT_V; break;
	case 'W': spriteId = ID_SPRITE_TEXT_W; break;
	case 'X': spriteId = ID_SPRITE_TEXT_X; break;
	case 'Y': spriteId = ID_SPRITE_TEXT_Y; break;
	case 'Z': spriteId = ID_SPRITE_TEXT_Z; break;
	case '!': spriteId = ID_SPRITE_TEXT_EXCLAIM; break;
	}
	if (spriteId == -1) {
		DebugOut(L"[ERROR] unknown character %c\n", c);
	}
	return spriteId;
}

void CText::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CHARACTER_BBOX_WIDTH / 2;
	top = y - CHARACTER_BBOX_HEIGHT / 2;
	right = left + CHARACTER_BBOX_WIDTH * text.length();
	top = top + CHARACTER_BBOX_HEIGHT;
}

void CText::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!hasLivingTime) return;
	if (GetTickCount64() - timer > TEXT_LIVING_TIME) Delete();
}

void CText::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	int spriteId;
	float xx = x;
	for (size_t i = 0; i < text.size(); i++) {
		if (text[i] == ' ') continue;
		xx = x + CHARACTER_BBOX_WIDTH * i;
		spriteId = GetSpriteId(text[i]);
		sprites->Get(spriteId)->Draw(xx, y);
	}
}
