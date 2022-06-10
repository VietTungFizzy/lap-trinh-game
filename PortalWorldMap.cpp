#include "PortalWorldMap.h"

void CPortalWorldMap::Render()
{
	CSprites::GetInstance()->Get(
		(state == PORTAL_WORLD_MAP_STATE_NORMAL) ? spriteId : ID_SPRITE_PORTAL_WORLD_MAP_FINISHED
	)->Draw(x, y);
}
