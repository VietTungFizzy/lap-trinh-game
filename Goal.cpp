#include "Goal.h"
#include "PlayScene.h"
#include "RouletteCard.h"
#include "GlobalState.h"
#include "Card.h"

void CGoal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GOAL_BBOX_WIDTH / 2;
	top = y - GOAL_BBOX_HEIGHT / 2;
	right = left + GOAL_BBOX_WIDTH;
	bottom = top + GOAL_BBOX_HEIGHT;
}

void CGoal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - timer > GOAL_DELAY_TIME_BEFORE_SWITCH_STATE) {
		switch (state) {
		case GOAL_STATE_MUSHROOM: SetState(GOAL_STATE_FLOWER); break;
		case GOAL_STATE_FLOWER: SetState(GOAL_STATE_STAR); break;
		case GOAL_STATE_STAR: SetState(GOAL_STATE_MUSHROOM); break;
		}
	}
}

void CGoal::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	int spriteId = ID_SPRITE_GOAL_MUSHROOM;
	switch (state) {
	case GOAL_STATE_MUSHROOM: spriteId = ID_SPRITE_GOAL_MUSHROOM; break;
	case GOAL_STATE_FLOWER: spriteId = ID_SPRITE_GOAL_FLOWER; break;
	case GOAL_STATE_STAR: spriteId = ID_SPRITE_GOAL_STAR; break;
	}
	sprites->Get(spriteId)->Draw(x, y);
}

void CGoal::SetState(int state)
{
	switch (state) {
	case GOAL_STATE_MUSHROOM:
	case GOAL_STATE_FLOWER:
	case GOAL_STATE_STAR:
		timer = GetTickCount64();
		break;
	case GOAL_STATE_TOUCHED:
		{
			CPlayScene* scene = (CPlayScene * )CGame::GetInstance()->GetCurrentScene();
			CGlobalState* gt = CGlobalState::GetInstance();
			int rouletteCardState, globalCardState;
			switch (this->state) {
				case GOAL_STATE_MUSHROOM: {
					rouletteCardState = ROULETTE_CARD_STATE_MUSHROOM; 
					globalCardState = CARD_STATE_MUSHROOM;
					break;
				}
				case GOAL_STATE_FLOWER: {
					rouletteCardState = ROULETTE_CARD_STATE_FLOWER; 
					globalCardState = CARD_STATE_FLOWER;
					break; 
				}
				case GOAL_STATE_STAR: {
					rouletteCardState = ROULETTE_CARD_STATE_STAR; 
					globalCardState = CARD_STATE_STAR;
					break;
				}
			}
			for (int i = 0; i < MAXIMUM_CARDS; i++) {
				if (gt->cards[i] == CARD_STATE_EMPTY) {
					gt->cards[i] = globalCardState;
					break;
				}
			}
			scene->AddObjects(new CRouletteCard(x, y, rouletteCardState));
			Delete();
			break;
		}
	}
	CGameObject::SetState(state);
}
