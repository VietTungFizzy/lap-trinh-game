#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#pragma region PHYSICS_ATTRIBUTE
#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.4f
#define MARIO_JUMP_RUN_SPEED_Y	0.4f

#define MARIO_GRAVITY			0.001f
#define MARIO_GRAVITY_SLOW_FALLING 0.0005f

#define MARIO_JUMP_DEFLECT_SPEED  0.3f
#define MARIO_SLOW_DOWN_COEFFICIENT 5

#define MARIO_FLY_UP_SPEED 0.2f
#define MARIO_SLOW_FALLING_SPEED 0.025f
#pragma endregion

#pragma region STATE

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_SMALL_TO_BIG	700
#define MARIO_STATE_BIG_TO_SMALL	701
#define MARIO_STATE_BIG_TO_RACCOON	702
#define MARIO_STATE_RACCOON_TO_BIG	703

#define MARIO_STATE_FLY_UP 800
#define MARIO_STATE_SLOW_FALLING 801

#define MARIO_STATE_KICKING 900
#pragma endregion

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

#define ID_ANI_MARIO_GRABBING_IDLE_LEFT 100
#define ID_ANI_MARIO_GRABBING_IDLE_RIGHT 110

#define ID_ANI_MARIO_GRABBING_WALK_LEFT 200
#define ID_ANI_MARIO_GRABBING_WALK_RIGHT 210

#define ID_ANI_MARIO_GRABBING_RUNNING_LEFT 211
#define ID_ANI_MARIO_GRABBING_RUNNING_RIGHT 212

#define ID_ANI_MARIO_KICKING_LEFT 300
#define ID_ANI_MARIO_KICKING_RIGHT 310

#define ID_ANI_MARIO_JUMP_WHILE_GRABBING_LEFT 220
#define ID_ANI_MARIO_JUMP_WHILE_GRABBING_RIGHT 230

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_GRABBING_IDLE_LEFT 1610
#define ID_ANI_MARIO_SMALL_GRABBING_IDLE_RIGHT 1611

#define ID_ANI_MARIO_SMALL_GRABBING_WALK_LEFT 1620
#define ID_ANI_MARIO_SMALL_GRABBING_WALK_RIGHT 1621

#define ID_ANI_MARIO_SMALL_GRABBING_RUNNING_LEFT 1622
#define ID_ANI_MARIO_SMALL_GRABBING_RUNNING_RIGHT 1623

#define ID_ANI_MARIO_SMALL_KICKING_LEFT 1630
#define ID_ANI_MARIO_SMALL_KICKING_RIGHT 1631

#define ID_ANI_MARIO_SMALL_JUMP_WHILE_GRABBING_LEFT 1640
#define ID_ANI_MARIO_SMALL_JUMP_WHILE_GRABBING_RIGHT 1641

// TRANSITION
#define ID_ANI_MARIO_BIG_TO_SMALL_RIGHT	1702
#define ID_ANI_MARIO_BIG_TO_SMALL_LEFT	1703

#define ID_ANI_MARIO_SMALL_TO_BIG_RIGHT 1700
#define ID_ANI_MARIO_SMALL_TO_BIG_LEFT 1701

#define ID_ANI_MARIO_BIG_RACCOON_TRANSITION 3000

// RACCOON
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 1800
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 1802

#define ID_ANI_MARIO_RACCOON_WALK_LEFT 1801
#define ID_ANI_MARIO_RACCOON_WALK_RIGHT 1803

#define ID_ANI_MARIO_RACCOON_JUMP_LEFT 1804
#define ID_ANI_MARIO_RACCOON_JUMP_RIGHT 1805

#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 1806
#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 1807

#define	ID_ANI_MARIO_RACCOON_SIT_LEFT	1808
#define	ID_ANI_MARIO_RACCOON_SIT_RIGHT	1809

#define ID_ANI_MARIO_RACCOON_FLY_LEFT	1810
#define ID_ANI_MARIO_RACCOON_FLY_RIGHT	1811

