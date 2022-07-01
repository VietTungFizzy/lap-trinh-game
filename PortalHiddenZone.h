#pragma once
#include "Portal.h"

#define PORTAL_HIDDEN_ZONE_WIDTH 16
#define PORTAL_HIDDEN_ZONE_HEIGHT 16

/*
* Object teleport back and forth between world
* with destination coordinates
*/
class CPortalHiddenZone :
    public CPortal
{
private:
    float dest_x;
    float dest_y;
    int spriteId;

public:
    CPortalHiddenZone(float x, float y, int spriteId, int scene_id, float dest_x, float dest_y);

    virtual void Render();

    int IsBlocking() { return 1; }
    void GetDestCoord(float& x, float& y) { x = dest_x; y = dest_y; }
};

