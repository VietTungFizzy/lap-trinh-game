#pragma once
#include "GameObject.h"
// For now, we assume a decorated object with multiple cell
// will have width and height 16px
#define DECORATED_OBJECT_WIDTH 16
#define DECORATED_OBJECT_HEIGHT 16

#define DECORATED_OBJECT_SINGLE_CELL_PARAMS 4
#define DECORATED_OBJECT_MULTIPLE_CELL_PARAMS 6

class CDecoratedObject : public CGameObject {
private:
	int spriteId;
	int length, height;
public:
	CDecoratedObject(
		float x, float y, 
		int spriteId, int type, 
		int length = 1, int height = 1
	) : CGameObject(x, y, type) {
		this->spriteId = spriteId;
		this->length = length;
		this->height = height;
	}



	void Update(DWORD dt) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};