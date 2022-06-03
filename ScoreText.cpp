#include "ScoreText.h"
#include "Sprites.h"
#include "debug.h"
void CScoreText::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	sprites->Get(spriteId)->Draw(x, y);
}

void CScoreText::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	vy *= SCORE_TEXT_SPEED_REDUCTION_COEFFICIENT;
	if (abs(vy) < SCORE_TEXT_EPSILON) isDeleted = true;
}

void CScoreText::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SCORE_TEXT_BBOX_WIDTH / 2;
	t = y - SCORE_TEXT_BBOX_HEIGHT / 2;
	r = l + SCORE_TEXT_BBOX_WIDTH;
	b = t + SCORE_TEXT_BBOX_HEIGHT;
}
