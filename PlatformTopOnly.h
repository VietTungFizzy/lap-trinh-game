#pragma once
#include "Platform.h"
class CPlatformTopOnly :
    public CPlatform
{
private:
	int isBlock;
public:
	CPlatformTopOnly(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :
		CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end)
	{
		this->isBlock = 0;
	}

	void Render() { CPlatform::Render(); RenderBoundingBox(); }
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) { CPlatform::GetBoundingBox(l, t, r, b); }
	int IsBlocking() { return isBlock; }
	void SetBlockMode(int mode) { isBlock = mode; }
};

