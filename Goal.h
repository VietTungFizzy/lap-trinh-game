#pragma once
#include "GameObject.h"
#define GOAL_BBOX_WIDTH 16
#define GOAL_BBOX_HEIGHT 16

#define ID_SPRITE_GOAL_MUSHROOM 41000
#define ID_SPRITE_GOAL_FLOWER 41001
#define ID_SPRITE_GOAL_STAR 41002

#define GOAL_STATE_MUSHROOM 0
#define GOAL_STATE_FLOWER 1
#define GOAL_STATE_STAR 2
#define GOAL_STATE_TOUCHED 3

#define GOAL_DELAY_TIME_BEFORE_SWITCH_STATE 100
class CGoal :
    public CGameObject
{
private:
	ULONGLONG timer;
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	void SetState(int state);

public:
	CGoal(float x, float y, int type): CGameObject(x, y, type) {
		SetState(GOAL_STATE_MUSHROOM);
	}

	virtual int IsBlocking() { return 0; }
};

