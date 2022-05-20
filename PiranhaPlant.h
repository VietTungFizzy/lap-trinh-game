#pragma once
#include "Plant.h"

#define PIRANHA_PLANT_BBOX_WIDTH 16
#define PIRANHA_PLANT_BBOX_HEIGHT 24

#define ID_ANI_PIRANHA_PLANT 5200

#define PIRANHA_PLANT_STATE_INACTIVE 0
#define PIRANHA_PLANT_STATE_GO_UP 1
#define PIRANHA_PLANT_STATE_STAY_UP 2
#define PIRANHA_PLANT_STATE_GO_DOWN 3
#define PIRANHA_PLANT_STATE_COOLDOWN 4

#define PIRANHA_PLANT_COOLDOWN_TIME 2000
#define PIRANHA_PLANT_STAY_UP_TIME 2000

#define PIRANHA_PLANT_GO_UP_SPEED 0.1f
#define PIRANHA_PLANT_GO_UP_DISTANCE 20

#define PIRANHA_PLANT_MAX_RANGE 128
#define PIRANHA_PLANT_MIN_RANGE 16

#define PIRANHA_PLANT_OFFSET_BOTTOM_Y 2
class CPiranhaPlant :
    public CPlant
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
public:
	CPiranhaPlant(float x, float y, int type);

	void SetState(int state);
	
};

