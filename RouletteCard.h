#pragma once
#include "GameObject.h"
#include "AssetIDs.h"

#define ID_ANI_ROULETTE_CARD_MUSROOM 3100
#define ID_ANI_ROULETTE_CARD_FLOWER 3101
#define ID_ANI_ROULETTE_CARD_STAR 3102

#define ROULETTE_CARD_STATE_MUSHROOM 0
#define ROULETTE_CARD_STATE_FLOWER 1
#define ROULETTE_CARD_STATE_STAR 2

#define ROULETTE_CARD_LIVING_TIME 1000

#define ROULETTE_CARD_GO_UP_SPEED 0.1f

#define ROULETTE_CARD_BBOX_WIDTH 16
#define ROULETTE_CARD_BBOX_HEIGHT 16

class CRouletteCard :
    public CGameObject
{
private:
	ULONGLONG timer;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	CRouletteCard(float x, float y, int state) : CGameObject(x, y, OBJECT_TYPE_ROULETTE_CARD_EFFECT) {
		this->state = state;
		vy = -ROULETTE_CARD_GO_UP_SPEED;
		timer = GetTickCount64();
	}

	virtual int IsBlocking() { return 0; }
};

