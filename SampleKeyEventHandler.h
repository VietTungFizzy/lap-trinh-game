#pragma once

#include "Scene.h"
#include<unordered_map>

#define KEY_DELAY 250
class CSampleKeyHandler: public CSceneKeyHandler
{
private:
	std::unordered_map<int, ULONGLONG> key_timer;
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CSampleKeyHandler(LPSCENE s) :CSceneKeyHandler(s) {};
};
