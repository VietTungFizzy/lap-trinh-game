#pragma once
#include "GameObject.h"
#include<string>

#define HUD_BBOX_WIDTH 230
#define HUD_BBOX_HEIGHT 26

#define HUD_AMOUNT_OF_CARD 3

#define ID_SPRITE_HUD_SKELETON 45000
#define ID_SPRITE_HUD_M_BUTTON 45005

#define HUD_POINT_SIZE 7
#define HUD_TIME_SIZE 3

#pragma region COMPONENT OFFSET
#define HUD_M_BUTTON_OFFSET_X 8
#define HUD_M_BUTTON_OFFSET_Y 15

#define HUD_LOCAL_COORDINATE_OFFSET 3

#define HUD_SKELETON_OFFSET_X 40

#define HUD_CARDS_OFFSET_X 168
#define HUD_CARDS_OFFSET_Y 10

#define HUD_WORLD_TEXT_OFFSET_X 37
#define HUD_WORLD_TEXT_OFFSET_Y 7

#define HUD_POWER_METER_OFFSET_X 52
#define HUD_POWER_METER_OFFSET_Y 7

#define HUD_POWER_METER_OFFSET_X 52
#define HUD_POWER_METER_OFFSET_Y 7

#define HUD_COIN_TEXT_OFFSET_X 140
#define HUD_COIN_TEXT_OFFSET_Y 7

#define HUD_LIVES_TEXT_OFFSET_X 37
#define HUD_LIVES_TEXT_OFFSET_Y 15

#define HUD_POINT_TEXT_OFFSET_X 52
#define HUD_POINT_TEXT_OFFSET_Y 15

#define HUD_TIME_TEXT_OFFSET_X 124
#define HUD_TIME_TEXT_OFFSET_Y 15

#pragma endregion

/*
*	HUD ties directly to global state. Whenever global state
*	changes, HUD will change accordingly
*/
class CHud :
    public CGameObject
{
private:
	CGameObject* livesText;
	CGameObject* pointText;
	CGameObject* coinText;
	CGameObject* timeText;
	CGameObject* worldText;
	CGameObject* powerMeter;

	CGameObject* cards[HUD_AMOUNT_OF_CARD];
private:
	// Convert integer to a string with a string with certain length
	// And add a certain amount of '0' before the number
	// if necessary
	std::string convertIntToString(int length, int content);
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	CHud(float x, float y, int type);
	void SetPosition(float x, float y);
};

