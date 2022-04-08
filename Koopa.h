#pragma once
#include "GameObject.h"
#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT_NORMAL 24
#define KOOPA_BBOX_HEIGHT_SHELL 16

#define KOOPA_ANI_ID_WALKING_LEFT 5006
#define KOOPA_ANI_ID_WALKING_RIGHT 5007
#define KOOPA_ANI_ID_SHELL  5008
#define KOOPA_SPRITE_ID_SHELL 33012

#define KOOPA_STATE_NORMAL 0
#define KOOPA_STATE_SHELL_MOVING 1
#define KOOPA_STATE_SHELL_STAY_STILL 2
#define KOOPA_STATE_RETURNING_TO_NORMAL 3

#define KOOPA_GRAVITY 0.002f
#define KOOPA_NORMAL_SPEED 0.03f

#define KOOPA_RENDER_OFFSET_Y 1
class CKoopa :
    public CGameObject
{
private:
    ULONGLONG timer;
    float boundaries_left, boundaries_right;
    bool isPlayerLeft;
public:
    CKoopa(float x, float y, int type, float boundaries_left, float boundaries_right) : CGameObject(x,y,type){
        this->state = KOOPA_STATE_NORMAL;
        this->vy = KOOPA_GRAVITY;
        this->vx = KOOPA_NORMAL_SPEED;
        this->boundaries_left = boundaries_left;
        this->boundaries_right = boundaries_right;
    }

    virtual void SetState(int state);
    void SetIsPlayerLeft(bool isPlayerLeft) { this->isPlayerLeft = isPlayerLeft; }
protected:
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    // When no collision has been detected (triggered by CCollision::Process)
    virtual void OnNoCollision(DWORD dt);
    // When collision with an object has been detected (triggered by CCollision::Process)
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }
};

