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
#include "SwitchBrick.h"
#include "Goal.h"
#include "Hud.h"
#include "GlobalState.h"
#include "PortalHiddenZone.h"

#include "SampleKeyEventHandler.h"

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2
#define SCENE_SECTION_CAMERA_BOUNDARIES 3
#define SCENE_SECTION_BACKGROUND_COLOR 4

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 204
#define SWITCH_ON_TIME 3000

#define CAMERA_CHECKING_OFFSET 32
#define HUD_POS_OFFSET_X 125
#define HUD_POS_OFFSET_Y 20

#define DEFAULT_TIME 300
#define ONE_SECOND 1000

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
	isCameraYLocked = false;
	isSwitchOn = false;
	switchTimer = gameTimer = 0;
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
	for (size_t i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
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
		obj = new CMario(x, y, object_type);
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
	case OBJECT_TYPE_PORTAL_HIDDEN_ZONE:
	{
		int spriteId = atoi(tokens[3].c_str());
		int scene_id = atoi(tokens[4].c_str());
		float dest_x = (float)atof(tokens[5].c_str());
		float dest_y = (float)atof(tokens[6].c_str());
		obj = new CPortalHiddenZone(x, y, spriteId, scene_id, dest_x, dest_y);
		break;
	}
	case OBJECT_TYPE_DECORATED:
	{
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
		else {
			DebugOut(L"[ERROR] The input is invalid\n");
			return;
		}
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
	case OBJECT_TYPE_INVINSIBLE_BRICK: {
		int length = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());
		obj = new CInvinsibleBrick(x, y, object_type, length, height); 
		break;
	}
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
	case OBJECT_TYPE_SWITCH_BRICK: obj = new CSwitchBrick(x, y, object_type); break;
	case OBJECT_TYPE_GOAL: obj = new CGoal(x, y, object_type); break;
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

	topBoundaries = (float)atof(tokens[0].c_str());
	leftBoundaries = (float)atof(tokens[1].c_str());
	bottomBoundaries = (float)atof(tokens[2].c_str());
	rightBoundaries = (float)atof(tokens[3].c_str());
}

void CPlayScene::_ParseSection_BACKGROUND_COLOR(string line)
{
	vector<string> tokens = split(line);

	// Skip invalid line
	if (tokens.size() < 3) return;

	float red = (float)atof(tokens[0].c_str());
	float blue = (float)atof(tokens[1].c_str());
	float green = (float)atof(tokens[2].c_str());

	bg_color = D3DXCOLOR(red / 255, blue / 255, green / 255, 0.0f);
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

	for (size_t i = 0; i < objects.size(); i++) {
		if (dynamic_cast<CPlant*>(objects[i])) {
			CPlant* t = (CPlant*)objects[i];
			t->SetPlayer(player);
		}
	}

	((CMario*)player)->SetBoundary(
		leftBoundaries + MARIO_BOUNDARY_OFFSET, 
		topBoundaries, 
		rightBoundaries - MARIO_BOUNDARY_OFFSET, 
		bottomBoundaries);

	// Init HUD
	hud = new CHud(0, 0, OBJECT_TYPE_HUD);

	// Setting global state
	CGlobalState * gs = CGlobalState::GetInstance();
	gs->time = DEFAULT_TIME;
	gameTimer = GetTickCount64();

	// If mario teleport from hidden zone
	if (gs->marioDestX != -1 && gs->marioDestY != -1) {
		((CMario*)player)->SetPosition(gs->marioDestX, gs->marioDestY);
		((CMario*)player)->SetLevel(gs->marioLevel);
		gs->ResetTeleportProperties();
	}

	CGame::GetInstance()->SetCamPos(0, 0);

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// Handling Timer
	if (isSwitchOn) {
		if (GetTickCount64() - switchTimer > SWITCH_ON_TIME) {
			isSwitchOn = false;
			switchTimer = 0;
		}
	}
	if (((CMario*)player)->GetState() != MARIO_STATE_DIE &&
		!((CMario*)player)->IsFlagOn(FLAG_IN_CUT_SCENE)) {
		if (GetTickCount64() - gameTimer > ONE_SECOND) {
			gameTimer = GetTickCount64();
			CGlobalState::GetInstance()->time--;
		}
	}
	if (CGlobalState::GetInstance()->time == 0) {
		player->SetState(MARIO_STATE_DIE);
	}
	
	// We know that Mario is the last object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (isInCamera(objects[i])) {
			objects[i]->Update(dt, &coObjects);
		}
			
	}
	player->Update(dt, &coObjects);
	hud->Update(dt, &coObjects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	AdjustCameraPosition();

	// Update HUD pos
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	((CHud*)hud)->SetPosition(cam_x + HUD_POS_OFFSET_X, cam_y + SCREEN_HEIGHT - HUD_POS_OFFSET_Y);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	// Always render decorated objects first
	for (size_t i = 0; i < decoratedObjects.size(); i++) {
		if (isInCamera(decoratedObjects[i])) {
			decoratedObjects[i]->Render();
		}
		
	}

	for (size_t i = 0; i < objects.size(); i++) {
		if(isInCamera(objects[i]) || dynamic_cast<CPlatform*>(objects[i]))
			objects[i]->Render();
	}
	player->Render();
	hud->Render();
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
	for (size_t i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

bool CPlayScene::isInCamera(LPGAMEOBJECT obj)
{
	CGame* game = CGame::GetInstance();
	float cam_t, cam_l, cam_r, cam_b;
	float obj_l, obj_t, obj_r, obj_b;
	game->GetCamPos(cam_l, cam_t);
	cam_r = cam_l + game->GetBackBufferWidth() + CAMERA_CHECKING_OFFSET;
	cam_b = cam_t + game->GetBackBufferHeight() + CAMERA_CHECKING_OFFSET;
	obj->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);

	if (cam_l >= obj_l &&
		cam_l <= obj_r &&
		cam_t >= obj_t &&
		cam_b <= obj_b) return true;

	if (obj_l >= cam_l &&
		obj_l <= cam_r &&
		obj_t >= cam_t &&
		obj_b <= cam_b) return true;

	if (cam_r >= obj_l &&
		cam_r <= obj_r &&
		cam_b >= obj_t &&
		cam_b <= obj_b) return true;

	if (obj_r >= cam_l &&
		obj_r <= cam_r &&
		obj_b >= cam_t &&
		obj_b <= cam_b) return true;

	if (cam_l >= obj_l &&
		cam_l <= obj_r &&
		cam_b >= obj_t &&
		cam_b <= obj_b) return true;

	if (obj_l >= cam_l &&
		obj_l <= cam_r &&
		obj_b >= cam_t &&
		obj_b <= cam_b) return true;

	if (cam_r >= obj_l &&
		cam_r <= obj_r &&
		cam_t >= obj_t &&
		cam_b <= obj_b) return true;

	if (obj_r >= cam_l &&
		obj_r <= cam_r &&
		obj_t >= cam_t &&
		obj_b <= cam_b) return true;

	return false;
}

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
	if (newCamX > rightBoundaries - SCREEN_WIDTH) newCamX = rightBoundaries - SCREEN_WIDTH;

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
	
	if (newCamY < topBoundaries) newCamY = topBoundaries;
	if (newCamY > (bottomBoundaries - SCREEN_HEIGHT)) newCamY = bottomBoundaries - SCREEN_HEIGHT;

	CGame::GetInstance()->SetCamPos(newCamX, newCamY);
}
