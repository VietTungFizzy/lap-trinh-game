#include "Animations.h"
#include "Sprites.h"
#include "debug.h"
#include "Mushroom.h"
#include "Mario.h"
#include "RewardingBrick.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "ScoreText.h"
#include "SuperLeaf.h"
#include "PSwitch.h"

CRewardingBrick::CRewardingBrick(float x, float y, int rewarding, int type, int spriteId): CBrick(x, y, spriteId, type)
{
	this->state = REWARDING_BRICK_NORMAL_STATE;
	this->rewarding = rewarding;
	this->initY = y;
	this->xEffect = x;
	this->yEffect = y;
	this->vyEffect = 0.0f;
	this->rewardDirection = MUSHROOM_DIRECTION_LEFT;
}
void CRewardingBrick::Render()
{
	if (state == REWARDING_BRICK_NORMAL_STATE) {
		CAnimations* animations = CAnimations::GetInstance();
		int aniId = (rewarding == REWARDING_1_UP_MUSHROOM || rewarding == REWARDING_P_SWITCH) ? 
			ID_ANI_REWARDING_BRICK_SPECIAL_ITEM : ID_ANI_REWARDING_BRICK_NORMAL_ITEM;
		animations->Get(aniId)->Render(x, y);
	}
	else {
		CSprites* s = CSprites::GetInstance();
		s->Get(this->spriteId)->Draw(x, y);

		if (state == REWARDING_BRICK_COIN_GO_UP_STATE || state == REWARDING_BRICK_COIN_GO_DOWN_STATE) {
			CAnimations* animations = CAnimations::GetInstance();
			animations->Get(ID_ANI_REWARDING_BRICK_COIN)->Render(xEffect, yEffect);
		}
	}
}

void CRewardingBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	switch (state) {
		case REWARDING_BRICK_GO_UP_STATE:
		{
			y -= vy * dt;
			if (abs(y - initY) > REWARDING_BRICK_GO_UP_DISTANCE) {
				float temp = initY;
				y = initY;
				vy = 0;
				switch (rewarding) {
				case REWARDING_COIN:
					{
						initY = temp - REWARDING_BRICK_BBOX_HEIGHT;
						yEffect = temp - REWARDING_BRICK_BBOX_HEIGHT;
						SetState(REWARDING_BRICK_COIN_GO_UP_STATE);
						break;
					}
				case REWARDING_SUPER_ITEM:
					{
						CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
						CMario* player = (CMario*)scence->GetPlayer();
						CGameObject* reward;
						if (player->GetLevel() == MARIO_LEVEL_SMALL) {
							float px, py;
							player->GetPosition(px, py);
							rewardDirection = (px > x) ? MUSHROOM_DIRECTION_LEFT : MUSHROOM_DIRECTION_RIGHT;
							reward = new CMushroom(x, y, rewardDirection, MUSHROOM_TYPE_SUPER, OBJECT_TYPE_MUSHROOM);
							scence->AddObjects(reward, true);
							reward->SetState(MUSHROOM_STATE_INACTIVE);
						}
						else {
							reward = new CSuperLeaf(x, y, OBJECT_TYPE_SUPER_LEAF);
							scence->AddObjects(reward, false);
							reward->SetState(SUPER_LEAF_STATE_GO_UP_INVISIBLE);
						}

						SetState(REWARDING_BRICK_FINISHED_STATE);
						break;
					}
				case REWARDING_1_UP_MUSHROOM:
					{
						CGameObject* reward;
						CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
						CMario* player = (CMario*)scence->GetPlayer();
						float px, py;
						player->GetPosition(px, py);
						rewardDirection = (px > x) ? MUSHROOM_DIRECTION_LEFT : MUSHROOM_DIRECTION_RIGHT;
						reward = new CMushroom(x, y, rewardDirection, MUSHROOM_TYPE_1_UP, OBJECT_TYPE_MUSHROOM);
						scence->AddObjects(reward, true);
						reward->SetState(MUSHROOM_STATE_INACTIVE);
						SetState(REWARDING_BRICK_FINISHED_STATE);
						break;
					}
				case REWARDING_P_SWITCH:
					{
						CGameObject* reward;
						CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
						float reward_x = x;
						float reward_y = y - REWARDING_BRICK_BBOX_HEIGHT / 2 - P_SWITCH_BBOX_HEIGHT / 2;
						reward = new CPSwitch(reward_x, reward_y, OBJECT_TYPE_P_SWITCH);
						scence->AddObjects(reward, true);
						SetState(REWARDING_BRICK_FINISHED_STATE);
						break;
					}
				}
			}
			break;
		}
		case REWARDING_BRICK_COIN_GO_UP_STATE:
		{
			yEffect += vyEffect * dt;
			if (abs(yEffect - initY) > REWARING_BRICK_COIN_DISTANCE) {
				SetState(REWARDING_BRICK_COIN_GO_DOWN_STATE);
			}
			break;
		}
		case REWARDING_BRICK_COIN_GO_DOWN_STATE:
		{
			yEffect += vyEffect * dt;
			if (yEffect > initY) {
				SetState(REWARDING_BRICK_TEXT_GO_UP_STATE);
			}
			break;
		}
		case REWARDING_BRICK_TEXT_GO_UP_STATE:
		{
			CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
			scene->AddObjects(new CScoreText(xEffect, yEffect, SCORE_TEXT_100, OBJECT_TYPE_SCORE_TEXT));
			SetState(REWARDING_BRICK_FINISHED_STATE);
			break;
		}
		default: break;
	}
}

void CRewardingBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - REWARDING_BRICK_BBOX_WIDTH / 2;
	t = y - REWARDING_BRICK_BBOX_HEIGHT / 2;
	r = l + REWARDING_BRICK_BBOX_WIDTH;
	b = t + REWARDING_BRICK_BBOX_HEIGHT;
}

void CRewardingBrick::SetState(int state)
{
	switch (state) {
		case REWARDING_BRICK_GO_UP_STATE:
		{
			vy = REWARDING_BRICK_GO_UP_SPEED;
			break;
		}
		case REWARDING_BRICK_COIN_GO_UP_STATE:
		{
			vyEffect = -REWARDING_BRICK_COIN_SPEED;
			break;
		}
		case REWARDING_BRICK_COIN_GO_DOWN_STATE:
		{
			vyEffect = REWARDING_BRICK_COIN_SPEED;
			break;
		}
		default: break;
	}
	CGameObject::SetState(state);
}
