#pragma once
#include "GameObject.h"
#define MARIO_GOING_INTO_PIPE_EFFECT_BBOX_WIDTH 16
#define MARIO_GOING_INTO_PIPE_EFFECT_BBOX_HEIGHT 16

#define ID_ANI_MARIO_GOING_INTO_PIPE_EFFECT_LEVEL_SMALL 21107
#define ID_ANI_MARIO_GOING_INTO_PIPE_EFFECT_LEVEL_BIG 21108
#define ID_ANI_MARIO_GOING_INTO_PIPE_EFFECT_LEVEL_RACCOON 21109

#define MARIO_GOING_INTO_PIPE_SPEED 0.05f
#define MARIO_GOING_INTO_PIPE_DIRECTION_UP -1
#define MARIO_GOING_INTO_PIPE_DIRECTION_DOWN 1

class CMarioGoingIntoPipeEffect :
    public CGameObject
{
private:
    int marioLevel;

protected:
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
public:
    CMarioGoingIntoPipeEffect(float x, float y, int direction, int marioLevel);
    virtual int IsBlocking() { return 0; }
};

