#pragma once
#include "GameObject.h"
class CPlant :
    public CGameObject
{
protected:
	CGameObject* player;
	float topY, bottomY;
	ULONGLONG timer_start;

protected:
	bool isPlayerInRange(float maxRange, float minRange);

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;

	void SetPlayer(CGameObject* player) { this->player = player; }
	int IsCollidable() { return 1; };

	CPlant(float x, float y, int type) : CGameObject(x, y, type) {}
};

