#include "TitleMenu.h"

void CTitleMenu::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TITLE_MENU_BBOX_WIDTH / 2;
	top = y - TITLE_MENU_BBOX_HEIGHT / 2;
	right = left + TITLE_MENU_BBOX_WIDTH;
	bottom = top + TITLE_MENU_BBOX_HEIGHT;
}

void CTitleMenu::Render()
{
	int spriteId = isOnePlayer ? ID_SPRITE_TITLE_MENU_1_PLAYER : ID_SPRITE_TITLE_MENU_2_PLAYER;
	CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
}

CTitleMenu::CTitleMenu(float x, float y, int type) : CGameObject(x, y, type)
{
	isOnePlayer = true;
}

void CTitleMenu::StartGame()
{
	CGame::GetInstance()->InitiateSwitchScene(TITLE_MENU_NEXT_SCENE_ID);
}
