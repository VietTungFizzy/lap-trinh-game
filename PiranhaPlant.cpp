#include "PiranhaPlant.h"
#include "Animations.h"
#include "PlayScene.h"

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIRANHA_PLANT_BBOX_WIDTH / 2;
	top = y - PIRANHA_PLANT_BBOX_HEIGHT / 2;
	right = left + PIRANHA_PLANT_BBOX_WIDTH;
	bottom = top + PIRANHA_PLANT_BBOX_HEIGHT;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (player == NULL) return;

	switch (state) {
	case PIRANHA_PLANT_STATE_INACTIVE:
		if (isPlayerInRange(PIRANHA_PLANT_MAX_RANGE, PIRANHA_PLANT_MIN_RANGE)) SetState(PIRANHA_PLANT_STATE_GO_UP);
		break;
	case PIRANHA_PLANT_STATE_GO_UP:
		y += vy * dt;
		if (y < topY) {
			SetState(PIRANHA_PLANT_STATE_STAY_UP);
		}
		break;
	case PIRANHA_PLANT_STATE_STAY_UP:
		if (GetTickCount64() - timer_start > PIRANHA_PLANT_STAY_UP_TIME) {
			SetState(PIRANHA_PLANT_STATE_GO_DOWN);
		}
		break;
	case PIRANHA_PLANT_STATE_GO_DOWN:
		y += vy * dt;
		if (y > bottomY) {
			SetState(PIRANHA_PLANT_STATE_COOLDOWN);
		}
		break;
	case PIRANHA_PLANT_STATE_COOLDOWN:
		if (!isPlayerInRange(PIRANHA_PLANT_MAX_RANGE, PIRANHA_PLANT_MIN_RANGE)) {
			SetState(PIRANHA_PLANT_STATE_INACTIVE);
		}
		else if (GetTickCount64() - timer_start > PIRANHA_PLANT_COOLDOWN_TIME) {
			SetState(PIRANHA_PLANT_STATE_GO_UP);
		}
		break;
	}
}

void CPiranhaPlant::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PIRANHA_PLANT)->Render(x, y);
}

CPiranhaPlant::CPiranhaPlant(float x, float y, int type) : CPlant(x, y, type)
{
	player = NULL;
	state = PIRANHA_PLANT_STATE_INACTIVE;
	topY = y - PIRANHA_PLANT_GO_UP_DISTANCE;
	bottomY = y + PIRANHA_PLANT_OFFSET_BOTTOM_Y;
	timer_start = 0;
}

void CPiranhaPlant::SetState(int state)
{
	switch (state) {
	case PIRANHA_PLANT_STATE_GO_UP:
		vy = -PIRANHA_PLANT_GO_UP_SPEED;
		break;
	case PIRANHA_PLANT_STATE_GO_DOWN:
		vy = PIRANHA_PLANT_GO_UP_SPEED;
		break;
	case PIRANHA_PLANT_STATE_COOLDOWN:
	case PIRANHA_PLANT_STATE_STAY_UP:
		vy = 0;
		timer_start = GetTickCount64();
		break;
	}

	CGameObject::SetState(state);
}
