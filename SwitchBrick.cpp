#include "SwitchBrick.h"
#include "BrokenBrickEffect.h"
#include "Animations.h"
#include "PlayScene.h"

void CSwitchBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SWITCH_BRICK_BBOX_WIDTH / 2;
	top = y - SWITCH_BRICK_BBOX_HEIGHT / 2;
	right = left + SWITCH_BRICK_BBOX_WIDTH;
	bottom = top + SWITCH_BRICK_BBOX_HEIGHT;
}

void CSwitchBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CPlayScene* scene = (CPlayScene * )CGame::GetInstance()->GetCurrentScene();
	SetState((scene->GetSwitchStatus() ? SWITCH_BRICK_STATE_COIN : SWITCH_BRICK_STATE_BRICK));
}

void CSwitchBrick::Render()
{
	if (state == SWITCH_BRICK_STATE_BRICK || state == SWITCH_BRICK_STATE_COIN) {
		CAnimations* animations = CAnimations::GetInstance();
		int aniId = (state == SWITCH_BRICK_STATE_BRICK) ? ID_ANI_SWITCH_BRICK_STATE_BRICK : ID_ANI_SWITCH_BRICK_STATE_COIN;
		animations->Get(aniId)->Render(x, y);
	}
}

void CSwitchBrick::SetState(int state)
{
	switch (state) {
	case SWITCH_BRICK_STATE_COIN_EATEN:
		Delete();
		break;
	case SWITCH_BRICK_STATE_DESTROYED:
		{
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			scene->AddObjects(new CBrokenBrickEffect(x, y));
			Delete();
			break;
		}
	}

	CGameObject::SetState(state);
}
