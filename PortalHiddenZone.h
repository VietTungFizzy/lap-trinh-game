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

public:
    CPortalHiddenZone(float x, float y, int scene_id, float dest_x, float dest_y );

};

