#pragma once
#include "Platform.h"
class CPlatformTopOnly :
    public CPlatform
{
public:
	CPlatformTopOnly(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
		int type) :
		CPlatform(x, y, 
			cell_width, cell_height, length, 
			sprite_id_begin, sprite_id_middle, sprite_id_end, 
			type)
	{
	}

	void Render() { CPlatform::Render(); RenderBoundingBox(); }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b) { CPlatform::GetBoundingBox(l, t, r, b); }
};

