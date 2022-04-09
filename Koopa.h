#pragma once
#include "GameObject.h"
#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT_NORMAL 24
#define KOOPA_BBOX_HEIGHT_SHELL 14

#define KOOPA_ANI_ID_WALKING_LEFT 5006
#define KOOPA_ANI_ID_WALKING_RIGHT 5007
#define KOOPA_ANI_ID_SHELL  5008
#define KOOPA_SPRITE_ID_SHELL 33012
#define KOOPA_ANI_ID_TRY_TO_POP_OUT_FULL_TIME 5009
#define KOOPA_ANI_ID_TRY_TO_POP_OUT_HALF_TIME 5010
#define KOOPA_ANI_ID_TRY_TO_POP_OUT_QUARTER_TIME 5011

#define KOOPA_STATE_NORMAL 0
#define KOOPA_STATE_SHELL_MOVING 1
#define KOOPA_STATE_SHELL_STAY_STILL 2
#define KOOPA_STATE_RETURNING_TO_NORMAL 3

#define KOOPA_GRAVITY 0.002f
#define KOOPA_NORMAL_SPEED 0.03f
#define KOOPA_SHELL_SPEED 0.1f

#define KOOPA_RENDER_OFFSET_Y 1

#define KOOPA_TIME_BEFORE_START_RETURNING_TO_NORMAL 2000
#define KOOPA_TIME_RETURNING_TO_NORMAL 2000
#define KOOPA_TIME_NOT_CAUSE_DAMAGE 100
class CKoopa :
    public CGameObject
{
private:
    ULONGLONG timer;
    float boundaries_left, boundaries_right;
    bool isPlayerLeft, isCauseDamageOn;
    float ax, ay;
public:
    CKoopa(float x, float y, int type) : CGameObject(x,y,type){
        this->state = KOOPA_STATE_NORMAL;
        this->vx = KOOPA_NORMAL_SPEED;
        this->boundaries_left = 0;
        this->boundaries_right = 0;
        this->ax = 0;
        this->ay = KOOPA_GRAVITY;
        this->isCauseDamageOn = true;
    }

    virtual void SetState(int state);
    void SetIsPlayerLeft(bool isPlayerLeft) { this->isPlayerLeft = isPlayerLeft; }

    bool GetCauseDamageMode() { return this->isCauseDamageOn; }
protected:
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }
};

