#pragma once
#define MAXIMUM_CARDS 3
/*
*	Global state can be used to connect between scenes
*	It also passes information to hud
*/
class CGlobalState
{
private:
	static CGlobalState* __instance;
public:
	int point;
	int lives;
	int power;
	int coin;
	int time;
	int cards[MAXIMUM_CARDS];
	int world;
public:
	static CGlobalState* GetInstance();
	void Init();
};