#define ID_ANI_MARIO_RACCOON_RUN_NO_POWER_LEFT 1812
#define ID_ANI_MARIO_RACCOON_RUN_NO_POWER_RIGHT 1813

#define ID_ANI_MARIO_RACCOON_RUN_FULL_POWER_LEFT 1814
#define ID_ANI_MARIO_RACCOON_RUN_FULL_POWER_RIGHT 1815

#define ID_ANI_MARIO_RACCOON_TURNING_FROM_LEFT_TO_RIGHT 1816
#define ID_ANI_MARIO_RACCOON_TURNING_FROM_RIGHT_TO_LEFT 1817

#define ID_ANI_MARIO_RACCOON_SLOW_FALLING_RIGHT	1819
#define ID_ANI_MARIO_RACCOON_SLOW_FALLING_LEFT 1818

#define ID_ANI_MARIO_RACCOON_GRABBING_IDLE_LEFT 1820
#define ID_ANI_MARIO_RACCOON_GRABBING_IDLE_RIGHT 1821

#define ID_ANI_MARIO_RACCOON_GRABBING_WALK_LEFT 1830
#define ID_ANI_MARIO_RACCOON_GRABBING_WALK_RIGHT 1831

#define ID_ANI_MARIO_RACCOON_GRABBING_RUNNING_LEFT 1832
#define ID_ANI_MARIO_RACCOON_GRABBING_RUNNING_RIGHT 1833

#define ID_ANI_MARIO_RACCOON_KICKING_LEFT 1840
#define ID_ANI_MARIO_RACCOON_KICKING_RIGHT 1841

#define ID_ANI_MARIO_RACCOON_JUMP_WHILE_GRABBING_LEFT 1850
#define ID_ANI_MARIO_RACCOON_JUMP_WHILE_GRABBING_RIGHT 1851
#pragma endregion


#pragma region BBOX_SETTING

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16


#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#pragma endregion

#pragma region MISC

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_UNTOUCHABLE_TIME 1000
#define MARIO_TRANSFORM_BIG_AND_RACCOON_TIME 400
#define MARIO_TRANSFORM_SMALL_AND_BIG_TIME 600
#define MARIO_MAX_POWER_DECREASE_TIME 3000
#define MARIO_KICKING_TIME 100

#define MINIMUM_ACCEL_VALUE 1e-4

#define NO_COMBO 0
#define COMBO_100 1
#define COMBO_200 2
#define COMBO_400 3

#define MARIO_MAX_POWER 50
#define MARIO_GRABBING_OBJ_OFFSET 2
#pragma endregion

#pragma region LEVEL

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON 3

#pragma endregion
class CMario : public CGameObject
{
private:
	BOOLEAN isSitting;
	BOOLEAN isMaxPower;
	BOOLEAN isOnPlatform;
	BOOLEAN isSlowFalling;

	BOOLEAN isGrabbing;
	CGameObject* grabbedObj;

	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int bottomBoundary;
	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	ULONGLONG transition_timer;
	ULONGLONG countdown_timer; // countdown before max power decrease

	int point; 
	int coin;
	int comboCount;
	int powerCount;

	
private:
	// Handling collision logic
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithRewardingBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithParaGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithParaKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

	void GetHit();
	void ScoringWithCombo();
	void ResetUntouchable() { untouchable = 0; untouchable_start = 0; }

public:
	CMario(float x, float y, int b, int type) : CGameObject(x, y, type)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		point = 0;
		bottomBoundary = b;

		powerCount = 0;
		isMaxPower = false;
		isSlowFalling = false;

		transition_timer = 0;
		countdown_timer = 0;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return this->level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	
	void AddPoint(int point) { this->point += point; }
	void ScoringPointWithoutCombo(int point);
	
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool isFullPower() { return isMaxPower; }
	bool isOnTheGround() { return isOnPlatform; }
	bool getSlowFallingFlag() { return isSlowFalling; }
	void setSlowFallingFlag(bool flag) { isSlowFalling = flag; }
	
	bool getGrabbingFlag() { return isGrabbing; }
};