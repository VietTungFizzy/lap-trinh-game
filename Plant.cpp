#include "Plant.h"

bool CPlant::isPlayerInRange(float maxRange, float minRange)
{
	float p_x, p_y, l, t, r, b;
	player->GetPosition(p_x, p_y);
	GetBoundingBox(l, t, r, b);
	return ((p_x > l - maxRange && p_x < l - minRange) ||
		(p_x < r + maxRange && p_x > r + minRange));
}
