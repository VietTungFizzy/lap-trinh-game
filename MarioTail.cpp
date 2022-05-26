#include "MarioTail.h"
#include "SwitchBrick.h"

void CMarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_TAIL_BBOX_WIDTH / 2;
	top = y - MARIO_TAIL_BBOX_HEIGHT / 2;
	right = left + MARIO_TAIL_BBOX_WIDTH;
	bottom = top + MARIO_TAIL_BBOX_HEIGHT;
}

void CMarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == MARIO_TAIL_STATE_ACTIVE) {
		CCollision::GetInstance()->Process(this, dt, coObjects);
		bool isOverlap;
		// Since we detect collision when mario almost finish wagging tail animation
		// We couldn't use Bounding Box Collision technique
		// Instead, we just detect whether mario tail's bounding box
		// and an object's bounding box overlap each other
		for (int i = 0; i < coObjects->size(); i++) {
			CGameObject* t = coObjects->at(i);
			isOverlap = CCollision::GetInstance()->DetectOverlap(this, t);

			if (isOverlap) OnCollisionWith(t);
		}
	}
}

void CMarioTail::OnCollisionWith(CGameObject* obj)
{
	switch (obj->GetType()) {
	case OBJECT_TYPE_SWITCH_BRICK:
		if (obj->GetState() == SWITCH_BRICK_STATE_BRICK)
			obj->SetState(SWITCH_BRICK_STATE_DESTROYED);
		break;
	}
}
