#include "IntroKeyHandler.h"
#include "IntroScene.h"
#include "TitleMenu.h"

void CIntroKeyHandler::KeyState(BYTE* states)
{
}

void CIntroKeyHandler::OnKeyDown(int KeyCode)
{
	CTitleMenu* menu = (CTitleMenu*)((CIntroScene*)CGame::GetInstance()->GetCurrentScene())->GetMenu();
	switch (KeyCode) {
		case DIK_Q: {
			menu->SwitchAmountOfPlayer();
			break;
		}
		case DIK_W: {
			menu->StartGame();
			break;
		}
	}
}

void CIntroKeyHandler::OnKeyUp(int KeyCode)
{
}
