#pragma once
#include "GameObject.h"
#include "AssetIDs.h"
#define MARIO_TAIL_BBOX_WIDTH 6
#define MARIO_TAIL_BBOX_HEIGHT 6

#define MARIO_TAIL_STATE_INACTIVE 0
#define MARIO_TAIL_STATE_ACTIVE 1

class CMarioTail :
    public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() {};
public:
	CMarioTail(float x, float y) : CGameObject(x, y, OBJECT_TYPE_MARIO_TAIL) {
		SetState(MARIO_TAIL_STATE_INACTIVE);
	}

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	void OnCollisionWith(CGameObject* obj);
};

