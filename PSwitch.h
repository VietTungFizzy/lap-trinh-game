#pragma once
#include "GameObject.h"

#define P_SWITCH_BBOX_WIDTH 16
#define P_SWITCH_BBOX_HEIGHT 16

#define ID_ANI_P_SWITCH_NORMAL  6000
#define ID_ANI_P_SWITCH_PRESSED  6001

#define P_SWITCH_GO_UP_SPEED 0.05f
#define P_SWITCH_GRAVITY 0.001f

#define P_SWITCH_STATE_NORMAL 0
#define P_SWITCH_STATE_PRESSED 1
class CPSwitch :
    public CGameObject
{
private:
    float ay;

protected:
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();

    virtual int IsBlocking() { return 1; }

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
public:
    CPSwitch(float x, float y, int type) : CGameObject(x, y, type) {
        ay = P_SWITCH_GRAVITY;
        vy = P_SWITCH_GO_UP_SPEED;
    }

};

