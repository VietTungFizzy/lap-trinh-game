#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"


class CPlayScene: public CScene
{
private:
	int topBoundaries, leftBoundaries, rightBoundaries, bottomBoundaries;
	BOOLEAN isCameraYLocked;
	BOOLEAN isSwitchOn;
	ULONGLONG timer;
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	// List of objects that can have impact with the game
	vector<LPGAMEOBJECT> objects;

	// List of objects don't have any impact with the game => Just serve as decoration
	vector<LPGAMEOBJECT> decoratedObjects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_CAMERA_BOUNDARIES(string line);

	void LoadAssets(LPCWSTR assetFile);
	
	void AdjustCameraPosition();
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	// Dynamically add object to play scene
	// If add at the beginning of list of objects, it guarantees
	// the object will be rendered behind all object
	// If add at the end of list of objects, it guarantees
	// the object will be rendered on top all object
	void AddObjects(LPGAMEOBJECT obj, bool isAddAtTheBeginning = false);

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	void SetSwitchOn() { isSwitchOn = true; timer = GetTickCount64(); }
	bool GetSwitchStatus() { return isSwitchOn; }

	bool isInCamera(LPGAMEOBJECT obj);

};

typedef CPlayScene* LPPLAYSCENE;

