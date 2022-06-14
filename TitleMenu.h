#pragma once
#include "GameObject.h"
#define TITLE_MENU_BBOX_WIDTH 112
#define TITLE_MENU_BBOX_HEIGHT 24

#define ID_SPRITE_TITLE_MENU_1_PLAYER 44000
#define ID_SPRITE_TITLE_MENU_2_PLAYER 44001

#define TITLE_MENU_NEXT_SCENE_ID 2

class CTitleMenu :
    public CGameObject
{
private:
	BOOLEAN isOnePlayer;

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render();
public:
	CTitleMenu(float x, float y, int type);

	void SwitchAmountOfPlayer() { isOnePlayer = !isOnePlayer; }
	void StartGame();
};

