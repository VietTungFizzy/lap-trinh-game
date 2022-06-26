#include "PortalHiddenZone.h"
#include "AssetIDs.h"

CPortalHiddenZone::CPortalHiddenZone(float x, float y, int scene_id, float dest_x, float dest_y) : 
	CPortal(x, y, x + PORTAL_HIDDEN_ZONE_WIDTH, y + PORTAL_HIDDEN_ZONE_HEIGHT, scene_id)
{
	this->dest_x = dest_x;
	this->dest_y = dest_y;
	type = OBJECT_TYPE_PORTAL_HIDDEN_ZONE;
}
