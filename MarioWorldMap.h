#pragma once
#include "GameObject.h"



#define MARIO_WORLD_MAP_STATE_IDLE 0
#define MARIO_WORLD_MAP_STATE_MOVING_LEFT 1
#define MARIO_WORLD_MAP_STATE_MOVING_RIGHT 2
#define MARIO_WORLD_MAP_STATE_MOVING_UP 3
#define MARIO_WORLD_MAP_STATE_MOVING_DOWN 4

#define MARIO_WORLD_MAP_LEVEL_SMALL 0
#define MARIO_WORLD_MAP_LEVEL_BIG 1
#define MARIO_WORLD_MAP_LEVEL_RACCOON 2

class CMarioWorldMap :
    public CGameObject
{
private:
    int level;

public:
    CMarioWorldMap(float x, float y, int type);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    void SetLevel(int level) { this->level = level; }
};

