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
	// Properties can be shown on HUD
	int point;
	int lives;
	int power;
	int coin;
	int time;
	int cards[MAXIMUM_CARDS];
	int world;

	// Properties don't show on HUD
	// 
	// Teleport to hidden zone properties
	int marioLevel;
	float marioDestX;
	float marioDestY;
	int destSceneId;
public:
	static CGlobalState* GetInstance();
	void Init();
	void ResetTeleportProperties();
};

