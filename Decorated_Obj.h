#pragma once
#include "GameObject.h"

class CDecoratedObject : public CGameObject {
private:
	int spriteId;
public:
	CDecoratedObject(float x, float y, int spriteId, int type) : CGameObject(x, y, type) {
		this->spriteId = spriteId;
	}
	void Update(DWORD dt) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
};