#include "WorldMapKeyHandler.h"
#include "MarioWorldMap.h"
#include "WorldScene.h"

void CWorldMapKeyHandler::KeyState(BYTE* states)
{
	
}

void CWorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	CMarioWorldMap* mario = (CMarioWorldMap*)((CWorldScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode) {
		case DIK_1: mario->SetLevel(MARIO_WORLD_MAP_LEVEL_SMALL); break;
		case DIK_2: mario->SetLevel(MARIO_WORLD_MAP_LEVEL_BIG); break;
		case DIK_3: mario->SetLevel(MARIO_WORLD_MAP_LEVEL_RACCOON); break;
		case DIK_UP: mario->SetState(MARIO_WORLD_MAP_STATE_MOVING_UP); break;
		case DIK_LEFT: mario->SetState(MARIO_WORLD_MAP_STATE_MOVING_LEFT); break;
		case DIK_RIGHT: mario->SetState(MARIO_WORLD_MAP_STATE_MOVING_RIGHT); break;
		case DIK_DOWN: mario->SetState(MARIO_WORLD_MAP_STATE_MOVING_DOWN); break;
		case DIK_A: mario->EnterWorld(); break;
	}
}

void CWorldMapKeyHandler::OnKeyUp(int KeyCode)
{
}
