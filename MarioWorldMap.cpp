#include "MarioWorldMap.h"
#include "AssetIDs.h"
#include "PortalWorldMap.h"

CMarioWorldMap::CMarioWorldMap(float x, float y, int type) : CGameObject(x, y, type)
{
	level = MARIO_WORLD_MAP_LEVEL_SMALL;
	currentNode = NULL;
	dest_x = dest_y = 0.0f;
}

void CMarioWorldMap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;

	for (size_t i = 0; i < coObjects->size(); i++) {
		CGameObject* obj = coObjects->at(i);
		if (dynamic_cast<CWorldMapNode*>(obj)) {
			if (CCollision::GetInstance()->DetectOverlap(this, obj)) {
				currentNode = obj;
				break;
			}
		}
	}

	if (vx != 0 || vy != 0) {
		if ((x > dest_x && vx > 0) ||
			(x < dest_x && vx < 0) ||
			(y > dest_y && vy > 0) ||
			(y < dest_y && vy < 0)) {
			x = dest_x;
			y = dest_y;
			vx = vy = dest_x = dest_y = 0;
			state = MARIO_WORLD_MAP_STATE_IDLE;
		}
	}
}

void CMarioWorldMap::Render()
{
	int aniId = ID_ANI_MARIO_WORLD_MAP_SMALL;
	switch (level) {
		case MARIO_WORLD_MAP_LEVEL_SMALL: aniId = ID_ANI_MARIO_WORLD_MAP_SMALL; break;
		case MARIO_WORLD_MAP_LEVEL_BIG: aniId = ID_ANI_MARIO_WORLD_MAP_BIG; break;
		case MARIO_WORLD_MAP_LEVEL_RACCOON: aniId = ID_ANI_MARIO_WORLD_MAP_RACCOON; break;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	
	// RenderBoundingBox();
}

void CMarioWorldMap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MARIO_WORLD_MAP_BBOX_WIDTH / 2;
	top = y - MARIO_WORLD_MAP_BBOX_HEIGHT / 2;
	right = left + MARIO_WORLD_MAP_BBOX_WIDTH;
	bottom = top + MARIO_WORLD_MAP_BBOX_HEIGHT;
}

void CMarioWorldMap::SetState(int state)
{
	if (this->state == MARIO_WORLD_MAP_STATE_MOVING_UP ||
		this->state == MARIO_WORLD_MAP_STATE_MOVING_LEFT ||
		this->state == MARIO_WORLD_MAP_STATE_MOVING_RIGHT ||
		this->state == MARIO_WORLD_MAP_STATE_MOVING_DOWN) return;

	switch (state) {
		case MARIO_WORLD_MAP_STATE_MOVING_UP: {
			if (currentNode == NULL) return;
			
			CWorldMapNode* node = dynamic_cast<CWorldMapNode*>(currentNode);
			if (node->HasAdjacentNode(WORLD_MAP_NODE_ADJACENT_NODE_UP)) {
				vy = -MARIO_WORLD_MAP_SPEED;
				dest_x = x;
				dest_y = y - WORLD_MAP_NODE_DISTANCE;
			}
			else return;
			
			break;
		}
		case MARIO_WORLD_MAP_STATE_MOVING_LEFT: {
			if (currentNode == NULL) return;

			CWorldMapNode* node = dynamic_cast<CWorldMapNode*>(currentNode);
			if (node->HasAdjacentNode(WORLD_MAP_NODE_ADJACENT_NODE_LEFT)) {
				vx = -MARIO_WORLD_MAP_SPEED;
				dest_x = x - WORLD_MAP_NODE_DISTANCE;
				dest_y = y;
			}
			else return;

			break;
		}
		case MARIO_WORLD_MAP_STATE_MOVING_RIGHT: {
			if (currentNode == NULL) return;

			CWorldMapNode* node = dynamic_cast<CWorldMapNode*>(currentNode);
			if (node->HasAdjacentNode(WORLD_MAP_NODE_ADJACENT_NODE_RIGHT)) {
				vx = MARIO_WORLD_MAP_SPEED;
				dest_x = x + WORLD_MAP_NODE_DISTANCE;
				dest_y = y;
			}
			else return;

			break;
		}
		case MARIO_WORLD_MAP_STATE_MOVING_DOWN: {
			if (currentNode == NULL) return;

			CWorldMapNode* node = dynamic_cast<CWorldMapNode*>(currentNode);
			if (node->HasAdjacentNode(WORLD_MAP_NODE_ADJACENT_NODE_DOWN)) {
				vy = MARIO_WORLD_MAP_SPEED;
				dest_x = x;
				dest_y = y + WORLD_MAP_NODE_DISTANCE;
			}
			else return;

			break;
		}
		case MARIO_WORLD_MAP_STATE_IDLE: {
			vx = vy = dest_x = dest_y = 0;
			break;
		}
	}

	CGameObject::SetState(state);
}

void CMarioWorldMap::EnterWorld()
{
	if (currentNode->GetType() == OBJECT_TYPE_PORTAL_WORLD_MAP) {
		CPortalWorldMap* portal = dynamic_cast<CPortalWorldMap*>(currentNode);
		CGame::GetInstance()->InitiateSwitchScene(portal->GetSceneId());
	}
}
