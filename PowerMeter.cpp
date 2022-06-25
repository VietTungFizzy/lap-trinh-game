#include "PowerMeter.h"
#include "GlobalState.h"
#include <cmath>

void CPowerMeter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - POWER_METER_BBOX_WIDTH / 2;
	top = y - POWER_METER_BBOX_HEIGHT / 2;
	right = left + POWER_METER_BBOX_WIDTH;
	bottom = top + POWER_METER_BBOX_HEIGHT;
}

void CPowerMeter::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int power = CGlobalState::GetInstance()->power;
	DebugOutTitle(L"%d", power);
	
	int full_parts = (int)floor((power / POWER_METER_VALUE_PER_ARROW));
	for (int i = 0; i < full_parts; i++) {
		animations->Get(ID_ANI_POWER_METER_ARROW_FULL_POWER)->Render(x + i * POWER_METER_ARROW_WIDTH, y);
	}
	for (int i = full_parts; i < POWER_METER_MAX_ARROW; i++) {
		animations->Get(ID_ANI_POWER_METER_ARROW_EMPTY_POWER)->Render(x + i * POWER_METER_ARROW_WIDTH, y);
	}
	
	if (full_parts == POWER_METER_MAX_ARROW) {
		animations->Get(ID_ANI_POWER_METER_BUTTON_FULL)->Render(x + POWER_METER_BUTTON_OFFSET_X, y);
	}
	else {
		animations->Get(ID_ANI_POWER_METER_BUTTON_EMPTY)->Render(x + POWER_METER_BUTTON_OFFSET_X, y);
	}
}
