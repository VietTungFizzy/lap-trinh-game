#include "PlatformTopOnly.h"
#include "debug.h"
void CPlatformTopOnly::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (player == NULL) {
		return; 
	}
	float l,t,r,b,pl, pt, pr, pb;
	player->GetBoundingBox(pl, pt, pr, pb);
	GetBoundingBox(l, t, r, b);

	if (pb < t) isBlock = 1;
	else isBlock = 0;
}
