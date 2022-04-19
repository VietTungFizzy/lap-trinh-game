#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_DEFLECT_SPEED_Y 0.6f
#define GOOMBA_DEFLECT_SPEED_X 0.1f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7
#define GOOMBA_RENDERING_OFFSET_Y 1

#define GOOMBA_DIE_TIMEOUT 500
#define GOOMBA_CONTACT_TIMEOUT 150

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE_FOR_MARIO 200
#define GOOMBA_STATE_CONTACT_WITH_SHELL 201
#define GOOMBA_STATE_DIE_FOR_SHELL 202

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_GOOMBA_CONTACT_WITH_SHELL_EFFECT 5112
#define ID_ANI_GOOMBA_DIE_FOR_SHELL 5113


class CGoomba : public CGameObject
{
protected:
	int isCollidable;
	float ax;				
	float ay; 

	ULONGLONG die_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return isCollidable; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y, int type);
	virtual void SetState(int state);
};