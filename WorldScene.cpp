#include <iostream>
#include <fstream>

#include "WorldScene.h"
#include "Utils.h"
#include "Textures.h"
#include "AssetIDs.h"

#include "Decorated_Obj.h"
#include "WorldMapNode.h"
#include "PortalWorldMap.h"

#include "WorldMapKeyHandler.h"

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_CAMERA_BOUNDARIES 3
#define SCENE_SECTION_BACKGROUND_COLOR 4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024


void CWorldScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CWorldScene::_ParseSection_ANIMATIONS(string line)
{

	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (size_t i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CWorldScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CWorldScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;
	switch (object_type) {
		case OBJECT_TYPE_MARIO_WORLD_MAP: {

			return;
			break;
		}
		case OBJECT_TYPE_DECORATED: {
			if (tokens.size() == DECORATED_OBJECT_SINGLE_CELL_PARAMS) {
				int sprite_id = atoi(tokens[3].c_str());
				obj = new CDecoratedObject(x, y, sprite_id, object_type);
			}
			else if (tokens.size() == DECORATED_OBJECT_MULTIPLE_CELL_PARAMS) {
				int length = atoi(tokens[3].c_str());
				int height = atoi(tokens[4].c_str());
				int sprite_id = atoi(tokens[5].c_str());
				obj = new CDecoratedObject(x, y, sprite_id, object_type, length, height);
			}
			else if (tokens.size() == DECORATED_OBJECT_ANIMATED_PARAMS) {
				int length = atoi(tokens[3].c_str());
				int height = atoi(tokens[4].c_str());
				int sprite_id = atoi(tokens[5].c_str());
				int isAnimated = atoi(tokens[6].c_str());
				obj = new CDecoratedObject(x, y, sprite_id, object_type, length, height, isAnimated);
			}
			else if (tokens.size() == DECORATED_OBJECT_MULTIPLE_CELL_CUSTOM_WIDTH_HEIGHT_PARAMS) {
				int length = atoi(tokens[3].c_str());
				int height = atoi(tokens[4].c_str());
				int sprite_id = atoi(tokens[5].c_str());
				int isAnimated = atoi(tokens[6].c_str());
				int cellWith = atoi(tokens[7].c_str());
				int cellHeight = atoi(tokens[8].c_str());
				obj = new CDecoratedObject(
					x, y,
					sprite_id, object_type,
					length, height,
					isAnimated,
					cellWith, cellHeight
				);
			}
			else {
				DebugOut(L"[ERROR] The input is invalid\n");
				return;
			}
			decoratedObjects.push_back(obj);
			return;
			break;
		}
		case OBJECT_TYPE_WORLD_MAP_NODE: {
			int spriteId = atoi(tokens[3].c_str());
			int adj_t = atoi(tokens[4].c_str());
			int adj_l = atoi(tokens[5].c_str());
			int adj_r = atoi(tokens[6].c_str());
			int adj_d = atoi(tokens[7].c_str());
			obj = new CWorldMapNode(x, y, object_type, adj_t, adj_l, adj_r, adj_d, spriteId);
			break;
		}
		case OBJECT_TYPE_PORTAL_WORLD_MAP: {
			int spriteId = atoi(tokens[3].c_str());
			int adj_t = atoi(tokens[4].c_str());
			int adj_l = atoi(tokens[5].c_str());
			int adj_r = atoi(tokens[6].c_str());
			int adj_d = atoi(tokens[7].c_str());
			int sceneId = atoi(tokens[8].c_str());
			obj = new CPortalWorldMap(x, y, object_type, adj_t, adj_l, adj_r, adj_d, spriteId, sceneId);
			break;
		}
	}

	objects.push_back(obj);
}

void CWorldScene::_ParseSection_CAMERA_BOUNDARIES(string line)
{
}

void CWorldScene::_ParseSection_BACKGROUND_COLOR(string line)
{
	vector<string> tokens = split(line);

	// Skip invalid line
	if (tokens.size() < 3) return;

	float red = (float)atof(tokens[0].c_str());
	float blue = (float)atof(tokens[1].c_str());
	float green = (float)atof(tokens[2].c_str());

	bg_color = D3DXCOLOR(red / 255, blue / 255, green / 255, 0.0f);
}

void CWorldScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

CWorldScene::CWorldScene(int id, LPCWSTR filePath): CScene(id, filePath)
{
	key_handler = new CWorldMapKeyHandler(this);
}

void CWorldScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line == "[CAMERA_BOUNDARIES]") { section = SCENE_SECTION_CAMERA_BOUNDARIES; continue; }
		if (line == "[BACKGROUND_COLOR]") { section = SCENE_SECTION_BACKGROUND_COLOR; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_CAMERA_BOUNDARIES: _ParseSection_CAMERA_BOUNDARIES(line); break;
		case SCENE_SECTION_BACKGROUND_COLOR: _ParseSection_BACKGROUND_COLOR(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CWorldScene::Unload()
{
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

void CWorldScene::Update(DWORD dt)
{
}

void CWorldScene::Render()
{
	// Always render decorated objects first
	for (size_t i = 0; i < decoratedObjects.size(); i++) {
		decoratedObjects[i]->Render();
	}

	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->Render();
	}
}

void CWorldScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

void CWorldScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CWorldScene::IsGameObjectDeleted),
		objects.end());
}

bool CWorldScene::IsGameObjectDeleted(const LPGAMEOBJECT& o)
{
	return o == NULL;
}
