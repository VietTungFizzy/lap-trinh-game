#pragma once
#include "GameObject.h"

#define MARIO_WORLD_MAP_BBOX_WIDTH 16
#define MARIO_WORLD_MAP_BBOX_HEIGHT 16

#define ID_ANI_MARIO_WORLD_MAP_SMALL 1900
#define ID_ANI_MARIO_WORLD_MAP_BIG 1901
#define ID_ANI_MARIO_WORLD_MAP_RACCOON 1902

#define MARIO_WORLD_MAP_STATE_IDLE 0
#define MARIO_WORLD_MAP_STATE_MOVING_LEFT 1
#define MARIO_WORLD_MAP_STATE_MOVING_RIGHT 2
#define MARIO_WORLD_MAP_STATE_MOVING_UP 3
#define MARIO_WORLD_MAP_STATE_MOVING_DOWN 4

#define MARIO_WORLD_MAP_LEVEL_SMALL 0
#define MARIO_WORLD_MAP_LEVEL_BIG 1
#define MARIO_WORLD_MAP_LEVEL_RACCOON 2

#define MARIO_WORLD_MAP_SPEED 0.1f
#define MARIO_WORLD_MAP_EPSILON (1e-4)

class CMarioWorldMap :
    public CGameObject
{
private:
    int level;
    CGameObject* currentNode;
    float dest_x, dest_y;
public:
    CMarioWorldMap(float x, float y, int type);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    void SetLevel(int level) { this->level = level; }
    void SetState(int state);

    void EnterWorld();
};

