#pragma once
#include "Scene.h"
#include "Game.h"
#include "GameObject.h"

class CIntroScene :
    public CScene
{
private:
	// List of objects that can have impact with the game
	vector<LPGAMEOBJECT> objects;

	// List of objects don't have any impact with the game => Just serve as decoration
	vector<LPGAMEOBJECT> decoratedObjects;

protected:
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_BACKGROUND_COLOR(string line);

	void LoadAssets(LPCWSTR assetFile);
public:
	CIntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Unload();
	virtual void Update(DWORD dt);
	virtual void Render();

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

