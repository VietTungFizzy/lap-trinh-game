#pragma once
#include "GameObject.h"
#define VENUS_FIRE_TRAP_BBOX_WIDTH	16
#define VENUS_FIRE_TRAP_BBOX_HEIGHT	32

#define VENUS_FIRE_TRAP_TRAVEL_DISTANCE 23

#define VENUS_FIRE_TRAP_INACTIVE_STATE 0
#define VENUS_FIRE_TRAP_GO_UP_STATE 1
#define VENUS_FIRE_TRAP_FIRE_STATE 2
#define VENUS_FIRE_TRAP_GO_DOWN_STATE 3
#define VENUS_FIRE_TRAP_COOLDOWN_STATE 4
#define VENUS_FIRE_TRAP_PREPARE_TO_FIRE_STATE 5

#define ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_LEFT 5002
#define ID_ANI_VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT 5003
#define ID_ANI_VENUS_FIRE_TRAP_LOOK_UP_LEFT 5004
#define ID_ANI_VENUS_FIRE_TRAP_LOOK_UP_RIGHT 5005
#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_DOWN_LEFT 33000
#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_DOWN_RIGHT 33007
#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_UP_LEFT 33002
#define VENUS_FIRE_TRAP_SPRITE_ID_LOOK_UP_RIGHT 33005

#define VENUS_FIRE_TRAP_COOLDOWN 2000
#define VENUS_FIRE_TRAP_WATCHING_TIME 2000
#define VENUS_FIRE_TRAP_PREPARE_TIME 500

#define VENUS_FIRE_TRAP_LOOK_UP_LEFT 0
#define VENUS_FIRE_TRAP_LOOK_UP_RIGHT 1
#define VENUS_FIRE_TRAP_LOOK_DOWN_LEFT 2
#define VENUS_FIRE_TRAP_LOOK_DOWN_RIGHT 3

#define VENUS_FIRE_TRAP_MAX_RANGE 128
#define VENUS_FIRE_TRAP_MIN_RANGE 16

#define VENUS_FIRE_TRAP_GO_UP_SPEED 0.02f
#define VENUS_FIRE_TRAP_OFFSET_BOTTOM_Y 8
#define VENUS_FIRE_TRAP_BULLET_SPAWN_Y_LOW_OFFSET 6
#define VENUS_FIRE_TRAP_BULLET_SPAWN_Y_HIGH_OFFSET 8
class CVenusFireTrap :
    public CGameObject
{
private:
	float topY, bottomY;
	float pivotPoint_y;
	bool isShot;
	CGameObject* player;
	CGameObject* bullet;
	ULONGLONG timer_start;
private:
	int decideWhereToLook();
	bool isPlayerInRange();
	bool isPlayerTooClose();
	void CalculatePositionForBullet(float& x, float& y, int & angle);
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	void SetPlayer(CGameObject* player) { this->player = player; }
	void SetState(int state);
	int IsCollidable() { return 1; };

	CVenusFireTrap(float x, float y, int type);
};

