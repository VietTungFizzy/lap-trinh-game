#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"
#include "AssetIDs.h"
#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 
	if (mario->IsFlagOn(FLAG_IN_CUT_SCENE)) return;

	switch (KeyCode)
	{
	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			mario->TailAttack();
			key_timer[DIK_A] = GetTickCount64();
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			if (mario->IsFlagOn(FLAG_ON_PLATFORM)) {
				mario->SetState(MARIO_STATE_JUMP);
			}
			else {
				bool isPlayerSpammingKey = GetTickCount64() - key_timer[DIK_S] <= KEY_DELAY;
				if (isPlayerSpammingKey) {
					if (mario->IsFlagOn(FLAG_MAX_POWER)) mario->SetState(MARIO_STATE_FLY_UP);
					else mario->SetState(MARIO_STATE_SLOW_FALLING);
				}
			}
			key_timer[DIK_S] = GetTickCount64();
		}
		else {
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsFlagOn(FLAG_IN_CUT_SCENE)) return;

	switch (KeyCode)
	{
	case DIK_A:
		if (mario->IsFlagOn(FLAG_GRABBING)) {
			// Set state kicking
			mario->SetState(MARIO_STATE_KICKING);
		}
		break;
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_RACCOON) {
			// TODO: need to find a better way to detect slow falling
			if (!mario->IsFlagOn(FLAG_SLOW_FALLING) && !mario->IsFlagOn(FLAG_MAX_POWER)) {
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
			}
		}
		else {
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
		}
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsFlagOn(FLAG_IN_CUT_SCENE)) return;
	ULONGLONG now = GetTickCount64();
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A) && 
			mario->IsFlagOn(FLAG_ON_PLATFORM) &&
			now - key_timer[DIK_A] > KEY_DELAY)
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A) && 
			mario->IsFlagOn(FLAG_ON_PLATFORM) &&
			now - key_timer[DIK_A] > KEY_DELAY)
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else {
		mario->SetState(MARIO_STATE_IDLE);
	}

	if (now - key_timer[DIK_S] > KEY_DELAY) mario->SetFlagOff(FLAG_SLOW_FALLING);
}