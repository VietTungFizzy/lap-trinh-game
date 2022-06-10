#pragma once
#include "WorldMapNode.h"
#define PORTAL_WORLD_MAP_STATE_NORMAL 0
#define PORTAL_WORLD_MAP_STATE_FINISHED 1

#define ID_SPRITE_PORTAL_WORLD_MAP_FINISHED 51007
class CPortalWorldMap :
    public CWorldMapNode
{
private:
    int sceneId;

public:
    CPortalWorldMap(float x, float y, int type,
        int adj_t, int adj_l, int adj_r, int adj_d,
        int spriteId, int sceneId): 
        CWorldMapNode(x, y, type, adj_t, adj_l, adj_r, adj_d, spriteId) {
        this->sceneId = sceneId;
        SetState(PORTAL_WORLD_MAP_STATE_NORMAL);
    }

    virtual void Render();
    int GetSceneId() { return sceneId; }
};

