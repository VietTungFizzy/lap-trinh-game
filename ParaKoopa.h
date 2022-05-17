#pragma once
#include "GameObject.h"
#include "Koopa.h"
#pragma region BBOX
#define PARA_KOOPA_BBOX_WIDTH 16
#define PARA_KOOPA_BBOX_HEIGHT_NORMAL 24
#define PARA_KOOPA_BBOX_HEIGHT_SHELL 14
#pragma endregion

#pragma region PHYSIC SETTINGS
#define PARA_KOOPA_GRAVITY 0.0008f
#define PARA_KOOPA_NORMAL_SPEED 0.04f
#define PARA_KOOPA_SHELL_SPEED 0.1f

#define PARA_KOOPA_JUMP_SPEED 0.27f
#pragma endregion

#pragma region ANIMATIONS
#define PARA_KOOPA_ANI_ID_WALKING_LEFT 5018
#define PARA_KOOPA_ANI_ID_WALKING_RIGHT 5019
#define PARA_KOOPA_ANI_ID_SHELL_MOVING  5020
#define PARA_KOOPA_ANI_ID_SHELL_STAY_STILL 5024
#define PARA_KOOPA_ANI_ID_TRY_TO_POP_OUT_FULL_TIME 5021
#define PARA_KOOPA_ANI_ID_TRY_TO_POP_OUT_HALF_TIME 5022
#define PARA_KOOPA_ANI_ID_TRY_TO_POP_OUT_QUARTER_TIME 5023

#define PARA_KOOPA_ANI_ID_WING_LEFT 5015
#define PARA_KOOPA_ANI_ID_WING_RIGHT 5014
#pragma endregion

#pragma region STATES
#define PARA_KOOPA_STATE_NORMAL_WITH_WING 0
#define PARA_KOOPA_STATE_SHELL_MOVING 1
#define PARA_KOOPA_STATE_SHELL_STAY_STILL 2
#define PARA_KOOPA_STATE_RETURNING_TO_NORMAL 3
#define PARA_KOOPA_STATE_NORMAL_WITHOUT_WING 4
#pragma endregion

#pragma region TIMER SETTINGS
#define PARA_KOOPA_TIME_BEFORE_START_RETURNING_TO_NORMAL 4000
#define PARA_KOOPA_TIME_RETURNING_TO_NORMAL 2000
#define PARA_KOOPA_TIME_NOT_CAUSE_DAMAGE 100
#pragma endregion

#pragma region MISC
#define PARA_KOOPA_RENDER_OFFSET_Y 1
#define PARA_KOOPA_RENDER_WING_OFFSET_X 5
#define PARA_KOOPA_RENDER_WING_OFFSET_Y 5
#pragma endregion
class CParaKoopa :
    public CKoopa
{
    /*
private:
    float ax, ay;
    ULONGLONG timer;
    */
public:
    CParaKoopa(float x, float y, int type, int koopaType = KOOPA_TYPE_RED): CKoopa(x, y, type, koopaType) {
        this->isCauseDamageOn = true;
        this->ax = 0;
        this->ay = PARA_KOOPA_GRAVITY;
        SetState(PARA_KOOPA_STATE_NORMAL_WITH_WING);
        this->vx = -PARA_KOOPA_NORMAL_SPEED;
        this->isGrabbed = false;
    }

    virtual void SetState(int state);
    bool GetCauseDamageMode() { return this->isCauseDamageOn; }
    void SetIsPlayerLeft(bool isPlayerLeft) { this->isPlayerLeft = isPlayerLeft; }
    /*
    void SetGrabbedFlag(bool flag) { isGrabbed = flag; }
    bool GetGrabbedFlag() { return isGrabbed; }
    */
protected:
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    /*
    virtual int IsCollidable() { return 1; }
    virtual int IsBlocking() { return 0; }
    */
};

