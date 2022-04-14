#pragma once
#include "GameObject.h"
#define SCORE_TEXT_SPEED 0.2f
#define SCORE_TEXT_SPEED_REDUCTION_COEFFICIENT 0.85f
#define SCORE_TEXT_EPSILON (1e-3)

#define SCORE_TEXT_100 61000
#define SCORE_TEXT_1000 61001
class CScoreText :
    public CGameObject
{
protected:
    int spriteId;
public:
    CScoreText(float x, float y, int spriteId, int type) : CGameObject(x, y, type) { 
        this->spriteId = spriteId; 
        this->vy = -SCORE_TEXT_SPEED;
    }
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b) {}
};

