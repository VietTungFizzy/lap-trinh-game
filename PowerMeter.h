#pragma once
#include "GameObject.h"
#define POWER_METER_BBOX_WIDTH 64
#define POWER_METER_BBOX_HEIGHT 7

#define ID_ANI_POWER_METER_ARROW_FULL_POWER 12001
#define ID_ANI_POWER_METER_ARROW_EMPTY_POWER 12002
#define ID_ANI_POWER_METER_BUTTON_EMPTY 12003
#define ID_ANI_POWER_METER_BUTTON_FULL 12000

#define POWER_METER_VALUE_PER_ARROW 8

#define POWER_METER_ARROW_WIDTH 8
#define POWER_METER_MAX_ARROW 6

#define POWER_METER_BUTTON_OFFSET_X 54

class CPowerMeter :
    public CGameObject
{
private:

protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render();
public:
	CPowerMeter(float x, float y, int type) : CGameObject(x, y, type) {}
};

