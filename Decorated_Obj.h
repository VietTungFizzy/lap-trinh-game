#pragma once
#include "GameObject.h"
// For now, we assume a decorated object with multiple cell
// will have width and height 16px
#define DECORATED_OBJECT_WIDTH 16
#define DECORATED_OBJECT_HEIGHT 16

#define DECORATED_OBJECT_SINGLE_CELL_PARAMS 4
#define DECORATED_OBJECT_MULTIPLE_CELL_PARAMS 6
#define DECORATED_OBJECT_ANIMATED_PARAMS 7
#define DECORATED_OBJECT_MULTIPLE_CELL_CUSTOM_WIDTH_HEIGHT_PARAMS 9

class CDecoratedObject : public CGameObject {
private:
	int spriteId;
	int length, height;
	int cellWidth, cellHeight;
	int isAnimated;
public:
	CDecoratedObject(
		float x, float y,
		int spriteId, int type,
		int length = 1, int height = 1,
		int isAnimated = 0,
		int cellWidth = DECORATED_OBJECT_WIDTH, int cellHeight = DECORATED_OBJECT_HEIGHT
	) : CGameObject(x, y, type) {
		this->spriteId = spriteId;
		this->length = length;
		this->height = height;
		this->isAnimated = isAnimated;
		this->cellWidth = cellWidth;
		this->cellHeight = cellHeight;
	}



	void Update(DWORD dt) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};