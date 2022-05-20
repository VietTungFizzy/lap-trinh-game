#pragma once
#include "Plant.h"
#pragma region BBOX
#define VENUS_FIRE_TRAP_BBOX_WIDTH	16
#define VENUS_FIRE_TRAP_BBOX_HEIGHT	32
#pragma endregion

#pragma region STATE
#define VENUS_FIRE_TRAP_INACTIVE_STATE 0
#define VENUS_FIRE_TRAP_GO_UP_STATE 1
#define VENUS_FIRE_TRAP_FIRE_STATE 2
#define VENUS_FIRE_TRAP_GO_DOWN_STATE 3
#define VENUS_FIRE_TRAP_COOLDOWN_STATE 4
#define VENUS_FIRE_TRAP_PREPARE_TO_FIRE_STATE 5
#pragma endregion

#pragma region TYPE
#define VENUS_FIRE_TRAP_TYPE_RED 0
#define VENUS_FIRE_TRAP_TYPE_GREEN 1
#pragma endregion

#pragma region ANIMATION & SPRITE
#define ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_LEFT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 5004 : 5100)
#define ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 5005 : 5101)

#define ID_ANI_VENUS_FIRE_TRAP_LOOK_UP_LEFT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 5006 : 5102)
#define ID_ANI_VENUS_FIRE_TRAP_LOOK_UP_RIGHT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 5007 : 5103)

#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_DOWN_LEFT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 33000 : 33100)
#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_DOWN_RIGHT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 33007 : 33107)

#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_UP_LEFT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 33002 : 33102) 
#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_UP_RIGHT(type) (type == VENUS_FIRE_TRAP_TYPE_RED ? 33005 : 33105)

#pragma endregion

#pragma region TIME
#define VENUS_FIRE_TRAP_COOLDOWN 2000
#define VENUS_FIRE_TRAP_WATCHING_TIME 2000
#define VENUS_FIRE_TRAP_PREPARE_TIME 500
#pragma endregion

#pragma region LOOK DIRECTION
#define VENUS_FIRE_TRAP_LOOK_UP_LEFT 0
#define VENUS_FIRE_TRAP_LOOK_UP_RIGHT 1
#define VENUS_FIRE_TRAP_LOOK_DOWN_LEFT 2
#define VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT 3
#pragma endregion

#pragma region PHYSIC
#define VENUS_FIRE_TRAP_MAX_RANGE 128
#define VENUS_FIRE_TRAP_MIN_RANGE 16

#define VENUS_FIRE_TRAP_GO_UP_SPEED 0.02f
#define VENUS_FIRE_TRAP_OFFSET_BOTTOM_Y 8
#define VENUS_FIRE_TRAP_BULLET_SPAWN_Y_LOW_OFFSET 6
#define VENUS_FIRE_TRAP_BULLET_SPAWN_Y_HIGH_OFFSET 8

#define VENUS_FIRE_TRAP_TRAVEL_DISTANCE 23
#pragma endregion
class CVenusFireTrap :
    public CPlant
{
private:
	float pivotPoint_y;
	bool isShot;
	CGameObject* bullet;
	int venusFireType;

private:
	int decideWhereToLook();
	bool isPlayerTooClose();
	void CalculatePositionForBullet(float& x, float& y, int & angle);
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	void SetState(int state);

	CVenusFireTrap(float x, float y, int type, int venusFireType);
};

