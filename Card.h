#pragma once
#include "GameObject.h"
#define CARD_BBOX_WIDTH 24
#define CARD_BBOX_HEIGHT 28

#define CARD_STATE_EMPTY 0
#define CARD_STATE_MUSHROOM 1
#define CARD_STATE_FLOWER 2
#define CARD_STATE_STAR 3

#define ID_SPRITE_CARD_EMPTY 43000
#define ID_SPRITE_CARD_MUSHROOM 43001
#define ID_SPRITE_CARD_FLOWER 43002
#define ID_SPRITE_CARD_STAR 43003

class CCard :
    public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render();

public:
	CCard(float x, float y, int type): CGameObject(x, y, type) {
		SetState(CARD_STATE_EMPTY);
	}

};

