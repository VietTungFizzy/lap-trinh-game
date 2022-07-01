#include "GlobalState.h"
#include"Card.h"
#include "AssetIDs.h"

#define DEFAULT_LIVES_FOR_MARIO 4
#define DEFAULT_VALUE_WORLD 1

CGlobalState* CGlobalState::__instance = NULL;

CGlobalState* CGlobalState::GetInstance()
{
	if (__instance == NULL) __instance = new CGlobalState();
    return __instance;
}

void CGlobalState::Init()
{
	time = coin = point = power = 0;
	lives = DEFAULT_LIVES_FOR_MARIO;
	world = DEFAULT_VALUE_WORLD;
	for (int i = 0; i < MAXIMUM_CARDS; i++) cards[i] = CARD_STATE_EMPTY;
	marioLevel = MARIO_LEVEL_SMALL;
	marioDestX = -1;
	marioDestY = -1;
	destSceneId = -1;
}

void CGlobalState::ResetTeleportProperties()
{
	marioLevel = MARIO_LEVEL_SMALL;
	marioDestX = -1;
	marioDestY = -1;
	destSceneId = -1;
}
