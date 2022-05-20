#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"

#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Decorated_Obj.h"
#include "RewardingBrick.h"
#include "InvinsibleBrick.h"
#include "Mushroom.h"
#include "PlatformTopOnly.h"
#include "VenusFireTrap.h"
#include "Koopa.h"
#include "ParaGoomba.h"
#include "ParaKoopa.h"
#include "PiranhaPlant.h"

#include "SampleKeyEventHandler.h"

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_CAMERA_BOUNDARIES 3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024
#define SCREEN_HEIGHT 240
#define CAMERA_CHECKING_OFFSET 16

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
	isCameraYLocked = false;
}

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		// Make sure add map settings before objects loading
		obj = new CMario(x, y, bottomBoundaries, object_type);
		player = (CMario*)obj;
		DebugOut(L"[INFO] Player object has been created!\n");
		return;
		break;
	}
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y, object_type); break;
	case OBJECT_TYPE_BRICK:
	{
		int sprite_id = atoi(tokens[3].c_str());
		obj = new CBrick(x, y, sprite_id, object_type);
		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y, object_type); break;

	case OBJECT_TYPE_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end,
			object_type
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_DECORATED:
	{
		int sprite_id = atoi(tokens[3].c_str());
		obj = new CDecoratedObject(x, y, sprite_id, object_type);
		decoratedObjects.push_back(obj);
		return;
		break;
	}
	case OBJECT_TYPE_REWARDING_BRICK: 
	{
		int rewarding_type = atoi(tokens[3].c_str());
		obj = new CRewardingBrick(x, y, rewarding_type, object_type);
		
		break;
	}
	case OBJECT_TYPE_INVINSIBLE_BRICK: obj = new CInvinsibleBrick(x, y, object_type); break;
	case OBJECT_TYPE_PLATFORM_TOP_ONLY:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		obj = new CPlatformTopOnly(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, 
			object_type
		);

		break;
	}
	case OBJECT_TYPE_VENUS_FIRE_TRAP: {
		int venus_fire_type = atoi(tokens[3].c_str());
		obj = new CVenusFireTrap(x, y, object_type, venus_fire_type); 
		break; 
	}
	case OBJECT_TYPE_KOOPA: {
		int koopa_type = atoi(tokens[3].c_str());
		obj = new CKoopa(x, y, object_type, koopa_type);
		break;
	}
	case OBJECT_TYPE_PARA_GOOMBA: obj = new CParaGoomba(x, y, object_type); break;
	case OBJECT_TYPE_PARA_KOOPA: obj = new CParaKoopa(x, y, object_type); break;
	case OBJECT_TYPE_PIRANHA_PLANT: obj = new CPiranhaPlant(x, y, object_type); break;
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::_ParseSection_CAMERA_BOUNDARIES(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2) {
		DebugOut(L"[ERROR] Invalid line when reading camera boundaries");
		return;
	}

	leftBoundaries = atoi(tokens[0].c_str());
	bottomBoundaries = atoi(tokens[1].c_str());
	rightBoundaries = atoi(tokens[2].c_str());
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
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

void CPlayScene::Load()
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
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_CAMERA_BOUNDARIES: _ParseSection_CAMERA_BOUNDARIES(line); break;
		}
	}

	f.close();

	for (size_t i = 0; i < objects.size(); i++) {
		if (dynamic_cast<CPlant*>(objects[i])) {
			CPlant* t = (CPlant*)objects[i];
			t->SetPlayer(player);
		}
	}

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the last object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	float cam_t, cam_l, cam_r, cam_b, x, y;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cam_l, cam_t);
	cam_r = cam_l + game->GetBackBufferWidth() + CAMERA_CHECKING_OFFSET;
	cam_b = cam_t + game->GetBackBufferHeight() + CAMERA_CHECKING_OFFSET;
	
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->GetPosition(x, y);
		if (cam_l < x && x < cam_r && cam_t < y && y < cam_b) {
			objects[i]->Update(dt, &coObjects);
		}
			
	}
	player->Update(dt, &coObjects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	AdjustCameraPosition();

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	float cam_t, cam_l, cam_r, cam_b, x, y;
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cam_l, cam_t);
	cam_r = cam_l + game->GetBackBufferWidth() + CAMERA_CHECKING_OFFSET;
	cam_b = cam_t + game->GetBackBufferHeight() + CAMERA_CHECKING_OFFSET;
	// Always render decorated objects first
	for (int i = 0; i < decoratedObjects.size(); i++) {
		decoratedObjects[i]->GetPosition(x, y);
		if (cam_l < x && x < cam_r && cam_t < y && y < cam_b)
			decoratedObjects[i]->Render();
	}

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->GetPosition(x, y);
		if (dynamic_cast<CPlatform*>(objects[i])) {
			objects[i]->Render();
		}
		else if(cam_l < x && x < cam_r && cam_t < y && y < cam_b)
			objects[i]->Render();
	}
	player->Render();
}

void CPlayScene::AddObjects(LPGAMEOBJECT obj, bool isAddAtTheBeginning)
{
	if (isAddAtTheBeginning) {
		objects.insert(objects.begin(), obj);
	}
	else {
		objects.push_back(obj);
	}
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
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
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::AdjustCameraPosition()
{
	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario*)player;
	// Update camera to follow mario
	float newCamX, newCamY;
	player->GetPosition(newCamX, newCamY);

	newCamX -= game->GetBackBufferWidth() / 2;
	newCamY -= game->GetBackBufferHeight() / 2;

	if (newCamX < leftBoundaries) newCamX = leftBoundaries;
	if (newCamX > rightBoundaries) newCamX = rightBoundaries;

	if (mario->GetLevel() != MARIO_LEVEL_RACCOON) {
		if (isCameraYLocked) {
			newCamY = bottomBoundaries - SCREEN_HEIGHT;
		}
		else {
			if (newCamY > (bottomBoundaries - SCREEN_HEIGHT)) {
				newCamY = bottomBoundaries - SCREEN_HEIGHT;
				isCameraYLocked = true;
			}
		}
	}
	else {
		isCameraYLocked = false;
	}
	
	if (newCamY < 0) newCamY = 0;
	if (newCamY > (bottomBoundaries - SCREEN_HEIGHT)) newCamY = bottomBoundaries - SCREEN_HEIGHT;

	CGame::GetInstance()->SetCamPos(newCamX, newCamY);
}
