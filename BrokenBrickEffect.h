#pragma once
#include "GameObject.h"

#define ID_SPRITE_BROKEN_BRICK_LEFT 21010
#define ID_SPRITE_BROKEN_BRICK_RIGHT 21011

#define BROKEN_BRICK_EFFECT_RUBBLES_AMOUNT 4

#define BROKEN_BRICK_EFFECT_TIME_DURATION 3000

#define BROKEN_BRICK_EFFECT_RUBBLE_FLY_SPEED_HIGH 0.2f
#define BROKEN_BRICK_EFFECT_RUBBLE_FLY_SPEED_LOW 0.1f
#define BROKEN_BRICK_EFFECT_RUBBLE_MOVE_SPEED_NEAR 0.05f
#define BROKEN_BRICK_EFFECT_RUBBLE_MOVE_SPEED_FAR 0.1f
#define BROKEN_BRICK_EFFECT_RUBBLE_GRABITY 0.0005f

#define RUBBLE_TOP_LEFT 0
#define RUBBLE_TOP_RIGHT 1
#define RUBBLE_BOTTOM_LEFT 2
#define RUBBLE_BOTTOM_RIGHT 3

#define BROKEN_BRICK_EFFECT_BBOX_WIDTH 16
#define BROKEN_BRICK_EFFECT_BBOX_HEIGHT 16
struct Rubble {
    float vx, vy, x, y, ay;
};

class CBrokenBrickEffect :
    public CGameObject
{
private:
    /*
    * There are 4 rubbles in this effect.
    * Each rubble has vx, vy, x, y and ay
    */
    Rubble rubbles[BROKEN_BRICK_EFFECT_RUBBLES_AMOUNT];

    // Timer to check before this object destroyed
    // ULONGLONG timer;
protected:
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b);

    
public:
    CBrokenBrickEffect(float x, float y);
    virtual int IsBlocking() { return 0; }
};

