#pragma once
#include "GameObject.h"
#include<string>

#define CHARACTER_BBOX_WIDTH 8
#define CHARACTER_BBOX_HEIGHT 8

#define TEXT_LIVING_TIME 3000

#define ID_SPRITE_TEXT_0 42000
#define ID_SPRITE_TEXT_1 42001
#define ID_SPRITE_TEXT_2 42002
#define ID_SPRITE_TEXT_3 42003
#define ID_SPRITE_TEXT_4 42004
#define ID_SPRITE_TEXT_5 42005
#define ID_SPRITE_TEXT_6 42006
#define ID_SPRITE_TEXT_7 42007
#define ID_SPRITE_TEXT_8 42008
#define ID_SPRITE_TEXT_9 42009

#define ID_SPRITE_TEXT_A 42010
#define ID_SPRITE_TEXT_B 42011
#define ID_SPRITE_TEXT_C 42012
#define ID_SPRITE_TEXT_D 42013
#define ID_SPRITE_TEXT_E 42014
#define ID_SPRITE_TEXT_F 42015
#define ID_SPRITE_TEXT_G 42016
#define ID_SPRITE_TEXT_H 42017
#define ID_SPRITE_TEXT_I 42018
#define ID_SPRITE_TEXT_J 42019
#define ID_SPRITE_TEXT_K 42020
#define ID_SPRITE_TEXT_L 42021
#define ID_SPRITE_TEXT_M 42022
#define ID_SPRITE_TEXT_N 42023
#define ID_SPRITE_TEXT_O 42024
#define ID_SPRITE_TEXT_P 42025
#define ID_SPRITE_TEXT_Q 42026
#define ID_SPRITE_TEXT_R 42027
#define ID_SPRITE_TEXT_S 42028
#define ID_SPRITE_TEXT_T 42029
#define ID_SPRITE_TEXT_U 42030
#define ID_SPRITE_TEXT_V 42031
#define ID_SPRITE_TEXT_W 42032
#define ID_SPRITE_TEXT_X 42033
#define ID_SPRITE_TEXT_Y 42034
#define ID_SPRITE_TEXT_Z 42035

#define ID_SPRITE_TEXT_EXCLAIM 42036

#define TEXT_CONTENT_0 "COURSE CLEAR !";
#define TEXT_CONTENT_1 "YOU GOT A CARD";
class CText :
    public CGameObject
{
private:
	std::string text;
	ULONGLONG timer;
	BOOLEAN hasLivingTime;

	int GetSpriteId(char c);
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	CText(float x, float y, int type, std::string text, bool hasLivingTime = false) : CGameObject(x, y, type) {
		this->text = text;
		timer = GetTickCount64();
		this->hasLivingTime = hasLivingTime;
	}
	// This function mostly used for HUD
	void UpdateText(std::string text);
};

