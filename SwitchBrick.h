#pragma once
#include "GameObject.h"
#define SWITCH_BRICK_BBOX_WIDTH 16
#define SWITCH_BRICK_BBOX_HEIGHT 16

#define ID_ANI_SWITCH_BRICK_STATE_BRICK 10002
#define ID_ANI_SWITCH_BRICK_STATE_COIN 11000

#define SWITCH_BRICK_STATE_BRICK 0
#define SWITCH_BRICK_STATE_COIN 1
#define SWITCH_BRICK_STATE_DESTROYED 2
#define SWITCH_BRICK_STATE_COIN_EATEN 3
class CSwitchBrick :
    public CGameObject
{
protected:
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();

public:
    CSwitchBrick(float x, float y, int type) : CGameObject(x, y, type) {}
    virtual int IsBlocking() { return (state == SWITCH_BRICK_STATE_BRICK); }
    virtual void SetState(int state);
};

